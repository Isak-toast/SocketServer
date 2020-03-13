#pragma once
#include "SocketServerDlg.h"
// CClientSocket 명령 대상

class CClientSocket : public CSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();

	CSocketServerDlg socketserver;
	CAsyncSocket* m_pListenSocket;
	void SetListenSocket(CAsyncSocket* pSocket);
	void OnClose(int nErrorCode);
	void OnReceive(int nErrorCode);
};


