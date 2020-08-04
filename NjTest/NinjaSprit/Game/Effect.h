#pragma once

#include<memory>
#include<list>
#include"../Geometry.h"
#include<memory>
class Camera;
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
	std::shared_ptr<Camera> camera_;
public:
	Effect(const Position2f& p, std::shared_ptr<Camera> camera):pos_(p),camera_(camera) {};
	bool IsDeletable()const {
		return isDeletable_;
	}
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

class EffectManager {
private:
	std::list<std::shared_ptr<Effect>> effects_;
	std::shared_ptr<Camera> camera_;
public:
	EffectManager(std::shared_ptr<Camera> c);
	void EmitBlood(const Position2f& p,bool isTurn=false);
	void EmitBlow3(const Position2f& p, bool isTurn = false);
	void Update();
	void Draw();

};