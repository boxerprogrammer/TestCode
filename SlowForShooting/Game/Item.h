#pragma once
#include"../Geometry.h"
#include"ItemType.h"


/// <summary>
/// �A�C�e����\���N���X
/// </summary>
class Item
{
private:
	int handle_;//�\���p
	ItemType type_;//�A�C�e�����(�擾�����Ƃ��Ƀv���C���[�ɉ����N�����̂�)
	Rect rect_;//�������`(�v���C���[���g�Ƃ̓�����)
	bool isEnabled_;//�\���A������A��L���ɂ��邩�ǂ���
public:
	Item(int handle,ItemType type);
	~Item();
	/// <summary>
	/// ��`����Ԃ�
	/// </summary>
	/// <returns>��`���</returns>
	const Rect& GetRect()const;

	/// <summary>
	/// ���݈ʒu��ݒ肷��
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPosition(const Position2& pos);

	/// <summary>
	/// �A�C�e����ʂ�Ԃ�
	/// </summary>
	/// <returns>�A�C�e�����</returns>
	const ItemType GetItemType()const;

	/// <summary>
	/// �L�����ǂ���(�����邩�H�\�����邩�H)
	/// �����Ȃ�������
	/// </summary>
	/// <returns>true:�L�� / false:����</returns>
	bool IsEnabled()const;

	/// <summary>
	/// ���������C�x���g
	/// </summary>
	void OnTouch();

	/// <summary>
	/// �X�V�֐�
	/// </summary>
	void Update();

	/// <summary>
	/// �\���֐�
	/// </summary>
	void Draw();
};

