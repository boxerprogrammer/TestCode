#pragma once

#include"Geometry.h"

//�v���g�^�C�v�錾�ł���
class EnemyFactory;

//�G�N���X
class Enemy
{
	friend EnemyFactory;
private:
	Position2f _pos;
	bool isDead = false;

	//���t�@�N�g���ɓ���Ă��炤���
	int _pictH;//�G�摜�p
	int _expH;//�����p

	//�R�s�[��������֎~����
	Enemy(const Enemy&);
	void operator=(const Enemy&);

	//�R���X�g���N�^
	Enemy();
public:
	
	~Enemy();
	void Update();
	void Draw();
};

