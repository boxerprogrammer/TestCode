#pragma once

#include"../Geometry.h"
#include"Bullet.h"
#include<map>
#include<list>
#include<memory>

using BulletList_t = std::list<std::shared_ptr<Bullet>>;

class BulletFactory
{
	std::map<BulletType, int> bulletImgTable_;//���[�h����摜�̃e�[�u��
	BulletList_t bullets_;//���ۂ̒e�̃��X�g
	
public:
	BulletFactory();
	void Create(BulletType bulletType, const Position2& pos, const Vector2& vel);//�e�𐶐�
	/// <summary>
	/// �����蔻��p�ɂ��ׂĂ̓G�̒e�ւ̎Q�Ƃ��擾����
	/// </summary>
	/// <returns>�G�̒e�̃��X�g</returns>
	const BulletList_t& GetBullets()const;
	void Update();
	void Draw();
};

