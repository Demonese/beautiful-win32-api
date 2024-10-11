#include "win32/win32.hpp"
#include <Windows.h>
#include "win32/abi.hpp"

int main() {
	uint32_t const dpi = win32::getDpiForSystem();
	win32::Rect rc{ 0, 0, 640, 480 };
	win32::adjustWindowRectExForDpi(rc, WS_OVERLAPPEDWINDOW, false, 0, dpi);
	return 0;
}
