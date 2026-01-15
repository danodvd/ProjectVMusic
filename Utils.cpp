#include "Utils.hpp"
#include <sstream>
#include <iomanip>

namespace Utils {
	std::string Utils::FormatDuration(double seconds) {
		int min = (int)seconds / 60;
		int sec = (int)seconds % 60;
		std::stringstream ss;
		ss << min << ":" << std::setfill('0') << std::setw(2) << sec;
		return ss.str();
	}
}