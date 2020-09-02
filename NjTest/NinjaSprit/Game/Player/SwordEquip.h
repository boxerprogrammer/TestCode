#pragma once
#include "Equipment.h"
#include"../../Geometry.h"
class Player;
class CapsuleCollider;
class Camera;
class ShadowClone;
class FanCollider;
/// <summary>
/// 斬撃エフェクト用構造体(扇形を改造)
/// 不変条件：v1→v2は時計回りである事
///          :v1の大きさとv2の大きさは同じである事
/// </summary>
struct Slash {
	Position2f center; //中心座標
	Vector2f v1; //扇形の端ベクトル①
	Vector2f v2; //扇形の端ベクトル②
	Slash() :center(0, 0), v1(0, 0), v2(0, 0) {}//無効です
	Slash(const Position2f& p, const Vector2f& inv1, const Vector2f& inv2);
	Slash(const Position2f& p, float r, float angle);
	Slash(const Position2f& p, const Vector2f& inv, float angle);
	void Draw(int graphH,int psH=-1,int normalH=-1,unsigned color=0xffffff,bool isTurn=false);
	float Radius()const;
	void AddAngle1(float angle);
	void AddAngle2(float angle);
	float GetAngle()const;
};

/// <summary>
/// 刀装備
/// </summary>
class SwordEquip : public Equipment
{
private:
	//回転方向
	enum class RotateDir {
		cw,
		ccw
	};
	RotateDir rotDir_= RotateDir::cw;
	Slash slash_;
	ShadowClone* shadow_ = nullptr;
	int normalH_ = -1;
	FanCollider* fanCollider_ = nullptr;
	int frame_=0;
	std::shared_ptr<Player>& player_;
	float swingTargetAngle_ = 0.0f;
	float swingPerAngle_ = 0.0f;
	float currentAngle_ = 0.0f;
	int swingFrame_ = 0;
	Vector2f direction_;

public:
	/// <summary>
	/// 鎖鎌装備
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="collisionMgr">当たり判定マネージャ</param>
	/// <param name="camera">カメラ</param>
	SwordEquip(std::shared_ptr<Player>& player,
		std::shared_ptr<CollisionManager> collisionMgr,
		std::shared_ptr<Camera> camera, ShadowClone* shadow = nullptr);
	/// <summary>
	/// 鎖鎌攻撃を加えます
	/// </summary>
	/// <param name="player">プレイヤーへの参照</param>
	/// <param name="input">入力参照</param>
	void Attack(const Player& player, const Input& input, Vector2f offset = Vector2f::ZERO)override;
	/// <summary>
	/// 追加入力(振り回しのためだけのメソッド)
	/// </summary>
	/// <param name="input">入力情報</param>
	void AdditionalInput(const Input& input);
	/// <summary>
	/// 毎フレーム更新用
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画用
	/// </summary>
	void Draw()override;
};

