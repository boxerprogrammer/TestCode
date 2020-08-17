#pragma once
#include<memory>
#include<list>
#include"../../Geometry.h"
class Enemy;
class EnemyManager;
class Camera;
/// <summary>
/// �G��������N���X
/// </summary>
class Spawner
{
protected:
	Position2f pos_;//��ʓ��ɃX�|�[�i�[�����邩�ǂ����̔��肪�K�v�Ȃ��ߍ��W������
	std::unique_ptr<Enemy> prototype_;//�v���g�^�C�v
	std::shared_ptr<EnemyManager> enemyManager_;//�G�}�l�[�W���ւ̎Q��
	std::shared_ptr<Camera> camera_;//�J�����ւ̎Q��
	//�����Ă���v���g�^�C�v�̃N���[��������
	//�����Ńv���g�^�C�v������ɃR�s�[����点�Ă���
	virtual Enemy* CreateClone()final;
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="pos">������̏ꏊ</param>
	/// <param name="prototype">�G�v���g�^�C�v</param>
	/// <param name="enemyManager">�G�}�l�[�W��</param>
	/// <param name="camera">�J�����ւ̎Q��</param>
	/// <returns></returns>
	Spawner(const Position2f& pos, Enemy* prototype, std::shared_ptr<EnemyManager> em,std::shared_ptr<Camera> c);
	virtual ~Spawner()=default;
	
	/// <summary>
	/// ���Ԃ��Ƃ̔����Ȃǂ��K�v�Ȃ��ߖ��t���[���X�V���s��
	/// </summary>
	virtual void Update() = 0;
};
