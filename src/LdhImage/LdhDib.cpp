#include "pch.h"
#include "LdhDib.h"
#define DIB_HEADER_MARKER ((WORD) ('M' << 8) | 'B')


LdhDib::LdhDib() :m_nWidth(0), m_nHeight(0), m_nBitCount(0), m_nDibSize(0), m_pDib(NULL) {}

LdhDib::LdhDib(const LdhDib& dib) : m_nWidth(dib.m_nWidth), m_nHeight(dib.m_nHeight), m_nBitCount(dib.m_nBitCount), m_nDibSize(dib.m_nDibSize), m_pDib(NULL)
{
	if (dib.m_pDib != NULL) {
		m_pDib = new BYTE[m_nDibSize];
		memcpy(m_pDib, dib.m_pDib, m_nDibSize);
	}
}

LdhDib::~LdhDib() {
	if (m_pDib)
		delete[] m_pDib;
}

BOOL LdhDib::CreateGrayBitmap(LONG nWidth, LONG nHeight) 
{
	if (m_pDib)
		DestroyBitmap();

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nBitCount = 8;

	DWORD dwWidthStep = (m_nWidth * m_nBitCount / 8 + 3) & ~3;
	DWORD dwSizeImage = (m_nHeight * dwWidthStep);
	m_nDibSize = sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD) * GetPaletteNums()) + dwSizeImage;

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL)
		return FALSE;

	// BITMAPINFOHEADER 구조체 설정
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
	lpbmi->biSize = sizeof(BITMAPINFOHEADER);
	lpbmi->biWidth = m_nWidth;
	lpbmi->biHeight = m_nHeight;
	lpbmi->biPlanes = 1;
	lpbmi->biBitCount = m_nBitCount;
	lpbmi->biCompression = BI_RGB;
	lpbmi->biSizeImage = dwSizeImage;
	lpbmi->biXPelsPerMeter = 0;
	lpbmi->biYPelsPerMeter = 0;
	lpbmi->biClrUsed = 0;
	lpbmi->biClrImportant = 0;

	// 그레이스케일 팔레트 설정
	RGBQUAD* pPal = (RGBQUAD*)((BYTE*)m_pDib + sizeof(BITMAPINFOHEADER));
	for (int i = 0; i < 256; i++)
	{
		pPal->rgbBlue = (BYTE)i;
		pPal->rgbGreen = (BYTE)i;
		pPal->rgbRed = (BYTE)i;
		pPal->rgbReserved = 0;
		pPal++;
	}

	// 픽셀 데이터 초기화
	BYTE* pData = GetDiBitsAddr();
	memset(pData, 0, dwSizeImage);

	return TRUE;
}

BOOL LdhDib::CreateRGBBitmap(LONG nWidth, LONG nHeight) {

	if (m_pDib)
		DestroyBitmap();

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nBitCount = 24;// True Color

	DWORD dwWidthStep = (m_nWidth * m_nBitCount / 8 + 3) & ~3;
	DWORD dwSizeImage = (m_nHeight * dwWidthStep);
	m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL) {
		return FALSE;
	}

	// BITMAPINFOHEADER 구조체 설정
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
	lpbmi->biSize = sizeof(BITMAPINFOHEADER);
	lpbmi->biWidth = m_nWidth;
	lpbmi->biHeight = m_nHeight;
	lpbmi->biPlanes = 1;
	lpbmi->biBitCount = m_nBitCount;
	lpbmi->biCompression = BI_RGB;
	lpbmi->biSizeImage = dwSizeImage;
	lpbmi->biXPelsPerMeter = 0;
	lpbmi->biYPelsPerMeter = 0;
	lpbmi->biClrUsed = 0;
	lpbmi->biClrImportant = 0;

	//픽셀 데이터 초기화
	BYTE* pData = GetDiBitsAddr();
	memset(pData, 0, dwSizeImage);

	return TRUE;
}
void LdhDib::DestroyBitmap()
{
	if (m_pDib)
	{
		delete[] m_pDib;
		m_pDib = NULL;
	}
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBitCount = 0;
	m_nDibSize = 0;
}

BOOL LdhDib::Load(const char* fileName) {

	const char* ext = strrchr(fileName, '.');
	if (!_strcmpi(ext, ".bmp"))
		return LoadBMP(fileName);
	else
		return FALSE;
}

