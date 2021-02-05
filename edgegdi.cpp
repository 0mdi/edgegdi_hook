#include "edgegdi.hpp"
#include "pattern.hpp"

#include <stdio.h>

uintptr_t edgegdi::g_pEdgeGdi = 0;
uintptr_t edgegdi::own_table[25] = { 0 };

//
// Original functions
edgegdi::BitBlt_t edgegdi::BitBltOrig = nullptr;
edgegdi::CreateDIBSection_t edgegdi::CreateDIBSectionOrig = nullptr;
edgegdi::CreateFontW_t edgegdi::CreateFontWOrig = nullptr;
edgegdi::CreateFontIndirectW_t edgegdi::CreateFontIndirectWOrig = nullptr;
edgegdi::CreateCompatibleDC_t edgegdi::CreateCompatibleDCOrig = nullptr;
edgegdi::CreateDCW_t edgegdi::CreateDCWOrig = nullptr;
edgegdi::CreateICW_t edgegdi::CreateICWOrig = nullptr;
edgegdi::InternalDeleteDC_t edgegdi::InternalDeleteDCOrig = nullptr;
edgegdi::InternalDeleteObject_t edgegdi::InternalDeleteObjectOrig = nullptr;
edgegdi::GetObjectType_t edgegdi::GetObjectTypeOrig = nullptr;
edgegdi::GetObjectW_t edgegdi::GetObjectWOrig = nullptr;
edgegdi::GetStockObject_t edgegdi::GetStockObjectOrig = nullptr;
edgegdi::GetTextMetricsW_t edgegdi::GetTextMetricsWOrig = nullptr;
edgegdi::GetTextExtentPoint32W_t edgegdi::GetTextExtentPoint32WOrig = nullptr;
edgegdi::SelectClipRgn_t edgegdi::SelectClipRgnOrig = nullptr;
edgegdi::SelectObject_t edgegdi::SelectObjectOrig = nullptr;
edgegdi::SetMapMode_t edgegdi::SetMapModeOrig = nullptr;
edgegdi::D3DKMTOpenAdapterFromHdc_t edgegdi::D3DKMTOpenAdapterFromHdcOrig = nullptr;
edgegdi::ExtCreateRegion_t edgegdi::ExtCreateRegionOrig = nullptr;
edgegdi::GdiTrackHCreate_t edgegdi::GdiTrackHCreate = nullptr;


int NotImplemented()
{
    return 1;
}


bool edgegdi::init()
{
    auto gdi32 = (uintptr_t)GetModuleHandleA("gdi32.dll");
    if (!gdi32)
        return false;

    /*
    48 89 05 ?? ?? ?? ?? E9 ?? ?? ?? ??
    \x48\x89\x05\x00\x00\x00\x00\xE9\x00\x00\x00\x00, xxx????x????
    */
    g_pEdgeGdi = FindPattern(gdi32, 0xFF000, "\x48\x89\x05\x00\x00\x00\x00\xE9\x00\x00\x00\x00", "xxx????x????", 0);
    if (g_pEdgeGdi)
    {
        uint32_t offset = *(uint32_t*)(g_pEdgeGdi + 3);
        g_pEdgeGdi = g_pEdgeGdi + 7 + offset;
    }
    printf("g_pEdgeGdi: 0x%p\n", g_pEdgeGdi);
    return g_pEdgeGdi != NULL;
}

