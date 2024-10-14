#include "StringUtility.h"
#include<DxLib.h>
#include<cassert>

std::wstring StringUtility::StringToWstring(const std::string& str)
{
    
    std::wstring ret;
    //��x�ڂ̌Ăяo���͕����񐔂�m�邽��
    auto result = MultiByteToWideChar(CP_UTF8,
                                    0,
                                    str.c_str(),//���͕�����
                                    str.length(),
                                    nullptr,
                                    0);
    assert(result >= 0);
    ret.resize(result);//�m�ۂ���
    //��x�ڂ̌Ăяo���͕ϊ�
    result = MultiByteToWideChar(CP_UTF8,
        0,
        str.c_str(),//���͕�����
        str.length(),
        ret.data(),
        ret.size());
    return ret;
}

std::string StringUtility::WstringToString(const std::wstring& wstr)
{
    std::string ret;
    //��x�ڂ̌Ăяo���͕����񐔂�m�邽��
    auto result = WideCharToMultiByte(
        CP_ACP,
        0,
        wstr.c_str(),//���͕�����
        wstr.length(),
        nullptr,
        0,
        nullptr,
        nullptr);
    assert(result >= 0);
    ret.resize(result);//�m�ۂ���
    //��x�ڂ̌Ăяo���͕ϊ�
    result = WideCharToMultiByte(
        CP_ACP,
        0,
        wstr.c_str(),//���͕�����
        wstr.length(),
        ret.data(),
        ret.size(),
        nullptr,
        nullptr);
    return ret;
}
