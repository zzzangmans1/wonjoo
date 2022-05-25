
// NetworkPacketCaptureDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "NetworkPacketCapture.h"
#include "NetworkPacketCaptureDlg.h"
#include "afxdialogex.h"


#define FAIL					-1

#define SWAP16(s)				(((((s) & 0xff) << 8) | (((s) >> 8) & 0xff)))
#define FLAGSUM(x, y, z)		(x << 3) | (y << 2) | (z << 1)

#define CStringToHex(x,y,z)		strtol(x.Mid(y, z), NULL, 16)	// *** CString to Hex
#define IsAlpha(x)				isalpha(x) ? x : '.'
#define CheckAscii(x)			(x > 0x2C && x < 0x123) ? x : '.'

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNetworkPacketCaptureDlg 대화 상자
CNetworkPacketCaptureDlg::CNetworkPacketCaptureDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NETWORKPACKETCAPTURE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

// DoDataExchange() - 다이얼로그 기반으로 여러가지 윈도우 컨트롤러를 손쉽게 만들 수 있다.
// 다이얼로그 컨트롤을 변수로 사용하려고 싶다면 여기에 작성해야 한다.
void CNetworkPacketCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_NetworkInterfaceControlList);
	DDX_Control(pDX, IDC_PACKET_INFO, m_PacketInfoTree);
	DDX_Control(pDX, IDC_STOP_BUTTON, m_PauseButton);
	DDX_Control(pDX, IDC_LIST2, m_PacketDataControlList);
}

BEGIN_MESSAGE_MAP(CNetworkPacketCaptureDlg, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, OnCustomdrawList)				// *** 컨트롤 리스트 행에 색칠할 함수
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CNetworkPacketCaptureDlg::OnNMDblclkList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CNetworkPacketCaptureDlg::OnLvnItemchangedList1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_PACKET_INFO, &CNetworkPacketCaptureDlg::OnTvnSelchangedPacketInfo)
	ON_BN_CLICKED(IDC_START_BUTTON, &CNetworkPacketCaptureDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP_BUTTON, &CNetworkPacketCaptureDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_CLEAR_BUTTON, &CNetworkPacketCaptureDlg::OnBnClickedClear)
	ON_COMMAND(ID_TSTART_BUTTON, &CNetworkPacketCaptureDlg::OnTbStartClickedWindows)
	ON_COMMAND(ID_TSTOP_BUTTON, &CNetworkPacketCaptureDlg::OnTbStopClickedWindows)
	ON_COMMAND(ID_TCLEAR_BUTTON, & CNetworkPacketCaptureDlg::OnTbClearClickedWindows)
	ON_COMMAND(ID_GITHUB_BUTTON, &CNetworkPacketCaptureDlg::Onsourcebutton)
	ON_COMMAND(ID_LOG_BUTTON, &CNetworkPacketCaptureDlg::OnLogButton)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CNetworkPacketCaptureDlg::OnHdnItemclickList1)					// ** 리스트 헤더 클릭해 SORT 처리
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CNetworkPacketCaptureDlg 메시지 처리기

BOOL CNetworkPacketCaptureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitToolBar();					// *** 툴바 생성 
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	ShowWindow(SW_SHOWMINIMIZED);														// *** 윈도우 켜질 때 항상 최대화로 켜지게
	setlocale(LC_ALL, "Korean");
	SetWindowText(_T("Packet Capture"));												// *** 윈도우 이름 변경

	//ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);									// *** 작업 표시줄 가리기

	ChoiceNetworkInterface ChoiceNet;													// *** 자식 다이얼로그 
	click_index = ChoiceNet.DoModal();
	ShowWindow(SW_SHOWMAXIMIZED);														// *** 윈도우 켜질 때 항상 최대화로 켜지게
	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.
	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	
	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	// *** 로그 서버 실행
	StartupInfo.lpTitle = "Server";			// *** 프로세스 이름
	StartupInfo.cb = sizeof(STARTUPINFO);

	CreateProcess("C:\\Users\\lenovo\\source\\repos\\SocketServer\\bin\\Debug\\netcoreapp3.1\\SocketServer.exe",
		NULL, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo);
	if (!ProcessInfo.hProcess)
	{
		AfxMessageBox("Socket Server Process Create Failed");
	}

	// *** 리스트 컨트롤의 크기를 얻어온다.
	m_NetworkInterfaceControlList.GetClientRect(&rect);
	m_NetworkInterfaceControlList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);

	// *** 리스트 컨트롤의 칼럼을 추가
	m_NetworkInterfaceControlList.InsertColumn(0, _T("No."), LVCFMT_LEFT, 100);
	m_NetworkInterfaceControlList.InsertColumn(1, _T("Time"), LVCFMT_LEFT, 200);
	m_NetworkInterfaceControlList.InsertColumn(2, _T("Source"), LVCFMT_LEFT, 400);
	m_NetworkInterfaceControlList.InsertColumn(3, _T("Destination"), LVCFMT_LEFT, 400);
	m_NetworkInterfaceControlList.InsertColumn(4, _T("Protocol"), LVCFMT_LEFT, 200);
	m_NetworkInterfaceControlList.InsertColumn(5, _T("Length"), LVCFMT_LEFT, 100);
	m_NetworkInterfaceControlList.InsertColumn(6, _T("Info"), LVCFMT_LEFT, rect.Width() - 1400);
	m_NetworkInterfaceControlList.InsertColumn(7, _T("Save Data"), LVCFMT_LEFT, 0);
	

	m_PacketDataControlList.InsertColumn(0, "Num", LVCFMT_LEFT, 0);
	m_PacketDataControlList.InsertColumn(1, "Data Name", LVCFMT_LEFT, 300);
	m_PacketDataControlList.InsertColumn(2, "Data Hex", LVCFMT_LEFT, 500);
	m_PacketDataControlList.InsertColumn(3, "Data Char", LVCFMT_LEFT, rect.Width()-800);
	/*
		SetExtendedStyle() - Control List 스타일 설정 함수
		LVS_EX_FULLROWSELECT : 한행 전체 선택
		LVS_EX_DOUBLEBUFFER : 깜빡임을 최소화 하기 위하여 이중버퍼링 사용한다.
		LVS_EX_HEADERDRAGDROP : 헤더를 드래그 해서 순서를 바꿀 수 있게 해준다.

	*/
	m_NetworkInterfaceControlList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER 
												| LVS_EX_HEADERDRAGDROP | LVS_EX_TRANSPARENTBKGND | LVS_EX_AUTOSIZECOLUMNS | LVS_EX_GRIDLINES);
	m_PacketDataControlList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER 
		| LVS_EX_HEADERDRAGDROP | LVS_EX_TRANSPARENTBKGND | LVS_EX_AUTOSIZECOLUMNS | LVS_EX_GRIDLINES);
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}	
// 툴바 그려주는 함수
void CNetworkPacketCaptureDlg::InitToolBar()
{
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_GRIPPER | CBRS_BORDER_ANY)
		|| !m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		EndDialog(IDCANCEL);
	}

	CRect rcClientStart;
	CRect rcClientNow;
	GetClientRect(rcClientStart);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0, reposQuery, rcClientNow);

	CPoint ptOffset(rcClientNow.left - rcClientStart.left, rcClientNow.top - rcClientStart.top);
	CRect rcChild;
	CWnd* pwndChild = GetWindow(GW_CHILD);
	while (pwndChild)
	{
		pwndChild->GetWindowRect(rcChild);
		ScreenToClient(rcChild);
		rcChild.OffsetRect(ptOffset);
		pwndChild->MoveWindow(rcChild, FALSE);
		pwndChild = pwndChild->GetNextWindow();
	}
	CRect rcWindow;
	GetWindowRect(rcWindow);
	rcWindow.right += rcClientStart.Width() - rcClientNow.Width();
	rcWindow.bottom += rcClientStart.Height() - rcClientNow.Height();
	MoveWindow(rcWindow, FALSE);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
}

void CNetworkPacketCaptureDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	// *** 서버 프로그램 스스로 꺼지지 않아서 프로그램 종료 시킬 때 같이 종료시킨다.
	// *** 종료 버튼을 눌렀다면
	else if (nID == SC_CLOSE)
	{
		// *** YES 버튼을 눌렀다면
		if (MessageBox("프로그램을 종료하시겠습니까?", "EXIT", MB_YESNO) == IDYES)
		{
			TerminateProcess(ProcessInfo.hProcess,0);								// *** 서버 생성한 프로세스 종료
			DWORD dwResult;
			::GetExitCodeThread(m_PCThread, &dwResult);
			PostQuitMessage(0);
		}
		// *** NO 버튼을 눌렀다면
		else
		{

		}
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CNetworkPacketCaptureDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CNetworkPacketCaptureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 다이얼로그 백그라운드 색상
BOOL CNetworkPacketCaptureDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;											// *** 윈도우 크기 받아올 변수 선언
	GetClientRect(rect);								// *** 윈도우 클라이언트 영역 크기 정보 얻는다.
	pDC->FillSolidRect(rect, RGB(255,255, 255));		// *** 테두리 색상과 채워진 색상이 동일하게 사격형 그리는 함수
	return TRUE;
}


void CNetworkPacketCaptureDlg::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	// 선택한 열이 있다면
	if(pNMItemActivate->iItem != FAIL)
	{
		CString FrameCnt = m_NetworkInterfaceControlList.GetItemText(pNMItemActivate->iItem, 0);
		CString Time = m_NetworkInterfaceControlList.GetItemText(pNMItemActivate->iItem, 1);
		CString Protocol = m_NetworkInterfaceControlList.GetItemText(pNMItemActivate->iItem, 4);
		CString Length = m_NetworkInterfaceControlList.GetItemText(pNMItemActivate->iItem, 5);
		CString SaveData = m_NetworkInterfaceControlList.GetItemText(pNMItemActivate->iItem, 7);

		if (ClickPacketFrameNumber != atoi(FrameCnt))
		{
			m_PacketInfoTree.DeleteAllItems();
			m_PacketDataControlList.DeleteAllItems();
			SetPacketInfoTree(FrameCnt, Time, Protocol, Length, SaveData);
			SetPacketHexList(SaveData, Protocol, CStringToHex(SaveData, 76, 4));
			ClickPacketFrameNumber = atoi(FrameCnt);
		}
	}
	
}

void CNetworkPacketCaptureDlg::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

// *** 쓰레드 동작 함수
void Packet_Handler(u_char* param, const pcap_pkthdr* header, const u_char* data)
{
	CNetworkPacketCaptureDlg* pDlg = (CNetworkPacketCaptureDlg*)AfxGetApp()->m_pMainWnd;
	
	if (pDlg->m_eThreadWork == CNetworkPacketCaptureDlg::ThreadWorkingType::THREAD_STOP)
	{
		pcap_breakloop(pDlg->m_NetworkDeviceHandler);
		pDlg->m_NetworkInterfaceControlList.DeleteAllItems();
		pDlg->m_PacketInfoTree.DeleteAllItems();
		pDlg->m_PacketDataControlList.DeleteAllItems();
		return;
	}
	pDlg->is_RunThreadOut = FALSE;

	pDlg->m_EthernetHeader = (ETHERNET_HEADER*)data;
	pDlg->m_IpHeader = (IP_HEADER*)(data +14);
	pDlg->m_IpHeaderLen = (pDlg->m_IpHeader->header_len & 0xF) * 4;

	// *** u_char* 데이터를 CString으로 변환
	std::string packet_dump_data_string;
	for (unsigned int i = 1; i < (header->caplen + 1); i++) {
		char* temp = NULL;

		int temp2 = data[i - 1];
		std::stringstream stream;
		stream << std::setw(2) << std::setfill('0') << std::hex << temp2;

		packet_dump_data_string += stream.str();
	}
	CString SaveData(packet_dump_data_string.c_str());

	// 타임 받아올 함수
	CTime tmCur = CTime::GetCurrentTime();
	pDlg->m_CurrentTime.Format("%d %d %d %d:%d:%d", tmCur.GetYear(), tmCur.GetMonth(), tmCur.GetDay(), tmCur.GetHour(), tmCur.GetMinute(), tmCur.GetSecond());
	pDlg->m_SourceIp = inet_ntoa(pDlg->m_IpHeader->src_ip); 
	pDlg->m_DestinationIp = inet_ntoa(pDlg->m_IpHeader->dst_ip);
	pDlg->m_PacketLength.Format("%d", header->caplen);
	
	if (pDlg->m_SourceIp.IsEmpty() || pDlg->m_DestinationIp.IsEmpty() || pDlg->m_PacketLength.IsEmpty()) {
		pcap_breakloop(pDlg->m_NetworkDeviceHandler);
		pDlg->m_NetworkInterfaceControlList.DeleteAllItems();
		pDlg->m_PacketInfoTree.DeleteAllItems();
		pDlg->m_PacketDataControlList.DeleteAllItems();
		return;
	}

	if (ntohs(pDlg->m_EthernetHeader->type)== 0x806)
	{
		pDlg->m_Protocol = "ARP";
		pDlg->m_ARPHeader = (ARP_HEADER*)(data + 14);
		pDlg->m_SourceIp.Format("%02X:%02X:%02X:%02X:%02X:%02X", pDlg->m_ARPHeader->sendmac.e_host[0], pDlg->m_ARPHeader->sendmac.e_host[1],
			pDlg->m_ARPHeader->sendmac.e_host[2], pDlg->m_ARPHeader->sendmac.e_host[3], pDlg->m_ARPHeader->sendmac.e_host[4], pDlg->m_ARPHeader->sendmac.e_host[5]);

		pDlg->m_DestinationIp.Format("%02X:%02X:%02X:%02X:%02X:%02X", pDlg->m_ARPHeader->targetmac.e_host[0], pDlg->m_ARPHeader->targetmac.e_host[1], pDlg->m_ARPHeader->targetmac.e_host[2],
			pDlg->m_ARPHeader->targetmac.e_host[3], pDlg->m_ARPHeader->targetmac.e_host[4], pDlg->m_ARPHeader->targetmac.e_host[5]);

		if (ntohs(pDlg->m_ARPHeader->opcode) == 1) pDlg->m_ARPPacketInfo.Format("who has %s ? Tell %d.%d.%d.%d",inet_ntoa(pDlg->m_ARPHeader->targetip), pDlg->m_ARPHeader->sendip[0], pDlg->m_ARPHeader->sendip[1], pDlg->m_ARPHeader->sendip[2], pDlg->m_ARPHeader->sendip[3]);
		else pDlg->m_ARPPacketInfo.Format("%d.%d.%d.%d is at %02X:%02X:%02X:%02X:%02X:%02X", pDlg->m_ARPHeader->sendip[0], pDlg->m_ARPHeader->sendip[1], pDlg->m_ARPHeader->sendip[2], pDlg->m_ARPHeader->sendip[3], pDlg->m_ARPHeader->sendmac.e_host[0], pDlg->m_ARPHeader->sendmac.e_host[1],
								pDlg->m_ARPHeader->sendmac.e_host[2], pDlg->m_ARPHeader->sendmac.e_host[3], pDlg->m_ARPHeader->sendmac.e_host[4], pDlg->m_ARPHeader->sendmac.e_host[5]);


		size_t ListControlCnt = pDlg->m_NetworkInterfaceControlList.GetItemCount();
		CString ListControlCntStr;
		ListControlCntStr.Format("%d", ListControlCnt + 1);
		pDlg->m_NetworkInterfaceControlList.InsertItem(ListControlCnt, ListControlCntStr, 0);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 1, pDlg->m_CurrentTime);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 2, pDlg->m_SourceIp);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 3, pDlg->m_DestinationIp);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 4, pDlg->m_Protocol);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 5, pDlg->m_PacketLength);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 6, pDlg->m_ARPPacketInfo);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 7, SaveData);
	}
	else if (pDlg->m_IpHeader->protocol == IPPROTO_TCP)
	{
		int flag_check = 0, spcCnt = 0;
		CString Flag = "";
		CString Window = "";

		pDlg->m_Protocol = "TCP";
		pDlg->m_TCPHeader = (TCP_HEADER*) ((u_char*)pDlg->m_IpHeader + pDlg->m_IpHeaderLen);

		pDlg->m_TCPHeader->flag_urg > 0 ? flag_check++, Flag += "URG " : 0;
		pDlg->m_TCPHeader->flag_syn > 0 ? flag_check++, Flag += "SYN " : 0;
		pDlg->m_TCPHeader->flag_rst > 0 ? flag_check++, Flag += "RST " : 0;
		pDlg->m_TCPHeader->flag_psh > 0 ? flag_check++, Flag += "PSH " : 0;
		pDlg->m_TCPHeader->flag_fin > 0 ? flag_check++, Flag += "FIN " : 0;
		pDlg->m_TCPHeader->flag_ece > 0 ? flag_check++, Flag += "ECE " : 0;
		pDlg->m_TCPHeader->flag_cwr > 0 ? flag_check++, Flag += "CWR " : 0;
		pDlg->m_TCPHeader->flag_ack > 0 ? flag_check++, Flag += "ACK " : 0;
		
		Flag.Insert(0, "[");
		if (flag_check == 2) {
			Flag.Insert(4, ",");
			Flag.Insert(9, "]");
		}
		else Flag.Insert(4, "]");
		
		pDlg->m_TCPPacketInfo.Format("%d -> %d", ntohs(pDlg->m_TCPHeader->src_port), ntohs(pDlg->m_TCPHeader->dst_port));
		Window.Format("Win=%d", SWAP16(pDlg->m_TCPHeader->window));
		pDlg->m_TCPPacketInfo += " " + Flag +" "+ Window;
		pDlg->m_TCPHeaderPayload = (TCP_HEADER_PAYLOAD*)(data + 54);

		// *** 데이터 바로 뒤에 TLS 헤더가 있다면
		if (pDlg->m_TCPHeaderPayload->opaquetype >= 20 && pDlg->m_TCPHeaderPayload->opaquetype <= 26)
		{
			if (((pDlg->m_TCPHeaderPayload->ver1 << 8) | pDlg->m_TCPHeaderPayload->ver2) == 0x301) {
				pDlg->m_Protocol.Format("TLSv1");
			}
			else if (((pDlg->m_TCPHeaderPayload->ver1 << 8) | pDlg->m_TCPHeaderPayload->ver2) == 0x303) {
				pDlg->m_Protocol.Format("TLSv1.2");
			}
			if (pDlg->m_TCPHeaderPayload->opaquetype == 20) {
				pDlg->m_TCPPacketInfo = "Change Cipher Spec";
			}
			else if (pDlg->m_TCPHeaderPayload->opaquetype == 21) {
				pDlg->m_TCPPacketInfo = "Encrypted Alert";
			}
			else if (pDlg->m_TCPHeaderPayload->opaquetype == 22) {
				if (data[59] == 1)
				{
					pDlg->m_TCPPacketInfo = "Client Hello";
				}
				else if (data[59] == 2)
				{
					pDlg->m_TCPPacketInfo = "Server Hello";
				}
				else if (data[59] == 3)
				{
					pDlg->m_TCPPacketInfo = "hello verify request RESERVED";
				}
				else if (data[59] == 4)
				{
					pDlg->m_TCPPacketInfo = "new session ticket";
				}
				else if (data[59] == 5)
				{
					pDlg->m_TCPPacketInfo = "end of early data";
				}
				else if (data[59] == 6)
				{
					pDlg->m_TCPPacketInfo = "hello retry request RESERVED";

				}
				else if (data[59] == 11)
				{
					pDlg->m_TCPPacketInfo = "ceritficate";
				}
				else if (data[59] == 16)
				{
					pDlg->m_TCPPacketInfo = "Client Key Exchange";
				}
				else {
					pDlg->m_TCPPacketInfo.Format(" %X", data[59]);
				}
			}
			else if (pDlg->m_TCPHeaderPayload->opaquetype == 23) {
				pDlg->m_TCPPacketInfo = "Application Data";
			}
		}
		// *** 데이터 바로 뒤에 TLS 헤더가 없다면, 있는지 체그
		else
		{
			for (int i = 54; i < header->caplen;i++)
			{
				if (data[i] >= 20 && data[i] <= 26)
				{
					
					if (data[i] == 20) {
						if (((data[i + 1] << 8) | data[i + 2]) == 0x301) pDlg->m_Protocol = "TLSv1";
						else if (((data[i + 1] << 8) | data[i + 2]) == 0x303) pDlg->m_Protocol = "TLSv1.2";
						else {
							continue;
						}
						pDlg->m_TCPPacketInfo = "Change Cipher Spec";
						break;
					}
					else if (data[i] == 21) {
						if (((data[i + 1] << 8) | data[i + 2]) == 0x301) pDlg->m_Protocol = "TLSv1";
						else if (((data[i + 1] << 8) | data[i + 2]) == 0x303) pDlg->m_Protocol = "TLSv1.2";
						else {
							continue;
						}
						pDlg->m_TCPPacketInfo = "Encrypted Alert";
						break;
					}
					else if (data[i] == 22) {
						if (((data[i + 1] << 8) | data[i + 2]) == 0x301) pDlg->m_Protocol = "TLSv1";
						else if (((data[i + 1] << 8) | data[i + 2]) == 0x303) pDlg->m_Protocol = "TLSv1.2";
						else {
							continue;
						}
						if (data[i+5] == 1)
						{
							pDlg->m_TCPPacketInfo = "Client Hello";
						}
						else if (data[i + 5] == 2)
						{
							pDlg->m_TCPPacketInfo = "Server Hello";
						}
						else if (data[i + 5] == 3)
						{
							pDlg->m_TCPPacketInfo = "hello verify request RESERVED";
						}
						else if (data[i + 5] == 4)
						{
							pDlg->m_TCPPacketInfo = "new session ticket";
						}
						else if (data[i + 5] == 5)
						{
							pDlg->m_TCPPacketInfo = "end of early data";
						}
						else if (data[i + 5] == 6)
						{
							pDlg->m_TCPPacketInfo = "hello retry request RESERVED";
						}
						else if (data[i + 5] == 11)
						{
							pDlg->m_TCPPacketInfo = "Ceritficate";
						}
						else if (data[i + 5] == 12)
						{
							pDlg->m_TCPPacketInfo = "Server Key Exchange RESERVED";
						}
						else if (data[i + 5] == 14)
						{
							pDlg->m_TCPPacketInfo = "Server Hello Done RESERVED";
						}
						else if (data[i + 5] == 16)
						{
							pDlg->m_TCPPacketInfo = "Client Key Exchange";
						}
						else {
							pDlg->m_TCPPacketInfo.Format(" %X", data[i+5]);
							continue;
						}
						break;
					}
					else if (data[i] == 23) {
						if (((data[i + 1] << 8) | data[i + 2]) == 0x301) pDlg->m_Protocol = "TLSv1";
						else if (((data[i + 1] << 8) | data[i + 2]) == 0x303) pDlg->m_Protocol = "TLSv1.2";
						else {
							continue;
						}
						pDlg->m_TCPPacketInfo = "Application Data";
						break;
					}
				}
			}
		}
		size_t ListControlCnt = pDlg->m_NetworkInterfaceControlList.GetItemCount();
		CString ListControlCntStr;
		ListControlCntStr.Format("%d", ListControlCnt + 1);
		pDlg->m_NetworkInterfaceControlList.InsertItem(ListControlCnt, ListControlCntStr, 0);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 1, pDlg->m_CurrentTime);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 2, pDlg->m_SourceIp);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 3, pDlg->m_DestinationIp);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 4, pDlg->m_Protocol);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 5, pDlg->m_PacketLength);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 6, pDlg->m_TCPPacketInfo);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 7, SaveData);
	}
	else if(pDlg->m_IpHeader->protocol == IPPROTO_UDP)
	{
		pDlg->m_Protocol = "UDP";
		pDlg->m_UDPHeader = (UDP_HEADER*)((u_char*)pDlg->m_IpHeader + pDlg->m_IpHeaderLen);
		pDlg->m_UDPPacketInfo.Format("%d -> %d Len = %d", ntohs(pDlg->m_UDPHeader->sport), ntohs(pDlg->m_UDPHeader->dport), SWAP16(pDlg->m_UDPHeader->length) - 8);
		if (ntohs(pDlg->m_UDPHeader->sport) == 53 || ntohs(pDlg->m_UDPHeader->dport) == 53)
		{
			int infocnt = 0;
			CString infoStr = "", infoStrtmp= "";
			pDlg->m_DNSHeader = (DNS_HEADER*)(data + 22 + pDlg->m_IpHeaderLen);//(DNS_HEADER*)((u_char*)pDlg->m_IpHeader + pDlg->m_IpHeaderLen) + 8;
			pDlg->m_Protocol = "DNS";

			for (infocnt = (22 + pDlg->m_IpHeaderLen + 13); data[infocnt] != NULL; infocnt++)
			{
				infoStrtmp.Format("%c", CheckAscii(data[infocnt]));
				infoStr += infoStrtmp;
			}
			
			pDlg->m_UDPPacketInfo.Format("Standard query %s 0x%04X %s %s ",pDlg->m_DNSHeader->IsResponse == 0 ? "" : "response", SWAP16(pDlg->m_DNSHeader->Xid), data[infocnt + 2] == 1 ? "A " : " ",infoStr );
		}
		
		if (ntohs(pDlg->m_UDPHeader->dport) == 1900 && pDlg->m_DestinationIp == "239.255.255.250") {
			pDlg->m_Protocol = "SSDP";
			int i = 42;
			pDlg->m_UDPPacketInfo = "";
			while (1)
			{
				if (data[i] == 0xd && data[i+1] == 0xa) break;
				pDlg->m_UDPPacketInfo += data[i++];
			}
		}
		size_t ListControlCnt = pDlg->m_NetworkInterfaceControlList.GetItemCount();
		CString ListControlCntStr;
		ListControlCntStr.Format(("%d"), ListControlCnt + 1);
		pDlg->m_NetworkInterfaceControlList.InsertItem(ListControlCnt, ListControlCntStr, 0);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 1, pDlg->m_CurrentTime);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 2, pDlg->m_SourceIp);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 3, pDlg->m_DestinationIp);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 4, pDlg->m_Protocol);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 5, pDlg->m_PacketLength);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 6, pDlg->m_UDPPacketInfo);
		pDlg->m_NetworkInterfaceControlList.SetItemText(ListControlCnt, 7, SaveData);
	}
	if (pDlg->m_NetworkInterfaceControlList.GetItemCount() == 1)
	{
		pDlg->SetPacketInfoTree("1",  pDlg->m_CurrentTime, pDlg->m_Protocol,pDlg->m_PacketLength, SaveData);
		if (pDlg->m_IpHeader->protocol == IPPROTO_UDP) pDlg->SetPacketHexList(SaveData, pDlg->m_Protocol, SWAP16(pDlg->m_UDPHeader->length));
		else pDlg->SetPacketHexList(SaveData, pDlg->m_Protocol, 0);
	}
	pDlg->is_RunThreadOut = TRUE;
	// *** 스레드를 정지 시켰을 때 애매하게 출력되고 정지되는거 방지
	if (pDlg->m_eThreadWork == CNetworkPacketCaptureDlg::ThreadWorkingType::THREAD_PAUSE)
	{
		pDlg->m_PCThread->SuspendThread();
	}
	// *** 리스트 컨트롤 꽉찰경우 EnsureVisible() 를 사용하여 자동으로 밑으로 내려준다.
	pDlg->m_NetworkInterfaceControlList.EnsureVisible(pDlg->m_NetworkInterfaceControlList.GetItemCount() - 1, FALSE);
	::memset((void*)data, 0, header->caplen * sizeof(u_char) );
}

