#pragma once


class ToggleSwitch
{
public:

	CD2DRectF box;
	CString label;
	bool on;
};

// コントロールの描画を行う
class ShapeRenderer
{
public:
	static HRESULT DrawPlay(ID2D1RenderTarget* pRT);
	static HRESULT DrawRectangle(ID2D1RenderTarget* pRT);
};

class ToggleSwitchRenderer
{
public:
	static HRESULT Draw(ID2D1RenderTarget* pRT, IDWriteTextFormat* pTF, const ToggleSwitch& ts);

};
