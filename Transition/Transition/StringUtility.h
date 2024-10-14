#pragma once

#include<string>

class StringUtility
{
public:
	static std::wstring StringToWstring(const std::string& str);
	static std::string WstringToString(const std::wstring& str);
};

