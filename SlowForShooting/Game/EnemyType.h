#pragma once
/// <summary>
/// 敵種別
/// </summary>
enum class EnemyType {
	Patapata,//一番の雑魚
	Insect,//編隊を組む虫
	Rock,//岩
	VolcanicRock,//火山岩
	ItemBringer,//アイテム持ってくるやつ
	MissileBringer,//ミサイル持ってくるやつ
};
enum class BurstType {
	ZakoBurst,//雑魚爆発
};
/// <summary>
/// データIDに基づく敵および発生種別
/// </summary>
enum class EnemySpawnerId {
	None,//0:なにもなし
	OnePatapata,//1:単独パタパタ
	TeamPatapata,//2:編隊パタパタ
	InfinitePatapata,//3:無限パタパタ
	TeamInsect,//4:編隊ー虫
	InfiniteRock,//5:岩無限沸き
	PosInfVolcanicRock,//6:無限火山岩湧き
	OneItemBringer,//7:アイテム落とす奴が単独
	OneMissleBringer//8:ミサイル落とす奴が単独
};