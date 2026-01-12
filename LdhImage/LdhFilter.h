#pragma once

#include "LdhImage.h"

void LdhFilterMean(LdhByteImage& imgSrc, LdhByteImage& imgDst);						// 20241021 평균 filter 함수
void LdhFilterWeightedMean(LdhByteImage& imgSrc, LdhByteImage& imgDst);				// 20241021 가중 filter 함수
void LdhFilterGaussian(LdhByteImage& imgSrc, LdhFloatImage& imgDst, float sigma);	// 20241022 가우시안 filter 함수
void LdhFilterLaplacian(LdhByteImage& imgSrc, LdhByteImage& imgDst);				// 20241023 라플라시안 filter함수
void LdhFilterUnsharpMask(LdhByteImage& imgSrc, LdhByteImage& imgDst);				// 20241023 언샤프 filter함수
void LdhFilterHighboost(LdhByteImage& imgSrc, LdhByteImage& imgDst, float alpha);	// 20241023 하이부스트 filter함수
void LdhFilterMedian(LdhByteImage& imgSrc, LdhByteImage& imgDst);					// 20241024 미디언 filter함수
void LdhFIlterDiffusion(LdhByteImage& imgSrc, LdhFloatImage& imgDst, float lambda, float k, int iter);	// 20241024 비등방성 filter 함수

void LdhNoiseGaussian(LdhByteImage& imgSrc, LdhByteImage& imgDst, int amount);		// 20241024 가우시안 잡음 함수
void LdhNoiseSaltNPepper(LdhByteImage& imgSrc, LdhByteImage& imgDst, int amount);	// 20241024 소금& 후추 잡음 함수