// *** 네트워크 인터페이스 세팅 쓰레드 
UINT CNetworkPacketCaptureDlg::PacketCaptureTFunction(LPVOID _mothod)
{
	CString i_cnt;

	pcap_if_t* alldevs,*choice_dev;
	pcap_t* phandle;
	
	bpf_program fcode;
	bpf_u_int32 NetMask = 0;

	char* errbuf = "";
	const char* filter = "tcp or udp or arp or rarp";
	size_t i = 0;

	/* 
		AfxGetApp() - MFC 응용프로그램의 최초에 생성된 스레드의 app를 반환해 준다.
	*/
	CNetworkPacketCaptureDlg* pDlg = (CNetworkPacketCaptureDlg*)AfxGetApp()->m_pMainWnd;	

	/*
		pcap_findalldevs() - 네트워크 디바이스 장치 찾기
	*/
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		AfxMessageBox(errbuf);
	}

	// 디바이스 선택 
	for (choice_dev = alldevs, i = 0; i < pDlg->click_index; choice_dev = choice_dev->next, i++);
	
	pDlg->SetWindowText(choice_dev->name);
	pDlg->m_MyDev = choice_dev;
	// 선택한 디바이스 오픈
	/*
		pcap_t *pcap_open_live(char *device,				디바이스 이름
							int snaplen,					받아 들이는 패킷의 최대 크기
							int promisc,					promiscuous mode로 설정할 것인지 1 - promiscuous 모드 , 0 - 자기에게 향하는 패킷만
							int to_ms,						읽기 시간 초과 설정
							char *errbuf					err 담을 버퍼
							);
	*/
	if ((phandle = pcap_open_live(choice_dev->name, 65536, 1, 1000, errbuf)) == NULL)
	{
		pcap_freealldevs(choice_dev);
		AfxMessageBox("pcap_open_live() error");
		return 0;
	}

	if (pcap_compile(phandle, &fcode, filter, 1, NULL))
	{
		pcap_close(phandle);
		AfxMessageBox("pcap_compile() error");
		return 0;
	}
	
	if (pcap_setfilter(phandle, &fcode) <= FAIL)
	{
		pcap_close(phandle);
		AfxMessageBox("pcap_setfileter() error");
		return 0;
	}
	pDlg->m_NetworkDeviceHandler = phandle;
	pcap_freealldevs(alldevs);

	size_t loop_ret = pcap_loop(phandle, 0, Packet_Handler, NULL);
	
	CString BREAK_LOOP_ERROR_MESSAGE;
	if (loop_ret == 0) {
		BREAK_LOOP_ERROR_MESSAGE = "패킷 소진";
	}
	else if (loop_ret == -1) {
		BREAK_LOOP_ERROR_MESSAGE = "ERROR";
	}
	else if (loop_ret == -2) {
		BREAK_LOOP_ERROR_MESSAGE = "패킷 스레드 종료";
	}
	pDlg->MessageBox(BREAK_LOOP_ERROR_MESSAGE, "LOOP BREAK MEESAGE");
	pcap_close(phandle);
	return 0;
}

// *** 컨트롤 리스트 색칠하는 부분
void CNetworkPacketCaptureDlg::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{

	CString strType;
	BOOL bTCPFlag = FALSE;
	BOOL bUDPFlag = FALSE;
	BOOL bSSDPFlag = FALSE;
	BOOL bARPFlag = FALSE;
	BOOL bDNSFlag = FALSE;
	BOOL bTLSFlag = FALSE;

	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;

	strType = m_NetworkInterfaceControlList.GetItemText(pLVCD->nmcd.dwItemSpec, 4);

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
			pLVCD->clrTextBk = RGB(115, 242, 255);  // 배경 색 변경 
		}
		// UDP 라면
		else if (bUDPFlag)
		{
			//pLVCD->clrText = RGB(0, 255, 0);
			pLVCD->clrTextBk = RGB(122, 255, 131);
		}
		else if (bSSDPFlag)
		{
			pLVCD->clrTextBk = RGB(255, 197, 78);
		}
		else if (bARPFlag)
		{
			pLVCD->clrTextBk = RGB(251, 255, 95);
		}
		else if (bDNSFlag)
		{
			pLVCD->clrTextBk = RGB(255, 98, 98);
		}
		else if (bTLSFlag)
		{
			pLVCD->clrTextBk = RGB(206, 148, 255);
		}
		else
		{
			pLVCD->clrText = RGB(0, 0, 0);
		}
		
		*pResult = CDRF_DODEFAULT;
	}
}

// *** 로그 스레드 
UINT CNetworkPacketCaptureDlg::ThreadClient(LPVOID param)
{
	CNetworkPacketCaptureDlg* pDlg = (CNetworkPacketCaptureDlg*)param;
	int i = 0;
	// 소켓 정보 데이터 설정
	WSADATA wsaData;
	// 소켓 실행.
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return 1;
	}
	// Internet의 Stream 방식으로 소켓 생성
	SOCKET sock = socket(PF_INET, SOCK_STREAM, 0);
	// 소켓 주소 설정
	SOCKADDR_IN addr;
	// 구조체 초기화
	::memset(&addr, 0, sizeof(addr));
	// 소켓은 Internet 타입
	addr.sin_family = AF_INET;
	// 127.0.0.1(localhost)로 접속하기
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	// 포트 9090으로 접속
	addr.sin_port = htons(9000);
	// 접속
	if (connect(sock, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		// 에러 콘솔 출력
		AfxMessageBox("connect error");
		return 1;
	}
	// telent은 한글자씩 데이터가 오기 때문에 글자를 모을 buffer가 필요하다.(wchar_t 타입으로 선언)
	vector<wchar_t> buffer;
	// 수신 데이터
	wchar_t x;
	while (1)
	{
		// 데이터를 받는다. 에러가 발생하면 멈춘다.
		// char* 형식으로 받기 때문에 타입 캐스팅을 한다.
		if (recv(sock, (char*)&x, sizeof(x), 0) == SOCKET_ERROR)
		{
			// 에러 콘솔 출력
			AfxMessageBox("recv error");
			break;
		}
		// 버퍼에 글자를 하나 넣는다.
		buffer.push_back(x);
		// \r\n>\0가 나오면 콘솔에 출력하고 콘솔로 부터 메시지를 기다린다.
		if (buffer.size() > 4 && *(buffer.end() - 4) == '\r' && *(buffer.end() - 3) == '\n' && *(buffer.end() - 2) == '>' && *(buffer.end() - 1) == '\0')
		{
			int idx = pDlg->m_NetworkInterfaceControlList.GetItemCount();
			// 버퍼 초기화
			buffer.clear();
			// 콘솔로 부터 입력을 받는다.
			char input[BUFFERSIZE];
			// 유저로 부터 입력 받기
			CString str;
			// 리스트 아이템 전체다 보내기
			for (int j = 0; j < 7; j++)
			{
				str += pDlg->m_NetworkInterfaceControlList.GetItemText(i, j) + "  ";
			}
			i++;
			strcpy(input, str);
			// 입력받은 길이를 받는다.
			int size = strlen(input);
			// 개행을 넣는다.
			wchar_t buffer[BUFFERSIZE * 2];
			// char*에서 wchar_t*으로 변환하는 함수
			mbstowcs(&buffer[0], input, BUFFERSIZE * 2);
			*(buffer + size) = '\r';
			*(buffer + size + 1) = '\n';
			*(buffer + size + 2) = '\0';
			// 서버로 보내기
			// send함수가 char* 형식으로 보낼 수 있기 때문에 타입 캐스팅을 한다.
			send(sock, (char*)buffer, wcslen(buffer) * 2, 0);
			// 아이템을 다 보냈으면 종료
			if (i == idx)
			{
				break;
			}
			else if (i > idx) {
				DWORD dwResult;
				::GetExitCodeThread(pDlg->m_LOGThread, &dwResult);
				AfxMessageBox("count up error Thread EXIT");
				return 1;
			}
			continue;
		}
	}


	// 서버 소켓 종료
	closesocket(sock);
	// 소켓 종료
	WSACleanup();
	pDlg->m_LOGThread = NULL;
	AfxMessageBox("Log Client Thread EXIT");
	return 1;
}

void CNetworkPacketCaptureDlg::OnTvnSelchangedPacketInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

// *** Start 버튼을 눌렀을 때 
void CNetworkPacketCaptureDlg::OnBnClickedStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// *** 스레드가 스탑인 경우 
	if (!is_PCThreadStart && m_eThreadWork == ThreadWorkingType::THREAD_STOP)
	{
		is_PCThreadStart = TRUE;

		m_eThreadWork = ThreadWorkingType::THREAD_RUNNING;

		m_PCThread = AfxBeginThread(PacketCaptureTFunction, this);
		SetDlgItemText(IDC_STATIC, "THREAD_STATUS: RUNNING");

		if (m_PCThread != NULL) {

			m_PCThread->m_bAutoDelete = FALSE;
		}

		if (m_PCThread == NULL) AfxMessageBox("스레드 생성 실패");
	}
	// *** 스레드가 실행중이고, RUNNING 중이라면 에러메시지
	else if (is_PCThreadStart && m_eThreadWork == ThreadWorkingType::THREAD_RUNNING)
	{
		MessageBox("캡쳐 실행 중", "오류", MB_OK | MB_ICONWARNING);
	}
	else if (m_eThreadWork == ThreadWorkingType::THREAD_PAUSE)
	{
		MessageBox("캡쳐 중지 중", "오류", MB_OK | MB_ICONWARNING);
	}
}

