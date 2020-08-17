#pragma once
#include"../Character.h"
#include<vector>
#include<array>
#include<list>
///プレイヤークラス
class GameplayingScene;
class Equipment;
class Input;
class PlayerInputListener;

class CollisionManager;
/// <summary>
/// プレイヤークラス
/// </summary>
class Player : public Character {
	friend PlayerInputListener;
private:
	int maskH_ = -1;
	int changeSE_ = -1;
	int runH_[6] = { -1,-1,-1,-1, -1, -1 };
	int jumpH_[4] = { -1, -1, -1, -1 };
	int fallH_[2] = { -1, -1 };
	std::array<Position2f, 60> moveHistory_;
	size_t currentMoveIndex_ = 0;
	void SetCurrentPosition(Position2f& pos);
	const Position2f& GetBackTimePosition(size_t backFrame)const;

	Position2f lastPos_;
	float velY_ = 0.0f;
	float accelY_ = 0.0f;
	bool isRight_ = true;
	std::vector<std::shared_ptr<Equipment>> equipments_;
	size_t currentEquipmentNo_ = 0;
	std::shared_ptr<CollisionManager> collisionManager_;

	void ExitUpdate();
	//通常状態
	void NormalUpdate();
	//下降状態
	void FallUpdate();
	//上昇状態
	void RiseUpdate();
	using Updater_t = void (Player::*)();
	using Drawer_t = void (Player::*)();

	void ExitDraw();
	//通常描画
	void NormalDraw();
	//下降描画
	void FallDraw();
	//上昇描画
	void RiseDraw();

	Updater_t updater_;
	Drawer_t drawer_;
	GameplayingScene* gameScene_;



	/// <summary>
	/// 現在の装備の参照を返す(入力リスナ用)
	/// 鎖鎌時に追加入力を可能にするため
	/// </summary>
	/// <returns>現在の装備の参照</returns>
	std::shared_ptr<Equipment> CurrentEquipment();

public:
	enum class Direction {
		left,
		right
	};
	/// <summary>
	/// プレイヤーの現在の位置を返す
	/// </summary>
	/// <returns>向きを表すenum</returns>
	Direction GetDirection()const;

	/// <summary>
	/// 当たったときイベント
	/// </summary>
	/// <param name="colInfo">衝突情報</param>
	void OnHit(CollisionInfo& colInfo)override;

	/// <param name="owner">持ち主ゲームシーン</param>
	Player(GameplayingScene* owner);
	~Player();
	/// <summary>
	/// 現在の武器で攻撃
	/// </summary>
	/// <param name="input">入力情報</param>
	void Attack(const Input& input);
	/// <summary>
	/// ジャンプ
	/// </summary>
	void Jump();
	/// <summary>
	/// 武器切り替え
	/// </summary>
	void NextEquip();
	/// <summary>
	/// シーン等から座標を設定する用
	///通常は自分でMoveするが、ステージ開始時に使用する
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPosition(const Position2f& pos);

	/// <summary>
	/// 現在の座標を返す
	/// </summary>
	/// <returns>現在の座標</returns>
	const Position2f& Position()const;

	/// <summary>
	/// 移動する
	/// </summary>
	/// <param name="vec">移動量</param>
	void Move(const Vector2f& vec);

	/// <summary>
	/// 毎フレーム座標や内部情報更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 毎フレーム描画
	/// </summary>
	void Draw()override;
	
	/// <summary>
	/// 現在の装備番号を返す
	/// UIに現在の装備を表示するため
	/// </summary>
	/// <returns>装備を表す識別番号</returns>
	size_t CurrentEquipmentNo()const;
};
