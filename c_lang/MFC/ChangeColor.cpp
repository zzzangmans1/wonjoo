// ChangeColor.cpp: 구현 파일
//

#include "pch.h"
#include "NetworkPacketCapture.h"
#include "ChangeColor.h"
#include "afxdialogex.h"


// ChangeColor 대화 상자

IMPLEMENT_DYNAMIC(ChangeColor, CDialogEx)

ChangeColor::ChangeColor(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

ChangeColor::~ChangeColor()
{
}

void ChangeColor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(ChangeColor, CDialogEx)
	ON_BN_CLICKED(IDC_SAVE_BUTTON, &ChangeColor::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &ChangeColor::OnNMDblclkList1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &ChangeColor::OnNMCustomdrawList1)
END_MESSAGE_MAP()


// ChangeColor 메시지 처리기


BOOL ChangeColor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// *** 칼럼 아이템 초기화
	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.DeleteColumn(0);
	m_ListCtrl.DeleteColumn(0);
	m_ListCtrl.DeleteColumn(0);
	m_ListCtrl.DeleteColumn(0);

	m_ListCtrl.InsertColumn(0, "PROTOCOL", LVCFMT_LEFT, 200);
	m_ListCtrl.InsertColumn(1, "RED", LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(1, "GREEN", LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(1, "BLUE", LVCFMT_LEFT, 100);

	SetWindowText("Protocol Color Change");

	ReadConfig();

	m_ListCtrl.InsertItem(0, tcp);
	m_ListCtrl.SetItemText(0, 1, tcpR);
	m_ListCtrl.SetItemText(0, 2, tcpG);
	m_ListCtrl.SetItemText(0, 3, tcpB);

	m_ListCtrl.InsertItem(0, udp);
	m_ListCtrl.SetItemText(0, 1, udpR);
	m_ListCtrl.SetItemText(0, 2, udpG);
	m_ListCtrl.SetItemText(0, 3, udpB);

	m_ListCtrl.InsertItem(0, ssdp);
	m_ListCtrl.SetItemText(0, 1, ssdpR);
	m_ListCtrl.SetItemText(0, 2, ssdpG);
	m_ListCtrl.SetItemText(0, 3, ssdpB);

	m_ListCtrl.InsertItem(0, arp);
	m_ListCtrl.SetItemText(0, 1, arpR);
	m_ListCtrl.SetItemText(0, 2, arpG);
	m_ListCtrl.SetItemText(0, 3, arpB);

	m_ListCtrl.InsertItem(0, dns);
	m_ListCtrl.SetItemText(0, 1, dnsR);
	m_ListCtrl.SetItemText(0, 2, dnsG);
	m_ListCtrl.SetItemText(0, 3, dnsB);

	m_ListCtrl.InsertItem(0, tls);
	m_ListCtrl.SetItemText(0, 1, tlsR);
	m_ListCtrl.SetItemText(0, 2, tlsG);
	m_ListCtrl.SetItemText(0, 3, tlsB);
	 
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER
		| LVS_EX_HEADERDRAGDROP | LVS_EX_TRANSPARENTBKGND | LVS_EX_AUTOSIZECOLUMNS | LVS_EX_GRIDLINES);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// *** 저장 버튼 클릭하였을 때
void ChangeColor::OnBnClickedOk()
{
	CString checkstr, R,G,B;

	GetDlgItemText(IDC_EDIT4, checkstr);
	if (checkstr.MakeUpper() == tcp)
	{
		GetDlgItemText(IDC_EDIT5, R);
		if ((atoi(R) >= 0) && (atoi(R) <= 255))
		{
			tcpR = R;
		}
		else {
			AfxMessageBox("RGB 값이 아닙니다.");
			return;
		}

		GetDlgItemText(IDC_EDIT6, G);
		if ((atoi(G) >= 0) && (atoi(G) <= 255))
		{
			tcpG = G;
		}
		else {
			AfxMessageBox("RGB 값이 아닙니다.");
			return;
		}
		GetDlgItemText(IDC_EDIT7, B);
		if ((atoi(B) >= 0) && (atoi(B) <= 255))
		{
			tcpB = B;
		}
		else {
			AfxMessageBox("RGB 값이 아닙니다.");
			return;
		}
		
	}
	else if (checkstr.MakeUpper() == udp)
	{

		GetDlgItemText(IDC_EDIT5, R);
		if ((atoi(R) >= 0) && (atoi(R) <= 255))
		{
			udpR = R;
		}
		else {
			AfxMessageBox("RGB 값이 아닙니다.");
			return;
		}
		GetDlgItemText(IDC_EDIT6, G);
		if ((atoi(G) >= 0) && (atoi(G) <= 255))
		{
			udpG = G;
		}
		else {
			AfxMessageBox("RGB 값이 아닙니다.");
			return;
		}
		GetDlgItemText(IDC_EDIT7, B);
		if ((atoi(B) >= 0) && (atoi(B) <= 255))
		{
			udpB = B;
		}
		else {
			AfxMessageBox("RGB 값이 아닙니다.");
			return;
		}
	}
	else if (checkstr.MakeUpper() == ssdp)
	{

		GetDlgItemText(IDC_EDIT5, R);
		if ((atoi(R) >= 0) && (atoi(R) <= 255))
		{
			ssdpR = R;
		}
		else {
			AfxMessageBox("RGB 값이 아닙니다.");
			return;
		}
		GetDlgItemText(IDC_EDIT6, G);
		if ((atoi(G) >= 0) && (atoi(G) <= 255))
		{
			ssdpG = G;
		}
		else {
			AfxMessageBox("RGB 값이 아닙니다.");
			return;
		}
		GetDlgItemText(IDC_EDIT7, B);
		if ((atoi(B) >= 0) && (atoi(B) <= 255))
		{
			ssdpB = B;
		}
		else {
			AfxMessageBox("RGB 값이 아닙니다.");
			return;
		}
	}
	else if (checkstr.MakeUpper() == arp)
	{

		GetDlgItemText(IDC_EDIT5, R);
		if ((atoi(R) >= 0) && (atoi(R) <= 255))
		{
			arpR = R;
		}
		else {
			AfxMessageBox("RGB 값이 아닙니다.");
			return;
		}
		GetDlgItemText(IDC_EDIT6, G);
		if ((atoi(G) >= 0) && (atoi(G) <= 255))
		{
			arpG = G;
		}
		else {
			AfxMessageBox("RGB 값이 아닙니다.");
			return;
		}
		GetDlgItemText(IDC_EDIT7, B);
		if ((atoi(B) >= 0) && (atoi(B) <= 255))
		{
			arpB = B;
		}
		else {
			AfxMessageBox("RGB 값이 아닙니다.");
			return;
		}
	}
	else if (checkstr.MakeUpper() == dns)
	{

		GetDlgItemText(IDC_EDIT5, R);
		if ((atoi(R) >= 0) && (atoi(R) <= 255))
		{
			dnsR = R;
		}
		else {
			AfxMessageBox("RGB 값이 아닙니다.");
			return;
		}
		GetDlgItemText(IDC_EDIT6, G);
		if ((atoi(G) >= 0) && (atoi(G) <= 255))
		{
			dnsG = G;
		}
		else {
			AfxMessageBox("RGB 값이 아닙니다.");
			return;
		}
		GetDlgItemText(IDC_EDIT7, B);
		if ((atoi(B) >= 0) && (atoi(B) <= 255))
		{
			dnsB = B;
		}
		else {
			AfxMessageBox("RGB 값이 아닙니다.");
			return;
		}
	}
	else if (checkstr.MakeUpper() == tls)
	{

		GetDlgItemText(IDC_EDIT5, R);
		if ((atoi(R) >= 0) && (atoi(R) <= 255))
		{
			tlsR = R;
		}
		else {
			AfxMessageBox("RGB 값이 아닙니다.");
			return;
		}
		GetDlgItemText(IDC_EDIT6, G);
		if ((atoi(G) >= 0) && (atoi(G) <= 255))
		{
			tlsG = G;
		}
		else {
			AfxMessageBox("RGB 값이 아닙니다.");
			return;
		}
		GetDlgItemText(IDC_EDIT7, B);
		if ((atoi(B) >= 0) && (atoi(B) <= 255))
		{
			tlsB = B;
		}
		else {
			AfxMessageBox("RGB 값이 아닙니다.");
			return;
		}
	}
	else
	{
		AfxMessageBox(checkstr + "는 프로토콜이 아닙니다");
	}

	SaveConfig();
	OnInitDialog();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void ChangeColor::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString idx;
	if (pNMItemActivate->iItem != -1)
	{
		idx.Format("%d", pNMItemActivate->iItem + 1);
		switch (pNMItemActivate->iItem)
		{
		case 0:
			SetDlgItemText(IDC_EDIT4, tls);
			SetDlgItemText(IDC_EDIT5, tlsR);
			SetDlgItemText(IDC_EDIT6, tlsG);
			SetDlgItemText(IDC_EDIT7, tlsB);
			break;
		case 1:
			SetDlgItemText(IDC_EDIT4, dns);
			SetDlgItemText(IDC_EDIT5, dnsR);
			SetDlgItemText(IDC_EDIT6, dnsG);
			SetDlgItemText(IDC_EDIT7, dnsB);
			break;
		case 2:
			SetDlgItemText(IDC_EDIT4, arp);
			SetDlgItemText(IDC_EDIT5, arpR);
			SetDlgItemText(IDC_EDIT6, arpG);
			SetDlgItemText(IDC_EDIT7, arpB);
			break;
		case 3:
			SetDlgItemText(IDC_EDIT4, ssdp);
			SetDlgItemText(IDC_EDIT5, ssdpR);
			SetDlgItemText(IDC_EDIT6, ssdpG);
			SetDlgItemText(IDC_EDIT7, ssdpB);
			break;
		case 4:
			SetDlgItemText(IDC_EDIT4, udp);
			SetDlgItemText(IDC_EDIT5, udpR);
			SetDlgItemText(IDC_EDIT6, udpG);
			SetDlgItemText(IDC_EDIT7, udpB);
			break;
		case 5:
			SetDlgItemText(IDC_EDIT4, tcp);
			SetDlgItemText(IDC_EDIT5, tcpR);
			SetDlgItemText(IDC_EDIT6, tcpG);
			SetDlgItemText(IDC_EDIT7, tcpB);
			break;
		default:
			break;
		}
	}
	*pResult = 0;
}

// *** 컨트롤 리스트 별 색상
void ChangeColor::OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	CString strType;
	BOOL bTCPFlag = FALSE;
	BOOL bUDPFlag = FALSE;
	BOOL bSSDPFlag = FALSE;
	BOOL bARPFlag = FALSE;
	BOOL bDNSFlag = FALSE;
	BOOL bTLSFlag = FALSE;

	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;

	strType = m_ListCtrl.GetItemText(pLVCD->nmcd.dwItemSpec, 0);

	if ((strType.Find(_T("TCP")) != -1))
	{
		bTCPFlag = TRUE;
	}

	if ((strType.Find(_T("UDP")) != -1))
	{
		bUDPFlag = TRUE;
	}

	if ((strType.Find(_T("SSDP")) != -1))
	{
		bSSDPFlag = TRUE;
	}

	if ((strType.Find(_T("ARP")) != -1))
	{
		bARPFlag = TRUE;
	}

	if ((strType.Find(_T("DNS")) != -1))
	{
		bDNSFlag = TRUE;
	}

	if ((strType.Find(_T("TLS")) != -1))
	{
		bTLSFlag = TRUE;
	}
	*pResult = 0;

	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
		*pResult = CDRF_NOTIFYITEMDRAW;

	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		// TCP 라면
		if (bTCPFlag)
		{
			//pLVCD->clrText = RGB(255, 0, 0);  // 글자 색 변경 
			pLVCD->clrTextBk = RGB(atoi(tcpR), atoi(tcpG), atoi(tcpB));  // 배경 색 변경 
		}
		// UDP 라면
		else if (bUDPFlag)
		{
			//pLVCD->clrText = RGB(0, 255, 0);
			pLVCD->clrTextBk = RGB(atoi(udpR), atoi(udpG), atoi(udpB));
		}
		else if (bSSDPFlag)
		{
			pLVCD->clrTextBk = RGB(atoi(ssdpR), atoi(ssdpG), atoi(ssdpB));
		}
		else if (bARPFlag)
		{
			pLVCD->clrTextBk = RGB(atoi(arpR), atoi(arpG), atoi(arpB));
		}
		else if (bDNSFlag)
		{
			pLVCD->clrTextBk = RGB(atoi(dnsR), atoi(dnsG), atoi(dnsB));
		}
		else if (bTLSFlag)
		{
			pLVCD->clrTextBk = RGB(atoi(tlsR), atoi(tlsG), atoi(tlsB));
		}
		else
		{
			pLVCD->clrText = RGB(0, 0, 0);
		}

		*pResult = CDRF_DODEFAULT;
	}
}