// *** Stop 버튼을 눌렀을 때
void CNetworkPacketCaptureDlg::OnBnClickedStop()
{
	// *** 스레드가 실행중이지 않고, 쓰레드 타입이 RUNNING이 아니라면 
	if (!is_PCThreadStart && m_eThreadWork != ThreadWorkingType::THREAD_RUNNING) {
		MessageBox(_T("캡처가 시작되지 않았습니다."), _T("오류"), MB_ICONWARNING);
	}
	// *** 스레드가 실행중이라면
	else {
		// *** 스레드 타입이 RUNNING 중이라면
		if (m_eThreadWork == ThreadWorkingType::THREAD_RUNNING) {
			if (MessageBox(_T("캡처를 멈추시겠습니까?"), _T("캡처 중지"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
				m_PauseButton.SetWindowText("Resume(&P)");
				SetDlgItemText(IDC_STATIC, "THREAD_STATUS: PAUSE");

				//m_PCThread->SuspendThread();
				if (m_PCThread == NULL) {
					m_PCThread;
				}
				m_eThreadWork = ThreadWorkingType::THREAD_PAUSE;
			}
		}
		// *** 스레드 타입이 PAUSE 중이라면
		else if(m_eThreadWork == ThreadWorkingType::THREAD_PAUSE){
			if (MessageBox(_T("캡처를 다시 시작 하시겠습니까?"), _T("캡처 다시 시작"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
				m_PauseButton.SetWindowText("Pause(&P)");
				SetDlgItemText(IDC_STATIC, "THREAD_STATUS: RUNNING");
				m_PCThread->ResumeThread();
				m_eThreadWork = ThreadWorkingType::THREAD_RUNNING;
			}
		}
	}
}

// *** 트리 세팅
int CNetworkPacketCaptureDlg::SetPacketInfoTree(CString framecnt,CString time, CString protocol, CString length, CString savedata)
{
	CString CStmp;
	// *** 트리 변수 선언
	HTREEITEM TR1 = NULL, TR1_1=NULL, TR1_2=NULL, TR1_3=NULL, TR1_4=NULL, TR1_5=NULL, TR1_6=NULL;
	HTREEITEM TR2 = NULL, TR2_1=NULL, TR2_2=NULL, TR2_3=NULL;
	HTREEITEM TR3 = NULL, TR3_1=NULL, TR3_2=NULL, TR3_3=NULL, TR3_4=NULL, TR3_5=NULL, TR3_6=NULL, TR3_7=NULL, TR3_8=NULL, TR3_9=NULL, TR3_10=NULL, TR3_11=NULL, TR3_12=NULL
					, TR3_6_1 = NULL, TR3_6_2 = NULL, TR3_6_3 = NULL;
	HTREEITEM ARPTR3 = NULL, ARPTR3_1 = NULL, ARPTR3_2 =NULL, ARPTR3_3 =NULL, ARPTR3_4 =NULL, ARPTR3_5=NULL, ARPTR3_6=NULL, ARPTR3_7=NULL, ARPTR3_8=NULL, ARPTR3_9=NULL;
	HTREEITEM TCPTR4 = NULL, TCPTR4_1 = NULL, TCPTR4_2 = NULL, TCPTR4_3 = NULL, TCPTR4_4 = NULL, TCPTR4_5 = NULL,
		TCPTR4_6 = NULL, TCPTR4_6_1 = NULL, TCPTR4_6_2 = NULL, TCPTR4_6_3 = NULL, TCPTR4_6_4 = NULL, TCPTR4_6_5 = NULL, TCPTR4_6_6 = NULL,
		TCPTR4_6_7 = NULL, TCPTR4_6_8 = NULL, TCPTR4_6_9 = NULL, TCPTR4_6_10 = NULL,
				TCPTR4_7=NULL, TCPTR4_8=NULL, TCPTR4_9=NULL, TCPTR4_10 = NULL;
	HTREEITEM UDPTR4 = NULL, UDPTR4_1=NULL, UDPTR4_2=NULL, UDPTR4_3=NULL, UDPTR4_4=NULL, UDPTR4_5=NULL;
	HTREEITEM SSDPTR5 = NULL, SSDPTR5_1 = NULL,SSDPTR5_2 = NULL, SSDPTR5_3 = NULL, SSDPTR5_4 = NULL, SSDPTR5_5 = NULL, SSDPTR5_6 = NULL, SSDPTR5_7 = NULL, SSDPTR5_8 = NULL, SSDPTR5_9;
	HTREEITEM DNSTR5 = NULL, DNSTR5_1 = NULL, DNSTR5_2 = NULL, DNSTR5_3 = NULL, DNSTR5_4 = NULL, DNSTR5_5=NULL, DNSTR5_6=NULL, DNSTR5_7=NULL, DNSTR5_8=NULL,
		DNSTR5_2_1, DNSTR5_2_2, DNSTR5_2_3, DNSTR5_2_4, DNSTR5_2_5, DNSTR5_2_6, DNSTR5_2_7, DNSTR5_2_8, DNSTR5_2_9, DNSTR5_2_10,
		DNSTR5_7_1,
		DNSTR5_8_1, DNSTR5_8_2, DNSTR5_8_3, DNSTR5_8_4, DNSTR5_8_5, DNSTR5_8_6, DNSTR5_8_7, DNSTR5_8_8, DNSTR5_8_9, DNSTR5_8_10,
		DNSTR5_8_11, DNSTR5_8_12, DNSTR5_8_13, DNSTR5_8_14, DNSTR5_8_15;
	HTREEITEM TLSTR5 = NULL,TLSTR5_TMP=NULL, TLSTR5_1 = NULL, TLSTR5_2 = NULL, TLSTR5_3 = NULL, TLSTR5_4 = NULL,
		TLSTR5_5=NULL, TLSTR5_6=NULL, TLSTR5_7=NULL,TLSTR5_8=NULL,
		TLSTR5_9=NULL, TLSTR5_10=NULL, TLSTR5_11=NULL, TLSTR5_12=NULL,
		TLSTR5_13=NULL, TLSTR5_14=NULL, TLSTR5_15=NULL, TLSTR5_16=NULL,

		TLSTR5_4_1 = NULL, TLSTR5_4_2 = NULL, TLSTR5_4_3 = NULL,
		TLSTR5_5_1 = NULL, TLSTR5_5_2 = NULL, TLSTR5_5_3 = NULL, TLSTR5_5_4=NULL,
		TLSTR5_6_1 = NULL, TLSTR5_6_2 = NULL, TLSTR5_6_3 = NULL, TLSTR5_6_4 =NULL,
		TLSTR5_7_1 = NULL, TLSTR5_7_2 = NULL, TLSTR5_7_3 = NULL, TLSTR5_7_4=NULL;


	// *** 트리 초기화
	m_PacketInfoTree.DeleteAllItems();
	m_PacketInfoTree.Invalidate();

	// *** 트리 데이터 선언
	CString TRS1, TRS1_1, TRS1_2, TRS1_3, TRS1_4, TRS1_5, TRS1_6;
	CString TRS2, TRS2_1, TRS2_2, TRS2_3;
	CString TRS3, TRS3_1, TRS3_2, TRS3_3, TRS3_4, TRS3_5, TRS3_6, TRS3_7, TRS3_8, TRS3_9, TRS3_10, TRS3_11, TRS3_12,
			TRS3_6_1, TRS3_6_2, TRS3_6_3;
	CString ARPTRS3, ARPTRS3_1, ARPTRS3_2, ARPTRS3_3, ARPTRS3_4, ARPTRS3_5, ARPTRS3_6, ARPTRS3_7, ARPTRS3_8, ARPTRS3_9;
	CString TCPTRS4, TCPTRS4_1, TCPTRS4_2, TCPTRS4_3, TCPTRS4_4, TCPTRS4_5, TCPTRS4_6,
		TCPTRS4_6_1, TCPTRS4_6_2, TCPTRS4_6_3, TCPTRS4_6_4, TCPTRS4_6_5,
		TCPTRS4_6_6, TCPTRS4_6_7, TCPTRS4_6_8, TCPTRS4_6_9, TCPTRS4_6_10,
		TCPTRS4_7, TCPTRS4_8, TCPTRS4_9, TCPTRS4_10;
	CString UDPTRS4, UDPTRS4_1, UDPTRS4_2, UDPTRS4_3, UDPTRS4_4, UDPTRS4_5;
	CString SSDPTRS5, SSDPTRS5_1, SSDPTRS5_2, SSDPTRS5_3, SSDPTRS5_4, SSDPTRS5_5, SSDPTRS5_6, SSDPTRS5_7, SSDPTRS5_8, SSDPTRS5_9;
	CString DNSTRS5, DNSTRS5_1, DNSTRS5_2, DNSTRS5_3, DNSTRS5_4, DNSTRS5_5, DNSTRS5_6, DNSTRS5_7, DNSTRS5_8,
		DNSTRS5_2_1, DNSTRS5_2_2, DNSTRS5_2_3, DNSTRS5_2_4, DNSTRS5_2_5, DNSTRS5_2_6, DNSTRS5_2_7, DNSTRS5_2_8, DNSTRS5_2_9, DNSTRS5_2_10,
		DNSTRS5_7_1,
		DNSTRS5_8_1, DNSTRS5_8_2, DNSTRS5_8_3, DNSTRS5_8_4, DNSTRS5_8_5, DNSTRS5_8_6, DNSTRS5_8_7, DNSTRS5_8_8, DNSTRS5_8_9, DNSTRS5_8_10,
		DNSTRS5_8_11, DNSTRS5_8_12, DNSTRS5_8_13, DNSTRS5_8_14, DNSTRS5_8_15;
	CString TLSTRS5,TLSTRS5_TMP, TLSTRS5_1, TLSTRS5_2, TLSTRS5_3, TLSTRS5_4,
		TLSTRS5_5, TLSTRS5_6, TLSTRS5_7, TLSTRS5_8,
		TLSTRS5_9, TLSTRS5_10, TLSTRS5_11, TLSTRS5_12,
		TLSTRS5_13, TLSTRS5_14, TLSTRS5_15, TLSTRS5_16,

		TLSTRS5_4_1, TLSTRS5_4_2, TLSTRS5_4_3,
		TLSTRS5_5_1 = NULL, TLSTRS5_5_2 = NULL, TLSTRS5_5_3 = NULL, TLSTRS5_5_4 = NULL,
		TLSTRS5_6_1 = NULL, TLSTRS5_6_2 = NULL, TLSTRS5_6_3 = NULL, TLSTRS5_6_4 = NULL,
		TLSTRS5_7_1 = NULL, TLSTRS5_7_2 = NULL, TLSTRS5_7_3 = NULL, TLSTRS5_7_4 = NULL;
	
	int tlscnt = 0;

	// *** 첫 번째 트리
	TRS1 = "Frame " + framecnt + ": " + length + " bytes on wire, " + length + " bytes captured on interface \\Device\\NPF_" + m_MyDev->name + ", id 0";
	TRS1_1.Format("Interface id: 0 (\\Device\\NPF_%s)",m_MyDev->name);
	TRS1_2.Format("Encapsulation type: Ethernet (1)");
	TRS1_3 = "Arrival Time: " + time;
	TRS1_4 = "Frame Number: " + framecnt;
	TRS1_5 = "Frame Length: " + length;
	TRS1_6 = "Capture Length: " + length;

	CString MACS;
	CString MACD;
	MACD.Format("(%02X:%02X:%02X:%02X:%02X:%02X)", CStringToHex(savedata, 0, 2), CStringToHex(savedata, 2, 2), CStringToHex(savedata,4, 2), CStringToHex(savedata ,6, 2), CStringToHex(savedata ,8, 2), CStringToHex(savedata, 10, 2));
	MACS.Format("(%02X:%02X:%02X:%02X:%02X:%02X)", CStringToHex(savedata ,12, 2), CStringToHex(savedata ,14, 2), CStringToHex(savedata , 16, 2), CStringToHex(savedata ,18 ,2), CStringToHex(savedata ,20, 2), CStringToHex(savedata ,22, 2));

	// *** 두 번째 트리
	TRS2 = "Ethernet II, Src : " + MACS + ", Dst: " + MACD;
	TRS2_1 = "Destination: " + MACD;
	TRS2_2 = "Source: "+ MACS;
	
	int type = CStringToHex(savedata, 24, 4);
	if (type == 0x800)
	{
		TRS2_3.Format("Type: IPv4 (0x%04X)", type);
		TRS3.Format("InternetProtocol Version %X, Src: %d.%d.%d.%d, Dst: %d.%d.%d.%d", CStringToHex(savedata, 28, 1), 
			CStringToHex(savedata, 52, 2), CStringToHex(savedata, 54, 2), CStringToHex(savedata, 56, 2), CStringToHex(savedata, 58, 2),
			CStringToHex(savedata, 60, 2), CStringToHex(savedata, 62, 2), CStringToHex(savedata, 64, 2), CStringToHex(savedata, 66, 2));

		TRS3_1.Format("%d%d%d%d .... = Version: %X", (CStringToHex(savedata, 28, 1) >> 3) & 1, (CStringToHex(savedata, 28, 1) >> 2) & 1, (CStringToHex(savedata, 28, 1) >> 1) & 1,
			(CStringToHex(savedata, 28, 1) >> 0) & 1, CStringToHex(savedata, 28, 1));

		TRS3_2.Format(".... %d%d%d%d = Header Length: %d bytes (%d)", (CStringToHex(savedata, 29, 1) >> 3) & 1, (CStringToHex(savedata, 29, 1) >> 2) & 1, (CStringToHex(savedata, 29, 1) >> 1) & 1,
			(CStringToHex(savedata, 29, 1) >> 0) & 1, CStringToHex(savedata, 29, 1) * 4, CStringToHex(savedata, 29, 1));
		TRS3_3.Format("Differentiated Services Field: 0x%02X", CStringToHex(savedata, 30, 2));

		TRS3_4.Format("Total Length: %d", CStringToHex(savedata, 32, 4));

		TRS3_5.Format("Identification: 0x%04X (%d)", CStringToHex(savedata, 36, 4), CStringToHex(savedata, 36, 4));

		TRS3_6.Format("Flags: 0x%02X", CStringToHex(savedata, 40, 2));
		TRS3_6_1.Format("%d... .... = Reserved bit: %s", (CStringToHex(savedata, 40, 1) >> 3) & 1, ((CStringToHex(savedata, 40, 1) >> 3) & 1) > 0 ? "Set" : "Not set");
		TRS3_6_2.Format(".%d.. .... = Don't fragment: %s", (CStringToHex(savedata, 40, 1) >> 2) & 1, ((CStringToHex(savedata, 40, 1) >> 2) & 1) > 0 ? "Set" : "Not set");
		TRS3_6_3.Format("..%d. .... = More fragments: %s", (CStringToHex(savedata, 40, 1) >> 1) & 1, ((CStringToHex(savedata, 40, 1) >> 1) & 1) > 0 ? "Set" : "Not set");
		
		TRS3_7.Format("...%d %d%d%d%d %d%d%d%d %d%d%d%d = Fragment Offset: %X", (CStringToHex(savedata, 40, 4) >> 12) & 1, (CStringToHex(savedata, 40, 4) >> 11) & 1, (CStringToHex(savedata, 40, 4) >> 10) & 1,
			(CStringToHex(savedata, 40, 4) >> 9) & 1, (CStringToHex(savedata, 40, 4) >> 8) & 1, (CStringToHex(savedata, 40, 4) >> 7) & 1, (CStringToHex(savedata, 40, 4) >> 6) & 1,
			(CStringToHex(savedata, 40, 4) >> 5) & 1, (CStringToHex(savedata, 40, 4) >> 4) & 1, (CStringToHex(savedata, 40, 4) >> 3) & 1, (CStringToHex(savedata, 40, 4) >> 2) & 1,
			(CStringToHex(savedata, 40, 4) >> 1) & 1, (CStringToHex(savedata, 40, 4) >> 0) & 1, m_IpHeader->frag_offset1);
		TRS3_8.Format("Time to Live: %d", CStringToHex(savedata, 44, 2));
		TRS3_9.Format("Protocol: %s (%d)", protocol, CStringToHex(savedata, 46, 2));

		//if (CStringToHex(savedata, 46, 2) == IPPROTO_TCP) TRS3_9.Format("Protocol: %s (%d)",protocol, CStringToHex(savedata, 46, 2));
		//else if (CStringToHex(savedata, 46, 2) == IPPROTO_UDP) TRS3_9.Format("Protocol: %s (%d)", protocol, CStringToHex(savedata, 46, 2));
		TRS3_10.Format("Header Checksum: 0x%04X", CStringToHex(savedata, 48, 4));
		TRS3_11.Format("Source Address: %d.%d.%d.%d", CStringToHex(savedata, 52, 2), CStringToHex(savedata, 54,2), CStringToHex(savedata, 56,2), CStringToHex(savedata, 58,2));			// 52+8
		TRS3_12.Format("Destination Address: %d.%d.%d.%d", CStringToHex(savedata, 60, 2), CStringToHex(savedata, 62,2), CStringToHex(savedata, 64,2), CStringToHex(savedata, 66,2));	
		// *** 세 번째 트리 
		int dnsanswcnt = 0;
		// *** TCP 라면
		if (!protocol.Compare("TCP") || (protocol.Find(_T("TLS")) != -1))
		{
			TCPTRS4.Format("Transmission Control Protocol, Src Port: %d, Dst Port: %d", CStringToHex(savedata, 68, 4), CStringToHex(savedata, 72, 4));
			TCPTRS4_1.Format("Source Port: %d", CStringToHex(savedata, 68, 4));
			TCPTRS4_2.Format("Destination Port: %d", CStringToHex(savedata, 72, 4));
			TCPTRS4_3.Format("Sequence Number: %u", CStringToHex(savedata, 76, 8));
			TCPTRS4_4.Format("Acknowledge Number: %u", CStringToHex(savedata, 84, 8));
			TCPTRS4_5.Format("%d%d%d%d .... = Header Length: %d bytes (%d)", (CStringToHex(savedata, 92, 1) >> 3) & 1, (CStringToHex(savedata, 92, 1) >> 2) & 1, (CStringToHex(savedata, 92, 1) >> 1) & 1,
				(CStringToHex(savedata, 92, 1) >> 0) & 1, CStringToHex(savedata, 92, 1) * 4, CStringToHex(savedata, 92, 1));
			TCPTRS4_6.Format("Flags: 0x%X", CStringToHex(savedata, 93, 3));
			TCPTRS4_6_1.Format("%d%d%d. .... .... = Reserved: %s", (CStringToHex(savedata, 93, 1) >> 3) & 1, (CStringToHex(savedata, 93, 1) >> 2) & 1, (CStringToHex(savedata, 93, 1) >> 1) & 1,
				((CStringToHex(savedata, 93, 1) >> 1) & 0x7) > 0 ? "Set" : "Not Set");
			TCPTRS4_6_2.Format("...%d .... .... = Nonce: %s", CStringToHex(savedata, 93, 1) & 1, (CStringToHex(savedata, 93, 1) & 1) > 0 ? "Set" : "Not Set");
			TCPTRS4_6_3.Format(".... %d... .... = Congestion Window Reduced (CWR): %s", (CStringToHex(savedata, 94, 1) >> 3) & 1, ((CStringToHex(savedata, 94, 1) >> 3) & 1) > 0 ? "Set" : "Not Set");
			TCPTRS4_6_4.Format(".... .%d.. .... = ECN-Echo: %s", (CStringToHex(savedata, 94, 1) >> 2) & 1, ((CStringToHex(savedata, 94, 1) >> 2) & 1) > 0 ? "Set" : "Not Set");
			TCPTRS4_6_5.Format(".... ..%d. .... = Urgent: %s", (CStringToHex(savedata, 94, 1) >> 1) & 1, ((CStringToHex(savedata, 94, 1) >> 1) & 1) > 0 ? "Set" : "Not Set");
			TCPTRS4_6_6.Format(".... ...%d .... = Acknowledgment: %s", (CStringToHex(savedata, 94, 1) >> 0) & 1, ((CStringToHex(savedata, 94, 1) >> 0) & 1) > 0 ? "Set" : "Not Set");
			TCPTRS4_6_7.Format(".... .... %d... = Push: %s", (CStringToHex(savedata, 95, 1) >> 3) & 1, ((CStringToHex(savedata, 95, 1) >> 3) & 1) > 0 ? "Set" : "Not Set");
			TCPTRS4_6_8.Format(".... .... .%d.. = Reset: %s", (CStringToHex(savedata, 95, 1) >> 2) & 1, ((CStringToHex(savedata, 95, 1) >> 2) & 1) > 0 ? "Set" : "Not Set");
			TCPTRS4_6_9.Format(".... .... ..%d. = Syn: %s", (CStringToHex(savedata, 95, 1) >> 1) & 1, ((CStringToHex(savedata, 95, 1) >> 1) & 1) > 0 ? "Set" : "Not Set");
			TCPTRS4_6_10.Format(".... .... ...%d = Fin: %s", (CStringToHex(savedata, 95, 1) >> 0) & 1, ((CStringToHex(savedata, 95, 1) >> 0) & 1) > 0 ? "Set" : "Not Set");

			TCPTRS4_7.Format("Window: %d", CStringToHex(savedata, 96, 4));
			TCPTRS4_8.Format("Checksum: 0x%X", CStringToHex(savedata, 100, 4));
			TCPTRS4_9.Format("Urgent Pointer: %d", CStringToHex(savedata, 104, 4));
			if ((protocol.Find(_T("TLS")) != -1))
			{
				// *** TCP 헤더 끝나고 바로 TLS 있다면
				if (CStringToHex(savedata, 108, 2) >= 20 && CStringToHex(savedata,108,2) <= 26)
				{
					TLSTRS5 = "Transport Layer Security";
					if (CStringToHex(savedata, 108, 2) == 20) {
						TLSTRS5_1.Format("Content Type: Change Cipher Spec (%d)", CStringToHex(savedata, 108, 2));
					}

					else if (CStringToHex(savedata, 108, 2) == 21) {
						TLSTRS5_1.Format("Content Type: Alert (%d)", CStringToHex(savedata, 108, 2));
					}

					else if (CStringToHex(savedata, 108, 2) == 22) {
						TLSTRS5_1.Format("Content Type: Handshake (%d)", CStringToHex(savedata, 108, 2));
					}
					else if (CStringToHex(savedata, 108, 2) == 23) {
						TLSTRS5_1.Format("Content Type: Application Data (%d)", CStringToHex(savedata, 108, 2));
					}
					if (CStringToHex(savedata, 110, 4) == 0x303) TLSTRS5_2.Format("Version: TLS 1.2 (0x%04X)", CStringToHex(savedata, 110, 4));
					else if (CStringToHex(savedata, 110, 4) == 0x301) TLSTRS5_2.Format("Version: TLS 1 (0x%04X)", CStringToHex(savedata, 110, 4));
					TLSTRS5_3.Format("Length: %d", CStringToHex(savedata, 114, 4));

					// *** COntent Type 이 Handshake 라면
					if (CStringToHex(savedata, 108, 2) == 22)
					{
						// *** 1 이면 Client Hello 2 이면 Server Hello
						if (CStringToHex(savedata, 118, 2) == 1)
						{
							TLSTRS5_4 = "Handshake Protocol: Client Hello";
							TLSTRS5_4_1.Format("Handshake Type: Client Hello (%d)", CStringToHex(savedata, 118, 2));
							TLSTRS5_4_2.Format("Length: %d", CStringToHex(savedata, 120, 6));
							TLSTRS5_4_3.Format("Version: TLS %s (0x%04X)", CStringToHex(savedata, 126, 4) == 0x303 ? "1.2" : "1", CStringToHex(savedata, 126, 4));
						}
						else if (CStringToHex(savedata, 118, 2) == 2)
						{
							TLSTRS5_4 = "Handshake Protocol: Server Hello";
							TLSTRS5_4_1.Format("Handshake Type: Server Hello (%d)", CStringToHex(savedata, 118, 2));
							TLSTRS5_4_2.Format("Length: %d", CStringToHex(savedata, 120, 6));
							TLSTRS5_4_3.Format("Version: TLS %s (0x%04X)", CStringToHex(savedata, 126, 4) == 0x303 ? "1.2" : "1", CStringToHex(savedata, 126, 4));
						}
						else if (CStringToHex(savedata, 118, 2) == 4)
						{
							TLSTRS5_4 = "Handshake Protocol: New Session Ticket";
							TLSTRS5_4_1.Format("Handshake Type: New Session Ticket (%d)", CStringToHex(savedata, 118, 2));
							TLSTRS5_4_2.Format("Length: %d", CStringToHex(savedata, 120, 6));
						}
						else if (CStringToHex(savedata, 118, 2) == 12)
						{
							TLSTRS5_4 = "Handshake Protocol: Server Key Exchange";
							TLSTRS5_4_1.Format("Handshake Type: Server Key Exchange (%d)", CStringToHex(savedata, 118, 2));
							TLSTRS5_4_2.Format("Length: %d", CStringToHex(savedata, 120, 6));
						}
						else if (CStringToHex(savedata, 118, 2) == 14)
						{
							TLSTRS5_4 = "Handshake Protocol: Server Hello Done";
							TLSTRS5_4_1.Format("Handshake Type: Server Hello Done (%d)", CStringToHex(savedata, 118, 2));
							TLSTRS5_4_2.Format("Length: %d", CStringToHex(savedata, 120, 6));
						}
						else if (CStringToHex(savedata, 118, 2) == 16)
						{
							TLSTRS5_4 = "Handshake Protocol: Client Key Exchange";
							TLSTRS5_4_1.Format("Handshake Type: Client Key Exchange (%d)", CStringToHex(savedata, 118, 2));
							TLSTRS5_4_2.Format("Length: %d", CStringToHex(savedata, 120, 6));
						}
					}
					int i = 118, k = 0;
					tlscnt = 0;
					while (i <= savedata.GetLength())
					{
						if ((((CStringToHex(savedata, 118 + k, 2) >= 20) && (CStringToHex(savedata, 118 + k, 2) <= 26))
							&& (CStringToHex(savedata, 120 + k, 2) == 3)) && ((CStringToHex(savedata, 122+k, 2) == 1) || (CStringToHex(savedata, 122+k, 2) == 3)))
						{
							if (tlscnt == 0)
							{
								if (CStringToHex(savedata, 118 +k, 2) == 20) {
									TLSTRS5_5.Format("Content Type: Change Cipher Spec (%d)", CStringToHex(savedata, 118 +k, 2));
								}

								else if (CStringToHex(savedata, 118 +k, 2) == 21) {
									TLSTRS5_5.Format("Content Type: Alert (%d)", CStringToHex(savedata, 118 +k, 2));
								}

								else if (CStringToHex(savedata, 118 +k, 2) == 22) {
									TLSTRS5_5.Format("Content Type: Handshake (%d)", CStringToHex(savedata, 118 +k, 2));
								}

								else if (CStringToHex(savedata, 118 +k, 2) == 23) {
									TLSTRS5_5.Format("Content Type: Application Data (%d)", CStringToHex(savedata, 118 +k, 2));
								}

								if (CStringToHex(savedata, 120+k, 4) == 0x303) TLSTRS5_6.Format("Version: TLS 1.2 (0x%04X)", CStringToHex(savedata, 120 +k, 4));
								else if (CStringToHex(savedata, 120 +k, 4) == 0x301) TLSTRS5_6.Format("Version: TLS 1 (0x%04X)", CStringToHex(savedata, 120+k, 4));
								TLSTRS5_7.Format("Length: %d", CStringToHex(savedata, 124 +k, 4));

								// *** COntent Type 이 Handshake 라면
								if (CStringToHex(savedata, 118 +k, 2) == 22)
								{
									// *** 1 이면 Client Hello 2 이면 Server Hello
									if (CStringToHex(savedata, 128+k, 2) == 1)
									{
										TLSTRS5_5_1 = "Handshake Protocol: Client Hello";
										TLSTRS5_5_2.Format("Handshake Type: Client Hello (%d)", CStringToHex(savedata, 128 +k, 2));
										TLSTRS5_5_3.Format("Length: %d", CStringToHex(savedata, 130+k, 6));
										TLSTRS5_5_4.Format("Version: TLS %s (0x%04X)", CStringToHex(savedata, 136+k, 4) == 0x303 ? "1.2" : "1", CStringToHex(savedata, 136 +k, 4));
									}
									else if (CStringToHex(savedata, 128 +k, 2) == 2)
									{
										TLSTRS5_5_1 = "Handshake Protocol: Server Hello";
										TLSTRS5_5_2.Format("Handshake Type: Server Hello (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_5_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
										TLSTRS5_5_4.Format("Version: TLS %s (0x%04X)", CStringToHex(savedata, 136 + k, 4) == 0x303 ? "1.2" : "1", CStringToHex(savedata, 136 + k, 4));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 4)
									{
										TLSTRS5_5_1 = "Handshake Protocol: New Session Ticket";
										TLSTRS5_5_2.Format("Handshake Type: New Session Ticket (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_5_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 12)
									{
										TLSTRS5_5_1 = "Handshake Protocol: Server Key Exchange";
										TLSTRS5_5_2.Format("Handshake Type: Server Key Exchange (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_5_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 14)
									{
										TLSTRS5_5_1 = "Handshake Protocol: Server Hello Done";
										TLSTRS5_5_2.Format("Handshake Type: Server Hello Done (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_5_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 16)
									{
										TLSTRS5_5_1 = "Handshake Protocol: Client Key Exchange";
										TLSTRS5_5_2.Format("Handshake Type: Client Key Exchange (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_5_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
								}
								else {
								}
								tlscnt++;
							}
							else if (tlscnt == 1)
							{
								if (CStringToHex(savedata, 118 + k, 2) == 20) {
									TLSTRS5_8.Format("Content Type: Change Cipher Spec (%d)", CStringToHex(savedata, 118 + k, 2));
								}

								else if (CStringToHex(savedata, 118 + k, 2) == 21) {
									TLSTRS5_8.Format("Content Type: Alert (%d)", CStringToHex(savedata, 118 + k, 2));
								}

								else if (CStringToHex(savedata, 118 + k, 2) == 22) {
									TLSTRS5_8.Format("Content Type: Handshake (%d)", CStringToHex(savedata, 118 + k, 2));
								}
								else if (CStringToHex(savedata, 118 + k, 2) == 23) {
									TLSTRS5_8.Format("Content Type: Application Data (%d)", CStringToHex(savedata, 118 + k, 2));
								}
								if (CStringToHex(savedata, 120 + k, 4) == 0x303) TLSTRS5_9.Format("Version: TLS 1.2 (0x%04X)", CStringToHex(savedata, 120 + k, 4));
								else if (CStringToHex(savedata, 120 + k, 4) == 0x301) TLSTRS5_9.Format("Version: TLS 1 (0x%04X)", CStringToHex(savedata, 120 + k, 4));
								TLSTRS5_10.Format("Length: %d", CStringToHex(savedata, 124 + k, 4));

								// *** COntent Type 이 Handshake 라면
								if (CStringToHex(savedata, 118 + k, 2) == 22)
								{
									// *** 1 이면 Client Hello 2 이면 Server Hello
									if (CStringToHex(savedata, 128 + k, 2) == 1)
									{
										TLSTRS5_6_1 = "Handshake Protocol: Client Hello";
										TLSTRS5_6_2.Format("Handshake Type: Client Hello (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_6_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
										TLSTRS5_6_4.Format("Version: TLS %s (0x%04X)", CStringToHex(savedata, 136 + k, 4) == 0x303 ? "1.2" : "1", CStringToHex(savedata, 136 + k, 4));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 2)
									{
										TLSTRS5_6_1 = "Handshake Protocol: Server Hello";
										TLSTRS5_6_2.Format("Handshake Type: Server Hello (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_6_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
										TLSTRS5_6_4.Format("Version: TLS %s (0x%04X)", CStringToHex(savedata, 136 + k, 4) == 0x303 ? "1.2" : "1", CStringToHex(savedata, 136 + k, 4));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 4)
									{
										TLSTRS5_6_1 = "Handshake Protocol: New Session Ticket";
										TLSTRS5_6_2.Format("Handshake Type: New Session Ticket (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_6_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 12)
									{
										TLSTRS5_6_1 = "Handshake Protocol: Server Key Exchange";
										TLSTRS5_6_2.Format("Handshake Type: Server Key Exchange (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_6_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 14)
									{
										TLSTRS5_6_1 = "Handshake Protocol: Server Hello Done";
										TLSTRS5_6_2.Format("Handshake Type: Server Hello Done (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_6_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 16)
									{
										TLSTRS5_6_1 = "Handshake Protocol: Client Key Exchange";
										TLSTRS5_6_2.Format("Handshake Type: Client Key Exchange (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_6_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
								}
								else {
								}
								tlscnt++;
							}
							else if (tlscnt == 2)
							{
								if (CStringToHex(savedata, 118 + k, 2) == 20) {
									TLSTRS5_11.Format("Content Type: Change Cipher Spec (%d)", CStringToHex(savedata, 118 + k, 2));
								}

								else if (CStringToHex(savedata, 118 + k, 2) == 21) {
									TLSTRS5_11.Format("Content Type: Alert (%d)", CStringToHex(savedata, 118 + k, 2));
								}

								else if (CStringToHex(savedata, 118 + k, 2) == 22) {
									TLSTRS5_11.Format("Content Type: Handshake (%d)", CStringToHex(savedata, 118 + k, 2));
								}
								else if (CStringToHex(savedata, 118 + k, 2) == 23) {
									TLSTRS5_11.Format("Content Type: Application Data (%d)", CStringToHex(savedata, 118 + k, 2));
								}
								if (CStringToHex(savedata, 120 + k, 4) == 0x303) TLSTRS5_12.Format("Version: TLS 1.2 (0x%04X)", CStringToHex(savedata, 120 + k, 4));
								else if (CStringToHex(savedata, 120 + k, 4) == 0x301) TLSTRS5_12.Format("Version: TLS 1 (0x%04X)", CStringToHex(savedata, 120 + k, 4));
								TLSTRS5_13.Format("Length: %d", CStringToHex(savedata, 124 + k, 4));

								// *** COntent Type 이 Handshake 라면
								if (CStringToHex(savedata, 118 + k, 2) == 22)
								{
									// *** 1 이면 Client Hello 2 이면 Server Hello
									if (CStringToHex(savedata, 128 + k, 2) == 1)
									{
										TLSTRS5_7_1 = "Handshake Protocol: Client Hello";
										TLSTRS5_7_2.Format("Handshake Type: Client Hello (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_7_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
										TLSTRS5_7_4.Format("Version: TLS %s (0x%04X)", CStringToHex(savedata, 136 + k, 4) == 0x303 ? "1.2" : "1", CStringToHex(savedata, 136 + k, 4));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 2)
									{
										TLSTRS5_7_1 = "Handshake Protocol: Server Hello";
										TLSTRS5_7_2.Format("Handshake Type: Server Hello (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_7_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
										TLSTRS5_7_4.Format("Version: TLS %s (0x%04X)", CStringToHex(savedata, 136 + k, 4) == 0x303 ? "1.2" : "1", CStringToHex(savedata, 136 + k, 4));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 4)
									{
										TLSTRS5_7_1 = "Handshake Protocol: New Session Ticket";
										TLSTRS5_7_2.Format("Handshake Type: New Session Ticket (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_7_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 12)
									{
										TLSTRS5_7_1 = "Handshake Protocol: Server Key Exchange";
										TLSTRS5_7_2.Format("Handshake Type: Server Key Exchange (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_7_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 14)
									{
										TLSTRS5_7_1 = "Handshake Protocol: Server Hello Done";
										TLSTRS5_7_2.Format("Handshake Type: Server Hello Done (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_7_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 16)
									{
										TLSTRS5_7_1 = "Handshake Protocol: Client Key Exchange";
										TLSTRS5_7_2.Format("Handshake Type: Client Key Exchange (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_7_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
								}
								else {
								}
								tlscnt++;
							}
						}
						k += 2;
						i ++;
					}
				}
				// *** TCP 헤더 끝나고 바로 TLS가 아니라면
				else
				{
				int j;
					for (j = 0; j < savedata.GetLength(); j+= 2)
					{
						if (CStringToHex(savedata, 108 + j, 2) >= 20 && CStringToHex(savedata, 108 + j, 2) <= 26)
						{
							TLSTRS5_TMP.Format("TCP DATA(%d bytes)", j/2);
							TLSTRS5 = "Transport Layer Security";
							if (CStringToHex(savedata, 108 + j, 2) == 20) {

								if (CStringToHex(savedata, 110 + j, 4) == 0x303) TLSTRS5_2.Format("Version: TLS 1.2 (0x%04X)", CStringToHex(savedata, 110 + j, 4));
								else if (CStringToHex(savedata, 110 + j, 4) == 0x301) TLSTRS5_2.Format("Version: TLS 1 (0x%04X)", CStringToHex(savedata, 110 + j, 4));
								else continue;
								TLSTRS5_1.Format("Content Type: Change Cipher Spec (%d)", CStringToHex(savedata, 108 + j, 2));
								TLSTRS5_3.Format("Length: %d", CStringToHex(savedata, 114 + j, 4));
								break;
							}
							else if (CStringToHex(savedata, 108 + j, 2) == 21) {

								if (CStringToHex(savedata, 110 + j, 4) == 0x303) TLSTRS5_2.Format("Version: TLS 1.2 (0x%04X)", CStringToHex(savedata, 110 + j, 4));
								else if (CStringToHex(savedata, 110 + j, 4) == 0x301) TLSTRS5_2.Format("Version: TLS 1 (0x%04X)", CStringToHex(savedata, 110 + j, 4));
								else continue;
								TLSTRS5_1.Format("Content Type: Alert (%d)", CStringToHex(savedata, 108 + j, 2));
								TLSTRS5_3.Format("Length: %d", CStringToHex(savedata, 114 + j, 4));
								break;
								
							}
							else if (CStringToHex(savedata, 108 + j, 2) == 22) {

								if (CStringToHex(savedata, 110 + j, 4) == 0x303) TLSTRS5_2.Format("Version: TLS 1.2 (0x%04X)", CStringToHex(savedata, 110 + j, 4));
								else if (CStringToHex(savedata, 110 + j, 4) == 0x301) TLSTRS5_2.Format("Version: TLS 1 (0x%04X)", CStringToHex(savedata, 110 + j, 4));
								else continue;
								TLSTRS5_1.Format("Content Type: Handshake (%d)", CStringToHex(savedata, 108 + j, 2));
								TLSTRS5_3.Format("Length: %d", CStringToHex(savedata, 114 + j, 4));
							}
							else if (CStringToHex(savedata, 108 + j, 2) == 23) {

								if (CStringToHex(savedata, 110 + j, 4) == 0x303) TLSTRS5_2.Format("Version: TLS 1.2 (0x%04X)", CStringToHex(savedata, 110 + j, 4));
								else if (CStringToHex(savedata, 110 + j, 4) == 0x301) TLSTRS5_2.Format("Version: TLS 1 (0x%04X)", CStringToHex(savedata, 110 + j, 4));
								else continue;
								TLSTRS5_1.Format("Content Type: Application Data (%d)", CStringToHex(savedata, 108 + j, 2));
								TLSTRS5_3.Format("Length: %d", CStringToHex(savedata, 114 + j, 4));
								break;
							}
							// *** 입력한 값중에 없다면
							else {
								if (CStringToHex(savedata, 110 + j, 4) == 0x303) TLSTRS5_2.Format("Version: TLS 1.2 (0x%04X)", CStringToHex(savedata, 110 + j, 4));
								else if (CStringToHex(savedata, 110 + j, 4) == 0x301) TLSTRS5_2.Format("Version: TLS 1 (0x%04X)", CStringToHex(savedata, 110 + j, 4));
								else continue;
								TLSTRS5_1.Format("Content Type: None Checking (%d)", CStringToHex(savedata, 108 + j, 2));
								TLSTRS5_3.Format("Length: %d", CStringToHex(savedata, 114 + j, 4));
								break;
							}
							// *** COntent Type 이 Handshake 라면
							if (CStringToHex(savedata, 108+j, 2) == 22)
							{
								// *** 1 이면 Client Hello 2 이면 Server Hello
								if (CStringToHex(savedata, 118+j, 2) == 1)
								{
									TLSTRS5_4 = "Handshake Protocol: Client Hello";
									TLSTRS5_4_1.Format("Handshake Type: Client Hello (%d)", CStringToHex(savedata, 118+j, 2));
									TLSTRS5_4_2.Format("Length: %d", CStringToHex(savedata, 120+j, 6));
									TLSTRS5_4_3.Format("Version: TLS %s (0x%04X)", CStringToHex(savedata, 126+j, 4) == 0x303 ? "1.2" : "1", CStringToHex(savedata, 126+j, 4));
									break;
									
								}
								else if (CStringToHex(savedata, 118+j, 2) == 2)
								{
									TLSTRS5_4 = "Handshake Protocol: Server Hello";
									TLSTRS5_4_1.Format("Handshake Type: Server Hello (%d)", CStringToHex(savedata, 118+j, 2));
									TLSTRS5_4_2.Format("Length: %d", CStringToHex(savedata, 120+j, 6));
									TLSTRS5_4_3.Format("Version: TLS %s (0x%04X)", CStringToHex(savedata, 126+j, 4) == 0x303 ? "1.2" : "1", CStringToHex(savedata, 126+j, 4));
									break;
									
								}
								else if (CStringToHex(savedata, 118 + j, 2) == 4)
								{
									TLSTRS5_4 = "Handshake Protocol: New Session Ticket";
									TLSTRS5_4_1.Format("Handshake Type: New Session Ticket (%d)", CStringToHex(savedata, 118 + j, 2));
									TLSTRS5_4_2.Format("Length: %d", CStringToHex(savedata, 120 + j, 6));
									break;
								}
								else if (CStringToHex(savedata, 118 + j, 2) == 11)
								{
									TLSTRS5_4 = "Handshake Protocol: Certificate";
									TLSTRS5_4_1.Format("Handshake Type: Certificate (%d)", CStringToHex(savedata, 118 + j, 2));
									TLSTRS5_4_2.Format("Length: %d", CStringToHex(savedata, 120 + j, 6));
									TLSTRS5_4_3.Format("Certificate Length: %d", CStringToHex(savedata, 126 + j, 6));
									break;
								}
								else if (CStringToHex(savedata, 118 + j, 2) == 12)
								{
									TLSTRS5_4 = "Handshake Protocol: Server Key Exchange";
									TLSTRS5_4_1.Format("Handshake Type: Server Key Exchange (%d)", CStringToHex(savedata, 118 + j, 2));
									TLSTRS5_4_2.Format("Length: %d", CStringToHex(savedata, 120 + j, 6));
									break;
								}
								else if (CStringToHex(savedata, 118 + j, 2) == 14)
								{
									TLSTRS5_4 = "Handshake Protocol: Server Hello Done";
									TLSTRS5_4_1.Format("Handshake Type: Server Hello Done (%d)", CStringToHex(savedata, 118 + j, 2));
									TLSTRS5_4_2.Format("Length: %d", CStringToHex(savedata, 120 + j, 6));
									break;
								}
								else if (CStringToHex(savedata, 118 + j, 2) == 16)
								{
									TLSTRS5_4 = "Handshake Protocol: Client Key Exchange";
									TLSTRS5_4_1.Format("Handshake Type: Client Key Exchange (%d)", CStringToHex(savedata, 118 + j, 2));
									TLSTRS5_4_2.Format("Length: %d", CStringToHex(savedata, 120 + j, 6));
									break;
								}
							}
						}
					}
					int i = 118, k = j;
					tlscnt = 0;
					while (i < savedata.GetLength())
					{
						if (((CStringToHex(savedata, 118 + k, 2) >= 20) && (CStringToHex(savedata, 118 + k, 2) <= 26))
							&& (CStringToHex(savedata, 120 + k, 2) == 3) && ((CStringToHex(savedata, 122 + k, 2) == 1) || (CStringToHex(savedata, 122 + k, 2) == 3)))
						{
							if (tlscnt == 0)
							{
								if (CStringToHex(savedata, 118 + k, 2) == 20) {
									TLSTRS5_5.Format("Content Type: Change Cipher Spec (%d)", CStringToHex(savedata, 118 + k, 2));
								}

								else if (CStringToHex(savedata, 118 + k, 2) == 21) {
									TLSTRS5_5.Format("Content Type: Alert (%d)", CStringToHex(savedata, 118 + k, 2));
								}

								else if (CStringToHex(savedata, 118 + k, 2) == 22) {
									TLSTRS5_5.Format("Content Type: Handshake (%d)", CStringToHex(savedata, 118 + k, 2));
								}
								else if (CStringToHex(savedata, 118 + k, 2) == 23) {
									TLSTRS5_5.Format("Content Type: Application Data (%d)", CStringToHex(savedata, 118 + k, 2));
								}
								if (CStringToHex(savedata, 120 + k, 4) == 0x303) TLSTRS5_6.Format("Version: TLS 1.2 (0x%04X)", CStringToHex(savedata, 120 + k, 4));
								else if (CStringToHex(savedata, 120+ k, 4) == 0x301) TLSTRS5_6.Format("Version: TLS 1 (0x%04X)", CStringToHex(savedata, 120 + k, 4));
								TLSTRS5_7.Format("Length: %d", CStringToHex(savedata, 124 + k, 4));

								// *** COntent Type 이 Handshake 라면
								if (CStringToHex(savedata, 118 + k, 2) == 22)
								{
									// *** 1 이면 Client Hello 2 이면 Server Hello
									if (CStringToHex(savedata, 128 + k, 2) == 1)
									{
										TLSTRS5_5_1 = "Handshake Protocol: Client Hello";
										TLSTRS5_5_2.Format("Handshake Type: Client Hello (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_5_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
										TLSTRS5_5_4.Format("Version: TLS %s (0x%04X)", CStringToHex(savedata, 136 + k, 4) == 0x303 ? "1.2" : "1", CStringToHex(savedata, 136 + k, 4));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 2)
									{
										TLSTRS5_5_1 = "Handshake Protocol: Server Hello";
										TLSTRS5_5_2.Format("Handshake Type: Server Hello (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_5_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
										TLSTRS5_5_4.Format("Version: TLS %s (0x%04X)", CStringToHex(savedata, 136 + k, 4) == 0x303 ? "1.2" : "1", CStringToHex(savedata, 136 + k, 4));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 4)
									{
										TLSTRS5_5_1 = "Handshake Protocol: New Session Ticket";
										TLSTRS5_5_2.Format("Handshake Type: New Session Ticket (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_5_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 12)
									{
										TLSTRS5_5_1 = "Handshake Protocol: Server Key Exchange";
										TLSTRS5_5_2.Format("Handshake Type: Server Key Exchange (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_5_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 14)
									{
										TLSTRS5_5_1 = "Handshake Protocol: Server Hello Done";
										TLSTRS5_5_2.Format("Handshake Type: Server Hello Done (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_5_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 16)
									{
										TLSTRS5_5_1 = "Handshake Protocol: Client Key Exchange";
										TLSTRS5_5_2.Format("Handshake Type: Client Key Exchange (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_5_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
								}
								else {
								}
								tlscnt++;
							}
							else if (tlscnt == 1)
							{
								if (CStringToHex(savedata, 118 + k, 2) == 20) {
									TLSTRS5_8.Format("Content Type: Change Cipher Spec (%d)", CStringToHex(savedata, 118 + k, 2));
								}

								else if (CStringToHex(savedata, 118 + k, 2) == 21) {
									TLSTRS5_8.Format("Content Type: Alert (%d)", CStringToHex(savedata, 118 + k, 2));
								}

								else if (CStringToHex(savedata, 118 + k, 2) == 22) {
									TLSTRS5_8.Format("Content Type: Handshake (%d)", CStringToHex(savedata, 118 + k, 2));
								}
								else if (CStringToHex(savedata, 118 + k, 2) == 23) {
									TLSTRS5_8.Format("Content Type: Application Data (%d)", CStringToHex(savedata, 118 + k, 2));
								}
								if (CStringToHex(savedata, 120 + k, 4) == 0x303) TLSTRS5_9.Format("Version: TLS 1.2 (0x%04X)", CStringToHex(savedata, 120 + k, 4));
								else if (CStringToHex(savedata, 120 + k, 4) == 0x301) TLSTRS5_9.Format("Version: TLS 1 (0x%04X)", CStringToHex(savedata, 120 + k, 4));
								TLSTRS5_10.Format("Length: %d", CStringToHex(savedata, 124 + k, 4));

								// *** COntent Type 이 Handshake 라면
								if (CStringToHex(savedata, 118 + k, 2) == 22)
								{
									// *** 1 이면 Client Hello 2 이면 Server Hello
									if (CStringToHex(savedata, 128 + k, 2) == 1)
									{
										TLSTRS5_6_1 = "Handshake Protocol: Client Hello";
										TLSTRS5_6_2.Format("Handshake Type: Client Hello (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_6_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
										TLSTRS5_6_4.Format("Version: TLS %s (0x%04X)", CStringToHex(savedata, 136 + k, 4) == 0x303 ? "1.2" : "1", CStringToHex(savedata, 136 + k, 4));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 2)
									{
										TLSTRS5_6_1 = "Handshake Protocol: Server Hello";
										TLSTRS5_6_2.Format("Handshake Type: Server Hello (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_6_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
										TLSTRS5_6_4.Format("Version: TLS %s (0x%04X)", CStringToHex(savedata, 136 + k, 4) == 0x303 ? "1.2" : "1", CStringToHex(savedata, 136 + k, 4));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 4)
									{
										TLSTRS5_6_1 = "Handshake Protocol: New Session Ticket";
										TLSTRS5_6_2.Format("Handshake Type: New Session Ticket (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_6_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 12)
									{
										TLSTRS5_6_1 = "Handshake Protocol: Server Key Exchange";
										TLSTRS5_6_2.Format("Handshake Type: Server Key Exchange (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_6_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 14)
									{
										TLSTRS5_6_1 = "Handshake Protocol: Server Hello Done";
										TLSTRS5_6_2.Format("Handshake Type: Server Hello Done (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_6_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 16)
									{
										TLSTRS5_6_1 = "Handshake Protocol: Client Key Exchange";
										TLSTRS5_6_2.Format("Handshake Type: Client Key Exchange (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_6_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
								}
								else {
								}
								tlscnt++;
							}
							else if (tlscnt == 2)
							{
								if (CStringToHex(savedata, 118 + k, 2) == 20) {
									TLSTRS5_11.Format("Content Type: Change Cipher Spec (%d)", CStringToHex(savedata, 118 + k, 2));
								}

								else if (CStringToHex(savedata, 118 + k, 2) == 21) {
									TLSTRS5_11.Format("Content Type: Alert (%d)", CStringToHex(savedata, 118 + k, 2));
								}

								else if (CStringToHex(savedata, 118 + k, 2) == 22) {
									TLSTRS5_11.Format("Content Type: Handshake (%d)", CStringToHex(savedata, 118 + k, 2));
								}
								else if (CStringToHex(savedata, 118 + k, 2) == 23) {
									TLSTRS5_11.Format("Content Type: Application Data (%d)", CStringToHex(savedata, 118 + k, 2));
								}
								if (CStringToHex(savedata, 120 + k, 4) == 0x303) TLSTRS5_12.Format("Version: TLS 1.2 (0x%04X)", CStringToHex(savedata, 120 + k, 4));
								else if (CStringToHex(savedata, 120 + k, 4) == 0x301) TLSTRS5_12.Format("Version: TLS 1 (0x%04X)", CStringToHex(savedata, 120 + k, 4));
								TLSTRS5_13.Format("Length: %d", CStringToHex(savedata, 124 + k, 4));

								// *** COntent Type 이 Handshake 라면
								if (CStringToHex(savedata, 118 + k, 2) == 22)
								{
									// *** 1 이면 Client Hello 2 이면 Server Hello
									if (CStringToHex(savedata, 128 + k, 2) == 1)
									{
										TLSTRS5_7_1 = "Handshake Protocol: Client Hello";
										TLSTRS5_7_2.Format("Handshake Type: Client Hello (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_7_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
										TLSTRS5_7_4.Format("Version: TLS %s (0x%04X)", CStringToHex(savedata, 136 + k, 4) == 0x303 ? "1.2" : "1", CStringToHex(savedata, 136 + k, 4));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 2)
									{
										TLSTRS5_7_1 = "Handshake Protocol: Server Hello";
										TLSTRS5_7_2.Format("Handshake Type: Server Hello (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_7_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
										TLSTRS5_7_4.Format("Version: TLS %s (0x%04X)", CStringToHex(savedata, 136 + k, 4) == 0x303 ? "1.2" : "1", CStringToHex(savedata, 136 + k, 4));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 4)
									{
										TLSTRS5_7_1 = "Handshake Protocol: New Session Ticket";
										TLSTRS5_7_2.Format("Handshake Type: New Session Ticket (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_7_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 12)
									{
										TLSTRS5_7_1 = "Handshake Protocol: Server Key Exchange";
										TLSTRS5_7_2.Format("Handshake Type: Server Key Exchange (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_7_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 14)
									{
										TLSTRS5_7_1 = "Handshake Protocol: Server Hello Done";
										TLSTRS5_7_2.Format("Handshake Type: Server Hello Done (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_7_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
									else if (CStringToHex(savedata, 128 + k, 2) == 16)
									{
										TLSTRS5_7_1 = "Handshake Protocol: Client Key Exchange";
										TLSTRS5_7_2.Format("Handshake Type: Client Key Exchange (%d)", CStringToHex(savedata, 128 + k, 2));
										TLSTRS5_7_3.Format("Length: %d", CStringToHex(savedata, 130 + k, 6));
									}
								}
								else {
								}
								tlscnt++;
							}
						}
						k += 2;
						i ++;
					}
				}
				
			}
			// *** IP Header에 Total Length가 40이상이면 TCP Payload 존재
			if (CStringToHex(savedata, 32, 4) > 40)
			{
				TCPTRS4_10.Format("TCP Payload (%d byte)", CStringToHex(savedata, 32, 4) - 40);
			}
		}
		// *** UDP 라면
		else if (!protocol.Compare("UDP") || !protocol.Compare("SSDP") || !protocol.Compare("DNS"))
		{
			UDPTRS4.Format("User Datagram Protocol, Src Port: %d, Dst Port: %d", CStringToHex(savedata, 68, 4), CStringToHex(savedata, 72, 4));
			UDPTRS4_1.Format("Source Port: %d", CStringToHex(savedata, 68, 4));
			UDPTRS4_2.Format("Destination Port: %d", CStringToHex(savedata, 72, 4));
			UDPTRS4_3.Format("Length : %d", CStringToHex(savedata, 76, 4));
			UDPTRS4_4.Format("Checksum: 0x%02X", CStringToHex(savedata, 80, 4));
			UDPTRS4_5.Format("UDP payload: (%d bytes)", CStringToHex(savedata, 76, 4) - sizeof(UDP_HEADER));
			int j = 0;
			CString copytmp = NULL;
			if (!protocol.Compare("SSDP"))
			{

				SSDPTRS5.Format("Simple Service Discovery Protocol");
				while (!(CStringToHex(savedata, 84 + j, 2) == 0xd && CStringToHex(savedata, 84 + j + 2, 2) == 0xa))
				{
					copytmp.Format("%c", CStringToHex(savedata, 84 + j, 2));
					SSDPTRS5_1 += copytmp + "";
					j += 2;
				}
				j += 4;
				while (!(CStringToHex(savedata, 84 + j, 2) == 0xd && CStringToHex(savedata, 84 + j + 2, 2) == 0xa))
				{
					copytmp.Format("%c", CStringToHex(savedata, 84 + j, 2));
					SSDPTRS5_2 += copytmp + "";
					j += 2;
				}
				j += 4;
				SSDPTRS5.Format("Simple Service Discovery Protocol");
				while (!(CStringToHex(savedata, 84 + j, 2) == 0xd && CStringToHex(savedata, 84 + j + 2, 2) == 0xa))
				{
					copytmp.Format("%c", CStringToHex(savedata, 84 + j, 2));
					SSDPTRS5_3 += copytmp + "";
					j += 2;
				}
				j += 4;
				while (!(CStringToHex(savedata, 84 + j, 2) == 0xd && CStringToHex(savedata, 84 + j + 2, 2) == 0xa))
				{
					copytmp.Format("%c", CStringToHex(savedata, 84 + j, 2));
					SSDPTRS5_4 += copytmp + "";
					j += 2;
				}
				j += 4;
				while (!(CStringToHex(savedata, 84 + j, 2) == 0xd && CStringToHex(savedata, 84 + j + 2, 2) == 0xa))
				{
					copytmp.Format("%c", CStringToHex(savedata, 84 + j, 2));
					SSDPTRS5_5 += copytmp + "";
					j += 2;
				}
				j += 4;
				if (!(CStringToHex(savedata, 84 + j, 2) == 0xd && CStringToHex(savedata, 84 + j + 2, 2) == 0xa))
				{
					while (!(CStringToHex(savedata, 84 + j, 2) == 0xd && CStringToHex(savedata, 84 + j + 2, 2) == 0xa))
					{
						copytmp.Format("%c", CStringToHex(savedata, 84 + j, 2));
						SSDPTRS5_6 += copytmp + "";
						j += 2;
					}
					j += 4;
				}
				if (!(CStringToHex(savedata, 84 + j, 2) == 0xd && CStringToHex(savedata, 84 + j + 2, 2) == 0xa))
				{
					while (!(CStringToHex(savedata, 84 + j, 2) == 0xd && CStringToHex(savedata, 84 + j + 2, 2) == 0xa))
					{
						copytmp.Format("%c", CStringToHex(savedata, 84 + j, 2));
						SSDPTRS5_7 += copytmp + "";
						j += 2;
					}
					j += 4;
				}
				if (!(CStringToHex(savedata, 84 + j, 2) == 0xd && CStringToHex(savedata, 84 + j + 2, 2) == 0xa))
				{
					while (!(CStringToHex(savedata, 84 + j, 2) == 0xd && CStringToHex(savedata, 84 + j + 2, 2) == 0xa))
					{
						copytmp.Format("%c", CStringToHex(savedata, 84 + j, 2));
						SSDPTRS5_8 += copytmp + "";
						j += 2;
					}
					j += 4;
				}

				if (!(CStringToHex(savedata, 84 + j, 2) == 0xd && CStringToHex(savedata, 84 + j + 2, 2) == 0xa))
				{
					while (!(CStringToHex(savedata, 84 + j, 2) == 0xd && CStringToHex(savedata, 84 + j + 2, 2) == 0xa))
					{
						copytmp.Format("%c", CStringToHex(savedata, 84 + j, 2));
						SSDPTRS5_9 += copytmp + "";
						j += 2;
					}
					j += 4;
				}
			}
			else if (!protocol.Compare("DNS"))
			{
				CString dnstmp, nametmp = "", cnametmp = "";
				DNSTRS5.Format("Domain Name System (%s)", ((CStringToHex(savedata, 88, 1) >> 3) & 1) > 0 ? "response" : "query");
				DNSTRS5_1.Format("Transaction ID: 0x%04X", CStringToHex(savedata, 84, 4));

				DNSTRS5_2.Format("Flags: 0x%04X Standard query %s", CStringToHex(savedata, 88, 4), (CStringToHex(savedata, 88, 1) >> 3) > 0 ? "response" : "");
				if ((CStringToHex(savedata, 88, 1) >> 3) > 0) DNSTRS5_2 += ((CStringToHex(savedata, 91, 1) >> 3) & 1 | (CStringToHex(savedata, 90, 1) >> 2) & 1 |
					(CStringToHex(savedata, 90, 1) >> 1) & 1 | (CStringToHex(savedata, 90, 1) >> 0) & 1) > 0 ? "error" : ", No error";
				DNSTRS5_2_1.Format("%d... .... .... .... = Response: Message is a %s", (CStringToHex(savedata, 88, 1) >> 3) & 1, (CStringToHex(savedata, 88, 1) >> 3) > 0 ? "response" : "query");
				DNSTRS5_2_2.Format(".%d%d%d %d... .... .... = Opcode: Standard query (%d)", (CStringToHex(savedata, 88, 1) >> 2) & 1, (CStringToHex(savedata, 88, 1) >> 1) & 1,
					(CStringToHex(savedata, 88, 1) >> 0) & 1, (CStringToHex(savedata, 89, 1) >> 3) & 1,
					(CStringToHex(savedata, 88, 1) >> 2) & 1 | (CStringToHex(savedata, 88, 1) >> 1) & 1 | (CStringToHex(savedata, 88, 1) >> 0) & 1 | ((CStringToHex(savedata, 89, 1) >> 3) & 1));

				if ((CStringToHex(savedata, 88, 1) >> 3) > 0) DNSTRS5_2_3.Format(".... .%d.. .... .... = Authoritative: Server is not an authority for domain", (CStringToHex(savedata, 89, 1) >> 2) & 1);
				DNSTRS5_2_4.Format(".... ..%d. .... .... = Truncated: Message is not truncated", (CStringToHex(savedata, 89, 1) >> 1) & 1);
				DNSTRS5_2_5.Format(".... ...%d .... .... = Recursion desired: Do qeury recursively", (CStringToHex(savedata, 89, 1) >> 0) & 1);
				if ((CStringToHex(savedata, 88, 1) >> 3) > 0) DNSTRS5_2_6.Format(".... .... %d... .... = Recursion available: Server can do recursive queries", (CStringToHex(savedata, 90, 1) >> 3) & 1);
				DNSTRS5_2_7.Format(".... .... .%d.. .... = Z: reserved (%d)", (CStringToHex(savedata, 90, 1) >> 2) & 1, (CStringToHex(savedata, 90, 1) >> 2) & 1);
				if ((CStringToHex(savedata, 88, 1) >> 3) > 0) DNSTRS5_2_8.Format(".... .... ..%d. .... = Answer authenticated: Answer/authority portion was not authenticated by the server", (CStringToHex(savedata, 90, 1) >> 1) & 1);
				DNSTRS5_2_9.Format(".... .... ...%d .... = Non-authenticated data: Unacceptable", (CStringToHex(savedata, 90, 1) >> 0) & 1);
				if ((CStringToHex(savedata, 88, 1) >> 3) > 0) DNSTRS5_2_10.Format(".... .... .... %d%d%d%d = Reply code: No error(%d)", (CStringToHex(savedata, 91, 1) >> 3) & 1, (CStringToHex(savedata, 90, 1) >> 2) & 1,
					(CStringToHex(savedata, 90, 1) >> 1) & 1, (CStringToHex(savedata, 90, 1) >> 0) & 1,
					((CStringToHex(savedata, 91, 1) >> 3) & 1 | (CStringToHex(savedata, 90, 1) >> 2) & 1 |
						(CStringToHex(savedata, 90, 1) >> 1) & 1 | (CStringToHex(savedata, 90, 1) >> 0) & 1));

				DNSTRS5_3.Format("Questions: %d", CStringToHex(savedata, 92, 4));
				DNSTRS5_4.Format("Answer RRs: %d", CStringToHex(savedata, 96, 4));
				dnsanswcnt = CStringToHex(savedata, 96, 4);
				DNSTRS5_5.Format("Authority RRs: %d", CStringToHex(savedata, 100, 4));
				DNSTRS5_6.Format("Additional RRs: %d", CStringToHex(savedata, 104, 4));
				DNSTRS5_7.Format("Queries");
				int j = 0;
				while (CStringToHex(savedata, 108 + j, 2) != 0x00)
				{
					dnstmp.Format("%c", IsAlpha(CStringToHex(savedata, 108 + j, 2)));
					DNSTRS5_7_1 += dnstmp;
					j += 2;
				}
				j += 4;
				nametmp = DNSTRS5_7_1;
				DNSTRS5_7_1 += ": type ";
				DNSTRS5_8_1 = DNSTRS5_7_1;
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_7_1 += "A, class";
					j += 4;
				}
				else if (CStringToHex(savedata, 108 + j, 2) == 5)
				{
					DNSTRS5_7_1 += "CNAME, class ";
					j += 4;
				}

				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_7_1 += " IN";
					j += 8;
				}
				else
				{

				}
				// *** answers
				DNSTRS5_8.Format("Answers");
				int bType = 0;
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_1 += " A, class ";
					bType = 1;
					j += 4;
				}
				else if (CStringToHex(savedata, 108 + j, 2) == 5)
				{
					DNSTRS5_8_1 += " CNAME, class";
					bType = 0;
					j += 4;
				}
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_1 += " IN";
					j += 12;
				}
				if (bType)		// *** type A
				{
					j += 2;
					DNSTRS5_8_1 += " addr ";
					dnstmp.Format("%d.%d.%d.%d", CStringToHex(savedata, 108 + j, 2), CStringToHex(savedata, 108 + j + 2, 2)
						, CStringToHex(savedata, 108 + j + 4, 2), CStringToHex(savedata, 108 + j + 6, 2));
					DNSTRS5_8_1 += dnstmp;
					DNSTRS5_8_2 = nametmp;
					j += 14;
				}
				else if (!bType)	// *** type CNAME
				{
					int cnamelength = CStringToHex(savedata, 108 + j, 2);
					j += 2;
					DNSTRS5_8_1 += " cname ";
					nametmp = "";
					for (int k = 0; k < cnamelength; k++)
					{
						if (k == (cnamelength - 2) && (CStringToHex(savedata, 108 + j, 2) == 0xc0))
						{
							j += 2;
							int idx = CStringToHex(savedata, 108 + j, 2);
							int o = 0;
							while (CStringToHex(savedata, 84 + (idx * 2) + o, 2) != 0)
							{
								if (CStringToHex(savedata, 84 +(idx * 2)+ o, 2) == 0xc0)
								{
									int thirdidx = CStringToHex(savedata, 108 + j, 2);
									int z = 0;
									while (CStringToHex(savedata, 84 + (thirdidx * 2) + z, 2) != 0)
									{
										cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 84 + (thirdidx * 2) + z, 2)));
										DNSTRS5_8_1 += cnametmp;
										nametmp += cnametmp;
										z += 2;
									}
									break;
								}
								cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 84 + (idx * 2) + o, 2)));
								DNSTRS5_8_1 += cnametmp;
								nametmp += cnametmp;
								o += 2;
							}
							j += 2;
							break;
						}
						cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 108 + j, 2)));
						DNSTRS5_8_1 += cnametmp;
						nametmp += cnametmp;
						j += 2;
					}
					DNSTRS5_8_2 = nametmp;
					j += 6;
				}
				/*
				CString CheckIdx;
				CheckIdx.Format("%X %X %X %X ", CStringToHex(savedata, 108 + j, 2), CStringToHex(savedata, 108 + j+2, 2),
												CStringToHex(savedata, 108 + j+4, 2), CStringToHex(savedata, 108 + j+6, 2));
				*/
				DNSTRS5_8_2 += ": type ";
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_2 += " A, class ";
					bType = 1;
					j += 4;
				}
				else if (CStringToHex(savedata, 108 + j, 2) == 5)
				{
					DNSTRS5_8_2 += " CNAME, class";
					bType = 0;
					j += 4;
				}
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_2 += " IN";
					j += 12;
				}
				if (bType)		// *** type A
				{
					j += 2;
					DNSTRS5_8_2 += " addr ";
					dnstmp.Format("%d.%d.%d.%d", CStringToHex(savedata, 108 + j, 2), CStringToHex(savedata, 108 + j + 2, 2)
						, CStringToHex(savedata, 108 + j + 4, 2), CStringToHex(savedata, 108 + j + 6, 2));
					DNSTRS5_8_2 += dnstmp;
					DNSTRS5_8_3 = nametmp;
					j += 14;
				}
				else if (!bType)	// *** type CNAME
				{
					int cnamelength = CStringToHex(savedata, 108 + j, 2);
					j += 2;
					DNSTRS5_8_2 += " cname ";
					nametmp = "";
					for (int k = 0; k < cnamelength; k++)
					{
						if (k == (cnamelength - 2) && (CStringToHex(savedata, 108 + j, 2) == 0xc0))
						{
							j += 2;
							int idx = CStringToHex(savedata, 108 + j, 2);
							int o = 0;
							while (CStringToHex(savedata, 84 + (idx * 2) + o, 2) != 0)
							{
								if (CStringToHex(savedata, 84 + (idx * 2) + o, 2) == 0xc0)
								{
									int thirdidx = CStringToHex(savedata, 108 + j, 2);
									int z = 0;
									while (CStringToHex(savedata, 84 + (thirdidx * 2) + z, 2) != 0)
									{
										cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 84 + (thirdidx * 2) + z, 2)));
										DNSTRS5_8_1 += cnametmp;
										nametmp += cnametmp;
										z += 2;
									}
									break;
								}
								cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 84 + (idx * 2) + o, 2)));
								DNSTRS5_8_2 += cnametmp;
								nametmp += cnametmp;
								o += 2;
							}
							j += 2;
							break;
						}
						cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 108 + j, 2)));
						DNSTRS5_8_2 += cnametmp;
						nametmp += cnametmp;
						j += 2;
					}
					DNSTRS5_8_3 = nametmp;
					j += 6;
				}

				DNSTRS5_8_3 += ": type ";
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_3 += " A, class ";
					bType = 1;
					j += 4;
				}
				else if (CStringToHex(savedata, 108 + j, 2) == 5)
				{
					DNSTRS5_8_3 += " CNAME, class";
					bType = 0;
					j += 4;
				}
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_3 += " IN";
					j += 12;
				}
				if (bType)		// *** type A
				{
					j += 2;
					DNSTRS5_8_3 += " addr ";
					dnstmp.Format("%d.%d.%d.%d", CStringToHex(savedata, 108 + j, 2), CStringToHex(savedata, 108 + j + 2, 2)
						, CStringToHex(savedata, 108 + j + 4, 2), CStringToHex(savedata, 108 + j + 6, 2));
					DNSTRS5_8_3 += dnstmp;
					DNSTRS5_8_4 = nametmp;
					j += 14;
				}
				else if (!bType)	// *** type CNAME
				{
					int cnamelength = CStringToHex(savedata, 108 + j, 2);
					j += 2;
					DNSTRS5_8_3 += " cname ";
					nametmp = "";
					for (int k = 0; k < cnamelength; k++)
					{
						if (k == (cnamelength - 2) && (CStringToHex(savedata, 108 + j, 2) == 0xc0))
						{
							j += 2;
							int idx = CStringToHex(savedata, 108 + j, 2);
							int o = 0;
							while (CStringToHex(savedata, 84 + (idx * 2) + o, 2) != 0)
							{
								cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 84 + (idx * 2) + o, 2)));
								DNSTRS5_8_3 += cnametmp;
								nametmp += cnametmp;
								o += 2;
							}
							j += 2;
							break;
						}
						cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 108 + j, 2)));
						DNSTRS5_8_3 += cnametmp;
						nametmp += cnametmp;
						j += 2;
					}
					DNSTRS5_8_4 = nametmp;
					j += 6;
				}

				DNSTRS5_8_4 += ": type ";
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_4 += " A, class ";
					bType = 1;
					j += 4;
				}
				else if (CStringToHex(savedata, 108 + j, 2) == 5)
				{
					DNSTRS5_8_4 += " CNAME, class";
					bType = 0;
					j += 4;
				}
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_4 += " IN";
					j += 12;
				}
				if (bType)		// *** type A
				{
					j += 2;
					DNSTRS5_8_4 += " addr ";
					dnstmp.Format("%d.%d.%d.%d", CStringToHex(savedata, 108 + j, 2), CStringToHex(savedata, 108 + j + 2, 2)
						, CStringToHex(savedata, 108 + j + 4, 2), CStringToHex(savedata, 108 + j + 6, 2));
					DNSTRS5_8_4 += dnstmp;
					DNSTRS5_8_5 = nametmp;
					j += 14;
				}
				else if (!bType)	// *** type CNAME
				{
					int cnamelength = CStringToHex(savedata, 108 + j, 2);
					j += 2;
					DNSTRS5_8_4 += " cname ";
					nametmp = "";
					for (int k = 0; k < cnamelength; k++)
					{
						if (k == (cnamelength - 2) && (CStringToHex(savedata, 108 + j, 2) == 0xc0))
						{
							j += 2;
							int idx = CStringToHex(savedata, 108 + j, 2);
							int o = 0;
							while (CStringToHex(savedata, 84 + (idx * 2) + o, 2) != 0)
							{
								cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 84 + (idx * 2) + o, 2)));
								DNSTRS5_8_4 += cnametmp;
								nametmp += cnametmp;
								o += 2;
							}
							j += 2;
							break;
						}
						cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 108 + j, 2)));
						DNSTRS5_8_4 += cnametmp;
						nametmp += cnametmp;
						j += 2;
					}
					DNSTRS5_8_5 = nametmp;
					j += 6;
				}

				DNSTRS5_8_5 += ": type ";
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_5 += " A, class ";
					bType = 1;
					j += 4;
				}
				else if (CStringToHex(savedata, 108 + j, 2) == 5)
				{
					DNSTRS5_8_5 += " CNAME, class";
					bType = 0;
					j += 4;
				}
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_5 += " IN";
					j += 12;
				}
				if (bType)		// *** type A
				{
					j += 2;
					DNSTRS5_8_5 += " addr ";
					dnstmp.Format("%d.%d.%d.%d", CStringToHex(savedata, 108 + j, 2), CStringToHex(savedata, 108 + j + 2, 2)
						, CStringToHex(savedata, 108 + j + 4, 2), CStringToHex(savedata, 108 + j + 6, 2));
					DNSTRS5_8_5 += dnstmp;
					DNSTRS5_8_6 = nametmp;
					j += 14;
				}
				else if (!bType)	// *** type CNAME
				{
					int cnamelength = CStringToHex(savedata, 108 + j, 2);
					j += 2;
					DNSTRS5_8_5 += " cname ";
					nametmp = "";
					for (int k = 0; k < cnamelength; k++)
					{
						if (k == (cnamelength - 2) && (CStringToHex(savedata, 108 + j, 2) == 0xc0))
						{
							j += 2;
							int idx = CStringToHex(savedata, 108 + j, 2);
							int o = 0;
							while (CStringToHex(savedata, 84 + (idx * 2) + o, 2) != 0)
							{
								cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 84 + (idx * 2) + o, 2)));
								DNSTRS5_8_5 += cnametmp;
								nametmp += cnametmp;
								o += 2;
							}
							j += 2;
							break;
						}
						cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 108 + j, 2)));
						DNSTRS5_8_5 += cnametmp;
						nametmp += cnametmp;
						j += 2;
					}
					DNSTRS5_8_6 = nametmp;
					j += 6;
				}

				DNSTRS5_8_6 += ": type ";
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_6 += " A, class ";
					bType = 1;
					j += 4;
				}
				else if (CStringToHex(savedata, 108 + j, 2) == 5)
				{
					DNSTRS5_8_6 += " CNAME, class";
					bType = 0;
					j += 4;
				}
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_6 += " IN";
					j += 12;
				}
				if (bType)		// *** type A
				{
					j += 2;
					DNSTRS5_8_6 += " addr ";
					dnstmp.Format("%d.%d.%d.%d", CStringToHex(savedata, 108 + j, 2), CStringToHex(savedata, 108 + j + 2, 2)
						, CStringToHex(savedata, 108 + j + 4, 2), CStringToHex(savedata, 108 + j + 6, 2));
					DNSTRS5_8_6 += dnstmp;
					DNSTRS5_8_7 = nametmp;
					j += 14;
				}
				else if (!bType)	// *** type CNAME
				{
					int cnamelength = CStringToHex(savedata, 108 + j, 2);
					j += 2;
					DNSTRS5_8_6 += " cname ";
					nametmp = "";
					for (int k = 0; k < cnamelength; k++)
					{
						if (k == (cnamelength - 2) && (CStringToHex(savedata, 108 + j, 2) == 0xc0))
						{
							j += 2;
							int idx = CStringToHex(savedata, 108 + j, 2);
							int o = 0;
							while (CStringToHex(savedata, 84 + (idx * 2) + o, 2) != 0)
							{
								cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 84 + (idx * 2) + o, 2)));
								DNSTRS5_8_6 += cnametmp;
								nametmp += cnametmp;
								o += 2;
							}
							j += 2;
							break;
						}
						cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 108 + j, 2)));
						DNSTRS5_8_6 += cnametmp;
						nametmp += cnametmp;
						j += 2;
					}
					DNSTRS5_8_7 = nametmp;
					j += 6;
				}

				DNSTRS5_8_7 += ": type ";
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_7 += " A, class ";
					bType = 1;
					j += 4;
				}
				else if (CStringToHex(savedata, 108 + j, 2) == 5)
				{
					DNSTRS5_8_7 += " CNAME, class";
					bType = 0;
					j += 4;
				}
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_7 += " IN";
					j += 12;
				}
				if (bType)		// *** type A
				{
					j += 2;
					DNSTRS5_8_7 += " addr ";
					dnstmp.Format("%d.%d.%d.%d", CStringToHex(savedata, 108 + j, 2), CStringToHex(savedata, 108 + j + 2, 2)
						, CStringToHex(savedata, 108 + j + 4, 2), CStringToHex(savedata, 108 + j + 6, 2));
					DNSTRS5_8_7 += dnstmp;
					DNSTRS5_8_8 = nametmp;
					j += 14;
				}
				else if (!bType)	// *** type CNAME
				{
					int cnamelength = CStringToHex(savedata, 108 + j, 2);
					j += 2;
					DNSTRS5_8_7 += " cname ";
					nametmp = "";
					for (int k = 0; k < cnamelength; k++)
					{
						if (k == (cnamelength - 2) && (CStringToHex(savedata, 108 + j, 2) == 0xc0))
						{
							j += 2;
							int idx = CStringToHex(savedata, 108 + j, 2);
							int o = 0;
							while (CStringToHex(savedata, 84 + (idx * 2) + o, 2) != 0)
							{
								cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 84 + (idx * 2) + o, 2)));
								DNSTRS5_8_7 += cnametmp;
								nametmp += cnametmp;
								o += 2;
							}
							j += 2;
							break;
						}
						cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 108 + j, 2)));
						DNSTRS5_8_7 += cnametmp;
						nametmp += cnametmp;
						j += 2;
					}
					DNSTRS5_8_8 = nametmp;
					j += 6;
				}

				DNSTRS5_8_8 += ": type ";
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_8 += " A, class ";
					bType = 1;
					j += 4;
				}
				else if (CStringToHex(savedata, 108 + j, 2) == 5)
				{
					DNSTRS5_8_8 += " CNAME, class";
					bType = 0;
					j += 4;
				}
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_8 += " IN";
					j += 12;
				}
				if (bType)		// *** type A
				{
					j += 2;
					DNSTRS5_8_8 += " addr ";
					dnstmp.Format("%d.%d.%d.%d", CStringToHex(savedata, 108 + j, 2), CStringToHex(savedata, 108 + j + 2, 2)
						, CStringToHex(savedata, 108 + j + 4, 2), CStringToHex(savedata, 108 + j + 6, 2));
					DNSTRS5_8_8 += dnstmp;
					DNSTRS5_8_9 = nametmp;
					j += 14;
				}
				else if (!bType)	// *** type CNAME
				{
					int cnamelength = CStringToHex(savedata, 108 + j, 2);
					j += 2;
					DNSTRS5_8_8 += " cname ";
					nametmp = "";
					for (int k = 0; k < cnamelength; k++)
					{
						if (k == (cnamelength - 2) && (CStringToHex(savedata, 108 + j, 2) == 0xc0))
						{
							j += 2;
							int idx = CStringToHex(savedata, 108 + j, 2);
							int o = 0;
							while (CStringToHex(savedata, 84 + (idx * 2) + o, 2) != 0)
							{
								cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 84 + (idx * 2) + o, 2)));
								DNSTRS5_8_8 += cnametmp;
								nametmp += cnametmp;
								o += 2;
							}
							j += 2;
							break;
						}
						cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 108 + j, 2)));
						DNSTRS5_8_8 += cnametmp;
						nametmp += cnametmp;
						j += 2;
					}
					DNSTRS5_8_9 = nametmp;
					j += 6;
				}

				DNSTRS5_8_9 += ": type ";
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_9 += " A, class ";
					bType = 1;
					j += 4;
				}
				else if (CStringToHex(savedata, 108 + j, 2) == 5)
				{
					DNSTRS5_8_9 += " CNAME, class";
					bType = 0;
					j += 4;
				}
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_9 += " IN";
					j += 12;
				}
				if (bType)		// *** type A
				{
					j += 2;
					DNSTRS5_8_9 += " addr ";
					dnstmp.Format("%d.%d.%d.%d", CStringToHex(savedata, 108 + j, 2), CStringToHex(savedata, 108 + j + 2, 2)
						, CStringToHex(savedata, 108 + j + 4, 2), CStringToHex(savedata, 108 + j + 6, 2));
					DNSTRS5_8_9 += dnstmp;
					DNSTRS5_8_10 = nametmp;
					j += 14;
				}
				else if (!bType)	// *** type CNAME
				{
					int cnamelength = CStringToHex(savedata, 108 + j, 2);
					j += 2;
					DNSTRS5_8_9 += " cname ";
					nametmp = "";
					for (int k = 0; k < cnamelength; k++)
					{
						if (k == (cnamelength - 2) && (CStringToHex(savedata, 108 + j, 2) == 0xc0))
						{
							j += 2;
							int idx = CStringToHex(savedata, 108 + j, 2);
							int o = 0;
							while (CStringToHex(savedata, 84 + (idx * 2) + o, 2) != 0)
							{
								cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 84 + (idx * 2) + o, 2)));
								DNSTRS5_8_9 += cnametmp;
								nametmp += cnametmp;
								o += 2;
							}
							j += 2;
							break;
						}
						cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 108 + j, 2)));
						DNSTRS5_8_9 += cnametmp;
						nametmp += cnametmp;
						j += 2;
					}
					DNSTRS5_8_10 = nametmp;
					j += 6;
				}

				DNSTRS5_8_10 += ": type ";
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_10 += " A, class ";
					bType = 1;
					j += 4;
				}
				else if (CStringToHex(savedata, 108 + j, 2) == 5)
				{
					DNSTRS5_8_10 += " CNAME, class";
					bType = 0;
					j += 4;
				}
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_10 += " IN";
					j += 12;
				}
				if (bType)		// *** type A
				{
					j += 2;
					DNSTRS5_8_10 += " addr ";
					dnstmp.Format("%d.%d.%d.%d", CStringToHex(savedata, 108 + j, 2), CStringToHex(savedata, 108 + j + 2, 2)
						, CStringToHex(savedata, 108 + j + 4, 2), CStringToHex(savedata, 108 + j + 6, 2));
					DNSTRS5_8_10 += dnstmp;
					DNSTRS5_8_11 = nametmp;
					j += 14;
				}
				else if (!bType)	// *** type CNAME
				{
					int cnamelength = CStringToHex(savedata, 108 + j, 2);
					j += 2;
					DNSTRS5_8_10 += " cname ";
					nametmp = "";
					for (int k = 0; k < cnamelength; k++)
					{
						if (k == (cnamelength - 2) && (CStringToHex(savedata, 108 + j, 2) == 0xc0))
						{
							j += 2;
							int idx = CStringToHex(savedata, 108 + j, 2);
							int o = 0;
							while (CStringToHex(savedata, 84 + (idx * 2) + o, 2) != 0)
							{
								cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 84 + (idx * 2) + o, 2)));
								DNSTRS5_8_10 += cnametmp;
								nametmp += cnametmp;
								o += 2;
							}
							j += 2;
							break;
						}
						cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 108 + j, 2)));
						DNSTRS5_8_10 += cnametmp;
						nametmp += cnametmp;
						j += 2;
					}
					DNSTRS5_8_11 = nametmp;
					j += 6;
				}

				DNSTRS5_8_11 += ": type ";
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_11 += " A, class ";
					bType = 1;
					j += 4;
				}
				else if (CStringToHex(savedata, 108 + j, 2) == 5)
				{
					DNSTRS5_8_11 += " CNAME, class";
					bType = 0;
					j += 4;
				}
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_11 += " IN";
					j += 12;
				}
				if (bType)		// *** type A
				{
					j += 2;
					DNSTRS5_8_11 += " addr ";
					dnstmp.Format("%d.%d.%d.%d", CStringToHex(savedata, 108 + j, 2), CStringToHex(savedata, 108 + j + 2, 2)
						, CStringToHex(savedata, 108 + j + 4, 2), CStringToHex(savedata, 108 + j + 6, 2));
					DNSTRS5_8_11 += dnstmp;
					DNSTRS5_8_12 = nametmp;
					j += 14;
				}
				else if (!bType)	// *** type CNAME
				{
					int cnamelength = CStringToHex(savedata, 108 + j, 2);
					j += 2;
					DNSTRS5_8_11 += " cname ";
					nametmp = "";
					for (int k = 0; k < cnamelength; k++)
					{
						if (k == (cnamelength - 2) && (CStringToHex(savedata, 108 + j, 2) == 0xc0))
						{
							j += 2;
							int idx = CStringToHex(savedata, 108 + j, 2);
							int o = 0;
							while (CStringToHex(savedata, 84 + (idx * 2) + o, 2) != 0)
							{
								cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 84 + (idx * 2) + o, 2)));
								DNSTRS5_8_11 += cnametmp;
								nametmp += cnametmp;
								o += 2;
							}
							j += 2;
							break;
						}
						cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 108 + j, 2)));
						DNSTRS5_8_11 += cnametmp;
						nametmp += cnametmp;
						j += 2;
					}
					DNSTRS5_8_12 = nametmp;
					j += 6;
				}

				DNSTRS5_8_12 += ": type ";
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_12 += " A, class ";
					bType = 1;
					j += 4;
				}
				else if (CStringToHex(savedata, 108 + j, 2) == 5)
				{
					DNSTRS5_8_12 += " CNAME, class";
					bType = 0;
					j += 4;
				}
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_12 += " IN";
					j += 12;
				}
				if (bType)		// *** type A
				{
					j += 2;
					DNSTRS5_8_12 += " addr ";
					dnstmp.Format("%d.%d.%d.%d", CStringToHex(savedata, 108 + j, 2), CStringToHex(savedata, 108 + j + 2, 2)
						, CStringToHex(savedata, 108 + j + 4, 2), CStringToHex(savedata, 108 + j + 6, 2));
					DNSTRS5_8_12 += dnstmp;
					DNSTRS5_8_13 = nametmp;
					j += 14;
				}
				else if (!bType)	// *** type CNAME
				{
					int cnamelength = CStringToHex(savedata, 108 + j, 2);
					j += 2;
					DNSTRS5_8_12 += " cname ";
					nametmp = "";
					for (int k = 0; k < cnamelength; k++)
					{
						if (k == (cnamelength - 2) && (CStringToHex(savedata, 108 + j, 2) == 0xc0))
						{
							j += 2;
							int idx = CStringToHex(savedata, 108 + j, 2);
							int o = 0;
							while (CStringToHex(savedata, 84 + (idx * 2) + o, 2) != 0)
							{
								cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 84 + (idx * 2) + o, 2)));
								DNSTRS5_8_12 += cnametmp;
								nametmp += cnametmp;
								o += 2;
							}
							j += 2;
							break;
						}
						cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 108 + j, 2)));
						DNSTRS5_8_12 += cnametmp;
						nametmp += cnametmp;
						j += 2;
					}
					DNSTRS5_8_13 = nametmp;
					j += 6;
				}

				DNSTRS5_8_13 += ": type ";
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_13 += " A, class ";
					bType = 1;
					j += 4;
				}
				else if (CStringToHex(savedata, 108 + j, 2) == 5)
				{
					DNSTRS5_8_13 += " CNAME, class";
					bType = 0;
					j += 4;
				}
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_13 += " IN";
					j += 12;
				}
				if (bType)		// *** type A
				{
					j += 2;
					DNSTRS5_8_13 += " addr ";
					dnstmp.Format("%d.%d.%d.%d", CStringToHex(savedata, 108 + j, 2), CStringToHex(savedata, 108 + j + 2, 2)
						, CStringToHex(savedata, 108 + j + 4, 2), CStringToHex(savedata, 108 + j + 6, 2));
					DNSTRS5_8_13 += dnstmp;
					DNSTRS5_8_14 = nametmp;
					j += 14;
				}
				else if (!bType)	// *** type CNAME
				{
					int cnamelength = CStringToHex(savedata, 108 + j, 2);
					j += 2;
					DNSTRS5_8_13 += " cname ";
					nametmp = "";
					for (int k = 0; k < cnamelength; k++)
					{
						if (k == (cnamelength - 2) && (CStringToHex(savedata, 108 + j, 2) == 0xc0))
						{
							j += 2;
							int idx = CStringToHex(savedata, 108 + j, 2);
							int o = 0;
							while (CStringToHex(savedata, 84 + (idx * 2) + o, 2) != 0)
							{
								cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 84 + (idx * 2) + o, 2)));
								DNSTRS5_8_13 += cnametmp;
								nametmp += cnametmp;
								o += 2;
							}
							j += 2;
							break;
						}
						cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 108 + j, 2)));
						DNSTRS5_8_13 += cnametmp;
						nametmp += cnametmp;
						j += 2;
					}
					DNSTRS5_8_14 = nametmp;
					j += 6;
				}

				DNSTRS5_8_14 += ": type ";
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_14 += " A, class ";
					bType = 1;
					j += 4;
				}
				else if (CStringToHex(savedata, 108 + j, 2) == 5)
				{
					DNSTRS5_8_14 += " CNAME, class";
					bType = 0;
					j += 4;
				}
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_14 += " IN";
					j += 12;
				}
				if (bType)		// *** type A
				{
					j += 2;
					DNSTRS5_8_14 += " addr ";
					dnstmp.Format("%d.%d.%d.%d", CStringToHex(savedata, 108 + j, 2), CStringToHex(savedata, 108 + j + 2, 2)
						, CStringToHex(savedata, 108 + j + 4, 2), CStringToHex(savedata, 108 + j + 6, 2));
					DNSTRS5_8_14 += dnstmp;
					DNSTRS5_8_15 = nametmp;
					j += 14;
				}
				else if (!bType)	// *** type CNAME
				{
					int cnamelength = CStringToHex(savedata, 108 + j, 2);
					j += 2;
					DNSTRS5_8_14 += " cname ";
					nametmp = "";
					for (int k = 0; k < cnamelength; k++)
					{
						if (k == (cnamelength - 2) && (CStringToHex(savedata, 108 + j, 2) == 0xc0))
						{
							j += 2;
							int idx = CStringToHex(savedata, 108 + j, 2);
							int o = 0;
							while (CStringToHex(savedata, 84 + (idx * 2) + o, 2) != 0)
							{
								cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 84 + (idx * 2) + o, 2)));
								DNSTRS5_8_14 += cnametmp;
								nametmp += cnametmp;
								o += 2;
							}
							j += 2;
							break;
						}
						cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 108 + j, 2)));
						DNSTRS5_8_14 += cnametmp;
						nametmp += cnametmp;
						j += 2;
					}
					DNSTRS5_8_15 = nametmp;
					j += 6;
				}

				DNSTRS5_8_15 += ": type ";
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_15 += " A, class ";
					bType = 1;
					j += 4;
				}
				else if (CStringToHex(savedata, 108 + j, 2) == 5)
				{
					DNSTRS5_8_15 += " CNAME, class";
					bType = 0;
					j += 4;
				}
				if (CStringToHex(savedata, 108 + j, 2) == 1)
				{
					DNSTRS5_8_15 += " IN";
					j += 12;
				}
				if (bType)		// *** type A
				{
					j += 2;
					DNSTRS5_8_15 += " addr ";
					dnstmp.Format("%d.%d.%d.%d", CStringToHex(savedata, 108 + j, 2), CStringToHex(savedata, 108 + j + 2, 2)
						, CStringToHex(savedata, 108 + j + 4, 2), CStringToHex(savedata, 108 + j + 6, 2));
					DNSTRS5_8_15 += dnstmp;
					//DNSTRS5_8_16 = nametmp;
					j += 14;
				}
				else if (!bType)	// *** type CNAME
				{
					int cnamelength = CStringToHex(savedata, 108 + j, 2);
					j += 2;
					DNSTRS5_8_15 += " cname ";
					nametmp = "";
					for (int k = 0; k < cnamelength; k++)
					{
						if (k == (cnamelength - 2) && (CStringToHex(savedata, 108 + j, 2) == 0xc0))
						{
							j += 2;
							int idx = CStringToHex(savedata, 108 + j, 2);
							int o = 0;
							while (CStringToHex(savedata, 84 + (idx * 2) + o, 2) != 0)
							{
								cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 84 + (idx * 2) + o, 2)));
								DNSTRS5_8_15 += cnametmp;
								nametmp += cnametmp;
								o += 2;
							}
							j += 2;
							break;
						}
						cnametmp.Format("%c", CheckAscii(CStringToHex(savedata, 108 + j, 2)));
						DNSTRS5_8_15 += cnametmp;
						nametmp += cnametmp;
						j += 2;
					}
					//DNSTRS5_8_16 = nametmp;
					j += 6;
				}
			}
		}

		TR1 = m_PacketInfoTree.InsertItem(TRS1, 0, 0, TVI_ROOT, TVI_LAST);
		TR1_1 = m_PacketInfoTree.InsertItem(TRS1_1, 0, 0, TR1, TVI_LAST);
		TR1_2 = m_PacketInfoTree.InsertItem(TRS1_2, 0, 0, TR1, TVI_LAST);
		TR1_3 = m_PacketInfoTree.InsertItem(TRS1_3, 0, 0, TR1, TVI_LAST);
		TR1_4 = m_PacketInfoTree.InsertItem(TRS1_4, 0, 0, TR1, TVI_LAST);
		TR1_5 = m_PacketInfoTree.InsertItem(TRS1_5, 0, 0, TR1, TVI_LAST);
		TR1_6 = m_PacketInfoTree.InsertItem(TRS1_6, 0, 0, TR1, TVI_LAST);

		TR2 = m_PacketInfoTree.InsertItem(TRS2, 0, 0, TVI_ROOT, TVI_LAST);
		TR2_1 = m_PacketInfoTree.InsertItem(TRS2_1, 0, 0, TR2, TVI_LAST);
		TR2_2 = m_PacketInfoTree.InsertItem(TRS2_2, 0, 0, TR2, TVI_LAST);
		TR2_3 = m_PacketInfoTree.InsertItem(TRS2_3, 0, 0, TR2, TVI_LAST);

		TR3 = m_PacketInfoTree.InsertItem(TRS3, 0, 0, TVI_ROOT, TVI_LAST);
		TR3_1 = m_PacketInfoTree.InsertItem(TRS3_1, 0, 0, TR3, TVI_LAST);
		TR3_2 = m_PacketInfoTree.InsertItem(TRS3_2, 0, 0, TR3, TVI_LAST);
		TR3_3 = m_PacketInfoTree.InsertItem(TRS3_3, 0, 0, TR3, TVI_LAST);
		TR3_4 = m_PacketInfoTree.InsertItem(TRS3_4, 0, 0, TR3, TVI_LAST);
		TR3_5 = m_PacketInfoTree.InsertItem(TRS3_5, 0, 0, TR3, TVI_LAST);
		TR3_6 = m_PacketInfoTree.InsertItem(TRS3_6, 0, 0, TR3, TVI_LAST);
		TR3_6_1 = m_PacketInfoTree.InsertItem(TRS3_6_1, 0, 0, TR3_6, TVI_LAST);
		TR3_6_2 = m_PacketInfoTree.InsertItem(TRS3_6_2, 0, 0, TR3_6, TVI_LAST);
		TR3_6_3 = m_PacketInfoTree.InsertItem(TRS3_6_3, 0, 0, TR3_6, TVI_LAST);

		TR3_7 = m_PacketInfoTree.InsertItem(TRS3_7, 0, 0, TR3, TVI_LAST);
		TR3_8 = m_PacketInfoTree.InsertItem(TRS3_8, 0, 0, TR3, TVI_LAST);
		TR3_9 = m_PacketInfoTree.InsertItem(TRS3_9, 0, 0, TR3, TVI_LAST);
		TR3_10 = m_PacketInfoTree.InsertItem(TRS3_10, 0, 0, TR3, TVI_LAST);
		TR3_11 = m_PacketInfoTree.InsertItem(TRS3_11, 0, 0, TR3, TVI_LAST);
		TR3_12 = m_PacketInfoTree.InsertItem(TRS3_12, 0, 0, TR3, TVI_LAST);

		if (!protocol.Compare("TCP") || (protocol.Find(_T("TLS")) != -1))
		{
			TCPTR4 = m_PacketInfoTree.InsertItem(TCPTRS4, 0, 0, TVI_ROOT, TVI_LAST);
			TCPTR4_1 = m_PacketInfoTree.InsertItem(TCPTRS4_1, 0, 0, TCPTR4, TVI_LAST);
			TCPTR4_2 = m_PacketInfoTree.InsertItem(TCPTRS4_2, 0, 0, TCPTR4, TVI_LAST);
			TCPTR4_3 = m_PacketInfoTree.InsertItem(TCPTRS4_3, 0, 0, TCPTR4, TVI_LAST);
			TCPTR4_4 = m_PacketInfoTree.InsertItem(TCPTRS4_4, 0, 0, TCPTR4, TVI_LAST);
			TCPTR4_5 = m_PacketInfoTree.InsertItem(TCPTRS4_5, 0, 0, TCPTR4, TVI_LAST);
			TCPTR4_6 = m_PacketInfoTree.InsertItem(TCPTRS4_6, 0, 0, TCPTR4, TVI_LAST);
			TCPTR4_6_1 = m_PacketInfoTree.InsertItem(TCPTRS4_6_1, 0, 0, TCPTR4_6, TVI_LAST);
			TCPTR4_6_2 = m_PacketInfoTree.InsertItem(TCPTRS4_6_2, 0, 0, TCPTR4_6, TVI_LAST);
			TCPTR4_6_3 = m_PacketInfoTree.InsertItem(TCPTRS4_6_3, 0, 0, TCPTR4_6, TVI_LAST);
			TCPTR4_6_4 = m_PacketInfoTree.InsertItem(TCPTRS4_6_4, 0, 0, TCPTR4_6, TVI_LAST);
			TCPTR4_6_5 = m_PacketInfoTree.InsertItem(TCPTRS4_6_5, 0, 0, TCPTR4_6, TVI_LAST);
			TCPTR4_6_6 = m_PacketInfoTree.InsertItem(TCPTRS4_6_6, 0, 0, TCPTR4_6, TVI_LAST);
			TCPTR4_6_7 = m_PacketInfoTree.InsertItem(TCPTRS4_6_7, 0, 0, TCPTR4_6, TVI_LAST);
			TCPTR4_6_8 = m_PacketInfoTree.InsertItem(TCPTRS4_6_8, 0, 0, TCPTR4_6, TVI_LAST);
			TCPTR4_6_9 = m_PacketInfoTree.InsertItem(TCPTRS4_6_9, 0, 0, TCPTR4_6, TVI_LAST);
			TCPTR4_6_10 = m_PacketInfoTree.InsertItem(TCPTRS4_6_10, 0, 0, TCPTR4_6, TVI_LAST);
			TCPTR4_7 = m_PacketInfoTree.InsertItem(TCPTRS4_7, 0, 0, TCPTR4, TVI_LAST);
			TCPTR4_8 = m_PacketInfoTree.InsertItem(TCPTRS4_8, 0, 0, TCPTR4, TVI_LAST);
			TCPTR4_9 = m_PacketInfoTree.InsertItem(TCPTRS4_9, 0, 0, TCPTR4, TVI_LAST);
			if ((protocol.Find(_T("TLS")) != -1))
			{
				TLSTR5 = m_PacketInfoTree.InsertItem(TLSTRS5, 0, 0, TVI_ROOT, TVI_LAST);
				if (!(CStringToHex(savedata, 108, 2) >= 20 && CStringToHex(savedata, 108, 2) <= 26))
				{
					TLSTR5_TMP = m_PacketInfoTree.InsertItem(TLSTRS5_TMP, 0, 0, TLSTR5, TVI_LAST);
				}
				TLSTR5_1 = m_PacketInfoTree.InsertItem(TLSTRS5_1, 0, 0, TLSTR5, TVI_LAST);
				TLSTR5_2 = m_PacketInfoTree.InsertItem(TLSTRS5_2, 0, 0, TLSTR5, TVI_LAST);
				TLSTR5_3 = m_PacketInfoTree.InsertItem(TLSTRS5_3, 0, 0, TLSTR5, TVI_LAST);

				// *** COntent Type 이 Handshake 라면
				if (CStringToHex(savedata, 108, 2) == 22)
				{
					TLSTR5_4 = m_PacketInfoTree.InsertItem(TLSTRS5_4, 0, 0, TLSTR5, TVI_LAST);
					TLSTR5_4_1 = m_PacketInfoTree.InsertItem(TLSTRS5_4_1, 0, 0, TLSTR5_4, TVI_LAST);
					TLSTR5_4_2 = m_PacketInfoTree.InsertItem(TLSTRS5_4_2, 0, 0, TLSTR5_4, TVI_LAST);
					TLSTR5_4_3 = m_PacketInfoTree.InsertItem(TLSTRS5_4_3, 0, 0, TLSTR5_4, TVI_LAST);
				}
				if (tlscnt >= 1)
				{
					TLSTR5_5 = m_PacketInfoTree.InsertItem(TLSTRS5_5, 0, 0, TLSTR5, TVI_LAST);
					TLSTR5_6 = m_PacketInfoTree.InsertItem(TLSTRS5_6, 0, 0, TLSTR5, TVI_LAST);
					TLSTR5_7 = m_PacketInfoTree.InsertItem(TLSTRS5_7, 0, 0, TLSTR5, TVI_LAST);
					if (TLSTRS5_5_1.IsEmpty() != TRUE)
					{
						TLSTR5_5_1 = m_PacketInfoTree.InsertItem(TLSTRS5_5_1, 0, 0, TLSTR5, TVI_LAST);
						TLSTR5_5_2 = m_PacketInfoTree.InsertItem(TLSTRS5_5_2, 0, 0, TLSTR5_5_1, TVI_LAST);
						TLSTR5_5_3 = m_PacketInfoTree.InsertItem(TLSTRS5_5_3, 0, 0, TLSTR5_5_1, TVI_LAST);
						TLSTR5_5_4 = m_PacketInfoTree.InsertItem(TLSTRS5_5_4, 0, 0, TLSTR5_5_1, TVI_LAST);
					}
				}
				if (tlscnt >= 2)
				{
					TLSTR5_8 = m_PacketInfoTree.InsertItem(TLSTRS5_8, 0, 0, TLSTR5, TVI_LAST);
					TLSTR5_9 = m_PacketInfoTree.InsertItem(TLSTRS5_9, 0, 0, TLSTR5, TVI_LAST);
					TLSTR5_10 = m_PacketInfoTree.InsertItem(TLSTRS5_10, 0, 0, TLSTR5, TVI_LAST);

					if (TLSTRS5_6_1.IsEmpty() != TRUE)
					{
						TLSTR5_6_1 = m_PacketInfoTree.InsertItem(TLSTRS5_6_1, 0, 0, TLSTR5, TVI_LAST);
						TLSTR5_6_2 = m_PacketInfoTree.InsertItem(TLSTRS5_6_2, 0, 0, TLSTR5_6_1, TVI_LAST);
						TLSTR5_6_3 = m_PacketInfoTree.InsertItem(TLSTRS5_6_3, 0, 0, TLSTR5_6_1, TVI_LAST);
						TLSTR5_6_4 = m_PacketInfoTree.InsertItem(TLSTRS5_6_4, 0, 0, TLSTR5_6_1, TVI_LAST);
					}
				}
				if (tlscnt >= 3)
				{
					TLSTR5_11 = m_PacketInfoTree.InsertItem(TLSTRS5_11, 0, 0, TLSTR5, TVI_LAST);
					TLSTR5_12 = m_PacketInfoTree.InsertItem(TLSTRS5_12, 0, 0, TLSTR5, TVI_LAST);
					TLSTR5_13 = m_PacketInfoTree.InsertItem(TLSTRS5_13, 0, 0, TLSTR5, TVI_LAST);

					if (TLSTRS5_7_1.IsEmpty() != TRUE)
					{
						TLSTR5_7_1 = m_PacketInfoTree.InsertItem(TLSTRS5_7_1, 0, 0, TLSTR5, TVI_LAST);
						TLSTR5_7_2 = m_PacketInfoTree.InsertItem(TLSTRS5_7_2, 0, 0, TLSTR5_7_1, TVI_LAST);
						TLSTR5_7_3 = m_PacketInfoTree.InsertItem(TLSTRS5_7_3, 0, 0, TLSTR5_7_1, TVI_LAST);
						TLSTR5_7_4 = m_PacketInfoTree.InsertItem(TLSTRS5_7_4, 0, 0, TLSTR5_7_1, TVI_LAST);
					}
				}
			}
			// *** IP Header에 Total Length가 40이상이면 TCP Payload 존재
			if (CStringToHex(savedata, 32, 4) > 40)
			{
				TCPTR4_10 = m_PacketInfoTree.InsertItem(TCPTRS4_10, 0, 0, TCPTR4, TVI_LAST);
			}
			
		}

		else if (!protocol.Compare("UDP") || !protocol.Compare("SSDP") || !protocol.Compare("DNS"))
		{
			UDPTR4 = m_PacketInfoTree.InsertItem(UDPTRS4, 0, 0, TVI_ROOT, TVI_LAST);
			UDPTR4_1 = m_PacketInfoTree.InsertItem(UDPTRS4_1, 0, 0, UDPTR4, TVI_LAST);
			UDPTR4_2 = m_PacketInfoTree.InsertItem(UDPTRS4_2, 0, 0, UDPTR4, TVI_LAST);
			UDPTR4_3 = m_PacketInfoTree.InsertItem(UDPTRS4_3, 0, 0, UDPTR4, TVI_LAST);
			UDPTR4_4 = m_PacketInfoTree.InsertItem(UDPTRS4_4, 0, 0, UDPTR4, TVI_LAST);
			UDPTR4_5 = m_PacketInfoTree.InsertItem(UDPTRS4_5, 0, 0, UDPTR4, TVI_LAST);
			/*
			https://www.minzkn.com/moniwiki/wiki.php/SimpleServiceDiscoveryProtocol
			위 사이트 참고

			검색요청: Multicast 주소 239.255.255.250:1900 으로 하기포맷에 맞춰서 UDP를 송출하면 해당 송신포트로 UPnP 장비로부터 응답을 받게 됩니다.
			M-SEARCH * HTTP/1.1\r\n
			HOST: 239.255.255.250:1900\r\n
			MAN: "ssdp:discover"\r\n
			MX: <호스트로부터 응답에 응할수 있는 초단위의 최대 시간을 지정합니다. 일반적으로 3초 이상을 권장합니다.>\r\n
			ST: <검색에 대응하는 서비스를 URN형식으로 지정합니다.>\r\n
			USER-AGENT: <OS>/<version> UPnP/1.1 <product>/<version>\r\n
			\r\n

			검색요청(M-SEARCH) 예시
			M-SEARCH * HTTP/1.1\r\n
			HOST: 239.255.255.250:1900\r\n
			MAN: "ssdp:discover"\r\n
			MX: 3\r\n
			ST: ssdp:all\r\n
			USER-AGENT: Linux/3.0.0 UPnP/1.1 HWPORT-CLIENT/1.0.0\r\n
			\r\n

			전파(NOTIFY) 예시
			NOTIFY * HTTP/1.1\r\n
			HOST: 239.255.255.250:1900\r\n
			CACHE-CONTROL: max-age=120\r\n
			DATE: Wed, 24 Aug 2016 10:58:00 GMT\r\n
			LOCATION: http://192.168.13.110:8888/descriptor.xml\r\n
			SERVER: Linux/2.6.29.6 UPnP/1.1 mzloader/5.0.1.3\r\n
			NT: urn:schemas-upnp-org:device:MediaServer:1\r\n
			USN: uuid:2cc2176e-699e-11e6-ef59-ec438b0186a6::urn:schemas-upnp-org:device:MediaServer:1\r\n
			NTS: ssdp:alive\r\n
			\r\n

			응답(M-SEARCH Reply) 형식
			HTTP/1.1 200 OK\r\n
			CACHE-CONTROL: max-age = <호스트의 정보가 유효할수 있는 최대 초단위의 시간이며 172800 보다 크게 지정하도록 한다.>\r\n
			DATE: <호스트가 응답할때 시점의 시간>\r\n
			EXT: \r\n
			LOCATION: <자세한 정보를 제공하는 HTTP접속주소>\r\n
			SERVER: <OS>/<version> UPnP/1.1 <product>/<version>\r\n
			ST: <요청받았던 ST내용>\r\n
			USN: <호스트의 서비스에 대응하는 UUID와 ST의 조합>\r\n
			\r\n

			응답(M-SEARCH Reply) 예시
			HTTP/1.1 200 OK\r\n
			CACHE-CONTROL: max-age=172800\r\n
			DATE: Wed Jul 15 10:10:00 2014 GMT\r\n
			EXT: \r\n
			LOCATION: http://192.168.13.110:80/upnp/descriptor?target=this.xml\r\n
			SERVER: Linux/3.0.0-r1 UPnP/1.1 HWPORT-DEVICE/1.0.0\r\n
			ST: upnp:rootdevice\r\n
			USN: uuid:862c2cba-65a7-11e4-ad55-000c293e0367::upnp:rootdevice\r\n
			\r\n
			*/
			if (!protocol.Compare("SSDP"))
			{
				SSDPTR5 = m_PacketInfoTree.InsertItem(SSDPTRS5, 0, 0, TVI_ROOT, TVI_LAST);
				SSDPTR5_1 = m_PacketInfoTree.InsertItem(SSDPTRS5_1, 0, 0, SSDPTR5, TVI_LAST);
				SSDPTR5_2 = m_PacketInfoTree.InsertItem(SSDPTRS5_2, 0, 0, SSDPTR5, TVI_LAST);
				SSDPTR5_3 = m_PacketInfoTree.InsertItem(SSDPTRS5_3, 0, 0, SSDPTR5, TVI_LAST);
				SSDPTR5_4 = m_PacketInfoTree.InsertItem(SSDPTRS5_4, 0, 0, SSDPTR5, TVI_LAST);
				SSDPTR5_5 = m_PacketInfoTree.InsertItem(SSDPTRS5_5, 0, 0, SSDPTR5, TVI_LAST);
				if (SSDPTRS5_6.IsEmpty() != TRUE)
				{
					SSDPTR5_6 = m_PacketInfoTree.InsertItem(SSDPTRS5_6, 0, 0, SSDPTR5, TVI_LAST);
				}
				if (SSDPTRS5_7.IsEmpty() != TRUE)
				{
					SSDPTR5_7 = m_PacketInfoTree.InsertItem(SSDPTRS5_7, 0, 0, SSDPTR5, TVI_LAST);
				}
				if (SSDPTRS5_8.IsEmpty() != TRUE)
				{
					SSDPTR5_8 = m_PacketInfoTree.InsertItem(SSDPTRS5_8, 0, 0, SSDPTR5, TVI_LAST);
				}
				if (SSDPTRS5_9.IsEmpty() != TRUE)
				{
					SSDPTR5_9 = m_PacketInfoTree.InsertItem(SSDPTRS5_9, 0, 0, SSDPTR5, TVI_LAST);
				}
			}
			else if(!protocol.Compare("DNS"))
			{
				DNSTR5 = m_PacketInfoTree.InsertItem(DNSTRS5, 0, 0, TVI_ROOT, TVI_LAST);
				DNSTR5_1 = m_PacketInfoTree.InsertItem(DNSTRS5_1, 0, 0, DNSTR5, TVI_LAST);
				DNSTR5_2 = m_PacketInfoTree.InsertItem(DNSTRS5_2, 0, 0, DNSTR5, TVI_LAST);
				DNSTR5_2_1 = m_PacketInfoTree.InsertItem(DNSTRS5_2_1, 0, 0, DNSTR5_2, TVI_LAST);
				DNSTR5_2_2 = m_PacketInfoTree.InsertItem(DNSTRS5_2_2, 0, 0, DNSTR5_2, TVI_LAST);
				if((CStringToHex(savedata, 88, 1)>>3) > 0)DNSTR5_2_3 = m_PacketInfoTree.InsertItem(DNSTRS5_2_3, 0, 0, DNSTR5_2, TVI_LAST);
				DNSTR5_2_4 = m_PacketInfoTree.InsertItem(DNSTRS5_2_4, 0, 0, DNSTR5_2, TVI_LAST);
				DNSTR5_2_5 = m_PacketInfoTree.InsertItem(DNSTRS5_2_5, 0, 0, DNSTR5_2, TVI_LAST);
				if ((CStringToHex(savedata, 88, 1) >> 3) > 0)DNSTR5_2_6 = m_PacketInfoTree.InsertItem(DNSTRS5_2_6, 0, 0, DNSTR5_2, TVI_LAST);
				DNSTR5_2_7 = m_PacketInfoTree.InsertItem(DNSTRS5_2_7, 0, 0, DNSTR5_2, TVI_LAST);
				if ((CStringToHex(savedata, 88, 1) >> 3) > 0)DNSTR5_2_8 = m_PacketInfoTree.InsertItem(DNSTRS5_2_8, 0, 0, DNSTR5_2, TVI_LAST);
				DNSTR5_2_9 = m_PacketInfoTree.InsertItem(DNSTRS5_2_9, 0, 0, DNSTR5_2, TVI_LAST);
				if ((CStringToHex(savedata, 88, 1) >> 3) > 0)DNSTR5_2_10 = m_PacketInfoTree.InsertItem(DNSTRS5_2_10, 0, 0, DNSTR5_2, TVI_LAST);

				DNSTR5_3 = m_PacketInfoTree.InsertItem(DNSTRS5_3, 0, 0, DNSTR5, TVI_LAST);
				DNSTR5_4 = m_PacketInfoTree.InsertItem(DNSTRS5_4, 0, 0, DNSTR5, TVI_LAST);
				DNSTR5_5 = m_PacketInfoTree.InsertItem(DNSTRS5_5, 0, 0, DNSTR5, TVI_LAST);
				DNSTR5_6 = m_PacketInfoTree.InsertItem(DNSTRS5_6, 0, 0, DNSTR5, TVI_LAST);
				DNSTR5_7 = m_PacketInfoTree.InsertItem(DNSTRS5_7, 0, 0, DNSTR5, TVI_LAST);
				DNSTR5_7_1 = m_PacketInfoTree.InsertItem(DNSTRS5_7_1, 0, 0, DNSTR5_7, TVI_LAST);
				DNSTR5_8 = m_PacketInfoTree.InsertItem(DNSTRS5_8, 0, 0, DNSTR5, TVI_LAST);
				// *** answer이 있다면
				if (dnsanswcnt > 0)
				{
					DNSTR5_8_1 = m_PacketInfoTree.InsertItem(DNSTRS5_8_1, 0, 0, DNSTR5_8, TVI_LAST);
					if(dnsanswcnt >= 2) DNSTR5_8_2 = m_PacketInfoTree.InsertItem(DNSTRS5_8_2, 0, 0, DNSTR5_8, TVI_LAST);
					if(dnsanswcnt >= 3) DNSTR5_8_3 = m_PacketInfoTree.InsertItem(DNSTRS5_8_3, 0, 0, DNSTR5_8, TVI_LAST);
					if (dnsanswcnt >= 4) DNSTR5_8_4 = m_PacketInfoTree.InsertItem(DNSTRS5_8_4, 0, 0, DNSTR5_8, TVI_LAST);
					if (dnsanswcnt >= 5) DNSTR5_8_5 = m_PacketInfoTree.InsertItem(DNSTRS5_8_5, 0, 0, DNSTR5_8, TVI_LAST);
					if (dnsanswcnt >= 6) DNSTR5_8_6 = m_PacketInfoTree.InsertItem(DNSTRS5_8_6, 0, 0, DNSTR5_8, TVI_LAST);
					if (dnsanswcnt >= 7) DNSTR5_8_7 = m_PacketInfoTree.InsertItem(DNSTRS5_8_7, 0, 0, DNSTR5_8, TVI_LAST);
					if (dnsanswcnt >= 8) DNSTR5_8_8 = m_PacketInfoTree.InsertItem(DNSTRS5_8_8, 0, 0, DNSTR5_8, TVI_LAST);
					if (dnsanswcnt >= 9) DNSTR5_8_9 = m_PacketInfoTree.InsertItem(DNSTRS5_8_9, 0, 0, DNSTR5_8, TVI_LAST);
					if (dnsanswcnt >= 10) DNSTR5_8_10 = m_PacketInfoTree.InsertItem(DNSTRS5_8_10, 0, 0, DNSTR5_8, TVI_LAST);
					if (dnsanswcnt >= 11) DNSTR5_8_11 = m_PacketInfoTree.InsertItem(DNSTRS5_8_11, 0, 0, DNSTR5_8, TVI_LAST);
					if (dnsanswcnt >= 12) DNSTR5_8_12 = m_PacketInfoTree.InsertItem(DNSTRS5_8_12, 0, 0, DNSTR5_8, TVI_LAST);
					if (dnsanswcnt >= 13) DNSTR5_8_13 = m_PacketInfoTree.InsertItem(DNSTRS5_8_13, 0, 0, DNSTR5_8, TVI_LAST);
					if (dnsanswcnt >= 14) DNSTR5_8_14 = m_PacketInfoTree.InsertItem(DNSTRS5_8_14, 0, 0, DNSTR5_8, TVI_LAST);
					if (dnsanswcnt >= 15) DNSTR5_8_15 = m_PacketInfoTree.InsertItem(DNSTRS5_8_15, 0, 0, DNSTR5_8, TVI_LAST);

					m_PacketInfoTree.Expand(DNSTR5, TVE_EXPAND);
					m_PacketInfoTree.Expand(DNSTR5_8, TVE_EXPAND);
				}
			}
		}
	}
	if (!protocol.Compare("ARP"))//(!protocol.Compare("ARP"))
	{
		TRS2_3.Format("Type: ARP (0x%04X)", type);

		TR1 = m_PacketInfoTree.InsertItem(TRS1, 0, 0, TVI_ROOT, TVI_LAST);
		TR1_1 = m_PacketInfoTree.InsertItem(TRS1_1, 0, 0, TR1, TVI_LAST);
		TR1_2 = m_PacketInfoTree.InsertItem(TRS1_2, 0, 0, TR1, TVI_LAST);
		TR1_3 = m_PacketInfoTree.InsertItem(TRS1_3, 0, 0, TR1, TVI_LAST);
		TR1_4 = m_PacketInfoTree.InsertItem(TRS1_4, 0, 0, TR1, TVI_LAST);
		TR1_5 = m_PacketInfoTree.InsertItem(TRS1_5, 0, 0, TR1, TVI_LAST);
		TR1_6 = m_PacketInfoTree.InsertItem(TRS1_6, 0, 0, TR1, TVI_LAST);

		TR2 = m_PacketInfoTree.InsertItem(TRS2, 0, 0, TVI_ROOT, TVI_LAST);
		TR2_1 = m_PacketInfoTree.InsertItem(TRS2_1, 0, 0, TR2, TVI_LAST);
		TR2_2 = m_PacketInfoTree.InsertItem(TRS2_2, 0, 0, TR2, TVI_LAST);
		TR2_3 = m_PacketInfoTree.InsertItem(TRS2_3, 0, 0, TR2, TVI_LAST);

		ARPTRS3.Format("Address Resolution Protocol (%s)", CStringToHex(savedata, 40, 4) == 2 ? "Reply" : "Request");
		ARPTRS3_1.Format("Hardware type : %s", CStringToHex(savedata, 28, 4) == 1 ? "Ethernet (1)" : "(2)");
		ARPTRS3_2.Format("Protocol type: %s", CStringToHex(savedata, 32, 4) == 0x800 ? "IPv4 (0x0800)" : "Noy IPv4");
		ARPTRS3_3.Format("Hardware size: %d", CStringToHex(savedata, 36, 2));
		ARPTRS3_4.Format("Protocol size: %d", CStringToHex(savedata, 38, 2));
		ARPTRS3_5.Format("Opcode: %s (%d)", CStringToHex(savedata, 40, 4) == 2 ? "Reply" : "Request", CStringToHex(savedata, 40, 4));
		ARPTRS3_6.Format("Sender MAC address: (%02X:%02X:%02X:%02X:%02X:%02X)", CStringToHex(savedata, 44, 2), CStringToHex(savedata, 46, 2), CStringToHex(savedata, 48, 2),
																				CStringToHex(savedata, 50, 2), CStringToHex(savedata, 52,2), CStringToHex(savedata, 54, 2));
		ARPTRS3_7.Format("Sender IP address: %d.%d.%d.%d" ,CStringToHex(savedata, 56, 2), CStringToHex(savedata, 58, 2), CStringToHex(savedata, 60, 2), CStringToHex(savedata, 62, 2));
		ARPTRS3_8.Format("Target MAC address: (%02X:%02X:%02X:%02X:%02X:%02X)", CStringToHex(savedata, 64, 2), CStringToHex(savedata, 66, 2), CStringToHex(savedata, 68, 2),
																				CStringToHex(savedata, 70, 2), CStringToHex(savedata, 72, 2), CStringToHex(savedata, 74, 2));
		
		ARPTRS3_9.Format("Target IP address: %d.%d.%d.%d", CStringToHex(savedata, 76, 2), CStringToHex(savedata, 78, 2), CStringToHex(savedata, 80, 2), CStringToHex(savedata, 82, 2));

		ARPTR3 = m_PacketInfoTree.InsertItem(ARPTRS3, 0, 0, TVI_ROOT, TVI_LAST);
		ARPTR3_1 = m_PacketInfoTree.InsertItem(ARPTRS3_1, 0, 0, ARPTR3, TVI_LAST);
		ARPTR3_2 = m_PacketInfoTree.InsertItem(ARPTRS3_2, 0, 0, ARPTR3, TVI_LAST);
		ARPTR3_3 = m_PacketInfoTree.InsertItem(ARPTRS3_3, 0, 0, ARPTR3, TVI_LAST);
		ARPTR3_4 = m_PacketInfoTree.InsertItem(ARPTRS3_4, 0, 0, ARPTR3, TVI_LAST);
		ARPTR3_5 = m_PacketInfoTree.InsertItem(ARPTRS3_5, 0, 0, ARPTR3, TVI_LAST);
		ARPTR3_6 = m_PacketInfoTree.InsertItem(ARPTRS3_6, 0, 0, ARPTR3, TVI_LAST);
		ARPTR3_7 = m_PacketInfoTree.InsertItem(ARPTRS3_7, 0, 0, ARPTR3, TVI_LAST);
		ARPTR3_8 = m_PacketInfoTree.InsertItem(ARPTRS3_8, 0, 0, ARPTR3, TVI_LAST);
		ARPTR3_9 = m_PacketInfoTree.InsertItem(ARPTRS3_9, 0, 0, ARPTR3, TVI_LAST);
	}
	// *** 트리 오픈
	//m_PacketInfoTree.Expand(TR1, TVE_EXPAND);
	//m_PacketInfoTree.Expand(TR2, TVE_EXPAND);
	//m_PacketInfoTree.Expand(TR3, TVE_EXPAND);
	m_PacketInfoTree.Expand(TLSTR5, TVE_EXPAND);
	//m_PacketInfoTree.Expand(UDPTR4, TVE_EXPAND);
	
	// *** 메모리 초기화
	savedata.Empty();
	return 0;
}

