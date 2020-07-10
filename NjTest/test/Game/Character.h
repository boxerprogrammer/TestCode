#pragma once
#include<memory>
#include"../Geometry.h"

class Input;
///キャラクター基底クラス
class Character
{
protected:
	Position2f pos_;//キャラ中心座標
public:
	Character();
	virtual ~Character() = default;
	///<summary>キャラクターの更新を行う</summary>
	virtual void Update() = 0;
	///<summary>キャラクターの表示を行う</summary>
	virtual void Draw() = 0;
};

