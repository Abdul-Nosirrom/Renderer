#pragma once

#include "IO/Keyboard.h"
#include "IO/Mouse.h"
#include "Graphics/Graphics.h"
#include "RomanceWin.h"
#include <memory>
#include <string>
#include <optional>

class RENDERENGINE_API Window
{
private:
	/// @brief	Singleton class managing registration of windows
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();

		// Singleton so we don't need these
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		
		static constexpr const char* s_ClassName = "Romance Dawn DX11 Engine Window";
		static WindowClass s_WindowClass;
		HINSTANCE m_hInst;
	};

public:
	Window(int width, int height, const char* name);
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void SetTitle(const std::string& title);

	Graphics& GFX() const { return *pGFX;  }

	/// @brief  Checks PeekMessage (for all windows, hence static), and handles it, returning a value if found an exit message
	static std::optional<int> ProcessMessages();

private:
	// Cannot be a member function, Win32 API is not C++, no OOP so need a "free function"
	/// @brief  Job is to setup our window class instance with the window API, then switch proc function
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	/// @brief  Job is to invoke our member function
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	/// @brief  Member function that is piped with window messages
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

private:
	int m_Width, m_Height;
	HWND m_hWnd;

	std::unique_ptr<Graphics> pGFX;

};

