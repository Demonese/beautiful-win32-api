#include "shared.hpp"
#include "win32/win32.hpp"

namespace win32 {
	static_assert(CHAR_BIT == 8, "unknown platform");
	static_assert(sizeof(char) == 1, "unknown platform");
	static_assert(sizeof(char) == sizeof(char8_t), "unknown platform");
	
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

	std::u8string to_u8string(std::wstring_view const& str) {
		std::u8string buffer;
		if (str.empty()) {
			return buffer;
		}
		int const count = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.length()), nullptr, 0, nullptr, nullptr);
		if (count <= 0) {
			return buffer;
		}
		buffer.resize(static_cast<int>(count));
		char* ptr = reinterpret_cast<char*>(const_cast<char8_t*>(buffer.data())); // FUCK C++ Standards Committee
		int const result = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.length()), ptr, count, nullptr, nullptr);
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

	std::wstring to_wstring(std::u8string_view const& str) {
		std::wstring buffer;
		if (str.empty()) {
			return buffer;
		}
		char const* ptr = reinterpret_cast<char*>(const_cast<char8_t*>(str.data())); // FUCK C++ Standards Committee
		int const count = MultiByteToWideChar(CP_UTF8, 0, ptr, static_cast<int>(str.length()), nullptr, 0);
		if (count <= 0) {
			return buffer;
		}
		buffer.resize(static_cast<int>(count));
		int const result = MultiByteToWideChar(CP_UTF8, 0, ptr, static_cast<int>(str.length()), buffer.data(), count);
		if (count != result) {
			buffer.clear();
		}
		return buffer;
	}
}
