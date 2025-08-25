//==========================
//
//ポーズ処理 [pause.h]
//Author:shuuhei Ida
//
//==========================

#ifndef _PAUSE_H_					//
#define _PAUSE_H_					// 二重のインクルード防止のマクロ

#include "main.h"

typedef enum
{
	PAUSE_CONTINUE =0,
	PAUSE_RETRY,
	PAUSE_QUIT,
	PAUSE_MAX
}PAUSE;

// プロトタイプ宣言
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

#endif // !_PAUSE_H_