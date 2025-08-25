//==========================
//
// 時間処理 [Time.cpp]
// Author shuuhei Ida
//
//==========================

#include"main.h"
#include "time.h"
#include"enemy.h"
#include"fade.h"

// マクロ定義
#define NUM_PLACE	(3)						// スコアの桁数

// グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;	// テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;	// 頂点バッファへのポインタ

D3DXVECTOR3 g_posTime;						// スコアの位置

int g_nTime;								// スコアの値

//======================
// タイマーの初期化処理
//======================
void InitTime(void)
{
	int nCntExplosion;

	LPDIRECT3DDEVICE9 pDevice;								// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureTime);
	g_posTime = D3DXVECTOR3(550.0f,0.0f,0.0f);
	g_nTime = 18000;

	for (nCntExplosion = 0; nCntExplosion < NUM_PLACE; nCntExplosion++)
	{		
		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*4 * NUM_PLACE,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffTime,
			NULL);
		
		VERTEX_2D* pVtx;		// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);
		for (nCntExplosion = 0; nCntExplosion < NUM_PLACE; nCntExplosion++)
		{
			// 頂点座標の設定(x,y,z,の順番になる、zの値は2Dの場合は必ず0にする)
			pVtx[0].pos = D3DXVECTOR3(g_posTime.x+(nCntExplosion*30), g_posTime.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posTime.x+(nCntExplosion+1)*30, g_posTime.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posTime.x+nCntExplosion*30, g_posTime.y+45, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posTime.x+(nCntExplosion+1)*30, g_posTime.y+45, 0.0f);

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
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;
		}
			// 頂点バッファをアンロックする
			g_pVtxBuffTime->Unlock();
	
	}
	
}

//====================
// タイマーの終了処理
//====================
void UninitTime(void)
{
	// テクスチャの破棄
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();

		g_pVtxBuffTime = NULL;
	}
}

//====================
// タイマーの更新処理
//====================
void UpdateTime(void)
{
	static int nFreamCounter = 0;

	if ((nFreamCounter % 60) == 0)
	{
		nFreamCounter = 0;
		g_nTime--;
	}

	int nCntTime;

	int aTexU[NUM_PLACE];

	aTexU[0] = g_nTime % 100000 / 10000;

	aTexU[1] = g_nTime % 10000 / 1000;

	aTexU[2] = g_nTime % 1000 / 100;

	VERTEX_2D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1, 0.0f);

		pVtx[1].tex = D3DXVECTOR2((aTexU[nCntTime] + 1) * 0.1, 0.0f);

		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1, 1.0f);

		pVtx[3].tex = D3DXVECTOR2((aTexU[nCntTime] + 1) * 0.1, 1.0f);

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();


	if (g_nTime <= 0)
	{
		SetFade(MODE_RESULT);
	}
}

//====================
// タイマーの描画処理
//====================
void DrawTime(void)
{
	int nCntTime;

	LPDIRECT3DDEVICE9 pDevice;					// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTime);

	for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntTime * 4,
				2);
	}
}
