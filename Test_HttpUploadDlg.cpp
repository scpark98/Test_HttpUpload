
// Test_HttpUploadDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Test_HttpUpload.h"
#include "Test_HttpUploadDlg.h"
#include "afxdialogex.h"

//#include <winhttp.h>
#include <afxinet.h>
#include "../../Common/Functions.h"

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


// CTestHttpUploadDlg 대화 상자



CTestHttpUploadDlg::CTestHttpUploadDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEST_HTTPUPLOAD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestHttpUploadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestHttpUploadDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTestHttpUploadDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTestHttpUploadDlg 메시지 처리기

BOOL CTestHttpUploadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CTestHttpUploadDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestHttpUploadDlg::OnPaint()
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
HCURSOR CTestHttpUploadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

std::wstring ErrorMessage(DWORD dwMessageId)
{
	HMODULE h = GetModuleHandle(_T("Winhttp"));
	constexpr DWORD dwSize = 512;
	HANDLE hHeap = GetProcessHeap();
	LPWSTR lpwszBuffer = static_cast<LPWSTR>(HeapAlloc(hHeap, 0, dwSize * sizeof(WCHAR)));
	if (lpwszBuffer) {
		if (0 == FormatMessageW(FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, h, dwMessageId, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), lpwszBuffer, dwSize, nullptr))
			_ltow_s(dwMessageId, lpwszBuffer, dwSize, 16);
		else {
			for (WCHAR* p; (p = wcschr(lpwszBuffer, L'\r')) != nullptr; *p = L' ') {}
			for (WCHAR* p; (p = wcschr(lpwszBuffer, L'\n')) != nullptr; *p = L' ') {}
		}

		std::wstring wsError = lpwszBuffer;
		HeapFree(hHeap, 0, lpwszBuffer);
		return wsError;
	}
	return std::wstring{};
}

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
void CTestHttpUploadDlg::OnBnClickedOk()
{
	//HttpUploadFile(L"http://54.180.43.235:4300/rest/saveImage", L"c:\\scpark\\media\\test_image\\HongKKonDai.jpg");
	if (HttpUploadFile(_T("http://54.180.43.235:4300/rest/saveImage"), _T("c:\\scpark\\media\\test_image\\idol.jpg"), 6340))
		AfxMessageBox(_T("Upload success."));
	else
		AfxMessageBox(_T("Upload failed."));
#if 0
	DWORD dwSize = 0;
	LPVOID lpOutBuffer = NULL;
	BOOL  bResults = FALSE;
	HINTERNET hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"A WinHTTP Example Program/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	std::wstring url{ L"http://54.180.43.235:4300/rest/saveImage" };
	URL_COMPONENTS components{};
	components.dwStructSize = sizeof(components);
	components.dwHostNameLength = (DWORD)-1;
	components.dwUrlPathLength = (DWORD)-1;
	if (!WinHttpCrackUrl(url.c_str(), static_cast<DWORD>(url.length()), 0, &components)) {
		wprintf((L"WinHttpCrackUrl(): " + ErrorMessage(GetLastError())).c_str());
	}

	std::wstring hostName(components.lpszHostName ? std::wstring{ components.lpszHostName, components.dwHostNameLength } : L"localhost");
	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect(hSession, hostName.c_str(),
			components.nPort, 0);

	// Create an HTTP request handle.
	if (hConnect)
		hRequest = WinHttpOpenRequest(hConnect, L"POST", components.lpszUrlPath,
			NULL, WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			0);

	const WCHAR* ContentType =
		L"Content-Type: multipart/form-data;boundary = 19024605111143684786787635207";

	std::ifstream in(L"c:\\scpark\\idol.jpg");
	std::stringstream buffer;
	buffer << in.rdbuf();
	std::string contents(buffer.str());

	std::string MultipartRequestBody =
		"--19024605111143684786787635207\r\n"
		"Content-Disposition: form-data; name=\"file\"; filename=\"Logfile\"\r\n"
		"Content-Type: image/jpeg\r\n"
		"\r\n";

	std::string finalBody = "--19024605111143684786787635207--\r\n";
	MultipartRequestBody += contents + "\r\n" + finalBody;

	bResults = WinHttpAddRequestHeaders(
		hRequest,
		ContentType,
		-1L,
		WINHTTP_ADDREQ_FLAG_ADD
	);

	bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
		WINHTTP_NO_REQUEST_DATA,
		0,
		MultipartRequestBody.length(),
		NULL);

	DWORD dwBytesWritten = 0;
	if (bResults)
		bResults = WinHttpWriteData(hRequest, MultipartRequestBody.c_str(),
			MultipartRequestBody.length(),
			&dwBytesWritten);

	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);

	if (bResults)
	{
		DWORD status{}, len = sizeof(status);
		bResults = WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, NULL, &status, &len, NULL);
		printf("Status code = %d.\n", status);
	}
	else
	{
		wprintf(L"WinHttpReceiveResponse(): %s\n", ErrorMessage(GetLastError()).c_str());
	}

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
#endif
#if 0
	BOOL bResults;

	HINTERNET hInternet = WinHttpOpen(L"sample", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	if (hInternet == NULL)
	{
		//utilLog.Write(LOG_LEVEL_RELEASE, _T("[ERROR] GetFileThread InternetOpen Fail=%d"), GetLastError());
		//mainDlg->PostMessage(UM_ERROR);
		return;
	}

	HINTERNET hConnect = WinHttpConnect(hInternet, L"54.180.43.235", 4300, 0);
		//InternetConnect(hInternet, "https://54.180.43.235", 4300, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (hConnect == NULL)
	{
		//utilLog.Write(LOG_LEVEL_RELEASE, _T("[ERROR] GetFileThread InternetConnect Fail=%d"), GetLastError());
		WinHttpCloseHandle(hInternet);
		//mainDlg->PostMessage(UM_ERROR);
		return;
	}
	/*
	int secureFlags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE; // http
	if (true)
	{
		//utilLog.Write(LOG_LEVEL_RELEASE, _T("[INFO] GetFileThread SecureFlags use HTTPS"));
		secureFlags = INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID; // https
	}
	*/

	HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", L"/rest/saveImage", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
	if (hRequest == NULL) {
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hInternet);
		return;
	}

	//LPCWSTR additionalHeaders = L"Accept: application/json\r\nContent-Type: multipart/form-data; boundary=----------------------------346435246262465368257857\r\n";
	LPCWSTR additionalHeaders = L"Content-Type: multipart/form-data; boundary=----------------------------346435246262465368257857\r\n";

	if (true)
	{
		char file[MAX_PATH] = "c:\\scpark\\idol.jpg";
		size_t filesize = GetFileSize(file);
		
		FILE* fp;
		fopen_s(&fp, file, "rb");

		char* fileData = new char[filesize];
		fread(fileData, 1, filesize, fp);

		fclose(fp);

		char* pszFormHeader = "------Boundary%08X\r\nContent-Disposition: form-data; name=\"uploaded\"; filename=\"%08x.dat\"\r\nContent-Type: application/octet-stream\r\n\r\n";
		char* pszFinalBoundary = "\r\n------Boundary%08X--\r\n";
		wchar_t* wszContentHeader = L"Content-Type: multipart/form-data; boundary=----Boundary%08X";
		wchar_t wszContentLength[256] = { 0 };


		WinHttpAddRequestHeaders(hRequest, additionalHeaders, -1L, WINHTTP_ADDREQ_FLAG_ADD);

		char postData1[] =
			"----------------------------346435246262465368257857\r\n"
			"Content-Disposition: form-data; name=\"file\"; filename=\"idol.jpg\"\r\n"
			//"Content-Disposition: form-data; chatIdx=\"6328\"; image=\"idol.jpg\"\r\n"
			//"Content-Disposition: form-data; name=\"file\"; \"chatIdx\"=\"6328\"; filename=\"idol.jpg\"; \"image\"=\"idol.jpg\"\r\n"
			//"Content-Disposition: form-data; name=\"file\"; chatIdx=\"6326\"; filename=\"temp.png\"; image=\"temp.png\"\r\n"
			"Content-Type: image/jpeg\r\n\r\n";
			"\r\n----------------------------346435246262465368257857--\r\n";
		char postData2[] =
			"\r\n----------------------------346435246262465368257857\r\n"
			"Content-Disposition: form-data; name=\"newData\"\r\n"
			"Content-Type: application/json\r\n\r\n"
			"{\"chatIdx\":\"6328\", \"image\":\"idol.jpg\"}"
			"\r\n----------------------------346435246262465368257857--\r\n";

		DWORD dwContentLength = strlen(additionalHeaders) + filesize ;
		wsprintf(wszContentLength, "Content-Length: %d", dwContentLength);

		if (hRequest)
			bResults = WinHttpSendRequest(hRequest,
				WINHTTP_NO_ADDITIONAL_HEADERS,
				0, WINHTTP_NO_REQUEST_DATA, 0,
				lstrlenA(postData1) + lstrlenA(postData2) + filesize, NULL);

		DWORD dwBytesWritten = 0;
		if (bResults)
			bResults = WinHttpWriteData(hRequest, postData1, lstrlenA(postData1), &dwBytesWritten);
		if (bResults)
			bResults = WinHttpWriteData(hRequest, (LPCVOID)fileData, filesize, &dwBytesWritten);
		if (bResults)
			bResults = WinHttpWriteData(hRequest, postData2, lstrlenA(postData2), &dwBytesWritten);

		if (bResults)
		{
			bResults = WinHttpReceiveResponse(hRequest, NULL);
			DWORD err = GetLastError();
		}
		else
		{
			DWORD err = GetLastError();
		}
	}

	//errorMessageID = ::GetLastError();
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hInternet) WinHttpCloseHandle(hInternet);
	// End the request.
