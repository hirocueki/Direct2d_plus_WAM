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

	if (m_spDWriteFactory)
		return hr;

	IFR(::DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED
		, __uuidof(IDWriteFactory)
		, reinterpret_cast<IUnknown**>(&m_spDWriteFactory))
		);

	//テキストに使用するフォーマットを確保します
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



#pragma region D2DTextDriver
#include <dwrite.h>

#pragma comment(lib, "dwrite.lib")

/*
*	テキストドライバを構築します
*
FontName			フォント名
FontSizeDIP			フォントサイズ（デバイス非依存ピクセルで指定する）
aTextAlignment
DWRITE_TEXT_ALIGNMENT_LEADING		段落テキストの先端が、レイアウト ボックスの先端に合わせて配置されます。
DWRITE_TEXT_ALIGNMENT_TRAILING		段落テキストの末端が、レイアウト ボックスの末端に合わせて配置されます。
DWRITE_TEXT_ALIGNMENT_CENTER		段落テキストの中心が、レイアウト ボックスの中心に合わせて配置されます。
aParagraphAlignment
DWRITE_PARAGRAPH_ALIGNMENT_NEAR		テキストフローの上端が、レイアウト ボックスの上端に合わせて配置されます。
DWRITE_PARAGRAPH_ALIGNMENT_FAR		テキスト フローの下端が、レイアウト ボックスの下端に合わせて配置されます。
DWRITE_PARAGRAPH_ALIGNMENT_CENTER	テキスト フローの中心が、レイアウト ボックスの中心に合わせて配置されます。
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
		NULL,						// フォントコレクション（NULL=システムフォント）
		DWRITE_FONT_WEIGHT_NORMAL,	//ふつう		(DWRITE_FONT_WEIGHT_BOLD)
		DWRITE_FONT_STYLE_NORMAL,	//スタイル(傾き)
		DWRITE_FONT_STRETCH_NORMAL,	// 文字の伸縮	（DWRITE_FONT_STRETCH_EXPANDED）
		FontSizeDIP,				//文字の大きさ	DIP ("デバイス非依存ピクセル") 単位でのフォントの論理サイズ。1 DIP は 1/96 インチです。
		L"ja-jp",					//ロケール(地域)/ja-jp/en-us
		&m_spTextFormat
		));

	m_spTextFormat->SetTextAlignment(aTextAlignment);
	m_spTextFormat->SetParagraphAlignment(aParagraphAlignment);

	mInitialized = true;

	return hr;
}
#pragma endregion

