//==========================
//
// �G���� [enemy.cpp]
// Author shuuhei Ida
//
//==========================

#include"main.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion .h"
#include"score.h"
#include"sound.h"
#include"camera.h"


// �}�N����`
#define NUM_ENEMY (4)				// �G�̎��
#define ENEMY_SIZE (20.0f)			//�@�G�̃T�C�Y

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureenemy[NUM_ENEMY] = {};	// �e�N�X�`���ւ̃|�C���^

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffenemy = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

Enemy g_aEnemy[MAX_ENEMY];						// �G�̏��

//================
// �G�̏���������
//================
void InitEnemy(void)
{
	int nCntEnemy;

	LPDIRECT3DDEVICE9 pDevice;								// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\4.jpg", &g_apTextureenemy[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\2.jpg", &g_apTextureenemy[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\enemy002.png", &g_apTextureenemy[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\enemy003.png", &g_apTextureenemy[3]);


	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// �G�̏��̏�����
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nType = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nLife = 5;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].nCnterState = 0;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].nScore = 0;

	}
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffenemy,
		NULL);
		
	VERTEX_2D* pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffenemy->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
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
	g_pVtxBuffenemy->Unlock();
		
}

//=================
// �G�̏I������
//=================
void UninitEnemy(void)
{
	int nCount;
	// �e�N�X�`���̔j��
	for (nCount = 0; nCount < NUM_ENEMY; nCount++)
	{
		if (g_apTextureenemy[nCount] != NULL)
		{
			g_apTextureenemy[nCount]->Release();
			g_apTextureenemy[nCount] = NULL;
		}
		
	}
		// ���_�o�b�t�@�̔j��
		if (g_pVtxBuffenemy != NULL)
		{
			g_pVtxBuffenemy->Release();

			g_pVtxBuffenemy = NULL;
		}
}

//==============
// �G�̍X�V����
//==============
void UpdateEnemy()
{
	VERTEX_2D* pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffenemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// �G���g�p����Ă��Ȃ�
		if (!g_aEnemy[nCntEnemy].bUse)
		{
			pVtx += 4;
			continue;
		}

		// �J�������W
		D3DXVECTOR3 Camerapos = Screenposition(g_aEnemy[nCntEnemy].pos);

		switch (g_aEnemy[nCntEnemy].state)
		{
		case ENEMYSTATE_NORMAL:
			g_aEnemy[nCntEnemy].nCnterState--;

			if (g_aEnemy[nCntEnemy].nCnterState <= 0)
			{
				// �G�̐ݒ�
				SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(-2.5f, 0.0f, 0.0f), 150, BULLETTYPE_ENEMY);
				g_aEnemy[nCntEnemy].nCnterState = 120;
			}
			break;
			
		case ENEMYSTATE_DAMAGE:
			g_aEnemy[nCntEnemy].nCnterState--;

			if (g_aEnemy[nCntEnemy].nCnterState <= 0)
			{
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
			}
			break;
		}

		// g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

		// ���_���W�̐ݒ�(x,y,z,�̏��ԂɂȂ�Az�̒l��2D�̏ꍇ�͕K��0�ɂ���)
		pVtx[0].pos = D3DXVECTOR3(Camerapos.x - ENEMY_SIZE, Camerapos.y - ENEMY_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(Camerapos.x + ENEMY_SIZE, Camerapos.y - ENEMY_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(Camerapos.x - ENEMY_SIZE, Camerapos.y + ENEMY_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(Camerapos.x + ENEMY_SIZE, Camerapos.y + ENEMY_SIZE, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffenemy->Unlock();

}

//=============
//�G�̕`�揈��
//=============
void DrawEnemy(void)
{
	int nCntEnemy;

	LPDIRECT3DDEVICE9 pDevice;					// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffenemy, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{// �G���g�p����Ă���
				// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0,g_apTextureenemy[g_aEnemy[nCntEnemy].nType]);
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntEnemy *4,
				2);
		}
	}
}

//===============
// �G�̐ݒ菈��
//===============
void SetEnemy(D3DXVECTOR3 pos, int nType)
{
	int nCntEnemy;

	VERTEX_2D* pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffenemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{// �������g�p����ĂȂ�
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].nCnterState = 0;
			g_aEnemy[nCntEnemy].nCnterState = ENEMYSTATE_NORMAL;
			g_aEnemy[nCntEnemy].nType = nType;
			g_aEnemy[nCntEnemy].nScore = 100;
			g_aEnemy[nCntEnemy].bUse = true;			// �g�p���Ă����Ԃɂ���

			pVtx[0].pos = D3DXVECTOR3(pos.x - ENEMY_SIZE,pos.y - ENEMY_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + ENEMY_SIZE,pos.y - ENEMY_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - ENEMY_SIZE,pos.y + ENEMY_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + ENEMY_SIZE,pos.y + ENEMY_SIZE, 0.0f);

			break;
		}
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffenemy->Unlock();
}

//=============
//�G�̔�e����
//=============
void HitEnemy(int nCntEnemy, int nDamage)
{
	VERTEX_2D* pVtx;		// ���_���ւ̃|�C���^
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffenemy->Lock(0, 0, (void**)&pVtx, 0);
	g_aEnemy[nCntEnemy].nLife -= nDamage;
	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		// �����̐ݒ�
		SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
		g_aEnemy[nCntEnemy].bUse = false;
		AddScore(100);
	}
	else
	{
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCnterState = 5;
		pVtx += (nCntEnemy*4);
		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		// �T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_HIT);

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffenemy->Unlock();
}

//============
//�G�̎擾
//============
Enemy*GetEnemy(void)
{
	return &g_aEnemy[0];	// �G�̏��̐擪�A�h���X��Ԃ�
}

//===============
//�G�̃J�E���g
//===============
int Enemycount(void)
{
	int nCntEnemy;					// 128�̕��̃f�[�^
	int nNoDeadEnemy = 0;			// �����Ă���̂̐�
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			nNoDeadEnemy++;
		}

	}
	return nNoDeadEnemy;

}
