#pragma once
#include "Collider.h"
class FanCollider :
    public Collider
{
public:

	FanCollider(std::shared_ptr<Character> owner, const Capsule& cup, const char* tag = "", bool isImmortal = false);
	~FanCollider();
	///外からいじれるように参照を返す
	FanShape& GetFanShape();

	/// <summary>
	/// ヒットしたかどうかを返す
	/// </summary>
	/// <param name="col">相手側コライダー</param>
	/// <returns>true:衝突、false:非衝突</returns>
	bool IsHit(CollisionInfo& col)override;//⑤派生クラスもCollisionInfoに変更

	/// <summary>
	/// デバッグ用。もしデバッグ円やデバッグ矩形を表示したい場合は
	///表示する
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 実際の座標を返す(扇形の中心点の座標)
	/// </summary>
	/// <returns>持ち主を考慮した起点の座標</returns>
	const Vector2f AcutualPosition();
};

