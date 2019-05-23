#pragma once

template<typename T>
struct Vector2D {
	T x;
	T y;
	Vector2D() : x(0), y(0) {}
	Vector2D(T inx, T iny) : x(inx),y(iny){}

	void operator+=(const Vector2D<T>& in){
		x += in.x;
		y += in.y;
	}
	void operator*=(float scale) {
		x *= scale;
		y *= scale;
	}
	void operator-=(const Vector2D<T>& in) {
		x -= in.x;
		y -= in.y;
	}

	Vector2D<int> ToIntVec()const {
		Vector2D<int> v(x,y);
		return v;
	}
	Vector2D<float> ToFloatVec()const {
		Vector2D<float> v(x, y);
		return v;
	}
	float Length()const {
		return hypot(x, y);
	}
	Vector2D<float> Normalized()const {
		auto len = Length();
		return Vector2D<float>((float)x / len, (float)y / len);
	}
};

//Vector2D<T>�̂��߂�+�I�y���[�^�I�[�o�[���[�h
template<typename T>
Vector2D<T> operator+(const Vector2D<T>& lv, const Vector2D<T>& rv) {
	return Vector2D<T>(lv.x+rv.x,lv.y+rv.y);
}

template<typename T>
Vector2D<T> operator-(const Vector2D<T>& lv, const Vector2D<T>& rv) {
	return Vector2D<T>(lv.x - rv.x, lv.y - rv.y);
}

template<typename T>
Vector2D<T> operator*(const Vector2D<T>& lv, const float scale) {
	return Vector2D<T>(lv.x*scale , lv.y*scale);
}

template<typename T>
bool operator==(const Vector2D<T>& lv, const Vector2D<T>& rv) {
	return lv.x == rv.x&&lv.y == rv.y;
}

template<typename T>
bool operator!=(const Vector2D<T>& lv, const Vector2D<T>& rv) {
	return lv.x != rv.x||lv.y != rv.y;
}

//�����^�x�N�g��
typedef Vector2D<int> Vector2;
typedef Vector2 Position2;

//�����x�N�g��
typedef Vector2D<float> Vector2f;
typedef Vector2f Position2f;



//�T�C�Y��\���\����
struct Size {
	Size();
	Size(int inw, int inh);
	int w;
	int h;
};

///��`��\������\����
struct Rect {
	Position2 center;
	Size size;
	Rect();
	Rect(int x, int y, int w, int h);
	Rect(Position2& pos, Size& sz);
	const int Left()const;
	const int Top()const ;
	const int Right() const;
	const int Bottom()const;
	const int Width()const { return size.w; }
	const int Height()const { return size.h; }

	///��`��`�悷��
	///@param color �F��0x00000000�`0xffffffff�Ŏw�肷��(�f�t�H���g��)
	void Draw(unsigned int color = 0xffffffff);

	///��`��`�悷��(�I�t�Z�b�g��)
	///@param offset ���炵���ʒu�ɕ\���������ꍇ�ɃI�t�Z�b�g�l���w��
	///@param color �F��0x00000000�`0xffffffff�Ŏw�肷��(�f�t�H���g��)
	void Draw(const Vector2& offset,unsigned int color = 0xffffffff);
	void Draw(const Vector2f& offset, unsigned int color = 0xffffffff);

	///���A�E�A��A�������`�����
	///@param left ��
	///@param right �E
	///@param top ��
	///@param bottom ��
	static Rect CreateRectFromLRTB(int left, int right, int top, int bottom);

	///��̋�`����d�Ȃ��`�����
	///@param rcA ��`A
	///@param rcB ��`B
	static Rect CreateOverlappedRangeRect(const Rect& rcA, const Rect& rcB);
};




class Geometry
{
public:
	Geometry();
	~Geometry();
};

