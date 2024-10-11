#pragma once
#include "win32/win32.hpp"

namespace win32::abi {
	template<typename ToType, typename FromType> ToType inline as(FromType value) { static_assert(false); }

	template<> inline HMODULE as(ModuleHandle* value) { return reinterpret_cast<HMODULE>(value); }
	template<> inline ModuleHandle* as(HMODULE value) { return reinterpret_cast<ModuleHandle*>(value); }

	template<> inline HWND as(WindowHandle* value) { return reinterpret_cast<HWND>(value); }
	template<> inline WindowHandle* as(HWND value) { return reinterpret_cast<WindowHandle*>(value); }
}
