#pragma once
#include<string>
#include<memory>
#include<vector>
/// <summary>
/// �񓯊��̎��Ƀt�@�C���̂ӂ������
/// �������̈�
/// ���ۂ̃t�@�C����Open�֐��̎��_�ł��ׂ�
/// ���[�h����Ă���B���[�h�����ƂƂ���Read��
/// �������A���łɃ��[�h���Ă���̂Ō���
/// �t�@�C���̃R�s�[�����Ă��邾��
/// </summary>
class MimicFile
{
private:
	int cursor_ = -1;//�t�@�C���̌��݈ʒu
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

