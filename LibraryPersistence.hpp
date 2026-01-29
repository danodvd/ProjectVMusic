#pragma once
#include <vector>
#include <string>
#include "SongData.hpp"

namespace LibraryPersistence {
	void SaveLibrary(const std::vector<SongData>& songs, const std::string filename);

	std::vector<SongData> LoadLibraryData(const std::string& filename);
}