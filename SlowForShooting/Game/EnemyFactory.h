#pragma once
#include<memory>
#include<list>
#include<map>
#include"../Geometry.h"
#include"EnemyType.h"

class Player;
class Enemy;

class Player;
class BulletFactory;
class ItemFactory;
/// <summary>
/// �G�H��
/// </summary>
class EnemyFactory
{
private:
	std::map<EnemyType, int> handleMap_;//�G�̎�ʂƉ摜�n���h���̃y�A
	std::map<BurstType, int> burstMap_;//�����̎�ʂƉ摜�n���h���̃y�A
	std::list<std::shared_ptr<Enemy>> enemies_;
	std::shared_ptr<Player> player_;
	std::shared_ptr<BulletFactory> bulletFactory_;
	std::shared_ptr<ItemFactory> itemFactory_;
	int frame_ = 0;
public:
	EnemyFactory(std::shared_ptr<Player> player, 
		std::shared_ptr<BulletFactory> bFactory,
		std::shared_ptr<ItemFactory> itemFactory);

	std::list<std::shared_ptr<Enemy>>& GetEnemies();

	/// <summary>
	/// �G�I�u�W�F�N�g�̐������s��
	/// �������āA�����̃��X�g�ŊǗ�����
	/// </summary>
	/// <param name="type">�G���</param>
	/// <param name="pos">�������W</param>
	/// <returns>�G�I�u�W�F�N�g</returns>
	std::shared_ptr<Enemy> Create(EnemyType type,const Position2 pos);

	/// <summary>
	/// �G�I�u�W�F�N�g�̐����݂̂��s��
	/// �������̃��X�g�ւ͒ǉ����Ȃ�
	/// </summary>
	/// <param name="type">�G���</param>
	/// <returns>�G�I�u�W�F�N�g��Ԃ�</returns>
	std::shared_ptr<Enemy> Make(EnemyType type);

	/// <summary>
	/// �����Ƃ��ēn���ꂽ�I�u�W�F�N�g��
	/// �G���X�g�ɒǉ�����
	/// </summary>
	/// <param name="enemy">�G�I�u�W�F�N�g</param>
	void AddEnemy(std::shared_ptr<Enemy> enemy);

	/// <summary>
	/// �S�G�̍X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �S�G�̕`��
	/// </summary>
	void Draw();
};

