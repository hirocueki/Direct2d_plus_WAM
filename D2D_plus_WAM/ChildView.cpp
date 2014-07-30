#include "stdafx.h"
#include "D2D_plus_WAM.h"
#include "ChildView.h"


#include "d2d/ShapeRenderer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this);
	
	Render(dc);
}



int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	VERIFY( SUCCEEDED(mDriver.Initialize(GetSafeHwnd()) ));

	return 0;
}


void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	mDriver.ResizeRenderTarget();

}


// rendering shapes
void CChildView::Render(HDC hDC)
{
	auto pRT = mDriver.GetRenderTarget();
	if (!pRT)
		return;

	if (!(pRT->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
	{
		pRT->BeginDraw();

		pRT->Clear(D2D1::ColorF(D2D1::ColorF::Black));

		ShapeRenderer::DrawRectangle(pRT);

		if (pRT->EndDraw() == D2DERR_RECREATE_TARGET)
		{
			mDriver.RecreateDeviceIndependentResources(pRT->GetHwnd());
		}
	}
}