// *** Control List Sort 처리 함수
int CNetworkPacketCaptureDlg::CompareItem(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrl* pList = ((SORTPARAM*)lParamSort)->pList;
	int iSortColumn = ((SORTPARAM*)lParamSort)->iSortColumn;
	bool bSortDirect = ((SORTPARAM*)lParamSort)->bSortDirect;
	int flag = ((SORTPARAM*)lParamSort)->flag;

	LVFINDINFO info1, info2;
	info1.flags = LVFI_PARAM;
	info1.lParam = lParam1;

	info2.flags = LVFI_PARAM;
	info2.lParam = lParam2;

	int irow1 = pList->FindItem(&info1, -1);
	int irow2 = pList->FindItem(&info2, -1);

	CString strItem1 = pList->GetItemText(irow1, iSortColumn);
	CString strItem2 = pList->GetItemText(irow2, iSortColumn);

	if (flag == flag)
	{
		int iItem1 = _tstoi(strItem1);
		int iItem2 = _tstoi(strItem2);

		if (bSortDirect) {
			return iItem1 == iItem2 ? 0 : iItem1 > iItem2;
		}
		else {
			return iItem1 == iItem2 ? 0 : iItem1 < iItem2;
		}
	}
	else {
		//return AfxMessageBox("정렬할 수 없습니다!");
	}
}

