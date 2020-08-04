#include "BombEquip.h"
#include"ProjectileManager.h"
#include"BombShot.h"
#include"Player.h"
#include"../../Input/Input.h"
#include"../Collision/CollisionManager.h"
#include"../Collision/CircleCollider.h"
#include<DxLib.h>


namespace {
	int throwH = -1;
	constexpr float equip_offset_y = -48.0f;
	constexpr float bomb_shot_speed = 10.0f;
	constexpr float bomb_radius = 20.0f;
}
using namespace std;
BombEquip::BombEquip(ProjectileManager& pm,std::shared_ptr<CollisionManager> col,shared_ptr<Camera> camera):
	pm_(pm),
Equipment(col,camera){
	if (throwH == -1) {
		throwH=LoadSoundMem(L"Resource/Sound/Game/bombthrow.wav");
	}
}

void 
BombEquip::Attack(const Player& player, const Input& input) {
	Vector2f vel=Vector2f::ZERO;
	if (player.GetDirection() == Player::Direction::right) {
		vel = Vector2f::RIGHT;
	}
	if (player.GetDirection() == Player::Direction::left) {
		vel = Vector2f::LEFT;
	}
	if (input.IsPressed("up")) {
		vel = Vector2f::UP;
	}
	if (input.IsPressed("down")) {
		vel = Vector2f::DOWN;
	}
	vel *= bomb_shot_speed;
	PlaySoundMem(throwH, DX_PLAYTYPE_BACK);
	pm_.AddProjectile(new BombShot(player.Position()+ Vector2f(0, equip_offset_y), vel,camera_));
	collisionManager_->AddCollider(new CircleCollider(pm_.GetProjectiles().back(),
		Circle(Position2f::ZERO,bomb_radius),
		tag_player_attack));
}