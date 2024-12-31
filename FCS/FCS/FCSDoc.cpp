
// FCSDoc.cpp: CFCSDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "FCS.h"
#endif

#include "FCSDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFCSDoc

IMPLEMENT_DYNCREATE(CFCSDoc, CDocument)

BEGIN_MESSAGE_MAP(CFCSDoc, CDocument)
END_MESSAGE_MAP()


// CFCSDoc 构造/析构

CFCSDoc::CFCSDoc() noexcept
{
	// TODO: 在此添加一次性构造代码
	m_isDraw = false;
	m_num = 0;
	step = 0.1;
	T = 20;
	Tf = 5; //正弦周期
	Ku = 1, Kp = 4, Ki = 0, Kd = 0;
	A = 10, A0 = 0.5;
	rh = 5;//幅度
	h = 0;
	m_type = 0;

}

CFCSDoc::~CFCSDoc()
{
}

BOOL CFCSDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}

void CFCSDoc::SetData(bool vm_isDraw, double vrh,double ve, double vQi, double vh, double vu, double vstep, double vT,
	double vTf, double vKu, double vKp, double vKi, double vKd, double vA, double vA0, bool vIssup, int vm_type) {
	rh = vrh; e = ve; Qi = vQi; h = vh; u = vu; step = vstep; T = vT; Tf = vTf; Ku = vKu; Kp = vKp; Ki = vKi;
	Kd = vKd; A = vA; A0 = vA0; Issup = vIssup; m_type = vm_type; m_isDraw = vm_isDraw;
}

void CFCSDoc::GetData(bool &vm_isDraw, double &vrh,double &ve, double &vQi, double &vh, double &vu, double &vstep, double &vT,
	double &vTf, double &vKu, double &vKp, double &vKi, double &vKd, double &vA, double &vA0,bool &vIssup, int &vm_type) {
	vrh = rh; ve = e; vQi = Qi; vh = h; vu = u; vstep = step; vT = T; vTf = Tf; vKu = Ku; vKp = Kp; vKi = Ki;
	vKd = Kd; vA = A; vA0 = A0; vIssup = Issup; vm_type = m_type; vm_isDraw = m_isDraw;
}

void CFCSDoc::SetIssup(bool sup) {
	Issup = sup;
}

void CFCSDoc::Setnum(int num) {
	m_num = num;
}

int CFCSDoc::Getnum( ) {
	return m_num;
}


// CFCSDoc 序列化

void CFCSDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
		ar << m_isDraw <<rh<< e << Qi << h << u << step << T << Tf << Ku << Kp << Ki << Kd << A << A0 << m_num << Issup << m_type;
	}
	else
	{
		// TODO: 在此添加加载代码
		ar >> m_isDraw >>rh>> e >> Qi >> h >> u >> step >> T >> Tf >> Ku >> Kp >> Ki >> Kd >> A >> A0 >> m_num >> Issup >> m_type;
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CFCSDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CFCSDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CFCSDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CFCSDoc 诊断

#ifdef _DEBUG
void CFCSDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFCSDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFCSDoc 命令
