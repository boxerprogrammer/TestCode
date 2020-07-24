#pragma once

#include<memory>
class Camera;
class Player;
class Input;
class CollisionManager;
class Equipment
{
protected:
	std::shared_ptr<CollisionManager> collisionManager_;
	std::shared_ptr<Camera> camera_;
public:
	Equipment(std::shared_ptr<CollisionManager> cm , std::shared_ptr<Camera> camera);
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

