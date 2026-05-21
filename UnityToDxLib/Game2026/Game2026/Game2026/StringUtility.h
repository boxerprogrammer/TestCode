#pragma once
#include<string>
class StringUtility
{
public:
	/// <summary>
	/// マルチバイト文字をワイド文字(Unicode)に変換します
	/// </summary>
	/// <param name="str">マルチバイト文字列</param>
	/// <returns>ワイド文字列</returns>
	static std::wstring GetWideStringFromString(const std::string& str);
	/// <summary>
	/// ワイド文字(Unicode)をマルチバイト文字に変換します
	/// </summary>
	/// <param name="wstr">ワイド文字列</param>
	/// <returns>マルチバイト文字列</returns>
	static std::string GetStringFromWideString(const std::wstring wstr);
};

