#include "ItemFactory.h"
#include<DxLib.h>
#include"../DrawFunctions.h"

ItemFactory::ItemFactory() {
	handle_ = my::MyLoadGraph(L"./Data/img/game/items.png");
}
ItemFactory::~ItemFactory() {

}

void ItemFactory::Update()
{
	for (auto& item : items_) {
		item->Update();
	}
	items_.remove_if([](auto& item) {
		return !item->IsEnabled();
		});
}

void ItemFactory::Draw()
{
	for (auto& item : items_) {
		item->Draw();
	}
}

std::list<std::shared_ptr<Item>>& ItemFactory::GetItems()
{
	return items_;
}

void ItemFactory::AddItem(const Position2& pos,ItemType type)
{
	items_.push_back(std::make_shared<Item>(handle_, type));
	items_.back()->SetPosition(pos);
}
