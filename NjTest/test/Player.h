#pragma once
struct Enemy;//�v���g�^�C�v�錾
struct Player
{
	//�錾���ɂ����Ă͒��g��
	//�m��K�v���Ȃ��B
	//�|�C���^�^(�Q�ƌ^)�̓T�C�Y��
	//���܂��Ă���(4�o�C�g��8�o�C�g)
	Enemy* enemy_;//Enemy*�̃|�C���^
};

int PlayerFunction(Player* p);

static int currentPlayerNo = 0;
