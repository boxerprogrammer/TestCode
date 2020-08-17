#pragma once
#include<unordered_map>
#include<string>
#include<memory>
class File;
/// <summary>
/// ファイルを管理する
/// </summary>
class FileManager
{
private:
	std::unordered_map<std::wstring, std::shared_ptr<File>> resourceTable_;
	FileManager();
	//コピー＆代入不可
	FileManager(const FileManager&) = delete;
	void operator=(const FileManager&) = delete;
	void LodaData(const wchar_t* filepath);
public:
	~FileManager() = default;
	/// <summary>
	/// FileManagerのシングルトンインスタンスを返す
	/// </summary>
	/// <returns>FileManagerインスタンス</returns>
	static FileManager& Instance();

	std::shared_ptr<File> Load(const wchar_t* filepath,const char* tag="");
	void DeleteAllResources();
	void DeleteResourcesByTag(const char* tag);
	void Delete(std::shared_ptr<File> f);
};

