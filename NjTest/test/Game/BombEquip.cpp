#include "BombEquip.h"
#include"ProjectileManager.h"
#include"BombShot.h"
#include"Player.h"
#include"../Input.h"

BombEquip::BombEquip(ProjectileManager& pm):pm_(pm){}

void 
BombEquip::Attack(const Player& player, const Input& input) {
	Vector2f vel(5, 0);
	if (input.IsPressed("right")) {
		vel = { 8, 0 };
	}
	if (input.IsPressed("left")) {
		vel = { -8, 0 };
	}
	if (input.IsPressed("up")) {
		vel = { 0, -8 };
	}
	if (input.IsPressed("down")) {
		vel = { 0, 8 };
	}
	pm_.AddProjectile( new BombShot(player.Position(), vel));
}