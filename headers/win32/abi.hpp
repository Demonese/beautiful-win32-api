#pragma once
#include "win32/win32.hpp"

namespace win32::abi {
	template<typename ToType, typename FromType> ToType inline as(FromType value) { static_assert(false); }

	// WTF ???
	template<> inline ::BOOL as(bool value) { return value ? TRUE : FALSE; }
	template<> inline bool as(::BOOL value) { return (value == FALSE) ? false : true; }

	template<> inline ::HMODULE as(ModuleHandle* value) { return reinterpret_cast<::HMODULE>(value); }
	template<> inline ModuleHandle* as(::HMODULE value) { return reinterpret_cast<ModuleHandle*>(value); }

	template<> inline ::HMONITOR as(MonitorHandle* value) { return reinterpret_cast<::HMONITOR>(value); }
	template<> inline MonitorHandle* as(::HMONITOR value) { return reinterpret_cast<MonitorHandle*>(value); }

	template<> inline ::HWND as(WindowHandle* value) { return reinterpret_cast<::HWND>(value); }
	template<> inline WindowHandle* as(::HWND value) { return reinterpret_cast<WindowHandle*>(value); }

	static_assert(sizeof(::RECT) == sizeof(Rect));
	template<> inline ::RECT* as(Rect* value) { return reinterpret_cast<::RECT*>(value); }
	template<> inline Rect* as(::RECT* value) { return reinterpret_cast<Rect*>(value); }

	inline bool adjustWindowRectExForDpi(::RECT* rect, uint32_t style, bool menu, uint32_t style_ex, uint32_t dpi) {
		return adjustWindowRectExForDpi(as<Rect*>(rect), style, menu, style_ex, dpi);
	}

	// similar to getDpiForWindow, return 0 if fail
	inline uint32_t getDpiForMonitor(::HMONITOR monitor) {
		return getDpiForMonitor(as<MonitorHandle*>(monitor));
	}

	inline uint32_t getDpiForWindow(::HWND window) {
		return getDpiForWindow(as<WindowHandle*>(window));
	}

	bool enableNonClientDpiScaling(::HWND window) {
		return enableNonClientDpiScaling(as<WindowHandle*>(window));
	}
}
