#include "shared.hpp"
#include "win32/win32.hpp"

namespace win32 {
	std::string to_string(std::wstring_view const& str) {
		std::string buffer;
		if (str.empty()) {
			return buffer;
		}
		int const count = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.length()), nullptr, 0, nullptr, nullptr);
		if (count <= 0) {
			return buffer;
		}
		buffer.resize(static_cast<int>(count));
		int const result = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.length()), buffer.data(), count, nullptr, nullptr);
		if (count != result) {
			buffer.clear();
		}
		return buffer;
	}

	std::wstring to_wstring(std::string_view const& str) {
		std::wstring buffer;
		if (str.empty()) {
			return buffer;
		}
		int const count = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.length()), nullptr, 0);
		if (count <= 0) {
			return buffer;
		}
		buffer.resize(static_cast<int>(count));
		int const result = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.length()), buffer.data(), count);
		if (count != result) {
			buffer.clear();
		}
		return buffer;
	}
}
