//=======================================
//
// �L�[�{�[�h���� [input.cpp]
// Author shuuhei Ida
//
//=======================================
#include "input.h"

// �}�N����`
#define NUM_KEY_MAX (256)					// �L�[�̍ő吔

// �O���[�o���ϐ�
LPDIRECTINPUT8 g_pInput = NULL;				// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;	// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
BYTE g_aKeyState[NUM_KEY_MAX];				// �L�[�{�[�h�v���X���
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];
XINPUT_STATE g_joyKeyState;					// �W���C�p�b�h�̃v���X���
XINPUT_STATE g_joyKeyStateTrigger;			// �W���C�p�b�h�̃g���K�[���
Vibration g_Vibration;						//�W���C�p�b�h�̐U��
bool g_bVibration = false;					//�U�����Ă��邩�̃t���O
DWORD g_EndVibrationtime = 0;				//�U�����I����������

//=========================
// �L�[�{�[�h�̏���������
//=========================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	// ���̓f�o�C�X�i�L�[�{�[�h�̐����j
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel (hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	g_pDevKeyboard->Acquire();
	
	return S_OK;
}

//=============================
// �L�[�{�[�h�̏I������
//=============================
void UninitKeyboard(void)
{
	// ���̓f�o�C�X(�L�[�{�[�h�̔j��)
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();

		g_pDevKeyboard = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInput != NULL)
	{
		g_pInput->Release();

		g_pInput = NULL;
	}

}

//========================
// �L�[�{�[�h�̍X�V����
//========================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];		// �L�[�{�[�h�̓��͏��

	int nCntKey;

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ 0x80)  & aKeyState[nCntKey] ;// �L�[�{�[�h�̃g���K�[����ۑ�
			g_aKeyState[nCntKey] = aKeyState[nCntKey];		// �L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();							// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//=============================
// �L�[�{�[�h�v���X�̏����擾
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
// �W���C�p�b�h�̏���������
//=========================
HRESULT InitJoypad(void)
{
	// �������̃N���A
	memset(&g_joyKeyState, 0, sizeof(XINPUT_STATE));

	// XInput�̃X�e�[�g�̐ݒ�(�L���ɂ���)
	XInputEnable(true);
	return S_OK;

}

//=======================
// �W���C�p�b�h�̏I������
//=======================
void UninitJoypad(void)
{
	// XInpuut�̃X�e�[�g��ݒ�(�����ɂ���)
	XInputEnable(false);
}

//=======================
// �W���C�p�b�h�̍X�V����
//=======================
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;

	// �W���C�p�b�h�̏�Ԃ��擾
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		g_joyKeyStateTrigger.Gamepad.wButtons = (g_joyKeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & joykeyState.Gamepad.wButtons;// �L�[�{�[�h�̃g���K�[����ۑ�
		g_joyKeyState = joykeyState;
	}

}

//==============================
// �W���C�p�b�h�̃v���X���̎擾
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
// �X�e�B�b�N�̈ړ�����
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
//�W���C�p�b�h�̐U��
//====================
void SetVibration(WORD wLeftMotorSpeed,WORD wRightMotorSpeed,int nCounttime)
{
	g_Vibration.wLeftMotorSpeed = wLeftMotorSpeed;
	g_Vibration.wRightMotorSpeed = wRightMotorSpeed;

	g_bVibration = true;
	g_EndVibrationtime = nCounttime+GetTickCount64();

	//XINPUT�̐ݒ�
	XINPUT_VIBRATION Vibration = {};
	Vibration.wLeftMotorSpeed = wLeftMotorSpeed;
	Vibration.wRightMotorSpeed = wRightMotorSpeed;
	XInputSetState(0, &Vibration);

}

//=====================
//�U���̍X�V����
//=====================
void UpdateVibration(void)
{
	if (g_bVibration = true && GetTickCount64() >= g_EndVibrationtime)
	{
		//XINPUT�̐ݒ�
		XINPUT_VIBRATION Vibration = {};
		XInputSetState(0, &Vibration);
		g_bVibration = false;
	}

}



