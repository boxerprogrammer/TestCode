#pragma once
#include "Transitor.h"
#include<vector>
#include<random>
/// <summary>
/// タイル状切り替え(ディゾルヴトランジションとも)
/// </summary>
class TileTransitor :
    public Transitor
{
private:
    int cellSize_=50;
    struct XYIdx{
        int xidx, yidx;
    };
    std::mt19937 mt_;
    std::vector<XYIdx> tiles_;
public:
    TileTransitor(int cellSize = 50, int interval = 60);
    virtual void Update() override;
    virtual void Draw() override;
};

