#pragma once
#include"Scene.h"
#include<memory>
#include<vector>
#include<array>

class TitleScene;
class Player;
class Background;
class InputListener;
class ProjectileManager;
class EnemyManager;
class EffectManager;
class CollisionManager;
class Spawner;
class Enemy;
class Stage;
class Camera;
class GameplayingScene:public Scene 
{
	friend TitleScene;
private:

	std::array<int,4> weaponUIH_;

	int bgm_ = -1;
	int bgmVolume_ = 150;
	int bossBgm_ = -1;
	int ashuraH_ = -1;

	GameplayingScene(SceneController&);

	std::shared_ptr<Player> player_;
	std::unique_ptr<Background> bg_;
	std::unique_ptr<ProjectileManager> projectileManager_;
	std::shared_ptr<EnemyManager> enemyManager_;
	std::vector<std::shared_ptr<Spawner>> spawners_;
	std::shared_ptr<EffectManager> effectManager_;
	std::shared_ptr<CollisionManager> collisionManager_;
	std::shared_ptr<Stage> stage_;
	std::shared_ptr<Camera> camera_;

	void InitializeUpdate(const Input&);
	void FadeinUpdate(const Input&);
	void NormalUpdate(const Input&);
	void Fadeout();
	void BossBattleUpdate(const Input&);
	void BossEnterUpdate(const Input&);
	void BossDyingUpdate(const Input&);
	void FadeoutUpdate(const Input&);

	using UpdateFunction_t = void (GameplayingScene::*)(const Input&);
	UpdateFunction_t updater_;



	void NormalDraw();
	void BossDraw();
	void FadeDraw();
	void (GameplayingScene::* drawer_)();

	std::vector<std::shared_ptr<InputListener>> listeners_;

public:
	~GameplayingScene();

	/// <summary>
	/// ボス戦に突入
	/// </summary>
	void OnBossEnter();
	/// <summary>
	/// ボス死亡
	/// </summary>
	void OnBossDie();

	/// <summary>
	/// シーンが持ってるプレイヤーを返す
	/// </summary>
	/// <returns>プレイヤーポインタ</returns>
	std::shared_ptr<Player>& GetPlayer();
	/// <summary>
	/// シーンが持ってるカメラを返す
	/// </summary>
	/// <returns>カメラポインタ</returns>
	std::shared_ptr<Camera>& GetCamera();
	/// <summary>
	/// シーンが持ってるステージ情報を返す
	/// </summary>
	/// <returns>ステージポインタ</returns>
	std::shared_ptr<Stage>& GetStage();

	/// <summary>
	/// シーンが持っている当たり判定マネージャを返す
	/// </summary>
	/// <returns>当たり判定マネージャ</returns>
	std::shared_ptr<CollisionManager> GetCollisionManager();

	/// <summary>
	/// シーンが持っているエフェクトマネージャを返す
	/// </summary>
	/// <returns>エフェクトマネージャ</returns>
	std::shared_ptr<EffectManager> GetEffectManager();

	/// <summary>
	/// シーンが持っている敵マネージャを返す
	/// </summary>
	/// <returns>敵マネージャ</returns>
	std::shared_ptr<EnemyManager> GetEnemyManager();

	/// <summary>
	/// シーンが持っている飛び道具管理オブジェクトへの参照を返す
	/// </summary>
	/// <returns>飛び道具管理オブジェクトの参照</returns>
	ProjectileManager& GetProjectileManager();

	/// <summary>
	/// 敵発生器を登録する
	/// </summary>
	/// <param name="spawner">敵発生器</param>
	void AddSpawner(Spawner* spawner);

	/// <summary>
	/// 入力リスナーを登録する
	/// </summary>
	/// <param name="listener">入力リスナオブジェクト</param>
	void AddListener(std::shared_ptr<InputListener> listener);
	/// <summary>
	/// ゲーム本編シーン更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void Update(const Input&)override;

	/// <summary>
	/// ゲーム本編描画
	/// </summary>
	void Draw()override;
};

