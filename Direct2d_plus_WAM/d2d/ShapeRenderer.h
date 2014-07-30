#pragma once

// コントロールの描画を行う
class ShapeRenderer
{
public:
	static HRESULT DrawPlay(ID2D1RenderTarget* pRT);
	static HRESULT DrawRectangle(ID2D1RenderTarget* pRT);
};

