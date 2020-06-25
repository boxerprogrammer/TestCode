#pragma once


//�T�C�Y��\���\����
struct Size {
	float w;//��
	float h;//����
};

//���W��\���\����
struct Vector2 {
	Vector2():x(0),y(0){}
	Vector2(float inx,float iny):x(inx),y(iny){}
	float x, y;
	///�x�N�g���̑傫����Ԃ��܂�
	float Magnitude()const;
	///�x�N�g���̑傫����2���Ԃ��܂�
	float Square()const;
	
	///���K��(�傫�����P��)���܂�
	void Normalize();

	///���K���x�N�g����Ԃ��܂�
	Vector2 Normalized()const;

	void operator+=(const Vector2& v);
	void operator-=(const Vector2& v);
	void operator*=(float scale);
	Vector2 operator*(float scale)const;
	Vector2 operator-() {
		return Vector2(-x, -y);
	}
};

Vector2 operator+(const Vector2& va, const Vector2 vb);
Vector2 operator-(const Vector2& va, const Vector2 vb);

///���ς�Ԃ�
float Dot(const Vector2& va, const Vector2& vb);

///�O�ς�Ԃ�
float Cross(const Vector2& va, const Vector2& vb);

///���ω��Z�q
float operator*(const Vector2& va, const Vector2& vb);

///�O�ω��Z�q
float operator%(const Vector2& va, const Vector2& vb);

//�Ƃ肠�����u���W�v���ĈӖ����ƃx�N�^���
//Position�̂ق����悭�ˁH���ė��R�ł��̖��O
typedef Vector2 Position2;

struct Circle {
	Position2 center; //���S���W
	float radius;//���a
	Circle() :radius(0), center(0, 0) {}
	Circle(float r,const Position2& p) :radius(r), center(p) {}
	Circle(const Position2& p,float r) :radius(r), center(p) {}
};

/// <summary>
/// ��`�\����
/// �s�Ϗ����Fv1��v2�͎��v���ł��鎖
///          :v1�̑傫����v2�̑傫���͓����ł��鎖
/// </summary>
struct Fan {
	Position2 center; //���S���W
	Vector2 v1; //��`�̒[�x�N�g���@
	Vector2 v2; //��`�̒[�x�N�g���A
	Fan() :center(0, 0),v1(0,0),v2(0,0) {}//�����ł�
	Fan(const Position2& p, const Vector2& inv1, const Vector2& inv2);
	Fan(const Position2& p, float r, float angle);
	Fan(const Position2& p, const Vector2& inv, float angle);
	void Draw(unsigned int color=0xffffff);
	float Radius()const;
	void AddAngle(float angle);
	float GetAngle()const;
};

float GetAngle2Vector(const Vector2& v1, const Vector2& v2);

struct Rect {
	Position2 pos; //���S���W
	int w, h;//��,����
	Rect() : pos(0, 0), w(0), h(0) {}
	Rect(float x, float y, int inw, int inh) :
		pos(x, y), w(inw), h(inh) {}
	Rect(const Position2& inpos, int inw, int inh) :
		pos(inpos), w(inw), h(inh)
	{}
	void SetCenter(float x, float y) {
		pos.x = x;
		pos.y = y;
	}
	void SetCenter(const Position2& inpos) {
		pos.x = inpos.x;
		pos.y = inpos.y;
	}
	const Vector2& Center() const{
		return pos;
	}
	float Left()const { return pos.x - w / 2; }
	float Top()const { return pos.y - h / 2; }
	float Right()const { return pos.x + w / 2; }
	float Bottom()const { return pos.y + h / 2; }
	float Width()const { return w; }
	float Height()const { return h; }
	void Draw();//�����̋�`��`�悷��
	void Draw(const Vector2& offset);//�����̋�`��`�悷��(�I�t�Z�b�g�t��)
};

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
Vector2 MultipleVec(const Matrix& mat, const Vector2& vec);

Matrix operator*(const Matrix& lmat, const Matrix& rmat);
Vector2 operator*(const Matrix& mat, const Vector2& vec);
