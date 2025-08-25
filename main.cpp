//==========================
//
//シューティングゲームα版
//Author shuuhei Ida
//
//==========================

#include"main.h"
#include "input.h"
#include"sound.h"
#include"title.h"
#include"game.h"
#include"result.h"
#include"fade.h"
#include "player.h"
#include <cstdio>
#include<crtdbg.h>

//マクロ定義
#define CLASS_NAME		"WindowClass"			//
#define WINDOW_NAME		"シューティングゲームα版"		// キャプション

//プロトタイプ宣言
LRESULT CALLBACK WindowProoc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);

void Uninit(void);

void Update(void);

void Draw(void);

void DrawDebug(void);

void DrawOperation(void);

//グローバル変数
LPDIRECT3D9 g_pD3D = NULL;	//

LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;


MODE g_mode = MODE_TITLE;

LPD3DXFONT g_pFont = NULL;

int g_nCountFPS = 0;

//==================
//メイン関数
//==================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdshoW)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);			// メモリリーク検知用のフラグ
#endif // _DEBUG
	WNDCLASSEX wcex =
	{
	sizeof(WNDCLASSEX),
	CS_CLASSDC,
	WindowProoc,
	0,
	0,
	hInstance,
	LoadIcon(NULL,IDI_APPLICATION),
	LoadCursor(NULL,IDC_ARROW),
	(HBRUSH)(COLOR_WINDOW),
	NULL,
	CLASS_NAME,
	LoadIcon(NULL,IDI_APPLICATION)
	};
	HWND hWnd;
	MSG msg;
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//
	DWORD dwCurrentTime ;		//現在時刻
	DWORD dwExecLastTime;		//最後に処理した時刻

	DWORD dwFrameCount =0;			//フレームカウント
	DWORD dwFPSLastTime =0;		//最後にFPSを計測した時刻

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウィンドを生成
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(rect.right - rect.left),
		(rect.bottom - rect.top),
		NULL,
		NULL,
		hInstance,
		NULL);

	// 初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{// 初期化処理が失敗した場合
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;

	dwExecLastTime = timeGetTime();

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// ウィンドの表示
	ShowWindow(hWnd, nCmdshoW);
	UpdateWindow(hWnd);

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// windowsの処理
			if (msg.message == WM_QUIT)
			{// WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				// メッセージ設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectXの処理
			dwCurrentTime = timeGetTime();	// 現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒経過
				// FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;								// FPSを観測した時刻を保存

				dwFrameCount = 0;
			}

			if (dwCurrentTime - dwExecLastTime >= (1000/60))
			{// 60分の１経過
				dwExecLastTime = dwCurrentTime;		// 処理開始の時刻[現在時刻]を保存

				dwFrameCount++;						// フレームカウントを加算

				// 更新処理
				Update();

				// 描画処理
				Draw();
			}

		}
		
	}
	// 分解能を戻す
	timeEndPeriod(1);

	// 終了処理
	Uninit();

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return(int)msg.wParam;

}

//=============
//初期化処理
//=============
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード

	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	// 現在のディスプレイモードを取得

	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	// デバイスプレゼンテーションパラメーターの設定

	ZeroMemory(&d3dpp, sizeof(d3dpp));// パラメーターのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;

	d3dpp.BackBufferHeight = SCREEN_HEIGHT;

	d3dpp.BackBufferFormat = d3ddm.Format;

	d3dpp.BackBufferCount = 1;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.EnableAutoDepthStencil = TRUE;

	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	d3dpp.Windowed = bWindow;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// Direct3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;

			}
		}
	}

	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				//カリングの設定
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				//┓
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//　アルファブレンドの設定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//┛

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	//┓
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	//｜
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	//　テクスチャの繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	//┛

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	//┓
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	//　テクスチャのアルファブレンドの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	//┛

	// デバッグ表示用フォントの生成
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH, "Terminal", &g_pFont);

	// キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// 各種オブジェクトの初期化処理
	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}
	InitSound(hWnd);	
	
	

	// モードの設定
	InitFade(g_mode);

	// プレイヤーの初期化処理
	return S_OK;
} 

//============
//終了処理
//============
void Uninit(void)
{
	// タイトル画面の終了処理
	UninitTitle();

	// ゲーム画面の終了処理
	UninitGame();

	// リザルト画面の終了処理
	UninitResult();

	// キーボードの初期化処理
	UninitKeyboard();
	// サウンドの停止
	StopSound();
	// サウンドの終了処理
	UninitSound();
	// ジョイパッドの終了処理
	UninitJoypad();
	// デバッグ表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	// Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//=============
//更新処理
//=============
void Update(void)
{// キーボードの更新処理
	UpdateKeyboard();
	// ジョイパッドの更新処理
	UpdateJoypad();
	UpdateVibration();

	// 各種オブジェクトの更新処理
	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();
		break;
	case MODE_GAME:
		UpdateGame();
		break;
	case MODE_RESULT:
		UpdateResult();
		break;
	}
	// フェードの更新処理
	UpdateFade();
	

}

//============
//描画処理
//============
void Draw(void)
{
// 画面クリア
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{// 描画開始が成功した場合
		// 各種オブジェクトの描画処理
		switch (g_mode)
		{
			case MODE_TITLE:
				DrawTitle();
				break;
			case MODE_GAME:
				DrawGame();
				break;
			case MODE_RESULT:
				DrawResult();
				break;
				
		}

		// フェードの描写処理
		DrawFade();

#ifdef _DEBUG
		// デバッグ表示
		DrawDebug();
#endif


		// 操作説明
		DrawOperation();

		
		// 描画終了
		g_pD3DDevice->EndScene();
	}
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=========
//描画処理
//=========
void DrawDebug(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect1 = { 0,20,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[256];									// 画面に表示する文字列
	char aStr1[256];									// 画面に表示する文字列

	// 文字列を代入
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);
	sprintf(&aStr1[0], "PLAYERPOS { %.2f,%.2f,%.2f }\n", GetPlayer()->pos.x, GetPlayer()->pos.y, GetPlayer()->pos.z);

	// テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStr1[0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

}

//=========
//操作説明
//=========
void DrawOperation(void)
{
	RECT rect = { 0,40,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[256];

	// 文字列を代入
	wsprintf(&aStr[0], "吸収:B");

	// テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//================
//デバイスの取得
//================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
void SetMode(MODE mode)
{
	// 現在の画面(モード)の終了処理
	switch (g_mode)
	{
		case MODE_TITLE:
			UninitTitle();
			break;
		case MODE_GAME:
			UninitGame();
			break;
		case MODE_RESULT:
			UninitResult();
			break;
	}
	// 新しい画面(モード)の初期化処理
	
		switch (mode)
		{
		case MODE_TITLE:
			InitTitle();
			break;

		case MODE_GAME:
			InitGame();
			break;

		case MODE_RESULT:
			InitResult();
			break;
		}
		g_mode = mode;
}
MODE GetMode(void)
{
	return g_mode;
}

//=======================
//ウィンドウプロシージャ
//=======================
LRESULT CALLBACK WindowProoc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;				// 返り値を格納

	switch (uMsg)
	{
	case WM_DESTROY:// ウィンドウ破棄のメッセージ
		// WM_QUITメッセージを送る
		PostQuitMessage(0);
	case WM_KEYDOWN:	// キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE: // [ESC]キーが押された
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				// ウィンドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);				
			}
			break;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
 