// *** 헥스 컨트롤 리스트 세팅
int CNetworkPacketCaptureDlg::SetPacketHexList(CString data, CString protocol, int udpsize)
{
	m_PacketDataControlList.DeleteAllItems();
	CString HexTemp, StrTemp;
	int listidx = 0; 
	m_PacketDataControlList.InsertItem(listidx, "0", 0);
	m_PacketDataControlList.SetItemText(listidx, 1, "Destination MAC Address");
	HexTemp.Format("%02X %02X %02X %02X %02X %02X", CStringToHex(data ,0, 2), CStringToHex(data ,2, 2), CStringToHex(data ,4, 2),
										CStringToHex(data ,6, 2), CStringToHex(data ,8, 2), CStringToHex(data ,10, 2));
	m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
	StrTemp.Format("%c %c %c %c %c %c", IsAlpha(CStringToHex(data, 0, 2)) , IsAlpha(CStringToHex(data, 2, 2)), IsAlpha(CStringToHex(data, 4, 2)),
										IsAlpha(CStringToHex(data ,6, 2)), IsAlpha(CStringToHex(data,8, 2)), IsAlpha(CStringToHex(data ,10, 2)));
	m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

	m_PacketDataControlList.InsertItem(listidx, "1", 0);
	m_PacketDataControlList.SetItemText(listidx, 1, "Source MAC Address");
	HexTemp.Format("%02X %02X %02X %02X %02X %02X", CStringToHex(data ,12, 2), CStringToHex(data, 14, 2), CStringToHex(data, 16, 2),
										CStringToHex(data ,18, 2), CStringToHex(data ,20, 2), CStringToHex(data,22, 2));
	m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
	StrTemp.Format("%c %c %c %c %c %c", IsAlpha(CStringToHex(data ,12, 2)), IsAlpha(CStringToHex(data,14, 2)), IsAlpha(CStringToHex(data,16, 2)),
										IsAlpha(CStringToHex(data,18, 2)), IsAlpha(CStringToHex(data,20, 2)), IsAlpha(CStringToHex(data,22, 2)));
	m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

	m_PacketDataControlList.InsertItem(listidx, "2", 0);
	m_PacketDataControlList.SetItemText(listidx, 1, "Type");
	HexTemp.Format("%02X %02X", CStringToHex(data ,24,2), CStringToHex(data ,26,2));
	m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
	StrTemp.Format("%c %c", IsAlpha(CStringToHex(data ,24, 2)), IsAlpha(CStringToHex(data, 26, 2)));
	m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

	if (CStringToHex(data, 24, 4) == 0x806)
	{
		m_PacketDataControlList.InsertItem(listidx, "3", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Hardware type");
		HexTemp.Format("%02X %02X", CStringToHex(data, 28, 2), CStringToHex(data, 30, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 28, 2)), IsAlpha(CStringToHex(data, 30, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		m_PacketDataControlList.InsertItem(listidx, "4", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Protocol type");
		HexTemp.Format("%02X %02X", CStringToHex(data, 32, 2), CStringToHex(data, 34, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 32, 2)), IsAlpha(CStringToHex(data, 34, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		m_PacketDataControlList.InsertItem(listidx, "5", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Hardware size");
		HexTemp.Format("%02X", CStringToHex(data, 36, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c", IsAlpha(CStringToHex(data, 36, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		m_PacketDataControlList.InsertItem(listidx, "6", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Protocol size");
		HexTemp.Format("%02X", CStringToHex(data, 38, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c", IsAlpha(CStringToHex(data, 38, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		m_PacketDataControlList.InsertItem(listidx, "7", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Opcode");
		HexTemp.Format("%02X %02X", CStringToHex(data, 40, 2), CStringToHex(data, 42, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 40, 2)), IsAlpha(CStringToHex(data, 42, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		m_PacketDataControlList.InsertItem(listidx, "8", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Sender MAC address");
		HexTemp.Format("%02X %02X %02X %02X %02X %02X", CStringToHex(data, 44, 2), CStringToHex(data, 46, 2), CStringToHex(data, 48, 2), 
														CStringToHex(data, 50, 2), CStringToHex(data, 52, 2), CStringToHex(data, 54, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c %c %c %c %c %c", IsAlpha(CStringToHex(data, 44, 2)), IsAlpha(CStringToHex(data, 46, 2)), IsAlpha(CStringToHex(data, 48, 2)),
											IsAlpha(CStringToHex(data, 50, 2)), IsAlpha(CStringToHex(data, 52, 2)), IsAlpha(CStringToHex(data, 54, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		m_PacketDataControlList.InsertItem(listidx, "9", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Sender IP address");
		HexTemp.Format("%02X %02X %02X %02X", CStringToHex(data, 56, 2), CStringToHex(data, 58, 2), CStringToHex(data, 60, 2), CStringToHex(data, 62, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c %c %c %c", IsAlpha(CStringToHex(data, 56, 2)), IsAlpha(CStringToHex(data, 58, 2)), IsAlpha(CStringToHex(data, 60, 2)),
			IsAlpha(CStringToHex(data, 62, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		m_PacketDataControlList.InsertItem(listidx, "10", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Target MAC address");
		HexTemp.Format("%02X %02X %02X %02X %02X %02X", CStringToHex(data, 64, 2), CStringToHex(data, 66, 2), CStringToHex(data, 68, 2),
			CStringToHex(data, 70, 2), CStringToHex(data, 72, 2), CStringToHex(data, 74, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c %c %c %c %c %c", IsAlpha(CStringToHex(data, 64, 2)), IsAlpha(CStringToHex(data, 66, 2)), IsAlpha(CStringToHex(data, 68, 2)),
			IsAlpha(CStringToHex(data, 70, 2)), IsAlpha(CStringToHex(data, 72, 2)), IsAlpha(CStringToHex(data, 74, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		m_PacketDataControlList.InsertItem(listidx, "11", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Target IP address");
		HexTemp.Format("%02X %02X %02X %02X", CStringToHex(data, 76, 2), CStringToHex(data, 78, 2), CStringToHex(data, 80, 2), CStringToHex(data, 82, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c %c %c %c", IsAlpha(CStringToHex(data, 76, 2)), IsAlpha(CStringToHex(data, 78, 2)), IsAlpha(CStringToHex(data, 80, 2)),
			IsAlpha(CStringToHex(data, 82, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);
	}
	else if (CStringToHex(data, 24, 4) == 0x800)
	{
		m_PacketDataControlList.InsertItem(listidx, "3", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Version");
		HexTemp.Format("%X", CStringToHex(data, 28, 1));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c", IsAlpha(CStringToHex(data, 28, 1)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		m_PacketDataControlList.InsertItem(listidx, "4", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Header Length");
		HexTemp.Format("%X", CStringToHex(data, 29, 1));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c", IsAlpha(CStringToHex(data, 29, 1)));
		m_PacketDataControlList.SetItemText(listidx++, 3, HexTemp);

		m_PacketDataControlList.InsertItem(listidx, "5", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Differentiated Services Field");
		HexTemp.Format("%02X", CStringToHex(data, 30, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c", IsAlpha(CStringToHex(data, 30, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		m_PacketDataControlList.InsertItem(listidx, "6", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Total Length");
		HexTemp.Format("%02X %02X", CStringToHex(data, 32, 2), CStringToHex(data, 34, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 32, 2)), IsAlpha(CStringToHex(data, 34, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		m_PacketDataControlList.InsertItem(listidx, "7", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Identification");
		HexTemp.Format("%02X %02X", CStringToHex(data, 36, 2), CStringToHex(data, 38, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 36, 2)), IsAlpha(CStringToHex(data, 38, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		m_PacketDataControlList.InsertItem(listidx, "8", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Flags");
		HexTemp.Format("%02X %02X", CStringToHex(data, 40, 2), CStringToHex(data, 42, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 40, 2)), IsAlpha(CStringToHex(data, 42, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		m_PacketDataControlList.InsertItem(listidx, "9", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Time to Live");
		HexTemp.Format("%02X", CStringToHex(data, 44, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c", IsAlpha(CStringToHex(data, 44, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		m_PacketDataControlList.InsertItem(listidx, "10", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Protocol");
		HexTemp.Format("%02X", CStringToHex(data, 46, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c", IsAlpha(CStringToHex(data, 46, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		m_PacketDataControlList.InsertItem(listidx, "11", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Header Checksum");
		HexTemp.Format("%02X %02X", CStringToHex(data, 48, 2), CStringToHex(data, 50, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 48, 2)), IsAlpha(CStringToHex(data, 50, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);


		m_PacketDataControlList.InsertItem(listidx, "12", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Source Address");
		HexTemp.Format("%02X %02X %02X %02X", CStringToHex(data, 52, 2), CStringToHex(data, 54, 2), CStringToHex(data, 56, 2), CStringToHex(data, 58, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c %c %c %c", IsAlpha(CStringToHex(data, 52, 2)), IsAlpha(CStringToHex(data, 54, 2)), IsAlpha(CStringToHex(data, 56, 2)), IsAlpha(CStringToHex(data, 58, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		m_PacketDataControlList.InsertItem(listidx, "13", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Destination Address");
		HexTemp.Format("%02X %02X %02X %02X", CStringToHex(data, 60, 2), CStringToHex(data, 62, 2), CStringToHex(data, 64, 2), CStringToHex(data, 66, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c %c %c %c", IsAlpha(CStringToHex(data, 60, 2)), IsAlpha(CStringToHex(data, 62, 2)), IsAlpha(CStringToHex(data, 64, 2)), IsAlpha(CStringToHex(data, 66, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		if (protocol == "TCP" || (protocol.Find(_T("TLS")) != -1))
		{
			m_PacketDataControlList.InsertItem(listidx, "14", 0);
			m_PacketDataControlList.SetItemText(listidx, 1, "Source Port");
			HexTemp.Format("%02X %02X ", CStringToHex(data, 68, 2), CStringToHex(data, 70, 2));
			m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
			StrTemp.Format("%c %c ", IsAlpha(CStringToHex(data, 68, 2)), IsAlpha(CStringToHex(data, 70, 2)));
			m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

			m_PacketDataControlList.InsertItem(listidx, "15", 0);
			m_PacketDataControlList.SetItemText(listidx, 1, "Destination Port");
			HexTemp.Format("%02X %02X", CStringToHex(data, 72, 2), CStringToHex(data, 74, 2));
			m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
			StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 72, 2)), IsAlpha(CStringToHex(data, 74, 2)));
			m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

			m_PacketDataControlList.InsertItem(listidx, "16", 0);
			m_PacketDataControlList.SetItemText(listidx, 1, "Sequence Number");
			HexTemp.Format("%02X %02X %02X %02X", CStringToHex(data, 76, 2), CStringToHex(data, 78, 2), CStringToHex(data, 80, 2), CStringToHex(data, 82, 2));
			m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
			StrTemp.Format("%c %c %c %c", IsAlpha(CStringToHex(data, 76, 2)), IsAlpha(CStringToHex(data, 78, 2)), IsAlpha(CStringToHex(data, 80, 2)), IsAlpha(CStringToHex(data, 82, 2)));
			m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

			m_PacketDataControlList.InsertItem(listidx, "17", 0);
			m_PacketDataControlList.SetItemText(listidx, 1, "Acknowledgment");
			HexTemp.Format("%02X %02X %02X %02X", CStringToHex(data, 84, 2), CStringToHex(data, 86, 2), CStringToHex(data, 88, 2), CStringToHex(data, 90, 2));
			m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
			StrTemp.Format("%c %c %c %c", IsAlpha(CStringToHex(data, 84, 2)), IsAlpha(CStringToHex(data, 86, 2)), IsAlpha(CStringToHex(data, 88, 2)), IsAlpha(CStringToHex(data, 90, 2)));
			m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

			m_PacketDataControlList.InsertItem(listidx, "18", 0);
			m_PacketDataControlList.SetItemText(listidx, 1, "Header Length");
			HexTemp.Format("%02X", CStringToHex(data, 92, 1));
			m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
			StrTemp.Format("%c", IsAlpha(CStringToHex(data, 92, 1)));
			m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

			m_PacketDataControlList.InsertItem(listidx, "19", 0);
			m_PacketDataControlList.SetItemText(listidx, 1, "Flags");
			HexTemp.Format("%02X %02X", CStringToHex(data, 93, 1), CStringToHex(data, 94, 2));
			m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
			StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 93, 1)), IsAlpha(CStringToHex(data, 94, 2)));
			m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

			m_PacketDataControlList.InsertItem(listidx, "20", 0);
			m_PacketDataControlList.SetItemText(listidx, 1, "Window");
			HexTemp.Format("%02X %02X", CStringToHex(data, 96, 2), CStringToHex(data, 98, 2));
			m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
			StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 96, 2)), IsAlpha(CStringToHex(data, 98, 2)));
			m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

			m_PacketDataControlList.InsertItem(listidx, "21", 0);
			m_PacketDataControlList.SetItemText(listidx, 1, "Checksum");
			HexTemp.Format("%02X %02X", CStringToHex(data, 100, 2), CStringToHex(data, 102, 2));
			m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
			StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 100, 2)), IsAlpha(CStringToHex(data, 102, 2)));
			m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

			m_PacketDataControlList.InsertItem(listidx, "22", 0);
			m_PacketDataControlList.SetItemText(listidx, 1, "Urgent Pointer");
			HexTemp.Format("%02X %02X", CStringToHex(data, 104, 2), CStringToHex(data, 106, 2));
			m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
			StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 104, 2)), IsAlpha(CStringToHex(data, 106, 2)));
			m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);
			
			// *** IP Header에 Total Length가 40 이상이면 데이터가 더 존재
			if (protocol == "TCP")
			{
				CString strcnt;
				m_PacketDataControlList.InsertItem(listidx, "23", 0);
				m_PacketDataControlList.SetItemText(listidx, 1, "Data");
				HexTemp = "";
				StrTemp = "";
				CString StrTempcat = "";
				int t, j=0, hexcheck = 0;
				for (t = 0; t < CStringToHex(data, 32, 4) - 40; t++)
				{
					if (hexcheck++ == 16) {
						m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
						m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);
						strcnt.Format("%d", listidx);
						m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
						HexTemp = "";
						StrTemp = "";
						hexcheck = 1;
					}
					HexTemp += data.Mid(108 + j, 2).MakeUpper() + " ";
					StrTempcat.Format("%c", IsAlpha(CStringToHex(data, 108 + j, 2)));
					StrTemp += StrTempcat;
					j += 2;
				}
				m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
				m_PacketDataControlList.SetItemText(listidx, 3, StrTemp);
			}
			else if ((protocol.Find(_T("TLS")) != -1))
			{
				CString strcnt;
				CString StrTempcat = "";
				int t = 0;
				strcnt.Format("%d", listidx);
				if (!((CStringToHex(data, 108, 2) >= 20) && (CStringToHex(data, 108, 2) <= 26))
					&& CStringToHex(data, 110, 2) != 3)
				{
					m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
					m_PacketDataControlList.SetItemText(listidx, 1, "Data");
					HexTemp = "";
					StrTemp = "";
					int hexcheck= 0;
					while (1)
					{
						if (
							((CStringToHex(data, 108 + t, 2) >= 20) &&
							(CStringToHex(data, 108 + t, 2) <= 26)) &&
							(CStringToHex(data,110+t, 2) == 3) && ((CStringToHex(data, 112 + t, 2) == 1) || (CStringToHex(data, 112 + t, 2) == 3))
							)
						{
							break;
						}
						if (hexcheck++ == 16) {
							m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
							m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);
							strcnt.Format("%d", listidx);
							m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
							HexTemp = "";
							StrTemp = "";
							hexcheck = 1;
						}
						HexTemp += data.Mid(108 + t, 2).MakeUpper() + " ";
						StrTempcat.Format("%c", IsAlpha(CStringToHex(data, 108 + t, 2)));
						StrTemp += StrTempcat;
						t += 2;
					}
				}
				strcnt.Format("%d", listidx);
				m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
				m_PacketDataControlList.SetItemText(listidx, 1, "Content Type");
				HexTemp.Format("%02X", CStringToHex(data, 108+t, 2));
				m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
				StrTemp.Format("%c", IsAlpha(CStringToHex(data, 108+t, 2)));
				m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

				strcnt.Format("%d", listidx);
				m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
				m_PacketDataControlList.SetItemText(listidx, 1, "Version");
				HexTemp.Format("%02X %02X", CStringToHex(data, 110+t, 2), CStringToHex(data, 112+t, 2));
				m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
				StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 110+t, 2)), IsAlpha(CStringToHex(data, 112+t, 2)));
				m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

				strcnt.Format("%d", listidx);
				m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
				m_PacketDataControlList.SetItemText(listidx, 1, "Length");
				HexTemp.Format("%02X %02X", CStringToHex(data, 114+t, 2), CStringToHex(data, 116 + t, 2));
				m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
				StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 114 + t, 2)), IsAlpha(CStringToHex(data, 116 + t, 2)));
				m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

				strcnt.Format("%d", listidx);
				m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
				m_PacketDataControlList.SetItemText(listidx, 1, "Data");
				HexTemp = "";
				StrTemp = "";
				StrTempcat = "";
				int j = t,z = 118+t, hexcheck = 0;
				for (z; z < data.GetLength(); z+= 2)
				{
					if (hexcheck++ == 16) {
						m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
						m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);
						strcnt.Format("%d", listidx);
						m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
						HexTemp = "";
						StrTemp = "";
						hexcheck = 1;
					}
					HexTemp += data.Mid(118 + j, 2).MakeUpper() + " ";
					StrTempcat.Format("%c", IsAlpha(CStringToHex(data, 118 + j, 2)));
					StrTemp += StrTempcat;
					j += 2;
					if (((CStringToHex(data, 118 + j, 2) >= 20) && (CStringToHex(data,118+j, 2) <= 26)) && (CStringToHex(data, 120+j, 2) == 3) && ((CStringToHex(data, 122+j, 2) == 1) || (CStringToHex(data, 122 + j, 2) == 3)))
					{
						m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
						m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);
						strcnt.Format("%d", listidx);
						m_PacketDataControlList.InsertItem(listidx, strcnt, 0);

						HexTemp = "";
						StrTemp = "";
						hexcheck = 1;

						m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
						m_PacketDataControlList.SetItemText(listidx, 1, "Content Type");
						HexTemp.Format("%02X", CStringToHex(data, 118 + j, 2));
						m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
						StrTemp.Format("%c", IsAlpha(CStringToHex(data, 118 + j, 2)));
						m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

						strcnt.Format("%d", listidx);
						m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
						m_PacketDataControlList.SetItemText(listidx, 1, "Version");
						HexTemp.Format("%02X %02X", CStringToHex(data, 120 + j, 2), CStringToHex(data, 122 + j, 2));
						m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
						StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 120 + j, 2)), IsAlpha(CStringToHex(data, 122 + j, 2)));
						m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

						strcnt.Format("%d", listidx);
						m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
						m_PacketDataControlList.SetItemText(listidx, 1, "Length");
						HexTemp.Format("%02X %02X", CStringToHex(data, 124 + j, 2), CStringToHex(data, 126 + j, 2));
						m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
						StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 124 + j, 2)), IsAlpha(CStringToHex(data, 126 + j, 2)));
						m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

						strcnt.Format("%d", listidx);
						m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
						m_PacketDataControlList.SetItemText(listidx, 1, "Data");
						HexTemp = "";
						StrTemp = "";
						StrTempcat = "";
						hexcheck = 0;
						j += 10;
						z += 10;
					}
				}
				m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
				m_PacketDataControlList.SetItemText(listidx, 3, StrTemp);
			}
		}
		else if (protocol == "UDP" || protocol == "SSDP" || protocol == "DNS")
		{
			m_PacketDataControlList.InsertItem(listidx, "14", 0);
			m_PacketDataControlList.SetItemText(listidx, 1, "Source Port");
			HexTemp.Format("%02X %02X", CStringToHex(data, 68, 2), CStringToHex(data, 70, 2));
			m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
			StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 68, 2)), IsAlpha(CStringToHex(data, 70, 2)));
			m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

			m_PacketDataControlList.InsertItem(listidx, "15", 0);
			m_PacketDataControlList.SetItemText(listidx, 1, "Destination Port");
			HexTemp.Format("%02X %02X", CStringToHex(data, 72, 2), CStringToHex(data, 74, 2));
			m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
			StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 72, 2)), IsAlpha(CStringToHex(data, 74, 2)));
			m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

			m_PacketDataControlList.InsertItem(listidx, "16", 0);
			m_PacketDataControlList.SetItemText(listidx, 1, "Length");
			HexTemp.Format("%02X %02X", CStringToHex(data, 76, 2), CStringToHex(data, 78, 2));
			m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
			StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 76, 2)), IsAlpha(CStringToHex(data, 78, 2)));
			m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

			m_PacketDataControlList.InsertItem(listidx, "17", 0);
			m_PacketDataControlList.SetItemText(listidx, 1, "Checksum");
			HexTemp.Format("%02X %02X", CStringToHex(data, 80, 2), CStringToHex(data, 82, 2));
			m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
			StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 80, 2)), IsAlpha(CStringToHex(data, 82, 2)));
			m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);
			if (protocol == "UDP" || protocol == "SSDP")
			{
				CString strcnt;
				m_PacketDataControlList.InsertItem(listidx, "18", 0);
				m_PacketDataControlList.SetItemText(listidx, 1, "Data");
				HexTemp = "";
				StrTemp = "";
				CString StrTempcat = "";
				int j = 0, hexcheck = 0;
				for (int i = 1; i < udpsize + 1; i++)
				{
					if (hexcheck++ == 16) {
						m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
						m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);
						strcnt.Format("%d", listidx);
						m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
						HexTemp = "";
						StrTemp = "";
						hexcheck = 1;
					}
					HexTemp += data.Mid(84 + j, 2).MakeUpper() + " ";
					if (protocol == "SSDP") StrTempcat.Format("%c", CStringToHex(data, 84 + j, 2));
					else if (protocol == "UDP") StrTempcat.Format("%c", IsAlpha(CStringToHex(data, 84 + j, 2)));
					StrTemp += StrTempcat;
					j += 2;
				}
				m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
				m_PacketDataControlList.SetItemText(listidx, 3, StrTemp);
			}
			else if (protocol == "DNS")
			{
				m_PacketDataControlList.InsertItem(listidx, "18", 0);
				m_PacketDataControlList.SetItemText(listidx, 1, "Transaction ID:");
				HexTemp.Format("%02X %02X", CStringToHex(data, 84, 2), CStringToHex(data, 86, 2));
				m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
				StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 84, 2)), IsAlpha(CStringToHex(data, 86, 2)));
				m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

				m_PacketDataControlList.InsertItem(listidx, "19", 0);
				m_PacketDataControlList.SetItemText(listidx, 1, "Flags:");
				HexTemp.Format("%02X %02X", CStringToHex(data, 88, 2), CStringToHex(data, 90, 2));
				m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
				StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 88, 2)), IsAlpha(CStringToHex(data, 90, 2)));
				m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

				m_PacketDataControlList.InsertItem(listidx, "20", 0);
				m_PacketDataControlList.SetItemText(listidx, 1, "Questions:");
				HexTemp.Format("%02X %02X", CStringToHex(data, 92, 2), CStringToHex(data, 94, 2));
				m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
				StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 92, 2)), IsAlpha(CStringToHex(data, 94, 2)));
				m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

				m_PacketDataControlList.InsertItem(listidx, "21", 0);
				m_PacketDataControlList.SetItemText(listidx, 1, "Answer RRs:");
				HexTemp.Format("%02X %02X", CStringToHex(data, 96, 2), CStringToHex(data, 98, 2));
				m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
				StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 96, 2)), IsAlpha(CStringToHex(data, 98, 2)));
				m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

				m_PacketDataControlList.InsertItem(listidx, "22", 0);
				m_PacketDataControlList.SetItemText(listidx, 1, "Authority RRs:");
				HexTemp.Format("%02X %02X", CStringToHex(data, 100, 2), CStringToHex(data, 102, 2));
				m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
				StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 100, 2)), IsAlpha(CStringToHex(data, 102, 2)));
				m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

				m_PacketDataControlList.InsertItem(listidx, "23", 0);
				m_PacketDataControlList.SetItemText(listidx, 1, "Additional RRs:");
				HexTemp.Format("%02X %02X", CStringToHex(data, 104, 2), CStringToHex(data, 106, 2));
				m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
				StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 104, 2)), IsAlpha(CStringToHex(data, 106, 2)));
				m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

				m_PacketDataControlList.InsertItem(listidx, "24", 0);
				m_PacketDataControlList.SetItemText(listidx, 1, "Queries");

				HexTemp = "";
				StrTemp = "";
				CString StrTempcat = "";
				int j = 0;
				while (CStringToHex(data, 108+ j, 2) != 0)
				{
					HexTemp += data.Mid(108 + j, 2).MakeUpper() + " ";
					StrTempcat.Format("%c", IsAlpha(CStringToHex(data, 108 + j, 2)));
					StrTemp += StrTempcat;
					j += 2;
				}
				j += 2;
				m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
				m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

				m_PacketDataControlList.InsertItem(listidx, "25", 0);
				m_PacketDataControlList.SetItemText(listidx, 1, "type");
				HexTemp.Format("%02X %02X", CStringToHex(data, 108 + j, 2), CStringToHex(data, 108 + j + 2, 2));
				m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
				StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 108 + j, 2)), IsAlpha(CStringToHex(data, 108 + j + 2, 2)));
				m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

				m_PacketDataControlList.InsertItem(listidx, "26", 0);
				m_PacketDataControlList.SetItemText(listidx, 1, "class");
				HexTemp.Format("%02X %02X", CStringToHex(data, 108 + j + 4, 2), CStringToHex(data, 108 + j + 6, 2));
				m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
				StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 108 + j + 4, 2)), IsAlpha(CStringToHex(data, 108 + j + 6, 2)));
				m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);
				j += 8;

				CString strcnt;
				strcnt.Format("%d", listidx);

				m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
				m_PacketDataControlList.SetItemText(listidx, 1, "Answers");
				for (int i = 0; i < CStringToHex(data, 98, 2); i++)						// *** Answers 카운트 만큼 
				{
					HexTemp.Format("%02X %02X", CStringToHex(data, 108+j, 2), CStringToHex(data, 108 + j+2, 2));
					m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
					StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 108 + j, 2)), IsAlpha(CStringToHex(data, 108+j+2, 2)));
					m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);
					j += 4;
					strcnt.Format("%d", listidx);
					
					if (CStringToHex(data, 108 + j, 2) == 0xc0)
					{
						m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
						m_PacketDataControlList.SetItemText(listidx, 1, "Pointer");
						HexTemp.Format("%02X %02X", CStringToHex(data, 108 + j, 2), CStringToHex(data, 108 + j + 2, 2));
						m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
						StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 108 + j, 2)), IsAlpha(CStringToHex(data, 108 + j + 2, 2)));
						m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);
						j += 4;
						strcnt.Format("%d", listidx);
					}
					m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
					m_PacketDataControlList.SetItemText(listidx, 1, "type");
					HexTemp.Format("%02X %02X", CStringToHex(data, 108 + j, 2), CStringToHex(data, 108 + j + 2, 2));
					int type = CStringToHex(data, 108 + j + 2, 2);
					m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
					StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 108 + j, 2)), IsAlpha(CStringToHex(data, 108 + j + 2, 2)));
					m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);
					strcnt.Format("%d", listidx);
					j += 4;

					m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
					m_PacketDataControlList.SetItemText(listidx, 1, "class");
					HexTemp.Format("%02X %02X", CStringToHex(data, 108 + j, 2), CStringToHex(data, 108 + j + 2, 2));
					m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
					StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 108 + j, 2)), IsAlpha(CStringToHex(data, 108 + j + 2, 2)));
					m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);
					strcnt.Format("%d", listidx);
					j += 4;

					m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
					m_PacketDataControlList.SetItemText(listidx, 1, "Time to live");
					HexTemp.Format("%02X %02X %02X %02X", CStringToHex(data, 108 + j, 2), CStringToHex(data, 108 + j + 2, 2),
															CStringToHex(data, 108 + j+4, 2), CStringToHex(data, 108 + j + 6, 2));
					m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
					StrTemp.Format("%c %c %c %c", IsAlpha(CStringToHex(data, 108 + j, 2)), IsAlpha(CStringToHex(data, 108 + j + 2, 2)),
													IsAlpha(CStringToHex(data, 108 + j + 4, 2)), IsAlpha(CStringToHex(data, 108 + j + 6, 2)));
					m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);
					strcnt.Format("%d", listidx);
					j += 8;

					m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
					m_PacketDataControlList.SetItemText(listidx, 1, "Data length");
					HexTemp.Format("%02X %02X", CStringToHex(data, 108 + j, 2), CStringToHex(data, 108 + j + 2, 2));
					m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
					StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 108 + j, 2)), IsAlpha(CStringToHex(data, 108 + j + 2, 2)));
					m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);
					strcnt.Format("%d", listidx);
					j += 4;

					if (type == 1)
					{
						m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
						m_PacketDataControlList.SetItemText(listidx, 1, "Address");
						HexTemp.Format("%02X %02X %02X %02X", CStringToHex(data, 108 + j, 2), CStringToHex(data, 108 + j + 2, 2),
																CStringToHex(data, 108 + j+4, 2), CStringToHex(data, 108 + j + 6, 2));
						m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
						StrTemp.Format("%c %c %c %c", IsAlpha(CStringToHex(data, 108 + j, 2)), IsAlpha(CStringToHex(data, 108 + j + 2, 2)),
														IsAlpha(CStringToHex(data, 108 + j+4, 2)), IsAlpha(CStringToHex(data, 108 + j + 6, 2)));
						m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);
						strcnt.Format("%d", listidx);
						j += 8;
						if (i != CStringToHex(data, 98, 2) - 1)
						{
							m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
							m_PacketDataControlList.SetItemText(listidx, 1, "Pointer");

						}
					}
					else if (type == 5)
					{
						m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
						m_PacketDataControlList.SetItemText(listidx, 1, "CNAME");
						HexTemp = "";
						StrTemp = "";
						while (CStringToHex(data, 108 + j, 2) != 0xC0)
						{
							HexTemp += data.Mid(108 + j, 2).MakeUpper() + " ";
							StrTempcat.Format("%c", IsAlpha(CStringToHex(data, 108 + j, 2)));
							StrTemp += StrTempcat;
							j += 2;
						}
						m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
						m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);
						strcnt.Format("%d", listidx);
						if (i != CStringToHex(data, 98, 2) - 1)
						{
							m_PacketDataControlList.InsertItem(listidx, strcnt, 0);
							m_PacketDataControlList.SetItemText(listidx, 1, "Pointer");

						}
					}
				}
				
			}
			
		}
	}
	

	return 0;
}

