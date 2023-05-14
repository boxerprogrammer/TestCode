#pragma once
#include"../Geometry.h"
#include<memory>
class Player;

/// <summary>
/// ���@�ɑ΂��Ē��E�\�ȕ���
/// </summary>
class Force
{
	friend Player;
private:
	int handle_;
	int idxX_ = 0;
	int frame_ = 0;
	Rect imgRect_ = {};//�`��̂��߂̋�`
	Rect rect_ = {};//�����蔻��̂��߂̋�`
	Player& player_;//�v���C���[�̎Q��
	int level_ = 0;//�t�H�[�X���x��(0�͕\������������Ȃ�)
	float pushSpeed = 0.0f;//�؂藣�����x
	void (Force::* updateFunc_)();

	//�z����Ԃ̃A�b�v�f�[�g
	void AttachedUpdate();

	//���V��Ԃ̃A�b�v�f�[�g
	void FloatingUpdate();

	//�O���܂��͌���ɐ؂藣��
	//�������Ă�̂��O���Ȃ�O�ɔ�΂�
	//�������Ă�̂����Ȃ���ɔ�΂�
	void SeparatingUpdate();

	//�z���`�F�b�N
	void CheckHoldable();

	//�����񂹏�Ԃ̃A�b�v�f�[�g
	//�t�H�[�X�������񂹂��Ă�Œ�
	void PullingUpdate();

	enum class HoldType {
		front,//�O��
		back//���
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
	/// �t�H�[�X��������Ԃ��ǂ���
	/// </summary>
	/// <returns>true:���@����؂藣����Ă�@false:���@�ɂ������Ă���</returns>
	bool IsSepareted()const;


	/// <summary>
	/// �����񂹂ƁA�؂藣���̐؂�ւ�
	/// </summary>
	void Switch();
};

