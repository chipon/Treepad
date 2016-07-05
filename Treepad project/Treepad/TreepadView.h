
// TreepadView.h : CTreepadView 类的接口
//
#pragma once
#include <vector>
#include "TreepadDoc.h"
#include "Seting.h"
using namespace std;

#define CLOSE1 8	//靠近线段端点参数
#define CLOSE2 0.3	//靠近线段参数
enum status { DEFAULT_STATU, MOVE_STATU, TRUNK_STATU, BRANCH_STATU, CLOSE_STATU ,RUN_STATU};
static double getAng(CPoint p1, CPoint p2) {
	return atan2(p2.y - p1.y, p2.x - p1.x);
}
static double getLen(CPoint p1, CPoint p2) {
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}
//角度都用弧度值存储
class Branch
{
protected:
	CPoint p;	//起点
public:
	double angle;	//角度
	double lenth;	//长度
	Branch() {}
	Branch(CPoint connect, double a, double len) {
		p = connect;
		angle = a;
		lenth = len;
	}
	Branch(CPoint p1, CPoint p2) {
		p = p1;
		angle = getAng(p1, p2);
		lenth = getLen(p1, p2);
	}
	CPoint getStart() {	//起点
		return p;
	}
	CPoint getEnd() {	//终点
		return CPoint(int(p.x + lenth*cos(angle)), int(p.y + lenth*sin(angle)));
	}
};

class Trunk :public Branch
{
public:
	vector<Branch> branch;
	Trunk() :Branch() {}
	Trunk(CPoint p1, CPoint p2) :Branch(p1, p2) {}
	Trunk(CPoint connect, double a, double len) :Branch(connect, a, len) {}
	void addBranch(Branch b) {
		branch.push_back(b);
	}
	void deleteBranch() {
		branch.pop_back();
	}
	UINT size() {
		return branch.size();
	}
	Branch operator[](int i) {
		return branch[i];
	}
	void clear() {
		branch.clear();
	}
	void updateBranch() {	//将顶部剩余部分变成分支
		double max = 0;
		CPoint start;
		for (UINT i = 0; i < branch.size(); ++i) {
			if (getLen(branch[i].getStart(), p)>max) {
				max = getLen(branch[i].getStart(), p);
				start = branch[i].getStart();
			}
		}
		if (start != getEnd()) {
			addBranch(Branch(start, getEnd()));
		}
		lenth -= getLen(start, getEnd());
	}
};

static bool close_point(CPoint p,Trunk t) {	//判断是否靠近主干端点
	return getLen(t.getEnd(), p) < CLOSE1;
}
static bool close_trunk(CPoint p, Trunk t) {	//判断是否靠近主干
	bool f1, f2, f3;
	CPoint p1 = t.getStart(), p2 = t.getEnd();
	if (p1.x == p2.x) {
		return abs(p.x - p1.x) < CLOSE1;
	}
	/*else if (p.x == p1.x) {
		return abs(p1.x - p2.x) < CLOSE1 && (p.y - p1.y)*(p.y - p2.y) < 0;
	}*/
	f1 = fabs((p.y - p1.y) / (double)(p.x - p1.x) - (p2.y - p1.y) / (double)(p2.x - p1.x))<CLOSE2;
	f2 = (p.x - p1.x)*(p.x - p2.x) < 0;
	f3 = (p.y - p1.y)*(p.y - p2.y) < 0;
	return f1&&f2&&f3;
}
static CPoint getConnectPoint(CPoint p0, Trunk t) {	//在靠近主干时求出连接点坐标
	CPoint p1 = t.getStart(), p2 = t.getEnd();
	int x, y;
	if (p1.x == p2.x) {
		x = p1.x; y = p0.y;
	}
	else {
		double k = (p2.y - p1.y) / (double)(p2.x - p1.x);
		if (p0.y == (int)(p1.y + k*(p0.x - p1.x))) {
			x = p0.x; y = p0.y;
		}
		else {
			x = (int)((p0.x + k*p0.y - k*p1.y + k*k*p1.x) / (double)(k*k + 1));
			y = (int)((p1.y + k*p0.x - k*p1.x + k*k*p0.y) / (double)(k*k + 1));
		}
	}
	return CPoint(x, y);
}

static int dam;	//直径
static int iter;	//迭代次数
static double rate;	//缩放率
static double dec;	//直径缩小率
static CPoint pos;	//起点的位置]
static bool f5;//标记是否开启设置窗口

class CTreepadView : public CView
{
protected: // 仅从序列化创建
	CTreepadView();
	DECLARE_DYNCREATE(CTreepadView)
	COLORREF color;	//颜色
	int cx, cy;	//窗口大小
	Trunk tree;	//保存树的结构
	Trunk del;	//保存删除的主干
	vector<Branch> past;//保存删除的树枝
	CSeting *set;	//设置对话框
	enum status statu;	//当前状态
	CString str;	//状态栏字符串
	bool f1;	//标记是否靠近端点
	bool f2;//标记画主干或分支
	bool f3;//标记是否按下ctrl键
	bool f4;//标记是否改变默认起点位置
	bool f6;//标记是否可撤销
	bool f7;//标记是否可反撤销
	CPoint p1,p2;	//记录画线起点和终点
public:
	CTreepadDoc* GetDocument() const;
	// 操作
public:

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
	virtual ~CTreepadView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDefault();
	afx_msg void OnRun();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDraw();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void paint();
	void draw_branch(vector<Branch> b,int k);
	CPoint map(CPoint p);
	afx_msg void OnStop();
	afx_msg void OnColor();
	afx_msg void OnReset();
	afx_msg void OnSeting();
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	afx_msg void OnBnClickedClose();
};

#ifndef _DEBUG  // TreepadView.cpp 中的调试版本
inline CTreepadDoc* CTreepadView::GetDocument() const
{
	return reinterpret_cast<CTreepadDoc*>(m_pDocument);
}
#endif

