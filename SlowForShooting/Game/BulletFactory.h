#pragma once

#include"../Geometry.h"
#include"Bullet.h"
#include<map>
#include<list>
#include<memory>

using BulletList_t = std::list<std::shared_ptr<Bullet>>;

class BulletFactory
{
	std::map<BulletType, int> bulletImgTable_;//ロードする画像のテーブル
	BulletList_t bullets_;//実際の弾のリスト
	
public:
	BulletFactory();
	void Create(BulletType bulletType, const Position2& pos, const Vector2& vel);//弾を生成
	/// <summary>
	/// 当たり判定用にすべての敵の弾への参照を取得する
	/// </summary>
	/// <returns>敵の弾のリスト</returns>
	const BulletList_t& GetBullets()const;
	void Update();
	void Draw();
};

