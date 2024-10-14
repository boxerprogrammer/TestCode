#include "KeyconfigScene.h"
#include"../Input.h"
#include"SceneManager.h"
#include<DxLib.h>
#include"../StringUtility.h"
#include<cassert>


void KeyconfigScene::CreateCommandStringList(const InputTable_t& table)
{
	commandStringList_.clear();
	commandStringList_.reserve(table.size());
	for (const auto& record : table) {
		commandStringList_.push_back(
					std::make_pair(
						record.first,
						StringUtility::StringToWstring(record.first)));
	}
}

void KeyconfigScene::EditInput(const std::string& key,Input& input)
{
	if (input.CheckExclusiveInput()) {
		return;
	}
	InputInfo inputInfo = {};
	//まずキーボードチェック
	for (int i = 0; i < keycode_.size(); ++i) {
		if (keycode_[i] && !lastKeycode_[i]) {
			//編集しました。
			inputInfo.type = InputType::keyboard;
			inputInfo.inputID = i;
			input.RewriteInput(key, inputInfo);
			inputTable_ = input.tempTable_;

			return;
		}
	}
	if (padInfo_ & (padInfo_ ^ lastpadInfo_)) {
		inputInfo.type = InputType::gamepad;
		inputInfo.inputID = padInfo_;
		input.RewriteInput(key, inputInfo);
		inputTable_ = input.tempTable_;

		return;
	}
	if (mouseInfo_ & (mouseInfo_ ^ lastmouseInfo_)) {
		inputInfo.type = InputType::mouse;
		inputInfo.inputID = mouseInfo_;
		input.RewriteInput(key, inputInfo);
		inputTable_ = input.tempTable_;

		return;
	}


}

