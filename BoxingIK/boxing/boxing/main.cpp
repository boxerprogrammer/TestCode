#include<DxLib.h>
#include<vector>
#include"Geometry.h"

using namespace std;
float Clampf(float value, float minV = 0.0f, float maxV = 1.0f) {
	return max(min(value, maxV), minV);
}

Position3f Clamp(Position3f value, Position3f minV = Position3f(0.0f, 0.0f, 0.0f), Position3f maxV = Position3f(1.0f, 1.0f, 1.0f)) {
	return Position3f(Clampf(value.x, minV.x, maxV.x), Clampf(value.y, minV.y, maxV.y), Clampf(value.z, minV.z, maxV.z));
}

VECTOR ToVECTOR(Vector3f v) {
	return VGet(v.x, v.y, v.z);
}

Vector3f ToVector3f(VECTOR v) {
	return Vector3f(v.x, v.y, v.z);
}

void
DrawCircle3D_XY(const Position3f& c, float radius, uint32_t div, uint32_t color) {

	float dw = DX_TWO_PI_F / static_cast<float>(div);
	float angle = 0.0f;
	for (auto i = 0u; i < div; ++i) {
		auto pos1 = c + Vector3f(radius * cosf(angle), radius * sinf(angle), 0);
		auto pos2 = c + Vector3f(radius * cosf(angle + dw), radius * sinf(angle + dw), 0);
		DrawLine3D(ToVECTOR(pos1), ToVECTOR(pos2), color);
		DrawLine3D(ToVECTOR(pos1 + Vector3f(1, 1, 0)), ToVECTOR(pos2 + Vector3f(1, 1, 0)), color);
		angle += dw;
	}
}

