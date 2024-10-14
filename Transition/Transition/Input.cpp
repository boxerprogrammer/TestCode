#include "Input.h"
#include<DxLib.h>
#include<cassert>
#include<algorithm>
#include<map>


enum class InputCommand {
	next,
	pause
};

bool Input::GetCurrentInput(const std::string& input) const
{
	auto it = currentState_.find(input);
	if (it == currentState_.end()) {
		return false;
	}
	return it->second;
}

bool Input::GetLastInput(const std::string& input) const
{
	auto it = lastState_.find(input);
	if (it == lastState_.end()) {
		return false;
	}
	return it->second;
}

Input::Input()
{
	//���ӂ��Q�[���̖���(������)�@
	inputTable_["next"] = {
								{InputType::keyboard,KEY_INPUT_RETURN},
								{InputType::gamepad,PAD_INPUT_R}, //PAD��start
								{InputType::mouse ,MOUSE_INPUT_LEFT} };

	
	inputTable_["cancel"] = {
								{InputType::keyboard,KEY_INPUT_ESCAPE}
							};

	inputTable_["pause"] = {
								{InputType::keyboard,KEY_INPUT_P},
								{InputType::gamepad,PAD_INPUT_L},//pad��select
								{InputType::mouse ,MOUSE_INPUT_RIGHT}
							};

	//�W�����v
	inputTable_["jump"] = {
								{InputType::keyboard,KEY_INPUT_Z},
								{InputType::gamepad,PAD_INPUT_A} //PAD��A
							};
	//�U��
	inputTable_["attack"] = {
								{InputType::keyboard,KEY_INPUT_X},
								{InputType::gamepad,PAD_INPUT_C} //PAD��X
	};


	//��
	inputTable_["up"] = {
								{InputType::keyboard,KEY_INPUT_UP},
								{InputType::gamepad,PAD_INPUT_UP} 
	};
	//��
	inputTable_["down"] = {
								{InputType::keyboard,KEY_INPUT_DOWN},
								{InputType::gamepad,PAD_INPUT_DOWN}
	};
	//��
	inputTable_["left"] = {
								{InputType::keyboard,KEY_INPUT_LEFT},
								{InputType::gamepad,PAD_INPUT_LEFT}
	};
	//�E
	inputTable_["right"] = {
								{InputType::keyboard,KEY_INPUT_RIGHT},
								{InputType::gamepad,PAD_INPUT_RIGHT}
	};





	//�L�[�R���t�B�O
	inputTable_["keyconfig"] = {
								{InputType::keyboard,KEY_INPUT_K}
	};


	LoadInputTable();

	tempTable_ = inputTable_;



	currentState_.clear();
	for (const auto& record : inputTable_) {
		currentState_[record.first] = false;
	}

	

	excludeOnConfigList_.reserve(4);
	excludeOnConfigList_.push_back("left");
	excludeOnConfigList_.push_back("right");
	excludeOnConfigList_.push_back("up");
	excludeOnConfigList_.push_back("down");

	auto padType= GetJoypadType(DX_INPUT_PAD1);

}

bool 
Input::CheckExclusiveInput() {
	char keycode[256];
	GetHitKeyStateAll(keycode);//�L�[�{�[�h����
	int padInfo = GetJoypadInputState(DX_INPUT_PAD1);//�p�b�h1�ԓ���
	int mouseInfo = GetMouseInput();//�}�E�X����

	for (const auto& exKey : excludeOnConfigList_) {

		for (const auto& input : inputTable_[exKey]) {
			switch (input.type) {
			case InputType::keyboard:
				if (keycode[input.inputID]) {
					return true;//�������͂���Ă���
				}
				break;
			case InputType::gamepad:
				if (padInfo&input.inputID) {
					return true;//�������͂���Ă���
				}
				break;
			case InputType::mouse:
				if (mouseInfo & input.inputID) {
					return true;//�������͂���Ă���
				}
				break;
			}
		}
	}
	return false;
}