bool edgegdi::hook(uintptr_t BitBltHook)
{
    auto win32u = GetModuleHandleA("win32u.dll");
    auto gdi32 = GetModuleHandleA("gdi32.dll");
    auto gdi32full = GetModuleHandleA("gdi32full.dll");
    auto ntdll = GetModuleHandleA("ntdll.dll");
    if (!gdi32full || !gdi32 || !win32u || !ntdll)
        return false;

    //
    // Initialize orig functions
    BitBltOrig = (BitBlt_t)GetProcAddress(gdi32full, "BitBlt");
    CreateDIBSectionOrig = (CreateDIBSection_t)GetProcAddress(gdi32full, "CreateDIBSection");
    CreateFontWOrig = (CreateFontW_t)GetProcAddress(gdi32full, "CreateFontW");
    CreateFontIndirectWOrig = (CreateFontIndirectW_t)GetProcAddress(gdi32full, "CreateFontIndirectW");
    CreateCompatibleDCOrig = (CreateCompatibleDC_t)GetProcAddress(gdi32full, "CreateCompatibleDC");
    CreateDCWOrig = (CreateDCW_t)GetProcAddress(gdi32, "bCreateDCW");
    CreateICWOrig = (CreateICW_t)CreateDCWOrig;
    InternalDeleteDCOrig = (InternalDeleteDC_t)GetProcAddress(gdi32, "InternalDeleteDC");
    InternalDeleteObjectOrig = (InternalDeleteObject_t)GetProcAddress(gdi32, "InternalDeleteObject");
    GetObjectTypeOrig = (GetObjectType_t)GetProcAddress(gdi32full, "GetObjectType");
    GetObjectWOrig = (GetObjectW_t)GetProcAddress(gdi32full, "GetObjectW");
    GetStockObjectOrig = (GetStockObject_t)GetProcAddress(gdi32full, "GetStockObject");
    GetTextMetricsWOrig = (GetTextMetricsW_t)GetProcAddress(gdi32full, "GetTextMetricsW");
    GetTextExtentPoint32WOrig = (GetTextExtentPoint32W_t)GetProcAddress(gdi32full, "GetTextExtentPoint32W");
    SelectClipRgnOrig = (SelectClipRgn_t)GetProcAddress(gdi32full, "SelectClipRgnImpl");
    SelectObjectOrig = (SelectObject_t)GetProcAddress(gdi32full, "SelectObjectImpl");
    SetMapModeOrig = (SetMapMode_t)GetProcAddress(gdi32full, "SetMapMode");
    D3DKMTOpenAdapterFromHdcOrig = (D3DKMTOpenAdapterFromHdc_t)GetProcAddress(win32u, "NtGdiDdDDIOpenAdapterFromHdc");
    ExtCreateRegionOrig = (ExtCreateRegion_t)GetProcAddress(gdi32full, "NtGdiExtCreateRegion");

    //
    // Initialize own_table
    own_table[0] = (uintptr_t)BitBltHook/*BitBltOrig*/;
    own_table[1] = (uintptr_t)CreateDIBSectionOrig;
    own_table[2] = (uintptr_t)CreateFontWOrig;
    own_table[3] = (uintptr_t)CreateFontIndirectWOrig;
    own_table[4] = (uintptr_t)CreateCompatibleDCOrig;
    own_table[5] = (uintptr_t)CreateDCWOrig;
    own_table[6] = (uintptr_t)CreateICWOrig;
    own_table[9] = (uintptr_t)InternalDeleteDCOrig;
    own_table[11] = (uintptr_t)InternalDeleteObjectOrig;
    own_table[12] = (uintptr_t)GetObjectTypeOrig;
    own_table[13] = (uintptr_t)GetObjectWOrig;
    own_table[14] = (uintptr_t)GetStockObjectOrig;
    own_table[15] = (uintptr_t)GetTextMetricsWOrig;
    own_table[16] = (uintptr_t)GetTextExtentPoint32WOrig;
    own_table[17] = (uintptr_t)SelectClipRgnOrig;
    own_table[18] = (uintptr_t)SelectObjectOrig;
    own_table[19] = (uintptr_t)SetMapModeOrig;
    own_table[22] = (uintptr_t)D3DKMTOpenAdapterFromHdcOrig;
    own_table[23] = (uintptr_t)NotImplemented;
    own_table[24] = (uintptr_t)NotImplemented;


    // Replace vtable ptr
    DWORD OldProtect;
    VirtualProtect((LPVOID)g_pEdgeGdi, sizeof(uintptr_t), PAGE_READWRITE, &OldProtect);
    *(uintptr_t**)g_pEdgeGdi = own_table;
    VirtualProtect((LPVOID)g_pEdgeGdi, sizeof(uintptr_t), OldProtect, &OldProtect);

    return true;
}
