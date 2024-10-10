#include "shared.hpp"
#include "win32/win32.hpp"

namespace win32 {
	bool Module::isLoaded() {
		return handle != nullptr;
	}
	void* Module::findSymbolPointer(std::string_view const& name) {
		return (handle != nullptr) ? GetProcAddress(static_cast<HMODULE>(handle), name.data()) : nullptr;
	}
	Module::Module(std::string_view const& name) : handle(nullptr) {
		std::wstring const wide_name(win32::to_wstring(name));
		handle = LoadLibraryExW(wide_name.c_str(), nullptr, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
	}
	Module::~Module() {
		if (handle) {
			FreeLibrary(static_cast<HMODULE>(handle));
		}
	}
}
