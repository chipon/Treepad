
// TreepadView.h : CTreepadView ��Ľӿ�
//
#pragma once
#include <vector>
#include "TreepadDoc.h"
#include "Seting.h"
using namespace std;

#define CLOSE1 8	//�����߶ζ˵����
#define CLOSE2 0.3	//�����߶β���
enum status { DEFAULT_STATU, MOVE_STATU, TRUNK_STATU, BRANCH_STATU, CLOSE_STATU ,RUN_STATU};
static double getAng(CPoint p1, CPoint p2) {
	return atan2(p2.y - p1.y, p2.x - p1.x);
}
static double getLen(CPoint p1, CPoint p2) {
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}
//�Ƕȶ��û���ֵ�洢
class Branch
{
protected:
	CPoint p;	//���
public:
	double angle;	//�Ƕ�
	double lenth;	//����
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
	CPoint getStart() {	//���
		return p;
	}
	CPoint getEnd() {	//�յ�
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
	void updateBranch() {	//������ʣ�ಿ�ֱ�ɷ�֧
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

static bool close_point(CPoint p,Trunk t) {	//�ж��Ƿ񿿽����ɶ˵�
	return getLen(t.getEnd(), p) < CLOSE1;
}
static bool close_trunk(CPoint p, Trunk t) {	//�ж��Ƿ񿿽�����
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
static CPoint getConnectPoint(CPoint p0, Trunk t) {	//�ڿ�������ʱ������ӵ�����
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

static int dam;	//ֱ��
static int iter;	//��������
static double rate;	//������
static double dec;	//ֱ����С��
static CPoint pos;	//����λ��]
static bool f5;//����Ƿ������ô���

class CTreepadView : public CView
{
protected: // �������л�����
	CTreepadView();
	DECLARE_DYNCREATE(CTreepadView)
	COLORREF color;	//��ɫ
	int cx, cy;	//���ڴ�С
	Trunk tree;	//�������Ľṹ
	Trunk del;	//����ɾ��������
	vector<Branch> past;//����ɾ������֦
	CSeting *set;	//���öԻ���
	enum status statu;	//��ǰ״̬
	CString str;	//״̬���ַ���
	bool f1;	//����Ƿ񿿽��˵�
	bool f2;//��ǻ����ɻ��֧
	bool f3;//����Ƿ���ctrl��
	bool f4;//����Ƿ�ı�Ĭ�����λ��
	bool f6;//����Ƿ�ɳ���
	bool f7;//����Ƿ�ɷ�����
	CPoint p1,p2;	//��¼���������յ�
public:
	CTreepadDoc* GetDocument() const;
	// ����
public:

	// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// ʵ��
public:
	virtual ~CTreepadView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // TreepadView.cpp �еĵ��԰汾
inline CTreepadDoc* CTreepadView::GetDocument() const
{
	return reinterpret_cast<CTreepadDoc*>(m_pDocument);
}
#endif

