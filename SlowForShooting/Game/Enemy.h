#pragma once
#include<memory>
#include"../Geometry.h"
#include"EnemyType.h"



class Player;
class BulletFactory;
/// <summary>
/// 敵の基底クラス
/// </summary>
class Enemy
{
protected://子クラスには見せたい情報
	std::shared_ptr<Player> player_ = nullptr;
	Rect rect_;//敵の矩形
	bool isEnabled_ = true;
	//発射するときに使える弾工場への参照
	std::shared_ptr<BulletFactory> bulletFactory_;
public:
	Enemy(std::shared_ptr<Player> player,const Position2& pos, std::shared_ptr<BulletFactory> bFactory);
	const Rect& GetRect()const;

	void SetPos(const Position2& pos);

	/// <summary>
	/// ダメージを受けた
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	virtual void OnDamage(int damage)=0;

	/// <summary>
	/// この敵は生きてるか？
	/// </summary>
	/// <returns>true 生きてる / false 死んでる</returns>
	bool IsEnabled()const;

	/// <summary>
	/// 衝突可能か？
	/// </summary>
	/// <returns>true 衝突可能  /  false 衝突不可</returns>
	virtual bool IsCollidable()const = 0;

	/// <summary>
	/// 敵オブジェクトの更新
	/// </summary>
	virtual void Update()=0;

	/// <summary>
	/// 敵オブジェクトの描画
	/// </summary>
	virtual void Draw()=0;

	/// <summary>
	/// 自分自身のクローンを返す
	/// </summary>
	/// <returns>敵オブジェクトのクローン</returns>
	virtual std::shared_ptr<Enemy> Clone() = 0;

};

