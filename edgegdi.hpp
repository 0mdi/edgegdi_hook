#pragma once
#include <Windows.h>
#include <stdint.h>


namespace edgegdi
{
	extern uintptr_t g_pEdgeGdi;
	extern uintptr_t own_table[25];

	bool init();
	bool hook(uintptr_t BitBltHook);

	//
	// All original functions
	using BitBlt_t = BOOL(*)(HDC, int, int, int, int, HDC, int, int, DWORD);
	extern BitBlt_t BitBltOrig;
	using CreateDIBSection_t = HBITMAP(*)(HDC, BITMAPINFO*, UINT, void**, HANDLE, DWORD);
	extern CreateDIBSection_t CreateDIBSectionOrig;
	using CreateFontW_t = HFONT(*)(int, int, int, int, int, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPCWSTR);
	extern CreateFontW_t CreateFontWOrig;
	using CreateFontIndirectW_t = HFONT(*)(LOGFONTW*);
	extern CreateFontIndirectW_t CreateFontIndirectWOrig;
	using CreateCompatibleDC_t = HDC(*)(HDC);
	extern CreateCompatibleDC_t CreateCompatibleDCOrig;
	using CreateDCW_t = HDC(*)(LPCWSTR, LPCWSTR, LPCWSTR, DEVMODEW*);
	extern CreateDCW_t CreateDCWOrig;
	using CreateICW_t = HDC(*)(LPCWSTR, LPCWSTR, LPCWSTR, DEVMODEW*);
	extern CreateICW_t CreateICWOrig;
	using InternalDeleteDC_t = BOOL(*)(PVOID);
	extern InternalDeleteDC_t InternalDeleteDCOrig;
	using InternalDeleteObject_t = BOOL(*)(PVOID);
	extern InternalDeleteObject_t InternalDeleteObjectOrig;
	using GetObjectType_t = DWORD(*)(HGDIOBJ);
	extern GetObjectType_t GetObjectTypeOrig;
	using GetObjectW_t = int(*)(HANDLE, int, LPVOID);
	extern GetObjectW_t GetObjectWOrig;
	using GetStockObject_t = HGDIOBJ(*)(int);
	extern GetStockObject_t GetStockObjectOrig;
	using GetTextMetricsW_t = BOOL(*)(HDC, LPTEXTMETRICW);
	extern GetTextMetricsW_t GetTextMetricsWOrig;
	using GetTextExtentPoint32W_t = BOOL(*)(HDC, LPCWSTR, int, LPSIZE);
	extern GetTextExtentPoint32W_t GetTextExtentPoint32WOrig;
	using SelectClipRgn_t = int(*)(HDC, HRGN);
	extern SelectClipRgn_t SelectClipRgnOrig;
	using SelectObject_t = HGDIOBJ(*)(HDC, HGDIOBJ);
	extern SelectObject_t SelectObjectOrig;
	using SetMapMode_t = int(*)(HDC, int);
	extern SetMapMode_t SetMapModeOrig;
	using D3DKMTOpenAdapterFromHdc_t = PVOID(*)(HDC);
	extern D3DKMTOpenAdapterFromHdc_t D3DKMTOpenAdapterFromHdcOrig;
	using ExtCreateRegion_t = HRGN(*)(XFORM*, DWORD, RGNDATA*);
	extern ExtCreateRegion_t ExtCreateRegionOrig;
	using GdiTrackHCreate_t = void(*)(HDC);
	extern GdiTrackHCreate_t GdiTrackHCreate;
}