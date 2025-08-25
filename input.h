//==========================
//
// キーボード処理 [input.h]
// Author shuuhei Ida
//
//==========================
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

// ジョイパッドのキーの種類
typedef enum
{
	JOYKEY_UP = 0,
	JOYKEY_DOWN,
	JOYKEY_LEFT,
	JOYKEY_RIGHT,
	JOYKEY_START,
	JOYKEY_A =12,
	JOYKEY_B,
	JOYKEY_X,
	JOYKEY_Y,
	JOYKEY_L_LITESTICK = 20,
	JOYKEY_L_RIGHTSTICK,
	JOYKEY_L_UPSTICK,
	JOYKEY_L_DOWNSTICK,
	JOYKEY_MAX
}JOYKEY;

typedef struct 
{
	WORD wLeftMotorSpeed;
	WORD wRightMotorSpeed;

}Vibration;

// プロトタイプ宣言
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);

void UninitKeyboard(void);

void UpdateKeyboard(void);

bool GetKeyboardPress(int nKey);

bool GetKeyboardTrigger(int nKey);

HRESULT InitJoypad(void);

void UninitJoypad(void);

void UpdateJoypad(void);

bool GetJoypadPress(JOYKEY key);

bool GetJoypadTrigger(JOYKEY key);

bool Stickmove(JOYKEY key);

void SetVibration(WORD wLeftMotorSpeed, WORD wRightMotorSpeed, int nCounttime);

void UpdateVibration(void);

#endif // !_INPUT_H_

