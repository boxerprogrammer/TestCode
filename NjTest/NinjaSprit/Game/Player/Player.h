#pragma once
#include"../Character.h"
#include<vector>
#include<array>
#include<list>
///�v���C���[�N���X
class GameplayingScene;
class Equipment;
class Input;
class PlayerInputListener;

class CollisionManager;
/// <summary>
/// �v���C���[�N���X
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
	//�ʏ���
	void NormalUpdate();
	//���~���
	void FallUpdate();
	//�㏸���
	void RiseUpdate();
	using Updater_t = void (Player::*)();
	using Drawer_t = void (Player::*)();

	void ExitDraw();
	//�ʏ�`��
	void NormalDraw();
	//���~�`��
	void FallDraw();
	//�㏸�`��
	void RiseDraw();

	Updater_t updater_;
	Drawer_t drawer_;
	GameplayingScene* gameScene_;



	/// <summary>
	/// ���݂̑����̎Q�Ƃ�Ԃ�(���̓��X�i�p)
	/// �������ɒǉ����͂��\�ɂ��邽��
	/// </summary>
	/// <returns>���݂̑����̎Q��</returns>
	std::shared_ptr<Equipment> CurrentEquipment();

public:
	enum class Direction {
		left,
		right
	};
	/// <summary>
	/// �v���C���[�̌��݂̈ʒu��Ԃ�
	/// </summary>
	/// <returns>������\��enum</returns>
	Direction GetDirection()const;

	/// <summary>
	/// ���������Ƃ��C�x���g
	/// </summary>
	/// <param name="colInfo">�Փˏ��</param>
	void OnHit(CollisionInfo& colInfo)override;

	/// <param name="owner">������Q�[���V�[��</param>
	Player(GameplayingScene* owner);
	~Player();
	/// <summary>
	/// ���݂̕���ōU��
	/// </summary>
	/// <param name="input">���͏��</param>
	void Attack(const Input& input);
	/// <summary>
	/// �W�����v
	/// </summary>
	void Jump();
	/// <summary>
	/// ����؂�ւ�
	/// </summary>
	void NextEquip();
	/// <summary>
	/// �V�[����������W��ݒ肷��p
	///�ʏ�͎�����Move���邪�A�X�e�[�W�J�n���Ɏg�p����
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPosition(const Position2f& pos);

	/// <summary>
	/// ���݂̍��W��Ԃ�
	/// </summary>
	/// <returns>���݂̍��W</returns>
	const Position2f& Position()const;

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="vec">�ړ���</param>
	void Move(const Vector2f& vec);

	/// <summary>
	/// ���t���[�����W��������X�V
	/// </summary>
	void Update()override;

	/// <summary>
	/// ���t���[���`��
	/// </summary>
	void Draw()override;
	
	/// <summary>
	/// ���݂̑����ԍ���Ԃ�
	/// UI�Ɍ��݂̑�����\�����邽��
	/// </summary>
	/// <returns>������\�����ʔԍ�</returns>
	size_t CurrentEquipmentNo()const;
};
