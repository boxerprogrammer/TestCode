#pragma once

#include<memory>
#include<list>
#include"../Geometry.h"
#include<memory>
class Camera;
/// <summary>
/// �G�t�F�N�g���N���X
/// �A�j���[�V������\�����A�\�����I������������
/// </summary>
class Effect
{
protected:
	int frame_=0;
	Position2f pos_;
	bool isDeletable_ = false;
	std::shared_ptr<Camera> camera_;
public:
	/// <param name="pos">�\�����W</param>
	/// <param name="camera">�J�����ւ̎Q��</param>
	Effect(const Position2f& pos, std::shared_ptr<Camera> camera):pos_(pos),camera_(camera) {};

	/// <summary>
	/// ���̃G�t�F�N�g�͍폜�\���H
	/// </summary>
	/// <returns>true:�폜�\ false:�폜�s��</returns>
	bool IsDeletable()const {
		return isDeletable_;
	}
	/// <summary>
	/// �G�t�F�N�g�t���[���X�V
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// �G�t�F�N�g�\��
	/// </summary>
	virtual void Draw() = 0;
};

/// <summary>
/// �G�t�F�N�g�Ǘ��N���X
/// </summary>
class EffectManager {
private:
	int bloodH_ = -1;
	int bloodSE_ = -1;
	int blowH_ = -1;
	int hitSE_ = -1;
	int groundExpH_ = -1;
	int smokeExpH_ = -1;
	int energyBallH_ = -1;
	std::list<std::shared_ptr<Effect>> effects_;
	std::shared_ptr<Camera> camera_;
public:
	/// <param name="camera">�J�����ւ̎Q��</param>
	EffectManager(std::shared_ptr<Camera> camera);
	/// <summary>
	/// �����Ԃ��G�t�F�N�g����
	/// </summary>
	/// <param name="pos">�\�����W</param>
	/// <param name="isTurn">���]�t���O</param>
	void EmitBlood(const Position2f& pos,bool isTurn=false);

	/// <summary>
	/// �n�ʔ����G�t�F�N�g����
	/// </summary>
	/// <param name="pos">�\�����W</param>
	/// <param name="isTurn">���]�t���O</param>
	void EmitGroundExplosion(const Position2f& pos, bool isTurn = false);

	/// <summary>
	/// ���������G�t�F�N�g����
	/// </summary>
	/// <param name="pos">�\�����W</param>
	/// <param name="isTurn">���]�t���O</param>
	void EmitSmokeExplosion(const Position2f& pos, bool isTurn = false);

	/// <summary>
	/// �G�l���M�[���G�t�F�N�g����
	/// </summary>
	/// <param name="pos">�\�����W</param>
	/// <param name="isTurn">���]�t���O</param>
	void EmitEnergyBall(const Position2f& pos, int lifetime=60,bool isTurn = false);

	/// <summary>
	/// �������R�G�t�F�N�g����
	/// </summary>
	/// <param name="pos">�\�����W</param>
	/// <param name="isTurn">���]�t���O</param>
	void EmitBlow3(const Position2f& pos, bool isTurn = false);

	/// <summary>
	/// �G�t�F�N�g���ׂ�Update
	/// </summary>
	void Update();

	/// <summary>
	/// �G�t�F�N�g���ׂĕ`��
	/// </summary>
	void Draw();

};