// *** Clear 버튼을 눌렀을 때
void CNetworkPacketCaptureDlg::OnBnClickedClear()
{
	int answer;
	// *** 쓰레드 타입이 STOP만 아니라면
	if (m_eThreadWork != ThreadWorkingType::THREAD_STOP)
	{
		answer = MessageBox(_T("캡처를 지울까요?"), _T("캡처 종료"), MB_YESNO | MB_ICONQUESTION);
	}
	else {
		MessageBox(_T("캡처가 시작되지 않았습니다."), _T("오류"), MB_ICONWARNING);
		return;
	}
	// *** 위 질문에 예를 대답하였으면
	if (answer == IDYES) {	
		if (m_PCThread == NULL) {
		}
		else {
			if (m_eThreadWork == ThreadWorkingType::THREAD_PAUSE) m_PauseButton.SetWindowText("Pause(&P)");
			/*
			m_PCThread->ResumeThread();
			is_PCThreadStart = FALSE;
			m_PCThread = NULL;
			m_PacketInfoTree.DeleteAllItems();
			m_NetworkInterfaceControlList.DeleteAllItems();
			m_PacketDataControlList.DeleteAllItems();
			m_eThreadWork = ThreadWorkingType::THREAD_STOP;
			Sleep(1000);
			*/

			m_PCThread->SuspendThread();
			DWORD dwResult;
			::GetExitCodeThread(m_PCThread,&dwResult);
			delete m_PCThread;
			m_PCThread = NULL;

			is_PCThreadStart = FALSE;
			m_eThreadWork = ThreadWorkingType::THREAD_STOP;
			m_PacketInfoTree.DeleteAllItems();
			m_NetworkInterfaceControlList.DeleteAllItems();
			m_PacketDataControlList.DeleteAllItems();
			SetDlgItemText(IDC_STATIC, "THREAD_STATUS: STOP");
			//MessageBox("캡처가 종료되었습니다.", "캡처 종료", MB_OK);
		}
	}
	else if (answer == IDNO) {	// 아니오
	}
}

