#pragma once
#include <Windows.h>
#include <vector>
#include "DXGIMessageMap.h"
#include "WindowExceptions.h"


#define GFX_EXCEPT_NOINFO(hr) GFXException(__LINE__, __FILE__, hr)
#define GFX_THROW_NOINFO(hrcall) if(HRESULT hr = hrcall; FAILED(hr)) throw GFXException(__LINE__, __FILE__, hr)

#ifdef _DEBUG  
	#define GFX_EXCEPT(hr) GFXException(__LINE__, __FILE__, hr, m_InfoManager.GetMessages())
	#define GFX_THROW_INFO(hrcall) m_InfoManager.Set(); if (HRESULT hr1 = hrcall; FAILED(hr1)) throw GFX_EXCEPT(hr1)
	#define GFX_THROW_INFO_ONLY(call) m_InfoManager.Set(); (call); {auto v = m_InfoManager.GetMessages(); if (!v.empty()) { throw GFXInfoException(__LINE__, __FILE__, v); } }
	#define GFX_DEVICE_REMOVED_EXCEPT(hrcall) if (HRESULT hr2 = hrcall; FAILED(hr2)) { if (hr2 == DXGI_ERROR_DEVICE_REMOVED) throw GFXDeviceRemovedException(__LINE__, __FILE__, pDevice->GetDeviceRemovedReason(), m_InfoManager.GetMessages()); else throw GFX_EXCEPT(hr2); }
#else 
	#define GFX_EXCEPT(hr) GFX_EXCEPT_NOINFO(hr)
	#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
	#define GFX_THROW_INFO_ONLY(call) (call)
	#define GFX_DEVICE_REMOVED_EXCEPT(hrcall) if (HRESULT hr2 = hrcall; FAILED(hr2)) { if (hr2 == DXGI_ERROR_DEVICE_REMOVED) throw GFXDeviceRemovedException(__LINE__, __FILE__, pDevice->GetDeviceRemovedReason()); else GFX_EXCEPT(hr2); }
#endif

#ifdef _DEBUG 
	#define INFOMAN() DXGIInfoManager& m_InfoManager = GetInfoManager()
#else 
	#define INFOMAN()
#endif 

/// @brief  Exception class that handles HResults
class GFXException : public RomanceException
{
public:
	GFXException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept
		: RomanceException(line, file), m_hResult(hr)
	{
		// Join all info messages with newline into a single string
		for (const auto& m : infoMsgs)
		{
			info += m;
			info.push_back('\n');
			info.push_back('\n');
		}

		// Remove final newline if it exists
		if (!info.empty())
		{
			info.pop_back();
		}
	}
	char const* what() const override
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode() << std::dec << " (" << (unsigned long)GetErrorCode() << ") " << std::endl
			<< "[Error String] " << GetErrorString()
			<< "[Description] " << HRErrors::TranslateErrorCode(m_hResult) << std::endl
			<< "[Error Info] " << std::endl << GetErrorInfo() << std::endl
			<< GetOriginString();
		m_whatBuffer = oss.str();
		return m_whatBuffer.c_str();
	}
	const char* GetType() const noexcept override
	{
		return "Romance Graphics Exception";
	}
	HRESULT GetErrorCode() const noexcept
	{
		return m_hResult;
	}
	std::string GetErrorString() const noexcept
	{
		static DXGIMessageMap messageMap;
		bool bFoundMsg;
		std::string errorString = messageMap(m_hResult, bFoundMsg);

		if (bFoundMsg) return errorString;

		return HRErrors::TranslateErrorCode(HRESULT_CODE(m_hResult));
	}
	std::string GetErrorInfo() const noexcept
	{
		return info;
	}
private:
	HRESULT m_hResult;
	std::string info;
};

/// @brief  Useful for context calls that dont return an HRESULT but can still throw a message
class GFXInfoException : public RomanceException
{
public:
	GFXInfoException(int line, const char* file, std::vector<std::string> infoMsgs = {}) noexcept
		: RomanceException(line, file)
	{
		// Join all info messages with newline into a single string
		for (const auto& m : infoMsgs)
		{
			info += m;
			info.push_back('\n');
			info.push_back('\n');
		}

		// Remove final newline if it exists
		if (!info.empty())
		{
			info.pop_back();
		}
	}
	char const* what() const override
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "\n[Error Info]\n" << GetErrorInfo() << std::endl
			<< GetOriginString();
		m_whatBuffer = oss.str();
		return m_whatBuffer.c_str();
	}
	const char* GetType() const noexcept override
	{
		return "Romance Graphics Info Exception";
	}
	std::string GetErrorInfo() const noexcept
	{
		return info;
	}
private:
	std::string info;
};

/// @brief  Specific device removed exception 
class GFXDeviceRemovedException : public GFXException
{
	using GFXException::GFXException;
public:
	const char* GetType() const noexcept override
	{
		return "Romance Exception [DEVICE REMOVED] (DXGI_ERROR_DEVICE_REMOVED)";
	}
};
