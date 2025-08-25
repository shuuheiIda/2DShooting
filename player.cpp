//============================
//
// プレイヤー処理 [player.cpp]
// Author shuuhei Ida
//
//============================
#include"main.h"
#include"player.h"
#include"input.h"
#include"bullet.h"
#include "explosion .h"
#include"sound.h"
#include"enemy.h"
#include"camera.h"
#include"fade.h"

//マクロ定義
#define MAX_SPEED	(5)										//最大移動速度
#define BULLET_LIFE (150.0f)								//弾の寿命
#define BULLET_LINE (2.5f)									//弾の発射方向

// グローバル変数

LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;					// テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;			// 頂点バッファへのポインタ

D3DXVECTOR3 g_rotPlayer;									// 向き

Player g_Player;

int g_nCounterAnimPlayer;									// アニメーションカウンター
int g_nPatternAnimPlayer;									// アニメーションパターンNo
int g_nBulletlvel;											// 弾のレベル
int g_nSpeedlvel;											// スピードのレベル
float g_fLengthPlayer;										// 対角線の長さ
float g_fAnglePlayer;										// 対角線の角度


//========================
//プレイヤーの初期化処理
//========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;								// デバイスへのポインタ

	g_Player.pos = D3DXVECTOR3(640.0f,350.0f,0.0f);			// 位置を初期化する

	g_Player.move = D3DXVECTOR3(0.0f,0.0f,0.0f);			// 移動量を初期化

	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//

	g_nCounterAnimPlayer = 0;								// カウンターを初期化する

	g_nPatternAnimPlayer = 0;								// パターンNo.を初期化する

	g_rotPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向きを初期化する(ｚ値を使用)

	g_Player.nLife = 1 ;									// ライフの初期化

	g_Player.nSpeed = 1;									// 速度の初期化

	g_Player.bDisp = true;
	
	g_Player.nPowerupCountor = 0;

	g_Player.nSpeedupCounter = 0;

	g_nBulletlvel = 0;

	g_Player.nStock = 3;									//残機の初期化

	g_Player.posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Player.nUnrivaledCnter = 0;

	// 対角線の長さを算出する
	g_fLengthPlayer = sqrtf(100 * 100 + 100 * 100) * 0.5f;

	// 対角線の角度を算出する
	g_fAnglePlayer = atan2f(100, 100);

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\player000.png", &g_pTexturePlayer);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0,(void**)&pVtx, 0);

	// 頂点座標の設定(x,y,z,の順番になる、zの値は2Dの場合は必ず0にする)
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_Player.pos.y + cosf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x =g_Player.pos.x - sinf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = g_Player.pos.y - cosf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x =g_Player.pos.x + sinf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x =g_Player.pos.x + sinf(g_rotPlayer.z + g_fAnglePlayer) *  g_fLengthPlayer;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_rotPlayer.z + g_fAnglePlayer) *g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//==========================
// プレイヤーの終了処理
//==========================
void UninitPlayer(void)
{
	// テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();

		g_pVtxBuffPlayer = NULL;
	}

}

