#include "win32/file.hpp"
#include "win32/win32.hpp"
#include <Windows.h>
#include <winrt/base.h>
#include "win32/abi.hpp"

namespace win32 {
	// note: volume name format \\?\Volume{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}\

	FindVolumeHandle* findFirstVolume(std::string& volume_name) {
		WCHAR buffer[MAX_PATH]{};
		HANDLE handle = FindFirstVolumeW(buffer, MAX_PATH);
		if (handle == INVALID_HANDLE_VALUE) {
			return nullptr;
		}
		volume_name.assign(to_string(buffer));
		return static_cast<FindVolumeHandle*>(handle);
	}

	bool findNextVolume(FindVolumeHandle* handle, std::string& volume_name) {
		WCHAR buffer[MAX_PATH]{};
		BOOL const result = FindNextVolumeW(static_cast<HANDLE>(handle), buffer, MAX_PATH);
		if (result) {
			volume_name.assign(to_string(buffer));
		}
		return abi::as<bool>(result);
	}

	bool findVolumeClose(FindVolumeHandle* handle) {
		return abi::as<bool>(FindVolumeClose(static_cast<HANDLE>(handle)));
	}

	bool getVolumePathNamesForVolumeName(std::string const& volume_name, std::vector<std::string>& volume_path_names) {
		auto const volume_name_wide = to_wstring(volume_name);
		DWORD buffer_size{};
		BOOL const prepare_result = GetVolumePathNamesForVolumeNameW(volume_name_wide.c_str(), nullptr, 0, &buffer_size);
		if (!prepare_result && GetLastError() != ERROR_MORE_DATA) {
			return false;
		}
		std::wstring buffer(buffer_size, L'\0');
		DWORD write_size{};
		BOOL const result = GetVolumePathNamesForVolumeNameW(volume_name_wide.c_str(), buffer.data(), buffer_size, &write_size);
		if (!result) {
			return false;
		}
		std::string const buffer_utf8(to_string(buffer));
		std::string_view volume_path_names_view(buffer_utf8);
		volume_path_names.clear();
		while (!volume_path_names_view.empty()) {
			auto const index = volume_path_names_view.find_first_of('\0');
			if (index == std::string_view::npos) {
				break;
			}
			if (auto const volume_path_name = volume_path_names_view.substr(0, index); !volume_path_name.empty()) {
				volume_path_names.emplace_back(volume_path_name);
			}
			auto const next_index = volume_path_names_view.find_first_not_of('\0', index + 1);
			if (next_index == std::string_view::npos) {
				break;
			}
			volume_path_names_view = volume_path_names_view.substr(next_index);
		}
		return true;
	}

	FindVolumeMountPointHandle* findFirstVolumeMountPoint(std::string const& volume_name, std::string& volume_mount_point) {
		auto const volume_name_wide = to_wstring(volume_name);
		WCHAR buffer[MAX_PATH]{};
		HANDLE handle = FindFirstVolumeMountPointW(volume_name_wide.c_str(), buffer, MAX_PATH);
		if (handle == INVALID_HANDLE_VALUE) {
		#ifndef NDEBUG
			try {
				winrt::throw_last_error();
			}
			catch (winrt::hresult_error const& e) {
				auto const message = winrt::to_string(e.message());
				std::printf("[winrt::hresult_error] %s\n", message.c_str());
			}
		#endif
			return nullptr;
		}
		volume_mount_point.assign(to_string(buffer));
		return static_cast<FindVolumeMountPointHandle*>(handle);
	}

	bool findNextVolumeMountPoint(FindVolumeMountPointHandle* handle, std::string& volume_mount_point) {
		WCHAR buffer[MAX_PATH]{};
		auto const result = FindNextVolumeMountPointW(static_cast<HANDLE>(handle), buffer, MAX_PATH);
		if (!result) {
			return false;
		}
		volume_mount_point.assign(to_string(buffer));
		return true;
	}

	bool findVolumeMountPointClose(FindVolumeMountPointHandle* handle) {
		return abi::as<bool>(FindVolumeMountPointClose(static_cast<HANDLE>(handle)));
	}

	uint32_t getLogicalDrives() {
		return GetLogicalDrives();
	}

	bool getLogicalDriveStrings(std::vector<std::string>& drives) {
		auto const buffer_size = GetLogicalDriveStringsW(0, nullptr);
		if (buffer_size == 0) {
			return false;
		}
		std::wstring buffer(buffer_size, L'\0');
		auto const result = GetLogicalDriveStringsW(buffer_size, buffer.data());
		if (result == 0 || result > buffer_size) {
			return false;
		}
		std::string const buffer_utf8(to_string(buffer));
		std::string_view drives_view(buffer_utf8);
		drives.clear();
		while (!drives_view.empty()) {
			auto const index = drives_view.find_first_of('\0');
			if (index == std::string_view::npos) {
				break;
			}
			if (auto const drive = drives_view.substr(0, index); !drive.empty()) {
				drives.emplace_back(drive);
			}
			auto const next_index = drives_view.find_first_not_of('\0', index + 1);
			if (next_index == std::string_view::npos) {
				break;
			}
			drives_view = drives_view.substr(next_index);
		}
		return true;
	}

	DriveType getDriveType(std::string const& root_path_name) {
		auto const root_path_name_wide = to_wstring(root_path_name);
		return static_cast<DriveType>(GetDriveTypeW(root_path_name_wide.c_str()));
	}
}
