#pragma once
#include "RomanceWin.h"
#include <string>
#include <unordered_map>

class DXGIMessageMap
{
public:
	DXGIMessageMap();
	std::string operator()(HRESULT hr, bool& bFoundCode) const;
private:
	std::unordered_map<DWORD, std::string> map;
};
