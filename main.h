//==========================
//
//シューティングゲームα版
//Author shuuhei Ida
//
//==========================

#ifndef _MAIN_H_						//
#define _MAIN_H_						// 二重のインクルード防止のマクロ
#include<Windows.h>
#include"d3dx9.h"						// 描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)	// ビルド時の警告文対処用のマクロ
#include "dinput.h"
#include"xaudio2.h"
#include"xinput.h"

// ライブラリのリンク
#pragma comment (lib,"d3d9.lib")	// 描画処理に必要
#pragma comment (lib,"d3dx9.lib")	// [d3d9.lib]の拡張ライブラリ
#pragma comment (lib,"dxguid.lib")	// DirectXコンポーネント(部品)使用に必要
#pragma comment (lib,"winmm.lib")	// システム時刻取得に必要
#pragma comment (lib,"dinput8.lib")	// 入力処理に必要
#pragma comment (lib,"xinput.lib")	// ジョイパッド処理に必要

// マクロ定義
#define SCREEN_WIDTH	(1280)		// ウィンドウの幅
#define SCREEN_HEIGHT	(720)		// ウィンドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)	// 頂点フォーマット

// 頂点座標の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float rhw;			// 座標変換用係数
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標(x,y)
}VERTEX_2D;

// 画面(モード)の種類
typedef enum
{
	MODE_TITLE = 0,
	MODE_GAME,
	MODE_RESULT,
	MODE_MAX
}MODE;


// プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);

MODE GetMode(void);



#endif // !_MAIN_H_