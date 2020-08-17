#pragma once
#include<string>
#include<vector>
class FileManager;
/// <summary>
/// �t�@�C�����̂��̂�\���N���X
/// </summary>
/// <note>
/// �摜�ƃT�E���h�t�@�C���Ɋւ��Ă͎��̂͂����ɂ͂Ȃ�
/// �������A�f�[�^�n�̏ꍇ�͎��̂�File�������ƂƂ���
/// </note>
class File
{
	friend FileManager;
public:
	/// <summary>
	/// �t�@�C���̎��
	/// </summary>
	enum class Type {
		image,//�摜�t�@�C��
		mask,//�}�X�N�摜�t�@�C��
		sound,//�T�E���h�t�@�C��(BGM������)
		data//����ȊO�̃f�[�^�n
	};
private:
	
	std::string tag_;//�^�O
	Type type_;//�t�@�C�����
	int handle_;//�t�@�C���n���h��(�摜�A�T�E���h�̎��Ɏg�p)
	std::vector<uint8_t>::iterator cursor_;//�f�[�^�ǂݍ��݂̍ۂ̋^���J�[�\��
	std::vector<uint8_t> data_;//�f�[�^�^�̎��ɂ̂ݎg�p
	File(int h,Type type,const char* tag="");
	void FCopyRead(void* data, size_t size);
public:
	~File();
	int Handle()const;
	std::vector<uint8_t> Data();
	bool IsLoaded()const;
	const std::string& GetTag()const;
	template<typename T>
	void CopyRead(T* data,size_t num) {
		FCopyRead(data,sizeof(T) * num);
	}
	template<typename T>
	void CopyRead(T& data) {
		FCopyRead(&data, sizeof(T));
	}
};

