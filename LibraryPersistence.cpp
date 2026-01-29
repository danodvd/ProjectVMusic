#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "LibraryPersistence.hpp"
#include "NewID3Reader.hpp"
#include <fstream>
#include <filesystem>
#include <codecvt>
#include <locale>
#include <iostream>

namespace LibraryPersistence {
	void SaveLibrary(const std::vector<SongData>& songs, const std::string filename) {
		std::wofstream file(filename);

		file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

		if (file.is_open()) {
			for (const auto& song : songs) {
				file << song.filePath << L"\n";
			}
			file.close();
		}
	}

	std::vector<SongData> LoadLibraryData(const std::string& filename) {
		std::vector<SongData> loadedSongs;
		std::wifstream file(filename);

		file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
		
        if (file.is_open()) {
            std::wstring path;
            int count = 0;

            while (std::getline(file, path)) {
                if (!path.empty() && path.back() == L'\r') {
                    path.pop_back();
                }

                if (!path.empty() && std::filesystem::exists(path)) {
                    SongData song = ID3Reader::ReadFromFile(path);

                    if (!song.filePath.empty()) {
                        loadedSongs.push_back(std::move(song));
                        count++;
                    }
                }
                else {
                    std::wcout << L"Error cargando ruta: " << path << std::endl;
                }
            }
            std::cout << "Se cargaron " << count << " canciones desde el archivo." << std::endl;
            file.close();
        }
        return loadedSongs;
    }
}