#pragma once
#include <string>
#include <Windows.h>
#include <unordered_map>

// Useful utility to check messageproc IDs
class WindowsMessageMap
{
public:
	WindowsMessageMap();
	std::string operator()(DWORD msg, LPARAM lp, WPARAM wp) const;
private:
	std::unordered_map<DWORD, std::string> map;
};