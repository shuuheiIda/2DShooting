//=======================================
//
// キーボード処理 [input.cpp]
// Author shuuhei Ida
//
//=======================================
#include "input.h"

// マクロ定義
#define NUM_KEY_MAX (256)					// キーの最大数

// グローバル変数
LPDIRECTINPUT8 g_pInput = NULL;				// DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;	// 入力デバイス(キーボード)へのポインタ
BYTE g_aKeyState[NUM_KEY_MAX];				// キーボードプレス情報
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];
XINPUT_STATE g_joyKeyState;					// ジョイパッドのプレス情報
XINPUT_STATE g_joyKeyStateTrigger;			// ジョイパッドのトリガー情報
Vibration g_Vibration;						//ジョイパッドの振動
bool g_bVibration = false;					//振動しているかのフラグ
DWORD g_EndVibrationtime = 0;				//振動を終了した時間

//=========================
// キーボードの初期化処理
//=========================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	// 入力デバイス（キーボードの生成）
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel (hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();
	
	return S_OK;
}

//=============================
// キーボードの終了処理
//=============================
void UninitKeyboard(void)
{
	// 入力デバイス(キーボードの破棄)
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();

		g_pDevKeyboard = NULL;
	}

	// DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();

		g_pInput = NULL;
	}

}

//========================
// キーボードの更新処理
//========================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];		// キーボードの入力情報

	int nCntKey;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ 0x80)  & aKeyState[nCntKey] ;// キーボードのトリガー情報を保存
			g_aKeyState[nCntKey] = aKeyState[nCntKey];		// キーボードのプレス情報を保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();							// キーボードへのアクセス権を獲得
	}
}

//=============================
// キーボードプレスの情報を取得
//=============================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=========================
// ジョイパッドの初期化処理
//=========================
HRESULT InitJoypad(void)
{
	// メモリのクリア
	memset(&g_joyKeyState, 0, sizeof(XINPUT_STATE));

	// XInputのステートの設定(有効にする)
	XInputEnable(true);
	return S_OK;

}

//=======================
// ジョイパッドの終了処理
//=======================
void UninitJoypad(void)
{
	// XInpuutのステートを設定(無効にする)
	XInputEnable(false);
}

//=======================
// ジョイパッドの更新処理
//=======================
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;

	// ジョイパッドの状態を取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		g_joyKeyStateTrigger.Gamepad.wButtons = (g_joyKeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & joykeyState.Gamepad.wButtons;// キーボードのトリガー情報を保存
		g_joyKeyState = joykeyState;
	}

}

//==============================
// ジョイパッドのプレス情報の取得
//==============================
bool GetJoypadPress(JOYKEY key)
{
	return (g_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

bool GetJoypadTrigger(JOYKEY key)
{
	return (g_joyKeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=====================
// スティックの移動処理
//=====================
bool Stickmove(JOYKEY key)
{
	if (key == JOYKEY_L_LITESTICK)
	{
		return(g_joyKeyState.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? true : false;
	}

	else if (key == JOYKEY_L_RIGHTSTICK)
	{
		return(g_joyKeyState.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? true : false;
	}

	else if (key == JOYKEY_L_UPSTICK)
	{
		return(g_joyKeyState.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? true : false;
	}

	else if (key == JOYKEY_L_DOWNSTICK)
	{
		return(g_joyKeyState.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? true : false;
	}
}

//====================
//ジョイパッドの振動
//====================
void SetVibration(WORD wLeftMotorSpeed,WORD wRightMotorSpeed,int nCounttime)
{
	g_Vibration.wLeftMotorSpeed = wLeftMotorSpeed;
	g_Vibration.wRightMotorSpeed = wRightMotorSpeed;

	g_bVibration = true;
	g_EndVibrationtime = nCounttime+GetTickCount64();

	//XINPUTの設定
	XINPUT_VIBRATION Vibration = {};
	Vibration.wLeftMotorSpeed = wLeftMotorSpeed;
	Vibration.wRightMotorSpeed = wRightMotorSpeed;
	XInputSetState(0, &Vibration);

}

//=====================
//振動の更新処理
//=====================
void UpdateVibration(void)
{
	if (g_bVibration = true && GetTickCount64() >= g_EndVibrationtime)
	{
		//XINPUTの設定
		XINPUT_VIBRATION Vibration = {};
		XInputSetState(0, &Vibration);
		g_bVibration = false;
	}

}



