#pragma once
#include <string>
#include <vector>
#include <filesystem>

struct HWND__;
typedef struct HWND__* HWND;

namespace FileUtils {
    std::vector<std::wstring> GetPlaylist(const std::wstring& referenceFile);

    std::wstring GetFileName(const std::wstring& fullPath);

    bool IsAudio(const std::filesystem::path& path);
}