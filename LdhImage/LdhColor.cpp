#include "pch.h"
#include "LdhColor.h"
#include "LdhFeature.h"

const double PI = 3.14159265358979323846;

void LdhInverse(LdhRGBImage& img)
{
	int size = img.GetSize();
	RGBBYTE* p = img.GetPixels();

	for (int i = 0; i < size; i++) {
		p[i].b = 255 - p[i].b;
		p[i].g = 255 - p[i].g;
		p[i].r = 255 - p[i].r;
	}
}


// --------------------------------
// R, G, B, H, S, I 의 범위는 [0,1]
// --------------------------------
void RGB_TO_HSI(double r, double g, double b, double& h, double& s, double& i)
{
	i = (r + g + b) / 3;

	if ((r == g) && (g == b))// gray scale일 경우
	{
		s = 0;
		h = 0;
	}
	else
	{
		double min_rgb;
		double temp;

		min_rgb = min(min(r, g), b);
		s = 1 - (min_rgb / i);
		temp = ((r - g) + (r - b)) / (2 * sqrt((r - g) * (r - g) + (r - b) * (g - b)));

		h = acos(temp) * 180 / PI;

		if (b > g)
			h = 360 - h;
		h /= 360;
	}
}

// --------------------------------
// R, G, B, H, S, I 의 범위는 [0,1]
// --------------------------------
void HSI_TO_RGB(double h, double s, double i, double& r, double& g, double& b)
{
	if (i == 0)// 검정색
	{
		r = 0.0;
		g = 0.0;
		b = 0.0;
		return;
	}
	
	if (s = 0.0)// gray scale 
	{
		r = i;
		g = i;
		b = i;
		return;
	}

	h *= 360;	// [0,1] 범위를 [0,360] 각도 범위로 변경

	if (h <= 120)
	{
		b = i * (1 - s);
		r = i * (1 + s * cos(h * PI / 180) / cos((60 - h) * PI / 180));
		r = limit(r, 0., 1.);
		g = 3 * i - (r + b);
	}
	else if (h <= 240)
	{
		h -= 120;

		r = i * (1 - s);
		g = i * (1 + s * cos(h * PI / 180) / cos((60 - h) * PI / 180));
		g = limit(g, 0., 1.);
		b = 3 * i - (r + g);
	}
	else
	{
		h -= 240;

		g = i * (1 - s);
		b = i * (1 + s * cos(h * PI / 180) / cos((60 - h) * PI / 180));
		b = limit(b, 0., 1.);
		r = 3 * i - (g + b);
	}



}

// -----------------------------------
// R, G, B, Y, U, V 의 범위는 [0, 255]
// -----------------------------------
void RGB_TO_YUV(BYTE r, BYTE g, BYTE b, BYTE& y, BYTE& u, BYTE& v)
{
	y = (BYTE)limit(0.299 * r + 0.587 * g + 0.114 * b + 0.5);
	u = (BYTE)limit(-0.169 * r - 0.331 * g + 0.500 * b + 128 + 0.5);
	v = (BYTE)limit(0.5 * r - 0.419 * g + 0.081 * b + 128 + 0.5);
}

// -----------------------------------
// R, G, B, Y, U, V 의 범위는 [0, 255]
// -----------------------------------
void YUV_TO_RGB(BYTE y, BYTE u, BYTE v, BYTE& r, BYTE& g, BYTE& b)
{
	r = (BYTE)limit(y + 1.4075 * (v - 128) + 0.5);
	g = (BYTE)limit(y - 0.3455*(u -128) - 0.7169*(v-128) + 0.5);
	b = (BYTE)limit(y + 1.7790 * (u - 128) + 0.5);

}

void LdhColorSplitRGB(LdhRGBImage& imgColor, LdhByteImage& imgR, LdhByteImage& imgG, LdhByteImage& imgB)
{
	int w = imgColor.GetWidth();
	int h = imgColor.GetHeight();
	int size = imgColor.GetSize();

	imgR.CreateImage(w, h);
	imgG.CreateImage(w, h);
	imgB.CreateImage(w, h);

	RGBBYTE* pColor = imgColor.GetPixels();
	BYTE* pR = imgR.GetPixels();
	BYTE* pG = imgG.GetPixels();
	BYTE* pB = imgB.GetPixels();

	for (int i = 0; i < size; i++) {
		pR[i] = pColor[i].r;
		pG[i] = pColor[i].g;
		pB[i] = pColor[i].b;
	}
}

void LdhColorSplitHSI(LdhRGBImage& imgColor, LdhByteImage& imgH, LdhByteImage& imgS, LdhByteImage& imgI)
{
	int w = imgColor.GetWidth();
	int h = imgColor.GetHeight();
	int size = imgColor.GetSize();

	imgH.CreateImage(w, h);
	imgS.CreateImage(w, h);
	imgI.CreateImage(w, h);

	RGBBYTE* pColor = imgColor.GetPixels();
	BYTE* pH = imgH.GetPixels();
	BYTE* pS = imgS.GetPixels();
	BYTE* pI = imgI.GetPixels();

	double r, g, b, hh, ss, ii;
	for (int i = 0; i < size; i++) {
		r = pColor[i].r/255;
		g =	pColor[i].g/255;
		b =	pColor[i].b/255;

		RGB_TO_HSI(r, g, b, hh, ss, ii);

		pH[i] = static_cast<BYTE>(limit(hh*255 + 0.5));
		pS[i] = static_cast<BYTE>(limit(ss*255 + 0.5));
		pI[i] = static_cast<BYTE>(limit(ii*255 + 0.5));

	}
}

