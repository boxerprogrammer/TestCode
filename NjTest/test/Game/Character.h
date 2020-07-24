#pragma once
#include<memory>
#include"../Geometry.h"


class Camera;
class Input;
struct CollisionInfo;
///�L�����N�^�[���N���X
class Character
{
protected:
	Position2f pos_;//�L�������S���W
	std::shared_ptr<Camera> camera_;
public:
	Character(std::shared_ptr<Camera> camera);

	///���W�A�N�Z�X
	virtual const Vector2f& GetPosition()const;
	virtual void SetPosition(const Vector2f& vel);

	virtual ~Character() = default;
	///<summary>�L�����N�^�[�̍X�V���s��</summary>
	virtual void Update() = 0;
	///<summary>�L�����N�^�[�̕\�����s��</summary>
	virtual void Draw() = 0;
	virtual void OnHit(CollisionInfo& ) = 0;
};

