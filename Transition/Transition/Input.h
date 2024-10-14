#pragma once

#include<string>
#include<unordered_map>
#include<vector>

//���͎��
enum class InputType {
	keyboard,//�L�[�{�[�h
	gamepad,//�Q�[���p�b�h
	mouse//�}�E�X
};

//�o�^�������͏��(�������)
struct InputInfo {
	InputType type;//���͎��
	//���ۂɒ��ׂ�l(PAD��}�E�X�Ȃ�r�b�g��\���L�[�{�[�h�Ȃ�L�[�R�[�h)
	//��\��
	int inputID;
};

//���͂̏�Ԃ�ێ����邽�߂̍\����
//struct InputState {
//	std::string key;//�Ώۂ̓��͂�����킷����
//	bool isPressed=false;//���̑Ώۂ�������Ă邩�ǂ���
//};

//�Q�[�����̈Ӗ��Ɠ��͂��ꂽ���ǂ������y�A�ɂ�������
//�Ƃɂ����Ώۂ́u�Ӗ��v�������ꂽ��l��true�ɂȂ�^
using InputState_t = std::unordered_map<std::string, bool >;

//�Q�[������"�Ӗ�"�Ǝ��ۂ̃L�[�R�[�h�̃y�A
//�L�[(first)�͖��߂̈Ӗ��A
//�Q�[�����̈Ӗ��Ǝ��ۂ̓���(�n�[�h����)�̑Ή��\��\���u�^�v
using InputTable_t = std::unordered_map<std::string, std::vector<InputInfo>>;

class KeyconfigScene;

class Input
{
	friend KeyconfigScene;
private:

	std::vector<std::string> excludeOnConfigList_;//�L�[�R���t�B�O���O���X�g

	//�Ӗ��ƃn�[�h���͂̑Ή��\
	InputTable_t inputTable_;
	InputTable_t tempTable_;

	//�������L�^
	InputState_t currentState_ = {};
	InputState_t lastState_ = {};

	//�l�̑���ƃR�s�[�̋֎~
	Input(const Input&) = delete;
	void operator=(const Input&) = delete;

	//���݂��̃L�[��������Ă邩
	bool GetCurrentInput(const std::string& input)const;
	//���O�ɂ��̃L�[��������Ă邩
	bool GetLastInput(const std::string& input)const;
public:
	Input();
	/// <summary>
	/// ���͏����X�V����(���t���[���Ă΂��)
	/// </summary>
	void Update();

	/// <summary>
	/// ���������u�Ԃ��ǂ�����Ԃ�(�������ςȂ���false)
	/// </summary>
	/// <param name="input">���͎��ʕ�����("next"��"pause"�Ȃ�)</param>
	/// <returns>�������u�ԂȂ�true</returns>
	bool IsTriggered(const std::string& input)const;

	/// <summary>
	/// �������Ă��邩�ǂ�����Ԃ�
	/// </summary>
	/// <param name="input">���͎��ʕ�����("next"��"pause"�Ȃ�)</param>
	/// <returns>�����Ă�Ȃ�true</returns>
	bool IsPressed(const std::string& input)const;

	/// <summary>
	/// ���̓e�[�u���̏�������
	/// </summary>
	/// <param name="key">���ߕ�����</param>
	/// <param name="inputInfo">�@�킩��̓��͏��(�n�[�h)</param>
	void RewriteInput(const std::string& key,
						const InputInfo& inputInfo);

	void CommitInputConfig();

	/// <summary>
	/// ���̓e�[�u�����̂�Ԃ�
	/// </summary>
	const InputTable_t GetInputTable()const;

	void SaveInputTable();
	void LoadInputTable();

	/// <summary>
	/// ���݂̓��͂����O���͂ɂ�����̂��ǂ������`�F�b�N����
	/// </summary>
	/// <returns>true:���O���͂ɂ�����, false:���O����K�v�͂Ȃ�</returns>
	bool CheckExclusiveInput();

};

