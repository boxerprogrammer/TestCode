#pragma once
#include"../Geometry.h"
#include<memory>
class EnemyFactory;
//���@�̒e���Ǘ�����N���X
class Missle
{
private:
	Rect rect_ = {};
	Vector2 vel_ = {};
	int handle_ = 0;
	int imgIdx_ = 0;
	//���e�͐����Ă���t���O(true:�����Ă� false:����ł�)
	bool isEnabled_ = false;
	std::shared_ptr<EnemyFactory> ef_;
public:
	Missle(int handle,std::shared_ptr<EnemyFactory> ef);
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

