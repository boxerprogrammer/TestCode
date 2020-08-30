#pragma once
#include "Spawner.h"

#include<memory>
#include<list>

class CollisionManager;
/// <summary>
/// ��ʂ̗�������G�𔭐�������N���X
/// </summary>
class SideSpawner :  public Spawner
{
private:
	int frame_=0;//��莞�Ԃ��ƂɓG�̔����̎��ȂǂɎg�p����
	std::shared_ptr<CollisionManager> collisionManager_;//
	//����������(�f�t�H���g�ł͐����Ȃ�)
	//-1�̂Ƃ��͖�������(����ȊO�̂Ƃ��͎w�蔭�����ɗ}�������)
	int limitSpawnNum_ = -1;
	int averageSpawnFrame_ = 60;//�����p�x(���ϔ����^�C�~���O)
	bool needBalance_ = false;
	struct Regulator {//�������邽�߂̍\����
		std::weak_ptr<Enemy> ownedWRef;//��Q��
		bool fromRight = true;//�E���痈�����ǂ���
		Regulator(std::weak_ptr<Enemy> ref, bool rgt) :ownedWRef(ref),fromRight(rgt){ }
	};
	std::list<Regulator> regulators_;//������

public:
	/// <summary>
	/// �������甭����
	/// </summary>
	/// <param name="pos">�ݒ肳��锭����̍��W</param>
	/// <param name="prototype">�G�̃v���g�^�C�v</param>
	/// <param name="enemyManager">�G�}�l�[�W��(�G�o�^�p)</param>
	/// <param name="collisionManager">�Փ˔���}�l�[�W��(�����蔻��o�^�p)</param>
	/// <param name="camera">�G������̔����^�C�~���O�̂��߂̃J�����ւ̎Q��</param>
	/// <param name="averageSpawnFrame">���ϔ����t���[������</param>
	/// <param name="limitSpawnNum">����������</param>
	/// <param name="needBalance">���E�o�����V���O���s��</param>
	SideSpawner(const Position2f& pos, 
		Enemy* prototype, 
		std::shared_ptr<EnemyManager> enemyManager,
		std::shared_ptr<CollisionManager> collisionManager, 
		std::shared_ptr<Camera> camera,
		int averageSpawnFrame=60,
		int limitSpawnNum=-1,
		bool needBalance=false);

	/// <summary>
	/// ���Ԃ��Ƃ̔����Ȃǂ��K�v�Ȃ��ߖ��t���[���X�V���s��
	/// </summary>
	void Update() override;
};

