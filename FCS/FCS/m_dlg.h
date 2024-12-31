#pragma once
#include "afxdialogex.h"


// m_dlg 对话框

class m_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(m_dlg)

public:
	m_dlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~m_dlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = MYDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double A;
	double h0;
	double A0;
	double Ku;
	double h;
	double T;//周期
	double t;
	double TT;
	double Kp;
	double Ki;
	double Kd;
	int m_type;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	void SetData(double rh, double vh, double step, double vT, double Tf,
		double vKu, double vKp, double vKi, double vKd, double vA, double vA0, int vm_type);
};
