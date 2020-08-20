#include "KeyconfigScene.h"
#include"SceneController.h"

#include<string>
#include<algorithm>
#include<DxLib.h>
#include"../Arithmetic.h"
#include"../System/File.h"
#include"../System/FileManager.h"
#include"../Arithmetic.h"
using namespace std;

namespace {
	wstring vkeyname[256] = {};
	void InitVkeyName() {
		vkeyname[KEY_INPUT_RETURN] = L"ENTERキー";
		vkeyname[KEY_INPUT_ESCAPE] = L"ESCキー";
		vkeyname[KEY_INPUT_0] = L"０キー";
		vkeyname[KEY_INPUT_1] = L"１キー";
		vkeyname[KEY_INPUT_2] = L"２キー";
		vkeyname[KEY_INPUT_3] = L"３キー";
		vkeyname[KEY_INPUT_4] = L"４キー";
		vkeyname[KEY_INPUT_5] = L"５キー";
		vkeyname[KEY_INPUT_6] = L"６キー";
		vkeyname[KEY_INPUT_7] = L"７キー";
		vkeyname[KEY_INPUT_8] = L"８キー";
		vkeyname[KEY_INPUT_9] = L"９キー";
		vkeyname[KEY_INPUT_A] = L"Ａキー";
		vkeyname[KEY_INPUT_B] = L"Ｂキー";
		vkeyname[KEY_INPUT_C] = L"Ｃキー";
		vkeyname[KEY_INPUT_D] = L"Ｄキー";
		vkeyname[KEY_INPUT_E] = L"Ｅキー";
		vkeyname[KEY_INPUT_F] = L"Ｆキー";
		vkeyname[KEY_INPUT_G] = L"Ｇキー";
		vkeyname[KEY_INPUT_H] = L"Ｈキー";
		vkeyname[KEY_INPUT_I] = L"Ｉキー";
		vkeyname[KEY_INPUT_J] = L"Ｊキー";
		vkeyname[KEY_INPUT_K] = L"Ｋキー";
		vkeyname[KEY_INPUT_L] = L"Ｌキー";
		vkeyname[KEY_INPUT_M] = L"Ｍキー";
		vkeyname[KEY_INPUT_N] = L"Ｎキー";
		vkeyname[KEY_INPUT_O] = L"Ｏキー";
		vkeyname[KEY_INPUT_P] = L"Ｐキー";
		vkeyname[KEY_INPUT_Q] = L"Ｑキー";
		vkeyname[KEY_INPUT_R] = L"Ｒキー";
		vkeyname[KEY_INPUT_S] = L"Ｓキー";
		vkeyname[KEY_INPUT_T] = L"Ｔキー";
		vkeyname[KEY_INPUT_U] = L"Ｕキー";
		vkeyname[KEY_INPUT_V] = L"Ｖキー";
		vkeyname[KEY_INPUT_W] = L"Ｗキー";
		vkeyname[KEY_INPUT_X] = L"Ｘキー";
		vkeyname[KEY_INPUT_Y] = L"Ｙキー";
		vkeyname[KEY_INPUT_Z] = L"Ｚキー";
		vkeyname[KEY_INPUT_BACK] = L"BackSpaceキー";
		vkeyname[KEY_INPUT_TAB] = L"Tabキー";
		vkeyname[KEY_INPUT_LSHIFT] = L"左Shiftキー";
		vkeyname[KEY_INPUT_RSHIFT] = L"右Shiftキー";
		vkeyname[KEY_INPUT_LCONTROL] = L"左Ctrlキー";
		vkeyname[KEY_INPUT_RCONTROL] = L"右Ctrlキー";
		vkeyname[KEY_INPUT_ESCAPE] = L"Escキー";
		vkeyname[KEY_INPUT_SPACE] = L"スペースキー";
		vkeyname[KEY_INPUT_PGUP] = L"PageUpキー";
		vkeyname[KEY_INPUT_PGDN] = L"PageDownキー";
		vkeyname[KEY_INPUT_END] = L"Endキー";
		vkeyname[KEY_INPUT_HOME] = L"Homeキー";
		vkeyname[KEY_INPUT_LEFT] = L"左キー";
		vkeyname[KEY_INPUT_UP] = L"上キー";
		vkeyname[KEY_INPUT_RIGHT] = L"右キー";
		vkeyname[KEY_INPUT_DOWN] = L"下キー";
		vkeyname[KEY_INPUT_INSERT] = L"Insertキー";
		vkeyname[KEY_INPUT_DELETE] = L"Deleteキー";
		vkeyname[KEY_INPUT_F1] = L"Ｆ１キー";
		vkeyname[KEY_INPUT_F2] = L"Ｆ２キー";
		vkeyname[KEY_INPUT_F3] = L"Ｆ３キー";
		vkeyname[KEY_INPUT_F4] = L"Ｆ４キー";
		vkeyname[KEY_INPUT_F5] = L"Ｆ５キー";
		vkeyname[KEY_INPUT_F6] = L"Ｆ６キー";
		vkeyname[KEY_INPUT_F7] = L"Ｆ７キー";
		vkeyname[KEY_INPUT_F8] = L"Ｆ８キー";
		vkeyname[KEY_INPUT_F9] = L"Ｆ９キー";
		vkeyname[KEY_INPUT_F10] = L"Ｆ１０キー";
		vkeyname[KEY_INPUT_F11] = L"Ｆ１１キー";
		vkeyname[KEY_INPUT_F12] = L"Ｆ１２キー";
	}
	constexpr int keyconfig_title_y = 32;//ポーズタイトルのY
	constexpr int menubase_x = 64;//メニューの左
	constexpr int menubase_y = 64;//メニューの一番上のY
	constexpr int menu_y_interval = 32;//メニューの間隔
	const wchar_t keyconfig_title[] = L"Key Config";
	
