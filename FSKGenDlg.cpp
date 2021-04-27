// FSKGenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FSKGen.h"
#include "FSKGenDlg.h"
#include "General.h"
#include "FSKBitStrGen.h"
#include "waveGen.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFSKGenDlg dialog




CFSKGenDlg::CFSKGenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFSKGenDlg::IDD, pParent)
	, m_typeRadio(0)
	, m_nameRadio(0)
	, m_numberRadio(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFSKGenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MONTH, m_month);
	DDX_Control(pDX, IDC_DAY, m_day);
	DDX_Control(pDX, IDC_HOUR, m_hour);
	DDX_Control(pDX, IDC_MINUTE, m_minute);
	DDX_Control(pDX, IDC_SAMPLING_FREQUNCY, m_samplingFreq);
	DDX_Radio(pDX, IDC_MDMF, m_typeRadio);
	DDX_Radio(pDX, IDC_NAME_RADIO, m_nameRadio);
	DDX_Radio(pDX, IDC_NUMBER_PRESENT_RADIO, m_numberRadio);
}

BEGIN_MESSAGE_MAP(CFSKGenDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SETFILEPATH, &CFSKGenDlg::OnBnClickedSetfilepath)
	ON_BN_CLICKED(IDC_MDMF, &CFSKGenDlg::OnBnClickedMdmf)
	ON_BN_CLICKED(IDC_SDMF, &CFSKGenDlg::OnBnClickedSdmf)
	ON_BN_CLICKED(IDC_GENERATE, &CFSKGenDlg::OnBnClickedGenerate)
END_MESSAGE_MAP()


// CFSKGenDlg message handlers

