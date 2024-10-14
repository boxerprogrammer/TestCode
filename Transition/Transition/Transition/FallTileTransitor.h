#pragma once
#include "Transitor.h"
#include<vector>
#include<random>
class FallTileTransitor :
    public Transitor
{
private:
    int cellSize_=50;
    struct XYIdx{
        int xidx, yidx;
        int yoffset=0;
        float vy=0.0f;
    };
    std::mt19937 mt_;
    std::vector<XYIdx> tiles_;
    float g_;
public:
    FallTileTransitor(int cellSize = 50, float gravity=0.0f,int interval = 60);
    virtual void Update() override;
    virtual void Draw() override;
    virtual bool IsEnd() const override;
};

