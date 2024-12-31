
// FCSView.cpp: CFCSView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "FCS.h"
#endif

#include "FCSDoc.h"
#include "FCSView.h"

#include "m_dlg.h"

#include<cmath>
#define Pi 3.14159

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFCSView

IMPLEMENT_DYNCREATE(CFCSView, CView)

BEGIN_MESSAGE_MAP(CFCSView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_C, &CFCSView::OnC)
	ON_WM_TIMER()
	ON_COMMAND(ID_START, &CFCSView::OnStart)
	ON_COMMAND(ID_PAUSE, &CFCSView::OnPause)
	ON_COMMAND(ID_STOP, &CFCSView::OnStop)
END_MESSAGE_MAP()

// CFCSView 构造/析构

CFCSView::CFCSView() noexcept
{
	// TODO: 在此处添加构造代码
	m_interval = 100;
	m_isDraw = false;
	eI = 0;
	m_num = 0;
	Issup = 1;
	step = 0.1;
	T=20;
	Tf=5; //正弦周期
	Ku=1, Kp=4, Ki=0, Kd=0;
	A=10, A0=0.5;
	rh=5;//幅度
}

CFCSView::~CFCSView()
{
}

BOOL CFCSView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CFCSView 绘图

void CFCSView::OnDraw(CDC* pDC)
{
	CFCSDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	// TODO: 在此处为本机数据添加绘制代码
	CRect rect;
	GetClientRect(&rect);
	pDC->SetMapMode(MM_LOMETRIC);
	CPoint ptLeft(rect.left, rect.top);
	CPoint ptRight(rect.right, rect.bottom);
	pDC->SetViewportOrg(ptRight.x / 2, ptRight.y / 2);

	pDC->DPtoLP(&ptLeft);// 设备坐标转换为逻辑坐标
	pDC->DPtoLP(&ptRight);

	//pDC->MoveTo(-ptRight.x, 0);
	//pDC->LineTo(ptRight.x, 0);
	//pDC->MoveTo(0, -ptRight.y);
	//pDC->LineTo(0,ptRight.y);

	bias1.x = m_interval;
	bias1.y = m_interval;
	bias2.x = ptLeft.x + m_interval;
	bias2.y = m_interval;
	bias3.x = ptLeft.x+m_interval;
	bias3.y = ptRight.y+m_interval;
	bias4.x = m_interval;
	bias4.y = ptRight.y + m_interval;

	dXInter = (ptRight.x - 3 * m_interval) / 20;
	dYInter1 = (ptLeft.y - 2 * m_interval - 50) / 10;
	dYInter2 = (ptLeft.y - 2 * m_interval - 25) / 20;
	dYInter3 = (ptLeft.y - 2 * m_interval - 25) / 10;
	

	//绘制各个坐标轴
	CPen BlackPen,GreenPen,RedPen,BluePen,YellowPen,WhitePen;
	BlackPen.CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
	BluePen.CreatePen(PS_SOLID, 8, RGB(0, 0, 255));
	GreenPen.CreatePen(PS_SOLID, 8, RGB(0, 255, 0));
	RedPen.CreatePen(PS_SOLID, 8, RGB(255, 0, 0));
	YellowPen.CreatePen(PS_SOLID, 8, RGB(192, 192, 0));
	WhitePen.CreatePen(PS_SOLID, 8, RGB(255, 255, 255));
	CPen* oldPen;
	oldPen = pDC->SelectObject(&BlackPen);
	LOGFONT lf;
	lf.lfHeight = 30;                    // 高度为30
	lf.lfWidth = 0;                     // 默认宽度
	lf.lfEscapement = 0;              // 逆时针旋转0度
	lf.lfUnderline = false;                // 下划线
	lf.lfItalic = false;                   // 非斜体
	lf.lfStrikeOut = false;                // 无删除线 
	lf.lfCharSet=GB2312_CHARSET;        // GB2312字符集 
	CFont NewFont, * pOldFont; 
	NewFont.CreateFontIndirect(&lf);
	pOldFont = pDC->SelectObject(&NewFont);


	//第一象限
	//x轴
	pDC->MoveTo(bias1);
	pDC->LineTo(ptRight.x-m_interval, bias1.y);
	pDC->MoveTo(ptRight.x - m_interval, bias1.y);
	pDC->LineTo(ptRight.x - m_interval-10, bias1.y - 10);
	pDC->MoveTo(ptRight.x - m_interval, bias1.y);
	pDC->LineTo(ptRight.x - m_interval - 10, bias1.y + 10);
	for (int i = 5; i <= 20; i += 5) {
		CString str;
		str.Format(_T("%d.0"), i);
		CPoint p(bias1.x + i * dXInter, bias1.y);
		pDC->TextOut(p.x-5, p.y - 10, str);
		if (i == 20) {
			pDC->TextOut(p.x, p.y + 50, _T("时间(s)"));
			
		}

		CPoint p2(bias1.x + i * dXInter, bias1.y+8);
		pDC->MoveTo(p);
		pDC->LineTo(p2);
	}
	//y轴
	pDC->MoveTo(bias1);
	pDC->LineTo(bias1.x, ptLeft.y-m_interval);
	pDC->MoveTo(bias1.x, ptLeft.y - m_interval);
	pDC->LineTo(bias1.x - 10, ptLeft.y - m_interval - 10);
	pDC->MoveTo(bias1.x, ptLeft.y - m_interval);
	pDC->LineTo(bias1.x + 10, ptLeft.y - m_interval - 10);
	for (int i = 0; i <= 10; i += 2) {
		CString str;
		str.Format(_T("%d.00"), i - 2);
		CPoint p(bias1.x, bias1.y + i * dYInter1);
		pDC->TextOut(p.x - 80, p.y+5, str);
		if (i == 10) {
			pDC->TextOut(p.x + 50, p.y + 50, _T("误差(m)"));
		}
		CPoint p2(p.x + 8, p.y);
		pDC->MoveTo(p);
		pDC->LineTo(p2);
	}

	//第二象限
	//x轴
	pDC->MoveTo(bias2);
	pDC->LineTo(- m_interval, bias2.y);
	pDC->MoveTo(-m_interval, bias2.y);
	pDC->LineTo(-m_interval - 10, bias2.y - 10);
	pDC->MoveTo(-m_interval, bias2.y);
	pDC->LineTo(-m_interval - 10, bias2.y + 10);
	for (int i = 5; i <= 20; i += 5) {
		CString str;
		str.Format(_T("%d.0"), i);

		CPoint p(bias2.x + i * dXInter, bias2.y);
		pDC->TextOut(p.x - 5, p.y - 10, str);
		if (i == 20) {
			pDC->TextOut(p.x, p.y + 50, _T("时间(s)"));
		}
		CPoint p2(bias2.x + i * dXInter, bias2.y + 8);
		pDC->MoveTo(p);
		pDC->LineTo(p2);
	}
	//y轴
	pDC->MoveTo(bias2);
	pDC->LineTo(bias2.x, ptLeft.y - m_interval);
	pDC->MoveTo(bias2.x, ptLeft.y - m_interval);
	pDC->LineTo(bias2.x+10, ptLeft.y - m_interval-10);
	pDC->MoveTo(bias2.x, ptLeft.y - m_interval);
	pDC->LineTo(bias2.x-10, ptLeft.y - m_interval-10);
	for (int i = 0; i <= 20; i += 4) {
		CString str;
		str.Format(_T("%d"), i);
		CPoint p(bias2.x, bias2.y + i * dYInter2);
		pDC->TextOut(p.x - 40, p.y + 5, str);
		if (i == 20) {
			pDC->TextOut(p.x + 50, p.y + 30, _T("进水流量(m^3/s)"));
			
		}
		CPoint p2(p.x + 8, p.y);
		pDC->MoveTo(p);
		pDC->LineTo(p2);
	}

	//第三象限
	//x轴
	pDC->MoveTo(bias3);
	pDC->LineTo(-m_interval, bias3.y);
	pDC->MoveTo(-m_interval, bias3.y);
	pDC->LineTo(-m_interval - 10, bias3.y - 10);
	pDC->MoveTo(-m_interval, bias3.y);
	pDC->LineTo(-m_interval - 10, bias3.y + 10);
	for (int i = 5; i <= 20; i += 5) {
		CString str;
		str.Format(_T("%d.0"), i);

		CPoint p(bias3.x + i * dXInter, bias3.y);
		pDC->TextOut(p.x - 5, p.y - 10, str);
		if (i == 20) {
			
			pDC->TextOut(p.x, p.y + 50, _T("时间(s)"));
			
		}
		CPoint p2(bias3.x + i * dXInter, bias3.y + 8);
		pDC->MoveTo(p);
		pDC->LineTo(p2);
	}
	//y轴
	pDC->MoveTo(bias3);
	pDC->LineTo(bias3.x,  - m_interval);
	pDC->MoveTo(bias3.x, -m_interval);
	pDC->LineTo(bias3.x + 10, -m_interval - 10);
	pDC->MoveTo(bias3.x, -m_interval);
	pDC->LineTo(bias3.x - 10, -m_interval - 10);
	for (int i = 0; i <= 10; i += 2) {
		CString str;
		str.Format(_T("%d"), i);
		CPoint p(bias3.x, bias3.y + i * dYInter3);
		pDC->TextOut(p.x - 40, p.y + 5, str);
		if (i == 10) {
			
			pDC->TextOut(p.x + 50, p.y + 50, _T("液位高度(m)"));
			
		}
		CPoint p2(p.x + 8, p.y);
		pDC->MoveTo(p);
		pDC->LineTo(p2);
	}

	//第四象限 容器
	CPoint p(bias4.x + 5 * dXInter, bias4.y);
	pDC->MoveTo(p);
	pDC->LineTo(p.x, p.y + 500);
	pDC->MoveTo(p);
	pDC->LineTo(p.x+10*dXInter, p.y);
	pDC->MoveTo(p.x + 10 * dXInter, p.y);
	pDC->LineTo(p.x + 10 * dXInter, p.y+500);

	pDoc->GetData(m_isDraw, rh, e[0], Qi[0], h[0], u[0], step, T, Tf,
		Ku, Kp, Ki, Kd, A, A0, Issup, m_type);
	eI = e[0];
	m_num = pDoc->Getnum();

	switch (m_type)
	{
	case 0: {
		for (int i = 0; i < T / step; i++) {
			r[i] = rh;
		}
	}
		  break;
	case 1: {
		for (int i = 0; i < T / step; i++) {
			r[i] = rh * sin(i * step * 2 * Pi / Tf) + h[0];
		}
	}break;
	default:
		break;
	}

	//绘制曲线
	if (m_isDraw) {
		pDC->SelectObject(&GreenPen);
		if (m_type==0) {
			
			pDC->MoveTo(bias3.x, bias3.y + r[0] * dYInter3);
			pDC->LineTo(bias3.x + 20 * dXInter, bias3.y + r[0] * dYInter3);

			pDC->MoveTo(p.x+5, p.y + r[0] * dYInter3);
			pDC->LineTo(p.x + 10 * dXInter-5, p.y + r[0] * dYInter3);
		}
		else {
			for (int i = 1; i < T / step; i++) {
				pDC->MoveTo(bias3.x + (i - 1) * step * dXInter, bias3.y + r[i - 1] * dYInter3);
				pDC->LineTo(bias3.x + i * step * dXInter, bias3.y + r[i] * dYInter3);
			}
		}
		

		for (int i = 1; i < T / step; i++) {
			K1 = step * (Qi[i-1] - A0 * sqrt(20 * h[i - 1]))/A;
			K2 = step * (Qi[i - 1] - A0 * sqrt(20 * (h[i - 1] + K1 / 2))) / A;
			K3 = step * (Qi[i - 1] - A0 * sqrt(20 * (h[i - 1] + K2 / 2))) / A;
			K4 = step * (Qi[i - 1] - A0 * sqrt(20 * (h[i - 1] + K3))) / A;
			h[i] = h[i - 1] + (K1 + 2 * K2 + 2 * K3 + K4) / 6;
			e[i] = r[i] - h[i];
			eI += e[i];
			u[i] = Kp * e[i] + Kd * (e[i] - e[i - 1]) / step + Ki * eI;
			Qi[i] = Ku * u[i];
			if (Qi[i] < 0) {
				Qi[i] = 0;
			}
			
		}
		//animation r
		for (int i = 0; i < m_num; i++) {
			pDC->SelectObject(&WhitePen);
			pDC->MoveTo(p.x + 5, p.y + r[i - 1] * dYInter3);
			pDC->LineTo(p.x + 10 * dXInter - 5, p.y + r[i - 1] * dYInter3);
			pDC->SelectObject(&GreenPen);
			pDC->MoveTo(p.x + 5, p.y + r[i] * dYInter3);
			pDC->LineTo(p.x + 10 * dXInter - 5, p.y + r[i] * dYInter3);
			pDC->SelectObject(&BlackPen);
			pDC->MoveTo(p);
			pDC->LineTo(p.x, p.y + 500);
			pDC->MoveTo(p);
			pDC->LineTo(p.x + 10 * dXInter, p.y);
			pDC->MoveTo(p.x + 10 * dXInter, p.y);
			pDC->LineTo(p.x + 10 * dXInter, p.y + 500);
		}
		//e
		pDC->SelectObject(&BluePen);
		for (int i = 1; i < m_num; i++) {
			CString str;
			str.Format(_T("当前误差: %2f m"), e[i]);
			pDC->TextOut(bias1.x + 15 * dXInter, bias1.y + 9 * dYInter1, str);
			pDC->MoveTo(bias1.x+(i - 1)* step*dXInter, bias1.y+(e[i - 1]+2)*dYInter1);
			pDC->LineTo(bias1.x + i * step * dXInter, bias1.y+(e[i]+2) * dYInter1);
		}

		//Qi
		pDC->SelectObject(&YellowPen);
		for (int i = 1; i < m_num; i++) {
			pDC->MoveTo(bias2.x + (i - 1) * step * dXInter, bias2.y + Qi[i - 1] * dYInter2);
			pDC->LineTo(bias2.x + i * step * dXInter, bias2.y + Qi[i] * dYInter2);
		}

		//h
		pDC->SelectObject(&RedPen);
		for (int i = 1; i < m_num; i++) {
			pDC->MoveTo(bias3.x + (i - 1) * step * dXInter, bias3.y + h[i - 1] * dYInter3);
			pDC->LineTo(bias3.x + i * step * dXInter, bias3.y + h[i] * dYInter3);
		}

		CBrush NewBrush;
		CBrush* pOldBrush;
		NewBrush.CreateSolidBrush( RGB(0, 192, 192));
		pOldBrush = pDC->SelectObject(&NewBrush);
		pDC->SelectStockObject(NULL_PEN);
		//液面高度
		if(m_num>0)
		pDC->Rectangle(p.x+2, p.y, p.x + 10 * dXInter, p.y + h[m_num-1] * dYInter3);
	}
}


