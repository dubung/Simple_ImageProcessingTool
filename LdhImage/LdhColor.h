#pragma once

#include "LdhImage.h"

void LdhInverse(LdhRGBImage& img);							// 20241014 RGB Image 반전 합수, 

void RGB_TO_HSI(double r, double g, double b, double& h, double& s, double& i);	// 20241102 RGB를 HSI로 변환하는 함수
void HSI_TO_RGB(double h, double s, double i, double& r, double& g, double& b);	// 20241102 HSI를 RGB로 변환하는 함수
void RGB_TO_YUV(BYTE r, BYTE g, BYTE b, BYTE& y, BYTE& u, BYTE& v);	// 20241102 RGB를 YUV로 변환하는 함수
void YUV_TO_RGB(BYTE y, BYTE u, BYTE v, BYTE& r, BYTE& g, BYTE& b);	// 20241102 YUV를 RGB로 변환하는 함수

void LdhColorSplitRGB(LdhRGBImage& imgColor, LdhByteImage& imgR, LdhByteImage& imgG, LdhByteImage& imgB);// 20241102 RGB 색상평면 나누는 함수
void LdhColorSplitHSI(LdhRGBImage& imgColor, LdhByteImage& imgH, LdhByteImage& imgS, LdhByteImage& imgI);//	20241102 HSI 색상평면 나누는 함수
void LdhColorSplitYUV(LdhRGBImage& imgColor, LdhByteImage& imgY, LdhByteImage& imgU, LdhByteImage& imgV);//	20241102 YUV 색상평면 나누는 함수

bool LdhColorCombineRGB(LdhByteImage& imgR, LdhByteImage& imgG, LdhByteImage& imgB, LdhRGBImage& imgColor);// 20241104 RGB 색상평면 합치는 함수 
bool LdhColorCombineHSI(LdhByteImage& imgH, LdhByteImage& imgS, LdhByteImage& imgI, LdhRGBImage& imgColor);// 20241104 HSI 색상평면 합치는 함수 
bool LdhColorCombineYUV(LdhByteImage& imgY, LdhByteImage& imgU, LdhByteImage& imgV, LdhRGBImage& imgColor);// 20241104 YUV 색상평면 합치는 함수 

void LdhColorEdge(LdhRGBImage& img, LdhByteImage& imgEdge);	// 20241104 컬러 영상의 엣지검출 함수

