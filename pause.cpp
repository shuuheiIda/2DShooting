//==========================
//
//フェード処理 [pause.cpp]
//Author:shuuhei Ida
//
//==========================

//インクルードファイル
#include "pause.h"
#include"game.h"
#include"fade.h"
#include"main.h"
#include"input.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePause[PAUSE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;	// 頂点バッファへのポインタ

int g_nSerect = 0;

//=================
//初期化処理
//=================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\8.png", &g_pTexturePause[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\9.png", &g_pTexturePause[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\quit.png", &g_pTexturePause[2]);

	VERTEX_2D* pVtx;		// 頂点情報へのポインタ

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < PAUSE_MAX; nCount++)
	{
		// 頂点座標の設定(x,y,z,の順番になる、zの値は2Dの場合は必ず0にする)
		pVtx[0].pos = D3DXVECTOR3(350.0f, 100.0f + (nCount * 120.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(600.0f, 100.0f + (nCount * 120.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(350.0f, 200.0f + (nCount * 120.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(600.0f, 200.0f + (nCount * 120.0f), 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

	g_nSerect = 0;
}

//==================
//終了処理
//==================
void UninitPause(void)
{
	//テクスチャの破棄
	for (int nCount = 0; nCount < PAUSE_MAX; nCount++)
	{
		if (g_pTexturePause[nCount] != NULL)
		{
			g_pTexturePause[nCount]->Release();
			g_pTexturePause[nCount] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//==================
//更新処理
//==================
void UpdatePause(void)
{
	VERTEX_2D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	//入力デバイス
	if (GetKeyboardTrigger(DIK_W))			//Wキーを押したら
	{
		g_nSerect++;
		if (g_nSerect >= PAUSE_MAX)
		{
			g_nSerect = PAUSE_CONTINUE;
		}
	}
	else if (GetKeyboardTrigger(DIK_S))		//Sキーをを押したら
	{
		g_nSerect--;
		if (g_nSerect < PAUSE_CONTINUE)
		{
			g_nSerect = PAUSE_QUIT;
		}
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		switch (g_nSerect)
		{
		case PAUSE_CONTINUE:

			SetEnablePause(false);
			break;

		case PAUSE_RETRY:

			SetFade(MODE_GAME);
			break;

		case PAUSE_QUIT:

			SetFade(MODE_TITLE);
			break;

		}
	}
	for (int nCount = 0; nCount < PAUSE_MAX; nCount++)
	{
		if (nCount == g_nSerect)
		{
			pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

		}
		else
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		pVtx += 4;
	}
	
	// 頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

}

//=================
//描画処理
//=================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;					// デバイスへのポインタ
	
	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCount = 0; nCount < PAUSE_MAX; nCount++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePause[nCount]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCount*4,
			2);
	}	
}