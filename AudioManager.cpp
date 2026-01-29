#include "NewAudioManager.hpp"

#pragma comment(lib, "bass.lib")
#include <iostream>

#include <algorithm> // Para std::clamp (C++17)
#include "SongData.hpp"

AudioManager::AudioManager() : stream(0), volume(1.0f) {
    eqBands = {
        { EQType::LowShelf,   80.f, 0.7f, 0.f },
        { EQType::Peaking,   250.f, 1.0f, 0.f }, 
        { EQType::Peaking,  1000.f, 1.0f, 0.f }, 
        { EQType::Peaking,  4000.f, 1.0f, 0.f },
        { EQType::HighShelf,8000.f, 0.7f, 0.f } 
    };

    for (auto& band : eqBands) {
        band.filters.resize(2);
        band.UpdateFilter(sampleRate);
    }
}

AudioManager::~AudioManager() {
    Free();
}

bool AudioManager::Init() {
    return BASS_Init(-1, 44100, 0, 0, NULL);
}

void AudioManager::Free() {
    Stop();
    if (stream) {
        BASS_StreamFree(stream);
        stream = 0;
    }
    BASS_Free();
}

void AudioManager::AddSong(const SongData& song, bool play) {
    playlist.push_back(song);

    if (play || !currentIndex.has_value()) {
        LoadByIndex(playlist.size() - 1, play);
    }
}

void AudioManager::LoadByIndex(size_t index, bool play) {
    if (playlist.empty()) return;

    size_t safeIndex = index;
    if (safeIndex >= playlist.size()) {
        safeIndex = 0;
    }

    currentIndex = safeIndex;
    LoadInternal(*currentIndex, play);
}

void AudioManager::ClearQueue() {
    Stop();
    if (stream) {
        BASS_StreamFree(stream);
        stream = 0;
    }
    playlist.clear();
    currentIndex.reset();
}

void AudioManager::PlayByID(SongID targetID) {
    for (size_t i = 0; i < playlist.size(); ++i) {
        if (playlist[i].id == targetID) {
            LoadByIndex(i, true);
            return;
        }
    }
    std::cout << "Canción con ID " << targetID << " no encontrada en la cola activa." << std::endl;
}

bool AudioManager::ContainsSong(SongID ID) const{
    for (size_t i = 0; i < playlist.size(); ++i) {
        if (playlist[i].id == ID) return true;
    }
    return false;
}

void AudioManager::TogglePause() {
    if (!stream) return;

    if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PLAYING)
        BASS_ChannelPause(stream);
    else
        BASS_ChannelPlay(stream, FALSE);
}

void AudioManager::Stop() {
    if (stream) {
        BASS_ChannelStop(stream);
        BASS_ChannelSetPosition(stream, 0, BASS_POS_BYTE);
    }
}

void AudioManager::SetVolume(float newVolume) {
    volume = std::clamp(newVolume, 0.0f, 1.0f);

    if (stream) {
        BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, volume);
    }
}

void AudioManager::Seek(double percentage) {
    if (!stream) return;

    QWORD len = BASS_ChannelGetLength(stream, BASS_POS_BYTE);
    QWORD newPos = static_cast<QWORD>(len * percentage);

    BASS_ChannelSetPosition(stream, newPos, BASS_POS_BYTE);
}

void AudioManager::Next() {
    if (playlist.empty()) return;

    int nextIndex = 0;
    if (currentIndex.has_value()) {
        nextIndex = *currentIndex + 1;
        if (nextIndex >= playlist.size()) nextIndex = 0;
    }

    LoadByIndex(nextIndex, true);
}

void AudioManager::Previous() {
    if (playlist.empty()) return;

    int prevIndex = 0;
    if (currentIndex.has_value()) {
        prevIndex = *currentIndex - 1;
        if (prevIndex < 0) prevIndex = playlist.size() - 1;
    }

    LoadByIndex(prevIndex, true);
}