//========================
// プレイヤーの更新処理
//========================
void UpdatePlayer(void)
{

	VERTEX_2D* pVtx;		// 頂点情報へのポインタ

	int nCntPlayer = 0;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_Player.state)
	{
	case PLAYERTYPE_NORMAL:

			break;

	case PLAYERTYPE_DAMAGE:
	
		g_Player.nCnterState--;
		if (g_Player.nCnterState <= 0)
		{
			g_Player.state = PLAYERTYPE_NORMAL;
		}
		break;
		
	case PLAYERTYPE_DEATH:

		g_Player.nStock--;
		if (g_Player.nStock <= 0)
		{
			SetFade(MODE_RESULT);
		}

		if (g_Player.nLife <= 0)
		{
			g_Player.bDisp = false;
			g_Player.state = PLAYERTYPE_WAIT;
		}
		break;
	
	case PLAYERTYPE_WAIT:
	
		g_Player.nCnterState--;
		if (g_Player.nCnterState <= 0)
		{
			g_Player.state = PLAYERTYPE_APPEEAR;
		}
		break;
	
	case PLAYERTYPE_APPEEAR:
	
		g_Player.nCnterState--;
		if (g_Player.nCnterState <= 0)
		{
			g_Player.bDisp = true;
			g_Player.nLife = 1;
			g_Player.state = PLAYERTYPE_UNRIVALED;
			g_Player.nUnrivaledCnter = 300;
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
			

		}
		break;

	case PLAYERTYPE_UNRIVALED:
		
		g_Player.nUnrivaledCnter--;
		if (g_Player.nUnrivaledCnter >= 0)
		{
			g_Player.bDisp = g_Player.bDisp ^ 1;
		}
		if(g_Player.nUnrivaledCnter <=0)
		{
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_Player.state = PLAYERTYPE_NORMAL;
		}
		break;

	default:
		break;
	}

	if (GetKeyboardTrigger(DIK_B) == true || GetJoypadTrigger(JOYKEY_Y) == true)		//パワーアップの方法
	{
		g_Player.state = PLAYERTYPE_SUCTION;
	}
	else if (GetKeyboardTrigger(DIK_N) == true  || GetJoypadTrigger(JOYKEY_X) == true)
	{
		g_Player.state = PLAYERTYPE_NORMAL;
	}

	// 位置を更新
	if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
	{
		if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
		{// 左上移動
			g_Player.move.x += sinf(D3DX_PI * 0.75f)*-g_Player.nSpeed ;
			g_Player.move.y += cosf(D3DX_PI * 0.75f)* g_Player.nSpeed ;
			// 移動量を更新
		}
		else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
		{// 左下移動
			g_Player.move.x += sinf(D3DX_PI * 0.25f)* -g_Player.nSpeed ;
			g_Player.move.y += cosf(D3DX_PI * 0.25f) * g_Player.nSpeed ;
		}
		else
		{// Aキーが押された
			g_Player.move.x += sinf(D3DX_PI * 0.5f) * -g_Player.nSpeed ;
			// 移動量を更新
			//g_Player.move.x += -1;
				// 頂点座標を更新
		}
	
	}
	
	else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
	{
			if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
			{// 右上移動
				g_Player.move.x += sinf(D3DX_PI * 0.75f)*g_Player.nSpeed ;
				g_Player.move.y += cosf(D3DX_PI * 0.75f)*g_Player.nSpeed ;
			}
			else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
			{// 右下移動
				g_Player.move.x += sinf(D3DX_PI * 0.25f) * g_Player.nSpeed ;
				g_Player.move.y += cosf(D3DX_PI * 0.25f)* g_Player.nSpeed ;
			}
			else
			{// Dキーが押された
				g_Player.move.x += sinf(D3DX_PI * 0.5f) * g_Player.nSpeed ;				//頂点座標を更新
			}
	}
	
	else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
	{// Wキーが押された
		
		if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
		{
			// Aキーが押された
				g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -g_Player.nSpeed ;
				// 移動量を更新
				// g_Player.move.x += -1;
				// 頂点座標を更新
			
		}
		else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
		{// 右下移動
			g_Player.move.x += sinf(D3DX_PI * 0.25f) * g_Player.nSpeed ;
			g_Player.move.y += cosf(D3DX_PI * 0.25f) * -g_Player.nSpeed ;
		}
		else
		{
			g_Player.move.y += cosf(D3DX_PI * 1.0f) * g_Player.nSpeed ;		//頂点座標を更新
		}
	}

	else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
	{// Sキーが押された
		if (GetKeyboardPress(DIK_A) == true)
		{// Aキーが押された
			g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -g_Player.nSpeed ;
			// 移動量を更新
		
		}
		else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
		{// 右下移動
			g_Player.move.x += sinf(D3DX_PI * 0.25f) * g_Player.nSpeed ;
			g_Player.move.y += cosf(D3DX_PI * 0.25f) * g_Player.nSpeed ;
		}
		else
		{
			g_Player.move.y += cosf(-D3DX_PI * 1.0f) * -g_Player.nSpeed ;					//頂点座標を更新
		}
		
	}

	if (g_Player.state != PLAYERTYPE_DEATH || g_Player.state != PLAYERTYPE_APPEEAR || g_Player.state != PLAYERTYPE_WAIT || g_Player.state != PLAYERTYPE_UNRIVALED)
	{
		if (GetKeyboardTrigger(DIK_SPACE) == true || GetJoypadTrigger(JOYKEY_A) == true)
		{// スペースキーが押された

			g_Player.state = PLAYERTYPE_NORMAL;
			// 現在のレベルに応じて弾の出す数を変更する
			switch (g_nBulletlvel)
			{
			case PLAYERBULLET_NORMAL:
				// 弾の設定処理
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 0.0f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				// サウンドの再生
				PlaySound(SOUND_LABEL_SE_SHOT);
				break;

			case PLAYERBULLET_LEVEL1:
				// 弾の設定処理
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 0.0f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 2.5f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				// サウンドの再生
				PlaySound(SOUND_LABEL_SE_SHOT);
				break;

			case PLAYERBULLET_LEVEL2:
				// 弾の設定処理
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 0.0f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 2.5f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, -2.5f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				// サウンドの再生
				PlaySound(SOUND_LABEL_SE_SHOT);
				break;

			case PLAYERBULLET_LEVEL3:
				// 弾の設定処理
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 0.0f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 2.5f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, -2.5f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 5.0f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				// サウンドの再生
				PlaySound(SOUND_LABEL_SE_SHOT);
				break;

			case PLAYERBULLET_LEVEL4:
				// 弾の設定処理
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 0.0f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 2.5f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, -2.5f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 5.0f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, -5.0f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				// サウンドの再生
				PlaySound(SOUND_LABEL_SE_SHOT);
				break;

			}
		}
	}
	

	g_Player.posold.x = g_Player.pos.x;
	g_Player.posold.y = g_Player.pos.y;

	// 位置を更新
	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.y += g_Player.move.y;


	// 移動量を更新
	g_Player.move.x += (0.0f - g_Player.move.x) * 0.1f;
	g_Player.move.y += (0.0f - g_Player.move.y) * 0.1f;

	// カメラ座標
	D3DXVECTOR3 CameraPos = Screenposition(g_Player.pos);

	// 移動制限
	
	if (g_Player.pos.x >= 2560.0f)
	{
		g_Player.pos.x = 2560.0f;
	}
	else if (g_Player.pos.x <=30.0f)
	{
		g_Player.pos.x = 30.0f;
	}

	if (g_Player.pos.y <= 50)
	{
		g_Player.pos.y = 50;
	}
	else if (g_Player.pos.y >= 670)
	{
		g_Player.pos.y = 670;
	}
	//else if (g_Player.pos.y >= SCREEN_HEIGHT)
	//{
	//	g_Player.pos.y = 0;
	//}
	//else if (g_Player.pos.y <= 0)
	//{
	//	g_Player.pos.y = SCREEN_HEIGHT;
	//}

	// 頂点座標の設定(x,y,z,の順番になる、zの値は2Dの場合は必ず0にする)
	pVtx[0].pos.x = CameraPos.x + sinf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = CameraPos.y + cosf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = CameraPos.x + sinf(g_rotPlayer.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = CameraPos.y + cosf(g_rotPlayer.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;


	pVtx[2].pos.x =CameraPos.x + sinf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = CameraPos.y + cosf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x =CameraPos.x + sinf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = CameraPos.y + cosf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

	SpeedUpPrlayer();
}

//=======================
// プレイヤーの描画処理
//=======================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;					// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer);

	if (g_Player.bDisp != false)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
	}
	
}

