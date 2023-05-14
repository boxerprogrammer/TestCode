#pragma once
#include"../Geometry.h"
//���@�̒e���Ǘ�����N���X
class Shot
{
private:
	Rect rect_ = {};
	Vector2 vel_ = {};
	int handle_=0;
	int imgIdx_ = 0;
	//���e�͐����Ă���t���O(true:�����Ă� false:����ł�)
	bool isEnabled_ = false;
public:
	Shot(int handle);
	void SetVelocity(const Vector2& vec);
	const Rect& GetRect()const;
	/// <summary>
	/// ��ڂ��I���Ď���
	/// </summary>
	void Kill();
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