	///string(マルチバイト文字列)からwstring(ワイド文字列)を得る
///@param str マルチバイト文字列
///@return 変換されたワイド文字列
	std::wstring
		GetWideStringFromString(const std::string& str) {
		//呼び出し1回目(文字列数を得る)
		auto num1 = MultiByteToWideChar(CP_ACP,
			MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
			str.c_str(), -1, nullptr, 0);

		std::wstring wstr;//stringのwchar_t版
		wstr.resize(num1);//得られた文字列数でリサイズ

		//呼び出し2回目(確保済みのwstrに変換文字列をコピー)
		auto num2 = MultiByteToWideChar(CP_ACP,
			MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
			str.c_str(), -1, &wstr[0], num1);

		assert(num1 == num2);//一応チェック
		return wstr;
	}

}
KeyconfigScene::KeyconfigScene(SceneController& c, const Vector2& offset) :
	Scene(c),
	updater_(&KeyconfigScene::InitializeUpdate),
	drawer_(&KeyconfigScene::NormalDraw) {
	InitVkeyName();
	frameRect_.pos += offset;
	auto& fileMgr = FileManager::Instance();
	titleW_ = static_cast<size_t>(DxLib::GetDrawStringWidth(keyconfig_title, static_cast<int>(wcslen(keyconfig_title))));
	openSE_ = fileMgr.Load(L"Resource/Sound/System/menuopen.wav")->Handle();
	closeSE_ = fileMgr.Load(L"Resource/Sound/System/menuclose.wav")->Handle();
	moveSE_ = fileMgr.Load(L"Resource/Sound/System/cursor.wav")->Handle();
	decideSE_ = fileMgr.Load(L"Resource/Sound/System/decide.wav")->Handle();
	cancelSE_ = fileMgr.Load(L"Resource/Sound/System/cancel.wav")->Handle();
	changeSE_ = fileMgr.Load(L"Resource/Sound/System/change.wav")->Handle();
}

void 
KeyconfigScene::CommitCurrentKeyConfig(const Input& input) {
	input.SetPeripheralReferenceTable(peripheralReferenceTable_);
}



void 
KeyconfigScene::CancelEdit(const Input& input) {
	peripheralReferenceTable_.clear();
	if (peripheralReferenceTable_.empty()) {
		string removableEvents[] = { "up","down","left","right" };
		for (const auto& ref : input.peripheralReferenceTable_) {
			if (count(begin(removableEvents), end(removableEvents), ref.first) == 0) {
				peripheralReferenceTable_.insert(ref);
			}
		}
		for (const auto& ref : peripheralReferenceTable_) {
			menuItems_.push_back(ref.first);
		}
		menuItems_.push_back("commit");
	}
}

//最初のUpdate
void
KeyconfigScene::InitializeUpdate(const Input& input) {
	if (peripheralReferenceTable_.empty()) {
		string removableEvents[] = { "up","down","left","right" };
		for (const auto& ref : input.peripheralReferenceTable_) {
			if (count(begin(removableEvents), end(removableEvents), ref.first) == 0) {
				peripheralReferenceTable_.insert(ref);
			}
		}
		for (const auto& ref : peripheralReferenceTable_) {
			menuItems_.push_back(ref.first);
		}
		menuItems_.push_back("commit");
	}
	updater_ = &KeyconfigScene::NormalUpdate;
}

