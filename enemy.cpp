//==========================
//
// 敵処理 [enemy.cpp]
// Author shuuhei Ida
//
//==========================

#include"main.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion .h"
#include"score.h"
#include"sound.h"
#include"camera.h"


// マクロ定義
#define NUM_ENEMY (4)				// 敵の種類
#define ENEMY_SIZE (20.0f)			//　敵のサイズ

// グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureenemy[NUM_ENEMY] = {};	// テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffenemy = NULL;	// 頂点バッファへのポインタ

Enemy g_aEnemy[MAX_ENEMY];						// 敵の情報

//================
// 敵の初期化処理
//================
void InitEnemy(void)
{
	int nCntEnemy;

	LPDIRECT3DDEVICE9 pDevice;								// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\4.jpg", &g_apTextureenemy[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\2.jpg", &g_apTextureenemy[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\enemy002.png", &g_apTextureenemy[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\enemy003.png", &g_apTextureenemy[3]);


	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// 敵の情報の初期化
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nType = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nLife = 5;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].nCnterState = 0;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].nScore = 0;

	}
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffenemy,
		NULL);
		
	VERTEX_2D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffenemy->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
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
	g_pVtxBuffenemy->Unlock();
		
}

//=================
// 敵の終了処理
//=================
void UninitEnemy(void)
{
	int nCount;
	// テクスチャの破棄
	for (nCount = 0; nCount < NUM_ENEMY; nCount++)
	{
		if (g_apTextureenemy[nCount] != NULL)
		{
			g_apTextureenemy[nCount]->Release();
			g_apTextureenemy[nCount] = NULL;
		}
		
	}
		// 頂点バッファの破棄
		if (g_pVtxBuffenemy != NULL)
		{
			g_pVtxBuffenemy->Release();

			g_pVtxBuffenemy = NULL;
		}
}

//==============
// 敵の更新処理
//==============
void UpdateEnemy()
{
	VERTEX_2D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffenemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// 敵が使用されていない
		if (!g_aEnemy[nCntEnemy].bUse)
		{
			pVtx += 4;
			continue;
		}

		// カメラ座標
		D3DXVECTOR3 Camerapos = Screenposition(g_aEnemy[nCntEnemy].pos);

		switch (g_aEnemy[nCntEnemy].state)
		{
		case ENEMYSTATE_NORMAL:
			g_aEnemy[nCntEnemy].nCnterState--;

			if (g_aEnemy[nCntEnemy].nCnterState <= 0)
			{
				// 敵の設定
				SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(-2.5f, 0.0f, 0.0f), 150, BULLETTYPE_ENEMY);
				g_aEnemy[nCntEnemy].nCnterState = 120;
			}
			break;
			
		case ENEMYSTATE_DAMAGE:
			g_aEnemy[nCntEnemy].nCnterState--;

			if (g_aEnemy[nCntEnemy].nCnterState <= 0)
			{
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
			}
			break;
		}

		// g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

		// 頂点座標の設定(x,y,z,の順番になる、zの値は2Dの場合は必ず0にする)
		pVtx[0].pos = D3DXVECTOR3(Camerapos.x - ENEMY_SIZE, Camerapos.y - ENEMY_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(Camerapos.x + ENEMY_SIZE, Camerapos.y - ENEMY_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(Camerapos.x - ENEMY_SIZE, Camerapos.y + ENEMY_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(Camerapos.x + ENEMY_SIZE, Camerapos.y + ENEMY_SIZE, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffenemy->Unlock();

}

//=============
//敵の描画処理
//=============
void DrawEnemy(void)
{
	int nCntEnemy;

	LPDIRECT3DDEVICE9 pDevice;					// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffenemy, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{// 敵が使用されている
				// テクスチャの設定
			pDevice->SetTexture(0,g_apTextureenemy[g_aEnemy[nCntEnemy].nType]);
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntEnemy *4,
				2);
		}
	}
}

//===============
// 敵の設定処理
//===============
void SetEnemy(D3DXVECTOR3 pos, int nType)
{
	int nCntEnemy;

	VERTEX_2D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffenemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{// 爆発が使用されてない
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].nCnterState = 0;
			g_aEnemy[nCntEnemy].nCnterState = ENEMYSTATE_NORMAL;
			g_aEnemy[nCntEnemy].nType = nType;
			g_aEnemy[nCntEnemy].nScore = 100;
			g_aEnemy[nCntEnemy].bUse = true;			// 使用している状態にする

			pVtx[0].pos = D3DXVECTOR3(pos.x - ENEMY_SIZE,pos.y - ENEMY_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + ENEMY_SIZE,pos.y - ENEMY_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - ENEMY_SIZE,pos.y + ENEMY_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + ENEMY_SIZE,pos.y + ENEMY_SIZE, 0.0f);

			break;
		}
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffenemy->Unlock();
}

//=============
//敵の被弾処理
//=============
void HitEnemy(int nCntEnemy, int nDamage)
{
	VERTEX_2D* pVtx;		// 頂点情報へのポインタ
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffenemy->Lock(0, 0, (void**)&pVtx, 0);
	g_aEnemy[nCntEnemy].nLife -= nDamage;
	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		// 爆発の設定
		SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
		g_aEnemy[nCntEnemy].bUse = false;
		AddScore(100);
	}
	else
	{
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCnterState = 5;
		pVtx += (nCntEnemy*4);
		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		// サウンドの再生
		PlaySound(SOUND_LABEL_SE_HIT);

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffenemy->Unlock();
}

//============
//敵の取得
//============
Enemy*GetEnemy(void)
{
	return &g_aEnemy[0];	// 敵の情報の先頭アドレスを返す
}

//===============
//敵のカウント
//===============
int Enemycount(void)
{
	int nCntEnemy;					// 128体分のデータ
	int nNoDeadEnemy = 0;			// 生きている個体の数
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			nNoDeadEnemy++;
		}

	}
	return nNoDeadEnemy;

}
