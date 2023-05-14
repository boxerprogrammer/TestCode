#pragma once
#include "Bullet.h"
class ZakoBullet :
    public Bullet
{
private:
    int handle_;
public:
    ZakoBullet(int handle);

    void Update();
    void Draw();
};

