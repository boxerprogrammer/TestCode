#pragma once

#include"../Character.h"
class Camera;
/// <summary>
/// 速度をもって飛んでいくオブジェクト基底クラス
/// </summary>
class Projectile : public Character
{
protected:
	Vector2f vel_;
//	bool isActive_=false;//有効フラグ
public:
	/// <summary>
	/// 死んだかどうか
	/// </summary>
	/// <returns>true:死んでる false:生きてる</returns>
	bool IsDead()const;
	/// <param name="camera">カメラへの参照</param>
	Projectile(std::shared_ptr<Camera> camera);
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

