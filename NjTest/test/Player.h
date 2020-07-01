#pragma once
struct Enemy;//プロトタイプ宣言
struct Player
{
	//宣言時においては中身を
	//知る必要がない。
	//ポインタ型(参照型)はサイズが
	//決まっている(4バイトか8バイト)
	Enemy* enemy_;//Enemy*のポインタ
};

int PlayerFunction(Player* p);

static int currentPlayerNo = 0;
