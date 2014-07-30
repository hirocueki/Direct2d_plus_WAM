#pragma once

#include <memory>
#include "d2d1.h"
#include <d2d1_1.h>
#include "CoCommon.h"

#pragma comment(lib, "d2d1")

//***********************************************************
// D2D1HwndRenderTarget をレンダーターゲットとするドライバークラス
class D2DDriver
{
public:
	D2DDriver();
	~D2DDriver();

	HRESULT Initialize(HWND hWnd);
	HRESULT RecreateDeviceIndependentResources(HWND hWnd);
    ID2D1HwndRenderTarget* GetRenderTarget() const;

	void ResizeRenderTarget();


private:
	HRESULT CreateResources();
	HRESULT CreateRenderTarget();
	HRESULT CreateDeviceIndependentResources( void );
	VOID DiscardDeviceIndependentResources(void);

	ID2D1Factory* GetFactory() const;


private:
	HWND m_hWnd;
	CComQIPtr<ID2D1Factory> m_pD2DFactory;
	CComQIPtr<ID2D1HwndRenderTarget> m_pRenderTarget;
};