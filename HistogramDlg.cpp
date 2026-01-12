// HistogramDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageTool.h"
#include "HistogramDlg.h"
#include "afxdialogex.h"

#include "LdhImage/LdhDib.h"
#include "LdhImage/LdhImage.h"
#include "LdhImage/LdhConvert.h"
#include "LdhImage/LdhEnhance.h"


// CHistogramDlg 대화 상자

IMPLEMENT_DYNAMIC(CHistogramDlg, CDialogEx)

CHistogramDlg::CHistogramDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HISTOGRAM, pParent)
{
	memset(m_Histogram, 0, sizeof(int) * 256);// 배열 초기화
}

CHistogramDlg::~CHistogramDlg()
{
}

void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CHistogramDlg 메시지 처리기

void CHistogramDlg::SetImage(LdhDib* pDib)
{
	if (pDib != NULL ) {

		if (pDib->GetBitCount() == 8)
		{
			LdhByteImage img;
			LdhDibToImage(*pDib, img);

			// 정규화된 히스토그램을 구한다.
			float histo[256] = { 0.f, };
			LdhHistogram(img, histo);

			// 정규화된 히스토 그램에서  최댓값을 구한다.
			float max_histo = histo[0];
			for (int i = 0; i < 256; i++)
				if (histo[i] > max_histo) max_histo = histo[i];

			// m_Histogram 배열의 최댓값이 100이 되도록 전체 배열의 값을 조절한다.
			for (int i = 0; i < 256; i++)
				m_Histogram[0][i] = static_cast<int>(histo[i] * 100 / max_histo);

		}
		else if (pDib->GetBitCount() == 24) {
			LdhRGBImage img;
			LdhDibToImage(*pDib, img);

			// 정규화된 히스토그램을 구한다.
			float histo[3][256] = { {0.f,}, };
			LdhHistogram(img, histo);

			// 정규화된 히스토그램에서 최대값을 구한다.

			float max_histo[3] = { histo[0][0], histo[1][0], histo[2][0] };

			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 256; j++)
					if (histo[i][j] > max_histo[i]) max_histo[i] = histo[i][j];

			// m_HIstogram 배열의 최대값이 100이 되도록 전체 배열의 값을 조절한다.
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 256; j++)
					m_Histogram[i][j] = static_cast<int>(histo[i][j] * 100 / max_histo[i]);
		}

		m_nImgBitCount = pDib->GetBitCount();

	}
	else {
		memset(m_Histogram, 0, sizeof(int) * 256);
	}
}


void CHistogramDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	CGdiObject* pOldPen = dc.SelectStockObject(DC_PEN);

	CRect rect;
	GetWindowRect(&rect);

	if (m_nImgBitCount == 8) {
		// 히스토 그램 박스 
		dc.SetDCPenColor(RGB(128, 128, 128));
		dc.MoveTo(20, 20);
		dc.LineTo(20, 120);
		dc.LineTo(275, 120);
		dc.LineTo(275, 20);
	
		// 각 GrayScale에 해당하는 히스토그램 출력
		dc.SetDCPenColor(RGB(0, 0, 0));
		for (int i = 0; i < 256; i++) {
			dc.MoveTo(20 + i, 120);
			dc.LineTo(20 + i, 120 - m_Histogram[0][i]);
		}
		// 그레이 스케일 레벨 출력
		for (int i = 0; i < 256; i++) {
			dc.SetDCPenColor(RGB(i, i, i));
			dc.MoveTo(20 + i, 130);
			dc.LineTo(20 + i, 145);
		}
	
		dc.SelectObject(pOldPen);
		MoveWindow(rect.left, rect.top, 310, 200, TRUE);
	}
	else if (m_nImgBitCount == 24)
	{
	
		// 히스토 그램 박스 
		for (int j = 0; j < 3; j++) {
	
			switch (j) {
			case 0:dc.SetDCPenColor(RGB(0, 0, 255));
				break;
			case 1:dc.SetDCPenColor(RGB(0, 255, 0));
				break;
			case 2:dc.SetDCPenColor(RGB(255, 0, 0));
				break;
			}

			dc.MoveTo(20 + j * 300, 20);
			dc.LineTo(20 + j * 300, 120);
			dc.LineTo(275 + j * 300, 120);
			dc.LineTo(275 + j * 300, 20);

			for (int i = 0; i < 256; i++) {
				dc.MoveTo((20 + i) + j * 300, 120);
				dc.LineTo((20 + i) + j * 300, 120 - m_Histogram[j][i]);
			}
			// 그레이 스케일 레벨 출력
			for (int i = 0; i < 256; i++) {
				switch (j) {
				case 0:dc.SetDCPenColor(RGB(0, 0, i));
					break;
				case 1:dc.SetDCPenColor(RGB(0, i, 0));
					break;
				case 2:dc.SetDCPenColor(RGB(i, 0, 0));
					break;
				}
	
				dc.MoveTo((20 + i) + j * 300, 130);
				dc.LineTo((20 + i) + j * 300, 145);
			}
		}

		dc.SelectObject(pOldPen);

		MoveWindow(rect.left, rect.top, 910, 200, TRUE);
	}
	

}
