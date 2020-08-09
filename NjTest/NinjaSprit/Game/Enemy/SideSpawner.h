#pragma once
#include "Spawner.h"
class CollisionManager;
/// <summary>
/// ��ʂ̗�������G�𔭐�������N���X
/// </summary>
class SideSpawner :  public Spawner
{
private:
	int frame_=0;//��莞�Ԃ��ƂɓG�̔����̎��ȂǂɎg�p����
	std::shared_ptr<CollisionManager> collisionManager_;//
public:
	/// <summary>
	/// �������甭����
	/// </summary>
	/// <param name="pos">�ݒ肳��锭����̍��W</param>
	/// <param name="prototype">�G�̃v���g�^�C�v</param>
	/// <param name="enemyManager">�G�}�l�[�W��(�G�o�^�p)</param>
	/// <param name="collisionManager">�Փ˔���}�l�[�W��(�����蔻��o�^�p)</param>
	/// <param name="camera">�G������̔����^�C�~���O�̂��߂̃J�����ւ̎Q��</param>
	/// <returns></returns>
	SideSpawner(const Position2f& pos, Enemy* prototype, 
		std::shared_ptr<EnemyManager>& enemyManager,
		std::shared_ptr<CollisionManager> collisionManager, 
		std::shared_ptr<Camera> camera);

	/// <summary>
	/// ���Ԃ��Ƃ̔����Ȃǂ��K�v�Ȃ��ߖ��t���[���X�V���s��
	/// </summary>
	void Update() override;
};

