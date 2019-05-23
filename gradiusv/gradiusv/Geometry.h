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

//Vector2D<T>のための+オペレータオーバーロード
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

//整数型ベクトル
typedef Vector2D<int> Vector2;
typedef Vector2 Position2;

//実数ベクトル
typedef Vector2D<float> Vector2f;
typedef Vector2f Position2f;



//サイズを表す構造体
struct Size {
	Size();
	Size(int inw, int inh);
	int w;
	int h;
};

///矩形を表現する構造体
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

	///矩形を描画する
	///@param color 色を0x00000000～0xffffffffで指定する(デフォルト白)
	void Draw(unsigned int color = 0xffffffff);

	///矩形を描画する(オフセットつき)
	///@param offset ずらした位置に表示したい場合にオフセット値を指定
	///@param color 色を0x00000000～0xffffffffで指定する(デフォルト白)
	void Draw(const Vector2& offset,unsigned int color = 0xffffffff);
	void Draw(const Vector2f& offset, unsigned int color = 0xffffffff);

	///左、右、上、下から矩形を作る
	///@param left 左
	///@param right 右
	///@param top 上
	///@param bottom 下
	static Rect CreateRectFromLRTB(int left, int right, int top, int bottom);

	///二つの矩形から重なり矩形を作る
	///@param rcA 矩形A
	///@param rcB 矩形B
	static Rect CreateOverlappedRangeRect(const Rect& rcA, const Rect& rcB);
};




class Geometry
{
public:
	Geometry();
	~Geometry();
};

