#include "SideSpawner.h"
#include"Enemy.h"
#include"../../Geometry.h"
#include"EnemyManager.h"
#include"../Collision/CollisionManager.h"
#include"../Collision/CircleCollider.h"
#include"../Camera.h"
#include<random>

using namespace std;

namespace {
	mt19937 mt;
}

SideSpawner::SideSpawner(const Position2f& pos, Enemy* prototype, std::shared_ptr<EnemyManager> em, std::shared_ptr<CollisionManager> cm,shared_ptr<Camera> c,
	int averageSpawnFrame,int limitSpawnNum,bool needBalance):
collisionManager_(cm)
,Spawner(pos,prototype,em,c),
averageSpawnFrame_(averageSpawnFrame),
limitSpawnNum_(limitSpawnNum),
needBalance_(needBalance){

}

/// <summary>
/// ���Ԃ��Ƃ̔����Ȃǂ��K�v�Ȃ��ߖ��t���[���X�V���s��
/// </summary>
void 
SideSpawner::Update() {
	auto rc = camera_->GetViewRange();
	if (pos_.x < rc.Left() || rc.Right() < pos_.x)return;

	//��������������
	if (limitSpawnNum_ > 0) {
		//�Ď����̓G�̒��Ŏ���ł�����̂��폜
		regulators_.remove_if([](auto& reg) {return reg.ownedWRef.expired(); });
		//�폜���������Ő��𒴂��Ă����琶�����Ȃ�
		if (regulators_.size() >= limitSpawnNum_) {
			return;
		}
	}

	static bool fromRight = false;
	auto variance=averageSpawnFrame_ / 3;//���U
	if (++frame_ % (averageSpawnFrame_+rand()%(variance*2)-variance) == 0) {
		auto clone=CreateClone();
		if (clone == nullptr)return;

		
		fromRight = !fromRight;
		if (needBalance_&&limitSpawnNum_ > 0) {
			auto count = std::count_if(regulators_.begin(), regulators_.end(), [](auto& reg) {
				return reg.fromRight;
				});
			fromRight = count*2 <= regulators_.size();
		}
		if (fromRight) {
			clone->SetPosition({ rc.Left() - 36.0f,480.0f });
		}
		else {
			clone->SetPosition({ rc.Right() + 36.0f,480.0f });
		}

		enemyManager_->AddEnemy(clone);
		auto shr = enemyManager_->Enemies().back();
		shr->SetWeakRef(shr);
		//��������������c
		if (limitSpawnNum_ > 0) {
			regulators_.emplace_back(enemyManager_->Enemies().back(),fromRight);
		}

		collisionManager_->AddCollider(
			new CircleCollider(enemyManager_->Enemies().back(),
			Circle(Position2f(0, 0), 50),
			tag_enemy_damage));
	}
}
