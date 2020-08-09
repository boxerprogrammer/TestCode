#pragma once
#include<list>
#include<memory>
class Enemy;
/// <summary>
/// �G�S�̂��Ǘ�����
/// </summary>
class EnemyManager
{
private:
	//�G���Ǘ����邽�߂Ƀ��X�g�ŊǗ�
	using Enemies_t = std::list<std::shared_ptr<Enemy>>;
	Enemies_t enemies_;
public:
	/// <summary>
	/// �G�I�u�W�F�N�g�����̃N���X�Ǘ����ɒu��
	/// </summary>
	/// <param name="enemy">�G�I�u�W�F�N�g</param>
	void AddEnemy(Enemy* enemy);

	/// <summary>
	/// �G���ׂĂւ̎Q�Ƃ�Ԃ�
	/// </summary>
	/// <returns>�G�I�u�W�F�N�g���X�g�ւ̎Q��</returns>
	Enemies_t& Enemies();

	/// <summary>
	/// �S�Ă̓G��Update���Ăяo��
	/// </summary>
	void Update();

	/// <summary>
	/// �S�Ă̓G��Draw���Ăяo��
	/// </summary>
	void Draw();
};

