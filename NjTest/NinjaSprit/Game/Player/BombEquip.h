#pragma once
#include"Equipment.h"
#include<memory>
class ProjectileManager;
class CollisionManager;
class Camera;
class BombEquip : public Equipment
{
private:
	ProjectileManager& pm_;
	int throwH = -1;
public:
	/// <summary>
	/// ���e����
	/// </summary>
	/// <param name="projectileManager">��ѓ���}�l�[�W���ւ̎Q��</param>
	/// <param name="collisionManager">�����蔻��}�l�[�W���ւ̎Q��</param>
	/// <param name="camera">�J�����I�u�W�F�N�g�ւ̎Q��</param>
	BombEquip(ProjectileManager& projectileManager,
		std::shared_ptr<CollisionManager> collisionManager,
		std::shared_ptr<Camera> camera);
	/// <summary>
	/// ���e�U���������܂�
	/// </summary>
	/// <param name="player">�v���C���[�ւ̎Q��</param>
	/// <param name="input">���͎Q��</param>
	void Attack(const Player& player, const Input& input)override;
};


