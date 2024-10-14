#pragma once
#include "Transitor.h"
class FadeTransitor :
    public Transitor
{
private:
	
public:
	FadeTransitor(int interval = 60);
	virtual void Update() override;
	virtual void Draw() override;
};

