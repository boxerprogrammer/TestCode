#pragma once

#include"Geometry.h"

class Material;
namespace mgt_lib {

/// <summary>
/// 座標軸
/// </summary>
struct Axis3D {
	Vector3 right;//右
	Vector3 up;//上
	Vector3 front;//前
};

/// <summary>
/// 図形基底クラス
/// </summary>
class Primitive3D
{
public:
	virtual void Draw(const Material& mat) = 0;
	virtual void WireframeDraw()const = 0;
	//virtual void WireframeDraw2()= 0;
};

}

