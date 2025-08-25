//==========================
//
// カメラ処理 [casmera.h]
// Author:shuuhei Ida
//
//==========================

#include "camera.h"

D3DXVECTOR3 g_Camerapos;

//==========================
// カメラの初期化処理
//==========================
void InitCamera(void)
{
	 g_Camerapos ={ (float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f ,0.0f}; 
}
//==========================
// カメラの更新処理
//==========================
void UpdateCamera(D3DXVECTOR3 pos)
{
	g_Camerapos = pos;


	if (g_Camerapos.x - (float)SCREEN_WIDTH * 0.5f <= 0.0f)		//画面左端に行ったとき
	{
		g_Camerapos.x = (float)SCREEN_WIDTH * 0.5f;
	}
	else if (g_Camerapos.x + (float)SCREEN_WIDTH * 0.5f >= WORLD_WIDTH)		//画面右端に行ったとき
	{
		g_Camerapos.x = WORLD_WIDTH - (float)SCREEN_WIDTH * 0.5f;
	}
	
	if (g_Camerapos.y - (float)SCREEN_HEIGHT * 0.5f <= 0.0f)
	{
		g_Camerapos.y = (float)SCREEN_HEIGHT * 0.5f;
	}
	else if (g_Camerapos.y + (float)SCREEN_HEIGHT * 0.5f >= WORLD_HEIGHT)
	{
		g_Camerapos.y = WORLD_HEIGHT - (float)SCREEN_HEIGHT * 0.5f;
	}
}
//==========================
// カメラ座標変換処理
//==========================
D3DXVECTOR3 Screenposition(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 screen_origin_position = D3DXVECTOR3(g_Camerapos.x - (float)SCREEN_WIDTH * 0.5f, g_Camerapos.y - (float)SCREEN_HEIGHT * 0.5f, 0.0f);

	// ワールド座標からスクリーン座標に変換する
	D3DXVECTOR3 screen_position = D3DXVECTOR3(pos.x - screen_origin_position.x, pos.y - screen_origin_position.y, 0.0f);

	return screen_position;
}


