#pragma once
#include"Scene.h"
class GameoverScene;
class PauseScene;
class TitleScene:public Scene 
{
	friend SceneController;
	friend GameoverScene;
	friend PauseScene;
private:
	TitleScene(SceneController& c);//�t�@�N�g���ɂ�����点�����Ȃ�
	int titleH_=-1;
	int startH_=-1;
	int startSE_ = -1;
	void FadeinUpdate(const Input&);
	void WaitUpdate(const Input&);
	void BlinkUpdate(const Input&);
	void FadeoutUpdate(const Input&);

	using UpdateFunction_t = void (TitleScene::*)(const Input&);
	UpdateFunction_t updater_;

	void NormalDraw();
	void FadeDraw();
	void (TitleScene::*drawer_)();

public:
	~TitleScene();
	/// <summary>
	/// �^�C�g���V�[���X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void Update(const Input& input)override;
	/// <summary>
	/// �^�C�g���V�[���`��
	/// </summary>
	void Draw()override;
};



