#include "SelectSongService.hpp"

namespace SelectSongService {
	void PlaySong(SongID songId, AudioManager& audioManager, const SongData* songDataIfExternal) {
        if (audioManager.ContainsSong(songId)) {
            audioManager.PlayByID(songId);
        }
        else if (songDataIfExternal != nullptr) {
            audioManager.ClearQueue();
            audioManager.AddSong(*songDataIfExternal, true); 
        }
    }
}