//キーコンフィグ編集中Update
void 
KeyconfigScene::EditUpdate(const Input& input) {
	if (input.IsTriggered("OK")) {
		PlaySoundMem(decideSE_, DX_PLAYTYPE_BACK);
		input.LockRawMode();
		updater_ = &KeyconfigScene::NormalUpdate;
	}else if(input.IsTriggered("cancel")) {
		PlaySoundMem(cancelSE_, DX_PLAYTYPE_BACK);
		CancelEdit(input);
		input.LockRawMode();
		updater_ = &KeyconfigScene::NormalUpdate;
	}
	else {
		//もし、OKもしくはキャンセル以外の入力があればそれは更新したい入力のはず
		auto& ref = peripheralReferenceTable_[menuItems_[currentSelectNo_]];
		for (auto& p : ref) {
			if (p.type == PeripheralType::keyboard) {
				const auto& rawkeystate = input.GetRawKeyboardState();
				for (int i = 0; i < rawkeystate.size(); ++i) {
					if (rawkeystate[i] && vkeyname[i] != L"") {
						if (i != p.inputIdx) {
							PlaySoundMem(changeSE_, DX_PLAYTYPE_BACK);
							p.inputIdx = i;
						}
						break;
					}
				}
			}
			else if(p.type==PeripheralType::gamepad){
				if (input.rawpadstate_ != 0) {
					if (p.inputIdx != input.rawpadstate_) {
						PlaySoundMem(changeSE_, DX_PLAYTYPE_BACK);
						p.inputIdx = input.rawpadstate_;
					}
				}
			}
		}
	}
}

//通常(編集中でない)Update
void
KeyconfigScene::NormalUpdate(const Input& input){
	if (input.IsTriggered("down")) {
		PlaySoundMem(moveSE_, DX_PLAYTYPE_BACK);
		currentSelectNo_ = ModuloIncrement(currentSelectNo_, menuItems_.size());
	}
	if (input.IsTriggered("up")) {
		PlaySoundMem(moveSE_, DX_PLAYTYPE_BACK);
		currentSelectNo_ = ModuloDecrement(currentSelectNo_, menuItems_.size());
	}
	if (input.IsTriggered("OK")) {
		PlaySoundMem(decideSE_, DX_PLAYTYPE_BACK);
		if (menuItems_[currentSelectNo_] == "commit") {
			CommitCurrentKeyConfig(input);
			CloseMenu();
		}
		else {
			updater_ = &KeyconfigScene::EditUpdate;
			input.UnlockRawMode();
		}
	}
	if (input.IsTriggered("cancel")) {
		PlaySoundMem(cancelSE_, DX_PLAYTYPE_BACK);
		CloseMenu();
	}
}
void
KeyconfigScene::NormalDraw(){
	auto& rc = frameRect_;
	DrawBox(rc.Left(), rc.Top(), rc.Right(), rc.Bottom(), 0x114411, true);
	DrawBoxAA(rc.Left(), rc.Top(), rc.Right(), rc.Bottom(), 0xffffff, false, 3);
	int y = rc.Top() + keyconfig_title_y;
	//ポーズのタイトル
	DrawString(2+rc.Left() + static_cast<int>((rc.Width() - titleW_) / 2),
		2+y,
		keyconfig_title, 0x000000);
	DrawString(rc.Left() + static_cast<int>((rc.Width() - titleW_) / 2),
		y,
		keyconfig_title, 0xffaaaa);

	
	uint32_t mcolor = 0xffffff;
	y = rc.Top() + menubase_y;
	for (auto& periRef : peripheralReferenceTable_) {
		int xoffset = 100;
		if (periRef.first == menuItems_[currentSelectNo_]) {
			if (updater_==&KeyconfigScene::EditUpdate) {
				xoffset += 10;
				mcolor = 0xffffaa;
			}
			else {
				mcolor = 0xaabbff;
			}
		}
		else {
			mcolor = 0xffffff;
		}
		auto wstr = GetWideStringFromString(periRef.first);
		DrawString(rc.Left() + menubase_x,
			y,
			wstr.c_str(), mcolor);
		
		for (auto& p : periRef.second) {
			const wchar_t* wcstr = (p.type == PeripheralType::gamepad) ? L"gamepad" : L"keyboard";
			if (p.type == PeripheralType::keyboard) {
				DrawFormatString(rc.Left() + menubase_x + xoffset,
					y,
					mcolor,
					L" %s=%s", wcstr, vkeyname[p.inputIdx].c_str());
			}
			else {
				DrawFormatString(rc.Left() + menubase_x + xoffset,
					y,
					mcolor,
					L" %s=%04x", wcstr, p.inputIdx);
			}
			xoffset += 180;
		}
		y += menu_y_interval;
	}
	y += menu_y_interval;
	if (menuItems_[currentSelectNo_]=="commit") {
		mcolor = 0xaabbff;
	}
	else {
		mcolor = 0xffffff;
	}
	DrawString(rc.Left() + menubase_x,
		y,
		L"決定", mcolor);
}

void
KeyconfigScene::Update(const Input& input) {
	(this->*updater_)(input);
}
void
KeyconfigScene::Draw() {
	(this->*drawer_)();
}

void 
KeyconfigScene::CloseMenu(){
	controller_.PopScene();
}