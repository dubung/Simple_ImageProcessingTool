// CAddNoiseDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageTool.h"
#include "AddNoiseDlg.h"
#include "afxdialogex.h"


// CAddNoiseDlg 대화 상자

IMPLEMENT_DYNAMIC(CAddNoiseDlg, CDialogEx)

CAddNoiseDlg::CAddNoiseDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADD_NOISE, pParent)
	, m_nNoiseType(0)
	, m_nAmount(5)
{

}

CAddNoiseDlg::~CAddNoiseDlg()
{
}

void CAddNoiseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NOISE_AMOUNT, m_nAmount);
	DDV_MinMaxInt(pDX, m_nAmount, 0, 100);
	DDX_Radio(pDX, IDC_NOISE_RADIO1, m_nNoiseType);
}


BEGIN_MESSAGE_MAP(CAddNoiseDlg, CDialogEx)
END_MESSAGE_MAP()


// CAddNoiseDlg 메시지 처리기


BOOL CAddNoiseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_AMOUNT))->SetRange(0, 100);

	// GetDlgItem 함수
	// CWnd* GetDlgItem(int nID) const;
	// GetDlgItem함수는 대화상자에 추가된 컨트롤의 아이디를 인자로 받아 그 컨트롤에 해당하는 
	// 클래스 형변환하여 상용해야한다. 만약 임시 지역 변수를 선언하여 포인터 주소를 받으려면 다음과같이 코드를 작성할수있음
	// CSpinButtonCtr* pSpin = (CSpinButtonCtrl*) GetDlgItem(IDC_SPIN_AMOUNT);
	// 아니면 IDC_SPIN_AMOUNT 를 Control 형 멤버변수를 이용하여 사용해도 무방함


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
