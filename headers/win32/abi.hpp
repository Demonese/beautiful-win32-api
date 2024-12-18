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
}

namespace win32 {
	inline bool adjustWindowRectExForDpi(::RECT* rect, uint32_t style, bool menu, uint32_t style_ex, uint32_t dpi) {
		return adjustWindowRectExForDpi(abi::as<Rect*>(rect), style, menu, style_ex, dpi);
	}

	// similar to getDpiForWindow, return 0 if fail
	inline uint32_t getDpiForMonitor(::HMONITOR monitor) {
		return getDpiForMonitor(abi::as<MonitorHandle*>(monitor));
	}

	// similar to getDpiScalingForSystem, return 0 if fail
	inline float getDpiScalingForMonitor(::HMONITOR monitor) {
		return static_cast<float>(getDpiForMonitor(abi::as<MonitorHandle*>(monitor))) / static_cast<float>(getUserDefaultScreenDpi());
	}

	inline uint32_t getDpiForWindow(::HWND window) {
		return getDpiForWindow(abi::as<WindowHandle*>(window));
	}

	inline float getDpiScalingForWindow(::HWND window) {
		return static_cast<float>(getDpiForWindow(abi::as<WindowHandle*>(window))) / static_cast<float>(getUserDefaultScreenDpi());
	}

	inline bool enableNonClientDpiScaling(::HWND window) {
		return enableNonClientDpiScaling(abi::as<WindowHandle*>(window));
	}
}
