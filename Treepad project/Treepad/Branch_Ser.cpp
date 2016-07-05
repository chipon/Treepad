#include "stdafx.h"
#include "Branch_Ser.h"

IMPLEMENT_SERIAL(CBranch_Ser, CObject, VERSIONABLE_SCHEMA | 2)
CBranch_Ser::CBranch_Ser()
{
}
CBranch_Ser::CBranch_Ser(int x11,int y11,int x21,int y21) {
	x1 = x11;
	y1 = y11;
	x2 = x21;
	y2 = y21;
	num = 0;
}
CBranch_Ser::CBranch_Ser(int x11, int y11, int x21, int y21,int n) {
	x1 = x11;
	y1 = y11;
	x2 = x21;
	y2 = y21;
	num = n;
}
void CBranch_Ser::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << x1 << y1 << x2 << y2 << num;
	}
	else {
		ar >> x1 >> y1 >> x2 >> y2 >> num;
	}
}
CBranch_Ser::~CBranch_Ser()
{
}
