#pragma once
#include"Scene.h"
class ContinueScene;

class GameoverScene:public Scene 
{
	friend ContinueScene;
private:
	GameoverScene(SceneController&);
	void FadeinUpdate(const Input&);
	void WaitUpdate(const Input&);
	void FadeoutUpdate(const Input&);

	using UpdateFunction_t = void (GameoverScene::*)(const Input&);
	UpdateFunction_t updater_;

	void NormalDraw();
	void FadeDraw();
	void (GameoverScene::* drawer_)();
public:
	~GameoverScene() = default;
	/// <summary>
	/// �Q�[���I�[�o�[�V�[���X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void Update(const Input&)override;

	/// <summary>
	/// �Q�[���I�[�o�[�`��
	/// </summary>
	void Draw()override;
};

