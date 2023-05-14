#pragma once

#include"../Geometry.h"
#include<memory>
class InputState;
class Enemy;
class Force;
class Player
{
private:
	/*Enemy* enemy_=nullptr;*/
	Rect rect_;//自機の現在の座標
	int handle_;//自機画像ハンドル
	int burstH_;//爆発ハンドル
	int imgIdx_ = 0;//表示する画像のインデックス
	int ultimateTimer_ = 0;//無敵時間
	bool hasMissile_ = false;
	std::shared_ptr<Force> force_;//フォース

	void (Player::* updateFunc_)(const InputState& input);
	void (Player::* drawFunc_)();

	void NormalUpdate(const InputState& input);
	void NormalDraw();

	void BurstUpdate(const InputState& input);
	void BurstDraw();

public:
	Player();
	void SetHasMissile(bool hasMissile);
	bool HasMissile()const;

	/// <summary>
	/// 当たり判定対象か？
	/// </summary>
	/// <returns>true 当たり判定対象 / false対象外</returns>
	bool IsCollidable()const;

	const Position2& GetPosition()const;

	/// <summary>
	/// プレイヤーの状態の更新
	/// </summary>
	/// <param name="input">入力</param>
	void Update(const InputState& input);

	/// <summary>
	/// プレイヤーの描画
	/// </summary>
	void Draw();

	void OnDamage();

	const Rect& GetRect()const;

	std::shared_ptr<Force> GetForce();
};

