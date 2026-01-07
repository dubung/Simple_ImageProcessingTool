#pragma once


// CBrightnessContrastDlg 대화 상자

class CBrightnessContrastDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBrightnessContrastDlg)

public:
	CBrightnessContrastDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CBrightnessContrastDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BRIGHTNESS_CONTRAST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// // 20241014 밝기 조절용 슬라이드 변수	// // 20241014 밝기 조절용 슬라이드 변수
	CSliderCtrl m_sliderBrightness;
	CSliderCtrl m_sliderContrast;
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeBrightnessEdit();
	afx_msg void OnEnChangeContrastEdit();
	int m_nBrightness;
	int m_nContrast;
};
