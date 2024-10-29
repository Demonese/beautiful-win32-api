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

	// Symbol: GetVolumeNameForVolumeMountPointW
	// UAC: Yes
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumenameforvolumemountpointw
	bool getVolumeNameForVolumeMountPoint(std::string const& volume_mount_point, std::string& volume_name);

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

	struct VolumeInformation {
		std::string volume_name;
		std::string file_system_name;
		uint32_t volume_serial_number{};
		uint32_t maximum_component_length{};

		struct FileSystemFlags {
			// Symbol: FILE_CASE_SENSITIVE_SEARCH
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t case_sensitive_search : 1;

			// Symbol: FILE_CASE_PRESERVED_NAMES
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t case_preserved_names : 1;

			// Symbol: FILE_UNICODE_ON_DISK
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t unicode_on_disk : 1;

			// Symbol: FILE_PERSISTENT_ACLS
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t persistent_acls : 1;

			// Symbol: FILE_FILE_COMPRESSION
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t file_compression : 1;

			// Symbol: FILE_VOLUME_QUOTAS
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t volume_quotas : 1;

			// Symbol: FILE_SUPPORTS_SPARSE_FILES
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t supports_sparse_files : 1;

			// Symbol: FILE_SUPPORTS_REPARSE_POINTS
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t supports_reparse_points : 1;

			// Symbol: FILE_SUPPORTS_REMOTE_STORAGE
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t supports_remote_storage : 1;

			// Symbol: FILE_RETURNS_CLEANUP_RESULT_INFO
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t returns_cleanup_result_info : 1;

			// Symbol: FILE_SUPPORTS_POSIX_UNLINK_RENAME
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t supports_posix_unlink_rename : 1;

			// Symbol: FILE_SUPPORTS_BYPASS_IO
			// Reference: winnt.h
			uint32_t supports_bypass_io : 1;

			// Symbol: FILE_SUPPORTS_STREAM_SNAPSHOTS
			// Reference: winnt.h
			uint32_t supports_stream_snapshots : 1;

			// Symbol: FILE_SUPPORTS_CASE_SENSITIVE_DIRS
			// Reference: winnt.h
			uint32_t supports_case_sensitive_dirs : 1;

			uint32_t _placeholder1 : 1;

			// Symbol: FILE_VOLUME_IS_COMPRESSED
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t volume_is_compressed : 1;

			// Symbol: FILE_SUPPORTS_OBJECT_IDS
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t supports_object_ids : 1;

			// Symbol: FILE_SUPPORTS_ENCRYPTION
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t supports_encryption : 1;

			// Symbol: FILE_NAMED_STREAMS
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t named_streams : 1;

			// Symbol: FILE_READ_ONLY_VOLUME
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t read_only_volume : 1;

			// Symbol: FILE_SEQUENTIAL_WRITE_ONCE
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t sequential_write_once : 1;

			// Symbol: FILE_SUPPORTS_TRANSACTIONS
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t supports_transactions : 1;

			// Symbol: FILE_SUPPORTS_HARD_LINKS
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t supports_hard_links : 1;

			// Symbol: FILE_SUPPORTS_EXTENDED_ATTRIBUTES
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t supports_extended_attributes : 1;

			// Symbol: FILE_SUPPORTS_OPEN_BY_FILE_ID
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t supports_open_by_file_id : 1;

			// Symbol: FILE_SUPPORTS_USN_JOURNAL
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t supports_usn_journal : 1;

			// Symbol: FILE_SUPPORTS_INTEGRITY_STREAMS
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t supports_integrity_streams : 1;

			// Symbol: FILE_SUPPORTS_BLOCK_REFCOUNTING
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t supports_block_refcounting : 1;

			// Symbol: FILE_SUPPORTS_SPARSE_VDL
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t supports_sparse_vdl : 1;

			// Symbol: FILE_DAX_VOLUME
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t dax_volume : 1;

			// Symbol: FILE_SUPPORTS_GHOSTING
			// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
			uint32_t supports_ghosting : 1;

			uint32_t _placeholder2 : 1;

			inline FileSystemFlags& operator=(uint32_t const value) { *reinterpret_cast<uint32_t*>(this) = value; return *this; }

			inline explicit operator uint32_t() const noexcept { return *reinterpret_cast<uint32_t const*>(this); }
		};

		static_assert(sizeof(FileSystemFlags) == sizeof(uint32_t));

		FileSystemFlags file_system_flags{};
	};

	// WARNING: remember to call SetErrorMode to avoid "insert a floppy disk or a compact disc" dialog
	// Symbol: GetVolumeInformationW
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationw
	bool getVolumeInformation(std::string const& root_path_name, VolumeInformation& info);

	// Symbol: GetVolumeInformationByHandleW
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationbyhandlew
	bool getVolumeInformationByHandle(void* handle, VolumeInformation& info);

	// Symbol: GetVolumePathNameW
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumepathnamew
	bool getVolumePathName(std::string const& file_name, std::string& volume_path_name);

	// Symbol: QueryDosDeviceW
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-querydosdevicew
	bool queryDosDevice(std::string const& device_name, std::vector<std::string>& target_path);

	// Symbol: SetVolumeLabelW
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-setvolumelabelw
	bool setVolumeLabel(std::string const& root_path_name, std::string const& volume_name);

	// Symbol: SetVolumeMountPointW
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-setvolumemountpointw
	bool setVolumeMountPoint(std::string const& volume_mount_point, std::string const& volume_name);

	// Symbol: DeleteVolumeMountPointW
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-deletevolumemountpointw
	bool deleteVolumeMountPoint(std::string const& volume_mount_point);

	// Symbols: DDD_*
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-definedosdevicew
	struct DefineDosDeviceFlags {
		// Symbol: DDD_RAW_TARGET_PATH
		// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-definedosdevicew
		uint32_t raw_target_path : 1;

		// Symbol: DDD_REMOVE_DEFINITION
		// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-definedosdevicew
		uint32_t remove_definition : 1;

		// Symbol: DDD_EXACT_MATCH_ON_REMOVE
		// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-definedosdevicew
		uint32_t exact_match_on_remove : 1;

		// Symbol: DDD_NO_BROADCAST_SYSTEM
		// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-definedosdevicew
		uint32_t no_broadcast_system : 1;

		uint32_t _placeholder1 : 28;

		inline DefineDosDeviceFlags& operator=(uint32_t const value) { *reinterpret_cast<uint32_t*>(this) = value; return *this; }

		inline explicit operator uint32_t() const noexcept { return *reinterpret_cast<uint32_t const*>(this); }
	};

	static_assert(sizeof(DefineDosDeviceFlags) == sizeof(uint32_t));

	// Symbol: DefineDosDeviceW
	// Reference: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-definedosdevicew
	bool defineDosDevice(DefineDosDeviceFlags const flags, std::string const& device_name, std::string const& target_path);
}
