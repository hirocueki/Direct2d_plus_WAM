#pragma once


class ToggleSwitch
{
public:

	CD2DRectF box;
	CString label;
	bool on;
};

// �R���g���[���̕`����s��
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
