#pragma once
#include<cmath>
#include<algorithm>
#include<cassert>



template<typename T>
struct Vector2D {
	T x;//x���W
	T y;//y���W
	Vector2D(T inx, T iny) :x(inx), y(iny) {}
	Vector2D() :x(0), y(0) {}
	/// <summary>
	/// ���Z
	/// </summary>
	/// <param name="rval">�E�Ӓl</param>
	/// <returns>���Z����</returns>
	Vector2D operator+(const Vector2D& rval)const {
		return Vector2D(x + rval.x, y + rval.y);
	}
	/// <summary>
	/// ���Z
	/// </summary>
	/// <param name="rval">�E�Ӓl</param>
	/// <returns>���Z����</returns>
	Vector2D operator-(const Vector2D& rval)const {
		return Vector2D(x - rval.x, y - rval.y);
	}
	Vector2D operator-()const {
		return Vector2D(-x, -y);
	}
	/// <summary>
	/// �����Ɏw��l�������Z����
	/// </summary>
	/// <param name="v">���Z�l</param>
	void operator+=(const Vector2D& v) {
		x += v.x;
		y += v.y;
	}
	/// <summary>
	/// �����ɂ��Ĉʒu�������Z����
	/// </summary>
	/// <param name="v">���Z�l</param>
	void operator-=(const Vector2D& v) {
		x -= v.x;
		y -= v.y;
	}
	/// <summary>
	/// �X�J���[�{����
	/// </summary>
	/// <param name="scale">�{��</param>
	void operator*=(float scale) {
		x *= scale;
		y *= scale;
	}
	/// <summary>
	/// �X�J���[�{�������̂�Ԃ�
	/// </summary>
	/// <param name="scale">�{��</param>
	/// <returns>�X�P�[�����O�������ʂ̒l</returns>
	Vector2D operator*(const float scale)const {
		return {x*scale,y*scale};
	}
	/// <summary>
	/// �x�N�g���ɑ΂���X�J���[�l���Z
	/// </summary>
	/// <param name="div">���Z�l</param>
	/// <returns>���Z���ꂽ���ʂ̒l</returns>
	Vector2D operator/(const float div)const {
		return { x / div,y / div };
	}
	/// <summary>
	/// �x�N�g���̑傫����Ԃ�
	/// </summary>
	/// <returns>�x�N�g���̑傫��</returns>
	float Magnitude()const {
		return hypot(x, y);
	}
	/// <summary>
	/// �x�N�g�����������ǂ���
	/// </summary>
	/// <returns>true:�����ł��� false:�L���ł���</returns>
	bool IsNil()const {
		return isnan(x) || isnan(y);
	}
	/// <summary>
	/// ���������ǂ���
	/// </summary>
	/// <param name="v">�E�Ӓl</param>
	/// <returns>true:������ false:�������Ȃ�</returns>
	bool operator==(const Vector2D<T>& v)const {
		assert(!IsNil());
		return x == v.x && y == v.y;
	}
	/// <summary>
	/// �s�����ǂ���
	/// </summary>
	/// <param name="v">�E�Ӓl</param>
	/// <returns>true:�������Ȃ� false:������</returns>
	bool operator!=(const Vector2D<T>& v) {
		assert(!IsNil());
		return !(x == v.x && y == v.y);
	}
	/// <summary>
	/// �x�N�g���̒�����2���Ԃ�
	/// </summary>
	/// <returns>�x�N�g���̒�����2��</returns>
	float SQMagnitude()const {
		return x*x+y*y;
	}
	/// <summary>
	/// �x�N�g���𐳋K������(�傫�����P�ɂ���)
	/// </summary>
	void Normalize() {
		float mag = Magnitude();
		assert(mag != 0.0f);
		x/=mag;
		y /= mag;
	}
	/// <summary>
	/// ���K���ς݂̃x�N�g����Ԃ�
	/// </summary>
	/// <returns>���K���x�N�g��</returns>
	Vector2D Normalized()const {
		float mag = Magnitude();
		assert(mag != 0.0f);
		return {x/mag,y/mag};
	}
	/// <summary>
	/// X������̊p�x��Ԃ�
	/// </summary>
	/// <returns>X������Ƃ����p�x</returns>
	float AngleFromX()const {
		return atan2(y, x);
	}
	static const Vector2D<float> ZERO;
	static const Vector2D<float> UP;
	static const Vector2D<float> DOWN;
	static const Vector2D<float> LEFT;
	static const Vector2D<float> RIGHT;
	static const Vector2D<float> NIL;
	
};


