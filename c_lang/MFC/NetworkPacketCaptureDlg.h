// NetworkPacketCaptureDlg.h: 헤더 파일
//

#pragma once

#include <pcap/pcap.h>
#include "Protocol.h"
#include "ChoiceNetworkInterface.h"   
#include <locale.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <afxsock.h>
#include <TlHelp32.h>			// *** 파일 종료할 때 찾는 라이브러리

// 소켓을 사용하기 위해서 라이브러리 참조해야 한다.
#pragma comment(lib, "ws2_32")
// inet_ntoa가 deprecated가 되었는데.. 사용하려면 아래 설정을 해야 한다.
#pragma warning(disable:4996)
#include <vector>
#include <thread>
// 소켓을 사용하기 위한 라이브러리
#include <WinSock2.h>

using namespace std;

// 수신 버퍼 사이즈
#define BUFFERSIZE 1024


void Packet_Handler(u_char* param, const pcap_pkthdr* header, const u_char* data);	// *** 클래스 외에 생성해야 pcap_loop 함수 실행가능

// CNetworkPacketCaptureDlg 대화 상자
class CNetworkPacketCaptureDlg : public CDialogEx
{
	// 생성입니다.
public:
	CNetworkPacketCaptureDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NETWORKPACKETCAPTURE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	CToolBar m_wndToolBar;														// *** 툴바 변수 선언

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);										// *** 다이얼로그 백그라운드 컬러 지정할 함수 선언

	afx_msg void Onsourcebutton();
	afx_msg void OnLogButton();
	void InitToolBar();															// *** 툴바 생성 함수 선언
											
	enum class ThreadWorkingType {
		THREAD_STOP,
		THREAD_RUNNING,
		THREAD_PAUSE
	};

	ETHERNET_HEADER* m_EthernetHeader;				// *** ETHERNET HEADER
	IP_HEADER* m_IpHeader;							// *** IP HEADER
	size_t m_IpHeaderLen;
	TCP_HEADER* m_TCPHeader;						// *** TCP HEADER
	UDP_HEADER* m_UDPHeader;						// *** UDP HEADER
	ARP_HEADER* m_ARPHeader;						// *** ARP HEADER
	DNS_HEADER* m_DNSHeader;						// *** DNS HEADER

	/*
	struct pcap_pkthdr {
		struct timeval ts;       time stamp			캡쳐된 시간정보가 저장된 멤버 ts
		bpf_u_int32 caplen;   length of portion present		캡쳐한 패킷의 길이가 저장된 멤버 caplen
		bpf_u_int32 len;        length this packet (off wire) 패킷의 길이가 저장된 멤버 len
	};
	*/
	pcap_pkthdr* header = NULL;
	pcap_if_t* m_MyDev = NULL;
	pcap_t* m_NetworkDeviceHandler = NULL;
	char errbuf[PCAP_ERRBUF_SIZE] = "";											// *** 네트워크 에러 변수 선언
	
	CListCtrl m_NetworkInterfaceControlList;									// *** 컨트롤 리스트 변수 선언
	CListCtrl m_PacketDataControlList;											// *** 패킷 데이터 리스트 
	CTreeCtrl m_PacketInfoTree;
	CButton m_PauseButton;


	CRect rect;																	// *** 윈도우 크기 받아올 변수 선언
	CString strcnt;																// *** 리스트 순번 변수 선언
	CString print_str;
	// 컨트롤 리스트 출력할 변수
	CString m_CurrentTime;														// *** 시간 받아올 변수
	CString m_SourceIp;
	CString m_DestinationIp;
	CString m_Protocol;
	CString m_PacketLength;
	CString m_UDPPacketInfo;
	CString m_TCPPacketInfo;
	CString m_ARPPacketInfo;
	CString m_PacketSaveData;

	size_t click_index;															// *** 클릭한 인덱스 
	int ClickPacketFrameNumber = 0;
	int i;
	int net_dev_idx;

	BOOL is_PCThreadStart = FALSE;													// *** 스레드 체크 변수
	BOOL is_LOGThreadStart = FALSE;
	BOOL is_RunThreadOut = FALSE;

	CWinThread* m_PCThread;
	CWinThread* m_LOGThread;
	ThreadWorkingType m_eThreadWork = ThreadWorkingType::THREAD_STOP;

	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO StartupInfo = { 0 };												// *** 로그 서버 실행 변수 선언

	// *** 함수
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);					// *** 컨트롤 리스트에 색상 넣을 함수 선언
	static UINT ThreadClient(LPVOID param);												// *** 소켓 통신 스레드
	static UINT CNetworkPacketCaptureDlg::PacketCaptureTFunction(LPVOID _method);	// *** PC 스레드
	int SetPacketInfoTree(CString framecnt, CString time, CString protocol, CString lenth, CString savedata);
	int SetPacketHexList(CString data, CString protocol , int udpsize);
	afx_msg void OnTvnSelchangedPacketInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedClear();
	afx_msg void OnTbStartClickedWindows();												// *** 툴바 클릭 시 이벤트 함수
	afx_msg void OnTbStopClickedWindows();
	afx_msg void OnTbClearClickedWindows();
};
