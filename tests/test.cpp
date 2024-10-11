#include "win32/win32.hpp"
#include <Windows.h>
#include "win32/abi.hpp"

int main() {
	win32::Rect rc{ 0, 0, 640, 480 };
	win32::adjustWindowRectExForDpi(rc, WS_OVERLAPPEDWINDOW, false, 0, USER_DEFAULT_SCREEN_DPI);
	return 0;
}
