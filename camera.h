//==========================
//
// カメラ処理 [casmera.h]
// Author:shuuhei Ida
//
//==========================

#ifndef _CAMERA_H_					//
#define _CAMERA_H_					// 二重のインクルード防止のマクロ4

#include "main.h"

#define WORLD_WIDTH	(2560)
#define WORLD_HEIGHT (720)

// プロトタイプ宣言
void InitCamera(void);
void UpdateCamera(D3DXVECTOR3 pos);
D3DXVECTOR3 Screenposition(D3DXVECTOR3 pos);

#endif // !_CAMERA_H_