#endif
}

/**
 * @ingroup HttpFileUpload
 * @brief HTTP POST 메소드로 파일을 업로드한다.
 * @param pszUrl   HTTP URL
 * @param pszFilePath 업로드 파일 full path
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CTestHttpUploadDlg::HttpUploadFile(CString url, CString filepath, int chatIndex)
{
	bool bRes = false;

	USES_CONVERSION;

	WCHAR* pszUrl;
	WCHAR* pszFilePath;
	
//#ifdef _UNICODE
//	pszUrl = url;

	pszUrl = T2W(url);
	pszFilePath = T2W(filepath);

	// pszUrl 에서 host, path 를 가져온다.
	WCHAR* pszHost = NULL;
	DWORD dwFlag = 0;
	int iPort = 80;

	if (!_wcsnicmp(pszUrl, L"http://", 7))
	{
		pszHost = pszUrl + 7;
	}
	else if (!_wcsnicmp(pszUrl, L"https://", 8))
	{
		pszHost = pszUrl + 8;
		dwFlag = INTERNET_FLAG_SECURE;
		iPort = 443;
	}
	else
	{
		return false;
	}

	const WCHAR* pszPath = wcsstr(pszHost, L"/");
	if (pszPath == NULL)
	{
		return false;
	}

	std::wstring strHost;
	strHost.append(pszHost, pszPath - pszHost);
	const WCHAR* pszPort = wcsstr(strHost.c_str(), L":");
	if (pszPort)
	{
		iPort = _wtoi(pszPort + 1);
		if (iPort <= 0) return false;
		strHost.erase(pszPort - strHost.c_str());
	}

	// 파일 경로에서 파일 이름을 가져온다.
	int iLen = wcslen(pszFilePath);
	WCHAR* pszFileName = NULL;
	for (int i = iLen - 1; i >= 0; --i)
	{
		if (pszFilePath[i] == L'\\')
		{
			pszFileName = pszFilePath + i + 1;
			break;
		}
	}

	// 파일 크기를 가져온다.
	struct _stat sttStat;
	if (_wstat(pszFilePath, &sttStat) == -1)
	{
		return false;
	}

	FILE* fd;
	_wfopen_s(&fd, pszFilePath, L"rb");
	if (fd == NULL)
	{
		return false;
	}
	else
	{
		char szBuf[8192];
		CInternetSession clsSession;

		// HTTP 연결하고 파일을 전송한다.
		CHttpConnection* pclsHttpConn = clsSession.GetHttpConnection(CString(strHost.c_str()), dwFlag, (INTERNET_PORT)iPort, NULL, NULL);
		if (pclsHttpConn)
		{
			CHttpFile* pclsHttpFile = pclsHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_POST, CString(pszPath));
			if (pclsHttpFile)
			{
				USES_CONVERSION;
				std::string strBoundary = "55CA7C5681214d98912C76366A8042BA";

				// HTTP 요청 header 를 생성한다.
				std::wstring strContentType = L"Content-Type: multipart/form-data; boundary=";
				//strContentType.append(A2T(strBoundary.c_str()));
				strContentType.append(strBoundary.begin(), strBoundary.end());
				strContentType.append(L"\r\n");
				pclsHttpFile->AddRequestHeaders(CString(strContentType.c_str()));// .c_str());
				
				std::string strBody;
				std::string strChatIndex = std::to_string(chatIndex);

				// body에 chatIdx를 저장한다.
				strBody.append("--");
				strBody.append(strBoundary);
				strBody.append("\r\n");
				strBody.append("Content-Disposition: form-data; name=\"chatIdx\"\r\n\r\n");
				strBody.append(strChatIndex);
				strBody.append("\r\n");

				// body에 파일명을 저장한다.
				strBody.append("--");
				strBody.append(strBoundary);
				strBody.append("\r\n");
				strBody.append("Content-Disposition: form-data; name=\"image\"; filename=\"");
				std::wstring filename(pszFilePath);
				strBody.append(filename.begin(), filename.end());
				strBody.append("\"\r\nContent-Type: application/octet-stream\r\n\r\n");

				while (1)
				{
					iLen = fread(szBuf, 1, sizeof(szBuf), fd);
					if (iLen <= 0) break;
					strBody.append(szBuf, iLen);
				}
				fclose(fd);

				strBody.append("\r\n");
				strBody.append("--");
				strBody.append(strBoundary);
				strBody.append("--\r\n");

				try
				{
					// HTTP 요청 header 를 전송한다.
					pclsHttpFile->SendRequestEx(strBody.length(), HSR_SYNC | HSR_INITIATE);

					// HTTP 요청 body 를 전송한다.
					pclsHttpFile->Write(strBody.c_str(), strBody.length());
					pclsHttpFile->EndRequest(HSR_SYNC);

					// HTTP 응답 body 를 수신한다.
					std::string strResponse;
					DWORD dwCode;
					while (1)
					{
						iLen = pclsHttpFile->Read(szBuf, sizeof(szBuf));
						if (iLen <= 0) break;
						strResponse.append(szBuf, iLen);
					}

					// 응답 코드가 200 OK 인지 확인한다.
					pclsHttpFile->QueryInfoStatusCode(dwCode);
					if (dwCode == HTTP_STATUS_OK)
					{
						bRes = true;
					}
				}
				catch (CInternetException* pclsException)
				{
					// 웹서버 연결에 실패하면 CInternetException 이 발생한다.
				}

				delete pclsHttpFile;
			}
			delete pclsHttpConn;
		}
		fclose(fd);
	}
	return bRes;
}