KeyconfigScene::KeyconfigScene(SceneManager& manager):Scene(manager)
{
	inputTable_.clear();

	keyboardNameMap_.clear();
	keyboardNameMap_[KEY_INPUT_A] = L"Ａキー";
	keyboardNameMap_[KEY_INPUT_B] = L"Ｂキー";
	keyboardNameMap_[KEY_INPUT_C] = L"Ｃキー";
	keyboardNameMap_[KEY_INPUT_D] = L"Ｄキー";
	keyboardNameMap_[KEY_INPUT_E] = L"Ｅキー";
	keyboardNameMap_[KEY_INPUT_F] = L"Ｆキー";
	keyboardNameMap_[KEY_INPUT_G] = L"Ｇキー";
	keyboardNameMap_[KEY_INPUT_H] = L"Ｈキー";
	keyboardNameMap_[KEY_INPUT_I] = L"Ｉキー";
	keyboardNameMap_[KEY_INPUT_J] = L"Ｊキー";
	keyboardNameMap_[KEY_INPUT_K] = L"Ｋキー";
	keyboardNameMap_[KEY_INPUT_L] = L"Ｌキー";
	keyboardNameMap_[KEY_INPUT_M] = L"Ｍキー";
	keyboardNameMap_[KEY_INPUT_N] = L"Ｎキー";
	keyboardNameMap_[KEY_INPUT_O] = L"Ｏキー";
	keyboardNameMap_[KEY_INPUT_P] = L"Ｐキー";
	keyboardNameMap_[KEY_INPUT_Q] = L"Ｑキー";
	keyboardNameMap_[KEY_INPUT_R] = L"Ｒキー";
	keyboardNameMap_[KEY_INPUT_S] = L"Ｓキー";
	keyboardNameMap_[KEY_INPUT_T] = L"Ｔキー";
	keyboardNameMap_[KEY_INPUT_U] = L"Ｕキー";
	keyboardNameMap_[KEY_INPUT_V] = L"Ｖキー";
	keyboardNameMap_[KEY_INPUT_W] = L"Ｗキー";
	keyboardNameMap_[KEY_INPUT_X] = L"Ｘキー";
	keyboardNameMap_[KEY_INPUT_Y] = L"Ｙキー";
	keyboardNameMap_[KEY_INPUT_Z] = L"Ｚキー";
	keyboardNameMap_[KEY_INPUT_0] = L"０キー";
	keyboardNameMap_[KEY_INPUT_1] = L"１キー";
	keyboardNameMap_[KEY_INPUT_2] = L"２キー";
	keyboardNameMap_[KEY_INPUT_3] = L"３キー";
	keyboardNameMap_[KEY_INPUT_4] = L"４キー";
	keyboardNameMap_[KEY_INPUT_5] = L"５キー";
	keyboardNameMap_[KEY_INPUT_6] = L"６キー";
	keyboardNameMap_[KEY_INPUT_7] = L"７キー";
	keyboardNameMap_[KEY_INPUT_8] = L"８キー";
	keyboardNameMap_[KEY_INPUT_9] = L"９キー";
	keyboardNameMap_[KEY_INPUT_BACK] = L"BackSpaceキー";
	keyboardNameMap_[KEY_INPUT_TAB] = L"Tabキー";
	keyboardNameMap_[KEY_INPUT_RETURN] = L"Enterキー";
	keyboardNameMap_[KEY_INPUT_LSHIFT] = L"左Shiftキー";
	keyboardNameMap_[KEY_INPUT_RSHIFT] = L"右Shiftキー";
	keyboardNameMap_[KEY_INPUT_LCONTROL] = L"左Ctrlキー";
	keyboardNameMap_[KEY_INPUT_RCONTROL] = L"右Ctrlキー";
	keyboardNameMap_[KEY_INPUT_ESCAPE] = L"Escキー";
	keyboardNameMap_[KEY_INPUT_SPACE] = L"スペースキー";
	keyboardNameMap_[KEY_INPUT_PGUP] = L"PageUpキー";
	keyboardNameMap_[KEY_INPUT_PGDN] = L"PageDownキー";
	keyboardNameMap_[KEY_INPUT_END] = L"Endキー";
	keyboardNameMap_[KEY_INPUT_HOME] = L"Homeキー";
	keyboardNameMap_[KEY_INPUT_LEFT] = L"左キー";
	keyboardNameMap_[KEY_INPUT_UP] = L"上キー";
	keyboardNameMap_[KEY_INPUT_RIGHT] = L"右キー";
	keyboardNameMap_[KEY_INPUT_DOWN] = L"下キー";
	keyboardNameMap_[KEY_INPUT_INSERT] = L"Insertキー";
	keyboardNameMap_[KEY_INPUT_DELETE] = L"Deleteキー";
	keyboardNameMap_[KEY_INPUT_F1] = L"Ｆ１キー";
	keyboardNameMap_[KEY_INPUT_F2] = L"Ｆ２キー";
	keyboardNameMap_[KEY_INPUT_F3] = L"Ｆ３キー";
	keyboardNameMap_[KEY_INPUT_F4] = L"Ｆ４キー";
	keyboardNameMap_[KEY_INPUT_F5] = L"Ｆ５キー";
	keyboardNameMap_[KEY_INPUT_F6] = L"Ｆ６キー";
	keyboardNameMap_[KEY_INPUT_F7] = L"Ｆ７キー";
	keyboardNameMap_[KEY_INPUT_F8] = L"Ｆ８キー";
	keyboardNameMap_[KEY_INPUT_F9] = L"Ｆ９キー";
	keyboardNameMap_[KEY_INPUT_F10] = L"Ｆ１０キー";
	keyboardNameMap_[KEY_INPUT_F11] = L"Ｆ１１キー";
	keyboardNameMap_[KEY_INPUT_F12] = L"Ｆ１２キー";


	padNameMap_.clear();

	auto padType = GetJoypadType(DX_INPUT_PAD1);
	if (padType == DX_PADTYPE_XBOX_360 || padType == DX_PADTYPE_XBOX_ONE) {
		padNameMap_[PAD_INPUT_A] = L"Ａボタン";
		padNameMap_[PAD_INPUT_B] = L"Ｂボタン";
		padNameMap_[PAD_INPUT_C] = L"Ｘボタン";
		padNameMap_[PAD_INPUT_X] = L"Ｙボタン";
		padNameMap_[PAD_INPUT_Y] = L"LSボタン";
		padNameMap_[PAD_INPUT_Z] = L"RSボタン";
		padNameMap_[PAD_INPUT_L] = L"Selectボタン";
		padNameMap_[PAD_INPUT_R] = L"Startボタン";
		padNameMap_[PAD_INPUT_START] = L"左押込ボタン";
		padNameMap_[PAD_INPUT_M] = L"右押込ボタン";
	}
	else {
		padNameMap_[PAD_INPUT_A] = L"Ａボタン";
		padNameMap_[PAD_INPUT_B] = L"Ｂボタン";
		padNameMap_[PAD_INPUT_C] = L"Ｃボタン";
		padNameMap_[PAD_INPUT_X] = L"Ｘボタン";
		padNameMap_[PAD_INPUT_Y] = L"Ｙボタン";
		padNameMap_[PAD_INPUT_Z] = L"Ｚボタン";
		padNameMap_[PAD_INPUT_L] = L"Ｌボタン";
		padNameMap_[PAD_INPUT_R] = L"Ｒボタン";
		padNameMap_[PAD_INPUT_START] = L"ＳＴＡＲＴボタン";
		padNameMap_[PAD_INPUT_M] = L"Ｍボタン";
	}

}

