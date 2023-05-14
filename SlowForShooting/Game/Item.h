#pragma once
#include"../Geometry.h"
#include"ItemType.h"


/// <summary>
/// アイテムを表すクラス
/// </summary>
class Item
{
private:
	int handle_;//表示用
	ItemType type_;//アイテム種別(取得したときにプレイヤーに何を起こすのか)
	Rect rect_;//当たり矩形(プレイヤー自身との当たり)
	bool isEnabled_;//表示、当たり、を有効にするかどうか
public:
	Item(int handle,ItemType type);
	~Item();
	/// <summary>
	/// 矩形情報を返す
	/// </summary>
	/// <returns>矩形情報</returns>
	const Rect& GetRect()const;

	/// <summary>
	/// 現在位置を設定する
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPosition(const Position2& pos);

	/// <summary>
	/// アイテム種別を返す
	/// </summary>
	/// <returns>アイテム種別</returns>
	const ItemType GetItemType()const;

	/// <summary>
	/// 有効かどうか(当たるか？表示するか？)
	/// 無効なら消される
	/// </summary>
	/// <returns>true:有効 / false:無効</returns>
	bool IsEnabled()const;

	/// <summary>
	/// 当たったイベント
	/// </summary>
	void OnTouch();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 表示関数
	/// </summary>
	void Draw();
};