//========================
// プレイヤーの当たり判定
//========================
void HitPlayer(int nDamage)
{
	VERTEX_2D* pVtx;		// 頂点情報へのポインタ
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);
	g_Player.nLife -= nDamage;
	if (g_Player.nLife <= 0)
	{
		// 爆発の設定
		SetExplosion(g_Player.pos,D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
		g_Player.nCnterState = 120;
		g_Player.state = PLAYERTYPE_DEATH;
		g_Player.bDisp = false;

	}
	else
	{
		g_Player.state = PLAYERTYPE_DAMAGE;
		g_Player.nCnterState = 5;
		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		SetVibration(50000, 50000, 100);

		// サウンドの再生
		PlaySound(SOUND_LABEL_SE_HIT);

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//====================
// スピードアップ処理
//====================
void SpeedUpPrlayer(void)
{
#if 0
	Bullet* pBullet = GetBullet();

	for (int nCnt = 0; nCnt < 128; nCnt++)
	{
		if (g_Player.posold.x - 35.0f > pBullet[nCnt].pos.x ||
			g_Player.posold.x + 35.0f < pBullet[nCnt].pos.x ||
			g_Player.posold.y - 35.0f > pBullet[nCnt].pos.y ||
			g_Player.posold.y + 35.0f < pBullet[nCnt].pos.y)
		{
			if (g_Player.nSpeed <= PLAYERSPEED_LEVEL4)
			{
				g_Player.nSpeedupCounter++;
				if (g_Player.nSpeedupCounter >= 10)
				{
					if (g_nSpeedlvel == PLAYERSPEED_NORMAL)
					{
						g_nSpeedlvel = PLAYERSPEED_LEVEL1;

						if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
						{
							if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
							{// 左上移動
								g_Player.move.x += sinf(D3DX_PI * 0.75f) * -1.0f;
								g_Player.move.y += cosf(D3DX_PI * 0.75f) * 1.0f;
								// 移動量を更新
							}
							else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
							{// 左下移動
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * -1.0f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.0f;
							}
							else
							{// Aキーが押された
								g_Player.move.x += sinf(D3DX_PI * 0.5f) * -1.0f;
								// 移動量を更新
								g_Player.move.x += -1;
								// 頂点座標を更新
							}

						}

						else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
						{
							if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
							{// 右上移動
								g_Player.move.x += sinf(D3DX_PI * 0.75f) * 1.0f;
								g_Player.move.y += cosf(D3DX_PI * 0.75f) * 1.0f;
							}
							else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
							{// 右下移動
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.0f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.0f;
							}
							else
							{// Dキーが押された
								g_Player.move.x += sinf(D3DX_PI * 0.5f) * 1.0f;				//頂点座標を更新
							}
						}

						else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
						{// Wキーが押された

							if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
							{
								// Aキーが押された
								g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -1.0f;
								// 移動量を更新
								g_Player.move.x += -1;
								// 頂点座標を更新

							}
							else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
							{// 右下移動
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.0f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * -1.0f;
							}
							else
							{
								g_Player.move.y += cosf(D3DX_PI * 1.0f) * 1.0f;		//頂点座標を更新
							}
						}

						else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
						{// Sキーが押された
							if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
							{// Aキーが押された
								g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -1.0f;
								// 移動量を更新

							}
							else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
							{// 右下移動
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.0f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.0f;
							}
							else
							{
								g_Player.move.y += cosf(-D3DX_PI * 1.0f) * -1.0f;					//頂点座標を更新
							}

						}

						/*g_nBulletlvel -= 10;
						if (g_nBulletlvel == PLAYERBULLET_NORMAL)
						{
							g_nBulletlvel = PLAYERBULLET_NORMAL;
						}*/
						g_nSpeedlvel = 0;
					}

					if (g_nSpeedlvel == PLAYERSPEED_LEVEL1)
					{
						g_nSpeedlvel = PLAYERSPEED_LEVEL2;

						if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
						{
							if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
							{// 左上移動
								g_Player.move.x += sinf(D3DX_PI * 0.75f) * -1.25f;
								g_Player.move.y += cosf(D3DX_PI * 0.75f) * 1.25f;
								// 移動量を更新
							}
							else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
							{// 左下移動
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * -1.25f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.25f;
							}
							else
							{// Aキーが押された
								g_Player.move.x += sinf(D3DX_PI * 0.5f) * -1.25f;
								// 移動量を更新
								g_Player.move.x += -1;
								// 頂点座標を更新
							}

						}

						else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
						{
							if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
							{// 右上移動
								g_Player.move.x += sinf(D3DX_PI * 0.75f) * 1.25f;
								g_Player.move.y += cosf(D3DX_PI * 0.75f) * 1.25f;
							}
							else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
							{// 右下移動
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.25f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.25f;
							}
							else
							{// Dキーが押された
								g_Player.move.x += sinf(D3DX_PI * 0.5f) * 1.25f;				//頂点座標を更新
							}
						}

						else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
						{// Wキーが押された

							if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
							{
								// Aキーが押された
								g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -1.25f;
								// 移動量を更新
								g_Player.move.x += -1;
								// 頂点座標を更新

							}
							else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
							{// 右下移動
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.25f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * -1.25f;
							}
							else
							{
								g_Player.move.y += cosf(D3DX_PI * 1.0f) * 1.25f;		//頂点座標を更新
							}
						}

						else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
						{// Sキーが押された
							if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
							{// Aキーが押された
								g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -1.25f;
								// 移動量を更新

							}
							else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
							{// 右下移動
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.25f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.25f;
							}
							else
							{
								g_Player.move.y += cosf(-D3DX_PI * 1.0f) * -1.25f;					//頂点座標を更新
							}

						}

						/*	g_nBulletlvel -= 10;
							if (g_nBulletlvel == PLAYERBULLET_NORMAL)
							{
								g_nBulletlvel = PLAYERBULLET_NORMAL;
							}*/
						g_nSpeedlvel = 0;
					}

					if (g_nSpeedlvel == PLAYERSPEED_LEVEL2)
					{
						g_nSpeedlvel = PLAYERSPEED_LEVEL3;

						if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
						{
							if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
							{// 左上移動
								g_Player.move.x += sinf(D3DX_PI * 0.75f) * -1.5f;
								g_Player.move.y += cosf(D3DX_PI * 0.75f) * 1.5f;
								// 移動量を更新
							}
							else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
							{// 左下移動
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * -1.5f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.5f;
							}
							else
							{// Aキーが押された
								g_Player.move.x += sinf(D3DX_PI * 0.5f) * -1.5f;
								// 移動量を更新
								g_Player.move.x += -1;
								// 頂点座標を更新
							}

						}

						else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
						{
							if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
							{// 右上移動
								g_Player.move.x += sinf(D3DX_PI * 0.75f) * 1.5f;
								g_Player.move.y += cosf(D3DX_PI * 0.75f) * 1.5f;
							}
							else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
							{// 右下移動
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.5f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.5f;
							}
							else
							{// Dキーが押された
								g_Player.move.x += sinf(D3DX_PI * 0.5f) * 1.5f;				//頂点座標を更新
							}
						}

						else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
						{// Wキーが押された

							if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
							{
								// Aキーが押された
								g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -1.5f;
								// 移動量を更新
								g_Player.move.x += -1;
								// 頂点座標を更新

							}
							else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
							{// 右下移動
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.5f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * -1.5f;
							}
							else
							{
								g_Player.move.y += cosf(D3DX_PI * 1.0f) * 1.5f;		//頂点座標を更新
							}
						}

						else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
						{// Sキーが押された
							if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
							{// Aキーが押された
								g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -1.5f;
								// 移動量を更新

							}
							else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
							{// 右下移動
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.5f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.5f;
							}
							else
							{
								g_Player.move.y += cosf(-D3DX_PI * 1.0f) * -1.5f;					//頂点座標を更新
							}

						}

						/*g_nBulletlvel -= 10;
						if (g_nBulletlvel == PLAYERBULLET_NORMAL)
						{
							g_nBulletlvel = PLAYERBULLET_NORMAL;
						}*/
						g_nSpeedlvel = 0;
					}

					if (g_nSpeedlvel == PLAYERSPEED_LEVEL3)
					{
						g_nSpeedlvel = PLAYERSPEED_LEVEL4;

						if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
						{
							if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
							{// 左上移動
								g_Player.move.x += sinf(D3DX_PI * 0.75f) * -1.75f;
								g_Player.move.y += cosf(D3DX_PI * 0.75f) * 1.75f;
								// 移動量を更新
							}
							else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
							{// 左下移動
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * -1.75f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.75f;
							}
							else
							{// Aキーが押された
								g_Player.move.x += sinf(D3DX_PI * 0.5f) * -1.75f;
								// 移動量を更新
								g_Player.move.x += -1;
								// 頂点座標を更新
							}

						}

						else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
						{
							if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
							{// 右上移動
								g_Player.move.x += sinf(D3DX_PI * 0.75f) * 1.75f;
								g_Player.move.y += cosf(D3DX_PI * 0.75f) * 1.75f;
							}
							else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
							{// 右下移動
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.75f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.75f;
							}
							else
							{// Dキーが押された
								g_Player.move.x += sinf(D3DX_PI * 0.5f) * 1.75f;				//頂点座標を更新
							}
						}

						else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
						{// Wキーが押された

							if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
							{
								// Aキーが押された
								g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -1.75f;
								// 移動量を更新
								g_Player.move.x += -1;
								// 頂点座標を更新

							}
							else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
							{// 右下移動
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.75f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * -1.75f;
							}
							else
							{
								g_Player.move.y += cosf(D3DX_PI * 1.0f) * 1.75f;		//頂点座標を更新
							}
						}

						else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
						{// Sキーが押された
							if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
							{// Aキーが押された
								g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -1.75f;
								// 移動量を更新

							}
							else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
							{// 右下移動
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.75f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.75f;
							}
							else
							{
								g_Player.move.y += cosf(-D3DX_PI * 1.0f) * -1.75f;					//頂点座標を更新
							}

						}

						/*g_nBulletlvel -= 10;
						if (g_nBulletlvel == PLAYERBULLET_NORMAL)
						{
							g_nBulletlvel = PLAYERBULLET_NORMAL;
						}*/
						g_nSpeedlvel = 0;
					}
				}
			}
		}
	

	}
#endif
}

//==================
//パワーアップ処理
//==================
void PowerUpPlayer(void)
{
	Enemy* pEnemy = GetEnemy();							// 敵の取得
	Bullet* pBullet = GetBullet();

	if (g_Player.state == PLAYERTYPE_SUCTION)		//パワーアップの方法
	{
		if (pBullet->type == BULLETTYPE_PLAYER) return;

		for (int nCnt = 0; nCnt < 128; nCnt++)
		{
			// 弾の座標に自機が重なった
			if (g_Player.pos.x - 35.0f <= pBullet[nCnt].pos.x &&
				g_Player.pos.x + 35.0f >= pBullet[nCnt].pos.x &&
				g_Player.pos.y - 35.0f <= pBullet[nCnt].pos.y &&
				g_Player.pos.y + 35.0f >= pBullet[nCnt].pos.y)
			{
				g_Player.nPowerupCountor++;

				if (g_Player.nPowerupCountor == 10)
				{
					if (g_nBulletlvel == PLAYERBULLET_NORMAL)
					{
						g_nBulletlvel = PLAYERBULLET_LEVEL1;

						// サウンドの再生
						PlaySound(SOUND_LABEL_SE_SHOT);

						g_nSpeedlvel -= 10;
						if (g_nSpeedlvel == PLAYERSPEED_NORMAL)
						{
							g_nSpeedlvel = PLAYERSPEED_NORMAL;
						}
						g_Player.nPowerupCountor = 0;

					}
					else if (g_nBulletlvel == PLAYERBULLET_LEVEL1)
					{
						g_nBulletlvel = PLAYERBULLET_LEVEL2;

						// サウンドの再生
						PlaySound(SOUND_LABEL_SE_SHOT);

						g_nSpeedlvel -= 10;
						if (g_nSpeedlvel == PLAYERSPEED_NORMAL)
						{
							g_nSpeedlvel = PLAYERSPEED_NORMAL;
						}
						g_Player.nPowerupCountor = 0;

					}
					else if (g_nBulletlvel == PLAYERBULLET_LEVEL2)
					{
						g_nBulletlvel = PLAYERBULLET_LEVEL3;

						// サウンドの再生
						PlaySound(SOUND_LABEL_SE_SHOT);

						g_nSpeedlvel -= 10;
						if (g_nSpeedlvel == PLAYERSPEED_NORMAL)
						{
							g_nSpeedlvel = PLAYERSPEED_NORMAL;
						}
						g_Player.nPowerupCountor = 0;

					}
					else if (g_nBulletlvel == PLAYERBULLET_LEVEL3)
					{
						g_nBulletlvel = PLAYERBULLET_LEVEL4;

						// サウンドの再生
						PlaySound(SOUND_LABEL_SE_SHOT);

						g_nSpeedlvel -= 10;
						if (g_nSpeedlvel == PLAYERSPEED_NORMAL)
						{
							g_nSpeedlvel = PLAYERSPEED_NORMAL;
						}
						g_Player.nPowerupCountor = 0;
					}

					if (g_nBulletlvel >= PLAYERBULLET_LEVEL4)
					{
						g_nBulletlvel = PLAYERBULLET_LEVEL4;
					}

				}
			}
		}
	}

}

//========================
// プレイヤーの取得
//========================
Player* GetPlayer(void)
{
	return &g_Player;
}