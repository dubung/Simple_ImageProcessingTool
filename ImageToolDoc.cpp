
// ImageToolDoc.cpp: CImageToolDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageTool.h"
#include "CFileNewDlg.h"

#endif

#include "ImageToolDoc.h"
#include <propkey.h>

#include "LdhImage/LdhEnhance.h"
#include "LdhImage/LdhImage.h"
#include "LdhImage/LdhConvert.h"
#include "LdhImage/LdhFilter.h"
#include "LdhImage/LdhGeometry.h"
#include "LdhImage/LdhFourier.h"
#include "LdhImage/LdhFeature.h"
#include "LdhImage/LdhColor.h"
#include "LdhImage/LdhSegment.h"

#include "BrightnessContrastDlg.h"
#include "GammaCorrectionDlg.h"
#include "HistogramDlg.h"
#include "ArithmeticLogicalDlg.h"
#include "GaussianDlg.h"
#include "AddNoiseDlg.h"
#include "DiffusionDlg.h"
#include "TranslateDlg.h"
#include "ResizeDlg.h"
#include "RotateDlg.h"
#include "FreqFilteringDlg.h"
#include "CannyEdgeDlg.h"
#include "HarrisCornerDlg.h"
#include "ColorCombineDlg.h"
#include "BinaizationDlg.h"
#include "FourierDescDlg.h"
#include "OpenCvCamDlg.h"

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include <algorithm>>
#include <functional>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img) \
	LdhByteImage img; \
	LdhDibToImage(m_Dib, img);

#define CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)\
	LdhRGBImage img; \
	LdhDibToImage(m_Dib, img);

#define CONVERT_IMAGE_TO_DIB(img,dib) \
	LdhDib dib;\
	LdhImageToDib(img,dib);

#define SHOW_SPECTRUM_PHASE_IMAGE


// CImageToolDoc

IMPLEMENT_DYNCREATE(CImageToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageToolDoc, CDocument)
	ON_COMMAND(ID_WINDOW_DUPLICATE, &CImageToolDoc::OnWindowDuplicate)
	ON_COMMAND(ID_EDIT_COPY, &CImageToolDoc::OnEditCopy)
	ON_COMMAND(ID_IMAGE_INVERSE, &CImageToolDoc::OnImageInverse)
	ON_COMMAND(ID_BRIGHTNESS_CONTRAST, &CImageToolDoc::OnBrightnessContrast)
	ON_COMMAND(ID_GAMMA_CORRECTION, &CImageToolDoc::OnGammaCorrection)
	ON_COMMAND(ID_VIEW_HISTOGRAM, &CImageToolDoc::OnViewHistogram)
	ON_COMMAND(ID_HISTO_STRETCHING, &CImageToolDoc::OnHistoStretching)
	ON_COMMAND(ID_HISTO_EQUALIZATION, &CImageToolDoc::OnHistoEqualization)
	ON_COMMAND(ID_ARITHMETIC_LOGICAL, &CImageToolDoc::OnArithmeticLogical)
	ON_COMMAND(ID_BITPLANE_SLICING, &CImageToolDoc::OnBitplaneSlicing)
	ON_COMMAND(ID_FILTER_MEAN, &CImageToolDoc::OnFilterMean)
	ON_COMMAND(ID_FILTER_WEIGHTED_MEAN, &CImageToolDoc::OnFilterWeightedMean)
	ON_COMMAND(ID_FILTER_GAUSSIAN, &CImageToolDoc::OnFilterGaussian)
	ON_COMMAND(ID_FILTER_LAPLACIAN, &CImageToolDoc::OnFilterLaplacian)
	ON_COMMAND(ID_FILTER_UNSHARP_MASK, &CImageToolDoc::OnFilterUnsharpMask)
	ON_COMMAND(ID_FILTER_HIGHBOOST, &CImageToolDoc::OnFilterHighboost)
	ON_COMMAND(ID_ADD_NOISE, &CImageToolDoc::OnAddNoise)
	ON_COMMAND(ID_FILTER_MEDIAN, &CImageToolDoc::OnFilterMedian)
	ON_COMMAND(ID_FILTER_DIFFUSION, &CImageToolDoc::OnFilterDiffusion)
	ON_COMMAND(ID_IMAGE_TRANSLATION, &CImageToolDoc::OnImageTranslation)
	ON_COMMAND(ID_IMAGE_RESIZE, &CImageToolDoc::OnImageResize)
	ON_COMMAND(ID_IMAGE_ROTATE, &CImageToolDoc::OnImageRotate)
	ON_COMMAND(ID_IMAGE_MIRROR, &CImageToolDoc::OnImageMirror)
	ON_COMMAND(ID_IMAGE_FLIP, &CImageToolDoc::OnImageFlip)
	ON_COMMAND(ID_FOURIER_DFT, &CImageToolDoc::OnFourierDft)
	ON_COMMAND(ID_FOURIER_DFTRC, &CImageToolDoc::OnFourierDftrc)
	ON_COMMAND(ID_FOURIER_FFT, &CImageToolDoc::OnFourierFft)
	ON_COMMAND(ID_FREQ_FILTERING, &CImageToolDoc::OnFreqFiltering)
	ON_COMMAND(ID_EDGE_ROBERTS, &CImageToolDoc::OnEdgeRoberts)
	ON_COMMAND(ID_EDGE_PREWITT, &CImageToolDoc::OnEdgePrewitt)
	ON_COMMAND(ID_EDGE_SOBEL, &CImageToolDoc::OnEdgeSobel)
	ON_COMMAND(ID_EDGE_CANNY, &CImageToolDoc::OnEdgeCanny)
	ON_COMMAND(ID_HOUGH_LINE, &CImageToolDoc::OnHoughLine)
	ON_COMMAND(ID_HARRIS_CORNER, &CImageToolDoc::OnHarrisCorner)
	ON_COMMAND(ID_COLOR_GRAYSCALE, &CImageToolDoc::OnColorGrayscale)
	ON_UPDATE_COMMAND_UI(ID_COLOR_GRAYSCALE, &CImageToolDoc::OnUpdateColorGrayscale)
	ON_COMMAND(ID_COLOR_SPLIT_RGB, &CImageToolDoc::OnColorSplitRgb)
	ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_RGB, &CImageToolDoc::OnUpdateColorSplitRgb)
	ON_COMMAND(ID_COLOR_SPLIT_HSI, &CImageToolDoc::OnColorSplitHsi)
	ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_HSI, &CImageToolDoc::OnUpdateColorSplitHsi)
	ON_COMMAND(ID_COLOR_SPLIT_SUV, &CImageToolDoc::OnColorSplitYuv)
	ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_SUV, &CImageToolDoc::OnUpdateColorSplitSuv)
	ON_COMMAND(ID_COLOR_COMBINE_RGB, &CImageToolDoc::OnColorCombineRgb)
	ON_COMMAND(ID_COLOR_COMBINE_HSI, &CImageToolDoc::OnColorCombineHsi)
	ON_COMMAND(ID_COLOR_COMBINE_YUV, &CImageToolDoc::OnColorCombineYuv)
	ON_COMMAND(ID_COLOR_EDGE, &CImageToolDoc::OnColorEdge)
	ON_UPDATE_COMMAND_UI(ID_COLOR_EDGE, &CImageToolDoc::OnUpdateColorEdge)
	ON_COMMAND(ID_SEGMENT_BINARIZATION, &CImageToolDoc::OnSegmentBinarization)
	ON_COMMAND(ID_SEGMENT_LABELING_DRAW, &CImageToolDoc::OnSegmentLabelingDraw)
	ON_COMMAND(ID_SEGMENT_LABELING_CUT, &CImageToolDoc::OnSegmentLabelingCut)
	ON_COMMAND(ID_CONTOUR_TRACING, &CImageToolDoc::OnContourTracing)
	ON_COMMAND(ID_MORPHOLOGY_EROSION, &CImageToolDoc::OnMorphologyErosion)
	ON_COMMAND(ID_MORPHOLOGY_DILATION, &CImageToolDoc::OnMorphologyDilation)
	ON_COMMAND(ID_MORPHOLOGY_OPENING, &CImageToolDoc::OnMorphologyOpening)
	ON_COMMAND(ID_MORPHOLOGY_CLOSING, &CImageToolDoc::OnMorphologyClosing)
	ON_COMMAND(ID_GRAYMORPH_EROSION, &CImageToolDoc::OnGraymorphErosion)
	ON_COMMAND(ID_GRAYMORPH_DILATION, &CImageToolDoc::OnGraymorphDilation)
	ON_COMMAND(ID_GRAYMORPH_OPENING, &CImageToolDoc::OnGraymorphOpening)
	ON_COMMAND(ID_GRAYMORPH_CLOSING, &CImageToolDoc::OnGraymorphClosing)
	ON_COMMAND(ID_FOURIER_DESCRIPTOR, &CImageToolDoc::OnFourierDescriptor)
	ON_COMMAND(ID_INVARIANT_MOMENTS, &CImageToolDoc::OnInvariantMoments)
	ON_COMMAND(ID_ZERNIKE_MOMENTS, &CImageToolDoc::OnZernikeMoments)
	ON_COMMAND(ID_TEMPLATE_MATCHING, &CImageToolDoc::OnTemplateMatching)
	ON_COMMAND(ID_OPENCVCAM, &CImageToolDoc::OnOpencvcam)
