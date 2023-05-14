#pragma once
#include"../Geometry.h"
//���@�̒e���Ǘ�����N���X
class ChargeShot
{
private:
	Rect rect_ = {};
	Vector2 vel_ = {};
	int handle_ = 0;
	int animFrame_ = 0;
	//�e�L���t���O
	bool isEnabled_ = false;
public:
	ChargeShot(int handle);
	const Rect& GetRect()const;

	/// <summary>
	/// �e�𔭎˂���
	/// </summary>
	/// <param name="pos">���ˏꏊ</param>
	void Fire(const Position2& pos);
	//�e�̗L�������t���O�𓾂�
	bool IsEnabled()const;

	void Update();
	void Draw();
};

