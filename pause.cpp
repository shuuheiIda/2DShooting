//==========================
//
//�t�F�[�h���� [pause.cpp]
//Author:shuuhei Ida
//
//==========================

//�C���N���[�h�t�@�C��
#include "pause.h"
#include"game.h"
#include"fade.h"
#include"main.h"
#include"input.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturePause[PAUSE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

int g_nSerect = 0;

//=================
//����������
//=================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\8.png", &g_pTexturePause[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\9.png", &g_pTexturePause[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\quit.png", &g_pTexturePause[2]);

	VERTEX_2D* pVtx;		// ���_���ւ̃|�C���^

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < PAUSE_MAX; nCount++)
	{
		// ���_���W�̐ݒ�(x,y,z,�̏��ԂɂȂ�Az�̒l��2D�̏ꍇ�͕K��0�ɂ���)
		pVtx[0].pos = D3DXVECTOR3(350.0f, 100.0f + (nCount * 120.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(600.0f, 100.0f + (nCount * 120.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(350.0f, 200.0f + (nCount * 120.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(600.0f, 200.0f + (nCount * 120.0f), 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();

	g_nSerect = 0;
}

//==================
//�I������
//==================
void UninitPause(void)
{
	//�e�N�X�`���̔j��
	for (int nCount = 0; nCount < PAUSE_MAX; nCount++)
	{
		if (g_pTexturePause[nCount] != NULL)
		{
			g_pTexturePause[nCount]->Release();
			g_pTexturePause[nCount] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//==================
//�X�V����
//==================
void UpdatePause(void)
{
	VERTEX_2D* pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	//���̓f�o�C�X
	if (GetKeyboardTrigger(DIK_W))			//W�L�[����������
	{
		g_nSerect++;
		if (g_nSerect >= PAUSE_MAX)
		{
			g_nSerect = PAUSE_CONTINUE;
		}
	}
	else if (GetKeyboardTrigger(DIK_S))		//S�L�[������������
	{
		g_nSerect--;
		if (g_nSerect < PAUSE_CONTINUE)
		{
			g_nSerect = PAUSE_QUIT;
		}
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		switch (g_nSerect)
		{
		case PAUSE_CONTINUE:

			SetEnablePause(false);
			break;

		case PAUSE_RETRY:

			SetFade(MODE_GAME);
			break;

		case PAUSE_QUIT:

			SetFade(MODE_TITLE);
			break;

		}
	}
	for (int nCount = 0; nCount < PAUSE_MAX; nCount++)
	{
		if (nCount == g_nSerect)
		{
			pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

		}
		else
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		pVtx += 4;
	}
	
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();

}

//=================
//�`�揈��
//=================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;					// �f�o�C�X�ւ̃|�C���^
	
	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCount = 0; nCount < PAUSE_MAX; nCount++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePause[nCount]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCount*4,
			2);
	}	
}