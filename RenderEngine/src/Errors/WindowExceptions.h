#pragma once
#include <Windows.h>
#include "RomanceException.h"

// NOTE: Don't throw exceptions in WinAPI/Callback functions, will just crash the program and will not allow us to retrieve error info as its windows that calls it not our program
// To do the above, we catch the exceptions in those WINAPI functions, store them, and them throw them later in our own functions
/* Error exception helper macros for windows */
#define RDWND_EXCEPT(hr) WindowException(__LINE__, __FILE__, hr) // useful macros for filling out line number and file location
#define RDWND_LAST_EXCEPT() WindowException(__LINE__, __FILE__, GetLastError())
#define RDWND_NOGFX_EXCEPT() WindowException(__LINE__, __FILE__)

namespace HRErrors
{
	inline std::string TranslateErrorCode(HRESULT hr)
	{
		char* pMsgBuf = nullptr;
		// Format Message takes hResult and returns a readable error message, filling out our char*. Returns length of error string
		DWORD nMsgLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |                 /*  Allocates a buffer for our message, lets pMsgBuf point to it */
			FORMAT_MESSAGE_FROM_SYSTEM |                            /* Searches message table for a requested message. Allows us to use GetLastError() */
			FORMAT_MESSAGE_IGNORE_INSERTS,                          /* Ignores insert formatting (e.g %1), so output message can be formatted later */
			nullptr,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&pMsgBuf),
			0, nullptr);                             /* nSize zero because we're using Allocate_Buffer, otherwise this would set the size of the output buffer. If Alloc flag is set, this is the min size */

		if (nMsgLen == 0)
		{
			return "Unidentified Error Code";
		}

		std::string errorString = pMsgBuf;
		LocalFree(pMsgBuf);
		return errorString;
	}
}

class WindowException : public RomanceException
{
public:
	WindowException(int line, const char* file, HRESULT hr) noexcept
		: RomanceException(line, file), m_hResult(hr)
	{}

	char const* what() const override
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] " << GetErrorCode() << std::endl
			<< "[Description] " << HRErrors::TranslateErrorCode(m_hResult) << std::endl
			<< GetOriginString();
		m_whatBuffer = oss.str();
		return m_whatBuffer.c_str();
	}
	const char* GetType() const noexcept override
	{
		return RomanceException::GetType();
	}
	HRESULT GetErrorCode() const noexcept
	{
		return m_hResult;
	}
	std::string GetErrorString() const noexcept
	{
		return HRErrors::TranslateErrorCode(m_hResult);
	}
private:
	HRESULT m_hResult;
};