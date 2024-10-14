#include "FileManager.h"
#include"File.h"
#include"ImageFile.h"
#include"SoundFile.h"
#include"ModelFile.h"
#include<DxLib.h>
#include<cassert>


std::shared_ptr<File>
FileManager::LoadImageFile(const wchar_t* path, bool isEternal) {
	auto it = fileTable_.find(path);
	if (it == fileTable_.end()) {//もし初回ロードだったらLoadGraphでロードする
		auto handle = LoadGraph(path);
		assert(handle > 0);//ファイルが見つからない等はダメ
		if (handle <= 0) {
			return nullptr;
		}
		fileTable_[path] = std::make_shared<ImageFile>(*this);
		auto& file = fileTable_[path];
		file->handle_ = handle;
		file->count_ = 0;
		file->path_ = path;
		file->isEternal_ = isEternal;
	}
	//ここに来た時点で既にテーブル内にファイルは登録されているはず
	//テーブルのコピーを返す(コピーコンストラクタの使用)。
	// 返す際に参照カウントを１上げる
	auto& file = fileTable_[path];
	++file->count_;
	//「ImageFileのコピー」を作りたいので、ダウンキャストする
	auto imgFile = std::dynamic_pointer_cast<ImageFile>(file);
	if (imgFile == nullptr) {
		return nullptr;
	}
	return std::make_shared<ImageFile>(*imgFile);
}

std::shared_ptr<File>
FileManager::LoadSoundFile(const wchar_t* path,bool isEternal) {
	auto it = fileTable_.find(path);
	if (it == fileTable_.end()) {
		auto handle = LoadSoundMem(path);
		assert(handle > 0);//ファイルが見つからない等はダメ
		if (handle <= 0) {
			return nullptr;
		}
		fileTable_[path] = std::make_shared<SoundFile>(*this);
		auto& file = fileTable_[path];
		file->handle_ = handle;
		file->count_ = 0;
		file->path_ = path;
		file->isEternal_ = isEternal;
	}

	auto& file = fileTable_[path];
	++file->count_;
	auto sndFile = std::dynamic_pointer_cast<SoundFile>(file);
	if (sndFile == nullptr) {
		return nullptr;
	}
	return std::make_shared<SoundFile>(*sndFile);
}

std::shared_ptr<File> 
FileManager::LoadModelFile(const wchar_t* path, bool isEternal)
{
	auto it = fileTable_.find(path);
	if (it == fileTable_.end()) {
		auto handle = MV1LoadModel(path);
		assert(handle > 0);//ファイルが見つからない等はダメ
		if (handle <= 0) {
			return nullptr;
		}
		fileTable_[path] = std::make_shared<ModelFile>(*this);
		auto& file = fileTable_[path];
		file->handle_ = handle;
		file->count_ = 0;
		file->path_ = path;
		file->isEternal_ = isEternal;
	}
	auto& file = fileTable_[path];
	++file->count_;
	auto mdlFile = std::dynamic_pointer_cast<ModelFile>(file);
	if (mdlFile == nullptr) {
		return nullptr;
	}
	return std::make_shared<ModelFile>(*mdlFile);
}

void 
FileManager::Delete(const std::wstring& path)
{
	auto it = fileTable_.find(path);
	if (it == fileTable_.end()) {
		return;
	}
	auto& file = it->second;
	if (file->isEternal_) {//常駐フラグがONならばDelete無効
		return;
	}
	--file->count_;
	if (file->count_ == 0) {
		file->Delete();
		fileTable_.erase(path);
	}
	
}
FileManager::~FileManager()
{
	//残ったファイルを全て削除
	for (auto& file : fileTable_) {
		file.second->Delete();
	}
	fileTable_.clear();//不要だけど、ここでやることでデバッグしやすく
}