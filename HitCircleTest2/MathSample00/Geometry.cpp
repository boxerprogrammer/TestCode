#include"Geometry.h"
#include<cmath>

Vector2 operator+(const Vector2& lval, const Vector2& rval) {
	return Vector2(lval.x + rval.x, 
		lval.y + rval.y);
}

Vector2 operator-(const Vector2& lval, const Vector2& rval) {
	return Vector2(lval.x - rval.x,
		lval.y - rval.y);
}

float
Vector2::Magnitude()const {
	return hypot(x, y);
}

void 
Vector2::Normalize() {
	auto mag = Magnitude();
	x /= mag;
	y /= mag;
}

Vector2 
Vector2::Normalized() {
	Vector2 ret(x, y);
	ret.Normalize();
	return ret;
}

void 
Vector2::operator+=(const Vector2& val) {
	x += val.x;
	y += val.y;
}
void 
Vector2::operator-=(const Vector2& val) {
	x -= val.x;
	y -= val.y;
}


void 
Vector2::operator*=(const float scale) {
	x *= scale;
	y *= scale;
}
void 
Vector2::operator/=(const float div) {
	x /= div;
	y /= div;
}

Vector2 operator*(const Vector2& val, const float scale) {
	return Vector2(val.x * scale, val.y * scale);
}
Vector2 operator/(const Vector2& val, const float div) {
	return Vector2(val.x / div, val.y / div);
}
