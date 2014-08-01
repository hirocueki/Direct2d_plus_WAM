
#pragma once

#include "d2d\D2DDriver.h"

class CChildView : public CWnd
{
public:
	CChildView();

private:

	D2DDriver mDriver;
	D2DTextDriver m_textDriver;

	void Render(HDC hDC);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual ~CChildView();

	// 生成された、メッセージ割り当て関数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

