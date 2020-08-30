#pragma once
#include<memory>
#include"../Geometry.h"


class Camera;
class Collider;
class Input;
struct CollisionInfo;
///キャラクター基底クラス
class Character
{
	friend Collider;
protected:
	Position2f pos_;//キャラ中心座標
	std::shared_ptr<Camera> camera_;
	//生きてるかどうか
	//だけど、一時的に休眠状態のときも
	//falseになったりする
	bool isActive_ = true;
public:
	/// <summary>
	/// そのキャラが生きてるかどうか
	/// </summary>
	/// <returns>true:生きてる/false:死んでる</returns>
	bool IsActive()const { return isActive_; }

	/// <summary>
	/// キャラクタ基底クラスコンストラクタ
	/// </summary>
	/// <param name="camera">カメラへの参照(表示に使用)</param>
	Character(std::shared_ptr<Camera> camera);

	///座標アクセス
	virtual const Vector2f& GetPosition()const;
	virtual void SetPosition(const Vector2f& vel);

	virtual ~Character() = default;
	///<summary>キャラクターの更新を行う</summary>
	virtual void Update() = 0;
	///<summary>キャラクターの表示を行う</summary>
	virtual void Draw() = 0;
	/// <summary>
	/// 衝突判定イベント
	/// </summary>
	/// <param name="me">衝突情報自分</param>
	/// <param name="another">衝突情報相手</param>
	virtual void OnHit(CollisionInfo& me,CollisionInfo& another) = 0;

};
/// <summary>
/// キャラクターが今落ちているかどうか
/// </summary>
/// <param name="velocity">速度ベクトル</param>
/// <returns>true 落下中</returns>
inline bool IsFalling(const Vector2f& velocity) {
	return velocity.y >= 0;
}