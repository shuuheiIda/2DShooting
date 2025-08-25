//==========================
//
// シューティングゲームα版
// Author shuuhei Ida
//
//==========================
#ifndef _GAME_H_
#define _GAME_H_

#include"main.h"

// プロトタイプ宣言

void InitGame(void);

void UninitGame(void);

void UpdateGame(void);

void DrawGame(void);

void SetEnablePause(bool bPause);

#endif // !_GAME_H_
