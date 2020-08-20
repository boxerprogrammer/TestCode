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
		vkeyname[KEY_INPUT_RETURN] = L"ENTER�L�[";
		vkeyname[KEY_INPUT_ESCAPE] = L"ESC�L�[";
		vkeyname[KEY_INPUT_0] = L"�O�L�[";
		vkeyname[KEY_INPUT_1] = L"�P�L�[";
		vkeyname[KEY_INPUT_2] = L"�Q�L�[";
		vkeyname[KEY_INPUT_3] = L"�R�L�[";
		vkeyname[KEY_INPUT_4] = L"�S�L�[";
		vkeyname[KEY_INPUT_5] = L"�T�L�[";
		vkeyname[KEY_INPUT_6] = L"�U�L�[";
		vkeyname[KEY_INPUT_7] = L"�V�L�[";
		vkeyname[KEY_INPUT_8] = L"�W�L�[";
		vkeyname[KEY_INPUT_9] = L"�X�L�[";
		vkeyname[KEY_INPUT_A] = L"�`�L�[";
		vkeyname[KEY_INPUT_B] = L"�a�L�[";
		vkeyname[KEY_INPUT_C] = L"�b�L�[";
		vkeyname[KEY_INPUT_D] = L"�c�L�[";
		vkeyname[KEY_INPUT_E] = L"�d�L�[";
		vkeyname[KEY_INPUT_F] = L"�e�L�[";
		vkeyname[KEY_INPUT_G] = L"�f�L�[";
		vkeyname[KEY_INPUT_H] = L"�g�L�[";
		vkeyname[KEY_INPUT_I] = L"�h�L�[";
		vkeyname[KEY_INPUT_J] = L"�i�L�[";
		vkeyname[KEY_INPUT_K] = L"�j�L�[";
		vkeyname[KEY_INPUT_L] = L"�k�L�[";
		vkeyname[KEY_INPUT_M] = L"�l�L�[";
		vkeyname[KEY_INPUT_N] = L"�m�L�[";
		vkeyname[KEY_INPUT_O] = L"�n�L�[";
		vkeyname[KEY_INPUT_P] = L"�o�L�[";
		vkeyname[KEY_INPUT_Q] = L"�p�L�[";
		vkeyname[KEY_INPUT_R] = L"�q�L�[";
		vkeyname[KEY_INPUT_S] = L"�r�L�[";
		vkeyname[KEY_INPUT_T] = L"�s�L�[";
		vkeyname[KEY_INPUT_U] = L"�t�L�[";
		vkeyname[KEY_INPUT_V] = L"�u�L�[";
		vkeyname[KEY_INPUT_W] = L"�v�L�[";
		vkeyname[KEY_INPUT_X] = L"�w�L�[";
		vkeyname[KEY_INPUT_Y] = L"�x�L�[";
		vkeyname[KEY_INPUT_Z] = L"�y�L�[";
		vkeyname[KEY_INPUT_BACK] = L"BackSpace�L�[";
		vkeyname[KEY_INPUT_TAB] = L"Tab�L�[";
		vkeyname[KEY_INPUT_LSHIFT] = L"��Shift�L�[";
		vkeyname[KEY_INPUT_RSHIFT] = L"�EShift�L�[";
		vkeyname[KEY_INPUT_LCONTROL] = L"��Ctrl�L�[";
		vkeyname[KEY_INPUT_RCONTROL] = L"�ECtrl�L�[";
		vkeyname[KEY_INPUT_ESCAPE] = L"Esc�L�[";
		vkeyname[KEY_INPUT_SPACE] = L"�X�y�[�X�L�[";
		vkeyname[KEY_INPUT_PGUP] = L"PageUp�L�[";
		vkeyname[KEY_INPUT_PGDN] = L"PageDown�L�[";
		vkeyname[KEY_INPUT_END] = L"End�L�[";
		vkeyname[KEY_INPUT_HOME] = L"Home�L�[";
		vkeyname[KEY_INPUT_LEFT] = L"���L�[";
		vkeyname[KEY_INPUT_UP] = L"��L�[";
		vkeyname[KEY_INPUT_RIGHT] = L"�E�L�[";
		vkeyname[KEY_INPUT_DOWN] = L"���L�[";
		vkeyname[KEY_INPUT_INSERT] = L"Insert�L�[";
		vkeyname[KEY_INPUT_DELETE] = L"Delete�L�[";
		vkeyname[KEY_INPUT_F1] = L"�e�P�L�[";
		vkeyname[KEY_INPUT_F2] = L"�e�Q�L�[";
		vkeyname[KEY_INPUT_F3] = L"�e�R�L�[";
		vkeyname[KEY_INPUT_F4] = L"�e�S�L�[";
		vkeyname[KEY_INPUT_F5] = L"�e�T�L�[";
		vkeyname[KEY_INPUT_F6] = L"�e�U�L�[";
		vkeyname[KEY_INPUT_F7] = L"�e�V�L�[";
		vkeyname[KEY_INPUT_F8] = L"�e�W�L�[";
		vkeyname[KEY_INPUT_F9] = L"�e�X�L�[";
		vkeyname[KEY_INPUT_F10] = L"�e�P�O�L�[";
		vkeyname[KEY_INPUT_F11] = L"�e�P�P�L�[";
		vkeyname[KEY_INPUT_F12] = L"�e�P�Q�L�[";
	}
	constexpr int keyconfig_title_y = 32;//�|�[�Y�^�C�g����Y
	constexpr int menubase_x = 64;//���j���[�̍�
	constexpr int menubase_y = 64;//���j���[�̈�ԏ��Y
	constexpr int menu_y_interval = 32;//���j���[�̊Ԋu
	const wchar_t keyconfig_title[] = L"Key Config";
	
	///string(�}���`�o�C�g������)����wstring(���C�h������)�𓾂�
///@param str �}���`�o�C�g������
///@return �ϊ����ꂽ���C�h������
	std::wstring
		GetWideStringFromString(const std::string& str) {
		//�Ăяo��1���(�����񐔂𓾂�)
		auto num1 = MultiByteToWideChar(CP_ACP,
			MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
			str.c_str(), -1, nullptr, 0);

		std::wstring wstr;//string��wchar_t��
		wstr.resize(num1);//����ꂽ�����񐔂Ń��T�C�Y

		//�Ăяo��2���(�m�ۍς݂�wstr�ɕϊ���������R�s�[)
		auto num2 = MultiByteToWideChar(CP_ACP,
			MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
			str.c_str(), -1, &wstr[0], num1);

		assert(num1 == num2);//�ꉞ�`�F�b�N
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

//�ŏ���Update
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

//�L�[�R���t�B�O�ҏW��Update
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
		//�����AOK�������̓L�����Z���ȊO�̓��͂�����΂���͍X�V���������͂̂͂�
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

//�ʏ�(�ҏW���łȂ�)Update
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
	//�|�[�Y�̃^�C�g��
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
		L"����", mcolor);
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