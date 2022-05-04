// NetworkPacketCaptureDlg.h: 헤더 파일
//

#pragma once

#include <pcap/pcap.h>

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

	void InitToolBar();															// *** 툴바 생성 함수 선언
	// *** 구조체 
	typedef struct ether_addr
	{
		unsigned char e_host[6];
	}ETHER_ADDR;

	typedef struct ethernet_header
	{
		ETHER_ADDR dst;
		ETHER_ADDR src;
		unsigned short type;
	}ETHERNET_HEADER;
	ETHERNET_HEADER *m_EthernetHeader;											// *** ethernet header 변수 선언
	//ip_header 
	typedef struct ip_header
	{
		unsigned char header_len : 4;
		unsigned char ver : 4;
		unsigned char tos;
		unsigned short total_len;
		unsigned short id;
		unsigned char frag_offset : 5;
		unsigned char mf : 1;
		unsigned char df : 1;
		unsigned char rz : 1;
		unsigned char frag_offset1;
		unsigned char ttl;
		unsigned char protocol;
		//1=ICMP, 2=IGMP, 6=TCP, 8= EGP, 17=UDP, 89=OSPF 등등.....
		unsigned short cksum;
		IN_ADDR src_ip;
		IN_ADDR dst_ip;
	}IP_HEADER;
	IP_HEADER* m_IpHeader;
	size_t m_IpHeaderLen;
	typedef struct tcp_header
	{
		unsigned short src_port;
		unsigned short dst_port;
		unsigned int seq;
		unsigned int ack;
		unsigned char ns : 1;
		unsigned char reserved : 3;
		unsigned char data_offset : 4;
		unsigned char flag_fin : 1;
		unsigned char flag_syn : 1;
		unsigned char flag_rst : 1;
		unsigned char flag_psh : 1;
		unsigned char flag_ack : 1;
		unsigned char flag_urg : 1;
		unsigned char flag_ece : 1;
		unsigned char flag_cwr : 1;
		unsigned short window;
		unsigned short cksum;
		unsigned short up;

	}TCP_HEADER;
	TCP_HEADER* m_TCPHeader;
	typedef struct udp_header
	{
		unsigned short sport;
		unsigned short dport;
		unsigned short length;
		unsigned short checksum;
	}UDP_HEADER;
	UDP_HEADER* m_UDPHeader;
	enum class ThreadWorkingType {
		THREAD_STOP,
		THREAD_RUNNING,
		THREAD_PAUSE
	};

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
	CString m_PacketSaveData;

	size_t click_index;															// *** 클릭한 인덱스 
	int ClickPacketFrameNumber = 0;
	int i;
	int net_dev_idx;

	BOOL is_ClearButtonCheck = FALSE;
	BOOL is_PCThreadStart = FALSE;													// *** 스레드 체크 변수
	CWinThread* m_PCThread;
	ThreadWorkingType m_eThreadWork = ThreadWorkingType::THREAD_STOP;

	// *** 함수
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);					// *** 컨트롤 리스트에 색상 넣을 함수 선언

	static UINT CNetworkPacketCaptureDlg::PacketCaptureTFunction(LPVOID _method);
	int SetPacketInfoTree(CString framecnt, CString time,CString source, CString destination, CString protocol, CString lenth, CString savedata);
	int SetPacketHexList(CString data, CString protocol , int udpsize);
	afx_msg void OnTvnSelchangedPacketInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedClear();
	afx_msg void OnTbStartClickedWindows();												// *** 툴바 클릭 시 이벤트 함수
	afx_msg void OnTbStopClickedWindows();
	afx_msg void OnTbClearClickedWindows();
};
