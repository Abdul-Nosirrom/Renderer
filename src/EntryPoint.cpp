#include "RomanceWin.h"
#include "Window.h"
#include "Camera.h"
#include "Errors/RomanceException.h"
#include <chrono>

/// <summary>
/// For a "Windows" application, the default entry point is WinMain, NOT Main
/// 'CALLBACK'/'WINAPI' is stdcall, which is an API declspec type thing for functions the window API will call (like this one)
/// </summary>
//int WINAPI WinMain(
//	_In_		HINSTANCE	hInstance,		/* Handle to application instance used to identify the EXE*/
//	_In_opt_	HINSTANCE	hPrevInstance,	/* Docs say has no meaning, was used in 16-bit, but now is always zero */
//	_In_		LPSTR		lpCmdLine,		/* Contains command line arguments as a unicode string */
//	_In_		int			nShowCmd)		/* Flag that indicates whether the main app window is minimized, maximized, or shown normally (See docs for specific flags) */

int main(int argc, char* argv[]) // SUBSYSTEM:CONSOLE, using default main() signature, but we're still able to initialize win32 
{
	try
	{
		Window newWind(800, 600, "Renderer");
		float ClearColor[4] = { 0.5, 0, 0, 1 };
		newWind.GFX().SetClearColor(ClearColor);
		// BEGIN time tests
		typedef std::chrono::high_resolution_clock Time;
		typedef std::chrono::milliseconds ms;
		typedef std::chrono::duration<float> fsec;
		float dt = 0.f;
		// ~
		while (true)
		{
			auto t0 = Time::now();

			// Process all messages pending
			if (const auto ecode = Window::ProcessMessages())
			{
				// if return optional has a value, that means its an exit code
				return *ecode;
			}

			Camera::Get().Update(dt);

			newWind.GFX().StartFrame(); // Clears render target
			newWind.GFX().Draw(dt); // Sets all buffers/pipeline state then calls draw
			newWind.GFX().EndFrame(); // Presents the backbuffer

			auto t1 = Time::now();
			fsec tInS = t1 - t0;
			dt = tInS.count();
		}
	}
	catch (const RomanceException& e)
	{
		// Setting first param to null (window Handle) means message box window has no parent window
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}