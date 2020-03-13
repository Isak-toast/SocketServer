
// SocketServerDlg.h: 헤더 파일
//

#pragma once

#include "CListenSocket.h"
// CSocketServerDlg 대화 상자

#pragma pack(push, 1)
typedef struct  _LPRGAZE_PACKET {
	unsigned char preamble[2];
	unsigned int totalLength;
	unsigned char msgType;
	unsigned char pData[255]; //tail 포함
	unsigned char parity;
} LPRGAZE_PACKET;
#pragma pack(pop)

class CSocketServerDlg : public CDialogEx
{
private:
	//CImage* m_hBitmap;
	CStatic m_jpgImage;
	CImage image, Win_image;
	CFileFind find;
	CString pathname;
	int index=0;
// 생성입니다.
public:
	CSocketServerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	CListenSocket m_ListenSocket;
	CListBox* clientList;
	BOOL popuptest=FALSE;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOCKETSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	void OnDestroy();

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_List;
	afx_msg void OnLbnSelchangeClientList();
	void BeginWaitCursor(DWORD dwMillisecond);
	afx_msg void OnBnClickedButton1();
	void Wait(DWORD dwMillisecond);
	void FileList();
	void MessageBox_control(CString strBuffer);
	CListBox m_ListBox;
};
