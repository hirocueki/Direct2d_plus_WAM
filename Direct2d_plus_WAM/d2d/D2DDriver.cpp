#include "../stdafx.h"
#include "D2DDriver.h"

D2DDriver::D2DDriver()
{
}

D2DDriver::~D2DDriver()
{
}

HRESULT D2DDriver::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;

	HRESULT hr = S_OK;

    IFR( CreateDeviceIndependentResources() );
   
    return hr;
}


HRESULT D2DDriver::CreateRenderTarget()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(m_hWnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	const auto pixelFormat = D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_PREMULTIPLIED
		);

	const auto props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, pixelFormat );

	IFR( GetFactory()->CreateHwndRenderTarget(
		props,
		D2D1::HwndRenderTargetProperties(m_hWnd, size),
		&m_pRenderTarget ) );

    return hr;

}

ID2D1HwndRenderTarget* D2DDriver::GetRenderTarget() const
{
    return m_pRenderTarget;
}

   
/*
 *	デバイス依存リソースの再構築（デバイスロスト用）
 */
HRESULT D2DDriver::RecreateDeviceIndependentResources(HWND hWnd)
{
	m_hWnd = hWnd;
	DiscardDeviceIndependentResources();
	return CreateDeviceIndependentResources();
}


/*
 *	デバイスに依存するリソースを構築します。
 *	D2Dファクトリからレンダーターゲットやビットマップのリソースを作成することができます。
 */
HRESULT D2DDriver::CreateDeviceIndependentResources( void )
{
	HRESULT hr = S_OK;

	IFR( CreateResources() );

	// レンダーターゲットの作成
	IFR( CreateRenderTarget() );

	return hr;
}




/*デバイス依存リソースの開放*/
VOID D2DDriver::DiscardDeviceIndependentResources(void)
{
	m_pD2DFactory.Release();
	m_pRenderTarget.Release();
}

ID2D1Factory* D2DDriver::GetFactory() const
{
	return m_pD2DFactory;
}

HRESULT D2DDriver::CreateResources()
{
	HRESULT hr = S_OK;

	IFR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory));

	return hr;
}
