#include"Geometry.h"


Vector2 operator+(const Vector2& lval, const Vector2& rval) {
	return Vector2(lval.x + rval.x, 
		lval.y + rval.y);
}

Vector2 operator-(const Vector2& lval, const Vector2& rval) {
	return Vector2(lval.x - rval.x,
		lval.y - rval.y);
}