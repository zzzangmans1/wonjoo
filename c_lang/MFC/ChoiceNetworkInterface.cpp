// ChoiceNetworkInterface.cpp: 구현 파일
//

#include "pch.h"
#include "NetworkPacketCapture.h"
#include "ChoiceNetworkInterface.h"
#include "afxdialogex.h"
#include <Windows.h>

#define FAIL										-1
// ChoiceNetworkInterface 대화 상자

IMPLEMENT_DYNAMIC(ChoiceNetworkInterface, CDialog)

ChoiceNetworkInterface::ChoiceNetworkInterface(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CHOICENETWORKINTERFACE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
}

ChoiceNetworkInterface::~ChoiceNetworkInterface()
{
}

void ChoiceNetworkInterface::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NETWROKINTERFACE, m_Netwrok_Interface);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
}

BOOL ChoiceNetworkInterface::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(_T("Choice Network Interface"));										// *** 윈도우 이름 변경
	//::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);	// *** 윈도우 켜질 때 항상위 고정

	SetDlgItemText(IDC_STATIC, "CHOICE NETWORK INTERFACE : ");

	m_Progress.SetRange(0, 100);
	m_brush.CreateSolidBrush(RGB(255, 255, 255));
	// *** 리스트 컨트롤의 크기를 얻어온다.
	m_Netwrok_Interface.GetClientRect(&rect);
	// *** 리스트 컨트롤의 칼럼을 추가
	m_Netwrok_Interface.InsertColumn(0, _T("No."), LVCFMT_LEFT, 50);
	m_Netwrok_Interface.InsertColumn(1, _T("Name"), LVCFMT_LEFT, 300);
	m_Netwrok_Interface.InsertColumn(2, _T("Description"), LVCFMT_LEFT, rect.Width() - 350);


	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	CFont font;
	LOGFONT Logfont;

	GetDlgItem(IDC_STATIC)->GetFont()->GetLogFont(&Logfont);
	GetDlgItem(IDC_STATIC)->GetFont()->GetLogFont(&Logfont);
	Logfont.lfHeight = 15;
	font.CreateFontIndirect(&Logfont);

	for (int i = 0; i < 100; i++)
	{
		m_Progress.SetPos(m_Progress.GetPos() + 1);
	}

	GetDlgItem(IDC_STATIC)->SetFont(&font);
	GetDlgItem(IDC_STATIC)->SetFont(&font);

	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		AfxMessageBox(_T("pcap_findalldevs error :"));
		return 0;
	}
	for (choice_dev = alldevs; choice_dev; choice_dev = choice_dev->next)
	{
		strcnt.Format(_T("%d"), i + 1);
		m_Netwrok_Interface.InsertItem(i, strcnt);
		m_Netwrok_Interface.SetItemText(i, 1, (LPCTSTR)choice_dev->name);
		m_Netwrok_Interface.SetItemText(i, 2, (LPCTSTR)choice_dev->description);
		i++;
	}
	/* list control 이 클릭하면 행 맨앞 열만 선택이 된다. 그래서 SetExtendedStyle 함수를 사용하여 전체 열을 선택하게 바꿀 것이다.
	LVS_EX_FULLROWSELECT 만 적용되면 전체 선택되대 열의 선이 보이지 않는다.
	LVS_EX_GRIDLINES를 적용하면 선택 시 열의 선이 보이게 한다.
	*/
	m_Netwrok_Interface.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	return 0;
}
// 툴바 그려주는 함수

BEGIN_MESSAGE_MAP(ChoiceNetworkInterface, CDialog)
	ON_WM_ERASEBKGND()
	ON_NOTIFY(NM_DBLCLK, IDC_NETWROKINTERFACE, &ChoiceNetworkInterface::OnNMDblclkNetwrokinterface)
	ON_NOTIFY(NM_CLICK, IDC_NETWROKINTERFACE, &ChoiceNetworkInterface::OnNMClickNetwrokinterface)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



BOOL ChoiceNetworkInterface::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;											// *** 윈도우 크기 받아올 변수 선언
	GetClientRect(rect);								// *** 윈도우 클라이언트 영역 크기 정보 얻는다.
	pDC->FillSolidRect(rect, RGB(255, 255, 255));		// *** 테두리 색상과 채워진 색상이 동일하게 사격형 그리는 함수
	return TRUE;
}


void ChoiceNetworkInterface::OnNMDblclkNetwrokinterface(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMView = (NM_LISTVIEW*)pNMHDR;
	click_index = pNMView->iItem;
	if (click_index != FAIL)															// *** 클릭한 곳에 값이 있다면
	{
		EndDialog(click_index);															// *** 부모 클래스에게 클릭한 인덱스 넘겨주기
	}
}


void ChoiceNetworkInterface::OnNMClickNetwrokinterface(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (pNMItemActivate->iItem != FAIL)
	{
		CString str;
		str.Format("CHOICE NETWORK INTERFACE : %d", pNMItemActivate->iItem + 1);
		SetDlgItemText(IDC_STATIC, str);
	}
	*pResult = 0;
}


HBRUSH ChoiceNetworkInterface::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	if (pWnd->GetDlgCtrlID() == IDC_STATIC) {
		pDC->SetBkColor(RGB(255, 255, 255));
		return m_brush;
	}
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}
