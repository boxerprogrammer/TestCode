#include<DxLib.h>
#include<cassert>
#include<array>
#include<string>
#include"Scene/SceneManager.h"
#include"Scene/TitleScene.h"
#include"InputState.h"
#include"DrawFunctions.h"
#include"SoundManager.h"
#include<iostream>
void Function(int count,int value) {
	DrawFormatString(50, 50, 0xffffff, _T("Count=%d"), count);
	DrawFormatString(50, 100, 0xffffff, _T("Value=%d"), value);
}

void RecursiveFunction(int count) {
	if (count < 10) {
		RecursiveFunction(count + 1);
	}
	DrawFormatString(50, 150, 0xffffff, _T("RecursiveCount=%d"), count);
}



//3D�̗v�f���܂Ƃ߂��\����
class Vector3 {
public:
	float x;
	float y;
	float z;
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	bool isWindowMode = true;
	ChangeWindowMode(isWindowMode);

	SetWindowText(L"�������V���[�e�B���O");
	
	//�Q�[�����ɃE�B���h�E���[�h��؂�ւ��Ă��O���t�B�b�N�X�n���h����
	//���Z�b�g���Ȃ�
	//SetChangeScreenModeGraphicsSystemResetFlag(false);
	
	//ChangeWindowMode��SetWindowText�́A��O�I��DxLib_Init()�̑O�ɏ����Ă܂���
	//��{�I��DxLib�̊֐���DxLib_Init()���s��ɏ����Ă��������B
	if (DxLib_Init() == -1) {
		return -1;
	}

	auto& soundManager = SoundManager::GetInstance();

	//����ʂɕ`�悵�Ă��������Ƃ����w��
	SetDrawScreen(DX_SCREEN_BACK);

	int count = 0;
	int value = 0;
	++value;
	int handle = 0;
	

	InputState input;
	SceneManager sceneManager;
	sceneManager.ChangeScene(new TitleScene(sceneManager));

	std::wstring str = L"�e�X�g";
	str += L"����";
	bool isTriggerEnter = false;
	while (ProcessMessage() != -1) {
		ClearDrawScreen();

		if (DxLib::CheckHitKey(KEY_INPUT_LALT)) {
			if (DxLib::CheckHitKey(KEY_INPUT_RETURN)) {
				if (!isTriggerEnter) {
					isWindowMode = !isWindowMode;
					ChangeWindowMode(isWindowMode);
					SetDrawScreen(DX_SCREEN_BACK);//�`�����Ē�`
				}
				isTriggerEnter = true;
			}
			else {
				SetDrawScreen(DX_SCREEN_BACK);//�`�����Ē�`
				isTriggerEnter = false;
			}

		}

		//���͂̍X�V
		input.Update();

		sceneManager.Update(input);
		sceneManager.Draw();

		auto fps=GetFPS();//Frame Per Second;
		auto drawcall = GetDrawCallCount();//�`�施�ߐ�

		DrawFormatString(10, 10, 0xffffff, L"FPS=%2.2f",fps);
		DrawFormatString(10, 30, 0xffffff, L"DC=%d",drawcall);

		//�O��ʂƗ���ʂ����ւ��āA������҂��Ă���
		ScreenFlip();
	}
	DxLib_End();
	return 0;
}