using Vector2 = Vector2D<int>;
using Vector2f = Vector2D<float>;
using Position2 = Vector2;
using Position2f = Vector2f;

/// <summary>
/// �����^�̃x�N�^���畂�������_�^�̃x�N�^�ɕϊ�
/// </summary>
/// <param name="vn">�����^�x�N�^</param>
/// <returns>���������^�x�N�^</returns>
Vector2f Vector2ToVector2F(const Vector2& vn);
/// <summary>
/// ���������^�̃x�N�^�𐮐��^�̃x�N�^�ɕϊ�
/// </summary>
/// <param name="vf">���������^�x�N�^</param>
/// <returns>�����^�x�N�^</returns>
Vector2 Vector2FToVector2(const Vector2f& vf);

/// <summary>
/// �ėp�N�����v�֐�(�l��min����max�̊ԂɎ��߂�)
/// </summary>
/// <typeparam name="T">�g�ݍ��݌^</typeparam>
/// <param name="value">�l</param>
/// <param name="minValue">�ŏ��l</param>
/// <param name="maxValue">�ő�l</param>
/// <returns>�N�����v���ʒl</returns>
template<typename T>
inline T Clamp(const T& value, const T& minValue, const T& maxValue) {
	return std::min(std::max(value, minValue), maxValue);
}

/// <summary>
/// �x�N�g���N�����v�֐�(�v�f�l��min����max�̊ԂɎ��߂�)
/// </summary>
/// <typeparam name="T">�g�ݍ��݌^</typeparam>
/// <param name="value">�l</param>
/// <param name="minValue">�ŏ��l�x�N�g��</param>
/// <param name="maxValue">�ő�l�x�N�g��</param>
/// <returns>�N�����v���ʒl</returns>
template<>
inline Vector2f Clamp(const Vector2f& val, const Vector2f& minVal, const Vector2f& maxVal) {
	return { Clamp(val.x,minVal.x,maxVal.x),Clamp(val.y,minVal.y,maxVal.y) };
}

/// <summary>
/// ���ς�Ԃ�
/// </summary>
/// <param name="va">����</param>
/// <param name="vb">�E��</param>
/// <returns>���ϒl</returns>
float Dot(const Vector2f& va,const Vector2f& vb);

/// <summary>
/// �O�ς�Ԃ�
/// </summary>
/// <param name="va">����</param>
/// <param name="vb">�E��</param>
/// <returns>�O�ϒl</returns>
float Cross(const Vector2f& va, const Vector2f& vb);

/// <summary>
/// �T�C�Y���`
/// ���g��size_t���Q��(�ϊ��ɋC��t���悤)
/// </summary>
struct Size {
	size_t w=0, h=0;
	Size() {}
	Size(size_t iw, size_t ih) :w(iw), h(ih) {}
	/// <summary>
	/// Size(size_t�^�Q��)����Vector2(int�^�Q��)�ɕϊ�
	/// </summary>
	/// <returns>�����x�N�^</returns>
	Vector2 ToIntVector() {
		return { static_cast<int>(w),static_cast<int>(h) };
	}
};

