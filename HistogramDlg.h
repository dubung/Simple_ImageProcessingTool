#pragma once


// CHistogramDlg 대화 상자

class CHistogramDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramDlg)

public:
	CHistogramDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CHistogramDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTOGRAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	int m_Histogram[3][256];// [0]: grayscale Or Blue [1]: Green [2]: Red // 20241018 Enum을 이용해서 0 1 2 를 나눌까 고민중
	void SetImage(LdhDib* pDib);
	int m_nImgBitCount;		// grayscale 이냐 true color이냐
	afx_msg void OnPaint();
};
