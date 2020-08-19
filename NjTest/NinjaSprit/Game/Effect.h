#pragma once

#include<memory>
#include<list>
#include"../Geometry.h"
#include<memory>
class Camera;
/// <summary>
/// エフェクト基底クラス
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
	/// <param name="pos">表示座標</param>
	/// <param name="camera">カメラへの参照</param>
	Effect(const Position2f& pos, std::shared_ptr<Camera> camera):pos_(pos),camera_(camera) {};

	/// <summary>
	/// このエフェクトは削除可能か？
	/// </summary>
	/// <returns>true:削除可能 false:削除不可</returns>
	bool IsDeletable()const {
		return isDeletable_;
	}
	/// <summary>
	/// エフェクトフレーム更新
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// エフェクト表示
	/// </summary>
	virtual void Draw() = 0;
};

/// <summary>
/// エフェクト管理クラス
/// </summary>
class EffectManager {
private:
	int bloodH_ = -1;
	int bloodSE_ = -1;
	int blowH_ = -1;
	int hitSE_ = -1;
	int groundExpH_ = -1;
	int smokeExpH_ = -1;
	int energyBallH_ = -1;
	std::list<std::shared_ptr<Effect>> effects_;
	std::shared_ptr<Camera> camera_;
public:
	/// <param name="camera">カメラへの参照</param>
	EffectManager(std::shared_ptr<Camera> camera);
	/// <summary>
	/// 血しぶきエフェクト発生
	/// </summary>
	/// <param name="pos">表示座標</param>
	/// <param name="isTurn">反転フラグ</param>
	void EmitBlood(const Position2f& pos,bool isTurn=false);

	/// <summary>
	/// 地面爆発エフェクト発生
	/// </summary>
	/// <param name="pos">表示座標</param>
	/// <param name="isTurn">反転フラグ</param>
	void EmitGroundExplosion(const Position2f& pos, bool isTurn = false);

	/// <summary>
	/// 煙幕爆発エフェクト発生
	/// </summary>
	/// <param name="pos">表示座標</param>
	/// <param name="isTurn">反転フラグ</param>
	void EmitSmokeExplosion(const Position2f& pos, bool isTurn = false);

	/// <summary>
	/// エネルギー球エフェクト発生
	/// </summary>
	/// <param name="pos">表示座標</param>
	/// <param name="isTurn">反転フラグ</param>
	void EmitEnergyBall(const Position2f& pos, int lifetime=60,bool isTurn = false);

	/// <summary>
	/// 小爆発３つエフェクト発生
	/// </summary>
	/// <param name="pos">表示座標</param>
	/// <param name="isTurn">反転フラグ</param>
	void EmitBlow3(const Position2f& pos, bool isTurn = false);

	/// <summary>
	/// エフェクトすべてUpdate
	/// </summary>
	void Update();

	/// <summary>
	/// エフェクトすべて描画
	/// </summary>
	void Draw();

};