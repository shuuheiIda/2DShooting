//==========================
//
// スコア処理 [score.cpp]
// Author shuuhei Ida
//
//==========================

#include"main.h"
#include "score.h"
#include"enemy.h"

// マクロ定義
#define NUM_PLACE	(8)						// スコアの桁数

// グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;	// テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;	// 頂点バッファへのポインタ

D3DXVECTOR3 g_posScore;						// スコアの位置

int g_nScore;								// スコアの値

//====================
// スコアの初期化処理
//====================
void InitScore(void)
{
	int nCntExplosion;

	LPDIRECT3DDEVICE9 pDevice;								// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureScore);
	g_posScore = D3DXVECTOR3(1020.0f,0.0f,0.0f);
	g_nScore = 0;

	for (nCntExplosion = 0; nCntExplosion < NUM_PLACE; nCntExplosion++)
	{		
		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*4 * NUM_PLACE,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffScore,
			NULL);
		
		VERTEX_2D* pVtx;		// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
		for (nCntExplosion = 0; nCntExplosion < NUM_PLACE; nCntExplosion++)
		{
			// 頂点座標の設定(x,y,z,の順番になる、zの値は2Dの場合は必ず0にする)
			pVtx[0].pos = D3DXVECTOR3(g_posScore.x+(nCntExplosion*30), g_posScore.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posScore.x+(nCntExplosion+1)*30, g_posScore.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posScore.x+nCntExplosion*30, g_posScore.y+45, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posScore.x+(nCntExplosion+1)*30, g_posScore.y+45, 0.0f);

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
			g_pVtxBuffScore->Unlock();
	
	}
	
}

//=================
//スコアの終了処理
//=================
void UninitScore(void)
{
	// テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();

		g_pVtxBuffScore = NULL;
	}
}

//==================
// スコアの更新処理
//==================
void UpdateScore(void)
{
}

//===================
// スコアの描画処理
//===================
void DrawScore(void)
{
	int nCntScore;

	LPDIRECT3DDEVICE9 pDevice;					// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntScore * 4,
				2);
	}
}

//===================
// スコアの設定処理
//===================
void SetScore(int nScore)
{
	int nCntScore;

	int aTexU[NUM_PLACE];

	g_nScore = nScore;

	aTexU[0] = g_nScore % 100000000 / 10000000;

	aTexU[1] = g_nScore % 10000000 / 1000000;

	aTexU[2] = g_nScore % 1000000 / 100000;

	aTexU[3] = g_nScore % 100000 / 10000;

	aTexU[4] = g_nScore % 10000 / 1000;

	aTexU[5] = g_nScore % 1000 / 100;

	aTexU[6] = g_nScore % 100 / 10;

	aTexU[7] = g_nScore % 10 / 1;

	VERTEX_2D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
			// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1, 0.0f);

			pVtx[1].tex = D3DXVECTOR2((aTexU[nCntScore]+1) * 0.1, 0.0f);

			pVtx[2].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1, 1.0f);

			pVtx[3].tex = D3DXVECTOR2((aTexU[nCntScore]+1) * 0.1, 1.0f);

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//===================
// スコアの加算処理
//===================
void AddScore(int nValue)
{
	int nCntScore;

	int aTexU[NUM_PLACE];

	g_nScore += nValue;

	aTexU[0] = g_nScore % 100000000 / 10000000;

	aTexU[1] = g_nScore % 10000000 / 1000000;

	aTexU[2] = g_nScore % 1000000 / 100000;

	aTexU[3] = g_nScore % 100000 / 10000;

	aTexU[4] = g_nScore % 10000 / 1000;

	aTexU[5] = g_nScore % 1000 / 100;

	aTexU[6] = g_nScore % 100 / 10;

	aTexU[7] = g_nScore % 10 / 1;

	VERTEX_2D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1, 0.0f);

		pVtx[1].tex = D3DXVECTOR2((aTexU[nCntScore] + 1) * 0.1, 0.0f);

		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1, 1.0f);

		pVtx[3].tex = D3DXVECTOR2((aTexU[nCntScore] + 1) * 0.1, 1.0f);

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}