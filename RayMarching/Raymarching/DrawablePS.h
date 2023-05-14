#pragma once
#include"Drawable.h"
class DrawablePS : public Drawable
{
private:
	int handle_;
	
public:
	DrawablePS() = default;
	DrawablePS(int graphicsH,int psH);
	virtual void Update()override;
	virtual void Draw()override;
};

