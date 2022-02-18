/*
** Main DLL Entry point when loaded
**
*/

#include "stdafx.h"
#include "JShark.h"

bool Setup() {
    return JShark::ShimStart();
}

void ShutDown() {
    JShark::ShimStop();
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved
) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            if (!Setup())
                return FALSE;
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            ShutDown();
            break;
        default:
            break;
    }
    return TRUE;
}

