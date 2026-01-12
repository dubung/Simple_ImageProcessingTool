#include "pch.h"
#include "LdhEnhance.h"
#include <math.h>

void LdhInverse(LdhByteImage& img)
{
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p[i] = 255 - p[i];
	}
}


void LdhBrightness(LdhByteImage& img, int n)
{
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p[i] = limit(p[i] + n);
	}
}

void LdhBrightness(LdhRGBImage& img, int n)
{

	int size = img.GetSize();
	RGBBYTE* p = img.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p[i].b = limit(p[i].b + n);
		p[i].g = limit(p[i].g + n);
		p[i].r = limit(p[i].r + n);
	}
}

void LdhContrast(LdhByteImage& img, int n)
{
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p[i] = static_cast<BYTE>(limit(p[i] + (p[i] - 128) * n / 100));
	}
}

void LdhContrast(LdhRGBImage& img, int n)
{
	int size = img.GetSize();
	RGBBYTE* p = img.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p[i].b = static_cast<BYTE>(limit(p[i].b + (p[i].b - 128) * n / 100));
		p[i].g = static_cast<BYTE>(limit(p[i].g + (p[i].g - 128) * n / 100));
		p[i].r = static_cast<BYTE>(limit(p[i].r + (p[i].r - 128) * n / 100));
	}
}

void LdhGammaCorrection(LdhByteImage& img, float gamma)
{
	float inv_gamma = 1.f / gamma; // 역수

	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	float gamma_table[256];// pow 함수는 느리기 때문에 gray scale 같은 경우 256가지 밖에 존재하지않아 먼저 저장을 해둠 look up table

	for (int i = 0; i < 256; i++)
		gamma_table[i] = pow((i / 255.f), inv_gamma); 
	
	for (int i = 0; i < size; i++)
		p[i] = static_cast<BYTE>(limit((gamma_table[p[i]] * 255 + 0.5f)));
}

void LdhGammaCorrection(LdhRGBImage& img, float gamma)
{
	float inv_gamma = 1.f / gamma;

	int size = img.GetSize();
	RGBBYTE* p = img.GetPixels();

	float gamma_table[256];
	for (int i = 0; i < 256; i++)
		gamma_table[i] = pow((i / 256.f), inv_gamma);


	for (int i = 0; i < size; i++) {
		p[i].b = static_cast<BYTE>(limit((gamma_table[p[i].b] * 255 + 0.5f)));
		p[i].g = static_cast<BYTE>(limit((gamma_table[p[i].g] * 255 + 0.5f)));
		p[i].r = static_cast<BYTE>(limit((gamma_table[p[i].r] * 255 + 0.5f)));
	}

}

void LdhHistogram(LdhByteImage& img, float histo[256])
{
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	// 히스토 그램 계산
	int cnt[256];
	memset(cnt, 0, sizeof(int) * 256);

	for (int i = 0; i < size; i++)
		cnt[p[i]]++;

	// 히스토그램 정규화(histogram normalization)
	for (int i = 0; i < 256; i++)
		histo[i] = static_cast<float>(cnt[i]) / size;

}

void LdhHistogram(LdhRGBImage& img, float histo[3][256])
{
	int size = img.GetSize();
	RGBBYTE* p = img.GetPixels();

	// 히스토그램 계산
	int cnt[3][256];

	for(int i = 0; i < 3; i++)
		memset(cnt[i], 0, sizeof(int) * 256);

	for (int i = 0; i < size; i++) {
		cnt[0][p[i].b]++; // Blue
		cnt[1][p[i].g]++; // Green
		cnt[2][p[i].r]++; // Red
	}

	// 히스토그램 정규화(histogram normalization)
	
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 256; j++)
			histo[i][j] = static_cast<float>(cnt[i][j]) / size;

}

