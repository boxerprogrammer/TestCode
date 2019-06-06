#include<DxLib.h>
#include<cmath>

bool IsHitCircle(int ax, int ay, int bx, int by,float ar,float br) {

	return hypot(bx-ax,by-ax)<=ar+br;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	SetWindowText("�~�̓����蔻��T���v��");
	ChangeWindowMode(true);
	SetGraphMode(640, 480, 32);
	DxLib_Init();

	SetDrawScreen(DX_SCREEN_BACK);

	const auto color_normal = GetColor(255, 255, 255);
	const auto color_hit = GetColor(255, 192, 192);

	

	int ax=50;
	int ay=50;

	int bx = 320;
	int by = 240;

	const float radiusA = 50;
	const float radiusB = 50;
	auto col = color_normal;

	char keystate[256];

	while (ProcessMessage() == 0) {
		ClearDrawScreen();
		GetHitKeyStateAll(keystate);
		if (keystate[KEY_INPUT_RIGHT]) {
			ax += 2;
		}
		if (keystate[KEY_INPUT_LEFT]) {
			ax -= 2;
		}

		if (keystate[KEY_INPUT_UP]) {
			ay -= 2;
		}
		if (keystate[KEY_INPUT_DOWN]) {
			ay += 2;
		}

		//hypot(bx - ax, by - ay) <= radiusA + radiusB
		if (IsHitCircle(ax,ay,bx,by,radiusA,radiusB)) {
			col = color_hit;
			//�������Ă���̂ŉ����߂��c�����A���ꂪ����B
			//������̂͂͂����茾���Ď��Y�ɓ��������c�I�I
			//�����~�������߂��c���ꂪ������c�I
			//���āA�ǂ������߂����B

			//�܂����������A��p����p���񂾂��ǁc�����߂��Ă��Ȃ��H
			//�����`�A���������˂��B�ǂ̕����ɉ����߂��܂����H
			//�@���������āc�m���Ă邩�ȁH�@�����Ă����̂͂ˁA3D�ɂ�����
			//���ʂɐ����ȃx�N�g���c�Ƃ��A2D�ɂ�����Ȑ������̒����ɑ΂���
			//�������Ă���x�N�g�����u�@���x�N�g���v���Ă����񂾂�B
			//����˂��B
			
			//�ł́A�~�ǂ����̓����蔻��̏ꍇ�@�������߂�̂͊ȒP�ł���B
			//�Ȃ��Ȃ璆�S���m�����Ԑ��c���ꂪ�@���ɂȂ邩�炾�B
			
			//�ŁA���̖@���Ƃ����ǂ��g���΂����̂��낤���H�v�̖͗͂@������
			//��������p����p�ɂ���Ď����Ă��܂����ߌ��̃x�N�g����
			//�@������������������ƍl������B�@�������ւ̎ˉe���~�����̂��B
			//�@�������ւ̎ˉe�͖@���x�N�g���𐳋K�����A����ƌ��̃x�N�g����
			//���ς��Ƃ邱�Ƃœ�����܂�V�EN�����A���ꂪ�D����u�ʁv
			//�Ȃ̂ŁA�v�Z���I�ɂ�V-(V�EN)N�ƂȂ�B
			//�����A�v���O���������̖�Y�B���Ȃ݂Ɍv�Z���̂͏�L�̌v�Z����
			//���ۂɑ��x��␳����Ɠ����ɍ��W���߂荞��łȂ��󋵂ɂ��Ȃ���
			//�����@������ꂿ�ၪ�[���̂ł���B�C��t���悤�B


			//

		}
		else {
			col = color_normal;
		}

		DrawCircle(ax, ay, radiusA, col, true);
		DrawCircle(bx, by, radiusB, col, true);

		ScreenFlip();
	}


	return 0;
}
