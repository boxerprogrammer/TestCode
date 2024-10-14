#include "EnemyManager.h"
#include"Enemy.h"
void EnemyManager::Update()
{
	for (auto& enemy : enemies_) {
		enemy->Update();
	}
}

void EnemyManager::Draw()
{
	for (auto& enemy : enemies_) {
		enemy->Draw();
	}
}
