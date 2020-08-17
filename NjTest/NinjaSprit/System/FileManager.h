#pragma once
#include<unordered_map>
#include<string>
#include<memory>
class File;
/// <summary>
/// �t�@�C�����Ǘ�����
/// </summary>
class FileManager
{
private:
	std::unordered_map<std::wstring, std::shared_ptr<File>> resourceTable_;
	FileManager();
	//�R�s�[������s��
	FileManager(const FileManager&) = delete;
	void operator=(const FileManager&) = delete;
	void LodaData(const wchar_t* filepath);
public:
	~FileManager() = default;
	/// <summary>
	/// FileManager�̃V���O���g���C���X�^���X��Ԃ�
	/// </summary>
	/// <returns>FileManager�C���X�^���X</returns>
	static FileManager& Instance();

	std::shared_ptr<File> Load(const wchar_t* filepath,const char* tag="");
	void DeleteAllResources();
	void DeleteResourcesByTag(const char* tag);
	void Delete(std::shared_ptr<File> f);
};

