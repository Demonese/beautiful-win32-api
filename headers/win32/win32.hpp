#pragma once
#include <string>
#include <string_view>

namespace win32 {
	std::string to_string(std::wstring_view const& str);

	std::u8string to_u8string(std::wstring_view const& str);

	std::wstring to_wstring(std::string_view const& str);

	std::wstring to_wstring(std::u8string_view const& str);

	struct ModuleHandle;

	class Module {
	private:
		ModuleHandle* handle;
	public:
		void* findSymbolPointer(std::string_view const& name);
		template<typename T>
		inline T findSymbol(std::string_view const& name) { return reinterpret_cast<T>(findSymbolPointer(name)); }
		inline operator bool() { return handle != nullptr; }
	public:
		Module(std::string_view const& name);
		~Module();
	};

	template<typename T>
	class ModuleSymbol {
	private:
		T* handle;
	public:
		inline T* get() { return handle; }
		inline operator bool() { return handle != nullptr; }
	public:
		inline ModuleSymbol(Module& m, std::string_view const& name) : handle(m.findSymbol<T*>(name)) {}
	};

	struct WindowHandle;

	struct MonitorHandle;

	struct Rect {
		int32_t left;
		int32_t top;
		int32_t right;
		int32_t bottom;
	};

	// USER_DEFAULT_SCREEN_DPI
	consteval inline uint32_t getUserDefaultScreenDpi() { return 96; }

	bool adjustWindowRectExForDpi(Rect* rect, uint32_t style, bool menu, uint32_t style_ex, uint32_t dpi);

	inline bool adjustWindowRectExForDpi(Rect& rect, uint32_t style, bool menu, uint32_t style_ex, uint32_t dpi) {
		return adjustWindowRectExForDpi(&rect, style, menu, style_ex, dpi);
	}

	uint32_t getDpiForSystem();

	inline float getDpiScalingForSystem() {
		return static_cast<float>(getDpiForSystem()) / static_cast<float>(getUserDefaultScreenDpi());
	}

	// similar to getDpiForWindow, return 0 if fail
	uint32_t getDpiForMonitor(MonitorHandle* monitor_handle);

	// similar to getDpiScalingForSystem, return 0 if fail
	inline float getDpiScalingForMonitor(MonitorHandle* monitor_handle) {
		return static_cast<float>(getDpiForMonitor(monitor_handle)) / static_cast<float>(getUserDefaultScreenDpi());
	}

	uint32_t getDpiForWindow(WindowHandle* window_handle);

	inline float getDpiScalingForWindow(WindowHandle* window_handle) {
		return static_cast<float>(getDpiForWindow(window_handle)) / static_cast<float>(getUserDefaultScreenDpi());
	}

	bool enableNonClientDpiScaling(WindowHandle* window_handle);

	int32_t getSystemMetricsForDpi(int32_t index, uint32_t dpi);
}