END_MESSAGE_MAP()


// CImageToolDoc 생성/소멸

CImageToolDoc::CImageToolDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CImageToolDoc::~CImageToolDoc()
{
}




// CImageToolDoc serialization

void CImageToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CImageToolDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CImageToolDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImageToolDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImageToolDoc 진단

#ifdef _DEBUG
void CImageToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageToolDoc 명령


BOOL CImageToolDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return m_Dib.Save(CT2A(lpszPathName));
}


BOOL CImageToolDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	BOOL res = m_Dib.Load(CT2A(lpszPathName));

	if (res)
		AfxPrintInfo(_T("[파일 열기] 파일 경로: %s, 가로 크기: %dPx, 세로 크기: %dPx, 색상수: %d"), lpszPathName, m_Dib.GetWidth(), m_Dib.GetHeight(), 0x01 << m_Dib.GetBitCount());

	return res;
}
BOOL CImageToolDoc::OnNewDocument() {

	if (!CDocument::OnNewDocument())
		return FALSE;

	BOOL ret = TRUE;
	CFileNewDlg dlg;
		
	if (theApp.m_pNewDib == NULL) 
	{
		if (dlg.DoModal() == IDOK)
		{
			if (dlg.m_nType == 0) // 글일 스케일 비트맵이면
				ret = m_Dib.CreateGrayBitmap(dlg.m_nWidth, dlg.m_nHeight);
			else
				ret = m_Dib.CreateRGBBitmap(dlg.m_nWidth, dlg.m_nHeight);
		}
		else
			ret = FALSE;
	
	}
	else
	{
		m_Dib = *(theApp.m_pNewDib);
		theApp.m_pNewDib = NULL;
	}
	return ret;
}


void CImageToolDoc::OnWindowDuplicate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxNewBitmap(m_Dib);
}


void CImageToolDoc::OnEditCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_Dib.IsValid())
		m_Dib.CopyToClipboard();
}


