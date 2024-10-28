#pragma once
#include <vector>
#include <string>

namespace win32 {
	// Symbol: HANDLE
	// Reference: https://learn.microsoft.com/en-us/windows/win32/sysinfo/handles-and-objects
	struct FindVolumeHandle;

	// Symbol: FindFirstVolumeW
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-findfirstvolumew
	FindVolumeHandle* findFirstVolume(std::string& volume_name);

	// Symbol: FindNextVolumeW
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-findnextvolumew
	bool findNextVolume(FindVolumeHandle* handle, std::string& volume_name);

	// Symbol: FindVolumeClose
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-findvolumeclose
	bool findVolumeClose(FindVolumeHandle* handle);

	// Symbol: GetVolumePathNamesForVolumeNameW
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumepathnamesforvolumenamew
	bool getVolumePathNamesForVolumeName(std::string const& volume_name, std::vector<std::string>& volume_path_names);

	// Symbol: HANDLE
	// Reference: https://learn.microsoft.com/en-us/windows/win32/sysinfo/handles-and-objects
	struct FindVolumeMountPointHandle;

	// Symbol: FindFirstVolumeMountPointW
	// UAC: Yes
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-findfirstvolumemountpointw
	FindVolumeMountPointHandle* findFirstVolumeMountPoint(std::string const& volume_name, std::string& volume_mount_point);

	// Symbol: FindNextVolumeMountPointW
	// UAC: Yes
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-findnextvolumemountpointw
	bool findNextVolumeMountPoint(FindVolumeMountPointHandle* handle, std::string& volume_mount_point);

	// Symbol: FindVolumeMountPointClose
	// UAC: Yes
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-findvolumemountpointclose
	bool findVolumeMountPointClose(FindVolumeMountPointHandle* handle);

	// Symbol: GetLogicalDrives
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getlogicaldrives
	uint32_t getLogicalDrives();

	// Symbol: GetLogicalDriveStringsW
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getlogicaldrivestringsw
	bool getLogicalDriveStrings(std::vector<std::string>& drives);

	// Symbols: DRIVE_*
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getdrivetypew
	enum class DriveType {
		// Symbol: DRIVE_UNKNOWN
		// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getdrivetypew
		unknown = 0,

		// Symbol: DRIVE_NO_ROOT_DIR
		// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getdrivetypew
		no_root_dir = 1,

		// Symbol: DRIVE_REMOVABLE
		// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getdrivetypew
		removable = 2,

		// Symbol: DRIVE_FIXED
		// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getdrivetypew
		fixed = 3,

		// Symbol: DRIVE_REMOTE
		// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getdrivetypew
		remote = 4,

		// Symbol: DRIVE_CDROM
		// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getdrivetypew
		cd_rom = 5,

		// Symbol: DRIVE_RAMDISK
		// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getdrivetypew
		ram_disk = 6,
	};

	// Symbol: GetDriveTypeW
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getdrivetypew
	DriveType getDriveType(std::string const& root_path_name);
}
