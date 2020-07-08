#pragma once

#include"Character.h"

/// <summary>
/// 速度をもって飛んでいくオブジェクト
/// </summary>
class Projectile : public Character
{
protected:
	Vector2f vel_;
	bool isActive_=false;
public:
	/// <summary>
	/// 死んだかどうか
	/// </summary>
	/// <returns></returns>
	bool IsDead()const;
	Projectile();
	virtual ~Projectile() = default;
	/// <summary>
	/// 現在座標に速度を加算する
	/// </summary>
	///<remarks>
	/// もし等速運動以外のことをやりたければUpdateを
	/// オーバーライドしてください
	///</remarks>
	virtual void Update()override;

	/// <summary>
	/// Drawは最後の派生クラスで実装する
	/// </summary>
	virtual void Draw() = 0;
};

