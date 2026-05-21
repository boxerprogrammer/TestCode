#include "KeyconfigScene.h"
#include"../Input.h"
#include"SceneController.h"
#include"../Application.h"
#include<DxLib.h>
#include"../StringUtility.h"

constexpr int expand_interval = 20;
constexpr int margin_size = 80;

void KeyconfigScene::AppearUpdate(Input&) {
	if (++frame_ >= expand_interval) {
		update_ = &KeyconfigScene::NormalUpdate;
		draw_ = &KeyconfigScene::NormalDraw;
	}
}

void KeyconfigScene::IntermediateUpdate(Input&) {
	GetHitKeyStateAll(lastKeyState_.data());
	lastPadState_ = GetJoypadInputState(DX_INPUT_PAD1);
	update_ = &KeyconfigScene::EditingUpdate;
	draw_ = &KeyconfigScene::EditingDraw;
}

void KeyconfigScene::NormalUpdate(Input& input) {
	if (input.IsTriggered("cancel")) {
		update_ = &KeyconfigScene::DisappearUpdate;
		draw_ = &KeyconfigScene::ExpandDraw;
	}
	if (input.IsTriggered("ok")) {
		if (selectedIndex_ < input_.configurableEventNames_.size()) {
			update_ = &KeyconfigScene::IntermediateUpdate;
			return;
		}
		else {
			auto menuIndex = selectedIndex_ - input_.configurableEventNames_.size();
			auto menuName = keyConfMenus_[menuIndex];
			keyConfExecTable_[menuName](input);
			return;
		}
	}
	int rowsNum = input_.configurableEventNames_.size()+
					keyConfMenus_.size();
	if (input.IsTriggered("up")) {
		selectedIndex_ = (selectedIndex_ + rowsNum - 1) % rowsNum;
	}
	if (input.IsTriggered("down")) {
		selectedIndex_ = (selectedIndex_ + 1) % rowsNum;
	}
}

void KeyconfigScene::DrawFrame(float rate){
	const auto& wsize = Application::GetInstance().GetWindowSize();

	const auto centerY = wsize.h / 2;//画面の真ん中のY座標
	auto height = (wsize.h - margin_size) / 2;//広がり切った時の高さ/2(真ん中から見た高さ)

	//元の画面の彩度を落とすためのセロファン
	DrawBoxAA(
		margin_size, centerY - height * rate,
		wsize.w - margin_size, centerY + height * rate,
		0x00aa00,
		true, 1.0f
	);

	//枠の描画(白枠)
	DrawBoxAA(margin_size, centerY - height * rate,
		wsize.w - margin_size, centerY + height * rate,
		0xffffff,
		false,
		3.0f);
}


void KeyconfigScene::EditingUpdate(Input& input)
{
	if (input.IsTriggered("ok")) {
		update_ = &KeyconfigScene::NormalUpdate;
		draw_ = &KeyconfigScene::NormalDraw;
		return;
	}

	std::array<char, 256> keyState;
	uint32_t padState;
	GetHitKeyStateAll(keyState.data());
	padState = GetJoypadInputState(DX_INPUT_PAD1);

	auto eventName = input_.configurableEventNames_[selectedIndex_];
	auto& inputRow = tempInputTable_[eventName];
	//キーボードチェック
	for (int i = 0; i < keyState.size();++i) {
		if (keyState[i] && !lastKeyState_[i]) {
			if (keyboardNameTable_.contains(i)) {
				for (auto& inputState : inputRow) {
					if (inputState.type == PeripheralType::keybd) {
						inputState.id = i;
						break;
					}
				}
			}
			break;
		}
	}
	//PADチェック
	uint32_t bit = 1;
	for (int i = 0; i < 32; ++i) {
		if ((padState & bit)&&!(lastPadState_&bit)) {
			if (padInputNameTable_.contains(bit)) {
				for (auto& inputState : inputRow) {
					if (inputState.type == PeripheralType::pad) {
						inputState.id = bit;
						break;
					}
				}
			}
			break;
		}
		bit <<= 1;//左ビットシフト
	}
	lastKeyState_ = keyState;
	lastPadState_ = padState;

}

