#pragma once

#include "LdhImage.h"
#include <vector>


class LdhPoint
{
public :
	int x;
	int y;

public :
	LdhPoint() : x(0), y(0) {}
	LdhPoint(int _x, int _y) : x(_x), y(_y){}
};

class LdhLineParam
{
public: 
	double rho; // p를 나타냄
	double ang; // th를 나타냄
	int vote;	// 축적 배열에서 누적값을 나태냄ㄴ

public:
	LdhLineParam() : rho(0), ang(0), vote(0){}
	LdhLineParam(double r, double a, int v): rho(r), ang(a), vote(v) {}
};

inline bool operator< (const LdhLineParam& lhs, const LdhLineParam& rhs)
{
	if (lhs.vote > rhs.vote)
		return true;
	else
		return false;
}
// 일단 이진화된 엣지를 저장함 일단대기 나중에 GrayScale로 가능학 변경 할 수도있음
void LdhEdgeRoberts(LdhByteImage& img, LdhByteImage& imgEdge);		// 20241030 Roberts 엣지 마스크 함수
void LdhEdgePrewitt(LdhByteImage& img, LdhByteImage& imgEdge);		// 20241030 Prewitt 엣지 마스크 함수
void LdhEdgeSobel(LdhByteImage& img, LdhByteImage& imgEdge);		// 20241030 Sobel 엣지 마스크 함수
void LdhEdgeCanny(LdhByteImage& imgSrc, LdhByteImage& imgEdge, float sigma, float th_low, float th_high); //20241030 Canny 엣지 함수
void LdhHoughLine(LdhByteImage& img, std::vector<LdhLineParam>& lines, int threshold = 60); //20241101 허프변환을 이용한 직선 검출 함수
void LdhDrawLine(LdhByteImage& img, LdhLineParam line, BYTE c);	// 20241101 직선을 그리는 함수
void LdhDrawLine(LdhByteImage& img, int x1, int y1, int x2, int y2, BYTE c);
void LdhHarrisCorner(LdhByteImage& img, std::vector<LdhPoint>& corners, double th);	//20241102 해리스 코너 검출 