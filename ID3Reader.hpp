#pragma once
#include "bass.h"
#include <string>
#include <vector>
#include <map>

struct ID3Tags {
	std::string titulo;
	std::string artista;
	std::string albumArtist;
	std::string track;
	std::string genre;
	std::string year;
	std::string comments;
	std::string lyrics;
	std::map<std::string, std::string> customTexts; // TXXX
	std::map<std::string, std::string> customURLs;
	std::string playCount;
	std::string encoder;
	std::string rating;
	std::string album;
	std::vector<unsigned char> imgData;
	bool tieneImagen = false;
};

class ID3Reader {
public: 
	static ID3Tags LeerTags(HSTREAM stream);

private: 
	static int LeerInt32Safe(const unsigned  char* ptr);
	static int LeerInt32(const unsigned  char* ptr);
	static int LeerInt24(const unsigned  char* ptr);

};