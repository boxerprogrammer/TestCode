#pragma once
#include"Scene.h"
#include<memory>//「スマートポインタ」を使うためのinclude
#include<array>//配列用
#include<list>
#include"../Geometry.h"
#include"../Game/EnemyType.h"

//プロトタイプ宣言
class Player;//プレイヤークラス
class Shot;//自機の弾
class ChargeShot;//波動砲(チャージ弾)
class EnemyFactory;//敵工場
class BulletFactory;//弾工場
class ItemFactory;//アイテム工場
class Stage;//ステージデータ
class Spawner;//敵発生器
class Missle;
/// <summary>
/// ゲーム中シーン
/// </summary>
class GameplayingScene : public Scene{
private:

	float px=0, py=0;
	float vx=0, vy=0;

	unsigned int fadeColor_ = 0x000000;//フェードの色(デフォ黒)

	static constexpr int fade_interval = 60;
	int fadeTimer_ = fade_interval;
	int fadeValue_ = 255;

	void FadeInUpdate(const InputState& input);
	void NormalUpdate(const InputState& input);
	bool CheckPauseForSlow(const InputState& input);
	void FadeOutUpdate(const InputState& input);

	using UpdateFunc_t = void (GameplayingScene::*)(const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;

	//プレイヤー
	std::shared_ptr<Player> player_;
	
	//ミサイル
	std::array<std::shared_ptr<Missle>, 4> missiles_;
	int missileTimer_ = 0;

	//弾
	std::array<std::shared_ptr<Shot>, 64> shots_;
	int rapidFireCount_ = 0;

	//波動砲(チャージ弾)
	std::array<std::shared_ptr<ChargeShot>,4> chargeShots_;
	int chargePower_ = 0;
	int gaugeImg_ = 0;
	int chargedGaugeImg_ = 0;

	//背景情報
	struct BackImg {
		int handle;//画像のハンドル
		float scrollSpeed;//スクロールのスピード(多重スクロール用)
		Size imgSize;//画像のサイズ
		//bool isTurnV = false;//上限反転ありか？
		//bool currentTurnV = false;//現在の画像は上下反転するか？
	};

	//背景
	std::array<BackImg, 3> bgImgs_;

	//現在のスクロール
	int scroll_ = 0;

	std::shared_ptr<EnemyFactory> enemyFactory_;//敵工場
	std::shared_ptr<BulletFactory> bulletFactory_;//弾工場
	std::shared_ptr<ItemFactory> itemFactory_;//アイテム管理者

	std::list<std::shared_ptr<Spawner>> spawners_;//敵発生器

	std::shared_ptr<Stage> stage_;
	int chipImg_;

	int eventWaitTimer_;//イベントによるスクロール待ち時間
	
	int tempScreenH_;//画面効果用スクリーンハンドル

	int quakeTimer_ = 0;//画面揺れタイマー
	float quakeX_=0.0f;//横揺れパラメータ

	//チップIDと座標から必要なスポーナーを設置する
	void SetSpawnerByChipIdAndPosition(EnemySpawnerId id,const Position2& pos);
	void SpawnersUpdate(int mapW,int mapH);

	int slowRate_ = 0;
	int frame_ = 0;

public:
	GameplayingScene(SceneManager& manager);
	~GameplayingScene();
	void Update(const InputState& input);
	void Draw();

};