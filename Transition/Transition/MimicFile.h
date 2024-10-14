#pragma once
#include<string>
#include<memory>
#include<vector>
/// <summary>
/// 非同期の時にファイルのふりをする
/// メモリ領域
/// 実際のファイルはOpen関数の時点ですべて
/// ロードされている。ロード完了とともにReadが
/// 許される、すでにロードしているので結局
/// ファイルのコピーをしているだけ
/// </summary>
class MimicFile
{
private:
	int cursor_ = -1;//ファイルの現在位置
	std::vector<uint8_t> data_;
	int fileHandle_;

public:
	MimicFile(){}
	//MimicFile(const wchar_t* filePath);
	~MimicFile();
	void Open(const wchar_t* filePath);
	bool IsReadable();
	bool Read(void* dstAddress, size_t size);
	void Close();
};