void LdhColorSplitYUV(LdhRGBImage& imgColor, LdhByteImage& imgY, LdhByteImage& imgU, LdhByteImage& imgV)
{
	int w = imgColor.GetWidth();
	int h = imgColor.GetHeight();
	int size = imgColor.GetSize();

	imgY.CreateImage(w, h);
	imgU.CreateImage(w, h);
	imgV.CreateImage(w, h);

	RGBBYTE* pColor = imgColor.GetPixels();
	BYTE* pY = imgY.GetPixels();
	BYTE* pU = imgU.GetPixels();
	BYTE* pV = imgV.GetPixels();

	for (int i = 0; i < size; i++)
	{
		RGB_TO_YUV(pColor[i].r, pColor[i].g, pColor[i].b, pY[i], pU[i], pV[i]);
	}
}

bool LdhColorCombineRGB(LdhByteImage& imgR, LdhByteImage& imgG, LdhByteImage& imgB, LdhRGBImage& imgColor)
{
	int w = imgR.GetWidth();
	int h = imgR.GetHeight();
	int size = imgR.GetSize();

	if (imgG.GetWidth() != w || imgG.GetHeight() != h || imgB.GetWidth() != w || imgB.GetHeight() != h)
		return false;

	imgColor.CreateImage(w, h);

	BYTE* pR = imgR.GetPixels();
	BYTE* pG = imgG.GetPixels();
	BYTE* pB = imgB.GetPixels();
	RGBBYTE* pColor = imgColor.GetPixels();

	for (int i = 0; i < size; i++)
	{
		pColor[i].r = pR[i];
		pColor[i].g = pG[i];
		pColor[i].b	= pB[i];
	}


	return true;
}

bool LdhColorCombineHSI(LdhByteImage& imgH, LdhByteImage& imgS, LdhByteImage& imgI, LdhRGBImage& imgColor)
{
	int w = imgH.GetWidth();
	int h = imgH.GetHeight();
	int size = imgH.GetSize();

	if (imgS.GetWidth() != w || imgS.GetHeight() != h || imgI.GetWidth() != w || imgI.GetHeight() != h)
		return false;

	imgColor.CreateImage(w, h);

	BYTE* pH = imgH.GetPixels();
	BYTE* pS = imgS.GetPixels();
	BYTE* pI = imgI.GetPixels();
	RGBBYTE* pColor = imgColor.GetPixels();

	double r, g, b, hh, ss, ii;
	for (int i = 0; i < size; i++)
	{
		hh = pH[i] / 255.;
		ss = pS[i] / 255.;
		ii = pI[i] / 255.;

		HSI_TO_RGB(hh, ss, ii, r, g, b);

		pColor[i].r = static_cast<BYTE>(limit(r * 255 + 0.5));
		pColor[i].g = static_cast<BYTE>(limit(g * 255 + 0.5));
		pColor[i].b = static_cast<BYTE>(limit(b * 255 + 0.5));

	}

	return true;

}

bool LdhColorCombineYUV(LdhByteImage& imgY, LdhByteImage& imgU, LdhByteImage& imgV, LdhRGBImage& imgColor)
{
	int w = imgY.GetWidth();
	int h = imgY.GetHeight();
	int size = imgY.GetSize();

	if (imgU.GetWidth() != w || imgU.GetHeight() != h || imgV.GetWidth() != w || imgV.GetHeight() != h)
		return false;

	imgColor.CreateImage(w, h);

	BYTE* pY = imgY.GetPixels();
	BYTE* pU = imgU.GetPixels();
	BYTE* pV = imgV.GetPixels();
	RGBBYTE* pColor = imgColor.GetPixels();

	for (int i = 0; i < size; i++)
		YUV_TO_RGB(pY[i], pU[i], pV[i], pColor[i].r, pColor[i].g, pColor[i].b);

	return true;

}

void LdhColorEdge(LdhRGBImage& img, LdhByteImage& imgEdge)
{
	LdhByteImage imgY, imgU, imgV;
	LdhColorSplitYUV(img, imgY, imgU, imgV);

	LdhByteImage edgeY, edgeU, edgeV;
	LdhEdgePrewitt(imgY, edgeY);
	LdhEdgePrewitt(imgU, edgeU);
	LdhEdgePrewitt(imgV, edgeV);

	int w = img.GetWidth();
	int h = img.GetHeight();
	int size = img.GetSize();

	imgEdge.CreateImage(w, h);
	BYTE* pEdge = imgEdge.GetPixels();
	BYTE* pY = edgeY.GetPixels();
	BYTE* pU = edgeU.GetPixels();
	BYTE* pV = edgeV.GetPixels();

	double dist;
	for (int i = 0; i < size; i++)
	{
		dist = (pY[i] * pY[i]) +
			(0.5 * pU[i]) * (0.5 * pU[i]) +
			(0.5 * pV[i]) * (0.5 * pV[i]);
		pEdge[i] = static_cast<BYTE>(limit(sqrt(dist)));
	}

}
