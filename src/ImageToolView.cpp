
// ImageToolView.cpp: CImageToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageTool.h"
#endif

#include "MainFrm.h"
#include "ImageToolDoc.h"
#include "ImageToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageToolView

IMPLEMENT_DYNCREATE(CImageToolView, CScrollView)

BEGIN_MESSAGE_MAP(CImageToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_VIEW_ZOOM1, &CImageToolView::OnViewZoom1)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM1, &CImageToolView::OnUpdateViewZoom1)
	ON_COMMAND(ID_VIEW_ZOOM2, &CImageToolView::OnViewZoom2)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM2, &CImageToolView::OnUpdateViewZoom2)
	ON_COMMAND(ID_VIEW_ZOOM3, &CImageToolView::OnViewZoom3)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM3, &CImageToolView::OnUpdateViewZoom3)
	ON_COMMAND(ID_VIEW_ZOOM4, &CImageToolView::OnViewZoom4)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM4, &CImageToolView::OnUpdateViewZoom4)
	ON_COMMAND(ID_VIEW_ZOOM0_5, &CImageToolView::OnViewZoom05)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM0_5, &CImageToolView::OnUpdateViewZoom05)
	ON_COMMAND(ID_VIEW_ZOOM0_25, &CImageToolView::OnViewZoom025)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM0_25, &CImageToolView::OnUpdateViewZoom025)
	ON_COMMAND(ID_VIEW_ZOOM0_125, &CImageToolView::OnViewZoom0125)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM0_125, &CImageToolView::OnUpdateViewZoom0125)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CImageToolView 생성/소멸

CImageToolView::CImageToolView() noexcept :m_dZoom(1)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CImageToolView::~CImageToolView()
{
}

BOOL CImageToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CImageToolView 그리기

void CImageToolView::OnDraw(CDC* pDC)
{
	CImageToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//if (pDoc->m_Dib.IsValid())
	//	pDoc->m_Dib.Draw(pDC->m_hDC);

	if (pDoc->m_Dib.IsValid()) {
		int w = pDoc->m_Dib.GetWidth();
		int h = pDoc->m_Dib.GetHeight();
		pDoc->m_Dib.Draw(pDC->m_hDC, 0, 0, w * m_dZoom, h * m_dZoom);
	}

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}

void CImageToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetScrollSizeToFit();

	//CSize sizeTotal;
	//
	//// TODO: 이 뷰의 전체 크기를 계산합니다.
	//CImageToolDoc* pDoc = GetDocument();
	//if (pDoc->m_Dib.IsValid()) {
	//	sizeTotal.cx = pDoc->m_Dib.GetWidth();
	//	sizeTotal.cy = pDoc->m_Dib.GetHeight();
	//}
	//else {
	//	sizeTotal.cx = sizeTotal.cy = 100;
	//}
	//
	//SetScrollSizes(MM_TEXT, sizeTotal);
	//ResizeParentToFit(TRUE);// CScrollView 클래스의 멤버 함수로써 차일드 프레임의 크기를 뷰의 크기에 맞춰 주는 작업을 수행
	//
	/*
		원형
		void ResizeParentToFit(BOOL bShrinkOnly = TRUE);
	*/

}

void CImageToolView::SetScrollSizeToFit(void)
{
	CSize sizeTotal;

	CImageToolDoc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid())
	{
		int w = pDoc->m_Dib.GetWidth();
		int h = pDoc->m_Dib.GetHeight();

		sizeTotal.cx = w * m_dZoom;
		sizeTotal.cy = h * m_dZoom;

	}
	else
	{
		sizeTotal.cx = sizeTotal.cy = 100;
	}

	SetScrollSizes(MM_TEXT, sizeTotal);

	ResizeParentToFit(TRUE);
	// CScrollView 클래스의 멤버 함수로써 차일드 프레임의 크기를 뷰의 크기에 맞춰 주는 작업을 수행
}


// CImageToolView 인쇄


void CImageToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CImageToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CImageToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CImageToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageToolView 진단

