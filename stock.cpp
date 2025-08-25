//==========================
//
// �X�g�b�N���� [stock.cpp]
// Author shuuhei Ida
//
//==========================

#include"main.h"
#include "stock.h"
#include"enemy.h"
#include"player.h"
#include"bullet.h"

// �}�N����`
#define NUM_STOCK	(3)						// �X�g�b�N�̌���

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturestock = NULL;	// �e�N�X�`���ւ̃|�C���^

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffstock = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

D3DXVECTOR3 g_posstock;						// �X�g�b�N�̈ʒu

int g_nstock;								// �X�g�b�N�̒l

//====================
// �X�g�b�N�̏���������
//====================
void InitStock(void)
{
	int nCntExplosion;

	LPDIRECT3DDEVICE9 pDevice;								// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\player000.png", &g_pTexturestock);
	g_posstock = D3DXVECTOR3(20.0f,0.0f,0.0f);
	g_nstock = 0;

	for (nCntExplosion = 0; nCntExplosion < NUM_STOCK; nCntExplosion++)
	{		
		// ���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*4 * NUM_STOCK,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffstock,
			NULL);
		
		VERTEX_2D* pVtx;		// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffstock->Lock(0, 0, (void**)&pVtx, 0);
		for (nCntExplosion = 0; nCntExplosion < NUM_STOCK; nCntExplosion++)
		{
			// ���_���W�̐ݒ�(x,y,z,�̏��ԂɂȂ�Az�̒l��2D�̏ꍇ�͕K��0�ɂ���)
			pVtx[0].pos = D3DXVECTOR3(g_posstock.x+(nCntExplosion*30), g_posstock.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posstock.x+(nCntExplosion+1)*30, g_posstock.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posstock.x+nCntExplosion*30, g_posstock.y+45, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posstock.x+(nCntExplosion+1)*30, g_posstock.y+45, 0.0f);

			// rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;
		}
			// ���_�o�b�t�@���A�����b�N����
			g_pVtxBuffstock->Unlock();
	
	}
	
}

//=================
//�X�g�b�N�̏I������
//=================
void UninitStock(void)
{
	// �e�N�X�`���̔j��
	if (g_pTexturestock != NULL)
	{
		g_pTexturestock->Release();
		g_pTexturestock = NULL;
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffstock != NULL)
	{
		g_pVtxBuffstock->Release();

		g_pVtxBuffstock = NULL;
	}
}

//==================
// �X�g�b�N�̍X�V����
//==================
void UpdateStock(void)
{
	Player* pPlayer = GetPlayer();
	Bullet* pBullet = GetBullet();

	if (pPlayer->pos.x - 35 <= pBullet->pos.x &&
		pPlayer->pos.x + 35 >= pBullet->pos.x &&
		pPlayer->pos.y - 35 <= pBullet->pos.y &&
		pPlayer->pos.y + 35 >= pBullet->pos.y)
	{
		pPlayer->nStock--;
	}
}

//===================
// �X�g�b�N�̕`�揈��
//===================
void DrawStock(void)
{

	Player* pPlayer = GetPlayer();

	int nCntstock;

	LPDIRECT3DDEVICE9 pDevice;					// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffstock, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturestock);

	for (nCntstock = 0; nCntstock < NUM_STOCK; nCntstock++)
	{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntstock * 4,
				2);
	}

}

