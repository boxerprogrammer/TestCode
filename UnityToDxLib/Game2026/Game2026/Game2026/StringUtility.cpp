#include "StringUtility.h"
#include<DxLib.h>
#include<cassert>
std::wstring 
StringUtility::GetWideStringFromString(const std::string& str)
{
    // MultiByteToWideChar関数は、マルチバイト文字列を
    // ワイド文字列に変換しますが、2回呼び出す必要がある関数です
    // １回目の呼び出しで、ワイド文字列のサイズを取得します
    // 1回目はサイズがわからないため、受け取るための引数にnullptrを
    // 指定して、文字列数を得る
    // 文字列数がわかったため、ワイド文字のサイズを確保しておく
    std::wstring wstr;//ワイド文字
    auto count = MultiByteToWideChar(CP_ACP, MB_COMPOSITE | MB_ERR_INVALID_CHARS,
        str.c_str(),
        str.length(),
        nullptr,// ここにnullptrを指定すると文字列数取得が目的で
                    // あることを示す。
        0);     //nullptrなので0にしておく
    wstr.resize(count);//ワイド文字列に必要な文字列数を確保
    // 2回目の呼び出しでは実際に文字列を代入します
    MultiByteToWideChar(CP_ACP, MB_COMPOSITE | MB_ERR_INVALID_CHARS,
        str.c_str(),
        str.length(),
        wstr.data(),//代入したい文字列のアドレス
        wstr.size());//代入したい文字列のサイズ
    return wstr;
}

std::string StringUtility::GetStringFromWideString(const std::wstring wstr)
{
    std::string str;
    int count = WideCharToMultiByte(CP_ACP, 0,
                                    wstr.data(), wstr.length(),
                                    nullptr, 0,
                                    nullptr, nullptr);
    assert(count > 0);
    str.resize(count);
    WideCharToMultiByte(CP_ACP, 0,
        wstr.data(), wstr.length(),
        str.data(),str.size(),
        nullptr, nullptr);

    return str;
}
