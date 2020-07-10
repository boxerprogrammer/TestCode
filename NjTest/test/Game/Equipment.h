#pragma once
class Player;
class Input;
class Equipment
{
public:
	virtual ~Equipment() = default;
	/// <summary>
	/// ���̕���ōU���������܂�
	/// </summary>
	/// <param name="player">�v���C���[�ւ̎Q��</param>
	/// <param name="input">���͎Q��</param>
	virtual void Attack(const Player& p, const Input& i) = 0;
	/// <summary>
	/// �ǉ�����(�قڍ����p)
	/// </summary>
	/// <param name="i">���݂�input</param>
	virtual void AdditionalInput(const Input& i) {};
	virtual void Update() {};
	virtual void Draw() {};
};

