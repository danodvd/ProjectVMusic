#pragma once
#include <string>

struct PlaybackInfo {
    std::wstring title;
    std::wstring artist;
    std::wstring album;
    std::wstring timeText;
    double pos;
    double len;
    float volume = 1.f;
    bool isPlaying = false;
    bool hasStopped = true;
};