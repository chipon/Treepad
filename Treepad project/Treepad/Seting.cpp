// Seting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Treepad.h"
#include "Seting.h"
#include "TreepadView.h"
#include "afxdialogex.h"


// CSeting �Ի���

IMPLEMENT_DYNAMIC(CSeting, CDialogEx)

CSeting::CSeting(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, x(0)
	, y(0)
	, it(0)
{
}

CSeting::~CSeting()
{
}

BOOL CSeting::OnInitDialog() {
	/*CString strTemp;
	((CComboBox*)GetDlgItem(IDC_COMBO1))->ResetContent();//����������������
	for (int i = 1; i <= 10; i++)
	{
		strTemp.Format(_T("%d"), i);
		((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(strTemp);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(iter);//���õ�n������Ϊ��ʾ�����ݡ�
	//sc1.SetRange(0, 1);
	//sc2.SetRange(1, 20);
	//sc3.SetRange(0, 1);*/
	return TRUE;
}

void CSeting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, cb);
	DDX_Control(pDX, IDC_RADIO1, rb1);
	DDX_Text(pDX, IDC_EDIT2, x);
	DDX_Text(pDX, IDC_EDIT3, y);
	DDX_Control(pDX, IDC_SLIDER2, sc1);
	DDX_Control(pDX, IDC_SLIDER3, sc2);
	DDX_Control(pDX, IDC_SLIDER4, sc3);
}


BEGIN_MESSAGE_MAP(CSeting, CDialogEx)

ON_BN_CLICKED(IDRESET, &CSeting::OnClickedIdreset)
END_MESSAGE_MAP()



void CSeting::OnCbnSelchangeCombo1()
{
	//int n = cb.GetCurSel();
	//CString s;
	//if (n >= 0)
	//{
	//	cb.GetLBText(n, s);
	//}
	//iter = _ttoi(s);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}




void CSeting::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	DestroyWindow();
	CDialogEx::OnCancel();
}


void CSeting::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	CDialog::PostNcDestroy();
	delete this;
	CDialogEx::PostNcDestroy();
}


void CSeting::OnClickedIdreset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