// *** 툴바 버튼
void CNetworkPacketCaptureDlg::OnTbStartClickedWindows()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// *** 스레드가 스탑인 경우 
	if (!is_PCThreadStart && m_eThreadWork == ThreadWorkingType::THREAD_STOP)
	{
		is_PCThreadStart = TRUE;

		m_eThreadWork = ThreadWorkingType::THREAD_RUNNING;

		m_PCThread = AfxBeginThread(PacketCaptureTFunction, this);
		SetDlgItemText(IDC_STATIC, "THREAD_STATUS: RUNNING");

		if (m_PCThread != NULL) {

			m_PCThread->m_bAutoDelete = TRUE;
		}

		if (m_PCThread == NULL) AfxMessageBox("스레드 생성 실패");
	}
	// *** 스레드가 실행중이고, RUNNING 중이라면 에러메시지
	else if (is_PCThreadStart && m_eThreadWork == ThreadWorkingType::THREAD_RUNNING)
	{
		MessageBox("캡쳐 실행 중", "오류", MB_OK | MB_ICONWARNING);
	}
	else if (m_eThreadWork == ThreadWorkingType::THREAD_PAUSE)
	{
		MessageBox("캡쳐 중지 중", "오류", MB_OK | MB_ICONWARNING);
	}
}

