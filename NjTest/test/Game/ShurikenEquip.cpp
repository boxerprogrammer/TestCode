#include "ShurikenEquip.h"
#include"ProjectileManager.h"
#include<DxLib.h>
#include"../Geometry.h"
#include"../Input.h"
#include"ShurikenShot.h"
#include"Player.h"
namespace {
	int throwH = -1;
}

ShurikenEquip::ShurikenEquip(ProjectileManager& pm) :pm_(pm) {
	if (throwH == -1) {
		throwH = LoadSoundMem(L"Resource/Sound/Game/shurikenthrow.wav");
	}


}

void
ShurikenEquip::Attack(const Player& player, const Input& input) {
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
		vel += { 1, 0 };
	}
	vel *= 10.0f;
	PlaySoundMem(throwH, DX_PLAYTYPE_BACK);
	pm_.AddProjectile(new ShurikenShot(player.Position(), vel));
}