//==========================
//
// ���ԏ��� [Time.cpp]
// Author shuuhei Ida
//
//==========================

#include"main.h"
#include "time.h"
#include"enemy.h"
#include"fade.h"

// �}�N����`
#define NUM_PLACE	(3)						// �X�R�A�̌���

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;	// �e�N�X�`���ւ̃|�C���^

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

D3DXVECTOR3 g_posTime;						// �X�R�A�̈ʒu

int g_nTime;								// �X�R�A�̒l

//======================
// �^�C�}�[�̏���������
//======================
void InitTime(void)
{
	int nCntExplosion;

	LPDIRECT3DDEVICE9 pDevice;								// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureTime);
	g_posTime = D3DXVECTOR3(550.0f,0.0f,0.0f);
	g_nTime = 18000;

	for (nCntExplosion = 0; nCntExplosion < NUM_PLACE; nCntExplosion++)
	{		
		// ���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*4 * NUM_PLACE,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffTime,
			NULL);
		
		VERTEX_2D* pVtx;		// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);
		for (nCntExplosion = 0; nCntExplosion < NUM_PLACE; nCntExplosion++)
		{
			// ���_���W�̐ݒ�(x,y,z,�̏��ԂɂȂ�Az�̒l��2D�̏ꍇ�͕K��0�ɂ���)
			pVtx[0].pos = D3DXVECTOR3(g_posTime.x+(nCntExplosion*30), g_posTime.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posTime.x+(nCntExplosion+1)*30, g_posTime.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posTime.x+nCntExplosion*30, g_posTime.y+45, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posTime.x+(nCntExplosion+1)*30, g_posTime.y+45, 0.0f);

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
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;
		}
			// ���_�o�b�t�@���A�����b�N����
			g_pVtxBuffTime->Unlock();
	
	}
	
}

//====================
// �^�C�}�[�̏I������
//====================
void UninitTime(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();

		g_pVtxBuffTime = NULL;
	}
}

//====================
// �^�C�}�[�̍X�V����
//====================
void UpdateTime(void)
{
	static int nFreamCounter = 0;

	if ((nFreamCounter % 60) == 0)
	{
		nFreamCounter = 0;
		g_nTime--;
	}

	int nCntTime;

	int aTexU[NUM_PLACE];

	aTexU[0] = g_nTime % 100000 / 10000;

	aTexU[1] = g_nTime % 10000 / 1000;

	aTexU[2] = g_nTime % 1000 / 100;

	VERTEX_2D* pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1, 0.0f);

		pVtx[1].tex = D3DXVECTOR2((aTexU[nCntTime] + 1) * 0.1, 0.0f);

		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1, 1.0f);

		pVtx[3].tex = D3DXVECTOR2((aTexU[nCntTime] + 1) * 0.1, 1.0f);

		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTime->Unlock();


	if (g_nTime <= 0)
	{
		SetFade(MODE_RESULT);
	}
}

//====================
// �^�C�}�[�̕`�揈��
//====================
void DrawTime(void)
{
	int nCntTime;

	LPDIRECT3DDEVICE9 pDevice;					// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTime);

	for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntTime * 4,
				2);
	}
}