void 
Input::RewriteInput(const std::string& key, const InputInfo& inputInfo) {
	auto it = tempTable_.find(key);

	if (it == tempTable_.end()) {
		assert(0 && "����Ȗ��ߓo�^����Ă܂���");
		return;
	}

	for (auto& record: it->second) {
		if (inputInfo.type == record.type) {
			record.inputID = inputInfo.inputID;
			return;
		}
	}
	//�V�K�o�^
	it->second.push_back(inputInfo);

}

void 
Input::Update()
{
	//�n�[�h�̓��͂������͑S���`�F�b�N���܂�
	char keycode[256];
	GetHitKeyStateAll(keycode);//�L�[�{�[�h����
	int padInfo=GetJoypadInputState(DX_INPUT_PAD1);//�p�b�h1�ԓ���
	int mouseInfo = GetMouseInput();//�}�E�X����

	lastState_ = currentState_;//�X�V�O�̓��e���L�^

	//���ׂĂ̓o�^�ς݂̓��͂��`�F�b�N����
	for (const auto& record : inputTable_) {
		bool pressed = false;
		for (const auto& inputInfo : record.second) {
			//���͎�ʂɂ���Ē��ו����Ⴄ
			switch (inputInfo.type) {
				case InputType::keyboard:
					pressed = keycode[inputInfo.inputID];
					break;
				case InputType::gamepad:
					pressed = padInfo & inputInfo.inputID;
					break;
				case InputType::mouse:
					pressed = mouseInfo & inputInfo.inputID;
					break;
			}
			if (pressed) {//�����厖
				break;
			}
		}
		currentState_[record.first] = pressed;
	}
}

bool Input::IsTriggered(const std::string& str) const
{
	return !GetLastInput(str) && GetCurrentInput(str);
}

bool Input::IsPressed(const std::string& str) const
{
	return GetCurrentInput(str);
}

const InputTable_t
Input::GetInputTable()const {
	auto ret = inputTable_;
	
	for (const auto& exclusion : excludeOnConfigList_) {
		ret.erase(exclusion);
	}
	return ret;
}

void
Input::SaveInputTable()
{
	FILE* fp=nullptr;
	errno_t err = fopen_s(&fp, "key.config", "wb");
	assert(err == 0);

	int size = inputTable_.size();
	fwrite(&size,sizeof(size),1,fp);

	for (const auto& record : inputTable_) {
		byte strSize = record.first.size();
		fwrite(&strSize, sizeof(strSize), 1, fp);//�R�}���h������
		fwrite(record.first.data(), sizeof(char), strSize, fp);//������
		//���͂̐�����������
		byte inputSize = record.second.size();
		fwrite(&inputSize, sizeof(inputSize), 1, fp);//���͐�
		for (const auto& inputInfo : record.second) {
			fwrite(&inputInfo, sizeof(inputInfo), 1, fp);
		}
	}
	fclose(fp);
}

void Input::LoadInputTable()
{
	
	auto handle = FileRead_open(L"key.config");
	if (handle == 0) {
		return;
	}

	int size;
	FileRead_read(&size, sizeof(size), handle);
	inputTable_.clear();
	for (int i = 0; i < size;++i) {
		std::string keyname;
		byte strSize = 0;
		FileRead_read(&strSize, sizeof(strSize), handle);//�R�}���h������
		keyname.resize(strSize);
		FileRead_read(keyname.data(), strSize,handle);//������
		//���͂̐�����������
		byte inputSize = 0;
		FileRead_read(&inputSize, sizeof(inputSize),handle);//���͐�
		for (int i = 0; i < inputSize; ++i) {
			InputInfo info = {};
			FileRead_read(&info, sizeof(info), handle);
			inputTable_[keyname].push_back(info);
		}
	}
	FileRead_close(handle);
}

void 
Input::CommitInputConfig()
{
	inputTable_=tempTable_;
}