void CImageToolDoc::OnImageInverse()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//LdhByteImage img;
	//LdhDibToImage(m_Dib, img);
	//LdhInverse(img);
	//
	//LdhDib dib;
	//LdhImageToDib(img, dib);
	if (m_Dib.GetBitCount() == 8) {

		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
		LdhInverse(img);
		CONVERT_IMAGE_TO_DIB(img, dib);

		AfxPrintInfo(_T("[반전] 입력 영상: %s"), GetTitle());
		AfxNewBitmap(dib);
	}
	else// 20241014 color image 반전 
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img);
		LdhInverse(img);
		CONVERT_IMAGE_TO_DIB(img, dib);

		AfxPrintInfo(_T("[반전] 입력 영상: %s"), GetTitle());
		//AfxNewBitmap(dib);		// 새로운 창에서 변경
		UpdateAllViews(NULL);	// 현재 창에서 변경
	}

}


void CImageToolDoc::OnBrightnessContrast()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CBrightnessContrastDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if (m_Dib.GetBitCount() == 8) {

			CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
				LdhBrightness(img, dlg.m_nBrightness);
				LdhContrast(img, dlg.m_nContrast);
			CONVERT_IMAGE_TO_DIB(img, dib);
			AfxNewBitmap(dib);
		}
		else if (m_Dib.GetBitCount() == 24) {

			CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
				LdhBrightness(img, dlg.m_nBrightness);
				LdhContrast(img, dlg.m_nContrast);
			CONVERT_IMAGE_TO_DIB(img, dib);
			AfxNewBitmap(dib);
		}

		AfxPrintInfo(_T("[밝기/명암비 조절] 입력 영상: %s, 밝기: %d, 명암비: %d%%"), GetTitle(), dlg.m_nBrightness, dlg.m_nContrast);
	}

}


void CImageToolDoc::OnGammaCorrection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CGammaCorrectionDlg dlg;
	if (dlg.DoModal() == IDOK)
	{

		if (m_Dib.GetBitCount() == 8) {

			CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
				LdhGammaCorrection(img,dlg.m_fGamma);
			CONVERT_IMAGE_TO_DIB(img, dib);
			AfxNewBitmap(dib);
		}
		else {
			CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
				LdhGammaCorrection(img, dlg.m_fGamma);
			CONVERT_IMAGE_TO_DIB(img, dib);
			AfxNewBitmap(dib);
		}

		AfxPrintInfo(_T("[밝기/명암비 조절] 입력 영상: %s, 감마: 4.2%f"), GetTitle(), dlg.m_fGamma);
	}
}


void CImageToolDoc::OnViewHistogram()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CHistogramDlg dlg;
	dlg.SetImage(&m_Dib);
	dlg.DoModal();
}


void CImageToolDoc::OnHistoStretching()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
			LdhHistogramStretching(img);
		CONVERT_IMAGE_TO_DIB(img, dib)
		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
			LdhHistogramStretching(img);
		CONVERT_IMAGE_TO_DIB(img, dib)
			AfxNewBitmap(dib);
	}
	AfxPrintInfo(_T("[히스토그램 스트레칭] 입력영상: %s"), GetTitle());
	
}


void CImageToolDoc::OnHistoEqualization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_Dib.GetBitCount() == 8)
	{

		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
			LdhHistogramEqualization(img);
		CONVERT_IMAGE_TO_DIB(img, dib);
		AfxNewBitmap(dib);

	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img);
		LdhHistogramEqualization(img);
		CONVERT_IMAGE_TO_DIB(img, dib);
		AfxNewBitmap(dib);
	}

		AfxPrintInfo(_T("[히스토그램 균등화] 입력영상: %s"), GetTitle());

}


void CImageToolDoc::OnArithmeticLogical()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CArithmeticLogicalDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;



		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, img1);
		CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, img2);
		LdhByteImage img3;


		bool ret = false;

		switch (dlg.m_nFunction)
		{
		case 0: ret = LdhAdd(img1, img2, img3); break;
		case 1: ret = LdhSub(img1, img2, img3); break;
		case 2:	ret = LdhAve(img1, img2, img3); break;
		case 3:	ret = LdhDiff(img1, img2, img3); break;
		case 4:	ret = LdhAND(img1, img2, img3); break;
		case 5:	ret = LdhOR(img1, img2, img3); break;
		}

		if (ret)
		{
			CONVERT_IMAGE_TO_DIB(img3, dib);

			TCHAR* op[] = { _T("덧셈"),_T("뺄셈"),_T("평균") ,_T("차이") ,_T("논리 AND") ,_T("논리 OR") };

			AfxPrintInfo(_T("[산술 및 논리 연산] [%s] 입력영상 #1 %s, 입력영상 #2 %s"), op[dlg.m_nFunction], pDoc1->GetTitle(), pDoc2->GetTitle());
			AfxNewBitmap(dib);
		}
		else AfxPrintInfo(_T("영상의 크기가 다릅니다!"));

	}
}