// CFCSView 打印

BOOL CFCSView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CFCSView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CFCSView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CFCSView 诊断

#ifdef _DEBUG
void CFCSView::AssertValid() const
{
	CView::AssertValid();
}

void CFCSView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFCSDoc* CFCSView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFCSDoc)));
	return (CFCSDoc*)m_pDocument;
}
#endif //_DEBUG


// CFCSView 消息处理程序


void CFCSView::OnC()
{
	// TODO: 在此添加命令处理程序代码
	m_dlg Dlg;
	CFCSDoc* pDoc = GetDocument();
	Dlg.SetData(rh, h[0], step, T, Tf, Ku, Kp, Ki, Kd, A, A0, m_type);
	//Dlg->Create(MYDIALOG);
	//Dlg->ShowWindow(SW_SHOW);
	if (Dlg.DoModal() == IDOK) {
		m_isDraw = true;
		Ku = Dlg.Ku;
		Ki = Dlg.Ki;
		Kd = Dlg.Kd;
		Kp = Dlg.Kp;
		rh = Dlg.h;
		e[0] = Dlg.h - Dlg.h0;
		eI += e[0];
		u[0] = Dlg.Kp * e[0];
		Qi[0] = Dlg.Ku*u[0];
		if (Qi[0] < 0) {
			Qi[0] = 0;
		}
		h[0] = Dlg.h0;
		step = Dlg.t;
		T = Dlg.TT;
		A = Dlg.A;
		A0 = Dlg.A0;
		Tf = Dlg.T;
		m_type = Dlg.m_type;
		if (m_type == 1) {
			//rh=Dlg.h0;
			e[0] = Dlg.h0 - Dlg.h0;
			eI = e[0];
			u[0] = Dlg.Kp * e[0];
			Qi[0] = Dlg.Ku * u[0];
		}
		
		pDoc->SetData(m_isDraw,rh,e[0], Qi[0], h[0], u[0], step, T, Tf,
			Ku, Kp, Ki, Kd, A, A0, Issup, m_type);
		Issup = true;
		pDoc->SetIssup(Issup);
		m_num = 0;
		pDoc->Setnum(m_num);
		Invalidate();	
		SetTimer(1, 100, NULL);
	}
	
	
}


