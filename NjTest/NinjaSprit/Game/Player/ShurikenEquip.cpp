#include "ShurikenEquip.h"
#include"../ProjectileManager.h"
#include"../Collision/CollisionManager.h"
#include"../Collision/CircleCollider.h"
#include<DxLib.h>
#include"../../Geometry.h"
#include"../../Input/Input.h"
#include"ShurikenShot.h"
#include"Player.h"
#include"../../System/File.h"
#include"../../System/FileManager.h"
namespace {
	
	constexpr float equip_offset_y = -48.0f;
}
using namespace std;

ShurikenEquip::ShurikenEquip(ProjectileManager& pm,shared_ptr<CollisionManager> cm, shared_ptr<Camera> camera) :
	projectileMgr_(pm),
Equipment(cm,camera){
	auto& fileMgr = FileManager::Instance();
	if (throwH_ == -1) {
		throwH_ = fileMgr.Load(L"Resource/Sound/Game/shurikenthrow.wav")->Handle();
	}


}

void
ShurikenEquip::Attack(const Player& player, const Input& input, Vector2f offset) {
	Vector2f vel(0, 0);
	if (input.IsPressed("right")) {
		vel += { 1, 0 };
	}
	if (input.IsPressed("left")) {
		vel += { -1, 0 };
	}
	if (input.IsPressed("up")) {
		vel += { 0, -1 };
	}
	if (input.IsPressed("down")) {
		vel += { 0, 1 };
	}
	if (vel.x == 0.0f && vel.y == 0.0f) {
		if (player.GetDirection() == Player::Direction::right) {
			vel += { 1, 0 };
		}
		else {
			vel += { -1, 0 };
		}
	}
	vel.Normalize();
	vel *= 10.0f;
	PlaySoundMem(throwH_, DX_PLAYTYPE_BACK);
	projectileMgr_.AddProjectile(new ShurikenShot(player.Position()+offset+Vector2f(0,equip_offset_y), vel,camera_));
	collisionManager_->AddCollider(new CircleCollider(
		projectileMgr_.GetProjectiles().back(),
		{ {0,0},10 },
		tag_player_attack
	));

}