#pragma once
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "SongData.hpp"
#include "Album.hpp"   // Para AlbumData
#include "Artist.hpp"  // Para ArtistData

class LibraryModel {
public:
    // Actualiza la referencia a los datos crudos
    void SetLibraryData(const std::vector<SongData>& rawData);

    // Métodos de obtención de datos ya procesados
    std::vector<const SongData*> GetSongsSortedByTitle() const;
    std::vector<AlbumData> GetAlbumsSorted() const;
    std::vector<ArtistData> GetArtistsSorted() const;
    std::vector<const SongData*> GetSongsFromAlbum(const AlbumData* album);
    std::vector<const SongData*> GetSongsFromArtist(const ArtistData* artist);
private:
    // Punteros seguros a los datos originales (para no copiar SongData pesados)
    std::vector<const SongData*> songPtrs;

    // Helpers internos
    static bool CaseInsensitiveCompare(const std::string& a, const std::string& b);
};