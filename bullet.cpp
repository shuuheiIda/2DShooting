//==========================
//
//�e���� [bullet.cpp]
//Author shuuhei Ida
//
//==========================

// �C���N���[�h�t�@�C��
#include"main.h"
#include "bullet.h"
#include "explosion .h"
#include"enemy.h"
#include"player.h"
#include"effect.h"
#include"sound.h"
#include"camera.h"

// �}�N����`
#define MAX_BULLET (128)				//�e�̍ő吔

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Bullet g_aBullet[MAX_BULLET];						//�e�̏��

//================
//�e�̏���������
//================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;								//�f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, 
		"data\\TEXTURE\\bullet000.png",
		&g_pTextureBullet);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// �e�̏��̏�����
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].bUse = false;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D* pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// ���_���W�̐ݒ�(x,y,z,�̏��ԂɂȂ�Az�̒l��2D�̏ꍇ�͕K��0�ɂ���)
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//=================
//�e�̏I������
//=================
void UninitBullet(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();

		g_pVtxBuffBullet = NULL;
	}

}

//==============
//�e�̍X�V����
//==============
void UpdateBullet(void)
{
	VERTEX_2D* pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{// �e���g�p����Ă���

		if (g_aBullet[nCntBullet].bUse == true)
		{

			Enemy* pEnemy;
			// �G�̎擾
			pEnemy = GetEnemy();

			
			// �e�̓����蔻��
			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{
				CollisionEnemy(&g_aBullet[nCntBullet]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
			{
				CollisionPlayer(&g_aBullet[nCntBullet]);
			}
			
			//// �e�̈ʒu����ʊO�ɏo��
			//if (g_aBullet[nCntBullet].pos.x > SCREEN_WIDTH)
			//{
			//	g_aBullet[nCntBullet].bUse = false;			// �g�p���Ă��Ȃ���Ԃɂ���
			//}
			g_aBullet[nCntBullet].nLife--;
			// �����̃J�E���g�_�E��
			if (g_aBullet[nCntBullet].nLife <= 0)
			{
				 SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
				g_aBullet[nCntBullet].bUse = false;			// �g�p���Ă��Ȃ���Ԃɂ���

				//�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_EXPLOSION);
			}

			// �e�̈ʒu�̍X�V
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			// �J�������W
			D3DXVECTOR3 CameraPos = Screenposition(g_aBullet[nCntBullet].pos);

			SetEffect(CameraPos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 30, g_aBullet[nCntBullet].nLife);

			// ���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(CameraPos.x - 20.0f,
				CameraPos.y - 20.0f, 0.0f);

			pVtx[1].pos = D3DXVECTOR3(CameraPos.x + 20.0f,
				CameraPos.y - 20.0f, 0.0f);

			pVtx[2].pos = D3DXVECTOR3(CameraPos.x - 20.0f,
				CameraPos.y + 20.0f, 0.0f);

			pVtx[3].pos = D3DXVECTOR3(CameraPos.x + 20.0f,
				CameraPos.y + 20.0f, 0.0f);

		}
		pVtx += 4;

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//==============
//�e�̕`�揈��
//==============
void DrawBullet(void)
{
	int nCntBullet;

	LPDIRECT3DDEVICE9 pDevice;					// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBullet);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{// �e���g�p����Ă���
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntBullet * 4,
				2);
		}
	}
}

//===============
// �e�̐ݒ菈��
//===============
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move,int nLife, BULLETTYPE type)
{
	VERTEX_2D* pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{// �e���g�p����ĂȂ�
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].nLife = nLife;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].bUse = true;			// �g�p���Ă����Ԃɂ���

			// �J�������W
			D3DXVECTOR3 CameraPos = Screenposition(g_aBullet[nCntBullet].pos);

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(CameraPos.x - 20.0f, CameraPos.y - 20.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(CameraPos.x + 20.0f, CameraPos.y - 20.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(CameraPos.x - 20.0f, CameraPos.y + 20.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(CameraPos.x + 20.0f, CameraPos.y + 20.0f, 0.0f);

			break;
		}

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

void CollisionEnemy(Bullet* pBullet)
{
	Enemy* pEnemy = GetEnemy();							// �G�̎擾

	int nCntBullet = 0;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{// �G���g�p����Ă���
			if (pEnemy->pos.x - 35.0f <= pBullet->pos.x &&
				pEnemy->pos.x + 35.0f >= pBullet->pos.x &&
				pEnemy->pos.y - 35.0f <= pBullet->pos.y &&
				pEnemy->pos.y + 35.0f >= pBullet->pos.y)
			{
				HitEnemy(nCntEnemy, 1);
				pBullet->bUse = false;
			}
		}
	}
}

void CollisionPlayer(Bullet* pBullet)
{
	Player* pPlayer = GetPlayer();							// �v���C���[�̎擾

	int nCntBullet = 0;

		if (pPlayer->pos.x - 35 <= pBullet->pos.x &&
			pPlayer->pos.x + 35 >= pBullet->pos.x &&
			pPlayer->pos.y - 35 <= pBullet->pos.y &&
			pPlayer->pos.y + 35 >= pBullet->pos.y )
		{

			if (pPlayer->state == PLAYERTYPE_NORMAL)
			{
				HitPlayer(1);
				pBullet->bUse = false;
			}
			else if (pPlayer->state == PLAYERTYPE_UNRIVALED)
			{
				pBullet->bUse = false;
			}
			else if(pPlayer->state == PLAYERTYPE_SUCTION)
			{
				PowerUpPlayer();
				pBullet->bUse = false;
			}
		}
	
	
}

Bullet* GetBullet(void)
{
	return &g_aBullet[0];
}
