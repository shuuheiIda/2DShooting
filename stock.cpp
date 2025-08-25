//==========================
//
// ストック処理 [stock.cpp]
// Author shuuhei Ida
//
//==========================

#include"main.h"
#include "stock.h"
#include"enemy.h"
#include"player.h"
#include"bullet.h"

// マクロ定義
#define NUM_STOCK	(3)						// ストックの桁数

// グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturestock = NULL;	// テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffstock = NULL;	// 頂点バッファへのポインタ

D3DXVECTOR3 g_posstock;						// ストックの位置

int g_nstock;								// ストックの値

//====================
// ストックの初期化処理
//====================
void InitStock(void)
{
	int nCntExplosion;

	LPDIRECT3DDEVICE9 pDevice;								// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\player000.png", &g_pTexturestock);
	g_posstock = D3DXVECTOR3(20.0f,0.0f,0.0f);
	g_nstock = 0;

	for (nCntExplosion = 0; nCntExplosion < NUM_STOCK; nCntExplosion++)
	{		
		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*4 * NUM_STOCK,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffstock,
			NULL);
		
		VERTEX_2D* pVtx;		// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffstock->Lock(0, 0, (void**)&pVtx, 0);
		for (nCntExplosion = 0; nCntExplosion < NUM_STOCK; nCntExplosion++)
		{
			// 頂点座標の設定(x,y,z,の順番になる、zの値は2Dの場合は必ず0にする)
			pVtx[0].pos = D3DXVECTOR3(g_posstock.x+(nCntExplosion*30), g_posstock.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posstock.x+(nCntExplosion+1)*30, g_posstock.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posstock.x+nCntExplosion*30, g_posstock.y+45, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posstock.x+(nCntExplosion+1)*30, g_posstock.y+45, 0.0f);

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;
		}
			// 頂点バッファをアンロックする
			g_pVtxBuffstock->Unlock();
	
	}
	
}

//=================
//ストックの終了処理
//=================
void UninitStock(void)
{
	// テクスチャの破棄
	if (g_pTexturestock != NULL)
	{
		g_pTexturestock->Release();
		g_pTexturestock = NULL;
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffstock != NULL)
	{
		g_pVtxBuffstock->Release();

		g_pVtxBuffstock = NULL;
	}
}

//==================
// ストックの更新処理
//==================
void UpdateStock(void)
{
	Player* pPlayer = GetPlayer();
	Bullet* pBullet = GetBullet();

	if (pPlayer->pos.x - 35 <= pBullet->pos.x &&
		pPlayer->pos.x + 35 >= pBullet->pos.x &&
		pPlayer->pos.y - 35 <= pBullet->pos.y &&
		pPlayer->pos.y + 35 >= pBullet->pos.y)
	{
		pPlayer->nStock--;
	}
}

//===================
// ストックの描画処理
//===================
void DrawStock(void)
{

	Player* pPlayer = GetPlayer();

	int nCntstock;

	LPDIRECT3DDEVICE9 pDevice;					// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffstock, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturestock);

	for (nCntstock = 0; nCntstock < NUM_STOCK; nCntstock++)
	{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntstock * 4,
				2);
	}

}

