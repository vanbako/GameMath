#pragma once

// Including SDKDDKVer.h defines the highest available Windows platform.
// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.
//#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN
#define OEMRESOURCE

#include <windows.h>
#include <d2d1.h>

template<class T>
void SafeRelease(T** ppT)
{
    if (*ppT != nullptr)
    {
        (*ppT)->Release();
        *ppT = nullptr;
    }
}
