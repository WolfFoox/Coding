
// FCSDoc.h: CFCSDoc 类的接口
//


#pragma once


class CFCSDoc : public CDocument
{
private:
	bool m_isDraw;
	double e;// 需要绘制的数据
	double Qi;// 需要绘制的数据
	double h;// 需要绘制的数据
	double u;//控制
	double step;
	double T;
	double Tf; //正弦周期
	double Ku, Kp, Ki, Kd;
	double A, A0;
	double rh;//幅度
	int m_num;
	bool Issup;
	int m_type;
protected: // 仅从序列化创建
	CFCSDoc() noexcept;
	DECLARE_DYNCREATE(CFCSDoc)

// 特性
public:

// 操作
public:
	void SetData(bool m_isDraw, double rh,double e, double Qi, double h, double u, double step, double T, double Tf,
		double Ku, double Kp, double Ki, double Kd, double A, double A0,bool Issup, int m_type);
	void GetData(bool &m_isDraw, double &rh,double& e, double& Qi, double& h, double& u, double& step, double& T, double& Tf,
		double& Ku, double& Kp, double& Ki, double& Kd, double& A, double& A0,bool& Issup, int& m_type);
	void Setnum(int num);
	void SetIssup(bool sup);
	int Getnum();
// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CFCSDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
