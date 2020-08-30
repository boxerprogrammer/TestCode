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
class EffectManager;
class CollisionManager;
class ShadowClone;
/// <summary>
/// �v���C���[�N���X
/// </summary>
class Player : public Character {
	friend PlayerInputListener;
private:
	int life_ = 100;
	int maskH_ = -1;
	int changeSE_ = -1;
	int runH_[6] = { -1,-1,-1,-1, -1, -1 };
	int jumpH_[4] = { -1, -1, -1, -1 };
	int fallH_[2] = { -1, -1 };
	std::array<Position2f, 60> moveHistory_;

	std::vector<std::shared_ptr<ShadowClone>> shadowClones_;

	size_t currentMoveIndex_ = 0;
	void SetCurrentPosition(Position2f& pos);
	const Position2f& GetBackTimePosition(size_t backFrame)const;

	Position2f lastPos_;
	float velY_ = 0.0f;
	float accelY_ = 0.0f;
	bool isRight_ = true;
	std::array<std::shared_ptr<Equipment>,4> equipments_;
	size_t currentEquipmentNo_ = 0;
	int knockbackFrame_ = 0;//�m�b�N�o�b�N�t���[��
	std::shared_ptr<CollisionManager> collisionManager_;

	std::shared_ptr<EffectManager> effectMgr_;


	void AdditionalInput(const Input& input);

	void InitUpdate();

	void DamageUpdate();

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

	Vector2f GetVelocity()const;

	/// <summary>
	/// �Փ˔���C�x���g
	/// </summary>
	/// <param name="me">�Փˏ�񎩕�</param>
	/// <param name="another">�Փˏ�񑊎�</param>
	void OnHit(CollisionInfo& me, CollisionInfo& another)override;

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