void KeyconfigScene::DisappearUpdate(Input&)
{
	if (--frame_ <= 0) {
		controller_.PopScene();
		return;
	}
}

void KeyconfigScene::ExpandDraw() {
	DrawFrame(static_cast<float>(frame_) / static_cast<float>(expand_interval));
}
void KeyconfigScene::NormalDraw() {
	DrawFrame(1.0f);//枠表示

	DrawRows(false);

}

void KeyconfigScene::DrawRows(bool isEditing)
{
	constexpr int text_top = margin_size + 50;
	constexpr int text_left = margin_size + 100;
	constexpr int row_height = 30;
	// 入力情報の表示
	int y = text_top;
	for (auto& inputRow : input_.configurableEventNames_) {
		uint32_t rowCol = 0xffffff;
		int x = text_left;
		if (selectedIndex_ < input_.configurableEventNames_.size()) {
			if (input_.configurableEventNames_[selectedIndex_] == inputRow) {
				DrawString(x - 30, y, L"⇒", 0xff0000);
				rowCol = 0xddccff;
				x += 10;
				if (isEditing) {
					rowCol = 0xaa0088;
					x += 20;
				}
			}
		}

		auto eventName = StringUtility::GetWideStringFromString(inputRow);
		DrawFormatString(x, y, rowCol, L"%s", eventName.c_str());
		const auto& actualInput = tempInputTable_[inputRow];
		x += 100;
		for (auto& data : actualInput) {
			std::wstring typeName = L"";
			switch (data.type) {
			case PeripheralType::keybd:
				typeName = L"keybd";
				DrawFormatString(x, y, rowCol, L"%s:%s",
					typeName.c_str(), keyboardNameTable_[data.id].c_str());
				break;
			case PeripheralType::pad:
				typeName = L"pad";
				DrawFormatString(x, y, rowCol, L"%s:%s",
					typeName.c_str(), padInputNameTable_[data.id].c_str());
				break;
			case PeripheralType::mouse:
				typeName = L"mouse";
				break;
			}
			x += 170;
		}
		y += row_height;
	}
	
	y += row_height * 2;

	//キーコンフィグのメニューを表示します
	for (auto& menuRow : keyConfMenus_) {
		int x = text_left + 200;
		uint32_t rowCol = 0xffffff;
		if (selectedIndex_ >= input_.configurableEventNames_.size()) {
			auto menuIndex = selectedIndex_ - input_.configurableEventNames_.size();
			if (menuRow == keyConfMenus_[menuIndex]) {
				DrawString(x - 30, y, L"⇒", 0xff0000);
				rowCol = 0xddccff;
				x += 10;
			}
		}
		DrawFormatString(x, y, rowCol, L"%s", menuRow.c_str());
		y += row_height;
	}

}
void KeyconfigScene::EditingDraw()
{
	DrawFrame(1.0);
	DrawRows(true);
}

