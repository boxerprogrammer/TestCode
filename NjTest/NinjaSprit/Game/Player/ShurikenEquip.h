#pragma once
#include "Equipment.h"
class ProjectileManager;
class Camera;
/// <summary>
/// �藠������
/// </summary>
class ShurikenEquip :
    public Equipment
{
private:
	int throwH_ = -1;
	ProjectileManager& projectileMgr_;
public:
	/// <summary>
	/// �藠�������R���X�g���N�^
	/// </summary>
	/// <param name="projectileMgr">��ѓ���}�l�[�W��</param>
	/// <param name="collisionMgr">�����蔻��}�l�[�W��</param>
	/// <param name="camera">�J����</param>
	ShurikenEquip(ProjectileManager& projectileMgr,
		std::shared_ptr<CollisionManager> collisionMgr,
		std::shared_ptr<Camera> camera);
	/// <summary>
	/// �藠���U���������܂�
	/// </summary>
	/// <param name="player">�v���C���[�ւ̎Q��</param>
	/// <param name="input">���͎Q��</param>
	void Attack(const Player& p, const Input& i, Vector2f offset = Vector2f::ZERO) override;
};

