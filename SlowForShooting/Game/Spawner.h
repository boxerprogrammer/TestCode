#pragma once
#include<memory>
#include"../Geometry.h"
#include"EnemyType.h"

class Enemy;
class EnemyFactory;
//�G������(Spawner)
//�G�͊�{�I�ɂ��̔����킩�甭������
//���N���X
class Spawner
{
protected:
	Position2 pos_;//�X�|�[�i�[������̍��W
	std::shared_ptr<Enemy> protoType_;//�v���g�^�C�v(�N���[����)
	std::shared_ptr<EnemyFactory> factory_;//�t�@�N�g���ւ̎Q��(�t�@�N�g���̃��X�g�ɒǉ����邽��)
	bool isEnabled_=true;//�X�|�[�i�[���g�̎���
public:
	Spawner(const Position2& pos ,//���W
		std::shared_ptr<Enemy> protoType,//�v���g�^�C�v
		std::shared_ptr<EnemyFactory> factory);//�t�@�N�g��

	bool IsEnabled()const;

	virtual std::shared_ptr<Enemy> Spawn() = 0;//�G����
	virtual void Update(int scroll) = 0;//�X�V
	
};

