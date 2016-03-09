//
// ui.h
//
#pragma once

#include <tchar.h> // Unicode対応の _T() 関数を使用するために。

// サンプルで使用する関数
void PRT(const _TCHAR* fmt, ...);	// printf()の代用関数。コンソールに出力。
void PassWindowsSystem(void);	// 一時的にWindowsに制御を渡します。
