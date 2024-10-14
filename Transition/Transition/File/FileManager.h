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
	/// �摜�t�@�C�������[�h
	/// </summary>
	/// <param name="path">�p�X</param>
	/// <param name="isEternal>�풓�t���O(�f�t�H���gfalse)</param>
	/// <returns>�摜�t�@�C����\���t�@�C���I�u�W�F�N�g</returns>
	std::shared_ptr<File> LoadImageFile(const wchar_t* path,bool isEternal=false);

	/// <summary>
	/// �T�E���h�t�@�C�������[�h
	/// </summary>
	/// <param name="path">�p�X</param>
	/// <returns>�T�E���h�t�@�C����\���t�@�C���I�u�W�F�N�g</returns>
	std::shared_ptr<File> LoadSoundFile(const wchar_t* path, bool isEternal = false);

	/// <summary>
	/// ���f���t�@�C�������[�h
	/// </summary>
	/// <param name="path">�p�X</param>
	/// <returns>���f���t�@�C����\���t�@�C���I�u�W�F�N�g</returns>
	std::shared_ptr<File> LoadModelFile(const wchar_t* path,bool isEternal = false);

	/// <summary>
	/// �w��̃t�@�C�����폜(��{�I�ɂ��ꂼ��̃t�@�C���I�u�W�F�N�g�̃f�X�g���N�^����Ă΂��)
	/// </summary>
	/// <param name="path">�p�X</param>
	void Delete(const std::wstring& path);
	
};

