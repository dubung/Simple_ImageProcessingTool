#pragma once
#include"LdhImage.h"

void LdhInverse(LdhByteImage& img);							// 20241010 ByteImage 반전 함수 

void LdhBrightness(LdhByteImage& img, int n);				// 20241014 Byte Image 밝기 조절 함수
void LdhBrightness(LdhRGBImage& img, int n);				// 20241014 RGB Image 밝기 조절 함수

void LdhContrast(LdhByteImage& img, int n);					// 20241014 Byte Image 명암비 조절 함수
void LdhContrast(LdhRGBImage& img, int n);					// 20241014 RGB	Image 명암비 조절 함수

void LdhGammaCorrection(LdhByteImage& img, float gamma);	// 20241015 Byte Image 감마 보정 함수
void LdhGammaCorrection(LdhRGBImage& img, float gamma);		// 20241015 RGB Image 감마 보정 함수

void LdhHistogram(LdhByteImage& img, float histo[256]);		// 20241018 Byte Image histogram 함수
void LdhHistogram(LdhRGBImage& img, float histo[3][256]);	// 20241018	RGB Image histogram 함수

void LdhHistogramStretching(LdhByteImage& img);				// 20241018 Byte Image histogram stretching 함수
void LdhHistogramStretching(LdhRGBImage& img);				// 20241018 RGB Image histogram stretching 함수

void LdhHistogramEqualization(LdhByteImage& img);			// 20241019 Byte Image histogram 평활화 함수
void LdhHistogramEqualization(LdhRGBImage& img);			// 20241019 RGB Image histogram 평활화 함수

bool LdhAdd(LdhByteImage& img1, LdhByteImage& img2, LdhByteImage& img3);	// 20241019 Byte Image 끼리 합연산 하는 함수
bool LdhAdd(LdhRGBImage& img1, LdhRGBImage& img2, LdhRGBImage& img3);		// 20241019 RGB Image 끼리 합연산 하는 함수

bool LdhSub(LdhByteImage& img1, LdhByteImage& img2, LdhByteImage& img3);	// 20241019 Byte Image 끼리 차연산하는 함수
bool LdhSub(LdhRGBImage& img1, LdhRGBImage& img2, LdhRGBImage& img3);		// 20241019 RGB	Image 끼리 차연산하는 함수

bool LdhAve(LdhByteImage& img1, LdhByteImage& img2, LdhByteImage& img3);	// 20241019 Byte Image 끼리 평균연산 하는 함수
bool LdhAve(LdhRGBImage& img1, LdhRGBImage& img2, LdhRGBImage& img3);		// 20241019 RGB Image 끼리 평균연산 하는 함수

bool LdhDiff(LdhByteImage& img1, LdhByteImage& img2, LdhByteImage& img3);	// 20241019 Byte Image 차이연산 하는 함수
bool LdhDiff(LdhRGBImage& img1, LdhRGBImage& img2, LdhRGBImage& img3);		// 20241019 RGB Image 차이연산 하는 함수

bool LdhAND(LdhByteImage& img1, LdhByteImage& img2, LdhByteImage& img3);	// 20241019 Byte Image AND연산 하는 함수
bool LdhAND(LdhRGBImage& img1, LdhRGBImage& img2, LdhRGBImage& img3);		// 20241019 RGB Image AND연산 하는 함수

bool LdhOR(LdhByteImage& img1, LdhByteImage& img2, LdhByteImage& img3);		// 20241019 Byte Image OR연산 하는 함수
bool LdhOR(LdhRGBImage& img1, LdhRGBImage& img2, LdhRGBImage& img3);		// 20241019 RGB Image OR연산 하는 함수

void LdhBitPlane(LdhByteImage& img1, LdhByteImage& img2, int bit);			// 20241020 Byte Image 의 비트 평면을 구하는 함수 