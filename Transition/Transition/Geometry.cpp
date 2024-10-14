#include "Geometry.h"



Vector2f ConvertToVector2f(const Vector2& v) {
	return Vector2f(v.x, v.y);
}

float Dot(const Vector2f& lval, const Vector2f& rval) {
	return lval.x*rval.x + lval.y*rval.y;
}

float Cross(const Vector2f& lval, const Vector2f& rval) {
	return lval.x*rval.y - lval.y*rval.x;
}

Vector2f 
operator*(const Vector2f& inv, const Mat3x3& mat) {
	Vector2f ret;
	ret.x = inv.x*mat.e[0][0] + inv.y*mat.e[1][0]+mat.e[2][0];
	ret.y = inv.x*mat.e[0][1] + inv.y*mat.e[1][1]+mat.e[2][1];
	return ret;
}

Mat3x3
operator*(const Mat3x3& lval, const Mat3x3& rval) {
	Mat3x3 ret;
	//for (int i = 0; i < 3; ++i) {//c
	//	for (int j = 0; j < 3; ++j) {//‰¡
	//		ret.e[i][j]= lval.e[i][j]* rval.e[j][i]+
	//	}
	//}
	ret.e[0][0] = lval.e[0][0] * rval.e[0][0] + lval.e[0][0] * rval.e[0][1] + lval.e[0][0] * rval.e[0][2];
	ret.e[0][1] = lval.e[0][1] * rval.e[1][0] + lval.e[0][1] * rval.e[1][1] + lval.e[0][1] * rval.e[1][2];
	ret.e[0][2] = lval.e[0][2] * rval.e[2][0] + lval.e[0][2] * rval.e[2][1] + lval.e[0][2] * rval.e[2][2];

	ret.e[1][0] = lval.e[1][0] * rval.e[0][0] + lval.e[1][0] * rval.e[0][1] + lval.e[1][0] * rval.e[0][2];
	ret.e[1][1] = lval.e[1][1] * rval.e[1][0] + lval.e[1][1] * rval.e[1][1] + lval.e[1][1] * rval.e[1][2];
	ret.e[1][2] = lval.e[1][2] * rval.e[2][0] + lval.e[1][2] * rval.e[2][1] + lval.e[1][2] * rval.e[2][2];

	ret.e[2][0] = lval.e[2][0] * rval.e[0][0] + lval.e[2][0] * rval.e[0][1] + lval.e[2][0] * rval.e[0][2];
	ret.e[2][1] = lval.e[2][1] * rval.e[1][0] + lval.e[2][1] * rval.e[1][1] + lval.e[2][1] * rval.e[1][2];
	ret.e[2][2] = lval.e[2][2] * rval.e[2][0] + lval.e[2][2] * rval.e[2][1] + lval.e[2][2] * rval.e[2][2];

	return ret;
}


Mat3x3 
Identity() {
	Mat3x3 m = { {{1,0,0},{0,1,0},{0,0,1} } };
	return m;
}
Mat3x3 
Rotate(float angle) {
	Mat3x3 m = { {{cos(angle),sin(angle),0},{-sin(angle),cos(angle),0},{0,0,1} } };
	return m;
}
Mat3x3 
Translate(const Vector2f& v) {
	Mat3x3 m = { {{1,0,0},{0,1,0},{v.x,v.y,1} } };
	return m;
}
Mat3x3 
CenteredRotate(const Position2f& p, float angle) {
	return Translate(-p)*Rotate(angle)*Translate(p);
}