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


const CString label1(_T("Enable Direct2D hoge"));
const CString label2(_T("ƒTƒ“ƒvƒ‹‚È‚Ì‚Å‚·"));

#define Margin 5.0f
#define Box_Height 100.0f
#define Control_Width 100.0f
#define Control_Height (Control_Width/2.0f)

#define ZeroF	0.0f

// rendering shapes
void CChildView::Render(HDC hDC)
{
	auto pRT = mDriver.GetRenderTarget();
	if (!pRT)
		return;

	if (!(pRT->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
	{
		pRT->BeginDraw();

		pRT->Clear(D2D1::ColorF(D2D1::ColorF::WhiteSmoke));

		const auto Canvas = pRT->GetSize();
		ASSERT(Canvas.width);
		ASSERT(Canvas.height);

		d2dutil::Boxies boxies;
		d2dutil::RectangleSlicer::Slice(CD2DRectF(ZeroF, ZeroF, Canvas.width, Canvas.height), 8, boxies);

		bool state = true;
		for each(auto box in boxies)
		{
			ToggleSwitch ts;
			ts.box = box;
			ts.label = label1;
			ts.on = state;
			
			state = !state;

			ToggleSwitchRenderer::Draw(pRT, mDriver.GetTextFormat(), ts);
		}


		if (pRT->EndDraw() == D2DERR_RECREATE_TARGET)
		{
			mDriver.RecreateDeviceIndependentResources(pRT->GetHwnd());
		}
	}
}
