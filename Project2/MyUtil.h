#pragma once


#include <vector>
#include <deque>
#include <algorithm>
#include <functional>
#include <iterator>

using namespace std;

#define PI 3.14159265f

// 角度を正規化
#define Normalize(theta)	(theta & 4095)

// 0～(n-1)の乱数を発生
unsigned long Rnd(unsigned long n);			// 再現できる乱数
int getRand(int min, int max);
float getRandF(int min, int max);
//----------------------------------------------------------------------
// テーブルの初期化			(乱数も適当に初期化)
void InitUtil(void);
// Sin,Cos
float Sin(int theta);
float Cos(int theta);

// ArcTan
int ArcTan(float x, float y);