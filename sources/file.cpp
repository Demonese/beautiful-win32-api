#include "win32/file.hpp"
#include "win32/win32.hpp"
#include <cassert>
#include <Windows.h>
#include <winrt/base.h>
#include "win32/abi.hpp"

namespace win32 {
	// Note: volume names are in format \\?\Volume{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}\
	// See: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumenameforvolumemountpointw
	//      "A reasonable size for the buffer to accommodate the largest possible volume GUID path is 50 characters."

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

	bool getVolumeNameForVolumeMountPoint(std::string const& volume_mount_point, std::string& volume_name) {
		auto const volume_mount_point_wide = to_wstring(volume_mount_point);
		WCHAR buffer[MAX_PATH + 1]{};
		if (!GetVolumeNameForVolumeMountPointW(volume_mount_point_wide.c_str(), buffer, MAX_PATH + 1)) {
			return false;
		}
		volume_name.assign(to_string(buffer));
		return true;
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

	static void testFileSystemFlags() {
	#ifndef NDEBUG
	#define TEST(flag_value, field) { VolumeInformation::FileSystemFlags flags{}; flags = flag_value; assert(flags.field); assert(uint32_t(flags) == flag_value); }
		TEST(FILE_CASE_SENSITIVE_SEARCH, case_sensitive_search);
		TEST(FILE_CASE_PRESERVED_NAMES, case_preserved_names);
		TEST(FILE_UNICODE_ON_DISK, unicode_on_disk);
		TEST(FILE_PERSISTENT_ACLS, persistent_acls);
		TEST(FILE_FILE_COMPRESSION, file_compression);
		TEST(FILE_VOLUME_QUOTAS, volume_quotas);
		TEST(FILE_SUPPORTS_SPARSE_FILES, supports_sparse_files);
		TEST(FILE_SUPPORTS_REPARSE_POINTS, supports_reparse_points);
		TEST(FILE_SUPPORTS_REMOTE_STORAGE, supports_remote_storage);
		TEST(FILE_RETURNS_CLEANUP_RESULT_INFO, returns_cleanup_result_info);
		TEST(FILE_SUPPORTS_POSIX_UNLINK_RENAME, supports_posix_unlink_rename);
		TEST(FILE_SUPPORTS_BYPASS_IO, supports_bypass_io);
		TEST(FILE_SUPPORTS_STREAM_SNAPSHOTS, supports_stream_snapshots);
		TEST(FILE_SUPPORTS_CASE_SENSITIVE_DIRS, supports_case_sensitive_dirs);
		TEST(FILE_VOLUME_IS_COMPRESSED, volume_is_compressed);
		TEST(FILE_SUPPORTS_OBJECT_IDS, supports_object_ids);
		TEST(FILE_SUPPORTS_ENCRYPTION, supports_encryption);
		TEST(FILE_NAMED_STREAMS, named_streams);
		TEST(FILE_READ_ONLY_VOLUME, read_only_volume);
		TEST(FILE_SEQUENTIAL_WRITE_ONCE, sequential_write_once);
		TEST(FILE_SUPPORTS_TRANSACTIONS, supports_transactions);
		TEST(FILE_SUPPORTS_HARD_LINKS, supports_hard_links);
		TEST(FILE_SUPPORTS_EXTENDED_ATTRIBUTES, supports_extended_attributes);
		TEST(FILE_SUPPORTS_OPEN_BY_FILE_ID, supports_open_by_file_id);
		TEST(FILE_SUPPORTS_USN_JOURNAL, supports_usn_journal);
		TEST(FILE_SUPPORTS_INTEGRITY_STREAMS, supports_integrity_streams);
		TEST(FILE_SUPPORTS_BLOCK_REFCOUNTING, supports_block_refcounting);
		TEST(FILE_SUPPORTS_SPARSE_VDL, supports_sparse_vdl);
		TEST(FILE_DAX_VOLUME, dax_volume);
		TEST(FILE_SUPPORTS_GHOSTING, supports_ghosting);
	#undef TEST
	#else
		std::ignore = nullptr;
	#endif
	}

	bool getVolumeInformation(std::string const& root_path_name, VolumeInformation& info) {
		testFileSystemFlags();
		auto const root_path_name_wide = to_wstring(root_path_name);
		WCHAR volume_name_buffer[MAX_PATH + 1]{};
		DWORD volume_serial_number{};
		DWORD maximum_component_length{};
		DWORD file_system_flags{};
		WCHAR file_system_name_buffer[MAX_PATH + 1]{};
		auto const result = GetVolumeInformationW(root_path_name_wide.c_str(), volume_name_buffer, MAX_PATH + 1, &volume_serial_number, &maximum_component_length, &file_system_flags, file_system_name_buffer, MAX_PATH + 1);
		if (!result) {
			return false;
		}
		info.volume_name.assign(to_string(volume_name_buffer));
		info.file_system_name.assign(to_string(file_system_name_buffer));
		info.volume_serial_number = volume_serial_number;
		info.maximum_component_length = maximum_component_length;
		info.file_system_flags = file_system_flags;
		return true;
	}

	bool getVolumeInformationByHandle(void* handle, VolumeInformation& info) {
		testFileSystemFlags();
		WCHAR volume_name_buffer[MAX_PATH + 1]{};
		DWORD volume_serial_number{};
		DWORD maximum_component_length{};
		DWORD file_system_flags{};
		WCHAR file_system_name_buffer[MAX_PATH + 1]{};
		auto const result = GetVolumeInformationByHandleW(static_cast<HANDLE>(handle), volume_name_buffer, MAX_PATH + 1, &volume_serial_number, &maximum_component_length, &file_system_flags, file_system_name_buffer, MAX_PATH + 1);
		if (!result) {
			return false;
		}
		info.volume_name.assign(to_string(volume_name_buffer));
		info.file_system_name.assign(to_string(file_system_name_buffer));
		info.volume_serial_number = volume_serial_number;
		info.maximum_component_length = maximum_component_length;
		info.file_system_flags = file_system_flags;
		return true;
	}

	bool getVolumePathName(std::string const& file_name, std::string& volume_path_name) {
		auto const file_name_wide = to_wstring(file_name);
		WCHAR volume_path_name_buffer[MAX_PATH + 1]{};
		if (!GetVolumePathNameW(file_name_wide.c_str(), volume_path_name_buffer, MAX_PATH + 1)) {
			return false;
		}
		volume_path_name.assign(to_string(volume_path_name_buffer));
		return true;
	}

	bool queryDosDevice(std::string const& device_name, std::vector<std::string>& target_path) {
		auto const device_name_wide = to_wstring(device_name);
		std::vector<wchar_t> buffer;
		for (;;) {
			buffer.resize(buffer.size() + 256);
			std::fill(buffer.begin(), buffer.end(), L'\0');
			auto const result = QueryDosDeviceW(device_name_wide.empty() ? nullptr : device_name_wide.c_str(), buffer.data(), static_cast<DWORD>(buffer.size()));
			if (result == 0) {
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
					continue;
				}
				else {
					return false;
				}
			}
			else {
				buffer.resize(result);
				break;
			}
		}
		std::string const buffer_utf8(to_string({ buffer.data(), buffer.size() }));
		std::string_view target_paths_view(buffer_utf8);
		target_path.clear();
		while (!target_paths_view.empty()) {
			auto const index = target_paths_view.find_first_of('\0');
			if (index == std::string_view::npos) {
				break;
			}
			if (auto const view = target_paths_view.substr(0, index); !view.empty()) {
				target_path.emplace_back(view);
			}
			auto const next_index = target_paths_view.find_first_not_of('\0', index + 1);
			if (next_index == std::string_view::npos) {
				break;
			}
			target_paths_view = target_paths_view.substr(next_index);
		}
		return true;
	}

