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
	updater_(&KeyconfigScene::InUpdate),
	drawer_(&KeyconfigScene::NormalDraw) {
	frameRect_.pos += offset;
	auto& fileMgr = FileManager::Instance();
	titleW_ = static_cast<size_t>(DxLib::GetDrawStringWidth(keyconfig_title, static_cast<int>(wcslen(keyconfig_title))));
	openSE_ = fileMgr.Load(L"Resource/Sound/System/menuopen.wav")->Handle();
	closeSE_ = fileMgr.Load(L"Resource/Sound/System/menuclose.wav")->Handle();
	moveSE_ = fileMgr.Load(L"Resource/Sound/System/cursor.wav")->Handle();
	decideSE_ = fileMgr.Load(L"Resource/Sound/System/decide.wav")->Handle();
	cancelSE_ = fileMgr.Load(L"Resource/Sound/System/cancel.wav")->Handle();
}

void 
KeyconfigScene::CommitCurrentKeyConfig(const Input& input) {
	input.SetPeripheraReferenceTable(peripheralReferenceTable_);
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
KeyconfigScene::InUpdate(const Input& input) {
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
				for (int i = 0; i < input.rawkeystate_.size(); ++i) {
					if (input.rawkeystate_[i]) {
						p.inputIdx = i;
						break;
					}
				}
			}
			else if(p.type==PeripheralType::gamepad){
				if (input.rawpadstate_ != 0) {
					p.inputIdx = input.rawpadstate_;
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
			DrawFormatString(rc.Left() + menubase_x + xoffset,
				y,
				mcolor,
				L" %s=%04x", wcstr,p.inputIdx);
			xoffset += 150;
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