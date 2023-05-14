#pragma once 
#include"Scene.h"
class InputState;
///�^�C�g���V�[��
class TitleScene : public Scene {
private:
	static constexpr int fade_interval = 20;
	int titleH_ = -1;//�^�C�g���摜
	int gradH_ = -1;
	int fadeTimer_ = fade_interval;//�t�F�[�h�^�C�}�[
	int fadeValue_ = 255;//����`�Ƃ̃u�����h�

	//�t�F�[�h�C���̎���Update�֐�
	void FadeInUpdate(const InputState& input);
	//�ʏ��Ԃ�Update�֐�
	void NormalUpdate(const InputState& input);
	//�t�F�[�h�A�E�g����Update�֐�
	void FadeOutUpdate(const InputState& input);

	//Update�p�����o�֐��|�C���^
	void (TitleScene::* updateFunc_)(const InputState& input);

	void Function();

public:
	TitleScene(SceneManager& manager);
	~TitleScene();

	void Update(const InputState& input);
	void Draw();

};