void LdhHistogramStretching(LdhByteImage& img)
{
	int size = img.GetSize();
	BYTE* p = img.GetPixels();
	int nLowSize5 , nHiSize5 ;
	nLowSize5 = nHiSize5 = size * 0.05;
	
	// 최대 최소 gray scale 값 계산하기
	BYTE gray_max, gray_min;
	gray_max = gray_min = p[0];

	int ByteTable[256] = { 0, };

	// 정직한 Min Max 값을 사용할 떄
	//for (int i = 0;i < size; i++)
	//{
	//	if (gray_max < p[i]) gray_max = p[i];
	//	if (gray_min > p[i]) gray_min = p[i];
	//}


	// 잡음 추가 되어 0과 255 픽셀이 최소 최대로 잡히지 않게끔 상위 하위 5%의 픽셀을 선정
	for (int i = 0; i < size; i++) {
		ByteTable[p[i]]++;
	}

	for (int i = 0; i < size; i++) {
		nLowSize5 -= ByteTable[i];
		nHiSize5 -= ByteTable[255 - i];
		if (nLowSize5 <= 0) gray_min = p[i];
		if (nHiSize5 <= 0) gray_max = p[i];

		if (nLowSize5 <= 0 && nHiSize5 <= 0)
			return;

	}

	if (gray_min == gray_max)
		return;

	// 히스토그램 스트레칭
	for (int i = 0; i < size; i++)
	{
		// (gray_min, 0) 과 (gray_max,255) 두점을 지나는 직선의 방정식
		p[i] = (p[i] - gray_min) * 255 / (gray_max - gray_min);
	}
}


void LdhHistogramStretching(LdhRGBImage& img)
{
	int size = img.GetSize();
	RGBBYTE* p = img.GetPixels();
	int nLowSize5, nHiSize5;
	

	// 최대 최소 gray scale 값 계산하기
	BYTE byte_max[3] = { p[0].b, p[0].g, p[0].r}, byte_min[3] = { p[0].b, p[0].g, p[0].r };

	BYTE ByteTable[3][256] = { {0,}, };

	// 정직한 Min Max 값을 사용할 떄
	//for(int i =0; i < 3; i++)
	//	for (int j = 0;j < size; j++)
	//	{
	//		if (byte_max[i] < p[j]) byte_max[i] = p[j].b;
	//		if (byte_min[i] > p[j]) byte_min[i] = p[j].b;
	//	}


	// 잡음 추가 되어 0과 255 픽셀이 최소 최대로 잡히지 않게끔 상위 하위 5%의 픽셀을 선정
	for (int i = 0; i < size; i++) {
		ByteTable[0][p[i].b]++;
		ByteTable[1][p[i].g]++;
		ByteTable[2][p[i].r]++;
	}

	for (int i = 0; i < 3; i++) {
		
		nLowSize5 = nHiSize5 = size * 0.05;

		for (int j = 0; j < size; j++) {
			nLowSize5 -= ByteTable[i][j];
			nHiSize5 -= ByteTable[i][255 - j];
			if (nLowSize5 <= 0) {
				BYTE min = 0;
				switch (i)
				{
				case 0:min = p[j].b;
					break;
				case 1:min = p[j].g;
					break;
				case 2:min = p[j].r;
					break;
				}
				
				byte_min[i] = min;
			}
			if (nHiSize5 <= 0) {
				BYTE max = 0;
				switch (i)
				{
				case 0:max = p[j].b;
					break;
				case 1:max = p[j].g;
					break;
				case 2:max = p[j].r;
					break;
				}

				byte_max[i] = max;

			}
			
			if (nLowSize5 <= 0 && nHiSize5 <= 0)
				return;

		}

	}
	if ((byte_min[0] == byte_max[0]) && (byte_min[1] == byte_max[1]) && (byte_min[2] == byte_max[2]))
		return;

	// 히스토그램 스트레칭
	for(int j =0 ; j <3; j++)
		for (int i = 0; i < size; i++)
		{
			// (gray_min, 0) 과 (gray_max,255) 두점을 지나는 직선의 방정식
			p[i].b = (p[i].b - byte_min[j]) * 255 / (byte_max[j] - byte_min[j]);
			p[i].g = (p[i].g - byte_min[j]) * 255 / (byte_max[j] - byte_min[j]);
			p[i].r = (p[i].r - byte_min[j]) * 255 / (byte_max[j] - byte_min[j]);
		}
}

