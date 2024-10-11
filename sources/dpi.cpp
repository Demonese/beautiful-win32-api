#include "shared.hpp"
#include "win32/win32.hpp"
#include "win32/abi.hpp"

namespace win32 {
	static Module user32("user32");

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
}
