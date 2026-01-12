#include "pch.h"
#include <assert.h>// C 언어 전처리기 매크로 중 하나이며 이 매크로는 표명을 구현하여 프로그램이 추정한 것을 확인하며 거짓인 경우 진단 메시지를 출력한다.  출처https://blog.naver.com/kawomo/221204334797
#include "LdhConvert.h"

void LdhDibToImage(LdhDib& dib, LdhByteImage& img)
{
	assert(dib.IsValid());
	assert(dib.GetBitCount() == 8);

	int w = dib.GetWidth();
	int h = dib.GetHeight();
	int ws = (w + 3) & ~3;
	BYTE* pDIBits = dib.GetDiBitsAddr();

	img.CreateImage(w, h);
	BYTE** pixels = img.GetPixel2D();

	for (int i = 0; i < h; i++)
	{
		memcpy(pixels[i], &pDIBits[(h - 1 - i) * ws], w);
	}
}

void LdhDibToImage(LdhDib& dib, LdhRGBImage& img) {
	assert(dib.IsValid());
	assert(dib.GetBitCount() == 24);

	int w = dib.GetWidth();
	int h = dib.GetHeight();
	int ws = (w * 3 + 3) & ~3;
	BYTE* pDIBits = dib.GetDiBitsAddr();

	img.CreateImage(w, h);
	RGBBYTE** pixels = img.GetPixel2D();

	for (int i = 0; i < h; i++)
		memcpy(pixels[i], &pDIBits[(h - 1 - i)* ws], w * 3);

}


void LdhImageToDib(LdhByteImage& img, LdhDib& dib) {
	assert(img.IsValid());

	int w = img.GetWidth();
	int h = img.GetHeight();
	int ws = (w + 3) & ~3;
	BYTE** pixels = img.GetPixel2D(); // 2차원포인터

	dib.CreateGrayBitmap(w, h);
	BYTE* pDIBits = dib.GetDiBitsAddr();

	for (int i = 0; i < h; i++) {
		memcpy(&pDIBits[(h - 1 - i) * ws], pixels[i], w);
	}

}

void LdhImageToDib(LdhFloatImage& img, LdhDib& dib)
{
	assert(img.IsValid());

	int w = img.GetWidth();
	int h = img.GetHeight();
	int ws = (w + 3) & ~3;

	float** pixels = img.GetPixel2D();

	dib.CreateGrayBitmap(w, h);
	BYTE* pDIBits = dib.GetDiBitsAddr();

	int i, j;
	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++)
			pDIBits[(h - 1 - j) * ws + i] = static_cast<BYTE>(limit(pixels[j][i] + 0.5f));

}

void LdhImageToDib(LdhRGBImage& img, LdhDib& dib)
{
	assert(img.IsValid());

	int w = img.GetWidth();
	int h = img.GetHeight();
	int ws = (w * 3 + 3) & ~3;
	RGBBYTE** pixels = img.GetPixel2D();

	dib.CreateRGBBitmap(w, h);
	BYTE* pDIBits = dib.GetDiBitsAddr();

	for (int i = 0; i < h; i++)
		memcpy(&pDIBits[(h - 1 - i) * ws], pixels[i], w * 3);
}