
// TreepadView.cpp : CTreepadView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Treepad.h"
#endif

#include "atlimage.h"
#include "TreepadView.h"
#include "resource.h"
#include "Seting.h"
#include "MainFrm.h"
#include "Branch_Ser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTreepadView

IMPLEMENT_DYNCREATE(CTreepadView, CView)

BEGIN_MESSAGE_MAP(CTreepadView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTreepadView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_DEFAULT, &CTreepadView::OnDefault)
	ON_COMMAND(ID_RUN, &CTreepadView::OnRun)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_DRAW, &CTreepadView::OnDraw)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_STOP, &CTreepadView::OnStop)
	ON_COMMAND(ID_COLOR, &CTreepadView::OnColor)
	ON_COMMAND(ID_RESET, &CTreepadView::OnReset)
	ON_COMMAND(ID_SETING, &CTreepadView::OnSeting)
	ON_COMMAND(ID_FILE_SAVE, &CTreepadView::OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, &CTreepadView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, &CTreepadView::OnFileSaveAs)
	ON_BN_CLICKED(IDCLOSE, &CTreepadView::OnBnClickedClose)
END_MESSAGE_MAP()

// CTreepadView 构造/析构

CTreepadView::CTreepadView()
{
	// TODO: 在此处添加构造代码
	f1 = false;
	f2 = true;
	f3 = false;
	f4 = false;
	f5 = false;
	f6 = false;
	f7 = false;
	statu = DEFAULT_STATU;
	rate = 1;
	dam = 6;	
	iter = 6;
	dec = 1;
	set = NULL;
	color= RGB(0, 255, 0);
}

CTreepadView::~CTreepadView()
{
	//if (!set) {
	//	delete set;
	//}
}

BOOL CTreepadView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTreepadView 绘制

void CTreepadView::OnDraw(CDC* pDC)
{
	CTreepadDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc||!tree.branch.size())
		return;
	CPen pen(PS_SOLID, dam, color);
	pDC->SelectObject(&pen);
	if (statu == MOVE_STATU|| statu == DEFAULT_STATU) {
		pDC->MoveTo(tree.getStart());
		pDC->LineTo(tree.getEnd());
		for (UINT i = 0; i < tree.size(); ++i) {
			pDC->MoveTo(tree.branch[i].getStart());
			pDC->LineTo(tree.branch[i].getEnd());
		}
	}
	else if (statu == RUN_STATU) {
		paint();
	}
	// TODO: 在此处为本机数据添加绘制代码
}


// CTreepadView 打印


void CTreepadView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTreepadView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTreepadView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTreepadView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CTreepadView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTreepadView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTreepadView 诊断

#ifdef _DEBUG
void CTreepadView::AssertValid() const
{
	CView::AssertValid();
}

void CTreepadView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTreepadDoc* CTreepadView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTreepadDoc)));
	return (CTreepadDoc*)m_pDocument;
}
#endif //_DEBUG


// CTreepadView 消息处理程序

void CTreepadView::draw_branch(vector<Branch> b,int k)
{
	k--;
	if (!k) {
		return;
	}
	double angle;
	double lenth;
	double rat1;	//分支点位置与主干长度的比例
	double rat2;	//分支长度与主干长度的比例
	CClientDC dc(this);
	CPen pen(PS_SOLID, (int)(dam*pow(dec,iter-k)), color);
	dc.SelectObject(&pen);
	for (UINT i = 0; i < b.size(); ++i) {
		vector<Branch> sub;
		for (UINT j = 0; j < b.size(); ++j) {
			rat1 = getLen(tree.getStart(), tree[j].getStart()) / tree.lenth;
			CPoint *p=new CPoint(b[i].getStart().x + (int)(rat1*b[i].lenth*cos(b[i].angle)),
			b[i].getStart().y + (int)(rat1*b[i].lenth*sin(b[i].angle)));
			angle = b[i].angle + tree[j].angle - tree.angle;
			rat2 = tree[j].lenth / (double)tree.lenth;
			lenth = b[i].lenth*rat2;
			dc.MoveTo(*p);
			dc.LineTo(int(p->x + lenth*cos(angle)), int(p->y + lenth*sin(angle)));
			sub.push_back(Branch(*p, angle, lenth));
		}
		draw_branch(sub, k);
	}
	ReleaseDC(&dc);
}

CPoint CTreepadView::map(CPoint p) {
	CPoint p1 = f4 ? pos : tree.getStart();
	CPoint p2 = tree.getStart();
	return CPoint(p1.x + (int)((p.x - p2.x)*rate), p1.y + (int)((p.y - p2.y)*rate));
}

