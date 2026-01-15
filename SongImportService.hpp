#include "SFML/Graphics/RenderWindow.hpp"
#include "NewAudioManager.hpp"
#include "SongData.hpp"
#include "NewID3Reader.hpp"

namespace SongImportService {
	std::vector<SongData> ImportFromDialog(HWND ownerHandle);
}