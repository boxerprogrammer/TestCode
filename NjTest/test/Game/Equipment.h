#pragma once
class Player;
class Input;
class Equipment
{
public:
	/// <summary>
	/// ���̕���ōU���������܂�
	/// </summary>
	/// <param name="player">�v���C���[�ւ̎Q��</param>
	/// <param name="input">���͎Q��</param>
	virtual void Attack(const Player& p, const Input& i) = 0;
};

