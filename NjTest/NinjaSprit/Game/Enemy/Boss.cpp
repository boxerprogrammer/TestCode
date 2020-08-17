#include "Boss.h"
#include"../../Scene/GameplayingScene.h"

Boss::Boss(GameplayingScene* gs):gameScene_(gs),
Enemy(gs->GetPlayer(),gs->GetCamera()){

}