void AudioManager::UpdateAutoPlay() {
    if (stream && currentIndex.has_value() && !playlist.empty()) {
        if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_STOPPED) {
            if (BASS_ChannelGetPosition(stream, BASS_POS_BYTE) > 0) {
                LoadByIndex(*currentIndex + 1, true);
            }
        }
    }
}

const SongData* AudioManager::GetCurrentSong() const {
    if (currentIndex.has_value() && *currentIndex < playlist.size()) {
        return &playlist[*currentIndex];
    }
    return nullptr;
}

double AudioManager::GetPositionSecs() const {
    if (!stream) return 0.0;
    QWORD pos = BASS_ChannelGetPosition(stream, BASS_POS_BYTE);
    return BASS_ChannelBytes2Seconds(stream, pos);
}

double AudioManager::GetLengthSecs() const {
    if (!stream) return 0.0;
    QWORD len = BASS_ChannelGetLength(stream, BASS_POS_BYTE);
    return BASS_ChannelBytes2Seconds(stream, len);
}

float AudioManager::GetVolume() const {
    return volume;
}

bool AudioManager::IsPlaying() const {
    if (!stream) return false;
    return BASS_ChannelIsActive(stream) == BASS_ACTIVE_PLAYING;
}

bool AudioManager::HasStopped() const {
    return BASS_ChannelIsActive(stream) == BASS_ACTIVE_STOPPED;
}

void AudioManager::GetFFT(float* fftData) const {
    if (stream && IsPlaying()) {
        BASS_ChannelGetData(stream, fftData, BASS_DATA_FFT2048);
    }
    else {
        std::fill_n(fftData, 1024, 0.0f);
    }
}

void AudioManager::SetEQ(int bandIndex, float gain) {
    if (bandIndex < 0 || bandIndex >= eqBands.size()) return;

    if (gain > 15.0f) gain = 15.0f;
    if (gain < -15.0f) gain = -15.0f;

    if (eqBands[bandIndex].gainDB != gain) {
        eqBands[bandIndex].gainDB = gain;

        eqBands[bandIndex].UpdateFilter(sampleRate);
    }
}

void AudioManager::UpdateEQCoefficients() {
    for (auto& band : eqBands) {
        band.UpdateFilter(sampleRate);
    }
}

float AudioManager::GetEQ(int bandIndex) const {
    if (bandIndex >= 0 && bandIndex < 3) return currentEqGains[bandIndex];
    return 0.f;
}

void AudioManager::LoadInternal(size_t index, bool play) {
    if (index >= playlist.size()) return;

    if (stream) {
        BASS_StreamFree(stream);
        stream = 0;
    }

    const auto& song = playlist[index];
    stream = BASS_StreamCreateFile(FALSE, song.filePath.c_str(), 0, 0, BASS_SAMPLE_FLOAT);

    if (stream) {
        BASS_CHANNELINFO info;
        BASS_ChannelGetInfo(stream, &info);

        for (auto& band : eqBands) {
            if (band.filters.size() != info.chans) {
                band.filters.clear();
                band.filters.resize(info.chans);
            }
            for (auto& f : band.filters) f.Reset();
        }

        UpdateEQCoefficients();

        dspHandle = BASS_ChannelSetDSP(stream, &EqualizerDSP, this, 1000);

        BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, volume);

        if (play) {
            BASS_ChannelPlay(stream, FALSE);
        }
    }
}

void CALLBACK AudioManager::EqualizerDSP(HDSP handle, DWORD channel, void* buffer, DWORD length, void* user) {
    AudioManager* self = (AudioManager*)user;
    float* data = (float*)buffer;

    DWORD numSamples = length / sizeof(float);

    BASS_CHANNELINFO info;
    BASS_ChannelGetInfo(channel, &info);
    DWORD channels = info.chans;

    for (DWORD i = 0; i < numSamples; i += channels) {
        for (DWORD ch = 0; ch < channels; ch++) {
            float sample = data[i + ch];

            for (auto& band : self->eqBands) {
                if (ch < band.filters.size()) {
                    sample = band.filters[ch].Process(sample);
                }
            }

            data[i + ch] = sample;
        }
    }
}