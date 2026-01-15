#pragma once
#define NOMINMAX
#include "Command.hpp"
#include <iostream>
#include <windows.h> 
#include "NewAudioManager.hpp"
#include "SongImportService.hpp"
#include "NewID3Reader.hpp"

class ImportSongCommand : public Command {
public:
    ImportSongCommand(AudioManager& audioRef, HWND ownerHandle)
        : audio(audioRef), owner(ownerHandle) {
    }

    void Execute() override {
        // 1. Obtenemos los datos del servicio (esto ya funcionaba)
        std::vector<SongData> songs = SongImportService::ImportFromDialog(owner);

        // 2. IMPORTANTE: Entregamos los datos al AudioManager
        if (!songs.empty()) {
            for (const auto& song : songs) {
                // El segundo parámetro 'true' indica que reproduzca si es la primera
                audio.AddSong(song, true);
            }
        }
    }

private:
    AudioManager& audio; // Referencia al manager
    HWND owner;
};