#pragma once
struct Player;
struct Enemy
{
	Player* player_;
	int no_;
	float life_;
};

static float GetEnemyLife(Enemy* enemy) {
	return enemy->life_;
}

