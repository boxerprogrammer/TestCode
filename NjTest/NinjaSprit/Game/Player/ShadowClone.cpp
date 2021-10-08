#include "ShadowClone.h"
#include"../Camera.h"
#include"SwordEquip.h"
#include"BombEquip.h"
#include"ShurikenEquip.h"
#include"../../Scene/GameplayingScene.h"
#include"ChainEquip.h"
#include"Player.h"
#include<DxLib.h>
using namespace std;
ShadowClone::ShadowClone(GameplayingScene* gs,Player* p, std::shared_ptr<Camera> cam):
player_(p),
camera_(cam){
	equipments_[sword_equip_no] = (make_shared<SwordEquip>(gs->GetPlayer(), gs->GetCollisionManager(), gs->GetCamera(), this));
	equipments_[bomb_equip_no]=(make_shared<BombEquip>(gs->GetProjectileManager(), gs->GetCollisionManager(), gs->GetCamera()));
	equipments_[shuriken_equip_no]=(make_shared<ShurikenEquip>(gs->GetProjectileManager(), gs->GetCollisionManager(), gs->GetCamera()));
	equipments_[chain_equip_no]=(make_shared<ChainEquip>(gs->GetPlayer(), gs->GetCollisionManager(), gs->GetCamera(),this));
}

const 
Position2f&
ShadowClone::GetPosition()const {
	return pos_;
}

/// <summary>
/// Œ»İ‚Ì•Ší‚ÅUŒ‚
/// </summary>
/// <param name="input">“ü—Íî•ñ</param>
void 
ShadowClone::Attack(const Input& input,size_t equipNo) {
	auto offset = pos_ - player_->GetPosition();
	equipments_[equipNo]->Attack(*player_, input,offset);
	currentEquipmentNo_ = equipNo;
}

void
ShadowClone::AdditionalInput(const Input& input) {
	equipments_[currentEquipmentNo_]->AdditionalInput(input);
}

void 
ShadowClone::Update(const Position2f& pos) {
	pos_ = pos;
	equipments_[currentEquipmentNo_]->Update();
}
void 
ShadowClone::Draw() {
	equipments_[currentEquipmentNo_]->Draw();
}