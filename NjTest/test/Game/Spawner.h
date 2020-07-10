#pragma once
#include<memory>
#include<list>
#include"../Geometry.h"
class Enemy;
/// <summary>
/// �G��������N���X
/// </summary>
class Spawner
{
protected:
	Position2f pos_;//��ʓ��ɃX�|�[�i�[�����邩�ǂ����̔��肪�K�v�Ȃ��ߍ��W������
	std::shared_ptr<Enemy> prototype_;//�v���g�^�C�v
	std::list<std::shared_ptr<Enemy>>& enemies_;//�G���X�g�ւ̎Q��(������͕ʂɂ���)
	///�ŏ��ɗ^����ꂽ�v���g�^�C�v�Ɋ�Â��N���[����������
	///�h���N���X��Spawn��Update����Ăяo���B
	std::shared_ptr<Enemy> CreateClone();
public:
	Spawner(const Position2f& pos,std::shared_ptr<Enemy> prototype, std::list<std::shared_ptr<Enemy>>& enemies);
	virtual ~Spawner()=default;
	/// <summary>
	/// �������Ƀ��X�g�ɕ��荞�ނ������ɃV�F�A�|�C���^���Ԃ�
	/// </summary>
	/// <returns></returns>
	virtual std::shared_ptr<Enemy> Spawn() = 0;
	
	/// <summary>
	/// ���Ԃ��Ƃ̔����Ȃǂ��K�v�Ȃ��ߖ��t���[���X�V���s��
	/// </summary>
	virtual void Update() = 0;
};
