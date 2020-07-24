#pragma once

#include<memory>
#include<list>

class Projectile;
class ProjectileManager
{
private:
	std::list<std::shared_ptr<Projectile>> projectiles_;
public:
	std::list<std::shared_ptr<Projectile>>& GetProjectiles();
	void AddProjectile(Projectile*);
	void Update();
	void Draw();
};