void CTreepadView::paint()
{
	CClientDC dc(this);
	CPen pen(PS_SOLID, dam, color);
	dc.SelectObject(&pen);
	cx = GetSystemMetrics(SM_CXFULLSCREEN);
	cy = GetSystemMetrics(SM_CYFULLSCREEN);
	tree.updateBranch();
	dc.MoveTo(map(tree.getStart()));
	dc.LineTo(map(tree.getEnd()));
	for (UINT i = 0; i < tree.size(); ++i) {
		dc.MoveTo(map(tree[i].getStart()));
		dc.LineTo(map(tree[i].getEnd()));
	}
	draw_branch(tree.branch,iter);
	ReleaseDC(&dc);
}

void CTreepadView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (statu == DEFAULT_STATU|| statu == RUN_STATU) {
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
	else if (statu == MOVE_STATU) {
		SetCursor(LoadCursor(NULL, IDC_CROSS));
	}
	if (statu == TRUNK_STATU|| statu == BRANCH_STATU) {
		CClientDC dc(this);
		CPen pen(PS_SOLID, dam, color);
		dc.SelectObject(&pen);
		dc.SetROP2(R2_NOTXORPEN);
		if (p1 != p2) {
			dc.MoveTo(p1);
			dc.LineTo(p2);
		}
		dc.MoveTo(p1);
		p2.x=f3?p1.x:point.x;
		p2.y = point.y;
		dc.LineTo(p2);
		ReleaseDC(&dc);
	}
	else if (statu == MOVE_STATU|| statu == CLOSE_STATU) {
		statu = MOVE_STATU;
		f1 = close_point(point, tree);
		if (f1) {
			HCURSOR hCur = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR2));
			SetCursor(hCur);
			statu = CLOSE_STATU;
		}
		else if (close_trunk(point, tree)) {
			HCURSOR hCur = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
			SetCursor(hCur);
			statu = CLOSE_STATU;
		}
	}
	str.Format(L"当前坐标 x=%d y=%d", point.x, point.y);
	((CMainFrame *)GetParent())->SetMessageText(str);
	CView::OnMouseMove(nFlags, point);
}

void CTreepadView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (statu == MOVE_STATU) {
		if (f2) {
			p1 = p2 = point;
			statu = TRUNK_STATU;
		}
	}
	else if (statu == CLOSE_STATU) {
		statu = BRANCH_STATU;
		if (f1) {
			p1 = p2 = tree.getEnd();
		}
		else {
			p1 = p2 =getConnectPoint(point, tree);
		}
	}
	CView::OnLButtonDown(nFlags, point);
}


void CTreepadView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ((statu == TRUNK_STATU|| statu == BRANCH_STATU)&&p1 != point) {
		CClientDC dc(this);
		CPen pen(PS_SOLID, dam, color);
		dc.SelectObject(&pen);
		dc.MoveTo(p1);
		dc.LineTo(f3?CPoint(p1.x,point.y):point);
		if (statu == TRUNK_STATU) {
			tree=Trunk(p1, f3 ? CPoint(p1.x, point.y) : point);
			f2 = false;
		}
		else {
			tree.addBranch(Branch(p1, point));
		}
		f6 = true;
		f7 = false;
		statu = MOVE_STATU;
	}
	CView::OnLButtonUp(nFlags, point);
}


void CTreepadView::OnDefault()
{
	statu = DEFAULT_STATU;
	// TODO: 在此添加命令处理程序代码
}


void CTreepadView::OnRun()
{
	statu = RUN_STATU;
	paint();
	// TODO: 在此添加命令处理程序代码
}


BOOL CTreepadView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return false;
}

void CTreepadView::OnDraw()
{
	statu = MOVE_STATU;
	// TODO: 在此添加命令处理程序代码
}



void CTreepadView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nChar==VK_CONTROL)
		f3 = true;
	if (f3 && (nChar == 'Z' || nChar == 'z')) {
		if (f6) {
			if (tree.size()) {
				past.push_back(tree.branch.back());
				tree.deleteBranch();
			}
			else {
				tree = Trunk(CPoint(0, 0), CPoint(0, 0));
				f6 = false;
				f2 = true;
			}
			statu = MOVE_STATU;
			InvalidateRect(NULL);
			f7 = true;
		}
	}
	else if (f3 && (nChar == 'Y' || nChar == 'y')) {
		if (f7) {
			//
		}
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CTreepadView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nChar == VK_CONTROL)
		f3 = false;
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CTreepadView::OnStop()
{
	statu = MOVE_STATU;
	InvalidateRect(NULL);
	// TODO: 在此添加命令处理程序代码
}


