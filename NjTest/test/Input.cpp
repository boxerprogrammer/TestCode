#include "Input.h"
#include<DxLib.h>
#include<unordered_map>
#include<string>
#include<cassert>
#include<array>

using namespace std;

namespace {
	constexpr int input_record_size = 2;///<summary>���̓o�b�t�@�i�[��</summary>
	int currentInputIndex_ = 0;///<summary>���݂̓��̓o�b�t�@���w���C���f�b�N�X</summary>
	using InputStateTable_t = unordered_map<string, bool>;
	array<InputStateTable_t,input_record_size> inputStateTable_;///<summary>���͊i�[�e�[�u��</summary>
	/// <summary>
	/// ���̓��̓o�b�t�@�C���f�b�N�X��Ԃ�
	/// </summary>
	/// <returns>���̓��̓o�b�t�@�C���f�b�N�X</returns>
	int GetNextInputBufferIndex() {
		return (static_cast<int>(currentInputIndex_) + 1) %
			static_cast<int>(inputStateTable_.size());
	}
	/// <summary>
	/// 1�t���[���O��\�����̓o�b�t�@�C���f�b�N�X��Ԃ�
	/// </summary>
	/// <returns>�P�t���[���O�̓��̓o�b�t�@�C���f�b�N�X</returns>
	size_t	GetLastInputBufferIndex() {
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
	bool& CurrentInput(const std::string cmd) {
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
	bool& LastInput(const std::string cmd) {
		auto& currentTbl = inputStateTable_[GetLastInputBufferIndex()];
		auto it = currentTbl.find(cmd);
		assert(it != currentTbl.end());
		return it->second;
	}
}

Input::Input() {
	currentInputIndex_ = 0;
	for (auto& currentTbl : inputStateTable_) {
		currentTbl["OK"] = false;
		currentTbl["pause"] = false;
		currentTbl["up"] = false;
		currentTbl["down"] = false;
		currentTbl["left"] = false;
		currentTbl["right"] = false;
		currentTbl["shot"] = false;
	}
}

void
Input::Update() {
	constexpr int keyboard_buffer_size = 256;
	char keystate[keyboard_buffer_size];
	GetHitKeyStateAll(keystate);

	currentInputIndex_ = GetNextInputBufferIndex();
	auto& currentTbl = inputStateTable_[currentInputIndex_];
	CurrentInput("OK") = keystate[KEY_INPUT_RETURN];
	CurrentInput("pause") = keystate[KEY_INPUT_P];
	CurrentInput("up") = keystate[KEY_INPUT_UP];
	CurrentInput("down") = keystate[KEY_INPUT_DOWN];
	CurrentInput("left") = keystate[KEY_INPUT_LEFT];
	CurrentInput("right") = keystate[KEY_INPUT_RIGHT];
	CurrentInput("shot") = keystate[KEY_INPUT_Z];
}

bool 
Input::IsPressed(const char* cmd)const {
	return CurrentInput(cmd);
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
