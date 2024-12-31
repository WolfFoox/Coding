// m_dlg.cpp: 实现文件
//

#include "pch.h"
#include "FCS.h"
#include "afxdialogex.h"
#include "m_dlg.h"


// m_dlg 对话框

IMPLEMENT_DYNAMIC(m_dlg, CDialogEx)

m_dlg::m_dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(MYDIALOG, pParent)
	, A(10)
	, h0(0)
	, A0(0.5)
	, Ku(1)
	, h(5)
	, T(5)
	, t(0.1)
	, TT(20)
	, Kp(4)
	, Ki(0)
	, Kd(0)
	, m_type(0)
{

}

m_dlg::~m_dlg()
{
}

void m_dlg::SetData(double rh, double vh, double step, double vT, double Tf,
	double vKu, double vKp, double vKi, double vKd, double vA, double vA0, int vm_type) {
	A=vA;
	h0=vh;
	A0=vA0;
	Ku=vKu;
	h=rh;
	T=Tf;
	t=step;
	TT=vT;
	Kp=vKp;
	Ki=vKi;
	Kd=vKd;
	m_type=vm_type;
}

void m_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, A);
	DDX_Text(pDX, IDC_EDIT2, h0);
	DDV_MinMaxDouble(pDX, h0, 0, 10);
	DDX_Text(pDX, IDC_EDIT3, A0);
	DDX_Text(pDX, IDC_EDIT4, Ku);
	DDX_Text(pDX, IDC_EDIT5, h);
	DDV_MinMaxDouble(pDX, h, 0, 10);
	DDX_Text(pDX, IDC_EDIT6, T);
	DDV_MinMaxDouble(pDX, T, 5, 10);
	DDX_Text(pDX, IDC_EDIT7, t);
	DDX_Text(pDX, IDC_EDIT8, TT);
	DDX_Text(pDX, IDC_EDIT9, Kp);
	DDX_Text(pDX, IDC_EDIT10, Ki);
	DDX_Text(pDX, IDC_EDIT11, Kd);
	DDX_Radio(pDX, IDC_RADIO1, m_type);
}


BEGIN_MESSAGE_MAP(m_dlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &m_dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &m_dlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &m_dlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// m_dlg 消息处理程序


void m_dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);//错误信息提示
	if (m_type == 0 && (h < 0 || h>10)) {
		int nChoice = MessageBox(_T("幅值范围为0-10米，超出范围，请重新输入"),
			_T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_type == 1&&(h<0||h>5)) {
		int nChoice = MessageBox(_T("幅值范围为0-5米，超出范围，请重新输入"),
			_T("错误"), MB_OK| MB_ICONERROR);
		return;
	}
	if (m_type == 1 && (T < 5 || T>10)) {
		int nChoice = MessageBox(_T("正弦周期范围为5-10s，超出范围，请重新输入"),
			_T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	UpdateData(FALSE);
	CDialogEx::OnOK();

}


void m_dlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	h0 = 0;
	UpdateData(FALSE);

}


void m_dlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	h0 = 5;
	UpdateData(FALSE);
}
