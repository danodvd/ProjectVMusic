#include "SongImportService.hpp"
#include "PlatformUtils.hpp"

namespace SongImportService {
	std::vector<SongData> ImportFromDialog(HWND ownerHandle) {
			std::vector<SongData> importedSongs;

			std::vector<std::wstring> paths = PlatformUtils::OpenAudioFiles(ownerHandle);

			if (paths.empty()) return importedSongs;

			importedSongs.reserve(paths.size());

			for (const auto& path : paths) {
				SongData data = ID3Reader::ReadFromFile(path);
				if (!data.filePath.empty()) importedSongs.push_back(std::move(data));
			}
			return importedSongs;
	}
}