#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSeting �Ի���

class CSeting : public CDialogEx
{
	DECLARE_DYNAMIC(CSeting)

public:
	CSeting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSeting();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox cb;
	CButton rb1;
	int x;
	int y;
	CSliderCtrl sc1;
	CSliderCtrl sc2;
	CSliderCtrl sc3;
	int it;
	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnClickedIdreset();
};
