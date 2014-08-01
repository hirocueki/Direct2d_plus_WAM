#include "stdafx.h"
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

void D2DDriver::ResizeRenderTarget()
{
	auto pRT = GetRenderTarget();
	if (!pRT)
		return;

	CRect rcSelf;
	::GetWindowRect(pRT->GetHwnd(), &rcSelf);
	pRT->Resize(D2D1::SizeU(rcSelf.Width(), rcSelf.Height()));
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

	if (m_spDWriteFactory)
		return hr;

	IFR(::DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED
		, __uuidof(IDWriteFactory)
		, reinterpret_cast<IUnknown**>(&m_spDWriteFactory))
		);

	//�e�L�X�g�Ɏg�p����t�H�[�}�b�g���m�ۂ��܂�
	GetDWriteFactory()->CreateTextFormat(
		L"Segoe UI",
		nullptr,
		DWRITE_FONT_WEIGHT_LIGHT,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		32.0f,
		L"en-US",
		&m_spTextFormat
		);

	m_spTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	m_spTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

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



#pragma region D2DTextDriver
#include <dwrite.h>

#pragma comment(lib, "dwrite.lib")

/*
*	�e�L�X�g�h���C�o���\�z���܂�
*
FontName			�t�H���g��
FontSizeDIP			�t�H���g�T�C�Y�i�f�o�C�X��ˑ��s�N�Z���Ŏw�肷��j
aTextAlignment
DWRITE_TEXT_ALIGNMENT_LEADING		�i���e�L�X�g�̐�[���A���C�A�E�g �{�b�N�X�̐�[�ɍ��킹�Ĕz�u����܂��B
DWRITE_TEXT_ALIGNMENT_TRAILING		�i���e�L�X�g�̖��[���A���C�A�E�g �{�b�N�X�̖��[�ɍ��킹�Ĕz�u����܂��B
DWRITE_TEXT_ALIGNMENT_CENTER		�i���e�L�X�g�̒��S���A���C�A�E�g �{�b�N�X�̒��S�ɍ��킹�Ĕz�u����܂��B
aParagraphAlignment
DWRITE_PARAGRAPH_ALIGNMENT_NEAR		�e�L�X�g�t���[�̏�[���A���C�A�E�g �{�b�N�X�̏�[�ɍ��킹�Ĕz�u����܂��B
DWRITE_PARAGRAPH_ALIGNMENT_FAR		�e�L�X�g �t���[�̉��[���A���C�A�E�g �{�b�N�X�̉��[�ɍ��킹�Ĕz�u����܂��B
DWRITE_PARAGRAPH_ALIGNMENT_CENTER	�e�L�X�g �t���[�̒��S���A���C�A�E�g �{�b�N�X�̒��S�ɍ��킹�Ĕz�u����܂��B
*/
HRESULT D2DTextDriver::SetUp(const CString& aFontName
	, FLOAT FontSizeDIP
	, DWRITE_TEXT_ALIGNMENT aTextAlignment /*=DWRITE_TEXT_ALIGNMENT_CENTER*/
	, DWRITE_PARAGRAPH_ALIGNMENT aParagraphAlignment /*=DWRITE_PARAGRAPH_ALIGNMENT_CENTER*/)
{
	HRESULT hr = S_OK;

	if (m_spFactory)
		return hr;

	IFR(::DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED
		, __uuidof(IDWriteFactory)
		, reinterpret_cast<IUnknown**>(&m_spFactory))
		);

	CString strFontName = aFontName;

	IFR(m_spFactory->CreateTextFormat(
		strFontName,
		NULL,						// �t�H���g�R���N�V�����iNULL=�V�X�e���t�H���g�j
		DWRITE_FONT_WEIGHT_NORMAL,	//�ӂ�		(DWRITE_FONT_WEIGHT_BOLD)
		DWRITE_FONT_STYLE_NORMAL,	//�X�^�C��(�X��)
		DWRITE_FONT_STRETCH_NORMAL,	// �����̐L�k	�iDWRITE_FONT_STRETCH_EXPANDED�j
		FontSizeDIP,				//�����̑傫��	DIP ("�f�o�C�X��ˑ��s�N�Z��") �P�ʂł̃t�H���g�̘_���T�C�Y�B1 DIP �� 1/96 �C���`�ł��B
		L"ja-jp",					//���P�[��(�n��)/ja-jp/en-us
		&m_spTextFormat
		));

	m_spTextFormat->SetTextAlignment(aTextAlignment);
	m_spTextFormat->SetParagraphAlignment(aParagraphAlignment);

	mInitialized = true;

	return hr;
}
#pragma endregion

