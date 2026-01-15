#include "NewAudioManager.hpp"
#include "SongData.hpp"

namespace SelectSongService {
    void PlaySong(SongID songId, AudioManager& audioManager, const SongData* songDataIfExternal = nullptr);
}