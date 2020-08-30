#include "CollisionManager.h"
#include"Collider.h"
#include"../Character.h"
#include<algorithm>
#include<DxLib.h>
#include"../../System/Debugger.h"

using namespace std;
namespace {
	//当たりパターンを登録しておく
	pair<string, string> hit_combination_table[] = { 
		{tag_enemy_damage,tag_player_attack},
		{tag_enemy_bullet,tag_player_attack},
		{tag_enemy_bullet,tag_player_damage},
		{tag_enemy_attack,tag_player_damage}
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
			if (!colB->IsActive())continue;
			
			auto cnt= count(
				begin(hit_combination_table),
				end(hit_combination_table),
				make_pair(colA->GetTag(),colB->GetTag()));
			if (cnt == 0)continue;

			CollisionInfo colInfoA = { colA };
			CollisionInfo colInfoB = { colB };
			if (colA->IsHit(colInfoB) || colB->IsHit(colInfoA)) {
				if (colInfoA.vec == Vector2f::ZERO) {
					colInfoA.vec = -colInfoB.vec;
				}
				colA->GetOwner()->OnHit(colInfoA,colInfoB);
				
				if (colA->GetOwner()->IsActive()) {
					colA->Sleep();
				}else {
					colA->Suicide();
				}
				
				
				colB->GetOwner()->OnHit(colInfoB,colInfoA);
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
		if (col->IsDeletable() || col->OwnerIsDead())continue;
		col->Awake();
	}
}
std::shared_ptr<Collider> 
CollisionManager::AddCollider(Collider* collider) {
	colliders_.emplace_back(collider);
	return colliders_.back();
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