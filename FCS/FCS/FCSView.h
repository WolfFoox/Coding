
// FCSView.h: CFCSView 类的接口
//

#pragma once


class CFCSView : public CView
{
protected: // 仅从序列化创建
	CFCSView() noexcept;
	DECLARE_DYNCREATE(CFCSView)

// 特性
public:
	CFCSDoc* GetDocument() const;

// 操作
public:
	static const int DATA_NUM = 5000;// 数据数目
	

private:
	double e[DATA_NUM] = {0};// 需要绘制的数据
	double Qi[DATA_NUM] = {0};// 需要绘制的数据
	double h[DATA_NUM] = {0};// 需要绘制的数据
	double u[DATA_NUM] = {0};//控制
	int m_interval;// 绘图周边空白
	CPoint bias1;
	CPoint bias2;
	CPoint bias3;
	CPoint bias4;
	double dXInter;
	double dYInter1;
	double dYInter2;
	double dYInter3;
	bool m_isDraw;
	double step;
	double T;
	double Tf; //正弦周期
	double K1, K2, K3, K4,Ku,Kp,Ki,Kd;
	double A,A0;
	double eI;
	double rh;//幅度
	int m_num;
	bool Issup;
	double r[DATA_NUM] = {0};
	int m_type;

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CFCSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnC();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStart();
	afx_msg void OnPause();
	afx_msg void OnStop();
};

#ifndef _DEBUG  // FCSView.cpp 中的调试版本
inline CFCSDoc* CFCSView::GetDocument() const
   { return reinterpret_cast<CFCSDoc*>(m_pDocument); }
#endif

