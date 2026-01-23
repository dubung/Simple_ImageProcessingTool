// OpenCvCamDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageTool.h"
#include "OpenCvCamDlg.h"
#include "afxdialogex.h"
#include "LdhImage/LdhImage.h"
#include "LdhImage/LdhConvert.h"


// COpenCvCamDlg 대화 상자

IMPLEMENT_DYNAMIC(COpenCvCamDlg, CDialogEx)

COpenCvCamDlg::COpenCvCamDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPENCV_CAM, pParent)
	, m_nCamColorType(0)
{

}

COpenCvCamDlg::~COpenCvCamDlg()
{
	//mat_frame = NULL;
}

void COpenCvCamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE_DISPLAY, m_picture);
	DDX_Radio(pDX, IDC_RADIO_COLOR1, m_nCamColorType);
}


BEGIN_MESSAGE_MAP(COpenCvCamDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CAM_START, &COpenCvCamDlg::OnBnClickedCamStart)
	ON_BN_CLICKED(IDC_CAM_STOP, &COpenCvCamDlg::OnBnClickedCamStop)
	ON_BN_CLICKED(IDC_GRAB_BUTTON, &COpenCvCamDlg::OnBnClickedGrabButton)
	ON_BN_CLICKED(IDOK, &COpenCvCamDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_COLOR2, &COpenCvCamDlg::OnBnClickedRadioColor2)
	ON_BN_CLICKED(IDC_RADIO_COLOR1, &COpenCvCamDlg::OnBnClickedRadioColor1)
END_MESSAGE_MAP()


// COpenCvCamDlg 메시지 처리기


void COpenCvCamDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void COpenCvCamDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//capture->read(mat_frame);
	

	// 화면에 보여주기 위한

	//int bpp = 8 * mat_frame.elemSize();
	//assert((bpp == 8 || bpp == 24 || bpp == 32));

	//int padding = 0;

	//if (bpp < 32)
	//	padding =4 - (mat_frame.cols % 4);

	//if (padding == 4)
	//	padding = 0;

	//int border = 0;

	//if (bpp < 32)
	//	border = 4 - (mat_frame.cols % 4);

	//cv::Mat mat_temp;
	//if (border > 0 || mat_frame.isContinuous() == false)
	//{
	//	cv::copyMakeBorder(mat_frame, mat_temp, 0, 0, 0, border, cv::BORDER_CONSTANT, 0);
	//}
	//else
	//	mat_temp = mat_frame;

	//RECT r;
	//m_picture.GetClientRect(&r);
	//cv::Size winSize(r.right, r.bottom);

	//cv::imshow("TEST",mat_frame);
	//

	CDialogEx::OnTimer(nIDEvent);
}


BOOL COpenCvCamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//capture->open(0); // 카메라 연결
	//capture->open(0); // 카메라 연결
	//if (!capture->isOpened())
		//AfxMessageBox(_T("카메라 연결에 실패 하였습니다."));


	CRect rect;
	CDC* pDC = m_picture.GetDC();
	m_picture.GetClientRect(rect);

	//capture->set(cv::CAP_PROP_FRAME_WIDTH, rect.Width());
	//capture->set(cv::CAP_PROP_FRAME_HEIGHT, rect.Height());



	GetDlgItem(IDC_GRAB_BUTTON)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void COpenCvCamDlg::OnPaint()
{

	//if (mat_frame.data != NULL)
	//{
	//	CRect rect;
	//	CDC* pDC = m_picture.GetDC();
	//	m_picture.GetClientRect(rect);
	//	DIsplayImage(pDC, rect, mat_frame);
	//	ReleaseDC(pDC);
	//}
	CDialogEx::OnPaint();
}

