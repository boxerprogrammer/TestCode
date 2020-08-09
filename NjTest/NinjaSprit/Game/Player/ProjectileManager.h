#pragma once

#include<memory>
#include<list>

class Projectile;
using ProjectileList_t = std::list<std::shared_ptr<Projectile>>;
/// <summary>
/// ��ѓ���Ǘ��N���X
/// </summary>
class ProjectileManager
{
private:
	ProjectileList_t projectiles_;
public:
	/// <summary>
	/// ���݂̔�ѓ���X�g�ւ̎Q�Ƃ�Ԃ�
	/// </summary>
	/// <returns>��ѓ���X�g</returns>
	ProjectileList_t& GetProjectiles();
	void AddProjectile(Projectile*);
	/// <summary>
	/// ��ѓ���ׂĂ�Update
	/// </summary>
	void Update();
	/// <summary>
	/// ��ѓ���ׂĂ�`��
	/// </summary>
	void Draw();
};

