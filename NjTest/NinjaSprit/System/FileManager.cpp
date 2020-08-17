#include "FileManager.h"
#include<DxLib.h>
#include<cassert>
#include<algorithm>
#include<functional>
#include"File.h"

using namespace std;

namespace {
	/// <summary>
	/// 拡張子を返す
	/// </summary>
	/// <param name="filepath">ファイルパス</param>
	/// <returns>拡張子文字列</returns>
	wstring GetExtension(const wstring& filepath) {
		auto idx=filepath.rfind(L'.');
		assert(idx != wstring::npos);
		++idx;
		return filepath.substr(idx, filepath.length() - idx);
	}
	struct HandleAndType {
		int handle;//ハンドル
		File::Type type;//ファイル種別
	};
	std::unordered_map<wstring, std::function<HandleAndType(const wchar_t*)>> loader_;

}

void
FileManager::LodaData(const wchar_t* path) {

}

FileManager::FileManager() {
	loader_[L"bmp"] = [](const wchar_t* path)->HandleAndType {
		return { LoadMask(path),File::Type::mask };
	};
	loader_[L"jpg"] = loader_[L"png"] = [](const wchar_t* path)->HandleAndType {
		return {LoadGraph(path),File::Type::image };
	};
	loader_[L"mp3"]=loader_[L"wav"] = [](const wchar_t* path)->HandleAndType {
		return { LoadSoundMem(path),File::Type::sound};
	};
	loader_[L"fmf"]= [this](const wchar_t* path)->HandleAndType {
		return { FileRead_open(path),File::Type::data  };
	};
}

FileManager&
FileManager::Instance() {
	static FileManager instance;
	return instance;
}

shared_ptr<File>
FileManager::Load(const wchar_t* filepath,const char* tag) {
	assert(DxLib_IsInit());
	if (resourceTable_.count(filepath) == 0) {
		auto ext = GetExtension(filepath);
#ifdef _DEBUG
		assert(loader_.count(ext) > 0);
#endif
		auto file=loader_[ext](filepath);
		resourceTable_[filepath].reset(new File(file.handle,file.type,tag));
	}
	return resourceTable_[filepath];
}
void
FileManager::DeleteAllResources() {
	resourceTable_.clear();
}

void 
FileManager::DeleteResourcesByTag(const char* tag) {
	assert(tag!=nullptr);
	auto it = resourceTable_.begin();
	while (it != resourceTable_.end()) {
		if (it->second->GetTag() == tag) {
			resourceTable_.erase(it);
		}
		else {
			++it;
		}
	}
}
void 
FileManager::Delete(std::shared_ptr<File> f) {
	auto it = find_if(resourceTable_.begin(), resourceTable_.end(), 
		[f](const pair<wstring,shared_ptr<File>>& res) {
			return res.second == f;
		});
	if (it == resourceTable_.end())return;
	resourceTable_.erase(it);
}