void LdhHistogramEqualization(LdhByteImage& img)
{
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	// 히스토그램 계산
	float hist[GRAY_VALUE];
	LdhHistogram(img, hist);

	// 히스토그램 누적 함수 계산
	float cdf[GRAY_VALUE] = { 0.f, };
	cdf[0] = hist[0];
	
	for (int i = 1; i < GRAY_VALUE; i++)
		cdf[i] = cdf[i - 1] + hist[i];

	// 히스토그램 균등화
	for (int i = 0; i < size; i++)
		p[i] = static_cast<BYTE>(limit(cdf[p[i]] * 255)); // 0~255 의 크기로 스케일링 하기위해서 255 곱해줌

}

void LdhHistogramEqualization(LdhRGBImage& img)
{
	int size = img.GetSize();
	RGBBYTE* p = img.GetPixels();

	// 히스토그램 누적 함수 계산
	float hist[RGB_VALUE][GRAY_VALUE];
	LdhHistogram(img, hist);
	
	// 히스토그램 누적함수 계산
	float cdf[RGB_VALUE][GRAY_VALUE]= { {0.f,}, };

	cdf[R_VALUE][0] = hist[R_VALUE][0];
	cdf[G_VALUE][0] = hist[G_VALUE][0];
	cdf[B_VALUE][0]	= hist[B_VALUE][0];

	for (int i = 1; i < GRAY_VALUE; i++)
	{
		cdf[R_VALUE][i] = cdf[R_VALUE][i - 1] + hist[R_VALUE][i];
		cdf[G_VALUE][i] = cdf[G_VALUE][i - 1] + hist[G_VALUE][i];
		cdf[B_VALUE][i] = cdf[B_VALUE][i - 1] + hist[B_VALUE][i];
	}

	// 히스토그램 균등화
	for (int i = 0; i < size; i++) {
		p[i].r = static_cast<BYTE>(limit(cdf[R_VALUE][p[i].r] * 255));
		p[i].g = static_cast<BYTE>(limit(cdf[G_VALUE][p[i].g] * 255));
		p[i].b = static_cast<BYTE>(limit(cdf[B_VALUE][p[i].b] * 255));
	}


}

bool LdhAdd(LdhByteImage& img1, LdhByteImage& img2, LdhByteImage& img3)
{
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetWidth())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();
	BYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
		p3[i] = limit(p1[i] + p2[i]);

	return true;
}

bool LdhAdd(LdhRGBImage& img1, LdhRGBImage& img2, LdhRGBImage& img3)
{
	int w = img1.GetWidth();
	int h = img2.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	RGBBYTE* p1 = img1.GetPixels();
	RGBBYTE* p2 = img2.GetPixels();
	RGBBYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i].b = p1[i].b + p2[i].b;
		p3[i].g = p1[i].g + p2[i].g;
		p3[i].r = p1[i].r + p2[i].r;
	}

	return true;
}

bool LdhSub(LdhByteImage& img1, LdhByteImage& img2, LdhByteImage& img3)
{
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetWidth())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();
	BYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
		p3[i] = limit(p1[i] - p2[i]);
	return true;
}

bool LdhSub(LdhRGBImage& img1, LdhRGBImage& img2, LdhRGBImage& img3)
{
	int w = img1.GetWidth();
	int h = img2.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	RGBBYTE* p1 = img1.GetPixels();
	RGBBYTE* p2 = img2.GetPixels();
	RGBBYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i].b = limit(p1[i].b - p2[i].b);
		p3[i].g = limit(p1[i].g - p2[i].g);
		p3[i].r = limit(p1[i].r - p2[i].r);
	}


	return true;
}

