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
/// �v���C���[�̕��g
/// </summary>
class ShadowClone
{
	friend Player;
	size_t currentEquipmentNo_ = 0;
	Position2f pos_;
	Player* player_;//�e�ւ̎Q��
	std::shared_ptr<Camera> camera_;//�J�����ւ̎Q��
	std::array<std::shared_ptr<Equipment>,4> equipments_;//����
	void AdditionalInput(const Input& input);
public:
	ShadowClone(GameplayingScene* gs,Player* p, std::shared_ptr<Camera> cam);
	~ShadowClone() = default;
	const Position2f& GetPosition()const;
	/// <summary>
	/// ���݂̕���ōU��
	/// </summary>
	/// <param name="input">���͏��</param>
	/// <param name="equipNo">������</param>
	void Attack(const Input& input,size_t equipNo);
	void Update(const Position2f& pos);
	void Draw();
};

