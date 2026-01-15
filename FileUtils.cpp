#include "FileUtils.hpp"
#include <algorithm>
#include <cwctype> 

namespace fs = std::filesystem;

namespace FileUtils {

    bool IsAudio(const fs::path& path) {
        if (!path.has_extension()) return false;

        std::wstring ext = path.extension().wstring();
        std::transform(ext.begin(), ext.end(), ext.begin(),
            [](wchar_t c) { return std::towlower(c); });

        return (ext == L".mp3" || ext == L".wav" || ext == L".ogg" || ext == L".flac");
    }

    std::wstring GetFileName(const std::wstring& fullPath) {
        if (fullPath.empty()) return L"Desconocido";
        return fs::path(fullPath).filename().wstring();
    }

    std::vector<std::wstring> GetPlaylist(const std::wstring& referenceFile) {
        std::vector<std::wstring> list;
        fs::path p(referenceFile);

        if (referenceFile.empty() || !fs::exists(p)) {
            return list;
        }

        try {
            fs::path folder = p.parent_path();
            if (fs::exists(folder) && fs::is_directory(folder)) {

                std::error_code ec;
                auto dirIter = fs::directory_iterator(folder, fs::directory_options::skip_permission_denied, ec);

                if (!ec) {
                    for (const auto& entry : dirIter) {
                        if (entry.is_regular_file() && IsAudio(entry.path())) {
                            list.push_back(entry.path().wstring());
                        }
                    }
                }
            }
        }
        catch (const fs::filesystem_error&) {

        }

        if (list.empty()) {
            list.push_back(referenceFile);
        }

        return list;
    }
}