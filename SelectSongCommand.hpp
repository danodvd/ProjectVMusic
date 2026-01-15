#pragma once
#include "Command.hpp"
#include "SongData.hpp"
#include "SelectSongService.hpp"
#include "NewAudioManager.hpp"
#include <iostream>
#include <windows.h> 

class SelectSongCommand : public Command {
public:
SelectSongCommand(const SongData& song, AudioManager& audioRef) 
        : targetID(song.id), 
          sourceData(song),
          audio(audioRef) {}

    void Execute() override {
        SelectSongService::PlaySong(targetID, audio, &sourceData);
    }

private:
    SongID targetID;
    SongData sourceData;
    AudioManager& audio;
};