#pragma once
#include"Primitive3D.h"
#include<array>
namespace mgt_lib {
/// <summary>
/// 直方体
/// </summary>
class Cuboid : public Primitive3D
{
private:
	Position3 center_;//中心点座標
	Axis3D axis_;//3D軸ベクトル
	Size3D whd_;//幅、高さ、奥行
	std::array<Position3,8> vertices_;//頂点
	std::array<uint16_t, 24> indices_;//インデックス
	std::array<DxLib::VERTEX3D, 8> vb_;
	int img_ = -1;
	void CalculateVertices();
public:
	Cuboid();
	Cuboid(const Position3& pos,const Axis3D& axis , const Size3D& size );
	virtual void Draw(const Material& mat)override;
	virtual void WireframeDraw()const override;
	const Position3& GetPos()const;
	const Axis3D& GetAxis()const;
	const Size3D& GetSize()const;
	//virtual void WireframeDraw2()override;
};
}
