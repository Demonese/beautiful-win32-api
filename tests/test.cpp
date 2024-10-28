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
		}
	}

	return 0;
}
