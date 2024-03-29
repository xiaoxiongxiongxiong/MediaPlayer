﻿
// MediaPlayerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MediaPlayer.h"
#include "MediaPlayerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMediaPlayerDlg 对话框



CMediaPlayerDlg::CMediaPlayerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_PLAYER, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMediaPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_PLAY, m_lstRecord);
	DDX_Control(pDX, IDC_BTN_CTRL, m_btnCtrl);
	DDX_Control(pDX, IDC_BTN_STOP, m_btnStop);
	DDX_Control(pDX, IDC_BTN_FORWARD, m_btnForward);
	DDX_Control(pDX, IDC_BTN_BACKWARD, m_btnBackward);
	DDX_Control(pDX, IDC_BTN_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_STC_DURATION, m_stcDuration);
	DDX_Control(pDX, IDC_BTN_LAST, m_btnLast);
	DDX_Control(pDX, IDC_BTN_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_BTN_VOICE, m_btnVoice);
}

BEGIN_MESSAGE_MAP(CMediaPlayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_CTRL, &CMediaPlayerDlg::OnBnClickedBtnCtrl)
	ON_BN_CLICKED(IDC_BTN_STOP, &CMediaPlayerDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_FORWARD, &CMediaPlayerDlg::OnBnClickedBtnForward)
	ON_BN_CLICKED(IDC_BTN_BACKWARD, &CMediaPlayerDlg::OnBnClickedBtnBackward)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CMediaPlayerDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_LAST, &CMediaPlayerDlg::OnBnClickedBtnLast)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CMediaPlayerDlg::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_VOICE, &CMediaPlayerDlg::OnBnClickedBtnVoice)
END_MESSAGE_MAP()


// CMediaPlayerDlg 消息处理程序

BOOL CMediaPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_toolTip.Create(this);
    m_toolTip.SetDelayTime(TTDT_INITIAL, 10);
	m_toolTip.SetDelayTime(TTDT_AUTOPOP, 30000);
    m_toolTip.SetMaxTipWidth(200);
    m_toolTip.SetTipTextColor(FONT_COLOR);

	m_toolTip.AddTool(GetDlgItem(IDC_BTN_CTRL), _T("播放/暂停"));
	m_toolTip.AddTool(GetDlgItem(IDC_BTN_STOP), _T("停止"));
	m_toolTip.AddTool(GetDlgItem(IDC_BTN_FORWARD), _T("快进"));
	m_toolTip.AddTool(GetDlgItem(IDC_BTN_BACKWARD), _T("快退"));
    m_toolTip.AddTool(GetDlgItem(IDC_BTN_LAST), _T("上一个"));
    m_toolTip.AddTool(GetDlgItem(IDC_BTN_NEXT), _T("下一个"));
    m_toolTip.AddTool(GetDlgItem(IDC_BTN_VOICE), _T("静音 开/关"));
	m_toolTip.AddTool(GetDlgItem(IDC_BTN_OPEN), _T("打开"));

	m_fontSize.CreatePointFont(120, _T("微软雅黑"), nullptr);
	GetDlgItem(IDC_STC_TITLE)->SetFont(&m_fontSize);
	m_lstRecord.SetFont(&m_fontSize);
	m_lstRecord.SetBkColor(BACK_COLOR);
	m_lstRecord.SetTextBkColor(BACK_COLOR);
	m_lstRecord.SetTextColor(FONT_COLOR);
	m_lstRecord.InsertItem(0, _T("张韶涵-阿刁"));
	m_brush.CreateSolidBrush(BACK_COLOR);
	m_stcDuration.SetFont(&m_fontSize);

	CRect btnRect;
	m_btnOpen.GetWindowRect(&btnRect);
	m_icoPause = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICO_PAUSE), IMAGE_ICON,
		btnRect.Width() - GAP_SIZE, btnRect.Height() - GAP_SIZE, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
    m_icoStart = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICO_PLAY), IMAGE_ICON,
        btnRect.Width() - GAP_SIZE, btnRect.Height() - GAP_SIZE, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
    m_icoStop = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICO_STOP), IMAGE_ICON,
        btnRect.Width() - GAP_SIZE, btnRect.Height() - GAP_SIZE, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
    m_icoForward = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICO_FORWARD), IMAGE_ICON,
        btnRect.Width() - GAP_SIZE, btnRect.Height() - GAP_SIZE, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
    m_icoBackward = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICO_BACKWARD), IMAGE_ICON,
        btnRect.Width() - GAP_SIZE, btnRect.Height() - GAP_SIZE, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
    m_icoLast = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICO_LAST), IMAGE_ICON,
        btnRect.Width() - GAP_SIZE, btnRect.Height() - GAP_SIZE, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
    m_icoNext = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICO_NEXT), IMAGE_ICON,
        btnRect.Width() - GAP_SIZE, btnRect.Height() - GAP_SIZE, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
    m_icoVoice = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICO_VOICE), IMAGE_ICON,
        btnRect.Width() - GAP_SIZE, btnRect.Height() - GAP_SIZE, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
    m_icoSilence = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICO_SILENCE), IMAGE_ICON,
        btnRect.Width() - GAP_SIZE, btnRect.Height() - GAP_SIZE, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
    m_icoOpen = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICO_FOLDER), IMAGE_ICON,
        btnRect.Width() - GAP_SIZE, btnRect.Height() - GAP_SIZE, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
	m_btnCtrl.SetIcon(m_icoPause);
	m_btnStop.SetIcon(m_icoStop);
	m_btnForward.SetIcon(m_icoForward);
	m_btnBackward.SetIcon(m_icoBackward);
	m_btnLast.SetIcon(m_icoLast);
	m_btnNext.SetIcon(m_icoNext);
	m_btnVoice.SetIcon(m_icoVoice);
    m_btnOpen.SetIcon(m_icoOpen);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMediaPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMediaPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
        CRect rect;
        CPaintDC dc(this);
        GetClientRect(rect);
        dc.FillSolidRect(rect, BACK_COLOR);  //设置背景颜色
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMediaPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