/// <summary>
/// ��`(�����^)��\��
/// </summary>
struct Rect {
	Position2 pos;//������W
	Size size;//���A����
	Rect() {}
	/// <summary>
	/// ������W�ƃT�C�Y�ŏ�����
	/// </summary>
	/// <param name="p">������W</param>
	/// <param name="s">�T�C�Y</param>
	Rect(const Position2& p, const Size& s) :pos(p), size(s) {}
	/// <summary>
	/// ���A��A���A�����ŏ�����
	/// </summary>
	/// <param name="x">��</param>
	/// <param name="y">��</param>
	/// <param name="w">��</param>
	/// <param name="h">��</param>
	Rect(int x, int y, size_t w, size_t h) :pos(x, y), size(w, h) {}
	/// <summary>
	/// �����W��Ԃ�(�����^)
	/// </summary>
	/// <returns>�����W</returns>
	int Left()const {
		return pos.x ;
	}
	/// <summary>
	/// �E���W��Ԃ�(�����^)
	/// </summary>
	/// <returns>�E���W</returns>
	int Right()const {
		return static_cast<int>(pos.x + size.w);
	}
	/// <summary>
	/// ����W��Ԃ�(�����^)
	/// </summary>
	/// <returns>����W</returns>
	int Top()const {
		return pos.y;
	}
	/// <summary>
	/// �����W��Ԃ�(�����^)
	/// </summary>
	/// <returns>�����W</returns>
	int Bottom()const {
		return static_cast<int>(pos.y + size.h);
	}
	/// <summary>
	/// �����W��Ԃ�(���������^)
	/// </summary>
	/// <returns>�����W</returns>
	float LeftF()const {
		return static_cast<float>(pos.x);
	}
	/// <summary>
	/// �E���W��Ԃ�(���������^)
	/// </summary>
	/// <returns>�����W</returns>
	float RightF()const {
		return static_cast<float>(pos.x + size.w);
	}
	/// <summary>
	/// ����W��Ԃ�(���������^)
	/// </summary>
	/// <returns>����W</returns>
	float TopF()const {
		return static_cast<float>(pos.y);
	}
	/// <summary>
	/// �����W��Ԃ�(���������^)
	/// </summary>
	/// <returns>�����W</returns>
	float BottomF()const {
		return static_cast<float>(pos.y+ size.h);
	}
	/// <summary>
	/// ����Ԃ�
	/// </summary>
	/// <returns>��</returns>
	size_t Width()const {
		return size.w;
	}
	/// <summary>
	/// ������Ԃ�
	/// </summary>
	/// <returns>����</returns>
	size_t Height()const {
		return size.h;
	}
	/// <summary>
	/// ���S�_��Ԃ�(�����^)
	/// </summary>
	/// <returns>���S�_</returns>
	Vector2 Center()const {
		return { pos.x + static_cast<int>(size.w / 2),
			static_cast<int>(pos.y + size.h / 2) };
	}
	/// <summary>
	/// ���S�_��Ԃ�(���������^)
	/// </summary>
	/// <returns>���S�_</returns>
	Vector2f CenterF()const {
		return { static_cast<float>(pos.x + size.w / 2),
			static_cast<float>(pos.y + size.h / 2) };
	}
};

/// <summary>
/// �~�\����
/// </summary>
struct Circle {
	Position2f center;///<���S
	float radius;///<���a
	Circle() :center{ 0,0 }, radius(0){}
	Circle(const Position2f& c,float r) :center(c), radius(r){}
};

/// <summary>
/// ����
/// </summary>
struct Segment {
	Position2f start;//�N�_
	Vector2f vec;//�N�_��������P�[�_�܂ł̃x�N�g��
	Segment() {}
	Segment(const Position2f& s, const Position2f& v) :start(s), vec(v) {}
	Segment(float x, float y, float vx, float vy) :start(x, y), vec(vx, vy) {}
	/// <summary>
	/// �I�_���W��Ԃ�
	/// </summary>
	/// <returns>�I�_���W</returns>
	Position2f End();
	static const Segment ZERO;///<�[������
	static const Segment NIL;///<��������
	/// <summary>
	/// �����Ȑ������ǂ���
	/// </summary>
	/// <returns>true:���� false:�L��</returns>
	bool IsNil();
};
/// <summary>
/// �J�v�Z���^�\����
/// </summary>
struct Capsule {
	Segment seg;//����
	float radius;//�J�v�Z���̌���
};

