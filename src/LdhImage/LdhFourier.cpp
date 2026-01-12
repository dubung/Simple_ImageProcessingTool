#include "pch.h"
#include "LdhFourier.h"

#include <math.h>
#include <algorithm>

const double PI = 3.14159265358979323846;

void DFT1d(double* re, double* im, int N, int dir)
{
	double* tr = new double[N];
	double* ti = new double[N];
	memcpy(tr, re, sizeof(double) * N);
	memcpy(ti, im, sizeof(double) * N);

	register int i, x;
	double sum_re, sum_im, temp;

	for (i = 0; i < N; i++)
	{
		sum_re = sum_im = 0;

		for (x = 0; x < N; x++)
		{
			temp = 2 * dir * PI * ((double)i * x / N);

			sum_re += (tr[x] * cos(temp) + ti[x] * sin(temp));
			sum_im += (ti[x] * cos(temp) - tr[x] * sin(temp));
		}

		re[i] = sum_re;
		im[i] = sum_im;

	}

	if (dir == -1) // IDFT
	{
		for (i = 0; i < N; i++)
		{
			re[i] /= (double)N;
			im[i] /= (double)N;
		}
	}

	delete[] tr;
	delete[] ti;
}

void FFT1d(double* re, double* im, int N, int dir)
{
	register int i, j, k;

	// -----------------------
	// 입력 데이터 순서 바꾸기
	// -----------------------
	
	int n2 = N >> 1;
	int nb = 0;

	while (N != (1 << nb))
		nb++;

	for (i = 0, j = 0; i < N - 1; i++)
	{
		if( i < j ){
			std::swap(re[i], re[j]);
			std::swap(im[i], im[j]);
		}
		k = n2;
		while (k <= j)
		{
			j -= k;
			k >>= 1;
		}
		j+=k;
	}

	// ----------------------------
	// 버터플라이(Butterfly 알고리즘
	// ----------------------------

	int i1, l, l1, l2;
	double c1, c2, t1, t2, u1, u2, z;

	c1 = -1.0;
	c2 = 0.0;
	l2 = 1;

	for (l = 0; l < nb; l++) {
		l1 = l2;
		l2 <<= 1;
		u1 = 1.0;
		u2 = 0.0;

		for (j = 0; j < l1; j++)
		{
			for (i = j; i < N; i += l2)
			{
				i1 = i + l1;
				t1 = u1 * re[i1] - u2 * im[i1];
				t2 = u1 * im[i1] + u2 * re[i1];
				re[i1] = re[i] - t1;
				im[i1] = im[i] - t2;
				re[i] += t1;
				im[i] += t2;
			}

			z = u1 * c1 - u2 * c2;
			u2 = u1 * c2 + u2 * c1;
			u1 = z;
		}
		c2 = sqrt((1.0 - c1) / 2.0);

		if (dir == 1)// Forward
			c2 = -c2;

		c1 = sqrt((1.0 + c1) / 2.0);
	}

	if (dir == -1)// IDFT
	{
		for (i = 0; i < N; i++) {
			re[i] /= static_cast<double>(N);
			im[i] /= static_cast<double>(N);
		}
	}

}

bool IsPowerOf2(int n)
{

	int ref = 1;
	while (ref < n)
		ref <<= 1;

	if (ref == n)
		return true;
	else
		return false;
}

LdhFourier::LdhFourier()
	:width(0), height(0)
{
}

void LdhFourier::SetImage(LdhByteImage& img)
{
	width = img.GetWidth();
	height = img.GetHeight();

	real.Convert(img);					// BYTE Type 을 double 형으로형변환 및 실수부에 영상의 그레이스케일 값 복사
	imag.CreateImage(width, height);	// 허수부는 모두 0으로 초기화
}

void LdhFourier::GetImage(LdhByteImage& img)
{
	if (!real.IsValid()) // 실수형부가 이상하면
		return;

	// 실수부의 값을 이용하여 출력 영상을 생성
	img.CreateImage(width, height);

	double* pRe = real.GetPixels();
	BYTE* ptr = img.GetPixels();

	int size = real.GetSize();
	for (int i = 0; i < size; i++)
	{
		ptr[i] = static_cast<BYTE>(limit(pRe[i] + 0.5));
	}
}

