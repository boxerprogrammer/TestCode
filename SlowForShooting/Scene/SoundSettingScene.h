#pragma once

#include"Scene.h"
#include"../InputState.h"

class SoundSettingScene : public Scene
{
private:
	int m_waitInterval = 60;//値が変化するインターバル
	int m_pressTime = 0;//押しっぱなしの時間

	/// <summary>
	/// BGMボリュームを加速しつつ変更する
	/// </summary>
	/// <param name="inputtype">キー入力値</param>
	/// <param name="changeVal">変化の値</param>
	void AccelerateChangeBGMVolue(const InputState& input,InputType inputtype,int changeVal);
	void AccelerateChangeSEVolue(const InputState& input,InputType inputtype, int changeVal);

public:
	SoundSettingScene(SceneManager& manager);
	~SoundSettingScene();
	void Update(const InputState& input);
	void Draw();
};

