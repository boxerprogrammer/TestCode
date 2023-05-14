#pragma once
#include"Scene.h"
#include<memory>//�u�X�}�[�g�|�C���^�v���g�����߂�include
#include<array>//�z��p
#include<list>
#include"../Geometry.h"
#include"../Game/EnemyType.h"

//�v���g�^�C�v�錾
class Player;//�v���C���[�N���X
class Shot;//���@�̒e
class ChargeShot;//�g���C(�`���[�W�e)
class EnemyFactory;//�G�H��
class BulletFactory;//�e�H��
class ItemFactory;//�A�C�e���H��
class Stage;//�X�e�[�W�f�[�^
class Spawner;//�G������
class Missle;
/// <summary>
/// �Q�[�����V�[��
/// </summary>
class GameplayingScene : public Scene{
private:

	float px=0, py=0;
	float vx=0, vy=0;

	unsigned int fadeColor_ = 0x000000;//�t�F�[�h�̐F(�f�t�H��)

	static constexpr int fade_interval = 60;
	int fadeTimer_ = fade_interval;
	int fadeValue_ = 255;

	void FadeInUpdate(const InputState& input);
	void NormalUpdate(const InputState& input);
	bool CheckPauseForSlow(const InputState& input);
	void FadeOutUpdate(const InputState& input);

	using UpdateFunc_t = void (GameplayingScene::*)(const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;

	//�v���C���[
	std::shared_ptr<Player> player_;
	
	//�~�T�C��
	std::array<std::shared_ptr<Missle>, 4> missiles_;
	int missileTimer_ = 0;

	//�e
	std::array<std::shared_ptr<Shot>, 64> shots_;
	int rapidFireCount_ = 0;

	//�g���C(�`���[�W�e)
	std::array<std::shared_ptr<ChargeShot>,4> chargeShots_;
	int chargePower_ = 0;
	int gaugeImg_ = 0;
	int chargedGaugeImg_ = 0;

	//�w�i���
	struct BackImg {
		int handle;//�摜�̃n���h��
		float scrollSpeed;//�X�N���[���̃X�s�[�h(���d�X�N���[���p)
		Size imgSize;//�摜�̃T�C�Y
		//bool isTurnV = false;//������]���肩�H
		//bool currentTurnV = false;//���݂̉摜�͏㉺���]���邩�H
	};

	//�w�i
	std::array<BackImg, 3> bgImgs_;

	//���݂̃X�N���[��
	int scroll_ = 0;

	std::shared_ptr<EnemyFactory> enemyFactory_;//�G�H��
	std::shared_ptr<BulletFactory> bulletFactory_;//�e�H��
	std::shared_ptr<ItemFactory> itemFactory_;//�A�C�e���Ǘ���

	std::list<std::shared_ptr<Spawner>> spawners_;//�G������

	std::shared_ptr<Stage> stage_;
	int chipImg_;

	int eventWaitTimer_;//�C�x���g�ɂ��X�N���[���҂�����
	
	int tempScreenH_;//��ʌ��ʗp�X�N���[���n���h��

	int quakeTimer_ = 0;//��ʗh��^�C�}�[
	float quakeX_=0.0f;//���h��p�����[�^

	//�`�b�vID�ƍ��W����K�v�ȃX�|�[�i�[��ݒu����
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