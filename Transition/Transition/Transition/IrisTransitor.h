#pragma once
#include"Transitor.h"
class IrisTransitor:
    public Transitor
{
private:
	int width_ = 100;
	int handleForMaskScreen_;//�}�X�N�]���p�O���t�B�b�N�n���h��
	int maskH_;//�}�X�N�n���h��
	float diagonalLength_;//�Ίp���̒���
	bool irisOut_=false;//�A�C���X�A�E�g�t���O(false�Ȃ�A�C���X�C��)
	int gHandle_=-1;
	bool isTiled_ = false;
public:
	IrisTransitor(bool irisOut=false,int interval = 60,bool isTiled=false , int gHandle=-1);
	~IrisTransitor();
	virtual void Update() override;
	virtual void Draw() override;
};

