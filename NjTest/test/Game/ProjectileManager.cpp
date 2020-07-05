#include "ProjectileManager.h"
#include"Projectile.h"

std::vector<std::shared_ptr<Projectile>>& 
ProjectileManager::Projectiles() {
	return projectiles_;
}
void 
ProjectileManager::AddProjectile(Projectile* p) {
	projectiles_.emplace_back(p);
}
void 
ProjectileManager::Update() {
	for (auto& p : projectiles_) {
		p->Update();
	}
}
void 
ProjectileManager::Draw() {
	for (auto& p : projectiles_) {
		p->Draw();
	}
}