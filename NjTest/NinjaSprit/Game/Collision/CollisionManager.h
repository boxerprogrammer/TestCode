#pragma once
#include<memory>
#include<list>
class Collider;
/// <summary>
/// �Փ˔����S�ʓI�ɊǗ�����
/// </summary>
class CollisionManager
{
private:
	std::list<std::shared_ptr<Collider>> colliders_;///<�����蔻��W����
public:
	/// <summary>
	/// �S�R���W�����𔻒肷��(���t���[��)
	/// </summary>
	void Update();

	/// <summary>
	/// �S�R���W������\������(�f�o�b�O�p)
	/// </summary>
	void DebugDraw();

	/// <summary>
	/// �R���W�����𔻒�Ώۂɒǉ�����
	/// </summary>
	/// <param name="collider">����ΏۃI�u�W�F�N�g</param>
	std::shared_ptr<Collider>  AddCollider(Collider* collider);
};

