#pragma once
#include <pcap/pcap.h>

// ChoiceNetworkInterface 대화 상자

class ChoiceNetworkInterface : public CDialog
{
	DECLARE_DYNAMIC(ChoiceNetworkInterface)

public:
	ChoiceNetworkInterface(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ChoiceNetworkInterface();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHOICENETWORKINTERFACE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	HICON m_hIcon;

	DECLARE_MESSAGE_MAP()
public:
	pcap_if_t* alldevs =NULL;														// *** 네트워크 디바이스 조회 변수 선언
	pcap_if_t* choice_dev;														// *** 네트워크 디바이스 선택 변수 선언
	char errbuf[PCAP_ERRBUF_SIZE];												// *** 네트워크 에러 변수 선언
	CListCtrl m_Netwrok_Interface;
	CBrush m_brush;																// *** 컬러 지정할 브러시 변수 선언

	CRect rect;																	// *** 윈도우 크기 받아올 변수 선언
	CString strcnt;																// *** 리스트 순번 변수 선언
	size_t click_index;
	int i;
	// *** 함수
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CProgressCtrl m_Progress;
	afx_msg void OnNMDblclkNetwrokinterface(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickNetwrokinterface(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
