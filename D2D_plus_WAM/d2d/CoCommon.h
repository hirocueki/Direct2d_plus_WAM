#pragma once


// IFR �}�N��
#ifndef IFR
#define IFR(expr) do {hr = (expr); _ASSERT(SUCCEEDED(hr)); if (FAILED(hr)) return(hr);} while(0)
#endif

