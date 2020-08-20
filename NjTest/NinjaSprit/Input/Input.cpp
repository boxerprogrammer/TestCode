#include "Input.h"
#include<DxLib.h>
#include<cassert>

using namespace std;

/// <summary>
/// ���̓��̓o�b�t�@�C���f�b�N�X��Ԃ�
/// </summary>
/// <returns>���̓��̓o�b�t�@�C���f�b�N�X</returns>
int 
Input::GetNextInputBufferIndex() {
	return (static_cast<int>(currentInputIndex_) + 1) %
		static_cast<int>(inputStateTable_.size());
}
/// <summary>
/// 1�t���[���O��\�����̓o�b�t�@�C���f�b�N�X��Ԃ�
/// </summary>
/// <returns>�P�t���[���O�̓��̓o�b�t�@�C���f�b�N�X</returns>
size_t	
Input::GetLastInputBufferIndex()const {
	return static_cast<size_t>(
		static_cast<int>(currentInputIndex_ - 1) + 
		(inputStateTable_.size())) % 
		static_cast<int>(inputStateTable_.size());
}

/// <summary>
/// ���݂̓��͏��ւ̎Q�Ƃ�Ԃ�
/// </summary>
/// <param name="cmd">�R�}���h������</param>
/// <returns>���ꂪ�����ꂽ���ǂ���</returns>
bool& 
Input::CurrentInput(const std::string cmd) {
	auto& currentTbl = inputStateTable_[currentInputIndex_];
	auto it = currentTbl.find(cmd);
	assert(it != currentTbl.end());
	return it->second;
}

bool 
Input::GetCurrentInput(const std::string cmd)const {
	auto& currentTbl = inputStateTable_[currentInputIndex_];
	auto it = currentTbl.find(cmd);
	assert(it != currentTbl.end());
	return it->second;
}
/// <summary>
/// ���O�̓��͏��ւ̎Q�Ƃ�Ԃ�
/// </summary>
/// <param name="cmd">�R�}���h������</param>
/// <returns>���ꂪ�����ꂽ���ǂ���</returns>
bool 
Input::LastInput(const std::string cmd)const {
	auto& currentTbl = inputStateTable_[GetLastInputBufferIndex()];
	auto it = currentTbl.find(cmd);
	assert(it != currentTbl.end());
	return it->second;
}

void 
Input::RegistAcceptPeripheral(const char* eventname, const std::vector<PeripheralInfo>& peri) {
	peripheralReferenceTable_[eventname] = peri;
	for (auto& currentTbl : inputStateTable_) {
		currentTbl[eventname] = false;
	}
}

Input::Input() {

	currentInputIndex_ = 0;
	RegistAcceptPeripheral("OK", { {PeripheralType::keyboard,KEY_INPUT_RETURN}, {PeripheralType::gamepad,PAD_INPUT_R} });//R��Xbox�R����start�ɂ�����
	RegistAcceptPeripheral("pause", { {PeripheralType::keyboard,KEY_INPUT_P}, {PeripheralType::gamepad,PAD_INPUT_L} });//L��Xbox�R����select�ɂ�����
	RegistAcceptPeripheral("cancel", { {PeripheralType::keyboard,KEY_INPUT_ESCAPE}, {PeripheralType::gamepad,PAD_INPUT_Z} });//�E�V�����_�[
	RegistAcceptPeripheral("up", { {PeripheralType::keyboard,KEY_INPUT_UP}, {PeripheralType::gamepad,PAD_INPUT_UP} });
	RegistAcceptPeripheral("down", { {PeripheralType::keyboard,KEY_INPUT_DOWN}, {PeripheralType::gamepad,PAD_INPUT_DOWN} });
	RegistAcceptPeripheral("left", { {PeripheralType::keyboard,KEY_INPUT_LEFT}, {PeripheralType::gamepad,PAD_INPUT_LEFT} });
	RegistAcceptPeripheral("right", { {PeripheralType::keyboard,KEY_INPUT_RIGHT}, {PeripheralType::gamepad,PAD_INPUT_RIGHT} });
	RegistAcceptPeripheral("shot", { {PeripheralType::keyboard,KEY_INPUT_Z}, {PeripheralType::gamepad,PAD_INPUT_C} });//X�R��X
	RegistAcceptPeripheral("jump", { {PeripheralType::keyboard,KEY_INPUT_X}, {PeripheralType::gamepad,PAD_INPUT_A} });//X�R��A
	RegistAcceptPeripheral("change", { {PeripheralType::keyboard,KEY_INPUT_C}, {PeripheralType::gamepad,PAD_INPUT_X} });//X�R��Y
}

bool 
Input::CheckPressed(const char* eventname, const char* keystate, int padstate) {
	bool ret = false;
	for (auto& p : peripheralReferenceTable_[eventname]) {
		if (p.type == PeripheralType::keyboard) {
			ret = ret||keystate[p.inputIdx];
		}
		else if (p.type == PeripheralType::gamepad) {
			ret = ret||(padstate&p.inputIdx);
		}
	}
	return ret;
}
void 
Input::SetPeripheralReferenceTable(const PeripheralReferenceTable_t& p)const {
	//���������X�V����
	//const�֐��ł͂��邪�AKeyconfigScene�̂Ƃ��ɂ̂�
	//mutable�Ƃ��čX�V����
	for (const auto& ref : p) {
		peripheralReferenceTable_[ref.first] = ref.second;
	}
}
void 
Input::UnlockRawMode()const {
	//���̊֐���const�����Amutable�ϐ��݂̂�
	//�ҏW�\�Ƃ��Ă���(friend��KeyconfigScene�ɑ΂��Ă̂�)
	isRawMode_ = true;	
	rawkeystate_.resize(256);
}
void 
Input::LockRawMode()const {
	//���̊֐���const�����Amutable�ϐ��݂̂�
	//�ҏW�\�Ƃ��Ă���(friend��KeyconfigScene�ɑ΂��Ă̂�)
	isRawMode_ = false;
	rawpadstate_ = 0;
	rawkeystate_.clear();
}
const std::vector<char>& 
Input::GetRawKeyboardState()const {
	return rawkeystate_;
}
const int 
Input::GetRawPadState()const {
	return rawpadstate_;
}

void
Input::Update() {
	constexpr int keyboard_buffer_size = 256;
	char keystate[keyboard_buffer_size];
	int pad1 = 0;
	if (GetJoypadNum() > 0) {
		pad1=DxLib::GetJoypadInputState(DX_INPUT_PAD1);
	}
	GetHitKeyStateAll(keystate);
	if (isRawMode_) {
		rawpadstate_ = pad1;
		copy(begin(keystate), end(keystate), rawkeystate_.begin());
	}
	currentInputIndex_ = GetNextInputBufferIndex();
	auto& currentTbl = inputStateTable_[currentInputIndex_];

	for (auto& periRef : peripheralReferenceTable_) {
		RecordInput(periRef.first.c_str(), keystate, pad1);
	}
}

void Input::RecordInput(const char* eventname, char  keystate[256], int pad1)
{
	CurrentInput(eventname) = CheckPressed(eventname, keystate, pad1);
}

bool 
Input::IsPressed(const char* cmd)const {

	return GetCurrentInput(cmd);
}



bool 
Input::IsTriggered(const char* cmd)const {
	return (IsPressed(cmd) && !LastInput(cmd));
}

bool 
Input::IsReleased(const char* cmd)const {
	auto lastIdx = GetLastInputBufferIndex();
	auto& lastTbl = inputStateTable_[lastIdx];
	return (!IsPressed(cmd) && LastInput(cmd));
}