KeyconfigScene::KeyconfigScene(SceneController& controller, Input& input):
	Scene(controller),
	input_(input),
	frame_(0)
{
	update_ = &KeyconfigScene::AppearUpdate;
	draw_ = &KeyconfigScene::ExpandDraw;

	tempInputTable_ = input.inputTable_;

	keyboardNameTable_[KEY_INPUT_BACK] = L"BSキー";
	keyboardNameTable_[KEY_INPUT_TAB] = L"Tabキー";
	keyboardNameTable_[KEY_INPUT_RETURN] = L"Enterキー";
	keyboardNameTable_[KEY_INPUT_ESCAPE] = L"ESCキー";
	keyboardNameTable_[KEY_INPUT_A] = L"Ａキー";
	keyboardNameTable_[KEY_INPUT_B] = L"Ｂキー";
	keyboardNameTable_[KEY_INPUT_C] = L"Ｃキー";
	keyboardNameTable_[KEY_INPUT_D] = L"Ｄキー";
	keyboardNameTable_[KEY_INPUT_E] = L"Ｅキー";
	keyboardNameTable_[KEY_INPUT_F] = L"Ｆキー";
	keyboardNameTable_[KEY_INPUT_G] = L"Ｇキー";
	keyboardNameTable_[KEY_INPUT_H] = L"Ｈキー";
	keyboardNameTable_[KEY_INPUT_I] = L"Ｉキー";
	keyboardNameTable_[KEY_INPUT_J] = L"Ｊキー";
	keyboardNameTable_[KEY_INPUT_K] = L"Ｋキー";
	keyboardNameTable_[KEY_INPUT_L] = L"Ｌキー";
	keyboardNameTable_[KEY_INPUT_M] = L"Ｍキー";
	keyboardNameTable_[KEY_INPUT_N] = L"Ｎキー";
	keyboardNameTable_[KEY_INPUT_O] = L"Ｏキー";
	keyboardNameTable_[KEY_INPUT_P] = L"Ｐキー";
	keyboardNameTable_[KEY_INPUT_Q] = L"Ｑキー";
	keyboardNameTable_[KEY_INPUT_R] = L"Ｒキー";
	keyboardNameTable_[KEY_INPUT_S] = L"Ｓキー";
	keyboardNameTable_[KEY_INPUT_T] = L"Ｔキー";
	keyboardNameTable_[KEY_INPUT_U] = L"Ｕキー";
	keyboardNameTable_[KEY_INPUT_V] = L"Ｖキー";
	keyboardNameTable_[KEY_INPUT_W] = L"Ｗキー";
	keyboardNameTable_[KEY_INPUT_X] = L"Ｘキー";
	keyboardNameTable_[KEY_INPUT_Y] = L"Ｙキー";
	keyboardNameTable_[KEY_INPUT_Z] = L"Ｚキー";
	keyboardNameTable_[KEY_INPUT_0] = L"０キー";
	keyboardNameTable_[KEY_INPUT_1] = L"１キー";
	keyboardNameTable_[KEY_INPUT_2] = L"２キー";
	keyboardNameTable_[KEY_INPUT_3] = L"３キー";
	keyboardNameTable_[KEY_INPUT_4] = L"４キー";
	keyboardNameTable_[KEY_INPUT_5] = L"５キー";
	keyboardNameTable_[KEY_INPUT_6] = L"６キー";
	keyboardNameTable_[KEY_INPUT_7] = L"７キー";
	keyboardNameTable_[KEY_INPUT_8] = L"８キー";
	keyboardNameTable_[KEY_INPUT_9] = L"９キー";

	padInputNameTable_[PAD_INPUT_A] = L"Ａボタン";
	padInputNameTable_[PAD_INPUT_B] = L"Ｂボタン";
	padInputNameTable_[PAD_INPUT_C] = L"Ⅹボタン";
	padInputNameTable_[PAD_INPUT_X] = L"Ｙボタン";
	padInputNameTable_[PAD_INPUT_Y] = L"左ショルダー";
	padInputNameTable_[PAD_INPUT_Z] = L"右ショルダー";
	padInputNameTable_[PAD_INPUT_L] = L"SELECTボタン";
	padInputNameTable_[PAD_INPUT_R] = L"STARTボタン";
	padInputNameTable_[PAD_INPUT_START] = L"左スティック押";
	padInputNameTable_[PAD_INPUT_M] = L"右スティック押";

	keyConfMenus_ = { L"保存して閉じる",
					L"保存せず閉じる",
					L"デフォルトに戻す" };

	keyConfExecTable_[L"保存して閉じる"] = [this](Input& input) {
		//本体側に変更を反映させる
		input_.inputTable_ = tempInputTable_;
		input_.SaveInputTable();
		update_ = &KeyconfigScene::DisappearUpdate;
		draw_ = &KeyconfigScene::ExpandDraw;
		};
	keyConfExecTable_[L"保存せず閉じる"] = [this](Input& input) {
		update_ = &KeyconfigScene::DisappearUpdate;
		draw_ = &KeyconfigScene::ExpandDraw;
		};
	keyConfExecTable_[L"デフォルトに戻す"] = [this](Input& input) {
		input_.InitializeInputTable();
		tempInputTable_ = input_.inputTable_;
		};

}


void 
KeyconfigScene::Update(Input& input) {
	(this->*update_)(input);
}
void 
KeyconfigScene::Draw() {
	(this->*draw_)();
}