void COpenCvCamDlg::FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)
{
	assert(bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);
	memset(bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height): -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;
	if (bpp == 8) {
		RGBQUAD* palette = bmi->bmiColors;
		for (int i = 0; i < 256; i++) {
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}

}
//
//void COpenCvCamDlg::DIsplayImage(CDC* pDC, CRect rect, cv::Mat& srcimg)
//{
//	cv::Mat img;
//	int step = ((int)(rect.Width() / 4)) * 4; 
//
//	if ((srcimg.empty()))
//		return;
//
//	resize(srcimg, img, cv::Size(step, rect.Height()));
//	uchar buffer[sizeof(BITMAPINFOHEADER) * 1024];
//
//	BITMAPINFO* bmi = (BITMAPINFO*)buffer;
//
//	int bmp_w = img.cols;
//	int bmp_h = img.rows;
//	int depth = img.depth();
//	int channels = img.channels();
//	int bpp = 8 * channels;
//
//	FillBitmapInfo(bmi, bmp_w, bmp_h, bpp, 0);
//
//	int from_x = MIN(0, bmp_w - 1);
//	int from_y = MIN(0, bmp_h - 1);
//	int sw = MAX(MIN(bmp_w - from_x, rect.Width()), 0);
//	int sh = MAX(MIN(bmp_h - from_y, rect.Height()), 0);
//
//
//	SetDIBitsToDevice(pDC->m_hDC, rect.left, rect.top, sw, sh, from_x, from_y, 0, sh, img.data + from_y * img.step, bmi, 0);
//	img.release();
//
//}
//

UINT ThreadImageCaptureFunc(LPVOID param)
{
	COpenCvCamDlg* pDlg = (COpenCvCamDlg*)param;
	
	while (pDlg->m_bThreadFlag) {
		//pDlg->capture->read(pDlg->mat_frame);
		if (pDlg->m_nCamColorType == 1);
			//cv::cvtColor(pDlg->mat_frame, pDlg->mat_frame, cv::COLOR_RGB2GRAY);


		pDlg->Invalidate(FALSE);
		Sleep(10);
	}
	return 0;
}



void COpenCvCamDlg::OnBnClickedCamStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pThread == NULL)
	{

		m_pThread = ::AfxBeginThread(ThreadImageCaptureFunc, this);
		if (m_pThread != NULL)
		{
			m_pThread->m_bAutoDelete = FALSE;
			m_bThreadFlag = TRUE;
		}
		
	}
	else if (m_bThreadFlag == FALSE)
	{
		m_pThread->ResumeThread();
		m_bThreadFlag = TRUE;
	}

}


void COpenCvCamDlg::OnBnClickedCamStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pThread != NULL)
	{
		m_pThread->SuspendThread();
		m_bThreadFlag = FALSE;
	}
}


void COpenCvCamDlg::OnBnClickedGrabButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//Mat2DIB(mat_frame, dib);
	AfxNewBitmap(dib);
}
//
//void COpenCvCamDlg::Mat2DIB(cv::Mat& MatSrc, LdhDib& dibSrc)
//{
//	int bpp = MatSrc.channels() * 8;
//	cv::Mat tmp;
//
//	if ( bpp == 24)
//	{
//		dibSrc.CreateRGBBitmap(tmp.cols, tmp.rows);
//		LdhRGBImage img;
//		LdhDibToImage(dibSrc, img);
//		RGBBYTE* tmpArr = img.GetPixels();
//		std::memcpy(tmpArr, tmp.data, tmp.cols * tmp.rows * sizeof(RGBBYTE));
//		LdhImageToDib(img, dibSrc);
//		
//	}
//	if (bpp == 8)
//	{
//
//		dibSrc.CreateGrayBitmap(tmp.cols, tmp.rows);
//		LdhByteImage img;
//		LdhDibToImage(dibSrc, img);
//
//		BYTE* tempArr = img.GetPixels();
//		std::memcpy(tempArr, tmp.data, tmp.cols * tmp.rows * sizeof(BYTE));
//		LdhImageToDib(img, dibSrc);
//
//	}
//
//
//	
//	
//}


void COpenCvCamDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedCamStop();
	if (m_pThread != NULL) {
		m_pThread->SuspendThread();
		DWORD dwResult;
		GetExitCodeThread(m_pThread->m_hThread, &dwResult);

		delete m_pThread;
		m_pThread = NULL;
		m_bThreadFlag = FALSE;
	}
	//capture->release();
	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}


void COpenCvCamDlg::OnBnClickedRadioColor2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nCamColorType = 1;
}


void COpenCvCamDlg::OnBnClickedRadioColor1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nCamColorType = 0;
}
