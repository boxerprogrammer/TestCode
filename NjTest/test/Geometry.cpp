#include"Geometry.h"
const Vector2D<float> Vector2D<float>::ZERO(0.0f,0.0f);

float Dot(const Vector2f& va, const Vector2f& vb) {
	return va.x * vb.x + va.y * vb.y;
}
float Cross(const Vector2f& va, const Vector2f& vb) {
	return va.x * vb.y - va.y * vb.x;
}