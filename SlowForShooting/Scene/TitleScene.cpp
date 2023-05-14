#include"TitleScene.h"
#include"../InputState.h"
#include"SceneManager.h"
#include"GameplayingScene.h"
#include"OptionScene.h"
#include"../DrawFunctions.h"
#include"../SoundManager.h"
#include<DxLib.h>

void TitleScene::FadeInUpdate(const InputState& input)
{
	fadeValue_ = fadeTimer_;// 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &TitleScene::NormalUpdate;
		fadeValue_ = 0;
	}
	
}

void TitleScene::NormalUpdate(const InputState& input)
{
	//�u���ցv�{�^���������ꂽ���V�[���ֈڍs����
	if (input.IsTriggered(InputType::next)) {
		updateFunc_ = &TitleScene::FadeOutUpdate;
		SoundManager::GetInstance().Play(L"decide");
	}
	if (input.IsTriggered(InputType::prev)) {
		manager_.ChangeScene(new OptionScene(manager_));
		return;
	}
}

void TitleScene::FadeOutUpdate(const InputState& input)
{
	
	if (fadeTimer_ == fade_interval) {
		manager_.ChangeScene(new GameplayingScene(manager_));
		return;
	}
	++fadeTimer_;
	fadeValue_ = static_cast<int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval)));
}

void TitleScene::Function()
{
}

TitleScene::TitleScene(SceneManager& manager) :Scene(manager) ,
updateFunc_(&TitleScene::FadeInUpdate) {
	titleH_=my::MyLoadGraph(L"Data/img/title.png");
	fadeTimer_ = 255;
}

TitleScene::~TitleScene()
{
	DeleteGraph(titleH_);
}

void
TitleScene::Update(const InputState& input) {
	(this->*updateFunc_)(input);
}

void
TitleScene::Draw() {
	//���ʂ̕`��
	DrawRotaGraph(320,240,1.0f,0.0f,titleH_,true);

	SetDrawBlendMode(DX_BLENDMODE_MULA, fadeValue_);


	//�����珑���摜�ƁA���łɕ`�悳��Ă�X�N���[���Ƃ�
	//�u�����h�̎d�����w�肵�Ă���B
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//��ʑS�̂�^�����ɓh��Ԃ�
	DrawBox(0, 0, 640, 480, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


}