#pragma once


#include <windows.h>	// rand() 等を使用するために。
#include <tchar.h> // Unicode対応の _T() 関数を使用するために。


//--------------------------------------------------------------------------------
// 定義
//--------------------------------------------------------------------------------

#define PRT_LEN_MAX 256			// 最大256文字まで出力可


class Core {
public:
	HANDLE hConsoleWindow;
public:

	// printf()の代用関数。コンソールに出力。
	void PRT(		
		const _TCHAR* format,
		...
		);

	// 一時的にWindowsに制御を渡します。
	// 思考中にこの関数を呼ぶと思考中断ボタンが有効になります。
	// 毎秒30回以上呼ばれるようにするとスムーズに中断できます。
	static void YieldWindowsSystem(
		void
		);

};

