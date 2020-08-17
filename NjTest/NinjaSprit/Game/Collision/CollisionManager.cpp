#include "CollisionManager.h"
#include"Collider.h"
#include"../Character.h"
#include<algorithm>
#include<DxLib.h>
#include"../../System/Debugger.h"

using namespace std;
namespace {
	pair<string, string> hit_combination_table[] = { 
		{tag_enemy_damage,tag_player_attack} 
	};
}



void 
CollisionManager::Update() {



	auto it=remove_if(colliders_.begin(), colliders_.end(),
		[](shared_ptr<Collider>& col) {
			return col->OwnerIsDead() || col->IsDeletable();
		});
	colliders_.erase(it,colliders_.end());
	for (auto& colA : colliders_) {
		if (colA->IsDeletable() || colA->OwnerIsDead())continue;
		if (!colA->IsActive())continue;
		for (auto& colB : colliders_) {
			if (colB->IsDeletable() || colB->OwnerIsDead())continue;
			if (colA == colB) {
				continue;
			}
			
			auto cnt= count(
				begin(hit_combination_table),
				end(hit_combination_table),
				make_pair(colA->GetTag(),colB->GetTag()));
			if (cnt == 0)continue;

			if (colA->IsHit(colB) || colB->IsHit(colA)) {
				CollisionInfo colInfoB = { colB };
				colA->GetOwner()->OnHit(colInfoB);
				if (colA->GetOwner()->IsActive()) {
					colA->Sleep();
				}else {
					colA->Suicide();
				}
				CollisionInfo colInfoA = { colA };
				colB->GetOwner()->OnHit(colInfoA);
				if (colB->GetOwner()->IsActive()) {
					colB->Sleep();
				}
				else {
					colB->Suicide();
				}
			}
		}
	}
	for (auto& col : colliders_) {
		col->Awake();
	}
}
void 
CollisionManager::AddCollider(Collider* collider) {
	colliders_.emplace_back(collider);
}

void 
CollisionManager::DebugDraw() {
#ifdef _DEBUG
	if (!Debugger::Instance().IsDebugMode())return;
	for (auto& c : colliders_) {
		c->Draw();
	}

	//当たり判定情報を出力
	DrawFormatString(300, 30, 0xffffff, L"コライダー数=%d", colliders_.size());
#endif
}