void CFCSView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFCSDoc* pDoc = GetDocument();
	if (nIDEvent == 1) {
		//CClientDC dc(this);
		//dc.SetMapMode(MM_LOMETRIC);
		//dc.SetViewportOrg(New_Org);
		m_num++;
		pDoc->Setnum(m_num);
		if (m_num >= T / step) { 
			KillTimer(1);
			pDoc->Setnum(m_num);
		}
		
	}
	Invalidate();

	CView::OnTimer(nIDEvent);
}


void CFCSView::OnStart()
{
	// TODO: 在此添加命令处理程序代码
	CFCSDoc* pDoc = GetDocument();
	if (m_num >= T / step) {
		m_num = 0;
		pDoc->Setnum(m_num);
	}
	m_num = pDoc->Getnum()-1;
	Issup = true;
	pDoc->SetIssup(Issup);
	SetTimer(1, 100, NULL);
}




void CFCSView::OnPause()
{
	// TODO: 在此添加命令处理程序代码
	CFCSDoc* pDoc = GetDocument();
	if (Issup == 1) {
		KillTimer(1);
		Issup = false;
		pDoc->SetIssup(Issup);
		//pDoc->Setnum(m_num);
	}
	else
	{
		SetTimer(1, 100, NULL);
		Issup = true;
		pDoc->SetIssup(Issup);
	}
}


void CFCSView::OnStop()
{
	// TODO: 在此添加命令处理程序代码
	CFCSDoc* pDoc = GetDocument();
	KillTimer(1);
	Invalidate();
	m_num = 0;
	pDoc->Setnum(m_num);
}
