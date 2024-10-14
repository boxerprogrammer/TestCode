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
	//左辺がゲームの命令(文字列)　
	inputTable_["next"] = {
								{InputType::keyboard,KEY_INPUT_RETURN},
								{InputType::gamepad,PAD_INPUT_R}, //PADのstart
								{InputType::mouse ,MOUSE_INPUT_LEFT} };

	
	inputTable_["cancel"] = {
								{InputType::keyboard,KEY_INPUT_ESCAPE}
							};

	inputTable_["pause"] = {
								{InputType::keyboard,KEY_INPUT_P},
								{InputType::gamepad,PAD_INPUT_L},//padのselect
								{InputType::mouse ,MOUSE_INPUT_RIGHT}
							};

	//ジャンプ
	inputTable_["jump"] = {
								{InputType::keyboard,KEY_INPUT_Z},
								{InputType::gamepad,PAD_INPUT_A} //PADのA
							};
	//攻撃
	inputTable_["attack"] = {
								{InputType::keyboard,KEY_INPUT_X},
								{InputType::gamepad,PAD_INPUT_C} //PADのX
	};


	//上
	inputTable_["up"] = {
								{InputType::keyboard,KEY_INPUT_UP},
								{InputType::gamepad,PAD_INPUT_UP} 
	};
	//下
	inputTable_["down"] = {
								{InputType::keyboard,KEY_INPUT_DOWN},
								{InputType::gamepad,PAD_INPUT_DOWN}
	};
	//左
	inputTable_["left"] = {
								{InputType::keyboard,KEY_INPUT_LEFT},
								{InputType::gamepad,PAD_INPUT_LEFT}
	};
	//右
	inputTable_["right"] = {
								{InputType::keyboard,KEY_INPUT_RIGHT},
								{InputType::gamepad,PAD_INPUT_RIGHT}
	};





	//キーコンフィグ
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
	GetHitKeyStateAll(keycode);//キーボード入力
	int padInfo = GetJoypadInputState(DX_INPUT_PAD1);//パッド1番入力
	int mouseInfo = GetMouseInput();//マウス入力

	for (const auto& exKey : excludeOnConfigList_) {

		for (const auto& input : inputTable_[exKey]) {
			switch (input.type) {
			case InputType::keyboard:
				if (keycode[input.inputID]) {
					return true;//無効入力されている
				}
				break;
			case InputType::gamepad:
				if (padInfo&input.inputID) {
					return true;//無効入力されている
				}
				break;
			case InputType::mouse:
				if (mouseInfo & input.inputID) {
					return true;//無効入力されている
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
		assert(0 && "そんな命令登録されてません");
		return;
	}

	for (auto& record: it->second) {
		if (inputInfo.type == record.type) {
			record.inputID = inputInfo.inputID;
			return;
		}
	}
	//新規登録
	it->second.push_back(inputInfo);

}

void 
Input::Update()
{
	//ハードの入力じたいは全部チェックします
	char keycode[256];
	GetHitKeyStateAll(keycode);//キーボード入力
	int padInfo=GetJoypadInputState(DX_INPUT_PAD1);//パッド1番入力
	int mouseInfo = GetMouseInput();//マウス入力

	lastState_ = currentState_;//更新前の内容を記録

	//すべての登録済みの入力をチェックする
	for (const auto& record : inputTable_) {
		bool pressed = false;
		for (const auto& inputInfo : record.second) {
			//入力種別によって調べ方が違う
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
			if (pressed) {//ここ大事
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
		fwrite(&strSize, sizeof(strSize), 1, fp);//コマンド文字列数
		fwrite(record.first.data(), sizeof(char), strSize, fp);//文字列
		//入力の数を書き込む
		byte inputSize = record.second.size();
		fwrite(&inputSize, sizeof(inputSize), 1, fp);//入力数
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
		FileRead_read(&strSize, sizeof(strSize), handle);//コマンド文字列数
		keyname.resize(strSize);
		FileRead_read(keyname.data(), strSize,handle);//文字列
		//入力の数を書き込む
		byte inputSize = 0;
		FileRead_read(&inputSize, sizeof(inputSize),handle);//入力数
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