void CImageToolDoc::OnBitplaneSlicing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		LdhByteImage imgPlane;

	// camera7.bmp 의 최하위 비트에는 7이 숨겨져있다.
	for (int i = 0; i < 8; i++)
	{
		LdhBitPlane(img, imgPlane, i);
		CONVERT_IMAGE_TO_DIB(imgPlane, dib)
			AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnFilterMean()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		LdhByteImage imgDst;
		LdhFilterMean(imgSrc, imgDst);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[평균 값 필터] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFilterWeightedMean()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		LdhByteImage imgDst;
	LdhFilterWeightedMean(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[가중 평균 값 필터] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFilterGaussian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGaussianDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
			LdhFloatImage imgDst;
		LdhFilterGaussian(imgSrc, imgDst, dlg.m_fSigma);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[가우시안 필터] 입력 영상: %s, Sigma:%4.2f"), GetTitle(), dlg.m_fSigma);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnFilterLaplacian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		LdhByteImage imgDst;
	LdhFilterLaplacian(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)
	
	AfxPrintInfo(_T("[라플라시안 필터] 입력영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFilterUnsharpMask()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		LdhByteImage imgDst;
	LdhFilterUnsharpMask(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[언샤프 마스크 필터] 입력영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFilterHighboost()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		LdhByteImage imgDst;
	float alpha = 1.2f;
	LdhFilterHighboost(imgSrc, imgDst, alpha);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[하이부스트 필터] 입력영상: %s, alpha = %4.2f"), GetTitle(), alpha);
	AfxNewBitmap(dib);

}


void CImageToolDoc::OnAddNoise()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CAddNoiseDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
			LdhByteImage imgDst;
		if (dlg.m_nNoiseType == 0)
			LdhNoiseGaussian(imgSrc, imgDst, dlg.m_nAmount);
		else
			LdhNoiseSaltNPepper(imgSrc, imgDst, dlg.m_nAmount);

		CONVERT_IMAGE_TO_DIB(imgDst, dib);

		TCHAR* noise[] = { _T("가우시안"), _T("소금&후추") };
		AfxPrintInfo(_T("[잡음추가] 입력영상: %s, 잡음 종류: %s, 잡음양: %d"), GetTitle(), noise[dlg.m_nNoiseType], dlg.m_nAmount);
		AfxNewBitmap(dib);
		
	}
}


void CImageToolDoc::OnFilterMedian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		LdhByteImage imgDst;
	LdhFilterMedian(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[미디언 필터] 입력영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFilterDiffusion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CDiffusionDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
			LdhFloatImage imgDst;
		LdhFIlterDiffusion(imgSrc, imgDst, dlg.m_fLambda, dlg.m_fK, dlg.m_nIteration);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)
			AfxPrintInfo(_T("[비등방성 확산 필터] 입력 영상: %s, Lambda: %4.2f, K: %4.2f, 반복 횟수: %d"), GetTitle(), dlg.m_fLambda, dlg.m_fK, dlg.m_nIteration);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnImageTranslation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CTranslateDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc);
		LdhByteImage imgDst;
		LdhTranslate(imgSrc, imgDst, dlg.m_nNewSX, dlg.m_nNewSY);
		CONVERT_IMAGE_TO_DIB(imgDst, dib);

		AfxPrintInfo(_T("[이동 변환] 입력 영상: %s, 가로 이동: %d, 세로 이동: %d"), GetTitle(), dlg.m_nNewSX, dlg.m_nNewSY);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnImageResize()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CResizeDlg dlg;
	dlg.m_nOldWidth = m_Dib.GetWidth();
	dlg.m_nOldHeight = m_Dib.GetHeight();
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc);
		LdhByteImage imgDst;
		switch (dlg.m_nInterpolation)
		{
		case 0:LdhResizeNearest(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight);
			break;
		case 1: LdhResizeBilinear(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight);
			break;
		case 2:LdhResizeCubic(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight);
			break;
		}
		CONVERT_IMAGE_TO_DIB(imgDst, dib);

		TCHAR* interpolation[] = { _T("최근방 이웃 보간법"),_T("양선형 보간법") ,_T("3차 회선 보간법") };
		AfxPrintInfo(_T("[크기 변환] 입력 영상: %s, , 새 가로 크기: %d, 새 세로 크기: %d, 보간법 : %s "), GetTitle(), dlg.m_nNewWidth, dlg.m_nNewHeight, interpolation[dlg.m_nInterpolation]);
		AfxNewBitmap(dib);

	}
}


void CImageToolDoc::OnImageRotate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CRotateDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc);
		LdhByteImage imgDst;
		
		switch (dlg.m_nRotate)
		{
		case 0: LdhRotate90(imgSrc, imgDst); break;
		case 1: LdhRotate180(imgSrc, imgDst); break;
		case 2: LdhRotate270(imgSrc, imgDst); break;
		case 3: LdhRotate(imgSrc, imgDst, dlg.m_fAngle); break;
		}

		CONVERT_IMAGE_TO_DIB(imgDst, dib);

		TCHAR* rotate[] = { _T("90도"),_T("180도") ,_T("270도")};
		
		if (dlg.m_nRotate != 3)
			AfxPrintInfo(_T("[회전 변환] 입력 영상: %s, 회전 각도: %s"), GetTitle(), rotate[dlg.m_nRotate]);
		else
			AfxPrintInfo(_T("[회전 변환] 입력 영상: %s, 회전 각도: %4.2f"), GetTitle(), dlg.m_fAngle);

		AfxNewBitmap(dib);

	}
}


