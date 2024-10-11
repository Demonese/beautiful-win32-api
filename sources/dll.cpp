#include "shared.hpp"
#include "win32/win32.hpp"
#include "win32/abi.hpp"

namespace win32 {
	void* Module::findSymbolPointer(std::string_view const& name) {
		return (handle != nullptr) ? GetProcAddress(abi::as<HMODULE>(handle), name.data()) : nullptr;
	}
	Module::Module(std::string_view const& name) : handle(nullptr) {
		std::wstring const wide_name(win32::to_wstring(name));
		handle = abi::as<ModuleHandle*>(LoadLibraryExW(wide_name.c_str(), nullptr, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS));
	}
	Module::~Module() {
		if (handle) {
			FreeLibrary(abi::as<HMODULE>(handle));
		}
	}
}
