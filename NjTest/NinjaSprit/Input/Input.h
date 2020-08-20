#pragma once
#include<string>
#include<array>
#include<vector>
#include<unordered_map>
constexpr int input_record_size = 2;///<summary>���̓o�b�t�@�i�[��</summary>
/// <summary>
/// ���Ӌ@����
/// </summary>
enum class PeripheralType {
	keyboard,///<�L�[�{�[�h
	gamepad,///<�Q�[���p�b�h
	xboxpad,///<xbox�p�b�h
};
/// <summary>
/// ���Ӌ@����͏��
/// </summary>
struct PeripheralInfo {
	PeripheralType type;///<�@����
	int inputIdx;///<���͂����ʂ��邽�߂̐��l
};
using PeripheralReferenceTable_t = std::unordered_map < std::string, std::vector<PeripheralInfo>>;
class KeyconfigScene;
class Input
{
	friend KeyconfigScene;
private:
	mutable std::vector<char> rawkeystate_;//���L�[�{�[�h�X�e�[�g
	mutable int rawpadstate_;//���p�b�h�X�e�[�g(1�R���̂�)
	mutable bool isRawMode_ = false;//�����[�h
	/// <summary>
	/// �����[�h���A�����b�N����
	/// ���̃A�����b�N�ɂ���Ēʏ�̓��[�J���ɂ̂ݑ��݂���
	/// ���L�[�{�[�h�A���p�b�h��񂪕ێ�����Q�Ƃł���悤�ɂȂ�
	/// </summary>
	void UnlockRawMode()const;

	/// <summary>
	/// �����[�h�����b�N����
	/// ���̃��[�h�Ő��f�[�^�ɃA�N�Z�X���悤�Ƃ���΃A�T�[�V�������N����
	/// </summary>
	void LockRawMode()const;
	/// <summary>
	/// ���L�[�{�[�h���������Ă���
	/// �A�����b�N���͒��g����
	/// </summary>
	/// <returns>���L�[�{�[�h���z��</returns>
	const std::vector<char>& GetRawKeyboardState()const;

	/// <summary>
	/// ���p�b�h���������Ă���
	/// �������P�Ԃ̂�
	/// </summary>
	/// <returns>���p�b�h���</returns>
	const int GetRawPadState()const;
	
	mutable PeripheralReferenceTable_t peripheralReferenceTable_;///�O�����炢�����p
	/// <summary>
	/// �O��������̓e�[�u�����������p�̊֐�
	/// </summary>
	/// <param name="prt">�ύX���s��ꂽ���̓e�[�u��(�ꕔ)</param>
	void SetPeripheralReferenceTable(const PeripheralReferenceTable_t& prt)const;

	int currentInputIndex_ = 0;///<summary>���݂̓��̓o�b�t�@���w���C���f�b�N�X</summary>
	using InputStateTable_t = std::unordered_map<std::string, bool>;
	std::array<InputStateTable_t, input_record_size> inputStateTable_;///<summary>���͊i�[�e�[�u��</summary>
	/// <summary>
	/// ���̓��̓o�b�t�@�C���f�b�N�X��Ԃ�
	/// </summary>
	/// <returns>���̓��̓o�b�t�@�C���f�b�N�X</returns>
	int GetNextInputBufferIndex();
	/// <summary>
	/// 1�t���[���O��\�����̓o�b�t�@�C���f�b�N�X��Ԃ�
	/// </summary>
	/// <returns>�P�t���[���O�̓��̓o�b�t�@�C���f�b�N�X</returns>
	size_t	GetLastInputBufferIndex()const;
	/// <summary>
	/// ���݂̓��͏��ւ̎Q�Ƃ�Ԃ�
	/// </summary>
	/// <param name="cmd">�R�}���h������</param>
	/// <returns>���ꂪ�����ꂽ���ǂ���</returns>
	bool& CurrentInput(const std::string cmd);
	bool GetCurrentInput(const std::string cmd)const;
	/// <summary>
	/// ���O�̓��͏��ւ̎Q�Ƃ�Ԃ�
	/// </summary>
	/// <param name="cmd">�R�}���h������</param>
	/// <returns>���ꂪ�����ꂽ���ǂ���</returns>
	bool LastInput(const std::string cmd)const;

	/// <summary>
	/// �C�x���g�ɑΉ�����@����͂�o�^����
	/// </summary>
	/// <param name="eventname">���̓C�x���g��</param>
	/// <param name="peri">�@����͏��z��(�x�N�^)</param>
	void RegistAcceptPeripheral(const char* eventname, const std::vector<PeripheralInfo>& peri);
	/// <summary>
	/// ���݂��ꂪ������Ă邩�`�F�b�N����
	/// </summary>
	/// <param name="eventname">���̓C�x���g��</param>
	/// <param name="keystate">�L�[�{�[�h���</param>
	/// <param name="padstate">�p�b�h���</param>
	/// <returns>true:������Ă� / false:������ĂȂ�</returns>
	bool CheckPressed(const char* eventname,const char* keystate,int padstate);
public:
	Input();
	/// <summary>
	/// ���͓��e���X�V����
	/// </summary>
	void Update();

	void RecordInput(const char* eventname, char  keystate[256], int pad1);

	/// <summary>
	/// �����Ă邩�ǂ�����Ԃ�
	/// </summary>
	/// <param name="cmd">�R�}���h������</param>
	/// <returns>������Ă�</returns>
	bool IsPressed(const char* cmd)const;

	/// <summary>
	/// �����ꂽ�u�ԂȂ̂��ǂ�����Ԃ�
	/// </summary>
	/// <param name="cmd">�R�}���h������</param>
	/// <returns>
	/// ��������ĂȂ�:false
	/// ��������Ē��O��������Ă�:false
	/// ��������āA���O�͉�����ĂȂ�:true
	///</returns>
	bool IsTriggered(const char* cmd)const;

	/// <summary>
	/// �����ꂽ�u�ԂȂ̂��ǂ�����Ԃ�
	/// </summary>
	/// <param name="cmd">�R�}���h������</param>
	/// <returns>
	/// ��������Ă�:false
	/// ��������ĂȂ����O��������Ȃ�:false
	/// ���O������āA��������ĂȂ�:true
	///</returns>
	bool IsReleased(const char* cmd)const;
};
///���͒ʒm���󂯎��(�C���^�[�t�F�[�X)
class InputListener {
public:
	virtual ~InputListener() = default;
	///�Ԃ����Ⴏ���b���t���[���Ă΂��
	///���������͂��󂯎��
	virtual void Notify(const Input& input) = 0;
};