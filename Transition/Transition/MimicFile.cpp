#include "MimicFile.h"
#include<DxLib.h>
#include<cassert>

//MimicFile::MimicFile(const wchar_t* filePath)
//{
//
//}

MimicFile::~MimicFile()
{
	Close();
}

void 
MimicFile::Open(const wchar_t* filePath)
{
	auto fsize = FileRead_size(filePath);
	fileHandle_ = FileRead_open(filePath);
	data_.resize(fsize);
	if (fsize > 0) {
		OutputDebugStringA("open filesize\n");
	}
	FileRead_read(data_.data(), data_.size(), fileHandle_);
}

bool MimicFile::IsReadable()
{
	if (cursor_ >= 0) {
		return true;
	}
	auto ret = CheckHandleASyncLoad(fileHandle_);
	assert(ret != -1);
	if (ret == 0 && cursor_==-1) {
		FileRead_close(fileHandle_);
		cursor_ = 0;
	}
	return ret == 0;
}

bool MimicFile::Read(void* dstAddress, size_t size)
{
	if (!IsReadable())return false;
	std::copy_n(&data_.at(cursor_), size, (uint8_t*)dstAddress);
	cursor_ += size;
	return true;
}

void MimicFile::Close()
{
	data_.clear();
}
