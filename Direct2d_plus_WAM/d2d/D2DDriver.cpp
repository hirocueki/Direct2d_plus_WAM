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
 *	�f�o�C�X�ˑ����\�[�X�̍č\�z�i�f�o�C�X���X�g�p�j
 */
HRESULT D2DDriver::RecreateDeviceIndependentResources(HWND hWnd)
{
	m_hWnd = hWnd;
	DiscardDeviceIndependentResources();
	return CreateDeviceIndependentResources();
}


/*
 *	�f�o�C�X�Ɉˑ����郊�\�[�X���\�z���܂��B
 *	D2D�t�@�N�g�����烌���_�[�^�[�Q�b�g��r�b�g�}�b�v�̃��\�[�X���쐬���邱�Ƃ��ł��܂��B
 */
HRESULT D2DDriver::CreateDeviceIndependentResources( void )
{
	HRESULT hr = S_OK;

	IFR( CreateResources() );

	// �����_�[�^�[�Q�b�g�̍쐬
	IFR( CreateRenderTarget() );

	return hr;
}




/*�f�o�C�X�ˑ����\�[�X�̊J��*/
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
