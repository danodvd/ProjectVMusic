#include "NewID3Reader.hpp"
#include "bass.h"
#include <vector>
#include <iostream>
#include <cstdlib>

namespace {

    int ReadInt32(const unsigned char* ptr) {
        return (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
    }

    int ReadInt32Safe(const unsigned char* ptr) {
        return (ptr[0] << 21) | (ptr[1] << 14) | (ptr[2] << 7) | ptr[3];
    }

    std::string ExtractText(const unsigned char* data, int size) {
        if (size <= 1) return "";

        int encoding = data[0];
        std::string resultado = "";

        if (encoding == 1 && size > 3) {
            for (int i = 3; i < size; i += 2) {
                char c = data[i];
                if (c != 0) {
                    resultado += c;
                }
            }
        }
        else {
            // ISO-8859-1 or UTF-8
            resultado = std::string(reinterpret_cast<const char*>(data + 1), size - 1);
        }
        return resultado;
    }
}

SongData ID3Reader::ReadFromFile(const std::wstring& filePath) {
    SongData data;
    data.id = SongData::GenerateID();
    data.filePath = filePath;

    data.title = "Desconocido";
    data.artist = "Artista Desconocido";
    data.album = "Album Desconocido";

    HSTREAM stream = BASS_StreamCreateFile(FALSE, filePath.c_str(), 0, 0, BASS_STREAM_DECODE | BASS_UNICODE);

    if (!stream) {

        return data;
    }

    QWORD lenBytes = BASS_ChannelGetLength(stream, BASS_POS_BYTE);
    data.duration = BASS_ChannelBytes2Seconds(stream, lenBytes);

    const char* rawTags = BASS_ChannelGetTags(stream, BASS_TAG_ID3V2);

    if (!rawTags) {
        BASS_StreamFree(stream);
        return data;
    }

    const unsigned char* ptr = (const unsigned char*)rawTags;

    if (ptr[0] == 'I' && ptr[1] == 'D' && ptr[2] == '3') {
        int version = ptr[3];
        int tagSize = ReadInt32Safe(ptr + 6);
        int pos = 10;

        while (pos < tagSize) {
            if (ptr[pos] == 0) break; 

            std::string frameID(reinterpret_cast<const char*>(ptr + pos), 4);

            int frameSize = 0;
            if (version == 4) frameSize = ReadInt32Safe(ptr + pos + 4);
            else frameSize = ReadInt32(ptr + pos + 4);

            int nextFramePos = pos + 10 + frameSize;

            if (frameID == "TIT2") {
                data.title = ExtractText(ptr + pos + 10, frameSize);
            }
            else if (frameID == "TPE1") {
                data.artist = ExtractText(ptr + pos + 10, frameSize);
            }
            else if (frameID == "TALB") {
                data.album = ExtractText(ptr + pos + 10, frameSize);
            }
            else if (frameID == "TPE2") {
                data.albumArtist = ExtractText(ptr + pos + 10, frameSize);
            }
            else if (frameID == "TCON") {
                data.genre = ExtractText(ptr + pos + 10, frameSize);
            }
            else if (frameID == "TYER" || frameID == "TDRC") {
                std::string yearStr = ExtractText(ptr + pos + 10, frameSize);
                if (!yearStr.empty()) {
                    data.year = std::atoi(yearStr.c_str());
                }
            }
            else if (frameID == "TRCK") {
                std::string trackStr = ExtractText(ptr + pos + 10, frameSize);
                if (!trackStr.empty()) {
                    data.trackNumber = std::atoi(trackStr.c_str());
                }
            }
            else if (frameID == "APIC") {
                int p = pos + 10;
                int encoding = ptr[p++];

                while (ptr[p] != 0 && p < nextFramePos) p++;
                p++;
                p++; 

                while (p < nextFramePos) {
                    if (ptr[p] == 0xFF && ptr[p + 1] == 0xD8) break; 
                    if (ptr[p] == 0x89 && ptr[p + 1] == 0x50) break; 
                    p++;
                }

                int imgSize = nextFramePos - p;
                if (imgSize > 0) {
                    data.coverImage.assign(ptr + p, ptr + p + imgSize);
                }
            }

            pos = nextFramePos;
        }
    }
    BASS_StreamFree(stream);

    return data;
}