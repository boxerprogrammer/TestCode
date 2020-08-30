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
	/// �{�X��ɓ˓�
	/// </summary>
	void OnBossEnter();
	/// <summary>
	/// �{�X���S
	/// </summary>
	void OnBossDie();

	/// <summary>
	/// �V�[���������Ă�v���C���[��Ԃ�
	/// </summary>
	/// <returns>�v���C���[�|�C���^</returns>
	std::shared_ptr<Player>& GetPlayer();
	/// <summary>
	/// �V�[���������Ă�J������Ԃ�
	/// </summary>
	/// <returns>�J�����|�C���^</returns>
	std::shared_ptr<Camera>& GetCamera();
	/// <summary>
	/// �V�[���������Ă�X�e�[�W����Ԃ�
	/// </summary>
	/// <returns>�X�e�[�W�|�C���^</returns>
	std::shared_ptr<Stage>& GetStage();

	/// <summary>
	/// �V�[���������Ă��铖���蔻��}�l�[�W����Ԃ�
	/// </summary>
	/// <returns>�����蔻��}�l�[�W��</returns>
	std::shared_ptr<CollisionManager> GetCollisionManager();

	/// <summary>
	/// �V�[���������Ă���G�t�F�N�g�}�l�[�W����Ԃ�
	/// </summary>
	/// <returns>�G�t�F�N�g�}�l�[�W��</returns>
	std::shared_ptr<EffectManager> GetEffectManager();

	/// <summary>
	/// �V�[���������Ă���G�}�l�[�W����Ԃ�
	/// </summary>
	/// <returns>�G�}�l�[�W��</returns>
	std::shared_ptr<EnemyManager> GetEnemyManager();

	/// <summary>
	/// �V�[���������Ă����ѓ���Ǘ��I�u�W�F�N�g�ւ̎Q�Ƃ�Ԃ�
	/// </summary>
	/// <returns>��ѓ���Ǘ��I�u�W�F�N�g�̎Q��</returns>
	ProjectileManager& GetProjectileManager();

	/// <summary>
	/// �G�������o�^����
	/// </summary>
	/// <param name="spawner">�G������</param>
	void AddSpawner(Spawner* spawner);

	/// <summary>
	/// ���̓��X�i�[��o�^����
	/// </summary>
	/// <param name="listener">���̓��X�i�I�u�W�F�N�g</param>
	void AddListener(std::shared_ptr<InputListener> listener);
	/// <summary>
	/// �Q�[���{�҃V�[���X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void Update(const Input&)override;

	/// <summary>
	/// �Q�[���{�ҕ`��
	/// </summary>
	void Draw()override;
};

