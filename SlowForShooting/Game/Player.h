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
	Rect rect_;//���@�̌��݂̍��W
	int handle_;//���@�摜�n���h��
	int burstH_;//�����n���h��
	int imgIdx_ = 0;//�\������摜�̃C���f�b�N�X
	int ultimateTimer_ = 0;//���G����
	bool hasMissile_ = false;
	std::shared_ptr<Force> force_;//�t�H�[�X

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
	/// �����蔻��Ώۂ��H
	/// </summary>
	/// <returns>true �����蔻��Ώ� / false�ΏۊO</returns>
	bool IsCollidable()const;

	const Position2& GetPosition()const;

	/// <summary>
	/// �v���C���[�̏�Ԃ̍X�V
	/// </summary>
	/// <param name="input">����</param>
	void Update(const InputState& input);

	/// <summary>
	/// �v���C���[�̕`��
	/// </summary>
	void Draw();

	void OnDamage();

	const Rect& GetRect()const;

	std::shared_ptr<Force> GetForce();
};

