#include "ResourceManager.h"

#include<DxLib.h>
#include<cassert>

int ResourceManager::LoadGraph(const std::wstring& path)
{
    int handle = DxLib::LoadGraph(path.c_str());
    assert(handle >= 0);
    return handle;
}
