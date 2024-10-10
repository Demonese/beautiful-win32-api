#pragma once
#include "win32/win32.hpp"

namespace win32::abi {
	template<typename ToType, typename FromType> ToType inline as(FromType value) { static_assert(false); }

	template<> inline HWND as(WindowHandle* value) { return reinterpret_cast<HWND>(value); }
	template<> inline WindowHandle* as(HWND value) { return reinterpret_cast<WindowHandle*>(value); }
}