#pragma once


#define SUCKSEX 0

#ifdef IS_DEBUG
#define shit(tit) L##tit
//#define CHK_ERR(hress, coc) case hress: return std::wstring(shit(coc)); break;
#define __ERROR(error_type, file) MessageBox(nullptr,(L"LINE: " + std::to_wstring(__LINE__) +L" FILE: " + shit(file)).c_str(), error_type, MB_OK | MB_ICONEXCLAMATION);
#define ERROR_BOOL(boolean) if(!boolean) {__ERROR(L"NOT 1(TRUE)", __FILE__)}
#define ERROR_IF(condition, error_type)  if(condition) {__ERROR(error_type, __FILE__)}
#define ERROR_MAINLOOP(mainLoop) if(mainLoop != SUCKSEX) {__ERROR(L"Main Loop Failed", __FILE__)}
#else
#define ERROR_BOOL(boolean) boolean
#define ERROR_IF(condition, error_type)
#define ERROR_MAINLOOP(mainLoop) if(mainLoop != SUCKSEX)///TODO Log feature
#endif