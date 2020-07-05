#pragma once

#include<memory>
#include<vector>

class Projectile;
class ProjectileManager
{
private:
	std::vector<std::shared_ptr<Projectile>> projectiles_;
public:
	std::vector<std::shared_ptr<Projectile>>& Projectiles();
	void AddProjectile(Projectile*);
	void Update();
	void Draw();
};

