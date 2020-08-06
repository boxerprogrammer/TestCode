#pragma once
#include "Collider.h"
#include"../Geometry.h"
#include<memory>
/// <summary>
/// 円の当たり判定クラス
/// </summary>
class CircleCollider :
    public Collider
{
private:
    Circle circle_;///<判定に使う円
   
public:
    const Circle& GetCircle()const;
    /// <summary>
    /// 実際の座標を返す(円なので中心点座標)
    /// </summary>
    /// <returns>持ち主を考慮した中心点の座標</returns>
    const Vector2f AcutualPosition();

    /// <summary>
    /// 何もなければ{0,0},0 「円のコライダー」
    /// </summary>
    CircleCollider(std::shared_ptr<Character> chr,const char* tag="");

    /// <summary>
    /// 円情報から円衝突判定を作る
    /// </summary>
    /// <param name="c">円情報</param>
    CircleCollider(std::shared_ptr<Character> chr,const Circle& c, const char* tag = "");

    /// <summary>
    /// 中心と半径から円衝突情報を作る
    /// </summary>
    /// <param name="p">中心</param>
    /// <param name="r">半径</param>
    CircleCollider(std::shared_ptr<Character> chr,const Position2f& p,float r,  const char* tag = "");

    bool IsHit(std::shared_ptr<Collider> col)override;
    void Draw()override;
    
};

