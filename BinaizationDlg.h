#pragma once
#include "pch.h"

#include "LdhImage/LdhDib.h";

// CBinaizationDlg 대화 상자

class CBinaizationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBinaizationDlg)

public:
	CBinaizationDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CBinaizationDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BINARIZATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_nThreshold;
	CSliderCtrl m_sliderThreshold;

	LdhDib m_DibSrc; // 입력영상의 축소 복사본
	LdhDib m_DibRes; // m_nThreshold 를 이용하여 m_DibSrc를 이진화한 영상
	void SetImage(LdhDib& dib);
	void MakePreviewImage();

	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnEnChangeThresholdEdit();
};
