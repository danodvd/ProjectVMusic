#pragma once
#include <string>
#include "SongData.hpp"

class ID3Reader {
public:
	static SongData ReadFromFile(const std::wstring& filePath);
};