void KeyconfigScene::Update(Input& input)
{
	GetHitKeyStateAll(keycode_.data());
	padInfo_ = GetJoypadInputState(DX_INPUT_PAD1);
	mouseInfo_ = GetMouseInput();

	if (inputTable_.empty()) {
		inputTable_ = input.GetInputTable();
		CreateCommandStringList(inputTable_);
	}

	if (input.IsTriggered("keyconfig")) {
		sceneManager_.PopScene();
	}


	
	if (input.IsTriggered("next")) {//編集中かそうでないかの切り替え
		isEditToggle_ = !isEditToggle_;
		if (cursorIndex_ == commandStringList_.size()) {
			input.CommitInputConfig();
			input.SaveInputTable();
			sceneManager_.PopScene();
			return;
		}
	}
	else {
		if (isEditToggle_) {
			if (cursorIndex_ < commandStringList_.size()) {
				auto& key = commandStringList_[cursorIndex_];
				EditInput(key.first, input);
			}
		}
	}

	if (isEditToggle_) {
		if (input.IsTriggered("cancel")) {
			if (cursorIndex_ < commandStringList_.size()) {
				auto& key = commandStringList_[cursorIndex_];
				input.tempTable_[key.first] = input.inputTable_[key.first];
				inputTable_ = input.tempTable_;
				isEditToggle_ = false;
			}
		}
	}

	if (!isEditToggle_) {
		if (input.IsTriggered("down")) {
			cursorIndex_ = (cursorIndex_ + 1) % (commandStringList_.size() + 1);
		}
		else if (input.IsTriggered("up")) {
			auto size = commandStringList_.size() + 1;
			cursorIndex_ = (size + cursorIndex_ - 1) % size;
		}
	}
	
	lastKeycode_ = keycode_;
	
	lastpadInfo_ = padInfo_;
	lastmouseInfo_ = mouseInfo_;

}

void KeyconfigScene::Draw()
{
	//地の色
	DrawBox(0, 0, 640, 480, 0x004400, true);
	//白枠
	DrawBox(0, 0, 640, 480,0xffffff, false);
	
	int x = 60;
	int y = 30;
	int count = 0;
	
	for (const auto& pair : commandStringList_) {
		x = 50;
		bool isEditting = false;
		int commandStrColor = 0xffffff;
		if (cursorIndex_ == count) {
			DrawString(30, y, L"▶", 0xff0000);
			x += 5;
			if (isEditToggle_) {
				isEditting = true;
				commandStrColor = 0xffbb88;
				x += 20;
			}
		}
		++count;

		
		//DrawFormatStringのためにワイド文字列に変換する
		//なので、ベクタをwstringにしておく
		DrawFormatString(x, y, commandStrColor,
			L"コマンド %s", pair.second.c_str());
		y += 20;
		
		auto& record = inputTable_[pair.first];
		x += 10;
		for (const auto& inputInfo : record) {
			std::wstring strKey = L"";
			switch (inputInfo.type) {
			case InputType::keyboard:
				{
					auto it = keyboardNameMap_.find(inputInfo.inputID);
					if (it == keyboardNameMap_.end()) {
						DrawFormatString(x, y, commandStrColor,
							L"KeyBD=%2x", inputInfo.inputID);
					}
					else {
						DrawFormatString(x, y, commandStrColor,
							L"KeyBD=%s", it->second.c_str());
					}
				}
				break;
			case InputType::gamepad:
			{
				std::wstring padInputName = L"";
				for (const auto& keyValue : padNameMap_) {
					if (keyValue.first & inputInfo.inputID) {
						padInputName = keyValue.second;
						break;
					}
				}
				if (padInputName == L"") {
					DrawFormatString(x, y, commandStrColor,
						L"GamePad=%2x", inputInfo.inputID);
				}
				else {
					DrawFormatString(x, y, commandStrColor,
						L"GamePad=%s", padInputName.c_str());
				}
			}
				break;
			case InputType::mouse:
				DrawFormatString(x, y, commandStrColor,
					L"Mouse=%2x", inputInfo.inputID);
				break;
			}
			x += 160;
		}
		y += 25;
	}
	if (cursorIndex_ == commandStringList_.size()) {
		DrawString(180, y+30, L"▶", 0xff0000);
	}
	DrawString(220, y+30,L"キーコンフィグ 確定",0xffffff);

	//カーソルの表示
	//アナログ入力テスト
	int analogX = 0;
	int analogY = 0;
	int result= GetJoypadAnalogInput(&analogX,&analogY, DX_INPUT_PAD1);

	DxLib::XINPUT_STATE xstate = {};
	GetJoypadXInputState(DX_INPUT_PAD1,&xstate);

	assert(result == 0);

	DrawFormatString(200, 300,  0xffffff, L"Analog x=%d, y=%d",analogX,analogY);
	DrawFormatString(200, 350, 0xffffff, L"RightLever x=%d, y=%d", xstate.ThumbRX, xstate.ThumbRY);
	std::wstring inputStr = L"";
	if (xstate.ThumbRX > 16384) {
		inputStr += L"右";
	}
	else if (xstate.ThumbRX < -16384) {
		inputStr += L"左";
	}

	if (xstate.ThumbRY > 16384) {
		inputStr += L":上";
	}
	else if (xstate.ThumbRY < -16384) {
		inputStr += L":下";
	}
	DrawFormatString(200, 450, 0xffffff, L"RightLever 状態=%s", inputStr.c_str());
	DrawFormatString(200, 400, 0xffffff, L"Trigger L=%d, R=%d", xstate.LeftTrigger, xstate.RightTrigger);

}
