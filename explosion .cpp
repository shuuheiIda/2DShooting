//==========================
//
// シューティングゲームα版
// Author shuuhei Ida
//
//==========================

#include"main.h"
#include"explosion .h"
#include"camera.h"

// マクロ定義
#define MAX_EXPLOSION (128)				// 爆発の最大数

// 構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;						// 位置
	D3DXCOLOR   col;						// 移動量
	int nCounterAnim;						// 寿命
	int nPatternAnim;
	bool bUse; 								// 使用しているかどうか
}Explosion;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;	// テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	// 頂点バッファへのポインタ

Explosion g_aExplosion[MAX_EXPLOSION];						// 爆発の情報


//===================
// 爆発の初期化処理
//===================
void InitExplosion(void)
{
	int nCntExplosion;

	LPDIRECT3DDEVICE9 pDevice;								// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\explosion000.png", &g_pTextureExplosion);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		// 爆発の情報の初期化
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR  (0.0f, 0.0f, 0.0f,0.0f);
		g_aExplosion[nCntExplosion].bUse = false;

		
		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*4 * MAX_EXPLOSION,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffExplosion,
			NULL);
		

		VERTEX_2D* pVtx;		// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
		for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
		{
			// 頂点座標の設定(x,y,z,の順番になる、zの値は2Dの場合は必ず0にする)
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx += 4;
		}
			// 頂点バッファをアンロックする
			g_pVtxBuffExplosion->Unlock();
	
	}
	
}

//=================
// 爆発の終了処理
//=================
void UninitExplosion(void)
{
	// テクスチャの破棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();

		g_pVtxBuffExplosion = NULL;
	}

}

//=================
// 爆発の更新処理
//=================
void UpdateExplosion(void)
{
	int nCntExplosion;

	VERTEX_2D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{// 爆発が使用されている
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			// 爆発の位置の更新
			g_aExplosion[nCntExplosion].pos ;

			// カメラ座標
			D3DXVECTOR3 Camerapos = Screenposition(g_aExplosion[nCntExplosion].pos);

			// 頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(Camerapos.x - 20.0f, Camerapos.y - 20.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(Camerapos.x + 20.0f, Camerapos.y - 20.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(Camerapos.x - 20.0f, Camerapos.y + 20.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(Camerapos.x + 20.0f, Camerapos.y + 20.0f, 0.0f);

			g_aExplosion[nCntExplosion].nCounterAnim++;			// カウンターを加算
			if ((g_aExplosion[nCntExplosion].nCounterAnim % 10) == 0)
			{
				g_aExplosion[nCntExplosion].nCounterAnim = 0;	// カウンターを初期値に戻す
				g_aExplosion[nCntExplosion].nPatternAnim++;		// パターンNoを更新する.
				if (g_aExplosion[nCntExplosion].nPatternAnim > 8)
				{
					g_aExplosion[nCntExplosion].bUse = false;	// 使用しない状態にする
				}
			}
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 0);
			pVtx[1].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim +1) * 0.125f, 0);
			pVtx[2].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 1);
			pVtx[3].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim + 1) * 0.125f, 1);
		}
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//=================
// 爆発の描画処理
//=================
void DrawExplosion(void)
{
	int nCntExplosion;

	LPDIRECT3DDEVICE9 pDevice;					// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{// 爆発が使用されている
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntExplosion *4,
				2);
		}
	}
}

//=================
// 爆発の設定処理
//=================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int nCntExplosion;

	VERTEX_2D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{// 爆発が使用されてない
			g_aExplosion[nCntExplosion].pos = pos;
			pVtx[0].pos = D3DXVECTOR3(pos.x-20.0f,pos.y -20.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x+20.0f,pos.y -20.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x-20.0f,pos.y +20.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x+20.0f,pos.y +20.0f, 0.0f);

			g_aExplosion[nCntExplosion].nCounterAnim = 0;	// カウンターを初期値に戻す
			g_aExplosion[nCntExplosion].nPatternAnim =0;		// パターンNoを更新する

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 0);
			pVtx[1].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim + 1) * 0.125f, 0);
			pVtx[2].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 1);
			pVtx[3].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim + 1) * 0.125f, 1);

			g_aExplosion[nCntExplosion].col = col;
			g_aExplosion[nCntExplosion].bUse = true;			// 使用している状態にする
			break;
		}
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}
