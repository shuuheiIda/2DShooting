//==========================
//
//ランニングマン
//Author shuuhei Ida
//
//==========================
#include"main.h"
#include"player2.h"

//グローバル変数

LPDIRECT3DTEXTURE9 g_pTexturePlayer2 = NULL;	//テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer2 = NULL;	//頂点バッファへのポインタ

D3DXVECTOR3 g_posPlayer2;

D3DXVECTOR3 g_movePlayer2;



//========================
//プレイヤーの初期化処理
//========================
void InitPlayer2(void)
{
	LPDIRECT3DDEVICE9 pDevice;								//デバイスへのポインタ



	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\2.jpg", &g_pTexturePlayer2);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer2,
		NULL);

	VERTEX_2D* pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer2->Lock(0, 0,(void**)&pVtx, 0);

	//頂点座標の設定(x,y,z,の順番になる、zの値は2Dの場合は必ず0にする)
	pVtx[0].pos = D3DXVECTOR3(0, 200, 0.0f);

	pVtx[1].pos = D3DXVECTOR3(300,200, 0.0f);

	pVtx[2].pos = D3DXVECTOR3(0,500, 0.0f);

	pVtx[3].pos = D3DXVECTOR3(300,500, 0.0f);

	//rhwの設定

	pVtx[0].rhw = 1.0f;

	pVtx[1].rhw = 1.0f;

	pVtx[2].rhw = 1.0f;

	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);

	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);

	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer2->Unlock();
}
//==========================
//プレイヤーの終了処理
//==========================
void UninitPlayer2(void)
{
	//テクスチャの破棄
	if (g_pTexturePlayer2 != NULL)
	{
		g_pTexturePlayer2->Release();
		g_pTexturePlayer2 = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffPlayer2 != NULL)
	{
		g_pVtxBuffPlayer2->Release();

		g_pVtxBuffPlayer2 = NULL;
	}

}
//========================
//プレイヤーの更新処理
//========================
void UpdatePlayer2(void)
{

}
//=======================
//プレイヤーの描画処理
//=======================
void DrawPlayer2(void)
{
	LPDIRECT3DDEVICE9 pDevice;					//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer2, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer2);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		2);
}
