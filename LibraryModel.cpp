#include "LibraryModel.hpp"
#include <cctype> // Para tolower

void LibraryModel::SetLibraryData(const std::vector<SongData>& rawData) {
    songPtrs.clear();
    songPtrs.reserve(rawData.size());
    for (const auto& song : rawData) {
        songPtrs.push_back(&song);
    }
}

bool LibraryModel::CaseInsensitiveCompare(const std::string& a, const std::string& b) {
    // Tu lógica de comparación existente
    size_t len = std::min(a.length(), b.length());
    for (size_t i = 0; i < len; i++) {
        char c1 = std::tolower(a[i]);
        char c2 = std::tolower(b[i]);
        if (c1 < c2) return true;
        if (c1 > c2) return false;
    }
    return a.length() < b.length();
}

std::vector<const SongData*> LibraryModel::GetSongsSortedByTitle() const {
    std::vector<const SongData*> sorted = songPtrs;
    std::sort(sorted.begin(), sorted.end(), [](const SongData* a, const SongData* b) {
        // Validación de strings vacíos para evitar crashes
        std::string tA = a->title.empty() ? "ZZZ" : a->title;
        std::string tB = b->title.empty() ? "ZZZ" : b->title;
        return CaseInsensitiveCompare(tA, tB);
        });
    return sorted;
}

std::vector<AlbumData> LibraryModel::GetAlbumsSorted() const {
    std::map<std::string, AlbumData> albumMap;

    // Agrupación
    for (const auto* song : songPtrs) {
        std::string albumName = song->album.empty() ? "Sin Álbum" : song->album;
        AlbumData& currentAlbum = albumMap[albumName];

        if (currentAlbum.name.empty()) {
            currentAlbum.name = albumName;
        }
        currentAlbum.songs.push_back(song);
    }

    std::vector<AlbumData> result;
    result.reserve(albumMap.size());
    for (auto& pair : albumMap) {
        result.push_back(pair.second);
    }

    // Ordenar álbumes alfabéticamente
    std::sort(result.begin(), result.end(), [](const AlbumData& a, const AlbumData& b) {
        std::string nameA(a.name.begin(), a.name.end());
        std::string nameB(b.name.begin(), b.name.end());
        return CaseInsensitiveCompare(nameA, nameB);
        });

    return result;
}

std::vector<ArtistData> LibraryModel::GetArtistsSorted() const {
    std::map<std::string, ArtistData> artistMap;

    // Agrupación
    for (const auto* song : songPtrs) {
        std::string artistName = song->artist.empty() ? "Desconocido" : song->artist;
        ArtistData& currentArtist = artistMap[artistName];

        if (currentArtist.name.empty()) { // Conversión de string a wstring
            currentArtist.name = artistName;
        }
        currentArtist.songs.push_back(song);
    }

    std::vector<ArtistData> result;
    for (auto& pair : artistMap) result.push_back(pair.second);

    // Ordenar
    std::sort(result.begin(), result.end(), [](const ArtistData& a, const ArtistData& b) {
        std::string nameA(a.name.begin(), a.name.end());
        std::string nameB(b.name.begin(), b.name.end());
        return CaseInsensitiveCompare(nameA, nameB);
        });

    return result;
}

std::vector<const SongData*> LibraryModel::GetSongsFromAlbum(const AlbumData* album) {
    std::vector<const SongData*> result;
    for (const auto* song : songPtrs) {
        if (song->album == album->name) {
            result.push_back(song);
        }
    }
    return result;
}

std::vector<const SongData*> LibraryModel::GetSongsFromArtist(const ArtistData* artist) {
    std::vector<const SongData*> result;
    for (const auto* song : songPtrs) {
        if(song->artist == artist->name) result.push_back(song);
    }
    return result;
}