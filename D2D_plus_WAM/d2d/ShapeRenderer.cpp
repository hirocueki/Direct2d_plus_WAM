#include "stdafx.h"
#include "ShapeRenderer.h"
#include "CoCommon.h"




HRESULT ShapeRenderer::DrawRectangle(ID2D1RenderTarget* pRT)
{
	HRESULT hr = S_OK;
	const auto Canvas = pRT->GetSize();
	ASSERT(Canvas.width);
	ASSERT(Canvas.height);

	const auto center = D2D1::Point2F(Canvas.width / 2, Canvas.height / 2);

	CComPtr<ID2D1SolidColorBrush> spBrush_Rect;
	IFR(pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DeepSkyBlue), &spBrush_Rect));

	CComPtr<ID2D1SolidColorBrush> spBrush_Circle;
	IFR(pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DeepPink), &spBrush_Circle));


	const auto rectangle = CD2DRectF(
		center.x - 100,
		center.y - 100,
		center.x + 100,
		center.y + 100);

	pRT->FillRectangle(rectangle, spBrush_Rect);

	const auto circle = D2D1::Ellipse(center, 20, 20);
	pRT->FillEllipse(circle, spBrush_Circle);

	return hr;
}

HRESULT ShapeRenderer::DrawPlay(ID2D1RenderTarget* pRT)
{
#define CIRCLE_RADIUS_RATIO	6
#define CIRCLE_STROKE		5.0f
	HRESULT hr = S_OK;

	const auto Canvas = pRT->GetSize();
	ASSERT(Canvas.width);
	ASSERT(Canvas.height);

	const auto center = D2D1::Point2F(Canvas.width / 2, Canvas.height / 2);

	CComPtr<ID2D1SolidColorBrush> spBrush;
	IFR(pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DeepSkyBlue), &spBrush));

	CComPtr<ID2D1SolidColorBrush> spBrushMic;
	IFR(pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::WhiteSmoke), &spBrushMic));

	// 中央の円を描画
	auto CircleRadius = Canvas.height / CIRCLE_RADIUS_RATIO;
	auto el = D2D1::Ellipse(center, CircleRadius, CircleRadius);
	pRT->FillEllipse(el, spBrush);
	pRT->DrawEllipse(el, spBrushMic, CIRCLE_STROKE);


	auto ArcRadius = CircleRadius / 3;
	auto MicStandStroke = 2.0f;

	auto MicRoundRect = D2D1::RectF(center.x - CircleRadius / 8
		, center.y - CircleRadius / 2
		, center.x + CircleRadius / 8
		, center.y + CircleRadius / 6);

	auto MicRoundRoundRect = D2D1::RoundedRect(MicRoundRect, CircleRadius / 8, CircleRadius / 8);

	auto fInflate = (MicRoundRect.right - MicRoundRect.left) / 3;
	auto ArcMicRect = D2D1::RectF(
		MicRoundRect.left - fInflate,
		MicRoundRect.top - fInflate,
		MicRoundRect.right + fInflate,
		MicRoundRect.bottom + fInflate);

	auto rcHalfArc = D2D1::RectF(ArcMicRect.left - MicStandStroke
		, ArcMicRect.top - MicStandStroke
		, ArcMicRect.right + MicStandStroke
		, ArcMicRect.bottom - (ArcMicRect.bottom - ArcMicRect.top) / 2);

	auto ArcMicRoundRect = D2D1::RoundedRect(ArcMicRect, CircleRadius / 4, CircleRadius / 4);

	auto ptStandLine1_1 = D2D1::Point2F(center.x, ArcMicRect.bottom);
	auto ptStandLine1_2 = D2D1::Point2F(center.x, ptStandLine1_1.y + ArcRadius / 2);

	auto ptStandLine2_1 = D2D1::Point2F(center.x - ArcRadius / 2, ptStandLine1_2.y);
	auto ptStandLine2_2 = D2D1::Point2F(center.x + ArcRadius / 2, ptStandLine1_2.y);


	// マイクスタンド部分の描画
	pRT->DrawRoundedRectangle(ArcMicRoundRect, spBrushMic, MicStandStroke);
	pRT->FillRectangle(rcHalfArc, spBrush);
	pRT->DrawLine(ptStandLine1_1, ptStandLine1_2, spBrushMic, MicStandStroke);
	pRT->DrawLine(ptStandLine2_1, ptStandLine2_2, spBrushMic, MicStandStroke);


	// マイクヘッド部分の描画
	pRT->FillRoundedRectangle(MicRoundRoundRect, spBrushMic);
	return hr;
}

/////////////////////////////////////////////////

// ポイントを中心とした矩形を返却する
CD2DRectF MakeRectToCenterPoint(D2D1_POINT_2F center, FLOAT width, FLOAT height)
{
	const CD2DRectF rect(
		center.x - width / 2,
		center.y - height / 2,
		center.x + width / 2,
		center.y + height/ 2
		);

	return rect;
}


HRESULT ToggleSwitchRenderer::Draw(ID2D1RenderTarget* pRT, IDWriteTextFormat* pTF, const ToggleSwitch& ts)
{
	HRESULT hr = S_OK;


	const auto Canvas = pRT->GetSize();
	ASSERT(Canvas.width);
	ASSERT(Canvas.height);

#define Control_Width 100.0f
#define Control_Height (Control_Width/2.0f)


#define ControlBall_Width	Control_Height-5.0f
#define ControlBall_Height	Control_Height-5.0f

	const CD2DRectF box = ts.box;
	const CString label = ts.label;
	const bool on = ts.on;

	const auto control_center = CD2DPointF(
		(box.right - box.left) / 4 * 3,
		box.top + ((box.bottom - box.top) / 2)
		);
	const CD2DRectF control = MakeRectToCenterPoint(control_center, Control_Width, Control_Height);
	
	const auto control_ball_center =
		on ?
		CD2DPointF(control_center.x + Control_Width / 4, control_center.y) :
		CD2DPointF(control_center.x - Control_Width / 4, control_center.y);


	const CD2DRectF control_ball = MakeRectToCenterPoint(control_ball_center, ControlBall_Width, ControlBall_Height);

	

	CComPtr<ID2D1SolidColorBrush> spBrushText;
	IFR(pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &spBrushText));

	CComPtr<ID2D1SolidColorBrush> spBrush;
	IFR(pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::WhiteSmoke), &spBrush));

	CComPtr<ID2D1SolidColorBrush> spBrushControl;
	IFR(pRT->CreateSolidColorBrush(
		on ? D2D1::ColorF(D2D1::ColorF::LightGreen) : D2D1::ColorF(D2D1::ColorF::LightGray)
		, &spBrushControl));

	CComPtr<ID2D1SolidColorBrush> spBrushShadow;
	IFR(pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightGray), &spBrushShadow));
	
	CComPtr<ID2D1SolidColorBrush> spBrushBall;
	IFR(pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &spBrushBall));


	pRT->FillRectangle(box, spBrush);
	pRT->FillRectangle(control, spBrushControl);
	pRT->DrawRectangle(control, spBrushShadow);
	pRT->FillRectangle(control_ball, spBrushBall);
	


	// メニューのレンダリング
	pRT->DrawText(
		label,
		_tcslen(label),
		pTF,
		box,
		spBrushText,
		D2D1_DRAW_TEXT_OPTIONS_NO_SNAP,
		DWRITE_MEASURING_MODE_NATURAL);

	return hr;
}
