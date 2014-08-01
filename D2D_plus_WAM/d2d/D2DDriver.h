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

	IDWriteFactory* GetDWriteFactory() const
	{
		return m_spDWriteFactory;
	}

	IDWriteTextFormat* GetTextFormat() const 
	{
		return m_spTextFormat;
	}

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
	CComPtr<IDWriteTextFormat> m_spTextFormat;
	CComPtr<IDWriteFactory> m_spDWriteFactory;
};


#pragma region D2Dテキスト描画ドライバー

class D2DTextDriver
{

public:
	D2DTextDriver() : mInitialized(false) {}

	HRESULT SetUp(const CString& aFontName
		, FLOAT FontSizeDIP
		, DWRITE_TEXT_ALIGNMENT aTextAlignment = DWRITE_TEXT_ALIGNMENT_CENTER
		, DWRITE_PARAGRAPH_ALIGNMENT aParagraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	IDWriteTextFormat* GetTextFormat(){ return m_spTextFormat; }

	bool IsInitialized() { return mInitialized; }


private:

	CComPtr<IDWriteTextFormat> m_spTextFormat;
	CComPtr<IDWriteFactory> m_spFactory;
	bool mInitialized;

};
#pragma endregion

#include <vector>
namespace d2dutil
{

#pragma region 矩形を指定個数分に分割するクラス
	typedef	std::vector<CD2DRectF>	Boxies;
	class RectangleSlicer
	{
	public:
#define BoxMargin 1.0f
		static int Slice(const CD2DRectF& region, int partsNumber, Boxies& boxies)
		{
			boxies.clear();
			const auto height = (region.bottom - region.top)/ partsNumber;
			
			CD2DRectF box = region;

			for (int i = 0; i<partsNumber; i++)
			{
				box.bottom = box.top + height;
				boxies.push_back(box);
				box.top = box.bottom + BoxMargin;
			}
			return (int)boxies.size();
		}
	};

#pragma endregion
};