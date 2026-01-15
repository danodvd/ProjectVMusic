#include "PlatformUtils.hpp"
#include <commdlg.h>
#include <filesystem> 

namespace PlatformUtils {
    bool OpenFileDialog(HWND owner, wchar_t* buffer, DWORD bufferSize, const wchar_t* filter, DWORD flags) {
        OPENFILENAMEW ofn = { 0 };
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = owner;
        ofn.lpstrFile = buffer;
        ofn.nMaxFile = bufferSize;
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | flags;

        return GetOpenFileNameW(&ofn) == TRUE;
    }

    std::wstring SelectBackgroundImg(HWND owner) {
        wchar_t szFile[MAX_PATH] = { 0 };

        if (OpenFileDialog(owner, szFile, MAX_PATH,
            L"Image Files\0*.png;*.jpg;*.jpeg;*.bmp\0All\0*.*\0",
            0)) {
            return std::wstring(szFile);
        }
        return L"";
    }

    std::vector<std::wstring> OpenAudioFiles(HWND owner) {
        std::vector<std::wstring> selected;
        std::vector<wchar_t> buffer(65536, 0);

        if (OpenFileDialog(owner, buffer.data(), (DWORD)buffer.size(),
            L"Audio Files\0*.mp3;*.wav;*.ogg;*.flac\0All\0*.*\0",
            OFN_ALLOWMULTISELECT | OFN_EXPLORER)) {

            wchar_t* p = buffer.data();
            std::wstring directory = p;
            p += directory.length() + 1;

            if (*p == 0) {
                selected.push_back(directory);
            }
            else {
                while (*p) {
                    std::wstring file = p;
                    std::filesystem::path fullPath = std::filesystem::path(directory) / file;
                    selected.push_back(fullPath.wstring());
                    p += file.length() + 1;
                }
            }
        }
        return selected;
    }
}