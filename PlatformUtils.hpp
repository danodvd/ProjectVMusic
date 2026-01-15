#define NOMINMAX
#include <string>
#include <vector>
#include <Windows.h>

namespace PlatformUtils {
	std::wstring SelectBackgroundImg(HWND owner);

	std::vector<std::wstring> OpenAudioFiles(HWND owner);
}