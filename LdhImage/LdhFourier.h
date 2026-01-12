#pragma once
#include "LdhImage.h"

class LdhFourier
{
public: 
	int width;
	int height;
	LdhDoubleImage real; // 실수부 
	LdhDoubleImage imag; // 허수부

public :
	LdhFourier(); // 생성자 멤버변수의 값들 초기화
	
	void SetImage(LdhByteImage& img);	// width, height 멤버 변숫값을 설정하고,입력 영상을 real영상에 복사 , imag영상은 0으로 초기화
	void GetImage(LdhByteImage& img);	// real영상을 LdhByteIMage 타입의 영상 img로 복사한다.
	void GetSpectrumImage(LdhByteImage& img);	// 푸리에 변환 후 호출되어 푸리에 스펙트럼 영상을 생성
	void GetPhaseImage(LdhByteImage& img);		// 푸리에 변환 후 호출되어 푸리에 위상값 영상을 생성

	// 영상의 푸리에 변환 함수
	void DFT(int dir);	// 영상의 이산 푸리에 변환 또는 역변환을 수행, dir 값이 1이면 순방향 변환, 01 이면 역방향 변환을 수행한다
	void DFTR(int dir);	// 영상의 행과 열을 분리하여 영상의 이산 푸리에 변환 또는 역변환을 수행한다.
	void FFT(int dir);	// 영상의 고속 이산 푸리에 변환 또는 역변환을 수행한다.

	// 주파수 공간에서의 필터링 함수
	void LowPassIdeal(int cutoff);		// 주파수 공간에서 이상적 저역 통과 필터를 수행한다.
	void HighPassIdeal(int cutoff);		// 주파수 공간에서 이상적 고역 통과 필터를 수행한다.
	void LowPassGaussian(int cutoff);	// 주파수 공간에서 가우시안 저역 통과 필터를 수행한다.
	void HighPassGaussian(int cutoff);	// 주파수 공간에서 가우시안 고역 통과 필터를 수행한다.

};
void DFT1d(double* re, double* im, int N, int dir); // 20241027 1차원 데이터에 대한 이산 푸리에 변환 함수
// re와 im 배열에 저장된 값을 이용하여 이산 푸리에 변환 또는 역변환을 수행한다. 
// N은배열의 크기를 나타낸다. dir값이 1이면 순반향 변화 -1이면 역반환 변환을 수행한다.
// 푸리에 변환 결관는 다시 re와 im에 저장된다.
void FFT1d(double* re, double* im, int N, int dir); // 20241027 고속 이산 푸리에변환 함수
// DFTd와 동일
bool IsPowerOf2(int n);
// n이 2의 승스로 표현되는 수이면 True를 반환 하고, 아니면 false를 반환한다. FF1d 함수에서 사용됨




