#pragma once
#include <vector>
#include "LdhImage.h"
#include "LdhFeature.h"


void LdhBinarization(LdhByteImage& imgSrc, LdhByteImage& imgDst, int threshold); // 20241104 이진화 함수
int LdhBinarizationIterative(LdhByteImage& imgSrc);	// 20241104 이진화 임계값 결정 알고리즘 함수
void LdhContourTracing(LdhByteImage& imgSrc, int sx, int sy, std::vector<LdhPoint>& cp);	// 20241108 외곽선 추적 구현 함수

void LdhMorphologyErosion(LdhByteImage& imgSrc, LdhByteImage& imgDst);	// 20241109 모폴로지 침식 함수
void LdhMorphologyDilation(LdhByteImage& imgSrc, LdhByteImage& imgDst);	// 20241109 모폴로지 팽창 함수
void LdhMorphologyOpening(LdhByteImage& imgSrc, LdhByteImage& imgDst);	// 20241109 모폴로지 오프닝 함수
void LdhMorphologyClosing(LdhByteImage& imgSrc, LdhByteImage& imgDst);	// 20241109 모폴로지 클로징 함수

void LdhMorphologyGrayErosion(LdhByteImage& imgSrc, LdhByteImage& imgDst);	// 20241109 gray scale 영상의 침식 함수
void LdhMorphologyGrayDilation(LdhByteImage& imgSrc, LdhByteImage& imgDst); // 20241109 gray scale 영상의 팽창 함수
void LdhMorphologyGrayOpening(LdhByteImage& imgSrc, LdhByteImage& imgDst);	// 20241109 gray scale 영상의 오프닝 함수
void LdhMorphologyGrayClosing(LdhByteImage& imgSrc, LdhByteImage& imgDst);	// 20241109 gray scale 영상의 클로징 함수

void LdhFourierDescriptor(LdhByteImage& img, int sx, int sy, int percent, std::vector<LdhPoint>& cp);	// 20241111 경계선 복원 함수

void LdhInvariantMoments(LdhByteImage& img, double m[7]);	// 20241112 불변 모멘트 구현 함수
double LdhGeometricMoment(LdhByteImage& img, int p, int q);	// 20241112 불변 모멘트 구현 함수

bool LdhZernikeMoments(LdhByteImage& img, int n, int m, double& zr, double& zi);	// 20241112 저니키 모멘트 구현 함수

LdhPoint LdhTemplateMatching(LdhByteImage& imgSrc, LdhByteImage& imgTmpl, LdhIntImage& imgMap);	// 20241113 템플릿 매칭 함수

class LdhLabelInfo
{
public: 
	std::vector<LdhPoint> pixels;// 객체 픽셀 좌표 목록
	int cx, cy; // 객체의 중심 좌표
	int minx, miny, maxx, maxy;// 객체를 감싸는 사각형 영역의 좌상귀과 우하귀 점의 좌표

public:
	LdhLabelInfo() : cx(0), cy(0), minx(9999), miny(9999), maxx(0), maxy(0)
	{
		pixels.clear();
	}
};

int LdhLabeling(LdhByteImage& imgSrc, LdhIntImage& imgDst, std::vector<LdhLabelInfo>& labels);



