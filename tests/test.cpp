#include "win32/win32.hpp"
#include "win32/file.hpp"
#include <iostream>
#include <ranges>
#include <Windows.h>
#include "win32/abi.hpp"

template<typename T, size_t BitCount = sizeof(T)* CHAR_BIT>
std::string formatToBits(T const value, char const byte_separator = '\0') {
	std::string buffer;
	buffer.reserve(BitCount);
	for (int offset = BitCount - 1; offset >= 0; offset -= 1) {
		buffer.push_back((value & (1 << offset)) ? '1' : '0');
		if (offset > 0 && (offset % 8) == 0 && byte_separator != '\0') {
			buffer.push_back(byte_separator);
		}
	}
	return buffer;
}

int main() {
	std::setlocale(LC_ALL, ".UTF-8");

	std::u8string_view hello(u8"你好，中国。");
	std::wstring wide_hello(win32::to_wstring(hello));

	uint32_t const dpi = win32::getDpiForSystem();
	win32::Rect rc{ 0, 0, 640, 480 };
	win32::adjustWindowRectExForDpi(rc, WS_OVERLAPPEDWINDOW, false, 0, dpi);

	std::string volume_name;
	std::string volume_mount_point;
	std::vector<std::string> volume_path_names;
	if (auto handle = win32::findFirstVolume(volume_name)) {
		do {
			std::cout << volume_name << std::endl;
			if (win32::getVolumePathNamesForVolumeName(volume_name, volume_path_names)) {
				for (auto const& volume_path_name : volume_path_names) {
					std::cout << "    " << volume_path_name << std::endl;
					switch (win32::getDriveType(volume_path_name)) {
					case win32::DriveType::unknown:
						std::cout << "        unknown"  << std::endl;
						break;
					case win32::DriveType::no_root_dir:
						std::cout << "        no_root_dir" << std::endl;
						break;
					case win32::DriveType::removable:
						std::cout << "        removable" << std::endl;
						break;
					case win32::DriveType::fixed:
						std::cout << "        fixed" << std::endl;
						break;
					case win32::DriveType::remote:
						std::cout << "        remote" << std::endl;
						break;
					case win32::DriveType::cd_rom:
						std::cout << "        cd_rom" << std::endl;
						break;
					case win32::DriveType::ram_disk:
						std::cout << "        ram_disk" << std::endl;
						break;
					}
				}
			}
			if (auto h2 = win32::findFirstVolumeMountPoint(volume_name, volume_mount_point)) {
				do {
					std::cout << "    " << volume_mount_point << std::endl;
				} while (win32::findNextVolumeMountPoint(h2, volume_mount_point));
			}
		} while (win32::findNextVolume(handle, volume_name));
		win32::findVolumeClose(handle);
	}

	auto const drives_mask = win32::getLogicalDrives();
	std::cout << formatToBits(drives_mask, '_') << std::endl;
	std::vector<std::string> drives;
	if (win32::getLogicalDriveStrings(drives)) {
		for (auto const& drive : drives) {
			std::cout << drive << std::endl;
			win32::VolumeInformation volume_info;
			if (win32::getVolumeInformation(drive, volume_info)) {
				std::cout << "    volume_name: " << volume_info.volume_name << std::endl;
				std::cout << "    file_system_name: " << volume_info.file_system_name << std::endl;
				std::cout << "    volume_serial_number: " << volume_info.volume_serial_number << std::endl;
				std::cout << "    maximum_component_length: " << volume_info.maximum_component_length << std::endl;
				std::cout << "    file_system_flags: " << std::endl;
			#define SHOW(field) if (volume_info.file_system_flags.field) std::cout << "        " #field << std::endl;
				SHOW(case_sensitive_search);
				SHOW(case_preserved_names);
				SHOW(unicode_on_disk);
				SHOW(persistent_acls);
				SHOW(file_compression);
				SHOW(volume_quotas);
				SHOW(supports_sparse_files);
				SHOW(supports_reparse_points);
				SHOW(supports_remote_storage);
				SHOW(returns_cleanup_result_info);
				SHOW(supports_posix_unlink_rename);
				SHOW(supports_bypass_io);
				SHOW(supports_stream_snapshots);
				SHOW(supports_case_sensitive_dirs);
				SHOW(volume_is_compressed);
				SHOW(supports_object_ids);
				SHOW(supports_encryption);
				SHOW(named_streams);
				SHOW(read_only_volume);
				SHOW(sequential_write_once);
				SHOW(supports_transactions);
				SHOW(supports_hard_links);
				SHOW(supports_extended_attributes);
				SHOW(supports_open_by_file_id);
				SHOW(supports_usn_journal);
				SHOW(supports_integrity_streams);
				SHOW(supports_block_refcounting);
				SHOW(supports_sparse_vdl);
				SHOW(dax_volume);
				SHOW(supports_ghosting);
			#undef SHOW
			}
		}
	}

	HANDLE file = CreateFileW(L"你好.wav", FILE_GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (file != INVALID_HANDLE_VALUE) {
		std::cout << "你好.wav" << std::endl;
		std::string volume_path_name;
		if (win32::getVolumePathName("你好.wav", volume_path_name)) {
			std::cout << "    volume_path_name: " << volume_path_name << std::endl;
		}
		win32::VolumeInformation volume_info;
		if (win32::getVolumeInformationByHandle(file, volume_info)) {
			std::cout << "    volume_name: " << volume_info.volume_name << std::endl;
			std::cout << "    file_system_name: " << volume_info.file_system_name << std::endl;
			std::cout << "    volume_serial_number: " << volume_info.volume_serial_number << std::endl;
			std::cout << "    maximum_component_length: " << volume_info.maximum_component_length << std::endl;
			std::cout << "    file_system_flags: " << std::endl;
		#define SHOW(field) if (volume_info.file_system_flags.field) std::cout << "        " #field << std::endl;
			SHOW(case_sensitive_search);
			SHOW(case_preserved_names);
			SHOW(unicode_on_disk);
			SHOW(persistent_acls);
			SHOW(file_compression);
			SHOW(volume_quotas);
			SHOW(supports_sparse_files);
			SHOW(supports_reparse_points);
			SHOW(supports_remote_storage);
			SHOW(returns_cleanup_result_info);
			SHOW(supports_posix_unlink_rename);
			SHOW(supports_bypass_io);
			SHOW(supports_stream_snapshots);
			SHOW(supports_case_sensitive_dirs);
			SHOW(volume_is_compressed);
			SHOW(supports_object_ids);
			SHOW(supports_encryption);
			SHOW(named_streams);
			SHOW(read_only_volume);
			SHOW(sequential_write_once);
			SHOW(supports_transactions);
			SHOW(supports_hard_links);
			SHOW(supports_extended_attributes);
			SHOW(supports_open_by_file_id);
			SHOW(supports_usn_journal);
			SHOW(supports_integrity_streams);
			SHOW(supports_block_refcounting);
			SHOW(supports_sparse_vdl);
			SHOW(dax_volume);
			SHOW(supports_ghosting);
		#undef SHOW
		}
		CloseHandle(file);
	}

	std::vector<std::string> target_path;
	if (win32::queryDosDevice("", target_path)) {
		for (auto const& tp : target_path) {
			std::cout << tp << std::endl;
		}
	}

	return 0;
}