BOOL LdhDib::Save(const char* fileName) {

	const char* ext = strrchr(fileName, '.');
	if (!_strcmpi(ext, ".bmp"))
		return SaveBMP(fileName);
	else
		return FALSE;
}

BOOL LdhDib::LoadBMP(const char* fileName) 
{

	FILE* fp = NULL;
	fopen_s(&fp, fileName, "rb");
	if (!fp)
		return false;

	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;

	// BITMAPFILEHEADER 읽기
	if (fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp) != 1) {
		fclose(fp);
		return false;
	}

	// BMP 파일임을 나타내는 "BM" 마커가 있는지 확인
	if (bmfh.bfType != DIB_HEADER_MARKER) {
		fclose(fp);
		return false;
	}

	// BITMAPINFOHEADER 읽기
	if (fread(&bmih, sizeof(BITMAPINFOHEADER), 1, fp) != 1) {
		fclose(fp);
		return false;
	}

	m_nWidth = bmih.biWidth;
	m_nHeight = bmih.biHeight;
	m_nBitCount = bmih.biBitCount;

	// 픽셀 데이터 공간 계산
	DWORD dwWidthStep = (DWORD)((m_nWidth * m_nBitCount / 8 + 3) & ~3);
	DWORD dwSizeImage = m_nHeight * dwWidthStep;

	if (m_nBitCount == 24)
		m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;
	else
		m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << m_nBitCount) + dwSizeImage;

	// Packed-DIB 저장할 메모리 공간 동적 할당
	if (m_pDib)
		DestroyBitmap();

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL)
	{
		fclose(fp);
		return FALSE;
	}

	// 파일로부터 Packed-DIB 크기만큼을 읽기
	fseek(fp, sizeof(BITMAPFILEHEADER), SEEK_SET);
	if (fread(m_pDib, sizeof(BYTE), m_nDibSize, fp) != m_nDibSize)
	{
		delete[] m_pDib;
		m_pDib = NULL;
		fclose(fp);
		return FALSE;
	}

	// 파일 닫기
	fclose(fp);

	return TRUE;

}

BOOL LdhDib::SaveBMP(const char* fileName)
{
	if (!IsValid())
		return FALSE;

	FILE* fp;
	fopen_s(&fp, fileName, "wb");

	if (!fp)
		return FALSE;

	BITMAPFILEHEADER bmfh;
	bmfh.bfType = DIB_HEADER_MARKER;
	bmfh.bfSize = (DWORD)sizeof(BITMAPFILEHEADER) + m_nDibSize;
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = (DWORD)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * GetPaletteNums());

	fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(GetBitmapInfoAddr(), m_nDibSize, 1, fp);

	fclose(fp);

	return TRUE;


}

void LdhDib::Draw(HDC hdc, int dx, int dy)
{
	if (m_pDib == NULL)
		return;
	LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
	LPVOID lpBits = (LPVOID)GetDiBitsAddr();

	::SetDIBitsToDevice(
		hdc,				// hdc	
		dx,					// DestX
		dy,					// DestY
		m_nWidth,			// nSrcWidth
		m_nHeight,			// nSrcHeight
		0,					// SrcX
		0,					// SrcY
		0,					// nStartScan
		m_nHeight,			// nNumScans
		lpBits,				// lpBits
		lpbi,				// lpBitsInfo
		DIB_RGB_COLORS);	// wUsage

}

void LdhDib::Draw(HDC hdc, int dx, int dy, int dw, int dh, DWORD dwRop)
{
	Draw(hdc, dx, dy, dw, dh, 0, 0, m_nWidth, m_nHeight, dwRop);
}

void LdhDib::Draw(HDC hdc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, DWORD dwRop)
{
	if (m_pDib == NULL)
		return;
	LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
	LPSTR lpDIBBits = (LPSTR)GetDiBitsAddr();

	SetStretchBltMode(hdc, COLORONCOLOR);
	::StretchDIBits(
		hdc,			// hdc
		dx,				// XDest
		dy,				// YDest
		dw,				// nDestWidth
		dh,				// nDestHeight
		sx,				// XSrc
		sy,				// YSrc
		sw,				// nSrcWidth
		sh,				// nSrcHeight
		lpDIBBits,		// lpBits
		lpbi,			// lpBitsInfo
		DIB_RGB_COLORS,	// wUSage
		SRCCOPY);		// dwROP
}

