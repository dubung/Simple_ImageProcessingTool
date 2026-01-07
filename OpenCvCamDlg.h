#pragma once
//#include "opencv2/opencv.hpp"
#include "LdhImage/LdhImage.h"
#include "LdhImage/LdhDib.h"
// COpenCvCamDlg 대화 상자

class COpenCvCamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COpenCvCamDlg)

public:
	COpenCvCamDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~COpenCvCamDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENCV_CAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_picture;
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	cv::VideoCapture* capture;
	cv::Mat mat_frame;
	CImage cimage_mfc;
	LdhDib dib;
	
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	CWinThread* m_pThread;

	BOOL m_bThreadFlag = FALSE;
	void FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin);
	void DIsplayImage(CDC* pDC, CRect rect, cv::Mat& srcimg);
	afx_msg void OnBnClickedCamStart();
	afx_msg void OnBnClickedCamStop();
	afx_msg void OnBnClickedGrabButton();

	void Mat2DIB(cv::Mat& MatSrc, LdhDib& dibSrc);

	afx_msg void OnBnClickedOk();
	int m_nCamColorType;
	afx_msg void OnBnClickedRadioColor2();
	afx_msg void OnBnClickedRadioColor1();
};

