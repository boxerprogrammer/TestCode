#pragma once
#include<memory>
#include"../Geometry.h"

class Input;
///�L�����N�^�[���N���X
class Character
{
protected:
	Position2f pos_;//�L�������S���W
public:
	Character();
	virtual ~Character() = default;
	///<summary>�L�����N�^�[�̍X�V���s��</summary>
	virtual void Update() = 0;
	///<summary>�L�����N�^�[�̕\�����s��</summary>
	virtual void Draw() = 0;
};