void CImageToolDoc::OnImageMirror()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc);
	LdhByteImage imgDst;
	LdhMirror(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib);

	AfxPrintInfo(_T("[좌우 대칭] 입력영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnImageFlip()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc);
	LdhByteImage imgDst;
	LdhFlip(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib);

	AfxPrintInfo(_T("[상하 대칭] 입력영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFourierDft()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	if (w * h > 128 * 128)
	{
		CString msg = _T("영상의 크기가 커서 시간이 오래 걸릴 수 있습니다.\n계속 하시겠습니까?");
		if (AfxMessageBox(msg, MB_OKCANCEL) != IDOK)
			return;
	}

	CWaitCursor wait;

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);

	LdhFourier fourier;
	fourier.SetImage(img);

	DWORD t1 = timeGetTime();
	fourier.DFT(1);

#ifdef SHOW_SPECTRUM_PHASE_IMAGE
	LdhByteImage imgSpec;
	fourier.GetSpectrumImage(imgSpec);

	CONVERT_IMAGE_TO_DIB(imgSpec, dibSpec)
		AfxNewBitmap(dibSpec);

	LdhByteImage imgPhase;
	fourier.GetPhaseImage(imgPhase);

	CONVERT_IMAGE_TO_DIB(imgPhase, dibPhase)
		AfxNewBitmap(dibPhase);
#endif

	fourier.DFT(-1);
	DWORD t2 = timeGetTime();

	LdhByteImage img2;
	fourier.GetImage(img2);

	CONVERT_IMAGE_TO_DIB(img2, dib)

		AfxPrintInfo(_T("[푸리에변환/DFT] 입력 영상: %s, 입력 영상 크기: 처리 시간: %dmsec"),
			GetTitle(), t2 - t1);
	AfxNewBitmap(dib);

}


void CImageToolDoc::OnFourierDftrc()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	if (256 * 256 < w * h)
	{
		CString msg = _T("영상의 크기가 커서 시간이 오래 걸릴수 있습니다.\n계속 진행하시겠습니까?");
		if (AfxMessageBox(msg, MB_OKCANCEL) != IDOK)
			return;
	}

	CWaitCursor wait;

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);

	LdhFourier fourier;
	fourier.SetImage(img);

	DWORD t1 = timeGetTime();
	fourier.DFTR(1);

#ifdef SHOW_SPECTRUM_PHASE_IMAGE
	LdhByteImage imgSpec;
	fourier.GetSpectrumImage(imgSpec);

	CONVERT_IMAGE_TO_DIB(imgSpec, dibSpec);
	AfxNewBitmap(dibSpec);

	LdhByteImage imgPhase;
	fourier.GetPhaseImage(imgPhase);

	CONVERT_IMAGE_TO_DIB(imgPhase, dibPhase);
	AfxNewBitmap(dibPhase);

#endif

	fourier.DFTR(-1);
	DWORD t2 = timeGetTime();

	LdhByteImage img2;
	fourier.GetImage(img2);

	CONVERT_IMAGE_TO_DIB(img2, dib);
	AfxPrintInfo(_T("[푸리에변환/DFTRC] 입력 영상: %s, 처리시간: %dmsec"), GetTitle(), t2 - t1);
	AfxNewBitmap(dib);



}


void CImageToolDoc::OnFourierFft()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	if (!IsPowerOf2(w) || !IsPowerOf2(h))
	{
		AfxMessageBox(_T("가로 또는 세로의 크기가 2의 승사가 아닙니다."));
		return;
	}
	CWaitCursor wait;

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);

	LdhFourier fourier;
	fourier.SetImage(img);

	DWORD t1 = timeGetTime();
	fourier.FFT(1);

#ifdef SHOW_SPECTRUM_PHASE_IMAGE
	LdhByteImage imgSpec;
	fourier.GetSpectrumImage(imgSpec);

	CONVERT_IMAGE_TO_DIB(imgSpec, dibSpec);
	AfxNewBitmap(dibSpec);

	LdhByteImage imgPhase;
	fourier.GetPhaseImage(imgPhase);

	CONVERT_IMAGE_TO_DIB(imgPhase, dibPhase);
	AfxNewBitmap(dibPhase);
#endif

	fourier.FFT(-1);
	DWORD t2 = timeGetTime();

	LdhByteImage img2;
	fourier.GetImage(img2);

	CONVERT_IMAGE_TO_DIB(img2, dib);

	AfxPrintInfo(_T("[푸리에변환/FFT] 입력 영상: %s, 입력 영상 크기: %dx%d, 처리 시간: %dmsec"), GetTitle(), w, h, t2 - t1);
	AfxNewBitmap(dib);

}


void CImageToolDoc::OnFreqFiltering()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	if (!IsPowerOf2(w) || !IsPowerOf2(h))
	{
		AfxPrintInfo(_T("가로 똑는 세로의 크기가 2의 승수가 아닙니다."));
		return;
	}

	CFreqFilteringDlg dlg;
	dlg.m_strRange.Format(_T("(0~%d)"), __min(w / 2, h / 2));
	if (dlg.DoModal() == IDOK)
	{
		CWaitCursor wait;
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);

		LdhFourier fourier;
		fourier.SetImage(img);
		fourier.FFT(1);

		if (dlg.m_nFilterType == 0)
		{
			if (dlg.m_nFilterShape == 0)
				fourier.LowPassIdeal(dlg.m_nCutoff);
			else
				fourier.LowPassGaussian	(dlg.m_nCutoff);
		}
		else
		{
			if (dlg.m_nFilterShape == 0)
				fourier.HighPassIdeal(dlg.m_nCutoff);
			else
				fourier.HighPassGaussian(dlg.m_nCutoff);
		}

		fourier.FFT(-1);

		LdhByteImage img2;
		fourier.GetImage(img2);
		CONVERT_IMAGE_TO_DIB(img2, dib);

		TCHAR* type[] = { _T("저역 통과 필터"), _T("고역 통과 필터") };
		TCHAR* shape[] = { _T("이상적(Ideal)"), _T("가우시안(Gaussian") };
		AfxPrintInfo(_T("[주파수 공간 필터링] 입력 영상: %s, 필터 종류: %s, 필터 모양: %s, 차단 주파수: %d"),
			GetTitle(), type[dlg.m_nFilterType], shape[dlg.m_nFilterShape], dlg.m_nCutoff);
		AfxNewBitmap(dib);

	}
}