	bool setVolumeLabel(std::string const& root_path_name, std::string const& volume_name) {
		auto const root_path_name_wide = to_wstring(root_path_name);
		auto const volume_name_wide = to_wstring(volume_name);
		return abi::as<bool>(SetVolumeLabelW(root_path_name_wide.c_str(), volume_name_wide.c_str()));
	}

	bool setVolumeMountPoint(std::string const& volume_mount_point, std::string const& volume_name) {
		auto const volume_mount_point_wide = to_wstring(volume_mount_point);
		auto const volume_name_wide = to_wstring(volume_name);
		return abi::as<bool>(SetVolumeMountPointW(volume_mount_point_wide.c_str(), volume_name_wide.c_str()));
	}

	bool deleteVolumeMountPoint(std::string const& volume_mount_point) {
		auto const volume_mount_point_wide = to_wstring(volume_mount_point);
		return abi::as<bool>(DeleteVolumeMountPointW(volume_mount_point_wide.c_str()));
	}

	bool defineDosDevice(DefineDosDeviceFlags const flags, std::string const& device_name, std::string const& target_path) {
		auto const device_name_wide = to_wstring(device_name);
		auto const target_path_wide = to_wstring(target_path);
		return abi::as<bool>(DefineDosDeviceW(static_cast<uint32_t>(flags), device_name_wide.c_str(), target_path_wide.empty() ? nullptr : target_path_wide.c_str()));
	}
}
