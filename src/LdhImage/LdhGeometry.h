#pragma once

#include "LdhImage.h"

void LdhTranslate(LdhByteImage& imgSrc, LdhByteImage& imgDst, int sx, int sy);		// 20241024 이동 변환 함수
void LdhResizeNearest(LdhByteImage& imgSrc, LdhByteImage& imgDst, int nw, int nh);	// 20241024 이웃 보간법 함수
void LdhResizeBilinear(LdhByteImage& imgSrc, LdhByteImage& imgDst, int nw, int nh);	// 20241024 양선형 보간법 함수
void LdhResizeCubic(LdhByteImage& imgSrc, LdhByteImage& imgDst, int nw, int nh);	// 20241024 3차회선 보간법 함수

void LdhRotate(LdhByteImage& imgSrc, LdhByteImage& imgDst, double angle);			// 20241025 회전이미지 회전 변환함수
void LdhRotate90(LdhByteImage& imgSrc, LdhByteImage& imgDst);						// 20241026  90도 회전 함수
void LdhRotate180(LdhByteImage& imgSrc, LdhByteImage& imgDst);						// 20241026 180도 회전 함수
void LdhRotate270(LdhByteImage& imgSrc, LdhByteImage& imgDst);						// 20241026 270도 회전 함수
void LdhMirror(LdhByteImage& imgSrc, LdhByteImage& imgDst);							// 20241026 좌우 대칭 변환 함수
void LdhFlip(LdhByteImage& imgSrc, LdhByteImage& imgDst);							// 20241026 상하 대칭 변환 함수

double cubic_interpolation(double v1, double v2, double v3, double v4, double d);	// 20241024 3차 회선 보간법 함수에서 사용될함수