void CNetworkPacketCaptureDlg::OnTbStopClickedWindows()
{
	// *** 스레드가 실행중이지 않고, 쓰레드 타입이 RUNNING이 아니라면 
	if (!is_PCThreadStart && m_eThreadWork != ThreadWorkingType::THREAD_RUNNING) {
		MessageBox(_T("캡처가 시작되지 않았습니다."), _T("오류"), MB_ICONWARNING);
	}
	// *** 스레드가 실행중이라면
	else {
		// *** 스레드 타입이 RUNNING 중이라면
		if (m_eThreadWork == ThreadWorkingType::THREAD_RUNNING) {
			if (MessageBox(_T("캡처를 멈추시겠습니까?"), _T("캡처 중지"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
				m_PauseButton.SetWindowText("Resume(&P)");
				SetDlgItemText(IDC_STATIC, "THREAD_STATUS: PAUSE");

				//m_PCThread->SuspendThread();
				if (m_PCThread == NULL) {
					m_PCThread;
				}
				m_eThreadWork = ThreadWorkingType::THREAD_PAUSE;
			}
		}
		// *** 스레드 타입이 PAUSE 중이라면
		else if (m_eThreadWork == ThreadWorkingType::THREAD_PAUSE) {
			if (MessageBox(_T("캡처를 다시 시작 하시겠습니까?"), _T("캡처 다시 시작"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
				m_PauseButton.SetWindowText("Pause(&P)");
				SetDlgItemText(IDC_STATIC, "THREAD_STATUS: RUNNING");
				m_PCThread->ResumeThread();
				m_eThreadWork = ThreadWorkingType::THREAD_RUNNING;
			}
		}
	}
}

void CNetworkPacketCaptureDlg::OnTbClearClickedWindows()
{
	int answer;
	// *** 쓰레드 타입이 STOP만 아니라면
	if (m_eThreadWork != ThreadWorkingType::THREAD_STOP)
	{
		answer = MessageBox(_T("캡처를 지울까요?"), _T("캡처 종료"), MB_YESNO | MB_ICONQUESTION);
	}
	else {
		MessageBox(_T("캡처가 시작되지 않았습니다."), _T("오류"), MB_ICONWARNING);
		return;
	}
	// *** 위 질문에 예를 대답하였으면
	if (answer == IDYES) {
		if (m_PCThread == NULL) {
		}
		else {
			if (m_eThreadWork == ThreadWorkingType::THREAD_PAUSE) m_PauseButton.SetWindowText("Pause(&P)");
			/*
			m_PCThread->ResumeThread();
			is_PCThreadStart = FALSE;
			m_PCThread = NULL;
			m_PacketInfoTree.DeleteAllItems();
			m_NetworkInterfaceControlList.DeleteAllItems();
			m_PacketDataControlList.DeleteAllItems();
			m_eThreadWork = ThreadWorkingType::THREAD_STOP;
			Sleep(1000);
			*/

			m_PCThread->SuspendThread();
			DWORD dwResult;
			::GetExitCodeThread(m_PCThread, &dwResult);
			delete m_PCThread;
			m_PCThread = NULL;

			is_PCThreadStart = FALSE;
			m_eThreadWork = ThreadWorkingType::THREAD_STOP;
			m_PacketInfoTree.DeleteAllItems();
			m_NetworkInterfaceControlList.DeleteAllItems();
			m_PacketDataControlList.DeleteAllItems();
			SetDlgItemText(IDC_STATIC, "THREAD_STATUS: STOP");
			//MessageBox("캡처가 종료되었습니다.", "캡처 종료", MB_OK);
		}
	}
	else if (answer == IDNO) {	// 아니오
	}
}

// *** 컨트롤 리스트 헤더 눌렀을 떄 이벤트
void CNetworkPacketCaptureDlg::OnHdnItemclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// 클릭한 컬럼의 인덱스
	int nColumn = pNMLV->iItem;

	// 현 리스트 컨트롤에 있는 데이터 총 자료 개수만큼 저장
	for (int i = 0; i < (m_NetworkInterfaceControlList.GetItemCount()); i++) {
		m_NetworkInterfaceControlList.SetItemData(i, i);
	}

	// 정렬 방식 저장
	m_bAscending = !m_bAscending;

	// 정렬 관련 구조체 변수 생성 및 데이터 초기화
	SORTPARAM sortparams;
	sortparams.pList = &m_NetworkInterfaceControlList;
	sortparams.iSortColumn = nColumn;
	sortparams.bSortDirect = m_bAscending;
	sortparams.flag = pNMLV->iItem;
	// 정렬 함수 호출
	m_NetworkInterfaceControlList.SortItems(&CompareItem, (LPARAM)&sortparams);
	*pResult = 0;
}

// *** 메뉴에 소스코드 버튼 누르면
void CNetworkPacketCaptureDlg::Onsourcebutton()
{
	system("explorer https://github.com/zzzangmans1/wonjoo/tree/main/c_lang/MFC");
}

// *** 메뉴에 로그 버튼을 누르면
void CNetworkPacketCaptureDlg::OnLogButton()
{
	if (m_LOGThread != NULL)
	{
		AfxMessageBox("LOG thread is running.");
		return;
	}

	if (m_NetworkInterfaceControlList.GetItemCount() == 0) {
		AfxMessageBox("The control list is empty.");
		return;
	}
	// *** Thread가 NULL이 아니고, Control List 값이 있으면 스레드 실행
	else {

		m_LOGThread = AfxBeginThread(ThreadClient, this);
	}
	// *** 스레드 생성 실패
	if (m_LOGThread == NULL)
	{
		AfxMessageBox("Log Client Thread Create Failed");
		return;
	}
	// *** 스레드 생성 성공
	else 
	{
		is_LOGThreadStart = TRUE;
		m_LOGThread->m_bAutoDelete = FALSE;
		//AfxMessageBox("Log Client Thread Success");
	}
	return;
}
