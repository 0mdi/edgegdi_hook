#include <Windows.h>
#include <stdio.h>
#include "edgegdi.hpp"


BOOL BitBltHook(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop)
{
    auto CurrentPid = GetCurrentProcessId();
    auto CurrentTid = GetCurrentThreadId();

    char buf[256] = { 0 };
    sprintf_s(buf, sizeof(buf), "%s (PID: %i, TID: %i) (X: %i (%i), Y: %i (%i)) (X: %i, Y: %i) (%X, %X)",
        __FUNCTION__,
        CurrentPid, CurrentTid,
        x, cx, y, cy,
        x1, y1,
        hdc, hdcSrc);
    printf(buf);

    return edgegdi::BitBltOrig(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
}

int main()
{
    SetConsoleTitleA("EDGEGDI.EXE");
    printf("Calling BitBlt.\n");
    BitBlt(nullptr, 0, 0, 0, 0, nullptr, 0, 0, 0);

    printf("Hooking BitBlt.\n");
    if (!edgegdi::init() || !edgegdi::hook((uintptr_t)BitBltHook))
    {
        printf("Failed to hook BitBlt.\n");
        return -1;
    }

    printf("Calling BitBlt again.\n");
    BitBlt((HDC)0xB16B00B5, 0, 0, 0, 0, (HDC)0xCAFEBABE, 0, 0, 0);

    getchar();
    return 0;
}