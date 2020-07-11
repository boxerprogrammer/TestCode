#pragma once
#include<memory>
#include<list>
#include"../../Geometry.h"
class Enemy;
class EnemyManager;
/// <summary>
/// �G��������N���X
/// </summary>
class Spawner
{
protected:
	Position2f pos_;//��ʓ��ɃX�|�[�i�[�����邩�ǂ����̔��肪�K�v�Ȃ��ߍ��W������
	std::unique_ptr<Enemy> prototype_;//�v���g�^�C�v
	std::shared_ptr<EnemyManager>& enemyManager_;
	virtual Enemy* CreateClone()final;
public:
	Spawner(const Position2f& pos, Enemy* prototype, std::shared_ptr<EnemyManager>& em);
	virtual ~Spawner()=default;
	
	/// <summary>
	/// ���Ԃ��Ƃ̔����Ȃǂ��K�v�Ȃ��ߖ��t���[���X�V���s��
	/// </summary>
	virtual void Update() = 0;
};
