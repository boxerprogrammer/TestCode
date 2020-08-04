#include"Geometry.h"
#include<algorithm>

const Vector2D<float> Vector2D<float>::UP(0.0f, -1.0f);
const Vector2D<float> Vector2D<float>::DOWN(0.0f, 1.0f);
const Vector2D<float> Vector2D<float>::LEFT(-1.0f, 0.0f);
const Vector2D<float> Vector2D<float>::RIGHT(1.0f, 0.0f);
const Vector2D<float> Vector2D<float>::ZERO(0.0f,0.0f);
const Vector2D<float> Vector2D<float>::NIL(NAN, NAN);
const Segment Segment::ZERO(Vector2f::ZERO, Vector2f::ZERO);
const Segment Segment::NIL(Vector2f::NIL, Vector2f::NIL);

using namespace std;



float Dot(const Vector2f& va, const Vector2f& vb) {
	return va.x * vb.x + va.y * vb.y;
}
float Cross(const Vector2f& va, const Vector2f& vb) {
	return va.x * vb.y - va.y * vb.x;
}

Position2f 
Segment::End() {
	return start + vec;
}

bool 
Segment::IsNil() {
	return isnan(vec.x)||isnan(vec.y);
}

Vector2f
Clamp(const Vector2f& val,const Vector2f& minVal, const Vector2f& maxVal) {
	return { std::clamp(val.x,minVal.x,maxVal.x),std::clamp(val.y,minVal.y,maxVal.y) };
}