BOOL CFSKGenDlg::OnInitDialog()
{
	USES_CONVERSION;
	char stemp[20];
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	// TODO: Add extra initialization here
	m_month.AddString(L"Jan");
	m_month.AddString(L"Feb");
	m_month.AddString(L"Mar");
	m_month.AddString(L"Apr");
	m_month.AddString(L"May");
	m_month.AddString(L"Jun");
	m_month.AddString(L"Jul");
	m_month.AddString(L"Aug");
	m_month.AddString(L"Sep");
	m_month.AddString(L"Oct");
	m_month.AddString(L"Nov");
	m_month.AddString(L"Dec");
	m_month.SetCurSel(0);
	for(int i=1; i < 32; i++)
	{
		_itoa(i,stemp,10);
		m_day.AddString(A2T(stemp));
	}
	m_day.SetCurSel(0);
	for(int i=0; i < 24; i++)
	{
		_itoa(i,stemp,10);
		m_hour.AddString(A2T(stemp));
	}
	m_hour.SetCurSel(0);
	for(int i=0; i < 60; i++)
	{
		_itoa(i,stemp,10);
		m_minute.AddString(A2T(stemp));
	}
	m_minute.SetCurSel(0);

	m_samplingFreq.AddString(L"8000");
	sampleRate[0]	= 8000;
	m_samplingFreq.AddString(L"16000");
	sampleRate[1]   = 16000;
	m_samplingFreq.AddString(L"22050");
	sampleRate[2]   = 22050;
//	m_samplingFreq.AddString("44100");
//	sampleRate[3]   = 44100;
	m_samplingFreq.SetCurSel(0);
	m_typeRadio		= 0;
	m_nameRadio		= 0;
	m_numberRadio	= 0;
	UpdateData();
	CString temp;
	temp.Format(L"FSK Generator program (Version %d.%d) --- www.dspcom.ir",MAJOR,MINOR);
	SetWindowText(temp);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFSKGenDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFSKGenDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFSKGenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFSKGenDlg::OnBnClickedSetfilepath()
{
	// TODO: Add your control notification handler code here
   TCHAR szFilters[]=
      L"Type Files (*.Wav)|*.wav|All Files (*.*)|*.*||";
   CFileDialog fileDlg (FALSE, L"Wav", L"*.Wav",
   OFN_HIDEREADONLY, szFilters, this);
   if( fileDlg.DoModal ()==IDOK )
   {
      pathName = fileDlg.GetPathName();
	  SetDlgItemText(IDC_FILE_PATH,(LPCTSTR)pathName);
   }		
}

void CFSKGenDlg::OnBnClickedMdmf()
{
	// TODO: Add your control notification handler code here
	processType(true);
}

void CFSKGenDlg::OnBnClickedSdmf()
{
	// TODO: Add your control notification handler code here
	processType(false);
}

void CFSKGenDlg::processType(bool param)
{
	GetDlgItem(IDC_NAME_RADIO)->EnableWindow(param);
	GetDlgItem(IDC_NOT_PRESENT_RADIO)->EnableWindow(param);
	GetDlgItem(IDC_NO_NAME_RADIO)->EnableWindow(param);
	GetDlgItem(IDC_NAME)->EnableWindow(param);	
}

void CFSKGenDlg::OnBnClickedGenerate()
{
	// TODO: Add your control notification handler code here
	uint8* data;
	uint32 size;
	if(CheckFileName()==false)
		return;

	GetDateAndTime();// for both type.
	if(GetTellNum()==false)
	{
		MessageBox(L"Input phone number.");
		return ;
	}
	switch(GetCIDType())
	{
		case MODE_MDMF:
			if(GetNameData()==false)
			{
				MessageBox(L"Input name.");
				return;
			}
			cid_type = MODE_MDMF;
		break;
		case MODE_SDMF:
			cid_type = MODE_SDMF;
		break;
		default:
		return;
	}
	m_fskGen = new CFSKBitStrGen();
	m_waveGen = new CWaveGen();
	m_fskGen->setMode(GetCIDType());
	m_fskGen->setDate(month,day);
	m_fskGen->setTime(hour,minute);
	m_fskGen->setTelNum(telNum);
	m_fskGen->setSampleRate(sampleRate[m_samplingFreq.GetCurSel()]);
	m_fskGen->setName((uint8*)name);
	m_fskGen->Generate();
	size = m_fskGen->GetBufferSize();
	data = new uint8[size];
	m_fskGen->GetData((uint16*)data);

	m_waveGen->SetFileName(path);
	m_waveGen->SetSampleRate(sampleRate[m_samplingFreq.GetCurSel()]);
	m_waveGen->SetWaveType(SAMPLE_TYPE_16BIT_PCM_INTEL);
	if(m_waveGen->OpenWaveFile()==FAILURE)
	{
		MessageBox(L"Wave File Opening Error.");
		delete  m_waveGen;
		delete	m_fskGen;
		return;
	}
	if(m_waveGen->WriteHeaderWave(0) == FAILURE)
	{
		MessageBox(L"Wave File writing Error.");
		delete  m_waveGen;
		delete	m_fskGen;
		return;
	}
	// write data into file.
//	m_waveGen->WriteData();
	m_waveGen->WriteData((void*)data,size);
	m_waveGen->WriteWaveSize();
	m_waveGen->CloseWaveFile();
	MessageBox(L"File created successfully.");
	delete []data;
	delete  m_waveGen;
	delete	m_fskGen;
}

bool CFSKGenDlg::CheckFileName(void)
{
	USES_CONVERSION;
	TCHAR wpath[MAX_PATH];
	GetDlgItemText(IDC_FILE_PATH,wpath,MAX_PATH);
	strcpy(path, T2A(wpath));
	if( strlen(path) == 0 )
	{
		MessageBox(L"First set output file name.");
		return false;
	}
	return true;
}

int CFSKGenDlg::GetCIDType(void)
{
	UpdateData(true);
	return m_typeRadio;
}

void CFSKGenDlg::GetDateAndTime(void)
{
	month  = m_month.GetCurSel() + 1;
	day    = m_day.GetCurSel()   + 1;
	hour   = m_hour.GetCurSel()     ;
	minute = m_minute.GetCurSel()   ;
}

bool CFSKGenDlg::GetTellNum(void)
{
	USES_CONVERSION;
	UpdateData(true);
	switch(m_numberRadio)
	{
	case NUMBER_PRESENT:
		TCHAR tn[255];
		GetDlgItemText(IDC_NUMBER,tn,255);
		strcpy((char*)telNum, T2A(tn));
		
		if(strlen((char*)telNum)==0)
			return false;
		break;
	case NUMBER_NOT_PRESENT:
		telNum[0] = 'O';
		telNum[1] = 0;
		break;
	}

	return true;
}

bool CFSKGenDlg::GetNameData(void)
{
	USES_CONVERSION;
	UpdateData(true);
	name[0] = 0;
	switch(m_nameRadio)
	{
	case NAME_PRESENT:
	{
		TCHAR stemp[260];
		GetDlgItemText(IDC_NAME, stemp, 255);
		strcpy(name, T2A(stemp));
		if (strlen(name) == 0)
			return false;
		return true;
	}
	case NAME_NOT_PRESENT:
		name[0] = 'O';
		name[1] = 0  ;
		return true;
	case NO_NAME:
		return true;

	}
	return false;
}
