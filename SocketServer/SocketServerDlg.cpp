
// SocketServerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SocketServer.h"
#include "SocketServerDlg.h"
#include "afxdialogex.h"
#include "CClientSocket.h"
#include "atlimage.h"

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


// CSocketServerDlg 대화 상자



CSocketServerDlg::CSocketServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SOCKETSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_hBitmap = NULL;
}

void CSocketServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_LIST2, m_ListBox);
	DDX_Control(pDX, IDC_STATIC_JPG, m_jpgImage);
}

BEGIN_MESSAGE_MAP(CSocketServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_SELCHANGE(IDC_CLIENT_LIST, &CSocketServerDlg::OnLbnSelchangeClientList)
	ON_BN_CLICKED(IDC_BUTTON1, &CSocketServerDlg::OnBnClickedButton1)
	ON_MESSAGE(UM_MYMESSAGE, &CSocketServerDlg::OnMyMessage)
END_MESSAGE_MAP()


// CSocketServerDlg 메시지 처리기

BOOL CSocketServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.
	clientList = (CListBox*)GetDlgItem(IDC_CLIENT_LIST);
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
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.


	CString strImagePath = _T("C:\\projects\\LPRgaze\\000709.jpg");

	Win_image.Load(strImagePath);

	TRACE(_T("Width: %d, Height: %d\n"), Win_image.GetWidth(), Win_image.GetHeight());

	this->SetWindowPos(&CWnd::wndTop, 0, 0, Win_image.GetWidth() + 400, Win_image.GetHeight(), SWP_NOREPOSITION);

	//GetDlgItem(IDC_LIST1)->SetWindowPos(NULL, 1300, 500, 300, 500, SWP_NOMOVE);
	//m_pImage = new CImage;
	/*
	CDC* pDC = m_jpgImage.GetDC();//dc 가져옴
	CRect cRect;//picture control 사각형좌표를 위한 crect 선언
	m_jpgImage.GetClientRect(cRect);//crect 연결
	CBrush cbWhite(RGB(255, 255, 255));//브러쉬 선언
	pDC->FillRect(cRect, &cbWhite);//지정된 브러쉬로 지정된 사각형 채움
	ReleaseDC(pDC);//DC 제거
	*/
	
	if (m_ListenSocket.Create(21000, SOCK_STREAM)) { // 소켓생성
		if (!m_ListenSocket.Listen()) {
			AfxMessageBox(_T("ERROR:Listen() return False"));
		}
	}
	else {
		AfxMessageBox(_T("ERROR:Failed to create server socket!"));
	}
	FileList();
	

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSocketServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSocketServerDlg::OnPaint()
{
	CDialogEx::OnPaint();
	//CPaintDC dc(this);
	//Invalidate(false);
	/*
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
	}*/
	
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSocketServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSocketServerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	POSITION pos;

	pos = m_ListenSocket.m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;

	// 생성되어있는 클라이언트 소켓이 없을때까지 체크하여 소켓닫기
	while (pos != NULL) {
		pClient = (CClientSocket*)m_ListenSocket.m_ptrClientSocketList.GetNext(pos);
		if (pClient != NULL) {
			pClient->ShutDown(); // 연결된 상대방 소켓에 연결이 종료됨을 알린다. 
			pClient->Close(); // 소켓을 닫는다

			delete pClient;
		}
	}
	m_ListenSocket.ShutDown();
	m_ListenSocket.Close();
}



void CSocketServerDlg::OnLbnSelchangeClientList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}
void CSocketServerDlg::Wait(DWORD dwMillisecond) {
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();

	while (GetTickCount() - dwStart < dwMillisecond)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}
void CSocketServerDlg::FileList() {

	CString strImagePath = _T("C:\\projects\\LPRgaze\\image\\test\\*.*");
	CString pathname, fname;

	CFileFind find;

	BOOL filefound = find.FindFile(strImagePath);


	while (filefound) {
		filefound = find.FindNextFile();

		if (find.IsArchived()) {
			fname = find.GetFileName();
			pathname = find.GetFilePath();
			m_ListBox.AddString(pathname);
			TRACE(fname + _T("\n"));
			TRACE(pathname + _T("\n"));
		}
		else
			continue;
	}
}
void CSocketServerDlg::MessageBox_control(CString strBuffer) {

	LPRGAZE_PACKET* m_packet = new LPRGAZE_PACKET;

	m_packet->preamble[0] = strBuffer[0];
	m_packet->preamble[1] = strBuffer[1];
	m_packet->totalLength = strBuffer[2] + strBuffer[3] + strBuffer[4] + strBuffer[5];
	m_packet->msgType = strBuffer[6];

	int Answer;
	Answer = AfxMessageBox(strBuffer, MB_YESNO);
	if (Answer == IDYES) {
		AfxMessageBox(_T("yes"));

	}
	else {
		//fopen_s(&fp, "test.csv", "w+");
		//fprintf(fp, strResult);

		AfxMessageBox(_T("no"));
	}

}
LRESULT CSocketServerDlg::OnMyMessage(WPARAM wParam, LPARAM lParam) 
{
	AfxMessageBox(_T("Hello!!"));
	TRACE(_T("test"));
	Invalidate(false);
	return 0;
}

void CSocketServerDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CPaintDC dc(this);
	/*
	int Answer;
	Answer = AfxMessageBox(_T("111111"), MB_YESNO|MB_APPLMODAL);
	if (Answer == IDYES) {
		AfxMessageBox(_T("yes"));

	}
	else {
		//fopen_s(&fp, "test.csv", "w+");
		//fprintf(fp, strResult);

		AfxMessageBox(_T("no"));
	}
	*/
	//AfxMessageBox(_T("Hello!!"));
	OnPaint();
	CString filename;
	m_ListBox.GetText(index, filename);
	TRACE(_T("%d\n"), index);
	TRACE(filename + _T("\n"));
	HRESULT hResult = image.Load(filename);
	index++;
	if (FAILED(hResult)) {
		CString strcmp = _T("ERROR: fail to load ");
		strcmp += filename + _T("\n");
		TRACE(strcmp);
		return;
	}

	int h = image.GetHeight();
	int w = image.GetWidth();

	CDC* pDC = m_jpgImage.GetDC();//dc 가져옴
	CRect cRect;//picture control 사각형좌표를 위한 crect 선언
	m_jpgImage.GetClientRect(cRect);//crect 연결
	int size = cRect.Width();
	int sz_gap = (cRect.Height() - cRect.Width()) / 2;
	CBrush cbWhite(RGB(255, 255, 255));//브러쉬 선언
	pDC->FillRect(cRect, &cbWhite);//지정된 브러쉬로 지정된 사각형 설정
	if (h > w)
		//		cImage.StretchBlt(pDC->m_hDC,(size-w*size/h)/2,sz_gap,w*size/h,size,SRCCOPY);
		image.TransparentBlt(pDC->m_hDC, (size - w * size / h) / 2, sz_gap, w * size / h, size, RGB(192, 192, 192));
	else
		//		cImage.StretchBlt(pDC->m_hDC,0,sz_gap+(size-h*size/w)/2,w*size/w,h*size/w,SRCCOPY);
		image.TransparentBlt(pDC->m_hDC, 0, sz_gap + (size - h * size / w) / 2, w * size / w, h * size / w, RGB(192, 192, 192));
		//image.ReleaseDC();
	image.Destroy();
	ReleaseDC(pDC);
	
}


