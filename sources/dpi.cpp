#include "shared.hpp"
#include "win32/win32.hpp"
#include "win32/abi.hpp"

// reference: https://github.com/tringi/win32-dpi

namespace win32 {
	static Module user32("user32");

	static Module shcore("shcore");

	static ModuleSymbol<decltype(EnableNonClientDpiScaling)> user32_EnableNonClientDpiScaling(user32, "EnableNonClientDpiScaling");

	bool enableNonClientDpiScaling(WindowHandle* window_handle) {
		if (user32_EnableNonClientDpiScaling) {
			return abi::as<bool>(user32_EnableNonClientDpiScaling.get()(abi::as<HWND>(window_handle)));
		}
		return true;
	}

	static ModuleSymbol<decltype(AdjustWindowRectExForDpi)> user32_AdjustWindowRectExForDpi(user32, "AdjustWindowRectExForDpi");

	bool adjustWindowRectExForDpi(Rect& rect, uint32_t style, bool menu, uint32_t style_ex, uint32_t dpi) {
		if (user32_AdjustWindowRectExForDpi) {
			return abi::as<bool>(user32_AdjustWindowRectExForDpi.get()(abi::as<RECT*>(&rect), style, abi::as<BOOL>(menu), style_ex, dpi));
		}
		return abi::as<bool>(AdjustWindowRectEx(abi::as<RECT*>(&rect), style, abi::as<BOOL>(menu), style_ex));
	}

	static ModuleSymbol<decltype(GetDpiForSystem)> user32_GetDpiForSystem(user32, "GetDpiForSystem");

	uint32_t getDpiForSystem() {
		if (user32_GetDpiForSystem) {
			return user32_GetDpiForSystem.get()();
		}
		if (HDC dc = GetDC(NULL)) {
			int const x = GetDeviceCaps(dc, LOGPIXELSX);
			[[maybe_unused]] int const y = GetDeviceCaps(dc, LOGPIXELSY);
			assert(x == y); // if hit this assert, tell me WHY
			ReleaseDC(NULL, dc);
			return static_cast<uint32_t>(x);
		}
		return USER_DEFAULT_SCREEN_DPI;
	}

	static ModuleSymbol<decltype(GetDpiForMonitor)> shcore_GetDpiForMonitor(shcore, "GetDpiForMonitor");

	uint32_t getDpiForMonitor(MonitorHandle* monitor_handle) {
		if (shcore_GetDpiForMonitor) {
			UINT x{};
			[[maybe_unused]] UINT y{};
			HRESULT const hr = shcore_GetDpiForMonitor.get()(abi::as<HMONITOR>(monitor_handle), MDT_DEFAULT, &x, &y);
			assert(x == y); // if hit this assert, tell me WHY
			return SUCCEEDED(hr) ? x : 0;
		}
		return getDpiForSystem();
	}

	static ModuleSymbol<decltype(GetDpiForWindow)> user32_GetDpiForWindow(user32, "GetDpiForWindow");

	uint32_t getDpiForWindow(WindowHandle* window_handle) {
		if (user32_GetDpiForWindow) {
			return user32_GetDpiForWindow.get()(abi::as<HWND>(window_handle));
		}
		if (HMONITOR monitor = MonitorFromWindow(abi::as<HWND>(window_handle), MONITOR_DEFAULTTONULL)) {
			return getDpiForMonitor(abi::as<MonitorHandle*>(monitor));
		}
		return getDpiForSystem();
	}

	static ModuleSymbol<decltype(GetSystemMetricsForDpi)> user32_GetSystemMetricsForDpi(user32, "GetSystemMetricsForDpi");

	int32_t getSystemMetricsForDpi(int32_t index, uint32_t dpi) {
		if (user32_GetSystemMetricsForDpi) {
			return user32_GetSystemMetricsForDpi.get()(index, dpi);
		}
		return GetSystemMetrics(index);
	}
}
