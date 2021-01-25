#pragma once

#include"Geometry.h"

class Ray
{
public:
	Position3 origin_;
	Vector3 dir_;
	Ray();
	Ray(const Position3& org, const Vector3& d);
	Position3 Origin()const;
	Vector3 Direction()const;
};