void ChangeColor::SaveConfig()
{
	CStdioFile file;

	if (!file.Open("C:\\Users\\lenovo\\Desktop\\config.txt", CStdioFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::shareDenyNone))
	{
		AfxMessageBox("파일 오픈 실패!");
	}
	tmp = tcp + " " + tcpR + " " + tcpG + " " + tcpB+"\n";
	file.WriteString(tmp);
	tmp = udp + " " + udpR + " " + udpG + " " + udpB + "\n";
	file.WriteString(tmp);
	tmp = ssdp + " " + ssdpR + " " + ssdpG + " " + ssdpB + "\n";
	file.WriteString(tmp);
	tmp = arp + " " + arpR + " " + arpG + " " + arpB + "\n";
	file.WriteString(tmp);
	tmp = dns + " " + dnsR + " " + dnsG + " " + dnsB + "\n";
	file.WriteString(tmp);
	tmp = tls + " " + tlsR + " " + tlsG + " " + tlsB + "\n";
	file.WriteString(tmp);

	file.Close();
}

void ChangeColor::ReadConfig()
{
	CStdioFile file;

	if (!file.Open("C:\\Users\\lenovo\\Desktop\\config.txt", CStdioFile::modeCreate | CFile::modeRead | CFile::modeNoTruncate | CFile::shareDenyNone))
	{
		AfxMessageBox("파일 오픈 실패!");
	}
	file.ReadString(tmp);
	AfxExtractSubString(tcp, tmp, 0, ' ');
	AfxExtractSubString(tcpR, tmp, 1, ' ');
	AfxExtractSubString(tcpG, tmp, 2, ' ');
	AfxExtractSubString(tcpB, tmp, 3, ' ');

	file.ReadString(tmp);
	AfxExtractSubString(udp, tmp, 0, ' ');
	AfxExtractSubString(udpR, tmp, 1, ' ');
	AfxExtractSubString(udpG, tmp, 2, ' ');
	AfxExtractSubString(udpB, tmp, 3, ' ');

	file.ReadString(tmp);
	AfxExtractSubString(ssdp, tmp, 0, ' ');
	AfxExtractSubString(ssdpR, tmp, 1, ' ');
	AfxExtractSubString(ssdpG, tmp, 2, ' ');
	AfxExtractSubString(ssdpB, tmp, 3, ' ');

	file.ReadString(tmp);
	AfxExtractSubString(arp, tmp, 0, ' ');
	AfxExtractSubString(arpR, tmp, 1, ' ');
	AfxExtractSubString(arpG, tmp, 2, ' ');
	AfxExtractSubString(arpB, tmp, 3, ' ');

	file.ReadString(tmp);
	AfxExtractSubString(dns, tmp, 0, ' ');
	AfxExtractSubString(dnsR, tmp, 1, ' ');
	AfxExtractSubString(dnsG, tmp, 2, ' ');
	AfxExtractSubString(dnsB, tmp, 3, ' ');

	file.ReadString(tmp);
	AfxExtractSubString(tls, tmp, 0, ' ');
	AfxExtractSubString(tlsR, tmp, 1, ' ');
	AfxExtractSubString(tlsG, tmp, 2, ' ');
	AfxExtractSubString(tlsB, tmp, 3, ' ');
	file.Close();
}
