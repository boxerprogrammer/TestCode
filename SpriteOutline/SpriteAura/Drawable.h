#pragma once
//�`��n���N���X
class Drawable
{
public:
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual ~Drawable() = default;
};

