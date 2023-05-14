#pragma once

/// <summary>
/// �x�N�g���\����
/// �v���C���[�̍��W�⑬�x��\�����߂̂���
/// </summary>
struct Vector2 {
	float x;
	float y;

	Vector2() :x(0), y(0) {}
	Vector2(float inx, float iny) :x(inx), y(iny) {};

	//���Z�A���Z
	Vector2 operator+(const Vector2& rval)const {
		return { x+rval.x , y+rval.y};
	}
	Vector2 operator-(const Vector2& rval)const {
		return { x - rval.x , y - rval.y };
	}
	void operator+=(const Vector2& rval);
	void operator-=(const Vector2& rval);

	//�X�J���[�{,�X�J���[����
	Vector2 operator*(float scale)const;
	Vector2 operator/(float div)const;

	void operator*=(float scale);
	void operator/=(float div);

	//�x�N�^�[�t�](-)
	Vector2 operator-()const;

	/// �x�N�g���̑傫����Ԃ�
	float Length()const;

	/// �x�N�g���̑傫����2���Ԃ�
	float SQLength()const;

	/// <summary>
	/// �x�N�g���̐��K��
	/// </summary>
	void Normalize();

	/// <summary>
	/// ���K�������x�N�g����Ԃ�
	/// </summary>
	Vector2 GetNormalized()const;

};

//���W��\���ʖ�
using Position2 = Vector2;

//�T�C�Y���܂Ƃ߂�\����
struct Size {
	int w;///��
	int h;///����
};

/// <summary>
/// ��`�\����
/// </summary>
struct Rect {
	Position2 center;//��`�̒��S
	Size size;//��`�̕��A����

	Rect();
	Rect(const Position2& c, const Size& sz);

	/// <summary>
	/// ����E�����w�肷�邱�Ƃŋ�`���\�z
	/// </summary>
	/// <param name="left">��</param>
	/// <param name="top">��</param>
	/// <param name="right">�E</param>
	/// <param name="bottom">��</param>
	void SetLTRB(int left,int top ,int right,int bottom);

	//��
	int Left()const;
	//��
	int Top()const;
	//�E
	int Right()const;
	//��
	int Bottom()const;

	const Position2& GetCenter()const;
	const Size& GetSize()const;

	/// <summary>
	/// �f�o�b�O�p��`�\��
	/// </summary>
	void Draw(unsigned int Color = 0xfffff)const;
	
	/// <summary>
	/// ������̋�`�Ɠ����������ǂ���
	/// </summary>
	/// <param name="rc">��`</param>
	/// <returns>true �������� false �͂��ꂽ</returns>
	bool IsHit(const Rect& rc)const;
};




