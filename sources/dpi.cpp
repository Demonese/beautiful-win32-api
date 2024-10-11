#include "shared.hpp"
#include "win32/win32.hpp"
#include "win32/abi.hpp"

// reference: https://github.com/tringi/win32-dpi

namespace win32 {
	static Module user32("user32");

	static Module shcore("shcore");

	int32_t scaleByDpi(int32_t value, uint32_t dpi) {
		return MulDiv(value, static_cast<int>(dpi), USER_DEFAULT_SCREEN_DPI);
	}

	uint32_t scaleByDpi(uint32_t value, uint32_t dpi) {
		assert(value > 0);
		return static_cast<uint32_t>(MulDiv(static_cast<int>(value), static_cast<int>(dpi), USER_DEFAULT_SCREEN_DPI));
	}

	// Windows Vista
	static ModuleSymbol<decltype(SetProcessDPIAware)> user32_SetProcessDPIAware(user32, "SetProcessDPIAware");

	bool setProcessDpiAware() {
		if (user32_SetProcessDPIAware) {
			return abi::as<bool>(user32_SetProcessDPIAware.get()());
		}
		return true;
	}

	// Windows 8.1
	static ModuleSymbol<decltype(SetProcessDpiAwareness)> shcore_SetProcessDpiAwareness(shcore, "SetProcessDpiAwareness");

	bool setProcessDpiAwareness(ProcessDpiAwareness awareness) {
		if (shcore_SetProcessDpiAwareness) {
			return SUCCEEDED(shcore_SetProcessDpiAwareness.get()(static_cast<PROCESS_DPI_AWARENESS>(awareness)));
		}
		return setProcessDpiAware();
	}

	// Windows 10 1703
	static ModuleSymbol<decltype(SetProcessDpiAwarenessContext)> user32_SetProcessDpiAwarenessContext(user32, "SetProcessDpiAwarenessContext");

	bool setProcessDpiAwarenessContext(ProcessDpiAwarenessContext context) {
		if (user32_SetProcessDpiAwarenessContext) {
			return abi::as<bool>(user32_SetProcessDpiAwarenessContext.get()(reinterpret_cast<DPI_AWARENESS_CONTEXT>(context)));
		}
		switch (context) {
		default:
			return setProcessDpiAwareness(ProcessDpiAwareness::unaware);
		case ProcessDpiAwarenessContext::system_aware:
			return setProcessDpiAwareness(ProcessDpiAwareness::system_dpi_aware);
		case ProcessDpiAwarenessContext::per_monitor_aware:
		case ProcessDpiAwarenessContext::per_monitor_aware_v2:
			return setProcessDpiAwareness(ProcessDpiAwareness::per_monitor_dpi_aware);
		}
	}

	// Windows 10 1607
	static ModuleSymbol<decltype(EnableNonClientDpiScaling)> user32_EnableNonClientDpiScaling(user32, "EnableNonClientDpiScaling");

	bool enableNonClientDpiScaling(WindowHandle* window_handle) {
		if (user32_EnableNonClientDpiScaling) {
			return abi::as<bool>(user32_EnableNonClientDpiScaling.get()(abi::as<HWND>(window_handle)));
		}
		return true;
	}

	// Windows 10 1607
	static ModuleSymbol<decltype(AdjustWindowRectExForDpi)> user32_AdjustWindowRectExForDpi(user32, "AdjustWindowRectExForDpi");

	bool adjustWindowRectExForDpi(Rect* rect, uint32_t style, bool menu, uint32_t style_ex, uint32_t dpi) {
		if (user32_AdjustWindowRectExForDpi) {
			return abi::as<bool>(user32_AdjustWindowRectExForDpi.get()(abi::as<RECT*>(rect), style, abi::as<BOOL>(menu), style_ex, dpi));
		}
		return abi::as<bool>(AdjustWindowRectEx(abi::as<RECT*>(rect), style, abi::as<BOOL>(menu), style_ex));
	}

	// Windows 10 1607
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

	// Windows 8.1
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

	// Windows 10 1607
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

	// Windows 10 1607
	static ModuleSymbol<decltype(GetSystemMetricsForDpi)> user32_GetSystemMetricsForDpi(user32, "GetSystemMetricsForDpi");

	int32_t getSystemMetricsForDpi(int32_t index, uint32_t dpi) {
		if (user32_GetSystemMetricsForDpi) {
			return user32_GetSystemMetricsForDpi.get()(index, dpi);
		}
		return GetSystemMetrics(index);
	}
}
