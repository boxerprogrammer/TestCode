#pragma once

#include<list>
#include<memory>
#include"../Geometry.h"
#include"Item.h"

/// <summary>
/// �A�C�e�����Ǘ�����
/// </summary>
class ItemFactory
{
private:
	std::list<std::shared_ptr<Item>> items_;//�A�C�e��(����)
	int handle_;//�������ƂɂȂ�n���h��
public:
	ItemFactory();
	~ItemFactory();
	
	void Update();
	void Draw();

	/// <summary>
	/// �S�A�C�e�����𓾂�(�����蔻��p)
	/// </summary>
	/// <returns></returns>
	std::list<std::shared_ptr<Item>>& GetItems();

	/// <summary>
	/// �V�����A�C�e����ǉ�����(ItemBringer�����񂾏ꏊ��)
	/// </summary>
	/// <param name="pos">�A�C�e����u���ꏊ</param>
	/// <param name="type">�A�C�e�����</param>
	void AddItem(const Position2& pos,ItemType type );
};

