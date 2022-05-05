
// NetworkPacketCaptureDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "NetworkPacketCapture.h"
#include "NetworkPacketCaptureDlg.h"
#include "afxdialogex.h"
#include <locale.h>
#include "ChoiceNetworkInterface.h"   
#include <WinSock2.h>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

#pragma warning (disable : 4996)

#define FAIL					-1

#define SWAP16(s)				(((((s) & 0xff) << 8) | (((s) >> 8) & 0xff)))
#define FLAGSUM(x, y, z)		(x << 3) | (y << 2) | (z << 1)

#define CStringToHex(x,y,z)		strtol(x.Mid(y, z), NULL, 16)	// *** CString to Hex
#define IsAlpha(x)				isalpha(x) ? x : '.'
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
	m_PacketDataControlList.InsertColumn(2, "Data Hex", LVCFMT_LEFT, 1000);
	m_PacketDataControlList.InsertColumn(3, "Data Char", LVCFMT_LEFT, rect.Width()-1300);
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
		CString Source = m_NetworkInterfaceControlList.GetItemText(pNMItemActivate->iItem, 2);
		CString Destination = m_NetworkInterfaceControlList.GetItemText(pNMItemActivate->iItem, 3);
		CString Protocol = m_NetworkInterfaceControlList.GetItemText(pNMItemActivate->iItem, 4);
		CString Length = m_NetworkInterfaceControlList.GetItemText(pNMItemActivate->iItem, 5);
		CString SaveData = m_NetworkInterfaceControlList.GetItemText(pNMItemActivate->iItem, 7);

		if (ClickPacketFrameNumber != atoi(FrameCnt))
		{
			m_PacketInfoTree.DeleteAllItems();
			m_PacketDataControlList.DeleteAllItems();
			SetPacketInfoTree(FrameCnt, Time, Source, Destination, Protocol, Length, SaveData);
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
	pDlg->m_EthernetHeader = (ETHERNET_HEADER*)data;
	pDlg->m_IpHeader = (IP_HEADER*)(data +14);
	pDlg->m_IpHeaderLen = (pDlg->m_IpHeader->header_len & 0xF) * 4;
	

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
	if (pDlg->m_IpHeader->protocol == IPPROTO_TCP)
	{
		pDlg->m_Protocol = "TCP";
		pDlg->m_TCPHeader = (TCP_HEADER*) ((u_char*)pDlg->m_IpHeader + pDlg->m_IpHeaderLen);
		pDlg->m_TCPPacketInfo.Format("%d -> %d", ntohs(pDlg->m_TCPHeader->src_port), ntohs(pDlg->m_TCPHeader->dst_port));
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
		pDlg->m_UDPPacketInfo.Format("%d -> %d Len = %d", ntohs(pDlg->m_UDPHeader->sport), ntohs(pDlg->m_UDPHeader->dport), SWAP16(pDlg->m_UDPHeader->length)-8);
		if (ntohs(pDlg->m_UDPHeader->dport) == 1900 && pDlg->m_DestinationIp == "239.255.255.250") {
			pDlg->m_Protocol = "SSDP";
			int i = 42;
			AfxMessageBox(pDlg->m_Protocol);
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
		pDlg->SetPacketInfoTree("1",  pDlg->m_CurrentTime, pDlg->m_SourceIp, pDlg->m_DestinationIp, pDlg->m_Protocol,pDlg->m_PacketLength, SaveData);
		if (pDlg->m_IpHeader->protocol == IPPROTO_UDP) pDlg->SetPacketHexList(SaveData, pDlg->m_Protocol, SWAP16(pDlg->m_UDPHeader->length));
		else pDlg->SetPacketHexList(SaveData, pDlg->m_Protocol, 0);
	}
	// 리스트 컨트롤 꽉찰경우 EnsureVisible() 를 사용하여 자동으로 
	pDlg->m_NetworkInterfaceControlList.EnsureVisible(pDlg->m_NetworkInterfaceControlList.GetItemCount() - 1, FALSE);
}

// 쓰레드 동작함수 패킷 캡쳐하는 부분
UINT CNetworkPacketCaptureDlg::PacketCaptureTFunction(LPVOID _mothod)
{
	CString i_cnt;

	pcap_if_t* alldevs,*choice_dev;
	pcap_t* phandle;
	
	bpf_program fcode;
	bpf_u_int32 NetMask = 0;

	char* errbuf = "";
	const char* filter = "tcp or udp";
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
		BREAK_LOOP_ERROR_MESSAGE = "패킷 종료";
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
		else
		{
			pLVCD->clrText = RGB(0, 0, 0);
		}
		
		*pResult = CDRF_DODEFAULT;
	}
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
				m_PCThread->SuspendThread();
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
int CNetworkPacketCaptureDlg::SetPacketInfoTree(CString framecnt,CString time,  CString source, CString destination, CString protocol, CString length, CString savedata)
{
	CString CStmp, TypeName;
	// *** 트리 변수 선언
	HTREEITEM TR1 = NULL, TR1_1=NULL, TR1_2=NULL, TR1_3=NULL, TR1_4=NULL, TR1_5=NULL, TR1_6=NULL;
	HTREEITEM TR2 = NULL, TR2_1=NULL, TR2_2=NULL, TR2_3=NULL;
	HTREEITEM TR3 = NULL, TR3_1=NULL, TR3_2=NULL, TR3_3=NULL, TR3_4=NULL, TR3_5=NULL, TR3_6=NULL, TR3_7=NULL, TR3_8=NULL, TR3_9=NULL, TR3_10=NULL, TR3_11=NULL, TR3_12=NULL;
	HTREEITEM TCPTR4 = NULL, TCPTR4_1 = NULL, TCPTR4_2 = NULL, TCPTR4_3 = NULL, TCPTR4_4 = NULL, TCPTR4_5 = NULL,
		TCPTR4_6 = NULL, TCPTR4_6_1 = NULL, TCPTR4_6_2 = NULL, TCPTR4_6_3 = NULL, TCPTR4_6_4 = NULL, TCPTR4_6_5 = NULL, TCPTR4_6_6 = NULL,
		TCPTR4_6_7 = NULL, TCPTR4_6_8 = NULL, TCPTR4_6_9 = NULL, TCPTR4_6_10 = NULL,
				TCPTR4_7=NULL, TCPTR4_8=NULL, TCPTR4_9=NULL;
	HTREEITEM UDPTR4 = NULL, UDPTR4_1=NULL, UDPTR4_2=NULL, UDPTR4_3=NULL, UDPTR4_4=NULL, UDPTR4_5=NULL;
	HTREEITEM SSDPTR5 = NULL, SSDPTR5_1 = NULL,SSDPTR5_2 = NULL, SSDPTR5_3 = NULL, SSDPTR5_4 = NULL, SSDPTR5_5 = NULL, SSDPTR5_6 = NULL, SSDPTR5_7 = NULL, SSDPTR5_8 = NULL, SSDPTR5_9;

	// *** 트리 초기화
	m_PacketInfoTree.DeleteAllItems();
	m_PacketInfoTree.Invalidate();

	// *** 트리 데이터 선언
	CString TRS1, TRS1_1, TRS1_2, TRS1_3, TRS1_4, TRS1_5, TRS1_6;
	CString TRS2, TRS2_1, TRS2_2, TRS2_3;
	CString TRS3, TRS3_1, TRS3_2, TRS3_3, TRS3_4, TRS3_5, TRS3_6, TRS3_7, TRS3_8, TRS3_9, TRS3_10, TRS3_11, TRS3_12;
	CString TCPTRS4, TCPTRS4_1, TCPTRS4_2, TCPTRS4_3, TCPTRS4_4, TCPTRS4_5, TCPTRS4_6,
		TCPTRS4_6_1, TCPTRS4_6_2, TCPTRS4_6_3, TCPTRS4_6_4, TCPTRS4_6_5,
		TCPTRS4_6_6, TCPTRS4_6_7, TCPTRS4_6_8, TCPTRS4_6_9, TCPTRS4_6_10,
		TCPTRS4_7, TCPTRS4_8, TCPTRS4_9;
	CString UDPTRS4, UDPTRS4_1, UDPTRS4_2, UDPTRS4_3, UDPTRS4_4, UDPTRS4_5;
	CString SSDPTRS5, SSDPTRS5_1, SSDPTRS5_2, SSDPTRS5_3, SSDPTRS5_4, SSDPTRS5_5, SSDPTRS5_6, SSDPTRS5_7, SSDPTRS5_8, SSDPTRS5_9;

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
	MACD.Format("(%X:%X:%X:%X:%X:%X)", CStringToHex(savedata, 0, 2), CStringToHex(savedata, 2, 2), CStringToHex(savedata,4, 2), CStringToHex(savedata ,6, 2), CStringToHex(savedata ,8, 2), CStringToHex(savedata, 10, 2));
	MACS.Format("(%X:%X:%X:%X:%X:%X)", CStringToHex(savedata ,12, 2), CStringToHex(savedata ,14, 2), CStringToHex(savedata , 16, 2), CStringToHex(savedata ,18 ,2), CStringToHex(savedata ,20, 2), CStringToHex(savedata ,22, 2));

	// *** 두 번째 트리
	TRS2 = "Ethernet II, Src : " + MACS + ", Dst: " + MACD;
	TRS2_1 = "Destination: " + MACD;
	TRS2_2 = "Source: "+ MACS;
	
	int type = CStringToHex(savedata, 24, 4);
	//short int type = SWAP16(m_EthernetHeader->type);			// *** 리틀엔디안 변경
	
	if (type == 0x800)
	{
		TypeName = "IPv4";
	}
	TRS2_3.Format("Type: %s (0x%04X)",TypeName, type);
	
	TRS3 = "InternetProtocol Version " + savedata.Mid(28,1) + ", Src: " + source + ", Dst: " + destination;

	TRS3_1.Format("%d%d%d%d .... = Version: %X", (CStringToHex(savedata, 28, 1) >> 3) & 1, (CStringToHex(savedata, 28, 1) >> 2) & 1, (CStringToHex(savedata, 28, 1) >> 1) & 1,
													(CStringToHex(savedata, 28, 1) >> 0) & 1, CStringToHex(savedata, 28, 1));
	
	TRS3_2.Format(".... %d%d%d%d = Header Length: %d bytes (%d)", (CStringToHex(savedata, 29, 1) >> 3) & 1, (CStringToHex(savedata, 29, 1) >> 2) & 1, (CStringToHex(savedata, 29, 1) >> 1) & 1,
																	(CStringToHex(savedata, 29, 1) >> 0) & 1, CStringToHex(savedata, 29, 1) * 4 , CStringToHex(savedata, 29, 1));
	TRS3_3.Format("Differentiated Services Field: 0x%02X", CStringToHex(savedata, 30, 2));

	TRS3_4.Format("Total Length: %d", CStringToHex(savedata, 32, 4));

	TRS3_5.Format("Identification: 0x%04X (%d)", CStringToHex(savedata,36,4),  CStringToHex(savedata, 36,4));
	
	TRS3_6.Format("Flags: 0x%02X", CStringToHex(savedata, 40, 2));
	TRS3_7.Format("...%d %d%d%d%d %d%d%d%d %d%d%d%d = Fragment Offset: %X", (CStringToHex(savedata,40,4) >> 12) & 1, (CStringToHex(savedata, 40, 4) >> 11) & 1, (CStringToHex(savedata, 40, 4) >> 10) & 1,
											(CStringToHex(savedata, 40, 4) >> 9) & 1, (CStringToHex(savedata, 40, 4) >> 8) & 1, (CStringToHex(savedata, 40, 4) >> 7) & 1, (CStringToHex(savedata, 40, 4) >> 6) & 1,
											(CStringToHex(savedata, 40, 4) >> 5) & 1, (CStringToHex(savedata, 40, 4) >> 4) & 1, (CStringToHex(savedata, 40, 4) >> 3) & 1, (CStringToHex(savedata, 40, 4) >> 2) & 1,
											(CStringToHex(savedata, 40, 4) >> 1) & 1, (CStringToHex(savedata, 40, 4) >> 0) & 1, m_IpHeader->frag_offset1);
	TRS3_8.Format("Time to Live: %d", CStringToHex(savedata, 44, 2));
	TRS3_9.Format("Protocol: %s (%d)", protocol, CStringToHex(savedata, 46, 2));
	
	//if (CStringToHex(savedata, 46, 2) == IPPROTO_TCP) TRS3_9.Format("Protocol: %s (%d)",protocol, CStringToHex(savedata, 46, 2));
	//else if (CStringToHex(savedata, 46, 2) == IPPROTO_UDP) TRS3_9.Format("Protocol: %s (%d)", protocol, CStringToHex(savedata, 46, 2));
	TRS3_10.Format("Header Checksum: 0x%04X", CStringToHex(savedata, 48, 4));
	TRS3_11 = "Source Address: "+source;			// 52+8
	TRS3_12 = "Destination Address: "+ destination; // 60+8

	// *** 세 번째 트리 
	// *** TCP 라면
	if (!protocol.Compare( "TCP"))
	{
		TCPTRS4.Format("Transmission Control Protocol, Src Port: %d, Dst Port: %d", CStringToHex(savedata, 68, 4), CStringToHex(savedata, 72, 4));
		TCPTRS4_1.Format("Source Port: %d", CStringToHex(savedata, 68, 4));
		TCPTRS4_2.Format("Destination Port: %d", CStringToHex(savedata, 72, 4));
		TCPTRS4_3.Format("Sequence Number: %u", CStringToHex(savedata, 76,8));
		TCPTRS4_4.Format("Acknowledge Number: %u", CStringToHex(savedata, 84,8));
		TCPTRS4_5.Format("%d%d%d%d .... = Header Length: %d bytes (%d)",(CStringToHex(savedata,92,1) >> 3) & 1, (CStringToHex(savedata, 92, 1) >> 2) & 1, (CStringToHex(savedata, 92, 1) >> 1) & 1, 
																		(CStringToHex(savedata, 92, 1) >> 0) & 1, CStringToHex(savedata, 92, 1) *4, CStringToHex(savedata, 92, 1));
		TCPTRS4_6.Format("Flags: 0x%X", CStringToHex(savedata,93, 3));
		TCPTRS4_6_1.Format("%d%d%d. .... .... = Reserved: %s", (CStringToHex(savedata, 93, 1) >> 3) & 1, (CStringToHex(savedata, 93, 1) >> 2) & 1, (CStringToHex(savedata, 93, 1) >> 1) & 1,
																((CStringToHex(savedata, 93, 1) >> 1) & 0xFFF) > 0 ? "Set" : "Not Set");
		TCPTRS4_6_2.Format("...%d .... .... = Nonce: %s", CStringToHex(savedata, 93, 1) & 1, (CStringToHex(savedata, 93, 1) & 1) > 0 ? "Set" : "Not Set");
		TCPTRS4_6_3.Format(".... %d... .... = Congestion Window Reduced (CWR): %s", (CStringToHex(savedata, 94, 1) >> 3) & 1, ((CStringToHex(savedata, 94, 1) >> 3) & 1) > 0 ? "Set" : "Not Set");
		TCPTRS4_6_4.Format(".... .%d.. .... = ECN-Echo: %s", (CStringToHex(savedata, 94, 1) >> 2) & 1, ((CStringToHex(savedata, 94, 1) >> 2) & 1) > 0 ? "Set" : "Not Set");
		TCPTRS4_6_5.Format(".... ..%d. .... = Urgent: %s", (CStringToHex(savedata, 94, 1) >> 1) & 1, ((CStringToHex(savedata, 94, 1) >> 1) & 1) > 0 ? "Set" : "Not Set");
		TCPTRS4_6_6.Format(".... ...%d .... = Acknowledgment: %s", (CStringToHex(savedata, 94, 1) >> 0) & 1, ((CStringToHex(savedata, 94, 1) >> 0) & 1) > 0 ? "Set" : "Not Set");
		TCPTRS4_6_7.Format(".... .... %d... = Push: %s", (CStringToHex(savedata, 95, 1) >> 3) & 1, ((CStringToHex(savedata, 95, 1) >> 3) & 1) > 0 ? "Set" : "Not Set");
		TCPTRS4_6_8.Format(".... .... .%d.. = Reset: %s", (CStringToHex(savedata, 95, 1) >> 2) & 1, ((CStringToHex(savedata, 95, 1) >> 2) & 1) > 0 ? "Set" : "Not Set");
		TCPTRS4_6_9.Format(".... .... ..%d. = Syn: %s", (CStringToHex(savedata, 95, 1) >> 1) & 1, ((CStringToHex(savedata, 95, 1) >> 1) & 1) > 0 ? "Set" : "Not Set");
		TCPTRS4_6_10.Format(".... .... ...%d = Fin: %s", (CStringToHex(savedata, 95, 1) >> 0) & 1, ((CStringToHex(savedata, 95, 1) >> 0) & 1) > 0 ? "Set" : "Not Set");

		TCPTRS4_7.Format("Window: %d", CStringToHex(savedata, 96,4));
		TCPTRS4_8.Format("Checksum: 0x%X", CStringToHex(savedata, 100, 4));
		TCPTRS4_9.Format("Urgent Pointer: %d", CStringToHex(savedata, 104, 4));
		
	}
	// *** UDP 라면
	else if (!protocol.Compare("UDP") || !protocol.Compare("SSDP"))
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
			while (!(CStringToHex(savedata, 84 + j, 2) == 0xd && CStringToHex(savedata, 84 + j + 2, 2) == 0xa))
			{
				copytmp.Format("%c", CStringToHex(savedata, 84 + j, 2));
				SSDPTRS5_6 += copytmp + "";
				j += 2;
			}
			j += 4;
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
	}
	TR1 = m_PacketInfoTree.InsertItem(TRS1, 0, 0,TVI_ROOT, TVI_LAST);
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
	TR3_7 = m_PacketInfoTree.InsertItem(TRS3_7, 0, 0, TR3, TVI_LAST);
	TR3_8 = m_PacketInfoTree.InsertItem(TRS3_8, 0, 0, TR3, TVI_LAST);
	TR3_9 = m_PacketInfoTree.InsertItem(TRS3_9, 0, 0, TR3, TVI_LAST);
	TR3_10 = m_PacketInfoTree.InsertItem(TRS3_10, 0, 0, TR3, TVI_LAST);
	TR3_11 = m_PacketInfoTree.InsertItem(TRS3_11, 0, 0, TR3, TVI_LAST);
	TR3_12 = m_PacketInfoTree.InsertItem(TRS3_12, 0, 0, TR3, TVI_LAST);

	if (!protocol.Compare("TCP"))
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

		m_PacketInfoTree.Expand(TCPTR4, TVE_EXPAND);
	}
	else if (!protocol.Compare("UDP") || !protocol.Compare("SSDP"))
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
			SSDPTR5_6 = m_PacketInfoTree.InsertItem(SSDPTRS5_6, 0, 0, SSDPTR5, TVI_LAST);
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
		m_PacketInfoTree.Expand(UDPTR4, TVE_EXPAND);
	}

	// *** 트리 오픈
	//m_PacketInfoTree.Expand(TR1, TVE_EXPAND);
	//m_PacketInfoTree.Expand(TR2, TVE_EXPAND);
	//m_PacketInfoTree.Expand(TR3, TVE_EXPAND);
	return 0;
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

	m_PacketDataControlList.InsertItem(listidx, "3", 0);
	m_PacketDataControlList.SetItemText(listidx, 1, "Version");
	HexTemp.Format("%X", CStringToHex(data, 28, 1));
	m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
	StrTemp.Format("%c", IsAlpha(CStringToHex(data ,28, 1)));
	m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

	m_PacketDataControlList.InsertItem(listidx, "4", 0);
	m_PacketDataControlList.SetItemText(listidx, 1, "Header Length");
	HexTemp.Format("%X", CStringToHex(data ,29,1));
	m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
	StrTemp.Format("%c", IsAlpha(CStringToHex(data ,29 ,1)));
	m_PacketDataControlList.SetItemText(listidx++, 3, HexTemp);

	m_PacketDataControlList.InsertItem(listidx, "5", 0);
	m_PacketDataControlList.SetItemText(listidx, 1, "Differentiated Services Field");
	HexTemp.Format("%02X", CStringToHex(data ,30,2));
	m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
	StrTemp.Format("%c", IsAlpha(CStringToHex(data ,30,2)));
	m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

	m_PacketDataControlList.InsertItem(listidx, "6", 0);
	m_PacketDataControlList.SetItemText(listidx, 1, "Total Length");
	HexTemp.Format("%02X %02X", CStringToHex(data ,32, 2), CStringToHex(data ,34,2));
	m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
	StrTemp.Format("%c %c", IsAlpha(CStringToHex(data ,32, 2)), IsAlpha(CStringToHex(data ,34, 2)));
	m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

	m_PacketDataControlList.InsertItem(listidx, "7", 0);
	m_PacketDataControlList.SetItemText(listidx, 1, "Identification");
	HexTemp.Format("%02X %02X", CStringToHex(data ,36, 2), CStringToHex(data,38, 2));
	m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
	StrTemp.Format("%c %c", IsAlpha(CStringToHex(data ,36, 2)), IsAlpha(CStringToHex(data ,38, 2)));
	m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

	m_PacketDataControlList.InsertItem(listidx, "8", 0);
	m_PacketDataControlList.SetItemText(listidx, 1, "Flags");
	HexTemp.Format("%02X %02X", CStringToHex(data ,40, 2), CStringToHex(data ,42, 2));
	m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
	StrTemp.Format("%c %c", IsAlpha(CStringToHex(data ,40, 2)), IsAlpha(CStringToHex(data, 42, 2)));
	m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

	m_PacketDataControlList.InsertItem(listidx, "9", 0);
	m_PacketDataControlList.SetItemText(listidx, 1, "Time to Live");
	HexTemp.Format("%02X", CStringToHex(data ,44, 2));
	m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
	StrTemp.Format("%c", IsAlpha(CStringToHex(data ,44, 2)));
	m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

	m_PacketDataControlList.InsertItem(listidx, "10", 0);
	m_PacketDataControlList.SetItemText(listidx, 1, "Protocol");
	HexTemp.Format("%02X", CStringToHex(data ,46, 2));
	m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
	StrTemp.Format("%c", IsAlpha(CStringToHex(data ,46, 2)));
	m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

	m_PacketDataControlList.InsertItem(listidx, "11", 0);
	m_PacketDataControlList.SetItemText(listidx, 1, "Header Checksum");
	HexTemp.Format("%02X %02X",  CStringToHex(data ,48, 2), CStringToHex(data ,50, 2));
	m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
	StrTemp.Format("%c %c", IsAlpha(CStringToHex(data ,48, 2)), IsAlpha(CStringToHex(data ,50, 2)));
	m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);


	m_PacketDataControlList.InsertItem(listidx, "12", 0);
	m_PacketDataControlList.SetItemText(listidx, 1, "Source Address");
	HexTemp.Format("%02X %02X %02X %02X", CStringToHex(data ,52,2),CStringToHex(data ,54,2) ,CStringToHex(data ,56,2) , CStringToHex(data ,58, 2));
	m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
	StrTemp.Format("%c %c %c %c", IsAlpha(CStringToHex(data ,52,2)), IsAlpha(CStringToHex(data ,54,2)), IsAlpha(CStringToHex(data ,56,2)), IsAlpha(CStringToHex(data ,58, 2)));
	m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

	m_PacketDataControlList.InsertItem(listidx, "13", 0);
	m_PacketDataControlList.SetItemText(listidx, 1, "Destination Address");
	HexTemp.Format("%02X %02X %02X %02X", CStringToHex(data ,60, 2), CStringToHex(data ,62, 2) , CStringToHex(data ,64, 2), CStringToHex(data ,66,2));
	m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
	StrTemp.Format("%c %c %c %c", IsAlpha(CStringToHex(data ,60, 2)), IsAlpha(CStringToHex(data ,62, 2)), IsAlpha(CStringToHex(data ,64, 2)), IsAlpha(CStringToHex(data ,66,2)));
	m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

	if (protocol == "TCP")
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
		StrTemp.Format("%c %c %c %c" , IsAlpha(CStringToHex(data, 76, 2)), IsAlpha(CStringToHex(data, 78, 2)), IsAlpha(CStringToHex(data, 80, 2)), IsAlpha(CStringToHex(data, 82, 2)));
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
		m_PacketDataControlList.SetItemText(listidx, 3, StrTemp);
	}
	else if (protocol == "UDP" || protocol == "SSDP")
	{
		m_PacketDataControlList.InsertItem(listidx, "14", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Source Port");
		HexTemp.Format("%02X %02X", CStringToHex(data ,68, 2), CStringToHex(data ,70, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c %c", IsAlpha(CStringToHex(data ,68, 2)), IsAlpha(CStringToHex(data ,70, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		m_PacketDataControlList.InsertItem(listidx, "15", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Destination Port");
		HexTemp.Format("%02X %02X", CStringToHex(data, 72, 2), CStringToHex(data,74, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c %c", IsAlpha(CStringToHex(data, 72, 2)), IsAlpha(CStringToHex(data ,74, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		m_PacketDataControlList.InsertItem(listidx, "16", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Length");
		HexTemp.Format("%02X %02X", CStringToHex(data ,76, 2), CStringToHex(data ,78, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c %c", IsAlpha(CStringToHex(data ,76, 2)), IsAlpha(CStringToHex(data ,78, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);

		m_PacketDataControlList.InsertItem(listidx, "17", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Checksum");
		HexTemp.Format("%02X %02X", CStringToHex(data ,80, 2), CStringToHex(data ,82, 2));
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		StrTemp.Format("%c %c", IsAlpha(CStringToHex(data,80, 2)), IsAlpha(CStringToHex(data ,82, 2)));
		m_PacketDataControlList.SetItemText(listidx++, 3, StrTemp);
		CString strcnt;
		m_PacketDataControlList.InsertItem(listidx, "18", 0);
		m_PacketDataControlList.SetItemText(listidx, 1, "Data");
		HexTemp = "";
		StrTemp = "";
		CString StrTempcat = "";
		int j = 0, hexcheck = 0;
		for (int i = 1; i < udpsize+1; i++)
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
			HexTemp += data.Mid(84+j, 2).MakeUpper()+" ";
			if(protocol == "SSDP") StrTempcat.Format("%c", CStringToHex(data, 84 + j, 2));
			else if(protocol == "UDP") StrTempcat.Format("%c", IsAlpha(CStringToHex(data, 84+ j, 2)));
			StrTemp += StrTempcat;
			j += 2;
		}
		m_PacketDataControlList.SetItemText(listidx, 2, HexTemp);
		m_PacketDataControlList.SetItemText(listidx, 3, StrTemp);
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
			is_PCThreadStart = FALSE;
			m_PCThread = NULL;
			m_PacketInfoTree.DeleteAllItems();
			m_NetworkInterfaceControlList.DeleteAllItems();
			m_PacketDataControlList.DeleteAllItems();
			m_eThreadWork = ThreadWorkingType::THREAD_STOP; 
			Sleep(1000);
			SetDlgItemText(IDC_STATIC, "THREAD_STATUS: STOP");
			MessageBox("캡처가 종료되었습니다.", "캡처 종료", MB_OK);
		}
	}
	else if (answer == IDNO) {	// 아니오
	}
}

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
				m_PCThread->SuspendThread();
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
			is_PCThreadStart = FALSE;
			m_PCThread = NULL;
			m_PacketInfoTree.DeleteAllItems();
			m_NetworkInterfaceControlList.DeleteAllItems();
			m_PacketDataControlList.DeleteAllItems();
			m_eThreadWork = ThreadWorkingType::THREAD_STOP;
			Sleep(1000);
			SetDlgItemText(IDC_STATIC, "THREAD_STATUS: STOP");
			MessageBox("캡처가 종료되었습니다.", "캡처 종료", MB_OK);
		}
	}
	else if (answer == IDNO) {	// 아니오
	}
}