void CImageToolDoc::OnEdgeRoberts()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	LdhByteImage imgEdge;
	LdhEdgeRoberts(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib);

	AfxPrintInfo(_T("[마스크 기반 엣지 검출/로버츠] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnEdgePrewitt()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	LdhByteImage imgEdge;
	LdhEdgePrewitt(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib);

	AfxPrintInfo(_T("[마스크 기반 엣지 검출/프리윗] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);

}


void CImageToolDoc::OnEdgeSobel()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	LdhByteImage imgEdge;
	LdhEdgeSobel(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib);

	AfxPrintInfo(_T("[마스크 기반 엣지 검출/소벨] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnEdgeCanny()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CCannyEdgeDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
		LdhByteImage imgEdge;
		LdhEdgeCanny(img, imgEdge, dlg.m_fSigma, dlg.m_fLowTh, dlg.m_fHighTh);
		CONVERT_IMAGE_TO_DIB(imgEdge, dib);

		AfxPrintInfo(_T("[캐니 엣지 검출] 입력 영상: %s, sigma: %4.2f, Low Th: %4.2f, High Th: %4.2f"), GetTitle(), dlg.m_fSigma, dlg.m_fLowTh, dlg.m_fHighTh);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnHoughLine()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	LdhByteImage imgEdge;
	LdhEdgeCanny(img, imgEdge, 1.4f, 30.f, 60.f);

	std::vector<LdhLineParam> lines;
	LdhHoughLine(imgEdge, lines);

	if (lines.size() == 0)
	{
		AfxMessageBox(_T("검출된 직선이 없습니다."));
		return;
	}


	std::sort(lines.begin(), lines.end());
	

	// 최대 10개의 직선만 그려줌.
	int cnt = __min(10, lines.size());
	for (int i = 0; i < cnt; i++)
		LdhDrawLine(img, lines[i], 255);

	CONVERT_IMAGE_TO_DIB(img, dib);

	AfxPrintInfo(_T("[허프 선 검출] 입력 영상: %s, 중요 직선: rho = %4.2f, angle = %4.2f, vote = %d"),
		GetTitle(), lines[0].rho, (lines[0].ang * 180 / 3.14f), lines[0].vote);
	AfxNewBitmap(dib);

}


void CImageToolDoc::OnHarrisCorner()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHarrisCornerDlg dlg;
	
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
		std::vector<LdhPoint> corners;
		LdhHarrisCorner(img, corners, dlg.m_nHarrisTh);

		BYTE** ptr = img.GetPixel2D();

		int x, y;
		for (LdhPoint cp : corners)
		{
			x = cp.x;
			y = cp.y;

			ptr[y - 1][x - 1] = ptr[y - 1][x] = ptr[y - 1][x + 1] = 0;
			ptr[y    ][x - 1] = ptr[y    ][x] = ptr[y    ][x + 1] = 0;
			ptr[y + 1][x - 1] = ptr[y + 1][x] = ptr[y + 1][x + 1] = 0;
		}

		CONVERT_IMAGE_TO_DIB(img, dib);

		AfxPrintInfo(_T("[해리스 코너 검출] 입력 영상: %s, Threshold: %d, 검출된 코너 갯수: %d"),
			GetTitle(), dlg.m_nHarrisTh, corners.size());

		AfxNewBitmap(dib);

	}

}


void CImageToolDoc::OnColorGrayscale()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor);
	LdhByteImage imgGray;
	imgGray.Convert(imgColor);
	CONVERT_IMAGE_TO_DIB(imgGray, dib);

	AfxPrintInfo(_T("[그레이스케일 변환] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);

}


void CImageToolDoc::OnUpdateColorGrayscale(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);

}


void CImageToolDoc::OnColorSplitRgb()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor);
	LdhByteImage imgR, imgG, imgB;
	LdhColorSplitRGB(imgColor, imgR, imgG, imgB);
	CONVERT_IMAGE_TO_DIB(imgR, dibR);
	CONVERT_IMAGE_TO_DIB(imgG, dibG);
	CONVERT_IMAGE_TO_DIB(imgB, dibB);
	
	AfxPrintInfo(_T("[색상 평면 나누기/RGB] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dibR);
	AfxNewBitmap(dibG);
	AfxNewBitmap(dibB);
}


void CImageToolDoc::OnUpdateColorSplitRgb(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorSplitHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor);
	LdhByteImage imgH, imgS, imgI;
	LdhColorSplitHSI(imgColor, imgH, imgS, imgI);
	CONVERT_IMAGE_TO_DIB(imgH, dibH);
	CONVERT_IMAGE_TO_DIB(imgS, dibS);
	CONVERT_IMAGE_TO_DIB(imgI, dibI);

	AfxPrintInfo(_T("[색상 평면 나누기/HSI] 입력 영상: %s"), GetTitle());

	AfxNewBitmap(dibH);
	AfxNewBitmap(dibS);
	AfxNewBitmap(dibI);
}


void CImageToolDoc::OnUpdateColorSplitHsi(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorSplitYuv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor);
	LdhByteImage imgY, imgU, imgV;
	LdhColorSplitYUV(imgColor, imgY, imgU, imgV);
	CONVERT_IMAGE_TO_DIB(imgY,dibY);
	CONVERT_IMAGE_TO_DIB(imgU,dibU);
	CONVERT_IMAGE_TO_DIB(imgV,dibV);

	AfxPrintInfo(_T("[색상 평면 나누기/YUV] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dibY);
	AfxNewBitmap(dibU);
	AfxNewBitmap(dibV);
}


void CImageToolDoc::OnUpdateColorSplitSuv(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorCombineRgb()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("RGB 색상 평면 합치기");
	if (dlg.DoModal() == IDOK)
	{
		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;
		CImageToolDoc* pDoc3 = (CImageToolDoc*)dlg.m_pDoc3;

		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, imgR);
		CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, imgG);
		CONVERT_DIB_TO_BYTEIMAGE(pDoc3->m_Dib, imgB);

		LdhRGBImage imgColor;
		if (!LdhColorCombineRGB(imgR, imgG, imgB, imgColor))
		{
			AfxMessageBox(_T("영상의 크기가 다릅니다."));
			return;
		}
		
		CONVERT_IMAGE_TO_DIB(imgColor, dib);
		
		AfxPrintInfo(_T("[색상 평면 합치기/RGB] 입력 영상 : R: %s, G: %s, B: %s"), pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnColorCombineHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("HSI 색상 평면 합치기");
	if (dlg.DoModal() == IDOK)
	{

		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;
		CImageToolDoc* pDoc3 = (CImageToolDoc*)dlg.m_pDoc3;

		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, imgH);
		CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, imgS);
		CONVERT_DIB_TO_BYTEIMAGE(pDoc3->m_Dib, imgI);

		LdhRGBImage imgColor;
		if (!LdhColorCombineHSI(imgH, imgS, imgI, imgColor))
		{
			AfxMessageBox(_T("영상의 크기가 다릅니다."));
			return;
		}

		CONVERT_IMAGE_TO_DIB(imgColor, dib);

		AfxPrintInfo(_T("[색상 평면 합치기/HSI] 입력 영상 : H: %s, S: %s, I: %s"), pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewBitmap(dib);
	}

}


