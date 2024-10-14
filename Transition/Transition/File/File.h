#pragma once
#include<string>
class FileManager;
class File
{
	friend FileManager;
protected:
	FileManager& manager_;
	int handle_=0;
	int count_=0;
	std::wstring path_=L"";
	bool isEternal_ = false;//í’“ƒtƒ‰ƒO
public:
	File(FileManager& manager);
	~File();
	virtual void Delete() = 0;
	int GetHandle()const;
};