void LdhFourier::GetSpectrumImage(LdhByteImage& img)
{

	register int i, j;

	img.CreateImage(width, height);
	BYTE** ptr = img.GetPixel2D();

	double** pRe = real.GetPixel2D();
	double** pIm = imag.GetPixel2D();

	// 스펙트럼 값을 저장할 임시 버퍼
	LdhDoubleImage spectrum(width, height);
	double** pSpec = spectrum.GetPixel2D();

	//-------------------------------------------------------------------------
	// 복소수 값의 절대값을 로그 변환하여 저장. 
	// 정규화된 결과 이미지 생성을 위하여 스펙트럼의 최대값 계산.
	//-------------------------------------------------------------------------

	double mag, max_value = 0;
	for (j = 0; j < height; j++)
		for (i = 0; i < width; i++)
		{
			mag = sqrt(pRe[j][i] * pRe[j][i] + pIm[j][i] * pIm[j][i]);
			pSpec[j][i] = log(mag + 1);

			if (pSpec[j][i] > max_value)
				max_value = pSpec[j][i];
		}

	//-------------------------------------------------------------------------
	// 출력 영상 생성. 푸리에 변환 결과를 SHIFT 하고, 
	// 출력 영상의 최대값이 255가 되도록 그레이스케일 값을 정규화.
	//-------------------------------------------------------------------------

	int x, y;
	for (j = 0; j < height; j++)
		for (i = 0; i < width; i++)
		{
			x = i + width / 2;
			y = j + height / 2;

			if (x >= width) x -= width;
			if (y >= height) y -= height;

			ptr[j][i] = static_cast<BYTE>(limit(pSpec[y][x] * 255 / max_value));
		}
}

void LdhFourier::GetPhaseImage(LdhByteImage& img)
{
	register int i, j;
	img.CreateImage(width, height);
	BYTE** ptr = img.GetPixel2D();

	double** pRe = real.GetPixel2D();
	double** pIm = imag.GetPixel2D();

	// 위상각을 저장할 임시 버퍼
	LdhDoubleImage phase(width, height);
	double** pPhase = phase.GetPixel2D();

	// ---------------------------------------------------------------
	// 복소수 값의 위상각을 저장, atan2 함수의 변환 값은 -PI ~ PI 이다
	// ---------------------------------------------------------------

	for(j = 0; j < height; j++)
		for (i = 0; i < width; i++)
		{
			pPhase[j][i] = atan2(pIm[j][i], pRe[j][i]);// Rad 단위
		}

	// --------------------------------------------------------
	// 출력 영상 생성, 푸리에 변환 결과를 SHIFT 하고,
	// 출력 영상의 최댓값이 255가 되도록 gray Scale 값을 정규화
	// --------------------------------------------------------

	int x, y;
	for(j =0; j<height; j++)
		for (i = 0; i < width; i++)
		{
			x = i + width / 2;
			y = j + height / 2;

			if (x >= width) x -= width;
			if (y >= height) y -= height;

			ptr[j][i] = static_cast<BYTE>(limit(pPhase[y][x] * 128 / PI + 128));
		}

}

void LdhFourier::DFT(int dir)
{
	if (!real.IsValid())
		return;

	// ------------------------------------------------------
	// real, imag 복사로 생성
	// ------------------------------------------------------
	LdhDoubleImage real_cpy = real;
	LdhDoubleImage imag_cpy = imag;

	double** pRe = real.GetPixel2D();
	double** pIm = imag.GetPixel2D();

	double** pReCpy = real_cpy.GetPixel2D();
	double** pImCpy = imag_cpy.GetPixel2D();

	//-------------------------------------------------------------------------
	// 이산 푸리에 변환 (또는 역변환)
	//-------------------------------------------------------------------------

	register int i, j, x, y;
	double sum_re, sum_im, temp;

	for (j = 0; j < height; j++)
		for (i = 0; i < width; i++)
		{
			sum_re = sum_im = 0;

			for (y = 0; y < height; y++)
				for (x = 0; x < width; x++)
				{
					temp = 2 * dir * PI * (static_cast<double>(i) * x / width + static_cast<double>(j) * y / height);
					sum_re += (pReCpy[y][x] * cos(temp) - pImCpy[y][x] * sin(temp));
					sum_im += (pReCpy[y][x] * sin(temp) + pImCpy[y][x] * sin(temp));
				}

			pRe[j][i] = sum_re;
			pIm[j][i] = sum_im;

			if (dir == -1) // 역변환인 경우
			{
				pRe[j][i] /= (width * height);
				pIm[j][i] /= (width * height);
			}
		}
}

void LdhFourier::DFTR(int dir)
{

	if (!real.IsValid())
		return;

	// -------------------------------------------
	// 행(Row) 단위 이산 푸리에 변환 (또는 역변환)
	// -------------------------------------------

	LdhDoubleImage real_temp, imag_temp;

	real_temp.CreateImage(width, 1);
	imag_temp.CreateImage(width, 1);

	double* re = real_temp.GetPixels();
	double* im = imag_temp.GetPixels();

	double** pRe = real.GetPixel2D();
	double** pIm = imag.GetPixel2D();

	register int i, j;

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			re[i] = pRe[j][i];
			im[i] = pIm[j][i];
		}

		DFT1d(re, im, width, dir);

		for (i = 0; i < width; i++)
		{
			pRe[j][i] = re[i];
			pIm[j][i] = im[i];
		}
	}
		
	// -----------------------------------------------
	// 열(Column) 단위 이사 푸리에 변환 ( 또는 역변환)
	// -----------------------------------------------

	real_temp.CreateImage(height, 1);
	imag_temp.CreateImage(height, 1);

	re = real_temp.GetPixels();
	im = imag_temp.GetPixels();

	memset(re, 0, sizeof(double) * height);
	memset(im, 0, sizeof(double) * height);

	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			re[j] = pRe[j][i];
			im[j] = pIm[j][i];
		}
		
		DFT1d(re, im, height, dir);

		for (j = 0; j < height; j++)
		{
			pRe[j][i] = re[j];
			pIm[j][i] = im[j];
		}
	}



}

