#pragma once
#include <string>
#include <vector>

using SongID = unsigned long long;

/**
* Represents the immutable metadata of a song.
*
* This structure contains no rendering or UI-specific data and can be
* used independently of any graphical framework.
*/
struct SongData {
    SongID id;
    std::wstring filePath;

    std::string title;
    std::string artist;
    std::string album;
    std::string albumArtist;

    int year = 0;
    int trackNumber = 0;
    std::string genre;

    double duration = 0.0;

    std::vector<unsigned char> coverImage;

    static SongID GenerateID() {
        static SongID counter = 0;
        return ++counter;
    }
};