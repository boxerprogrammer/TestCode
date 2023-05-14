#pragma once

#include<list>
#include<memory>
#include"../Geometry.h"
#include"Item.h"

/// <summary>
/// アイテムを管理する
/// </summary>
class ItemFactory
{
private:
	std::list<std::shared_ptr<Item>> items_;//アイテム(複数)
	int handle_;//おおもとになるハンドル
public:
	ItemFactory();
	~ItemFactory();
	
	void Update();
	void Draw();

	/// <summary>
	/// 全アイテム情報を得る(当たり判定用)
	/// </summary>
	/// <returns></returns>
	std::list<std::shared_ptr<Item>>& GetItems();

	/// <summary>
	/// 新しくアイテムを追加する(ItemBringerが死んだ場所に)
	/// </summary>
	/// <param name="pos">アイテムを置く場所</param>
	/// <param name="type">アイテム種別</param>
	void AddItem(const Position2& pos,ItemType type );
};

