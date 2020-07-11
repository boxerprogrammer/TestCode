#pragma once

#include<memory>
#include<list>
#include"../Geometry.h"

/// <summary>
/// エフェクトクラス
/// アニメーションを表示し、表示し終わったら消える
/// </summary>
class Effect
{
protected:
	int frame_=0;
	Position2f pos_;
	bool isDeletable_ = false;
public:
	Effect(const Position2f& p):pos_(p) {};
	bool IsDeletable()const {
		return isDeletable_;
	}
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

class EffectManager {
private:
	std::list<std::shared_ptr<Effect>> effects_;
public:
	EffectManager();
	void EmitBlood(const Position2f& p,bool isTurn=false);
	void Update();
	void Draw();

};