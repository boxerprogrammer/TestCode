#include "Spawner.h"
#include"Enemy.h"
#include"EnemyManager.h"

using namespace std;


Spawner::Spawner(const Position2f& pos, Enemy* prototype, shared_ptr<EnemyManager>& em,shared_ptr<Camera> c)
:
pos_(pos),
prototype_(prototype),
enemyManager_(em),
camera_(c){


}

Enemy* 
Spawner::CreateClone() {
	return prototype_->MakeClone();
}