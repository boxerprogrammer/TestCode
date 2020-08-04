#include "EnemyManager.h"
#include"Enemy.h"
#include<algorithm>

using namespace std;

void 
EnemyManager::AddEnemy(Enemy* enemy) {
	enemies_.emplace_back(enemy);
}
list<shared_ptr<Enemy>>& 
EnemyManager::Enemies() {
	return enemies_;
}
void 
EnemyManager::Update() {
	for (auto& e : enemies_) {
		e->Update();
	}
	auto it = remove_if(enemies_.begin(), enemies_.end(), [](shared_ptr<Enemy>& e) {
		return e->IsDeletable();
		});
	enemies_.erase(it, enemies_.end());
}
void 
EnemyManager::Draw() {
	for (auto& e : enemies_) {
		e->Draw();
	}
}