void CTreepadView::OnColor()
{
	CColorDialog colorDlg(RGB(0, 0, 0));
	if (IDOK == colorDlg.DoModal()){
		color = colorDlg.GetColor();
	}
	//InvalidateRect(NULL);
	// TODO: 在此添加命令处理程序代码
}


void CTreepadView::OnReset()
{
	statu = MOVE_STATU;
	tree.clear();
	f2 = true;
	tree = Trunk(CPoint(0, 0), CPoint(0, 0));
	InvalidateRect(NULL);
	// TODO: 在此添加命令处理程序代码
}


void CTreepadView::OnSeting()
{
	if (!f5) {
		set = new CSeting();
		set->Create(IDD_DIALOG1,this);
		set->ShowWindow(SW_SHOW);
		f5 = true;
	}
	// TODO: 在此添加命令处理程序代码
}


void CTreepadView::OnFileSave()
{
	if (!tree.size()) {
		return;
	}
	BOOL isOpen = FALSE;  
	CString defaultDir = L"D:\\Treepad\\Iteration";
	CString filter = L"Treepad File (*.tpf)||";
	CString fileName = L"file1.tpf";         //默认打开的文件名
	CFileDialog openFileDlg(isOpen, L"tpf", fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\" + fileName;
	if (result == IDOK) {
		filePath = openFileDlg.GetPathName();
	}
	tree.updateBranch();
	CBranch_Ser c(tree.getStart().x, tree.getStart().y, tree.getEnd().x, tree.getEnd().y,tree.size());
	CFile myFile(filePath, CFile::modeCreate | CFile::modeWrite);
	CArchive arStore(&myFile, CArchive::store);
	arStore.WriteObject(&c);
	for (UINT i = 0; i < tree.size(); ++i) {
		CBranch_Ser *d=new CBranch_Ser(tree[i].getStart().x, tree[i].getStart().y, tree[i].getEnd().x, tree[i].getEnd().y);
		arStore.WriteObject(d);
	}
	arStore.Flush();
	arStore.Close();
	// TODO: 在此添加命令处理程序代码
}

void CTreepadView::OnFileOpen()
{
	BOOL isOpen = TRUE;
	CString defaultDir = L"D:\\Treepad\\Iteration";
	CString filter = L"Treepad File (*.tpf)||";
	CString fileName = L"file1.tpf";
	CFileDialog openFileDlg(isOpen, L"tpf", fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	if (result == IDOK) {
		CClientDC dc(this);
		CBranch_Ser b(0,0,0,0);
		CString filePath = openFileDlg.GetPathName();
		CFile myFile(filePath, CFile::modeRead);
		myFile.SeekToBegin();
		CArchive arLoad(&myFile, CArchive::load);
		CBranch_Ser* t = (CBranch_Ser*)arLoad.ReadObject(RUNTIME_CLASS(CBranch_Ser));
		tree = Trunk(CPoint(t->x1, t->y1), CPoint(t->x2, t->y2));
		for (int i = 0; i < t->num; ++i) {
			CBranch_Ser* p = (CBranch_Ser*)arLoad.ReadObject(RUNTIME_CLASS(CBranch_Ser));
			str.Format(L"x=%d y=%d", p->x1, p->y1);
			tree.addBranch(Branch(CPoint(p->x1, p->y1), CPoint(p->x2, p->y2)));
		}
		arLoad.Close();
		statu = MOVE_STATU;
		InvalidateRect(NULL);
	}
	// TODO: 在此添加命令处理程序代码
}




void CTreepadView::OnFileSaveAs()
{
	if (!tree.size()) {
		return;
	}
	BOOL isOpen = FALSE;
	CString defaultDir = L"D:\\Treepad\\Iteration";
	CString filter = L"PNG File (*.png)||";
	CString fileName = L"image1.png";         //默认打开的文件名
	CFileDialog openFileDlg(isOpen, L"png", fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\" + fileName;
	if (result == IDOK) {
		filePath = openFileDlg.GetPathName();
		CRect rcBk;
		GetClientRect(&rcBk);
		CDC* pDC = GetDC();
		CImage Image;
		Image.Create(rcBk.Width(), rcBk.Height(), 24);
		::BitBlt(Image.GetDC(), 0, 0, rcBk.Width(), rcBk.Height(), pDC->m_hDC, 0, 0, SRCCOPY);
		Image.Save(filePath);
		Image.ReleaseDC();
		Image.Destroy();
	}
	// TODO: 在此添加命令处理程序代码
}


void CTreepadView::OnBnClickedClose()
{
	set->DestroyWindow();
	f5 = false;
	CString s("ok");
	CClientDC dc(this);
	dc.TextOutW(100, 100, s);
	delete set;
	set = NULL;
	// TODO: 在此添加控件通知处理程序代码
}
