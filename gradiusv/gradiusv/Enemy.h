#pragma once

#include"Geometry.h"

//プロトタイプ宣言ですね
class EnemyFactory;

//敵クラス
class Enemy
{
	friend EnemyFactory;
private:
	Position2f _pos;
	bool isDead = false;

	//↓ファクトリに入れてもらうやつ
	int _pictH;//敵画像用
	int _expH;//爆発用

	//コピーも代入も禁止する
	Enemy(const Enemy&);
	void operator=(const Enemy&);

	//コンストラクタ
	Enemy();
public:
	
	~Enemy();
	void Update();
	void Draw();
};

