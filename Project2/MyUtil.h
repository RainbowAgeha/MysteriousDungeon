#pragma once


#include <vector>
#include <deque>
#include <algorithm>
#include <functional>
#include <iterator>

using namespace std;

#define PI 3.14159265f

// �p�x�𐳋K��
#define Normalize(theta)	(theta & 4095)

// 0�`(n-1)�̗����𔭐�
unsigned long Rnd(unsigned long n);			// �Č��ł��闐��
int getRand(int min, int max);
float getRandF(int min, int max);
//----------------------------------------------------------------------
// �e�[�u���̏�����			(�������K���ɏ�����)
void InitUtil(void);
// Sin,Cos
float Sin(int theta);
float Cos(int theta);

// ArcTan
int ArcTan(float x, float y);