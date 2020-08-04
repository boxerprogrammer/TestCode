#include "ProjectileManager.h"
#include"Projectile.h"
#include<algorithm>

using namespace std;

//std::vector<std::shared_ptr<Projectile>>& 
//ProjectileManager::Projectiles() {
//	return projectiles_;
//}

std::list<std::shared_ptr<Projectile>>& 
ProjectileManager::GetProjectiles() {
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
	auto it=remove_if(projectiles_.begin(), projectiles_.end(),
		[](shared_ptr<Projectile>& p) {
			return p->IsDead();
		});
	projectiles_.erase(it, projectiles_.end());
}
void 
ProjectileManager::Draw() {
	for (auto& p : projectiles_) {
		p->Draw();
	}
}