#include "SoundSettingScene.h"
#include<DxLib.h>
#include"../SoundManager.h"
#include"../InputState.h"
#include"SceneManager.h"

namespace {
	constexpr int init_wait_interval = 60;//キー入力待ち間隔初期値
	constexpr int wait_interval_decrement_period = 5;//入力待ち間隔が落ちていく間隔
}

void SoundSettingScene::AccelerateChangeBGMVolue(const InputState& input, InputType inputtype, int changeVal)
{
	auto& soundMgr = SoundManager::GetInstance();
	if (input.IsPressed(inputtype)) {
		if (input.IsTriggered(inputtype)) {//押した瞬間
			m_pressTime = 0;
			m_waitInterval = init_wait_interval;
		}
		if (m_pressTime % m_waitInterval == 0) {
			soundMgr.SetBGMVolume(std::max(std::min(soundMgr.GetBGMVolume() + changeVal, 255),0));
		}
		if (m_pressTime % wait_interval_decrement_period == 0) {
			m_waitInterval = std::max(m_waitInterval - 1, 1);
		}
		m_pressTime++;
	}
}

void SoundSettingScene::AccelerateChangeSEVolue(const InputState& input,InputType inputtype, int changeVal)
{
	auto& soundMgr = SoundManager::GetInstance();
	if (input.IsPressed(inputtype)) {
		if (input.IsTriggered(inputtype)) {//押した瞬間
			m_pressTime = 0;
			m_waitInterval = init_wait_interval;
		}
		if (m_pressTime % m_waitInterval == 0) {
			soundMgr.SetSEVolume(std::max(std::min(soundMgr.GetSEVolume() + changeVal, 255), 0));
		}
		if (m_pressTime % wait_interval_decrement_period == 0) {
			m_waitInterval = std::max(m_waitInterval - 1, 1);
		}
		m_pressTime++;
	}
}

SoundSettingScene::SoundSettingScene(SceneManager& manager):
	Scene(manager)
{
}

SoundSettingScene::~SoundSettingScene()
{
}

void SoundSettingScene::Update(const InputState& input)
{
	auto& soundMgr = SoundManager::GetInstance();
	//BGM
	AccelerateChangeBGMVolue(input, InputType::up, +1);//上で増える
	AccelerateChangeBGMVolue(input, InputType::down, -1);//下で減る

	//SE
	AccelerateChangeSEVolue(input, InputType::right, +1);//右で増える
	AccelerateChangeSEVolue(input, InputType::left, -1);//左で減る

	if (input.IsTriggered(InputType::pause)) {
		manager_.PopScene();
		soundMgr.SaveSoundConfig();
	}

}

void SoundSettingScene::Draw()
{
	constexpr int pw_width = 450;//キーコンフィグ枠の幅
	constexpr int pw_height = 350;//キーコンフィグ枠の高さ
	constexpr int pw_start_x = (640 - pw_width) / 2 + 50;//キーコンフィグ枠左
	constexpr int pw_start_y = (480 - pw_height) / 2 + 50;//キーコンフィグ枠上

	//サウンドセッティングウィンドウ背景
	DrawBox(pw_start_x, pw_start_y,
		pw_start_x + pw_width,
		pw_start_y + pw_height,
		0x884444, true);

	//サウンドセッティング中メッセージ
	DrawString(pw_start_x + 10,
		pw_start_y + 10, L"SoundSetting...", 0xffffaa);

	auto& soungMgr = SoundManager::GetInstance();
	//BGMの設定
	DrawFormatString(pw_start_x + 20,
		pw_start_y + 50,0xffffff, L"BGM Volume=%d",soungMgr.GetBGMVolume() );

	DrawFormatString(pw_start_x + 20,
		pw_start_y + 70, 0xffffff, L"SE Volume=%d", soungMgr.GetSEVolume());


	//サウンドセッティングウィンドウ枠線
	DrawBox(pw_start_x, pw_start_y,
		pw_start_x + pw_width,
		pw_start_y + pw_height,
		0xffffff, false);
}
