#include "BombEquip.h"
#include"ProjectileManager.h"
#include"BombShot.h"
#include"Player.h"
#include"../../Input.h"
#include"../CollisionManager.h"
#include"../CircleCollider.h"
#include<DxLib.h>


namespace {
	int throwH = -1;
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
	Vector2f vel(5, 0);
	if (player.GetDirection() == Player::Direction::right) {
		vel = { 10, 0 };
	}
	if (player.GetDirection() == Player::Direction::left) {
		vel = { -10, 0 };
	}
	if (input.IsPressed("up")) {
		vel = { 0, -10 };
	}
	if (input.IsPressed("down")) {
		vel = { 0, 10 };
	}
	PlaySoundMem(throwH, DX_PLAYTYPE_BACK);
	pm_.AddProjectile(new BombShot(player.Position(), vel,camera_));
	collisionManager_->AddCollider(new CircleCollider(pm_.GetProjectiles().back(),
		Circle(Position2f(0,0),20),
		tag_player_attack));
}