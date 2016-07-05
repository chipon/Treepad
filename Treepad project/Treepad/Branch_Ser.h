#pragma once
#include "afx.h"
class CBranch_Ser :
	public CObject
{
public:
	int x1, y1;	//起点
	int x2, y2;	//终点
	int num;	//分支个数
	DECLARE_SERIAL(CBranch_Ser)
	CBranch_Ser::CBranch_Ser(int x11, int y11, int x21, int y21);
	CBranch_Ser(int x11, int y11, int x21, int y21, int n);
	void Serialize(CArchive& ar);
	CBranch_Ser();
	~CBranch_Ser();
};

