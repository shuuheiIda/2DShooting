//==========================
//
//フェード処理 [fade.h]
//Author shuuhei Ida
//
//==========================

#ifndef _FADE_H_					//
#define _FADE_H_					// 二重のインクルード防止のマクロ

#include "main.h"

// フェードの状態
typedef enum
{
	FADE_NONE =0,
	FADE_IN,
	FADE_OUT,
	FADE_MAX
}FADE;

// プロトタイプ宣言
void InitFade(MODE modeNext);

void UninitFade(void);

void UpdateFade(void);

void DrawFade(void);

void SetFade(MODE modeNext);

FADE GetFade(void);

#endif // !_Fade_H_