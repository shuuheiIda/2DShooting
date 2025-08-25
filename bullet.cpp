//==========================
//
//弾処理 [bullet.cpp]
//Author shuuhei Ida
//
//==========================

// インクルードファイル
#include"main.h"
#include "bullet.h"
#include "explosion .h"
#include"enemy.h"
#include"player.h"
#include"effect.h"
#include"sound.h"
#include"camera.h"

// マクロ定義
#define MAX_BULLET (128)				//弾の最大数

// グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];						//弾の情報

//================
//弾の初期化処理
//================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;								//デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, 
		"data\\TEXTURE\\bullet000.png",
		&g_pTextureBullet);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// 弾の情報の初期化
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].bUse = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
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
	g_pVtxBuffBullet->Unlock();
}

//=================
//弾の終了処理
//=================
void UninitBullet(void)
{
	// テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();

		g_pVtxBuffBullet = NULL;
	}

}

//==============
//弾の更新処理
//==============
void UpdateBullet(void)
{
	VERTEX_2D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{// 弾が使用されている

		if (g_aBullet[nCntBullet].bUse == true)
		{

			Enemy* pEnemy;
			// 敵の取得
			pEnemy = GetEnemy();

			
			// 弾の当たり判定
			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{
				CollisionEnemy(&g_aBullet[nCntBullet]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
			{
				CollisionPlayer(&g_aBullet[nCntBullet]);
			}
			
			//// 弾の位置が画面外に出た
			//if (g_aBullet[nCntBullet].pos.x > SCREEN_WIDTH)
			//{
			//	g_aBullet[nCntBullet].bUse = false;			// 使用していない状態にする
			//}
			g_aBullet[nCntBullet].nLife--;
			// 寿命のカウントダウン
			if (g_aBullet[nCntBullet].nLife <= 0)
			{
				 SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
				g_aBullet[nCntBullet].bUse = false;			// 使用していない状態にする

				//サウンドの再生
				PlaySound(SOUND_LABEL_SE_EXPLOSION);
			}

			// 弾の位置の更新
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			// カメラ座標
			D3DXVECTOR3 CameraPos = Screenposition(g_aBullet[nCntBullet].pos);

			SetEffect(CameraPos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 30, g_aBullet[nCntBullet].nLife);

			// 頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(CameraPos.x - 20.0f,
				CameraPos.y - 20.0f, 0.0f);

			pVtx[1].pos = D3DXVECTOR3(CameraPos.x + 20.0f,
				CameraPos.y - 20.0f, 0.0f);

			pVtx[2].pos = D3DXVECTOR3(CameraPos.x - 20.0f,
				CameraPos.y + 20.0f, 0.0f);

			pVtx[3].pos = D3DXVECTOR3(CameraPos.x + 20.0f,
				CameraPos.y + 20.0f, 0.0f);

		}
		pVtx += 4;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//==============
//弾の描画処理
//==============
void DrawBullet(void)
{
	int nCntBullet;

	LPDIRECT3DDEVICE9 pDevice;					// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBullet);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{// 弾が使用されている
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntBullet * 4,
				2);
		}
	}
}

//===============
// 弾の設定処理
//===============
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move,int nLife, BULLETTYPE type)
{
	VERTEX_2D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{// 弾が使用されてない
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].nLife = nLife;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].bUse = true;			// 使用している状態にする

			// カメラ座標
			D3DXVECTOR3 CameraPos = Screenposition(g_aBullet[nCntBullet].pos);

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(CameraPos.x - 20.0f, CameraPos.y - 20.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(CameraPos.x + 20.0f, CameraPos.y - 20.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(CameraPos.x - 20.0f, CameraPos.y + 20.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(CameraPos.x + 20.0f, CameraPos.y + 20.0f, 0.0f);

			break;
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

void CollisionEnemy(Bullet* pBullet)
{
	Enemy* pEnemy = GetEnemy();							// 敵の取得

	int nCntBullet = 0;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{// 敵が使用されている
			if (pEnemy->pos.x - 35.0f <= pBullet->pos.x &&
				pEnemy->pos.x + 35.0f >= pBullet->pos.x &&
				pEnemy->pos.y - 35.0f <= pBullet->pos.y &&
				pEnemy->pos.y + 35.0f >= pBullet->pos.y)
			{
				HitEnemy(nCntEnemy, 1);
				pBullet->bUse = false;
			}
		}
	}
}

void CollisionPlayer(Bullet* pBullet)
{
	Player* pPlayer = GetPlayer();							// プレイヤーの取得

	int nCntBullet = 0;

		if (pPlayer->pos.x - 35 <= pBullet->pos.x &&
			pPlayer->pos.x + 35 >= pBullet->pos.x &&
			pPlayer->pos.y - 35 <= pBullet->pos.y &&
			pPlayer->pos.y + 35 >= pBullet->pos.y )
		{

			if (pPlayer->state == PLAYERTYPE_NORMAL)
			{
				HitPlayer(1);
				pBullet->bUse = false;
			}
			else if (pPlayer->state == PLAYERTYPE_UNRIVALED)
			{
				pBullet->bUse = false;
			}
			else if(pPlayer->state == PLAYERTYPE_SUCTION)
			{
				PowerUpPlayer();
				pBullet->bUse = false;
			}
		}
	
	
}

Bullet* GetBullet(void)
{
	return &g_aBullet[0];
}