bool LdhAve(LdhByteImage& img1, LdhByteImage& img2, LdhByteImage& img3)
{
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetWidth())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();
	BYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
		p3[i] = (p1[i] + p2[i]) / 2;

	return true;
}

bool LdhAve(LdhRGBImage& img1, LdhRGBImage& img2, LdhRGBImage& img3)
{
	int w = img1.GetWidth();
	int h = img2.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	RGBBYTE* p1 = img1.GetPixels();
	RGBBYTE* p2 = img2.GetPixels();
	RGBBYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i].b = (p1[i].b + p2[i].b) / 2;
		p3[i].g = (p1[i].g + p2[i].g) / 2;
		p3[i].r = (p1[i].r + p2[i].r) / 2;
	}

	return true;
}

bool LdhDiff(LdhByteImage& img1, LdhByteImage& img2, LdhByteImage& img3)
{
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetWidth())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();
	BYTE* p3 = img3.GetPixels();

	int diff = 0;
	for (int i = 0; i < size; i++) 
	{
		diff = (p1[i] - p2[i]);
		p3[i] = static_cast<BYTE>((diff > 0) ? diff : -diff);
	}

	return true;
}

bool LdhDiff(LdhRGBImage& img1, LdhRGBImage& img2, LdhRGBImage& img3)
{
	int w = img1.GetWidth();
	int h = img2.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	RGBBYTE* p1 = img1.GetPixels();
	RGBBYTE* p2 = img2.GetPixels();
	RGBBYTE* p3 = img3.GetPixels();


	int diff_r = 0, diff_g = 0, diff_b = 0;
	for (int i = 0; i < size; i++)
	{
		diff_b = (p1[i].b - p1[i].b);
		diff_g = (p1[i].g - p1[i].g);
		diff_r = (p1[i].r - p1[i].r);

		p3[i].b = static_cast<BYTE>((diff_b > 0) ? diff_b : -diff_b);
		p3[i].g = static_cast<BYTE>((diff_g > 0) ? diff_g : -diff_g);
		p3[i].r = static_cast<BYTE>((diff_r > 0) ? diff_r : -diff_r);
	}

	return true;
}


bool LdhAND(LdhByteImage& img1, LdhByteImage& img2, LdhByteImage& img3)
{
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetWidth())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();
	BYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
		p3[i] = static_cast<BYTE>(p1[i] & p2[i]);

	return true;
}

bool LdhAND(LdhRGBImage& img1, LdhRGBImage& img2, LdhRGBImage& img3)
{
	int w = img1.GetWidth();
	int h = img2.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	RGBBYTE* p1 = img1.GetPixels();
	RGBBYTE* p2 = img2.GetPixels();
	RGBBYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i].b = static_cast<BYTE>(p1[i].b & p2[i].b);
		p3[i].g = static_cast<BYTE>(p1[i].g & p2[i].g);
		p3[i].r = static_cast<BYTE>(p1[i].r & p2[i].r);
	}

	return true;
}
bool LdhOR(LdhByteImage& img1, LdhByteImage& img2, LdhByteImage& img3)
{
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetWidth())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();
	BYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
		p3[i] = static_cast<BYTE>(p1[i] | p2[i]);

	return true;
}

bool LdhOR(LdhRGBImage& img1, LdhRGBImage& img2, LdhRGBImage& img3)
{
	int w = img1.GetWidth();
	int h = img2.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	RGBBYTE* p1 = img1.GetPixels();
	RGBBYTE* p2 = img2.GetPixels();
	RGBBYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i].b = static_cast<BYTE>(p1[i].b | p2[i].b);
		p3[i].g = static_cast<BYTE>(p1[i].g | p2[i].g);
		p3[i].r = static_cast<BYTE>(p1[i].r | p2[i].r);
	}

	return true;
}

void LdhBitPlane(LdhByteImage& img1, LdhByteImage& img2, int bit)
{
	img2.CreateImage(img1.GetWidth(), img1.GetHeight());

	int size = img1.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p2[i] = (p1[i] & (1 << bit)) ? 255 : 0;
	}
}