BOOL LdhDib::CopyToClipboard() {

	if (!::OpenClipboard(NULL))
		return FALSE;

	// DIB 전체를 전역 메모리 블럭에 복사
	DWORD dwDibSize = GetDibSize();
	HANDLE hDib = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, dwDibSize);

	if (hDib == NULL)
	{
		::CloseClipboard();
		return FALSE;
	}

	LPVOID lpDib = ::GlobalLock((HGLOBAL)hDib);

	if (!lpDib)// lpDib 가 NULL일수도 있음으로 예외처리
		return FALSE;

	memcpy(lpDib, GetBitmapInfoAddr(), dwDibSize);
	::GlobalUnlock(hDib);

	// 클립보드에 데이터 입력
	::EmptyClipboard();
	::SetClipboardData(CF_DIB, hDib);
	::CloseClipboard();

	return TRUE;
}

BOOL LdhDib::PasterFromClipboard()
{

	// CF_DIB 타입이 아니라면 종료 하기위하여
	if (!::IsClipboardFormatAvailable(CF_DIB))
		return FALSE;

	// 클립보드 열기
	if (!::OpenClipboard(NULL)) {
		::CloseClipboard();
		return FALSE;
	}

	// 클립보드 내용을 받아온다.
	HANDLE hDib = ::GetClipboardData(CF_DIB);
	if (hDib == NULL)
	{
		::CloseClipboard();
		return FALSE;
	}

	// 메모리 블럭의 크기는 DIB 전체 크기와 동일
	DWORD dwSize = (DWORD)::GlobalSize((HGLOBAL)hDib);
	LPVOID lpDib = ::GlobalLock((HGLOBAL)hDib);

	if (lpDib != NULL) {

		LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)lpDib;
		m_nWidth = lpbi->biWidth;
		m_nHeight = lpbi->biHeight;
		m_nBitCount = lpbi->biBitCount;
	}
	else {

		::GlobalUnlock(hDib);
		::CloseClipboard();
		return FALSE;
	}

	DWORD dwWidthStep = (DWORD)((m_nWidth * m_nBitCount / 8 + 3) & ~3);
	DWORD dwSizeImage = dwWidthStep * m_nHeight;

	if (m_nBitCount == 24)
		m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;
	else
		m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << m_nBitCount) + dwSizeImage;

	// 현재 설정된 LdhDib 객체가 있으면 삭제한다.
	if (m_pDib)
		DestroyBitmap();

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL)
	{
		::GlobalUnlock(hDib);
		::CloseClipboard();
		return FALSE;
	}
	if (!lpDib)
		return FALSE;

	memcpy(m_pDib, lpDib, m_nDibSize);

	::GlobalUnlock(hDib);
	::CloseClipboard();

	return TRUE;

}

BYTE* LdhDib::GetDiBitsAddr() const
{

	if (m_pDib == NULL)
		return NULL;

	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
	return ((BYTE*)m_pDib + lpbmi->biSize + (sizeof(RGBQUAD) * GetPaletteNums()));

}
int LdhDib::GetPaletteNums() const
{
	switch (m_nBitCount)
	{
	case 1:	return 2;
	case 4: return 16;
	case 8: return 256;
	default: return 0;
	}
}

LdhDib& LdhDib::operator=(const LdhDib& dib)
{
	if (this == &dib) // 재귀검사
		return *this;

	if (m_pDib)
		delete[] m_pDib;

	m_nWidth = dib.m_nWidth;
	m_nHeight = dib.m_nHeight;
	m_nBitCount = dib.m_nBitCount;
	m_nDibSize = dib.m_nDibSize;
	m_pDib = NULL;
	if (dib.m_pDib != NULL) {
		m_pDib = new BYTE[m_nDibSize];
		memcpy(m_pDib, dib.m_pDib, m_nDibSize);
	}
	return *this;
}

void LdhDib::Copy(LdhDib* pImage) {
	
	this->operator=(pImage[0]);// ? 되나 궁금한데 imsi

}