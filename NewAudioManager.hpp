#pragma once
#include <vector>
#include <string>
#include <optional>
#include "bass.h"
#include "SongData.hpp"
#include "BiquadFilter.hpp"
#include <array>

struct SongData; // Forward declaration

enum class EQType {
    Peaking,
    LowShelf,
    HighShelf
};

struct EQBand {
    EQType type;
    float freq;
    float q;
    float gainDB;

    std::vector<BiquadFilter> filters;

    void UpdateFilter(float sampleRate) {
        float safeSlope = (q <= 0.001f) ? 0.001f : q;

        for (auto& f : filters) {
            switch (type) {
            case EQType::LowShelf:
                f.SetLowShelf(sampleRate, freq, gainDB, safeSlope);
                break;

            case EQType::Peaking:
                f.SetPeakingEQ(sampleRate, freq, q, gainDB);
                break;

            case EQType::HighShelf:
                f.SetHighShelf(sampleRate, freq, gainDB, safeSlope);
                break;
            }
        }
    }
};
/**
* The system's audio manager.
* 
* An AudioManager contains the necessary logic for playing and managing audio or playlists.
*/
class AudioManager {
public:

    AudioManager();
    ~AudioManager();

    bool Init();
    void Free();

    // Queue management
    void AddSong(const SongData& song, bool play = true);
    void LoadByIndex(size_t index, bool play = true);
    void ClearQueue();
    void PlayByID(SongID targetID);

    bool ContainsSong(SongID id) const;

    // Playback
    void TogglePause();
    void Stop();
    void SetVolume(float volume);
    void Seek(double percentage);
    void Next();
    void Previous();

    void UpdateAutoPlay(); 

    // Queries
    const SongData* GetCurrentSong() const;
    double GetPositionSecs() const;
    double GetLengthSecs() const;
    float GetVolume() const;
    bool IsPlaying() const;
    bool HasStopped() const;
    void GetFFT(float* fftData) const;
    const std::vector<SongData>& GetQueue() const { return playlist; }

    void SetEQ(int bandIndex, float gain); 
    void UpdateEQCoefficients();
    float GetEQ(int bandIndex) const;

private:
    HSTREAM stream = 0;
    HDSP dspHandle = 0;
    std::vector<SongData> playlist;
    std::optional<size_t> currentIndex;
    float sampleRate = 44100.0f;
    float volume = 1.f;

    void LoadInternal(size_t index, bool play);

    std::vector<HFX> eqFX;
    std::vector<EQBand> eqBands;

    std::array<float, 3> currentEqGains = { 0.0f, 0.0f, 0.0f };

    static void CALLBACK EqualizerDSP(HDSP handle, DWORD channel, void* buffer, DWORD length, void* user);
};