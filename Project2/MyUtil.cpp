#include "MyUtil.h"

//==========================================================================
// �����֌W
//==========================================================================
static unsigned long mRnd = 12345;
const unsigned long alpha = 12869;
const unsigned long beta = 6925;

//----------------------------------------------------------------------
// �Č��\�ȗ����𓾂�
unsigned long Rnd(unsigned long n) {
	mRnd = alpha * mRnd + beta;
	return mRnd % n;
}

//==========================================================================
// �O�p�֐�
//==========================================================================

float SinCosTable[4096 + 1024];

//----------------------------------------------------------------------
// �e�[�u��������
void InitUtil(void) {
	// Sin/Cos�e�[�u��������
	for (int theta = 0; theta < 4096 + 1024; theta++)
		SinCosTable[theta] = (float)sin(theta * 2.0 * PI / 4096.0);
	// �������ꉞ������
	mRnd = 12345;
}

//----------------------------------------------------------------------
// sin
float Sin(int theta) {
	return SinCosTable[theta & 4095];
}
//----------------------------------------------------------------------
// cos
float Cos(int theta) {
	return SinCosTable[(theta + 1024) & 4095];
}

// ArcTan
int ArcTan(float x, float y) {
	float at = atan2((float)y, (float)x);
	return (int)(at * 2048 / PI);
}

int getRand(int min, int max) {
	return Rnd(max - min + 1) + min;
}
float getRandF(int min, int max) {
	return (float)getRand(min, max);
}