#pragma once
#include <exception>
#include <sstream>

/// @brief  Base exception class which will output line number and file name of where exception was thrown
class RomanceException : public std::exception
{
public:
	RomanceException() = delete;
	RomanceException(int line, const char* file) noexcept : m_line(line), m_file(file) {}

	/// @brief  std::exception virtual override
	const char* what() const override
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< GetOriginString();
		m_whatBuffer = oss.str();
		// Have to return the char* through our buffer, if we return oss.c_str(), its lifetime ends after this scope
		// so it'll be a garbage ptr upon return
		return m_whatBuffer.c_str();
	}
	/// @brief  Custom type of exception for base classes to override and specify
	virtual const char* GetType() const noexcept
	{
		return "Romance Exception";
	}

	int GetLine() const noexcept
	{
		return m_line;
	}

	const std::string& GetFile() const noexcept
	{
		return m_file;
	}

	/// @brief  Formatted exception type with line and file, returns where this exception was thrown from
	std::string GetOriginString() const noexcept
	{
		std::ostringstream oss;
		oss << "[File] " << m_file << std::endl
			<< "[Line] " << m_line;
		return oss.str();
	}

private:
	int m_line;
	std::string m_file;
protected:
	/// @brief  Mutable as this buffer will be filled out in 'what()' which is const
	mutable std::string m_whatBuffer;
};