#include "shared.hpp"
#include "win32/win32.hpp"
#include "win32/abi.hpp"

namespace win32 {
	static Module user32("user32");

	static ModuleSymbol<decltype(EnableNonClientDpiScaling)> user32_EnableNonClientDpiScaling(user32, "EnableNonClientDpiScaling");

	bool enableNonClientDpiScaling(WindowHandle* window_handle) {
		if (user32_EnableNonClientDpiScaling) {
			return user32_EnableNonClientDpiScaling.get()(abi::as<HWND>(window_handle));
		}
		return true;
	}
}
