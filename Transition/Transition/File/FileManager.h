#pragma once

#include<memory>
#include<string>
#include<unordered_map>

class File;
class FileManager
{
private:
	std::unordered_map<std::wstring, std::shared_ptr<File>> fileTable_;
public:
	~FileManager();

	/// <summary>
	/// 画像ファイルをロード
	/// </summary>
	/// <param name="path">パス</param>
	/// <param name="isEternal>常駐フラグ(デフォルトfalse)</param>
	/// <returns>画像ファイルを表すファイルオブジェクト</returns>
	std::shared_ptr<File> LoadImageFile(const wchar_t* path,bool isEternal=false);

	/// <summary>
	/// サウンドファイルをロード
	/// </summary>
	/// <param name="path">パス</param>
	/// <returns>サウンドファイルを表すファイルオブジェクト</returns>
	std::shared_ptr<File> LoadSoundFile(const wchar_t* path, bool isEternal = false);

	/// <summary>
	/// モデルファイルをロード
	/// </summary>
	/// <param name="path">パス</param>
	/// <returns>モデルファイルを表すファイルオブジェクト</returns>
	std::shared_ptr<File> LoadModelFile(const wchar_t* path,bool isEternal = false);

	/// <summary>
	/// 指定のファイルを削除(基本的にそれぞれのファイルオブジェクトのデストラクタから呼ばれる)
	/// </summary>
	/// <param name="path">パス</param>
	void Delete(const std::wstring& path);
	
};

