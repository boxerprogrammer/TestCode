#include "PauseScene.h"
#include<DxLib.h>
#include"../InputState.h"
#include"SceneManager.h"
#include"KeyConfigScene.h"
#include"SoundSettingScene.h"
PauseScene::PauseScene(SceneManager& manager):Scene(manager)
{
}

PauseScene::~PauseScene()
{
}

void PauseScene::Update(const InputState& input)
{
	if (input.IsTriggered(InputType::pause)) {
		manager_.PopScene();
		return;
	}
	if (input.IsTriggered(InputType::keyconf)) {
		manager_.PushScene(new KeyConfigScene(manager_,input));
	}

	if (input.IsTriggered(InputType::next)) {
		manager_.PushScene(new SoundSettingScene(manager_));
	}

}

void PauseScene::Draw()
{
	

	constexpr int pw_width = 400;//�|�[�Y�g�̕�
	constexpr int pw_height = 300;//�|�[�Y�g�̍���
	constexpr int pw_start_x =(640 - pw_width) / 2;//�|�[�Y�g��
	constexpr int pw_start_y = (480 - pw_height) / 2;//�|�[�Y�g��

	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//��Z����
	//�|�[�Y�E�B���h�E�Z���t�@��(����)
	DrawBox(pw_start_x, pw_start_y,
		pw_start_x + pw_width,
		pw_start_y + pw_height,
		0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�ʏ�`��ɖ߂�

	//�|�[�Y�����b�Z�[�W
	DrawString(pw_start_x+10, 
		pw_start_y+ 10, L"Pausing...", 0xffff88);

	//�|�[�Y�E�B���h�E�g��
	DrawBox(pw_start_x, pw_start_y,
		pw_start_x + pw_width,
		pw_start_y + pw_height,
		0xffffff,false);

}