#ifdef _DEBUG
void CImageToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageToolDoc* CImageToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageToolDoc)));
	return (CImageToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageToolView 메시지 처리기


BOOL CImageToolView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//return CScrollView::OnEraseBkgnd(pDC); // 상위 클래스인 CScrollView 클래스의 OnEraseBkgnd 
	// 이 함수는 뷰 영역 전체를 흰색으로 칠하는 작업이 수행된다.

	CBrush br;
	// CBrush는 특정영역을 채우는 색상 또는 패턴에 대한 정보를 가지는 브러쉬를 추상화한 클래스 HS_DIAGCROSS 는 대각선 모양 색상은 옅은 회색으로 지정됨
	br.CreateHatchBrush(HS_DIAGCROSS, RGB(200, 200, 200));

	// FillOutsideRect 함수는 CScrolView 클래스의 멤버 함수로 스크롤되는 영역의 바깥 부분을 지정된 브러쉬로 채워준다
	FillOutsideRect(pDC, &br);

	return TRUE;
}


void CImageToolView::OnViewZoom1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_dZoom = 1;
	SetScrollSizeToFit();
	Invalidate(TRUE);
}


void CImageToolView::OnUpdateViewZoom1(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_dZoom == 1);
}


void CImageToolView::OnViewZoom2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_dZoom = 2;
	SetScrollSizeToFit();
	Invalidate(TRUE);
}


void CImageToolView::OnUpdateViewZoom2(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_dZoom == 2);
}


void CImageToolView::OnViewZoom3()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_dZoom = 3;
	SetScrollSizeToFit();
	Invalidate(TRUE);
}


void CImageToolView::OnUpdateViewZoom3(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_dZoom == 3);
}


void CImageToolView::OnViewZoom4()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_dZoom = 4;
	SetScrollSizeToFit();
	Invalidate(TRUE);
}


void CImageToolView::OnUpdateViewZoom4(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_dZoom == 4);
}


void CImageToolView::OnViewZoom05()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_dZoom = 0.5;
	SetScrollSizeToFit();
	Invalidate(TRUE);
}


void CImageToolView::OnUpdateViewZoom05(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_dZoom == 0.5);
}


void CImageToolView::OnViewZoom025()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_dZoom = 0.25;
	SetScrollSizeToFit();
	Invalidate(TRUE);
}


void CImageToolView::OnUpdateViewZoom025(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_dZoom == 0.25);
}


void CImageToolView::OnViewZoom0125()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_dZoom = 0.125;
	SetScrollSizeToFit();
	Invalidate(TRUE);
}


void CImageToolView::OnUpdateViewZoom0125(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_dZoom == 0.2);
}


void CImageToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CPoint pt = point + GetScrollPosition();
	pt.x /= m_dZoom;
	pt.y /= m_dZoom;
	ShowImageInfo(pt);

	CScrollView::OnMouseMove(nFlags, point);
}
void CImageToolView::ShowImageInfo(CPoint point)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd(); // Main Frame 의 주소값을 받아온다 20241010
	CImageToolDoc* pDoc = GetDocument(); // CDOc의 주소값을 받아온다.

	int w = pDoc->m_Dib.GetWidth();
	int h = pDoc->m_Dib.GetHeight();
	int c = pDoc->m_Dib.GetPaletteNums();

	CString strText;

	// 상태바에 마우스 좌표 표시

	if (point.x >= 0 && point.y >= 0 && point.x < w && point.y < h)
	{
		strText.Format(_T("(%d,%d)"), point.x, point.y);
		pFrame->m_wndStatusBar.SetPaneText(0, strText);//20241010 
		/*
			BOOL SetPaneText(int nIndex, LPCTSTR lpszNewText, BOOL bUpdate = TRUE)
			nIndex		: 상태바 구역의 인덱스 0부터 시작함
			lpszNewText : 출력하고자 하는 문자열 포인터
			bUpdate		: 변경 사항을 바로 적용시킬것인가 
		*/
	}

		
	// 상태바에 이미지 정보 표시
	if (c == 0)
	{
		strText.Format(_T("w:%d  h:%d  c:16M"), w, h);
	}
	else
	{
		strText.Format(_T("w:%d  h:%d  c:%d"), w, h, c);
	}

	pFrame->m_wndStatusBar.SetPaneText(1, strText);

}

