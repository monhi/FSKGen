// FSKGenDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "general.h"

class CFSKBitStrGen;
class CWaveGen;
// CFSKGenDlg dialog
class CFSKGenDlg : public CDialog
{
// Construction
public:
	CFSKGenDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FSKGEN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox	m_month;
public:
	CComboBox	m_day;
	CComboBox	m_hour;
	CComboBox	m_minute;
	CComboBox	m_samplingFreq;
public:
	afx_msg void OnBnClickedSetfilepath();
	afx_msg void OnBnClickedMdmf();
	afx_msg void OnBnClickedSdmf();
			void processType(bool);
private:
	int		m_nameRadio;
	int		m_numberRadio;
   CString	pathName ;
	int		m_typeRadio;
	afx_msg void OnBnClickedGenerate();
	bool	CheckFileName(void);
	char	path[MAX_PATH];
	uint8	telNum[MAX_PATH];
	char    name[MAX_PATH];
	uint8	cid_type;
	int		GetCIDType(void);
	void	GetDateAndTime(void);
	uint8	month,day,hour,minute;
	
	CFSKBitStrGen	*m_fskGen;
	CWaveGen		*m_waveGen;
	uint32			sampleRate[20];
	bool			GetTellNum(void);
	bool			GetNameData(void);
};