MATRIX 
GetLookAtMatrix(const Vector3f& lookVec,const Vector3f& upVec) {
	auto Z = VNorm(ToVECTOR(lookVec));
	auto Y = VNorm(ToVECTOR(upVec));
	auto X = VNorm(VCross(Y, Z));
	Y = VNorm(VCross(Z,X));

	MATRIX ret = {};
	ret.m[0][0] = X.x; ret.m[0][1] = X.y; ret.m[0][2] = X.z;
	ret.m[1][0] = Y.x; ret.m[1][1] = Y.y; ret.m[1][2] = Y.z;
	ret.m[2][0] = Z.x; ret.m[2][1] = Z.y; ret.m[2][2] = Z.z;
	ret.m[3][3] = 1.0f;
	
	return ret;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetWindowText(L"�{�N�V���OIK");
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {
		return -1;
	}
	int screen_width, screen_height;
	GetWindowSize(&screen_width, &screen_height);
	SetDrawScreen(DX_SCREEN_BACK);

	SetCameraPositionAndTarget_UpVecY(VGet(0, 150, -400), VGet(0, 0, 0));

	auto rt = MakeScreen(screen_width, screen_height);

	auto lglove=MV1LoadModel(L"boxing_glove_l.mqo");

	constexpr float shoulder_width = 150.0f;
	int currentX = 512;
	int capturedNo = -1;
	int lastMouseInput = 0;
	bool captured = false;
	char keystate[256];
	char lastKeyState[256];
	//Z�����ɐL�т�
	vector<Position3f> positionsL(3);
	vector<Position3f> positionsR(3);
	for (auto i = 0u; i < positionsL.size(); ++i) {
		positionsL[i] = Position3f(-shoulder_width, 240, static_cast<float>(50 + i * 250));
		positionsR[i] = Position3f(shoulder_width, 240, static_cast<float>(50 + i * 250));
	}
	std::vector<float> edgeLensL(positionsL.size() - 1);
	std::vector<float> edgeLensR(positionsR.size() - 1);
	for (auto i = 0u; i < edgeLensL.size(); ++i) {
		edgeLensL[i] = (positionsL[i + 1u] - positionsL[i]).Length();
		edgeLensR[i] = (positionsR[i + 1u] - positionsR[i]).Length();
	}

	DxLib::SetUseZBuffer3D(true);
	DxLib::SetWriteZBuffer3D(true);

	float angle = 0.0f;

	VECTOR campos = GetCameraPosition();
	float angleH = GetCameraAngleHRotate();
	float fistAngleAxis = 0.0f;//��{�̌������猝���[�J��Z���S�ɂǂꂭ�炢��]���Ă邩(���[��)
	Vector3f rotV(1, 0, 0);//IK���S��
	const Vector3f leftToBodyVec = {};//�̑��̃x�N�g��

	//VECTOR rotV = VGet(0, 0, 1);///IK���S��
	enum class EPositionType {
		Shoulder=0,
		Elbow=1,
		Fist=2
	};

	//���r�̃��[��
	//�@�O�r�̉�]�����͐e�w����(���[�J��Y)��������(���[�J��X)
	//�A�I�͓����ɂ͋Ȃ���Ȃ�
	//�B�I�͏㑤�ɂ͋Ȃ���Ȃ�
	//�]���藝�̂��ߊp�x�͊m�肵�Ă���B�v�͎����ǂ����邩��
	//�e�w������Y�Ƃ���
	//�i�b�N��������Z�Ƃ���
	//����n�̏ꍇ�A�E����X�Ȃ̂�X=Cross(Y,Z)�ł���B
	//���r�̎��́u�܂������v���珬�w(-Y)�������͍���(-X)�ɋȂ���Ȃ�
	//�E�r�̎��́u�܂������v���珬�w(-Y)�������͉E��(+X)�ɋȂ���Ȃ�
	//�܂��e�w�����ƃ^�[�Q�b�g����x�N�g�������
	//�ŁA�����ɍ��v���Ȃ��ꍇ�̓^�[�Q�b�g�Ǝ�̍b���̃x�N�g����p���Ă��Ȃ���
	//�A�̃`�F�b�N�c
	//���r�F�܂����~�^�[�Q�b�g�����Ƃ��遨���ʃ^�[�Q�b�g�x�N�g���~��r�x�N�g������������Ă���E(0,1,0)>0���e�w�ł�蒼��

	//�ЂƂ܂��e�w�ƃ^�[�Q�b�g�x�N�g��(�����猝)�̊O�ς����Ƃ���
	//�������A���ϒl�̐�Βl��1�ɋ߂�����ꍇ�͎�̍b�������Ƃ���

	//�����O�r�{��r���O�̏ꍇ�͕I�Ȃ����s��Ȃ����ߍ��W�������ė]���藝����
	//�Ȃ���ꍇ����{�I�ɑO�r�̌����͎�̎w��(��)�����Ɠ���Ƃ���

	while (ProcessMessage() == 0) {
		ClearDrawScreen();
		SetCameraPositionAndAngle(campos, 0, angleH, 0);


		GetHitKeyStateAll(keystate);
		if (keystate[KEY_INPUT_LEFT]) {
			angleH -= 0.01f;
		}
		if (keystate[KEY_INPUT_RIGHT]) {
			angleH += 0.01f;
		}
		if (keystate[KEY_INPUT_A]) {
			campos.x -= 4.f;
		}
		if (keystate[KEY_INPUT_D]) {
			campos.x += 4.f;
		}

		if (keystate[KEY_INPUT_W]) {
			campos.z += 4.f;
		}
		if (keystate[KEY_INPUT_S]) {
			campos.z -= 4.f;
		}
		if (keystate[KEY_INPUT_R]) {
			fistAngleAxis += 0.05f;
		}
		if (keystate[KEY_INPUT_E]) {
			fistAngleAxis -= 0.05f;
		}
		fistAngleAxis=std::min(std::max(fistAngleAxis, -DX_PI_F / 2.0f),DX_PI_F/2.0f);

		//auto tmpRotV = ToVector3f(VTransformSR(ToVECTOR(rotV), MGetRotX(fistAngleAxis)));
		
		DxLib::DrawBox(0, 0, screen_width, screen_height, 0xaaaaaa, true);


		//for (int i = -5; i < 15; ++i) {
		//	for (int j = -5; j < 20; ++j) {
		//		DrawSphere3D(VGet(i * 100, -60, j * 100), 50, 32, 0xff0000, 0xaaaaaa, true);
		//	}
		//}


		positionsL[2] = Position3f(-shoulder_width, 240 - sin(angle) * 120, 425 + cos(angle) * 120);
		positionsR[2] = Position3f(shoulder_width, 240 - sin(angle+DX_PI_F) * 120, 425 + cos(angle + DX_PI_F) * 120);//�ʑ������炷
		angle += 0.05f;

		VECTOR targetVectorL;
		MATRIX mL;
		VECTOR vL;
		float thetaL;
		//���r
		{
			//position[0]��position[2]�̈ʒu�֌W����position[1]�����߂�
			auto targetVec = positionsL[2] - positionsL[0];//��������ւ̃x�N�g��(�����A)
			float A = targetVec.Length();//��������ւ̃x�N�g���̒���
			float B = edgeLensL[0];//������I�ւ̃x�N�g���̒���
			float C = edgeLensL[1];//�I������ւ̃x�N�g���̒���
			targetVec = targetVec.Normalized();

			//�]���藝��p���Ċp�x�����߂�
			thetaL = acosf((A * A + B * B - C * C) / (2 * A * B));

			targetVectorL = ToVECTOR(targetVec);
			mL = MGetRotAxis(VCross(targetVectorL, VCross(VGet(1, 0, 0), targetVectorL)), thetaL);//���ƃ^�[�Q�b�g�̊O��
			vL = VTransformSR(targetVectorL, mL);

			if (A < B + C) {
				positionsL[1] = positionsL[0] + ToVector3f(vL).Normalized() * edgeLensL[0];
			}
			else {
				positionsL[1] = positionsL[0] + targetVec * edgeLensL[0];
			}

			////�^�[�Q�b�g�~�I�̃`�F�b�N
			//auto elbowVec = ToVECTOR(positionsL[1] - positionsL[0]);
			//auto vc = VCross(targetVectorL, elbowVec);
			//if (VDot(VGet(0, 1, 0), vc) > 0) {
			//	//���Ȃ���
			//	mL = MGetRotAxis(VCross(targetVectorL, VCross(VGet(0, 1, 0), targetVectorL)), thetaL);//�e�w�ƃ^�[�Q�b�g�̊O��
			//	vL = VTransformSR(targetVectorL, mL);
			//	if (A < B + C) {
			//		positionsL[1] = positionsL[0] + ToVector3f(vL).Normalized() * edgeLensL[0];
			//	}
			//	else {
			//		positionsL[1] = positionsL[0] + targetVec * edgeLensL[0];
			//	}
			//}

		}
		//�E�r
		{
			//position[0]��position[2]�̈ʒu�֌W����position[1]�����߂�
			auto targetVec = positionsR[2] - positionsR[0];//��������ւ̃x�N�g��(�����A)
			float A = targetVec.Length();//��������ւ̃x�N�g���̒���
			float B = edgeLensR[0];//������I�ւ̃x�N�g���̒���
			float C = edgeLensR[1];//�I������ւ̃x�N�g���̒���
			targetVec = targetVec.Normalized();

			//�]���藝��p���Ċp�x�����߂�
			float theta = acosf((A * A + B * B - C * C) / (2 * A * B));

			auto targetVector = ToVECTOR(targetVec);
			auto m = MGetRotAxis(VCross(targetVector, VCross(VGet(1, 0, 0), targetVector)), theta);
			auto v = VTransformSR(targetVector, m);

			if (A < B + C) {
				positionsR[1] = positionsR[0] + ToVector3f(v).Normalized() * edgeLensR[0];
			}
			else {
				positionsR[1] = positionsR[0] + targetVec * edgeLensR[0];
			}
		}


		Vector3f lookat = ( positionsL[2] - positionsL[1]).Normalized();
		Vector3f up = { 0,1,0 };
		
		auto lookAtMat = GetLookAtMatrix(lookat, up);
		MATRIX mat = lookAtMat;
		mat = MMult(mat, MGetRotAxis(ToVECTOR(lookat),fistAngleAxis));
		mat = MMult(mat,MGetTranslate(ToVECTOR(positionsL[2])));
		//�������񂱂̃}�g���N�X��Y����(�e�w����)��ϊ�����
		auto newY = VTransformSR(VGet(0,1,0),mat);
		auto newAxis= VTransformSR(VGet(-1, 0, 0), mat);
		if (fabsf(VDot(newY, targetVectorL) < 0.99)) {
			newAxis= VNorm( VCross(newY,targetVectorL ));//���Ȃ̂Ń}�C�i�X
		}
		mL = MGetRotAxis(VCross(targetVectorL, VCross(newAxis, targetVectorL)), thetaL);
		vL = VTransformSR(targetVectorL, mL);
		//���Čv�Z
		{
			auto targetVec = ToVector3f(targetVectorL);// positionsL[2] - positionsL[0];//��������ւ̃x�N�g��(�����A)
			float A = targetVec.Length();//��������ւ̃x�N�g���̒���
			float B = edgeLensL[0];//������I�ւ̃x�N�g���̒���
			float C = edgeLensL[1];//�I������ւ̃x�N�g���̒���
			if (A < B + C) {
				positionsL[1] = positionsL[0] + ToVector3f(vL).Normalized() * edgeLensL[0];
			}
			else {
				positionsL[1] = positionsL[0] + targetVec * edgeLensL[0];
			}
			//�^�[�Q�b�g�~�I�̃`�F�b�N
			auto elbowVec = ToVECTOR(positionsL[1] - positionsL[0]);
			auto vc = VCross(targetVectorL, elbowVec);
			if (VDot(VGet(0, 1, 0), vc) > 0) {

				mL = MGetRotAxis(VCross(targetVectorL, VNorm(VCross(targetVectorL, newY))), thetaL);
				vL = VTransformSR(targetVectorL, mL);

				//���Ȃ���
				//mL = MGetRotAxis(VCross(targetVectorL, VCross(VGet(0, -1, 0), targetVectorL)), thetaL);//�e�w�ƃ^�[�Q�b�g�̊O��
				//vL = VTransformSR(targetVectorL, mL);
				if (A < B + C) {
					positionsL[1] = positionsL[0] + ToVector3f(vL).Normalized() * edgeLensL[0];
				}
				else {
					positionsL[1] = positionsL[0] + targetVec * edgeLensL[0];
				}
			}
		}
		//�Čv�Z
		lookat = (positionsL[2] - positionsL[1]).Normalized();
		auto upVector = VTransformSR(VGet(0, 1, 0),MMult(mL,MGetRotAxis(ToVECTOR(lookat),fistAngleAxis)));// VTransformSR(VGet(0, 1, 0), m);
		lookAtMat = GetLookAtMatrix(lookat, ToVector3f(upVector));

		//mat = lookAtMat;
		//mat = MMult(mat, MGetTranslate(ToVECTOR(positions[2])));
		DxLib::MV1SetMatrix(lglove, mat);
		DxLib::MV1DrawModel(lglove);

		//�`��
		DrawCircle3D_XY(Position3f(425, 240, 0), 120, 32, 0x000000);
		for (int i = 0; i < positionsL.size(); ++i) {
			if (i < positionsL.size() - 1) {
				DrawSphere3D(ToVECTOR(positionsL[i]), 20, 32, 0xeeeeee, 0xffffff, true);
				DrawCapsule3D(ToVECTOR(positionsL[i]),
					ToVECTOR(positionsL[i + 1u]), 5, 32,
					0xeeffee, 0xffffff, true);
				DrawSphere3D(ToVECTOR(positionsR[i]), 20, 32, 0xeeeeee, 0xffffff, true);
				DrawCapsule3D(ToVECTOR(positionsR[i]),
					ToVECTOR(positionsR[i + 1u]), 5, 32,
					0xeeffee, 0xffffff, true);
			}
		}

		std::copy(begin(keystate), end(keystate), begin(lastKeyState));

		DxLib::ScreenFlip();
	}
	DxLib_End();
	return 0;
}
