#pragma once


#define SUCKSEX 0

#ifdef IS_DEBUG
#define DX_DEBUG_LAYER D3D11_CREATE_DEVICE_DEBUG
#define shit(tit) L##tit
#define __ERROR(error_type, file) MessageBox(nullptr,(L"LINE: " + std::to_wstring(__LINE__) +L" FILE: " + shit(file)).c_str(), error_type, MB_OK | MB_ICONEXCLAMATION);
#define ERROR_BOOL(boolean) if(!boolean) {__ERROR(L"NOT 1(TRUE)", __FILE__)}
#define ERROR_IF(condition, error_type)  if(condition) {__ERROR(error_type, __FILE__)}
#define ERROR_MAINLOOP(mainLoop) if(mainLoop != SUCKSEX) {__ERROR(L"Main Loop Failed", __FILE__)}
#define ERROR_HRESULT(hressult) if(hres = hressult != S_OK) __ERROR(std::to_wstring(hres).c_str(), __FILE__)
#define ERROR_PRESENT(hressult) if(hressult == DXGI_ERROR_DEVICE_REMOVED) {__ERROR(L"DEVICE REMOVED! ", __FILE__);}
#else
#define DX_DEBUG_LAYER 0
#define ERROR_BOOL(boolean) boolean
#define ERROR_IF(condition, error_type)
#define ERROR_MAINLOOP(mainLoop) if(mainLoop != SUCKSEX)///TODO Log feature
#define ERROR_HRESULT(hressult) hressult
#define ERROR_PRESENT(hressult) hressult
#endif