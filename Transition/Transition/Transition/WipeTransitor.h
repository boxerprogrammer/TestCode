#pragma once
#include "Transitor.h"
class WipeTransitor :
    public Transitor
{
private:
	int gradationH_;
public:
	WipeTransitor(TransitDirection dir=TransitDirection::left, int interval = 60);
	virtual void Update() override;
	virtual void Draw() override;
};

