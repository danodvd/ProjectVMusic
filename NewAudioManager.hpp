#pragma once
#include <vector>
#include <string>
#include <optional>
#include "bass.h"
#include "SongData.hpp"

struct SongData; // Forward declaration

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

private:
    HSTREAM stream = 0;
    std::vector<SongData> playlist;
    std::optional<size_t> currentIndex;
    float volume = 1.f;

    void LoadInternal(size_t index, bool play);
};