namespace DxLib {
	struct tagVECTOR;
	const tagVECTOR& V2V(const Vector2f& v);
}
float GetAngle2Vector(const Vector2f& v1, const Vector2f& v2);

/// <summary>
/// �s��
/// </summary>
struct Matrix {
	float m[3][3];
};

///�P�ʍs���Ԃ�
Matrix IdentityMat();

///���s�ړ��s���Ԃ�
///@param x X�������s�ړ���
///@param y Y�������s�ړ���
Matrix TranslateMat(float x, float y);

///��]�s���Ԃ�
///@param angle ��]�p�x
Matrix RotateMat(float angle);

///�Q�̍s��̏�Z��Ԃ�
///@param lmat ���Ӓl(�s��)
///@param rmat �E�Ӓl(�s��)
///@attention ��Z�̏����ɒ��ӂ��Ă�������
Matrix MultipleMat(const Matrix& lmat, const Matrix& rmat);

///�x�N�g���ɑ΂��čs���Z��K�p���A���ʂ̃x�N�g����Ԃ�
///@param mat �s��
///@param vec �x�N�g��
Vector2f MultipleVec(const Matrix& mat, const Vector2f& vec);

Matrix operator*(const Matrix& lmat, const Matrix& rmat);
Vector2f operator*(const Matrix& mat, const Vector2f& vec);

/// <summary>
/// ��`�\����
/// </summary>
///<attention>
///�s�Ϗ���
///v1��v2�͕K�����v���ł��邱��
///v1��v2�̑傫���͕K�������ł��邱��
///</attention>
struct FanShape {
	Position2f center;///<���S���W
	Vector2f v1;///<��`�̒[�_�@�܂ł̃x�N�g��
	Vector2f v2;///<��`�̒[�_�A�܂ł̃x�N�g��
	FanShape(const Position2f& incenter, const Vector2f& inV1, const Vector2f& inV2);
	FanShape(const Position2f& incenter, const Vector2f& inV1, float angle);
	void AddAngle1(float angle);
	void AddAngle2(float angle);
	void SetAngle1(float angle);
	void SetAngle2(float angle);

	/// <summary>
	/// ��`��`�悷��
	/// </summary>
	void Draw(int graphH=-1, float amp=0.0f);
	/// <summary>
	/// ���a��Ԃ�
	/// </summary>
	/// <returns>���a</returns>
	float Radius();
	/// <summary>
	/// V1��V2�̊p�x��Ԃ�
	/// </summary>
	/// <returns>V1��V2�̊p�x</returns>
	float GetAngle();
	
};

/// <summary>
/// �a���O�Ս\����
/// </summary>
///<attention>
///�s�Ϗ���
///v1��v2�͕K�����v���ł��邱��
///v1��v2�̑傫���͕K�������ł��邱��
///</attention>
struct SlashShape {
	Position2f center;///<���S���W
	Vector2f v1;///<�[�_�@�܂ł̃x�N�g��
	Vector2f v2;///<�[�_�A�܂ł̃x�N�g��
	SlashShape(const Position2f& incenter, const Vector2f& inV1, const Vector2f& inV2);
	SlashShape(const Position2f& incenter, const Vector2f& inV1, float angle);
	void AddAngle1(float angle);
	void AddAngle2(float angle);
	void SetAngle1(float angle);
	void SetAngle2(float angle);

	/// <summary>
	/// �X���b�V���G�t�F�N�g��`�悷��
	/// </summary>
	void Draw(int graphH = -1, float amp = 0.0f,int psH=-1,int normalH=-1);
	/// <summary>
	/// ���a��Ԃ�
	/// </summary>
	/// <returns>���a</returns>
	float Radius();
	/// <summary>
	/// V1��V2�̊p�x��Ԃ�
	/// </summary>
	/// <returns>V1��V2�̊p�x</returns>
	float GetAngle();

};
