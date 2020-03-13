// CClientSocket.cpp: 구현 파일
//

#include "pch.h"
#include "SocketServer.h"
#include "CClientSocket.h"
#include "CListenSocket.h"
#include "SocketServerDlg.h"

// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}


// CClientSocket 멤버 함수
void CClientSocket::SetListenSocket(CAsyncSocket* pSocket)
{
	m_pListenSocket = pSocket;
}
// CClientSocket 멤버 함수


void CClientSocket::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);

	CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
	pServerSocket->CloseClientSocket(this);
}
void temp()
{
	AfxMessageBox(_T("hello"), MB_YESNO);
}
void CClientSocket::OnReceive(int nErrorCode)
{
	CString strTmp = _T(""), strIPAddress = _T("");
	UINT uPortNumber = 0;
	unsigned char strBuffer[1024];
	wchar_t strResult[1024];

	::ZeroMemory(strBuffer, sizeof(strBuffer)); // :: 붙이고 안붙이고 차이 알아보기
	//
	GetPeerName(strIPAddress, uPortNumber);
	if (Receive(strBuffer, sizeof(strBuffer)) > 0) { // 전달된 데이터(문자열)가 있을 경우
		CSocketServerDlg* pMain = (CSocketServerDlg*)AfxGetMainWnd();
		strTmp.Format(_T("[%s:%d]: %s"), strIPAddress, uPortNumber, strBuffer);
		pMain->m_List.AddString(strTmp);  // 메시지 리스트(메시지창?)에 입력받은 메시지 띄우기
		pMain->m_List.SetCurSel(pMain->m_List.GetCount() - 1);
		//int nLen = WideCharToMultiByte(CP_UTF8, 0, strBuffer, lstrlenW(strBuffer), NULL, 0, NULL, NULL);
		//WideCharToMultiByte(CP_UTF8, 0, strBuffer, lstrlenW(strBuffer), (LPSTR)strResult, nLen, NULL, NULL);
		//temp();
		//CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
		//pServerSocket->SendAllMessage(strBuffer); // 다른 클라이언트들에게 메시지 전달

		//AfxMessageBox(_T("hello"), MB_YESNO);
		//pMain->PostMessage(UM_MYMESSAGE, 0, 0);
		pMain->MessageBox_control(strBuffer);
		//팝업창띄우고  o,x 확인해서 결과를 csv파일로 저장
		//WideCharToMultiByte(CP_ACP, MB_PRECOMPOSED, strBuffer, 1024, strResult, 1024, NULL, NULL);

		//FILE* fp;

	}

	CSocket::OnReceive(nErrorCode);
}