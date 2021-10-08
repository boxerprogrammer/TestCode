#pragma once
#include"../../Geometry.h"
#include<memory>
#include<array>
class Player;
class Camera;
class Equipment;
class GameplayingScene;
class Input;
/// <summary>
/// プレイヤーの分身
/// </summary>
class ShadowClone
{
	friend Player;
	size_t currentEquipmentNo_ = 0;
	Position2f pos_;
	Player* player_;//親への参照
	std::shared_ptr<Camera> camera_;//カメラへの参照
	std::array<std::shared_ptr<Equipment>,4> equipments_;//装備
	void AdditionalInput(const Input& input);
public:
	ShadowClone(GameplayingScene* gs,Player* p, std::shared_ptr<Camera> cam);
	~ShadowClone() = default;
	const Position2f& GetPosition()const;
	/// <summary>
	/// 現在の武器で攻撃
	/// </summary>
	/// <param name="input">入力情報</param>
	/// <param name="equipNo">武器情報</param>
	void Attack(const Input& input,size_t equipNo);
	void Update(const Position2f& pos);
	void Draw();
};