HBRUSH CMediaPlayerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkColor(BACK_COLOR);
		return (HBRUSH)m_brush.GetSafeHandle();
	}

	return hbr;
}


void CMediaPlayerDlg::OnBnClickedBtnCtrl()
{
	if (nullptr == m_pMediaPtr)
	{
		MessageBox(_T("没有可播放的文件"), _T("提示"), MB_OK | MB_ICONWARNING);
		return;
	}

	static bool is_play = false;
	if (!is_play)
    {
        CRect rect;
        CWnd * pScreen = GetDlgItem(IDC_STC_SCREEN);
        pScreen->GetClientRect(&rect);

        if (!m_pMediaPtr->start(pScreen->GetSafeHwnd(), rect.Width(), rect.Height()))
        {
            MessageBox(_T("播放文件失败！"), _T("提示"), MB_OK | MB_ICONWARNING);
            return;
        }
        m_btnCtrl.SetIcon(m_icoStart);
		
		int64_t duration = m_pMediaPtr->getVideoDuration();
		int a = 0;

    }
	else
    {
		m_pMediaPtr->pause();
        m_btnCtrl.SetIcon(m_icoPause);
    }
	is_play = !is_play;
}


void CMediaPlayerDlg::OnBnClickedBtnStop()
{
	m_pMediaPtr->stop();
}


void CMediaPlayerDlg::OnBnClickedBtnForward()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMediaPlayerDlg::OnBnClickedBtnBackward()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMediaPlayerDlg::OnBnClickedBtnOpen()
{
	if (m_pMediaPtr)
	{
		MessageBox(_T("请先关闭当前文件！"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}

	CString strExtFilter = _T("MP4|*.mp4|MKV|*.mkv|All Files|*.*||");
	CFileDialog dlg(TRUE, _T("MP4"), nullptr, OFN_READONLY | OFN_FILEMUSTEXIST, strExtFilter);
	if (IDOK == dlg.DoModal())
	{
        CString strFilePath = dlg.GetPathName();

		m_pMediaPtr = new CMediaPlayerImpl();
		if (nullptr == m_pMediaPtr)
		{
			MessageBox(_T("打开文件失败！"), _T("提示"), MB_OK | MB_ICONWARNING);
			return;
		}

		char filename[MAX_PATH] = { 0 };
		memcpy(filename, strFilePath.GetBuffer(strFilePath.GetLength()), MAX_PATH);
		m_strFilePath = filename;

		if (!m_pMediaPtr->open(filename))
		{
            MessageBox(_T("打开文件失败！"), _T("提示"), MB_OK | MB_ICONWARNING);
			delete m_pMediaPtr;
			m_pMediaPtr = nullptr;
            return;
		}
	}
}


BOOL CMediaPlayerDlg::PreTranslateMessage(MSG * pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;

    if (m_toolTip.m_hWnd != NULL)
    {
        m_toolTip.RelayEvent(pMsg);
    }

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMediaPlayerDlg::OnBnClickedBtnLast()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMediaPlayerDlg::OnBnClickedBtnNext()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMediaPlayerDlg::OnBnClickedBtnVoice()
{
    static bool is_silence = true;
    if (!is_silence)
        m_btnVoice.SetIcon(m_icoVoice);
    else
        m_btnVoice.SetIcon(m_icoSilence);
	is_silence = !is_silence;
}
