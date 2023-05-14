#pragma once

#include<map>
#include<vector>
#include<string>

//�Q�[�����Ŏg�p����鉼�z���͏��
enum class InputType {
	next,///���֍s���{�^��
	prev,///�O�ɖ߂�{�^��
	pause,///�|�[�Y�{�^��
	keyconf,///�L�[�R���t�B�O
	change,///�ύX
	up,///��
	down,///��
	left,///��
	right,///��
	shot,///�e����
	switching,//�t�H�[�X�̐؂藣���A�z�����݂̐؂�ւ�
	slow,//�Q�[�����̓������X���[���[�V�����ɂ���
	max//�ő���̓C���f�b�N�X
};

/// <summary>
/// ���͑��u�J�e�S��
/// </summary>
enum class InputCategory {
	keybd,//�L�[�{�[�h
	pad,//�Q�[���p�b�h
	mouse//�}�E�X
};

/// <summary>
/// ���͏��
/// </summary>
struct InputInfo{
	InputCategory cat;//���͑��u�J�e�S��
	int id;//����ID(KEY_INPUT_�`,PAD_INPUT_�`,MOUSE_INPUT_�`)
};

//�t�����h�p�Ƀv���g�^�C�v�錾
class KeyConfigScene;

/// <summary>
/// ���͏�Ԃ��Ǘ�����
/// </summary>
class InputState {
	//KeyConfigScene�ɂ����A���ׂĂ�������B
	friend KeyConfigScene;
private:
	//���z���͏��ƁA���ۂ̓��͂̃e�[�u�������
	//�L�[(first)=InputType
	//�l(second)=std::vector<InputInfo>
	using InputMap_t = std::map< InputType, std::vector<InputInfo> >;
	InputMap_t inputMapTable_;//���ۂ̓��͂ƃQ�[���{�^���̑Ή��e�[�u��
	
	//���������p�̈ꎞ�I��inputMapTable_�̃R�s�[
	//�����Ȃ菑�������Ă��܂�Ȃ��悤��
	InputMap_t tempMapTable_;

	//���Z�b�g�p�L�[�}�b�v�e�[�u��
	InputMap_t defaultMapTable_;

	//���̓^�C�v�Ƃ��̖��O�̑Ή��e�[�u��
	std::map<InputType, std::wstring> inputNameTable_;

	std::vector<bool> currentInput_;//���݂̓��͏��(�����Ă邩�����ĂȂ���)
	std::vector<bool> lastInput_;//���O�̓��͏��(���O�����Ă邩�����ĂȂ���)

public:
	InputState();

	/// <summary>
	/// �������u�Ԃ�true�ɂȂ�
	/// </summary>
	/// <param name="type">InputType</param>
	/// <returns>�������u�ԂȂ�true,����ȊO�Ȃ�false</returns>
	bool IsTriggered(InputType type)const;

	/// <summary>
	/// ������Ă���true�ɂȂ�
	/// </summary>
	/// <param name="type">InputType</param>
	/// <returns>������Ă���true,������ĂȂ��Ȃ�false</returns>
	bool IsPressed(InputType type)const;
	/// <summary>
	/// ���͏����X�V����
	/// (����)���t���[��Update���Ă΂Ȃ��Ɠ��͏�Ԃ�
	/// �X�V����܂���B
	/// </summary>
	void Update();

	/// <summary>
	/// ���͏����X�V����
	/// </summary>
	/// <param name="type">�Q�[���ɂ�������͎��</param>
	/// <param name="cat">���̓J�e�S��(keybd��)</param>
	/// <param name="id">���ۂ̓���</param>
	void RewriteInputInfo(InputType type , InputCategory cat , int id);

	/// <summary>
	/// ���ݕҏW���̃L�[�R���t�B�O���m�肷��
	/// </summary>
	void CommitChangedInputInfo();
	
	/// <summary>
	/// ���ݕҏW���̃L�[�R���t�B�O�̕ύX���Ȃ��������Ƃɂ���
	/// </summary>
	void RollbackChangedInputInfo();

	/// <summary>
	/// �L�[�}�b�v���f�t�H���g�Ƀ��Z�b�g����
	/// </summary>
	void ResetInputInfo();

	/// <summary>
	/// �L�[�}�b�v���t�@�C���ɕۑ�����
	/// </summary>
	void SaveKeyInfo()const;

	void LoadKeyInfo();

};