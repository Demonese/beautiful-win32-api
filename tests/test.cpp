#include "win32/win32.hpp"
#include <Windows.h>
#include "win32/abi.hpp"

int main() {
	std::u8string_view hello(u8"你好，中国。");
	std::wstring wide_hello(win32::to_wstring(hello));

	uint32_t const dpi = win32::getDpiForSystem();
	win32::Rect rc{ 0, 0, 640, 480 };
	win32::adjustWindowRectExForDpi(rc, WS_OVERLAPPEDWINDOW, false, 0, dpi);
	return 0;
}
