//==========================
//
//�V���[�e�B���O�Q�[������
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

//�}�N����`
#define CLASS_NAME		"WindowClass"			//
#define WINDOW_NAME		"�V���[�e�B���O�Q�[������"		// �L���v�V����

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProoc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);

void Uninit(void);

void Update(void);

void Draw(void);

void DrawDebug(void);

void DrawOperation(void);

//�O���[�o���ϐ�
LPDIRECT3D9 g_pD3D = NULL;	//

LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;


MODE g_mode = MODE_TITLE;

LPD3DXFONT g_pFont = NULL;

int g_nCountFPS = 0;

//==================
//���C���֐�
//==================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdshoW)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);			// ���������[�N���m�p�̃t���O
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
	DWORD dwCurrentTime ;		//���ݎ���
	DWORD dwExecLastTime;		//�Ō�ɏ�����������

	DWORD dwFrameCount =0;			//�t���[���J�E���g
	DWORD dwFPSLastTime =0;		//�Ō��FPS���v����������

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// �E�B���h�𐶐�
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

	// ����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{// ���������������s�����ꍇ
		return -1;
	}

	// ����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;

	dwExecLastTime = timeGetTime();

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// �E�B���h�̕\��
	ShowWindow(hWnd, nCmdshoW);
	UpdateWindow(hWnd);

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// windows�̏���
			if (msg.message == WM_QUIT)
			{// WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				// ���b�Z�[�W�ݒ�
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectX�̏���
			dwCurrentTime = timeGetTime();	// ���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5�b�o��
				// FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;								// FPS���ϑ�����������ۑ�

				dwFrameCount = 0;
			}

			if (dwCurrentTime - dwExecLastTime >= (1000/60))
			{// 60���̂P�o��
				dwExecLastTime = dwCurrentTime;		// �����J�n�̎���[���ݎ���]��ۑ�

				dwFrameCount++;						// �t���[���J�E���g�����Z

				// �X�V����
				Update();

				// �`�揈��
				Draw();
			}

		}
		
	}
	// ����\��߂�
	timeEndPeriod(1);

	// �I������
	Uninit();

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return(int)msg.wParam;

}

//=============
//����������
//=============
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h

	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�����p�����[�^

	// Direct3D�I�u�W�F�N�g�̐���

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	// ���݂̃f�B�X�v���C���[�h���擾

	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	// �f�o�C�X�v���[���e�[�V�����p�����[�^�[�̐ݒ�

	ZeroMemory(&d3dpp, sizeof(d3dpp));// �p�����[�^�[�̃[���N���A

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

	// Direct3D�f�o�C�X�̐���
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

	// �����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				//�J�����O�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				//��
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//�@�A���t�@�u�����h�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//��

	// �T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	//��
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	//�b
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	//�@�e�N�X�`���̌J��Ԃ��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	//��

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	//��
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	//�@�e�N�X�`���̃A���t�@�u�����h�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	//��

	// �f�o�b�O�\���p�t�H���g�̐���
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH, "Terminal", &g_pFont);

	// �L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// �e��I�u�W�F�N�g�̏���������
	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}
	InitSound(hWnd);	
	
	

	// ���[�h�̐ݒ�
	InitFade(g_mode);

	// �v���C���[�̏���������
	return S_OK;
} 

//============
//�I������
//============
void Uninit(void)
{
	// �^�C�g����ʂ̏I������
	UninitTitle();

	// �Q�[����ʂ̏I������
	UninitGame();

	// ���U���g��ʂ̏I������
	UninitResult();

	// �L�[�{�[�h�̏���������
	UninitKeyboard();
	// �T�E���h�̒�~
	StopSound();
	// �T�E���h�̏I������
	UninitSound();
	// �W���C�p�b�h�̏I������
	UninitJoypad();
	// �f�o�b�O�\���p�t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	// Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//=============
//�X�V����
//=============
void Update(void)
{// �L�[�{�[�h�̍X�V����
	UpdateKeyboard();
	// �W���C�p�b�h�̍X�V����
	UpdateJoypad();
	UpdateVibration();

	// �e��I�u�W�F�N�g�̍X�V����
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
	// �t�F�[�h�̍X�V����
	UpdateFade();
	

}

//============
//�`�揈��
//============
void Draw(void)
{
// ��ʃN���A
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{// �`��J�n�����������ꍇ
		// �e��I�u�W�F�N�g�̕`�揈��
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

		// �t�F�[�h�̕`�ʏ���
		DrawFade();

#ifdef _DEBUG
		// �f�o�b�O�\��
		DrawDebug();
#endif


		// �������
		DrawOperation();

		
		// �`��I��
		g_pD3DDevice->EndScene();
	}
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=========
//�`�揈��
//=========
void DrawDebug(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect1 = { 0,20,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[256];									// ��ʂɕ\�����镶����
	char aStr1[256];									// ��ʂɕ\�����镶����

	// ���������
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);
	sprintf(&aStr1[0], "PLAYERPOS { %.2f,%.2f,%.2f }\n", GetPlayer()->pos.x, GetPlayer()->pos.y, GetPlayer()->pos.z);

	// �e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStr1[0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

}

//=========
//�������
//=========
void DrawOperation(void)
{
	RECT rect = { 0,40,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[256];

	// ���������
	wsprintf(&aStr[0], "�z��:B");

	// �e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//================
//�f�o�C�X�̎擾
//================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
void SetMode(MODE mode)
{
	// ���݂̉��(���[�h)�̏I������
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
	// �V�������(���[�h)�̏���������
	
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
//�E�B���h�E�v���V�[�W��
//=======================
LRESULT CALLBACK WindowProoc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;				// �Ԃ�l���i�[

	switch (uMsg)
	{
	case WM_DESTROY:// �E�B���h�E�j���̃��b�Z�[�W
		// WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
	case WM_KEYDOWN:	// �L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE: // [ESC]�L�[�������ꂽ
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);
			if (nID == IDYES)
			{
				// �E�B���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);				
			}
			break;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
 
