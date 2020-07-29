#pragma once
#include"Scene.h"
#include<memory>
#include<vector>

class TitleScene;
class Player;
class Background;
class InputListener;
class ProjectileManager;
class EnemyManager;
class EffectManager;
class CollisionManager;
class Spawner;
class Stage;
class Camera;
class GameplayingScene:public Scene 
{
	friend TitleScene;
private:
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

	void FadeinUpdate(const Input&);
	void NormalUpdate(const Input&);
	void FadeoutUpdate(const Input&);

	using UpdateFunction_t = void (GameplayingScene::*)(const Input&);
	UpdateFunction_t updater_;



	void NormalDraw();
	void FadeDraw();
	void (GameplayingScene::* drawer_)();

	std::vector<std::shared_ptr<InputListener>> listeners_;

public:
	~GameplayingScene();
	std::shared_ptr<Player>& GetPlayer();
	std::shared_ptr<Camera>& GetCamera();
	std::shared_ptr<Stage>& GetStage();
	ProjectileManager& GetProjectileManager();
	std::shared_ptr<CollisionManager> GetCollisionManager();
	void AddListener(std::shared_ptr<InputListener> listener);
	void Update(const Input&)override;
	void Draw()override;
};

