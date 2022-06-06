#pragma once

#define FAIL	-1
// ChangeColor 대화 상자

class ChangeColor : public CDialogEx
{
	DECLARE_DYNAMIC(ChangeColor)

public:
	ChangeColor(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ChangeColor();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CListCtrl m_ListCtrl;					// *** 리스트 컨트롤 변수 
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult);

	CString tcp, tcpR, tcpG, tcpB,
		udp, udpR, udpG, udpB,
		ssdp, ssdpR, ssdpG, ssdpB,
		arp, arpR, arpG, arpB,
		dns, dnsR, dnsG, dnsB,
		tls, tlsR, tlsG, tlsB,
		tmp;
};
