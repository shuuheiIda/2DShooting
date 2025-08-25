//==========================
//
// シューティングゲームα版
// Author shuuhei Ida
//
//==========================
#include "game.h"
#include"player.h"
#include"bg.h"
#include "input.h"
#include"bullet.h"
#include "explosion .h"
#include"enemy.h"
#include"score.h"
#include"fade.h"
#include"effect.h"
#include"sound.h"
#include"pause.h"
#include"camera.h"
#include"time.h"

bool g_bPause = false;		// ポーズ状態のオン/オフ

//===================
// ゲームの初期化処理
//===================
void InitGame(void)
{

	InitCamera();

	InitBG();
	
	InitExplosion();
	
	InitBullet();
	
	InitPlayer();
	
	InitScore();

	InitEffect();

	// 敵の初期化処理
	InitEnemy();
	SetEnemy(D3DXVECTOR3(1000.0f, 150.0f, 0.0f), 0);
	SetEnemy(D3DXVECTOR3(1000.0f, 250.0f, 0.0f), 1);
	SetEnemy(D3DXVECTOR3(1000.0f, 350.0f, 0.0f), 2);
	SetEnemy(D3DXVECTOR3(1000.0f, 450.0f, 0.0f), 3);
	SetScore(0);
	InitTime();

	// サウンドの再生
	PlaySound(SOUND_LABEL_BGM001);

	InitPause();

	g_bPause = false;
}

//==================
// ゲームの終了処理
//==================
void UninitGame(void)
{
	StopSound();

	UninitBG();
	
	UninitPlayer();
	
	UninitExplosion();
	
	UninitEnemy();
	
	UninitScore();

	UninitTime();

	UninitEffect();

	UninitPause();


}

//=================
// ゲームの更新処理
//=================
void UpdateGame(void)
{
	Player* pPlayer = GetPlayer();

	int nNoDeadenemy;
	// 敵の取得
	nNoDeadenemy = Enemycount();

	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = (g_bPause) ? false : true;
	}

	if (g_bPause == false)
	{
		UpdateCamera(pPlayer->pos);

		UpdateBG();
	
		UpdateBullet();
	
		UpdatePlayer();
	
		UpdateExplosion();
	
		UpdateEnemy();

		UpdateEffect();

		UpdateTime();

	}
	else
	{
		// ポーズのみ更新
		UpdatePause();
	}

	if (nNoDeadenemy == 0)
	{
		SetFade(MODE_RESULT);
	}

	//if (GetKeyboardTrigger(DIK_RETURN) == true)
	//{// 決定キーが押された
	//	// モード設定(ゲーム画面に移行)
	//	SetFade(MODE_RESULT);
	//}

}

//==================
// ゲームの描写処理
//==================
void DrawGame(void)
{
	DrawBG();

	DrawPlayer();

	DrawBullet();

	DrawExplosion();

	DrawEnemy();

	DrawScore();

	DrawTime();

	DrawEffect();
	
	if (g_bPause == true)
	{
		// ポーズを描画
		DrawPause();
	}
}

//==================
// ポーズの設定
//==================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}