void CImageToolDoc::OnColorCombineYuv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("YUV 색상 평면 합치기");
	if (dlg.DoModal() == IDOK)
	{

		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;
		CImageToolDoc* pDoc3 = (CImageToolDoc*)dlg.m_pDoc3;

		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, imgY);
		CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, imgU);
		CONVERT_DIB_TO_BYTEIMAGE(pDoc3->m_Dib, imgV);

		LdhRGBImage imgColor;
		if (!LdhColorCombineYUV(imgY, imgU, imgV, imgColor))
		{
			AfxMessageBox(_T("영상의 크기가 다릅니다."));
			return;
		}

		CONVERT_IMAGE_TO_DIB(imgColor, dib);

		AfxPrintInfo(_T("[색상 평면 합치기/YUV] 입력 영상 : Y: %s, U: %s, V: %s"), pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnColorEdge()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, img);
	LdhByteImage imgEdge;
	LdhColorEdge(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib);

	AfxPrintInfo(_T("[컬러 엣지 검출] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateColorEdge(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnSegmentBinarization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CBinaizationDlg dlg;

	dlg.SetImage(m_Dib);
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
		LdhByteImage imgRes;
		LdhBinarization(img, imgRes, dlg.m_nThreshold);
		CONVERT_IMAGE_TO_DIB(imgRes, dib);

		AfxPrintInfo(_T("[이진화] 입력 영상: %s, 임계값: %d"), GetTitle(), dlg.m_nThreshold);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnSegmentLabelingDraw()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	LdhIntImage imgLabel;
	std::vector<LdhLabelInfo> labels;
	int label_cnt = LdhLabeling(img, imgLabel, labels);
	// 객체를 감싸는 사각형 그리기
	BYTE** ptr = img.GetPixel2D();
	for (LdhLabelInfo& info : labels)
	{
		for (int j = info.miny; j <= info.maxy; j++)
			ptr[j][info.minx] = ptr[j][info.maxx] = 128;
		for (int i = info.minx; i <= info.maxx; i++)
			ptr[info.miny][i] = ptr[info.maxy][i] = 128;
	}
	CONVERT_IMAGE_TO_DIB(img, dib);

	AfxPrintInfo(_T("[레이블링] 입력 영상: %s, 객체 갯수: %d"), GetTitle(), label_cnt);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnSegmentLabelingCut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	LdhIntImage imgLabel;
	std::vector<LdhLabelInfo> labels;
	int label_cnt = LdhLabeling(img, imgLabel, labels);

	BYTE** ptr = img.GetPixel2D();
	for (LdhLabelInfo& info : labels)
	{
		LdhByteImage imgObj(info.maxx - info.minx + 1, info.maxy - info.miny + 1);
		BYTE** pObj = imgObj.GetPixel2D();

		for (LdhPoint& pt : info.pixels)
			pObj[pt.y - info.miny][pt.x - info.minx] = 255;
		
		CONVERT_IMAGE_TO_DIB(imgObj, dibObj);
		AfxNewBitmap(dibObj);
		
	}
	
	AfxPrintInfo(_T("[레이블링] 입력 영상: %s, 객체 갯수: %d"), GetTitle(), label_cnt);
}


void CImageToolDoc::OnContourTracing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	LdhIntImage imgLabel;
	std::vector<LdhLabelInfo> labels;
	int label_cnt = LdhLabeling(img, imgLabel, labels);

	LdhByteImage imgContour(img.GetWidth(), img.GetHeight());
	BYTE** ptr = imgContour.GetPixel2D();
	for (LdhLabelInfo& info : labels)
	{
		std::vector<LdhPoint>cp;
		LdhContourTracing(img, info.pixels[0].x, info.pixels[0].y, cp);

		for (LdhPoint& pt : cp)
			ptr[pt.y][pt.x] = 255;
	}

	CONVERT_IMAGE_TO_DIB(imgContour, dib);
	AfxPrintInfo(_T("[외곽선 추적] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnMorphologyErosion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	LdhByteImage imgDst;
	LdhMorphologyErosion(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib);
	
	AfxPrintInfo(_T("[이진 모폴로지/침식] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnMorphologyDilation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	LdhByteImage imgDst;
	LdhMorphologyDilation(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib);

	AfxPrintInfo(_T("[이진 모폴로지/팽창] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnMorphologyOpening()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	LdhByteImage imgDst;
	LdhMorphologyOpening(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib);

	AfxPrintInfo(_T("[이진 모폴로지/열기] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnMorphologyClosing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	LdhByteImage imgDst;
	LdhMorphologyClosing(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib);

	AfxPrintInfo(_T("[이진 모폴로지/닫기] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnGraymorphErosion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	LdhByteImage imgDst;
	LdhMorphologyGrayErosion(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib);

	AfxPrintInfo(_T("[그레이스케일 모폴로지/침식] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnGraymorphDilation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	LdhByteImage imgDst;
	LdhMorphologyGrayDilation(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib);

	AfxPrintInfo(_T("[그레이스케일 모폴로지/팽창] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnGraymorphOpening()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	LdhByteImage imgDst;
	LdhMorphologyGrayOpening(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib);

	AfxPrintInfo(_T("[그레이스케일 모폴로지/열기] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnGraymorphClosing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	LdhByteImage imgDst;
	LdhMorphologyGrayClosing(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib);

	AfxPrintInfo(_T("[그레이스케일 모폴로지/닫기] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFourierDescriptor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFourierDescDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
		LdhIntImage imgLabel;
		std::vector<LdhLabelInfo> labels;
		int label_cnt = LdhLabeling(img, imgLabel, labels);

		LdhByteImage imgContour;
		imgContour.CreateImage(img.GetWidth(), img.GetHeight());
		BYTE** ptr = imgContour.GetPixel2D();

		for (LdhLabelInfo& info : labels)
		{
			std::vector<LdhPoint> cp;
			LdhFourierDescriptor(img, info.pixels[0].x, info.pixels[0].y, dlg.m_nPercent, cp);
			for (LdhPoint pt : cp)
			{
				ptr[pt.y][pt.x] = 255;
			}
		}
		CONVERT_IMAGE_TO_DIB(imgContour, dib);
		AfxPrintInfo(_T("[푸리에 기술자] 입력 영상: %s, 객체 개수: %d, 복원 비율: %d%%"), GetTitle(), label_cnt, dlg.m_nPercent);
		AfxNewBitmap(dib);

	}
}


void CImageToolDoc::OnInvariantMoments()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	double m[7] = { 0., };
	LdhInvariantMoments(img, m);

	double mm[7] = { 0., };
	CString strMoments = _T("");
	for (int i = 0; i < 7; i++)
	{
		if (m[i] == 0) continue;
		mm[i] = abs(log(abs(m[i])));
		strMoments.AppendFormat(_T("m[%d] = %6.3lf"), i + 1, mm[i]);
		if (i < 6) strMoments.Append(_T(", "));
	}

	AfxPrintInfo(_T("[불변 모멘트] 입력 영상: %s, 불변 모멘트(log): %s"), GetTitle(), strMoments);
}


void CImageToolDoc::OnZernikeMoments()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);

	AfxPrintInfo(_T("[저니키 모멘트] 입력 영상: %s, 저니키 모멘트(Mag) (n <= 6):"), GetTitle());

	double zr, zi;
	CString strMoments = _T("");
	for (int n = 0; n <= 6; n++)
	{
		strMoments = _T("  ");
		for (int m = (n % 2); m <= n; m += 2)
		{
			LdhZernikeMoments(img, n, m, zr, zi);
			strMoments.AppendFormat(_T("|zm(%d,%d)| = %7.3lf"), n, m, hypot(zr, zi));
			if (m < n - 1)strMoments.Append(_T(", "));
		}
		AfxPrintInfo(strMoments);
	}
}


void CImageToolDoc::OnTemplateMatching()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	TCHAR szFilter[] = _T("Bitmap Files(*BMP)|*.BMP|AllFiles (*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if (dlg.DoModal() != IDOK)
		return;

	CString pathName = dlg.GetPathName();
	LdhDib dibTmpl;
	if (!dibTmpl.Load(CT2A(pathName)))
	{
		AfxMessageBox(_T("파일을 불러오지 못했습니다."));
		return;
	}

	AfxNewBitmap(dibTmpl);

	if (m_Dib.GetWidth() < dibTmpl.GetWidth() || m_Dib.GetHeight() < dibTmpl.GetHeight())
	{
		AfxMessageBox(_T("템플릿 영상의 크기가 입력 영상보다 큽니다."));
		return;
	}

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	CONVERT_DIB_TO_BYTEIMAGE(dibTmpl, imgTmpl);
	LdhIntImage imgMap;
	LdhPoint dp = LdhTemplateMatching(img, imgTmpl, imgMap);

#if 1
	{
		LdhByteImage imgCvt(img.GetWidth(), img.GetHeight());
		BYTE* pCvt = imgCvt.GetPixels();
		int* pMap = imgMap.GetPixels();

		int max_value = 0;
		for (int i = 0; i < img.GetSize(); i++)
		{
			if (pMap[i] > max_value) max_value = pMap[i];
		}

		if (max_value != 0)
		{
			for (int i = 0; i < img.GetSize(); i++)
			{
				pCvt[i] = pMap[i] * 255 / max_value;
			}
		}

		CONVERT_IMAGE_TO_DIB(imgCvt, dibMap);
		AfxNewBitmap(dibMap);

	}
#endif
	{// 입력 영상에 BOX 크기 그리기
		int tw2 = imgTmpl.GetWidth() / 2;
		int th2 = imgTmpl.GetHeight() / 2;

		int minx = dp.x - tw2;
		int maxx = dp.x + tw2;
		int miny = dp.y - th2;
		int maxy = dp.y + th2;

		BYTE** ptr = img.GetPixel2D();

		for (int j = miny; j < maxy; j++)
			ptr[j][minx] = ptr[j][maxx] = 255;
		for (int i = minx; i < maxx; i++)
			ptr[miny][i] = ptr[maxy][i] = 255;
	}
	
	CONVERT_IMAGE_TO_DIB(img, dib);

	AfxPrintInfo(_T("[템플릿 매칭] 입력 영상: %s, 템플릿 영상: %s, 검출좌표: (%d,%d)"), GetTitle(), dlg.GetFileName(), dp.x, dp.y);
	AfxNewBitmap(dib);

}


void CImageToolDoc::OnOpencvcam()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	COpenCvCamDlg dlg;
	dlg.capture = new cv::VideoCapture(0);
	if (!dlg.capture->isOpened())
	{
		AfxMessageBox(_T("웹캠을 열수 없습니다."));
	}
	dlg.DoModal();

}
