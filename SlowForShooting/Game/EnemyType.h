#pragma once
/// <summary>
/// �G���
/// </summary>
enum class EnemyType {
	Patapata,//��Ԃ̎G��
	Insect,//�ґ���g�ޒ�
	Rock,//��
	VolcanicRock,//�ΎR��
	ItemBringer,//�A�C�e�������Ă�����
	MissileBringer,//�~�T�C�������Ă�����
};
enum class BurstType {
	ZakoBurst,//�G������
};
/// <summary>
/// �f�[�^ID�Ɋ�Â��G����є������
/// </summary>
enum class EnemySpawnerId {
	None,//0:�Ȃɂ��Ȃ�
	OnePatapata,//1:�P�ƃp�^�p�^
	TeamPatapata,//2:�ґ��p�^�p�^
	InfinitePatapata,//3:�����p�^�p�^
	TeamInsect,//4:�ґ��[��
	InfiniteRock,//5:�△������
	PosInfVolcanicRock,//6:�����ΎR��N��
	OneItemBringer,//7:�A�C�e�����Ƃ��z���P��
	OneMissleBringer//8:�~�T�C�����Ƃ��z���P��
};