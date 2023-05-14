#pragma once

#include"Scene.h"
#include"../InputState.h"

class SoundSettingScene : public Scene
{
private:
	int m_waitInterval = 60;//�l���ω�����C���^�[�o��
	int m_pressTime = 0;//�������ςȂ��̎���

	/// <summary>
	/// BGM�{�����[�����������ύX����
	/// </summary>
	/// <param name="inputtype">�L�[���͒l</param>
	/// <param name="changeVal">�ω��̒l</param>
	void AccelerateChangeBGMVolue(const InputState& input,InputType inputtype,int changeVal);
	void AccelerateChangeSEVolue(const InputState& input,InputType inputtype, int changeVal);

public:
	SoundSettingScene(SceneManager& manager);
	~SoundSettingScene();
	void Update(const InputState& input);
	void Draw();
};

