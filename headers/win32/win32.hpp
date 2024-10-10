#pragma once
#include <string>
#include <string_view>

namespace win32 {
	std::string to_string(std::wstring_view const& str);

	std::wstring to_wstring(std::string_view const& str);

	class Module {
	private:
		void* handle;
	public:
		bool isLoaded();
		void* findSymbolPointer(std::string_view const& name);
		template<typename T>
		inline T findSymbol(std::string_view const& name) { return reinterpret_cast<T>(findSymbolPointer(name)); }
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

	bool enableNonClientDpiScaling(WindowHandle* window_handle);
}
