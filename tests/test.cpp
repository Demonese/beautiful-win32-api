#include "win32/win32.hpp"
#include <Windows.h>
#include "win32/abi.hpp"

int main() {
	HWND hwnd{};
	auto v1 = win32::abi::as<win32::WindowHandle*>(hwnd);

	return 0;
}
