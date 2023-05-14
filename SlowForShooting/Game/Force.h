#pragma once
#include"../Geometry.h"
#include<memory>
class Player;

/// <summary>
/// 自機に対して着脱可能な武器
/// </summary>
class Force
{
	friend Player;
private:
	int handle_;
	int idxX_ = 0;
	int frame_ = 0;
	Rect imgRect_ = {};//描画のための矩形
	Rect rect_ = {};//当たり判定のための矩形
	Player& player_;//プレイヤーの参照
	int level_ = 0;//フォースレベル(0は表示も当たりもない)
	float pushSpeed = 0.0f;//切り離し速度
	void (Force::* updateFunc_)();

	//吸着状態のアップデート
	void AttachedUpdate();

	//浮遊状態のアップデート
	void FloatingUpdate();

	//前方または後方に切り離す
	//くっついてるのが前方なら前に飛ばす
	//くっついてるのが後ろなら後ろに飛ばす
	void SeparatingUpdate();

	//吸着チェック
	void CheckHoldable();

	//引き寄せ状態のアップデート
	//フォースが引き寄せられてる最中
	void PullingUpdate();

	enum class HoldType {
		front,//前方
		back//後方
	};
	HoldType holdType_= HoldType::front;
	void Pull();
	void Push();
public:
	Force(Player& player);
	const Rect& GetRect()const;
	void SetPosition(const Position2& pos);
	const Position2& GetPosition()const;
	int GetLevel()const;
	void Update();
	void Draw();
	void LevelUp();
	void ResetLevel();
	/// <summary>
	/// フォースが分離状態かどうか
	/// </summary>
	/// <returns>true:自機から切り離されてる　false:自機にくっついている</returns>
	bool IsSepareted()const;


	/// <summary>
	/// 引き寄せと、切り離しの切り替え
	/// </summary>
	void Switch();
};