void LdhFourier::FFT(int dir)
{
	if (!real.IsValid())
		return;

	if (!IsPowerOf2(width) || !IsPowerOf2(height))
		return;

	// -----------------------------------------
	// 행(ROW) 단위 이산 푸리에 변환 또는 역변환
	// -----------------------------------------

	LdhDoubleImage real_temp, imag_temp;

	real_temp.CreateImage(width, 1);
	imag_temp.CreateImage(width, 1);

	double* re = real_temp.GetPixels();
	double* im = imag_temp.GetPixels();

	double** pRe = real.GetPixel2D();
	double** pIm = imag.GetPixel2D();

	register int i, j;

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			re[i] = pRe[j][i];
			im[i] = pIm[j][i];
		}

		FFT1d(re, im, width, dir);

		for (i = 0; i < width; i++)
		{
			pRe[j][i] = re[i];
			pIm[j][i] = im[i];
		}
	}

	// --------------------------------------------
	// 열(COLUMN) 단위 이산 푸리에 변환 또는 역변환
	// --------------------------------------------

	real_temp.CreateImage(height, 1);
	imag_temp.CreateImage(height, 1);

	re = real_temp.GetPixels();
	im = imag_temp.GetPixels();

	memset(re, 0, sizeof(double) * height);
	memset(im, 0, sizeof(double) * height);

	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			re[j] = pRe[j][i];
			im[j] = pIm[j][i];
		}

		FFT1d(re, im, height, dir);

		for (j = 0; j < height; j++)
		{
			pRe[j][i] = re[j];
			pIm[j][i] = im[j];
		}

	}
}

void LdhFourier::LowPassIdeal(int cutoff)
{
	register int i, j, x, y;
	int cx = width / 2;
	int cy = height / 2;

	double** pRe = real.GetPixel2D();
	double** pIm = imag.GetPixel2D();

	for(j = 0; j < height; j++) 
		for (i = 0; i < width; i++)
		{
			x = i + cx;
			y = j + cy;

			if (x >= width) x -= width;
			if (y >= height) y -= height;

			if ((x - cx) * (x - cx) + (y - cy) * (y - cy) > (cutoff * cutoff))
				pRe[j][i] = pIm[j][i] = 0;
		}
}

void LdhFourier::HighPassIdeal(int cutoff)
{
	register int i, j, x, y;
	int cx = width / 2;
	int cy = height / 2;

	double** pRe = real.GetPixel2D();
	double** pIm = imag.GetPixel2D();

	for( j = 0; j< height; j++)
		for (i = 0; i < width; i++)
		{
			x = i + cx;
			y = j + cy;

			if (x >= width) x -= width;
			if (y >= height) y -= height;

			if ((x - cx) * (x - cx) + (y - cy) * (y - cy) < (cutoff * cutoff))
				pRe[j][i] = pIm[j][i] = 0;
		}
}

void LdhFourier::LowPassGaussian(int cutoff)
{
	register int i, j, x, y;
	double dist2, hval;

	int cx = width / 2;
	int cy = height / 2;

	double** pRe = real.GetPixel2D();
	double** pIm = imag.GetPixel2D();

	for (j = 0; j < height; j++)
		for (i = 0; i < width; i++)
		{
			x = i + cx;
			y = j + cy;

			if (x >= width) x -= width;
			if (y >= height) y -= height;

			dist2 = static_cast<double>((x - cx) * (x - cx) + (y - cy) * (y - cy));

			hval = exp(-dist2 / (2 * cutoff * cutoff));

			pRe[j][i] *= hval;
			pIm[j][i] *= hval;
		}
}

void LdhFourier::HighPassGaussian(int cutoff)
{
	register int i, j, x, y;
	double dist2, hval;

	int cx = width / 2;
	int cy = height / 2;

	double** pRe = real.GetPixel2D();
	double** pIm = imag.GetPixel2D();

	for(j = 0; j< height; j++)
		for (i = 0; i < width; i++)
		{
			x = i + cx;
			y = j + cy;

			if (x >= width) x -= width;
			if (y >= height) y -= height;

			dist2 = static_cast<double>((x - cx) * (x - cx) + (y - cy) * (y - cy));

			hval = 1.0 - exp(-dist2 / (2 * cutoff * cutoff));

			pRe[j][i] *= hval;
			pIm[j][i] *= hval;
		}
}
