//============================
//
// �v���C���[���� [player.cpp]
// Author shuuhei Ida
//
//============================
#include"main.h"
#include"player.h"
#include"input.h"
#include"bullet.h"
#include "explosion .h"
#include"sound.h"
#include"enemy.h"
#include"camera.h"
#include"fade.h"

//�}�N����`
#define MAX_SPEED	(5)										//�ő�ړ����x
#define BULLET_LIFE (150.0f)								//�e�̎���
#define BULLET_LINE (2.5f)									//�e�̔��˕���

// �O���[�o���ϐ�

LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;					// �e�N�X�`���ւ̃|�C���^

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;			// ���_�o�b�t�@�ւ̃|�C���^

D3DXVECTOR3 g_rotPlayer;									// ����

Player g_Player;

int g_nCounterAnimPlayer;									// �A�j���[�V�����J�E���^�[
int g_nPatternAnimPlayer;									// �A�j���[�V�����p�^�[��No
int g_nBulletlvel;											// �e�̃��x��
int g_nSpeedlvel;											// �X�s�[�h�̃��x��
float g_fLengthPlayer;										// �Ίp���̒���
float g_fAnglePlayer;										// �Ίp���̊p�x


//========================
//�v���C���[�̏���������
//========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;								// �f�o�C�X�ւ̃|�C���^

	g_Player.pos = D3DXVECTOR3(640.0f,350.0f,0.0f);			// �ʒu������������

	g_Player.move = D3DXVECTOR3(0.0f,0.0f,0.0f);			// �ړ��ʂ�������

	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//

	g_nCounterAnimPlayer = 0;								// �J�E���^�[������������

	g_nPatternAnimPlayer = 0;								// �p�^�[��No.������������

	g_rotPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����������������(���l���g�p)

	g_Player.nLife = 1 ;									// ���C�t�̏�����

	g_Player.nSpeed = 1;									// ���x�̏�����

	g_Player.bDisp = true;
	
	g_Player.nPowerupCountor = 0;

	g_Player.nSpeedupCounter = 0;

	g_nBulletlvel = 0;

	g_Player.nStock = 3;									//�c�@�̏�����

	g_Player.posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Player.nUnrivaledCnter = 0;

	// �Ίp���̒������Z�o����
	g_fLengthPlayer = sqrtf(100 * 100 + 100 * 100) * 0.5f;

	// �Ίp���̊p�x���Z�o����
	g_fAnglePlayer = atan2f(100, 100);

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\player000.png", &g_pTexturePlayer);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D* pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0,(void**)&pVtx, 0);

	// ���_���W�̐ݒ�(x,y,z,�̏��ԂɂȂ�Az�̒l��2D�̏ꍇ�͕K��0�ɂ���)
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_Player.pos.y + cosf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x =g_Player.pos.x - sinf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = g_Player.pos.y - cosf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x =g_Player.pos.x + sinf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x =g_Player.pos.x + sinf(g_rotPlayer.z + g_fAnglePlayer) *  g_fLengthPlayer;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_rotPlayer.z + g_fAnglePlayer) *g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//==========================
// �v���C���[�̏I������
//==========================
void UninitPlayer(void)
{
	// �e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();

		g_pVtxBuffPlayer = NULL;
	}

}

//========================
// �v���C���[�̍X�V����
//========================
void UpdatePlayer(void)
{

	VERTEX_2D* pVtx;		// ���_���ւ̃|�C���^

	int nCntPlayer = 0;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_Player.state)
	{
	case PLAYERTYPE_NORMAL:

			break;

	case PLAYERTYPE_DAMAGE:
	
		g_Player.nCnterState--;
		if (g_Player.nCnterState <= 0)
		{
			g_Player.state = PLAYERTYPE_NORMAL;
		}
		break;
		
	case PLAYERTYPE_DEATH:

		g_Player.nStock--;
		if (g_Player.nStock <= 0)
		{
			SetFade(MODE_RESULT);
		}

		if (g_Player.nLife <= 0)
		{
			g_Player.bDisp = false;
			g_Player.state = PLAYERTYPE_WAIT;
		}
		break;
	
	case PLAYERTYPE_WAIT:
	
		g_Player.nCnterState--;
		if (g_Player.nCnterState <= 0)
		{
			g_Player.state = PLAYERTYPE_APPEEAR;
		}
		break;
	
	case PLAYERTYPE_APPEEAR:
	
		g_Player.nCnterState--;
		if (g_Player.nCnterState <= 0)
		{
			g_Player.bDisp = true;
			g_Player.nLife = 1;
			g_Player.state = PLAYERTYPE_UNRIVALED;
			g_Player.nUnrivaledCnter = 300;
			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
			

		}
		break;

	case PLAYERTYPE_UNRIVALED:
		
		g_Player.nUnrivaledCnter--;
		if (g_Player.nUnrivaledCnter >= 0)
		{
			g_Player.bDisp = g_Player.bDisp ^ 1;
		}
		if(g_Player.nUnrivaledCnter <=0)
		{
			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_Player.state = PLAYERTYPE_NORMAL;
		}
		break;

	default:
		break;
	}

	if (GetKeyboardTrigger(DIK_B) == true || GetJoypadTrigger(JOYKEY_Y) == true)		//�p���[�A�b�v�̕��@
	{
		g_Player.state = PLAYERTYPE_SUCTION;
	}
	else if (GetKeyboardTrigger(DIK_N) == true  || GetJoypadTrigger(JOYKEY_X) == true)
	{
		g_Player.state = PLAYERTYPE_NORMAL;
	}

	// �ʒu���X�V
	if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
	{
		if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
		{// ����ړ�
			g_Player.move.x += sinf(D3DX_PI * 0.75f)*-g_Player.nSpeed ;
			g_Player.move.y += cosf(D3DX_PI * 0.75f)* g_Player.nSpeed ;
			// �ړ��ʂ��X�V
		}
		else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
		{// �����ړ�
			g_Player.move.x += sinf(D3DX_PI * 0.25f)* -g_Player.nSpeed ;
			g_Player.move.y += cosf(D3DX_PI * 0.25f) * g_Player.nSpeed ;
		}
		else
		{// A�L�[�������ꂽ
			g_Player.move.x += sinf(D3DX_PI * 0.5f) * -g_Player.nSpeed ;
			// �ړ��ʂ��X�V
			//g_Player.move.x += -1;
				// ���_���W���X�V
		}
	
	}
	
	else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
	{
			if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
			{// �E��ړ�
				g_Player.move.x += sinf(D3DX_PI * 0.75f)*g_Player.nSpeed ;
				g_Player.move.y += cosf(D3DX_PI * 0.75f)*g_Player.nSpeed ;
			}
			else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
			{// �E���ړ�
				g_Player.move.x += sinf(D3DX_PI * 0.25f) * g_Player.nSpeed ;
				g_Player.move.y += cosf(D3DX_PI * 0.25f)* g_Player.nSpeed ;
			}
			else
			{// D�L�[�������ꂽ
				g_Player.move.x += sinf(D3DX_PI * 0.5f) * g_Player.nSpeed ;				//���_���W���X�V
			}
	}
	
	else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
	{// W�L�[�������ꂽ
		
		if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
		{
			// A�L�[�������ꂽ
				g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -g_Player.nSpeed ;
				// �ړ��ʂ��X�V
				// g_Player.move.x += -1;
				// ���_���W���X�V
			
		}
		else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
		{// �E���ړ�
			g_Player.move.x += sinf(D3DX_PI * 0.25f) * g_Player.nSpeed ;
			g_Player.move.y += cosf(D3DX_PI * 0.25f) * -g_Player.nSpeed ;
		}
		else
		{
			g_Player.move.y += cosf(D3DX_PI * 1.0f) * g_Player.nSpeed ;		//���_���W���X�V
		}
	}

	else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
	{// S�L�[�������ꂽ
		if (GetKeyboardPress(DIK_A) == true)
		{// A�L�[�������ꂽ
			g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -g_Player.nSpeed ;
			// �ړ��ʂ��X�V
		
		}
		else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
		{// �E���ړ�
			g_Player.move.x += sinf(D3DX_PI * 0.25f) * g_Player.nSpeed ;
			g_Player.move.y += cosf(D3DX_PI * 0.25f) * g_Player.nSpeed ;
		}
		else
		{
			g_Player.move.y += cosf(-D3DX_PI * 1.0f) * -g_Player.nSpeed ;					//���_���W���X�V
		}
		
	}

	if (g_Player.state != PLAYERTYPE_DEATH || g_Player.state != PLAYERTYPE_APPEEAR || g_Player.state != PLAYERTYPE_WAIT || g_Player.state != PLAYERTYPE_UNRIVALED)
	{
		if (GetKeyboardTrigger(DIK_SPACE) == true || GetJoypadTrigger(JOYKEY_A) == true)
		{// �X�y�[�X�L�[�������ꂽ

			g_Player.state = PLAYERTYPE_NORMAL;
			// ���݂̃��x���ɉ����Ēe�̏o������ύX����
			switch (g_nBulletlvel)
			{
			case PLAYERBULLET_NORMAL:
				// �e�̐ݒ菈��
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 0.0f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				// �T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_SHOT);
				break;

			case PLAYERBULLET_LEVEL1:
				// �e�̐ݒ菈��
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 0.0f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 2.5f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				// �T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_SHOT);
				break;

			case PLAYERBULLET_LEVEL2:
				// �e�̐ݒ菈��
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 0.0f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 2.5f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, -2.5f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				// �T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_SHOT);
				break;

			case PLAYERBULLET_LEVEL3:
				// �e�̐ݒ菈��
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 0.0f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 2.5f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, -2.5f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 5.0f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				// �T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_SHOT);
				break;

			case PLAYERBULLET_LEVEL4:
				// �e�̐ݒ菈��
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 0.0f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 2.5f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, -2.5f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, 5.0f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(BULLET_LINE, -5.0f, 0.0f), BULLET_LIFE, BULLETTYPE_PLAYER);
				// �T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_SHOT);
				break;

			}
		}
	}
	

	g_Player.posold.x = g_Player.pos.x;
	g_Player.posold.y = g_Player.pos.y;

	// �ʒu���X�V
	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.y += g_Player.move.y;


	// �ړ��ʂ��X�V
	g_Player.move.x += (0.0f - g_Player.move.x) * 0.1f;
	g_Player.move.y += (0.0f - g_Player.move.y) * 0.1f;

	// �J�������W
	D3DXVECTOR3 CameraPos = Screenposition(g_Player.pos);

	// �ړ�����
	
	if (g_Player.pos.x >= 2560.0f)
	{
		g_Player.pos.x = 2560.0f;
	}
	else if (g_Player.pos.x <=30.0f)
	{
		g_Player.pos.x = 30.0f;
	}

	if (g_Player.pos.y <= 50)
	{
		g_Player.pos.y = 50;
	}
	else if (g_Player.pos.y >= 670)
	{
		g_Player.pos.y = 670;
	}
	//else if (g_Player.pos.y >= SCREEN_HEIGHT)
	//{
	//	g_Player.pos.y = 0;
	//}
	//else if (g_Player.pos.y <= 0)
	//{
	//	g_Player.pos.y = SCREEN_HEIGHT;
	//}

	// ���_���W�̐ݒ�(x,y,z,�̏��ԂɂȂ�Az�̒l��2D�̏ꍇ�͕K��0�ɂ���)
	pVtx[0].pos.x = CameraPos.x + sinf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = CameraPos.y + cosf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = CameraPos.x + sinf(g_rotPlayer.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = CameraPos.y + cosf(g_rotPlayer.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;


	pVtx[2].pos.x =CameraPos.x + sinf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = CameraPos.y + cosf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x =CameraPos.x + sinf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = CameraPos.y + cosf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();

	SpeedUpPrlayer();
}

//=======================
// �v���C���[�̕`�揈��
//=======================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;					// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePlayer);

	if (g_Player.bDisp != false)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
	}
	
}

//========================
// �v���C���[�̓����蔻��
//========================
void HitPlayer(int nDamage)
{
	VERTEX_2D* pVtx;		// ���_���ւ̃|�C���^
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);
	g_Player.nLife -= nDamage;
	if (g_Player.nLife <= 0)
	{
		// �����̐ݒ�
		SetExplosion(g_Player.pos,D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
		g_Player.nCnterState = 120;
		g_Player.state = PLAYERTYPE_DEATH;
		g_Player.bDisp = false;

	}
	else
	{
		g_Player.state = PLAYERTYPE_DAMAGE;
		g_Player.nCnterState = 5;
		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		SetVibration(50000, 50000, 100);

		// �T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_HIT);

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//====================
// �X�s�[�h�A�b�v����
//====================
void SpeedUpPrlayer(void)
{
#if 0
	Bullet* pBullet = GetBullet();

	for (int nCnt = 0; nCnt < 128; nCnt++)
	{
		if (g_Player.posold.x - 35.0f > pBullet[nCnt].pos.x ||
			g_Player.posold.x + 35.0f < pBullet[nCnt].pos.x ||
			g_Player.posold.y - 35.0f > pBullet[nCnt].pos.y ||
			g_Player.posold.y + 35.0f < pBullet[nCnt].pos.y)
		{
			if (g_Player.nSpeed <= PLAYERSPEED_LEVEL4)
			{
				g_Player.nSpeedupCounter++;
				if (g_Player.nSpeedupCounter >= 10)
				{
					if (g_nSpeedlvel == PLAYERSPEED_NORMAL)
					{
						g_nSpeedlvel = PLAYERSPEED_LEVEL1;

						if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
						{
							if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
							{// ����ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.75f) * -1.0f;
								g_Player.move.y += cosf(D3DX_PI * 0.75f) * 1.0f;
								// �ړ��ʂ��X�V
							}
							else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
							{// �����ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * -1.0f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.0f;
							}
							else
							{// A�L�[�������ꂽ
								g_Player.move.x += sinf(D3DX_PI * 0.5f) * -1.0f;
								// �ړ��ʂ��X�V
								g_Player.move.x += -1;
								// ���_���W���X�V
							}

						}

						else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
						{
							if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
							{// �E��ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.75f) * 1.0f;
								g_Player.move.y += cosf(D3DX_PI * 0.75f) * 1.0f;
							}
							else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
							{// �E���ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.0f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.0f;
							}
							else
							{// D�L�[�������ꂽ
								g_Player.move.x += sinf(D3DX_PI * 0.5f) * 1.0f;				//���_���W���X�V
							}
						}

						else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
						{// W�L�[�������ꂽ

							if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
							{
								// A�L�[�������ꂽ
								g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -1.0f;
								// �ړ��ʂ��X�V
								g_Player.move.x += -1;
								// ���_���W���X�V

							}
							else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
							{// �E���ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.0f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * -1.0f;
							}
							else
							{
								g_Player.move.y += cosf(D3DX_PI * 1.0f) * 1.0f;		//���_���W���X�V
							}
						}

						else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
						{// S�L�[�������ꂽ
							if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
							{// A�L�[�������ꂽ
								g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -1.0f;
								// �ړ��ʂ��X�V

							}
							else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
							{// �E���ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.0f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.0f;
							}
							else
							{
								g_Player.move.y += cosf(-D3DX_PI * 1.0f) * -1.0f;					//���_���W���X�V
							}

						}

						/*g_nBulletlvel -= 10;
						if (g_nBulletlvel == PLAYERBULLET_NORMAL)
						{
							g_nBulletlvel = PLAYERBULLET_NORMAL;
						}*/
						g_nSpeedlvel = 0;
					}

					if (g_nSpeedlvel == PLAYERSPEED_LEVEL1)
					{
						g_nSpeedlvel = PLAYERSPEED_LEVEL2;

						if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
						{
							if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
							{// ����ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.75f) * -1.25f;
								g_Player.move.y += cosf(D3DX_PI * 0.75f) * 1.25f;
								// �ړ��ʂ��X�V
							}
							else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
							{// �����ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * -1.25f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.25f;
							}
							else
							{// A�L�[�������ꂽ
								g_Player.move.x += sinf(D3DX_PI * 0.5f) * -1.25f;
								// �ړ��ʂ��X�V
								g_Player.move.x += -1;
								// ���_���W���X�V
							}

						}

						else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
						{
							if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
							{// �E��ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.75f) * 1.25f;
								g_Player.move.y += cosf(D3DX_PI * 0.75f) * 1.25f;
							}
							else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
							{// �E���ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.25f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.25f;
							}
							else
							{// D�L�[�������ꂽ
								g_Player.move.x += sinf(D3DX_PI * 0.5f) * 1.25f;				//���_���W���X�V
							}
						}

						else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
						{// W�L�[�������ꂽ

							if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
							{
								// A�L�[�������ꂽ
								g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -1.25f;
								// �ړ��ʂ��X�V
								g_Player.move.x += -1;
								// ���_���W���X�V

							}
							else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
							{// �E���ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.25f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * -1.25f;
							}
							else
							{
								g_Player.move.y += cosf(D3DX_PI * 1.0f) * 1.25f;		//���_���W���X�V
							}
						}

						else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
						{// S�L�[�������ꂽ
							if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
							{// A�L�[�������ꂽ
								g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -1.25f;
								// �ړ��ʂ��X�V

							}
							else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
							{// �E���ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.25f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.25f;
							}
							else
							{
								g_Player.move.y += cosf(-D3DX_PI * 1.0f) * -1.25f;					//���_���W���X�V
							}

						}

						/*	g_nBulletlvel -= 10;
							if (g_nBulletlvel == PLAYERBULLET_NORMAL)
							{
								g_nBulletlvel = PLAYERBULLET_NORMAL;
							}*/
						g_nSpeedlvel = 0;
					}

					if (g_nSpeedlvel == PLAYERSPEED_LEVEL2)
					{
						g_nSpeedlvel = PLAYERSPEED_LEVEL3;

						if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
						{
							if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
							{// ����ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.75f) * -1.5f;
								g_Player.move.y += cosf(D3DX_PI * 0.75f) * 1.5f;
								// �ړ��ʂ��X�V
							}
							else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
							{// �����ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * -1.5f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.5f;
							}
							else
							{// A�L�[�������ꂽ
								g_Player.move.x += sinf(D3DX_PI * 0.5f) * -1.5f;
								// �ړ��ʂ��X�V
								g_Player.move.x += -1;
								// ���_���W���X�V
							}

						}

						else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
						{
							if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
							{// �E��ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.75f) * 1.5f;
								g_Player.move.y += cosf(D3DX_PI * 0.75f) * 1.5f;
							}
							else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
							{// �E���ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.5f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.5f;
							}
							else
							{// D�L�[�������ꂽ
								g_Player.move.x += sinf(D3DX_PI * 0.5f) * 1.5f;				//���_���W���X�V
							}
						}

						else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
						{// W�L�[�������ꂽ

							if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
							{
								// A�L�[�������ꂽ
								g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -1.5f;
								// �ړ��ʂ��X�V
								g_Player.move.x += -1;
								// ���_���W���X�V

							}
							else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
							{// �E���ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.5f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * -1.5f;
							}
							else
							{
								g_Player.move.y += cosf(D3DX_PI * 1.0f) * 1.5f;		//���_���W���X�V
							}
						}

						else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
						{// S�L�[�������ꂽ
							if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
							{// A�L�[�������ꂽ
								g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -1.5f;
								// �ړ��ʂ��X�V

							}
							else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
							{// �E���ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.5f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.5f;
							}
							else
							{
								g_Player.move.y += cosf(-D3DX_PI * 1.0f) * -1.5f;					//���_���W���X�V
							}

						}

						/*g_nBulletlvel -= 10;
						if (g_nBulletlvel == PLAYERBULLET_NORMAL)
						{
							g_nBulletlvel = PLAYERBULLET_NORMAL;
						}*/
						g_nSpeedlvel = 0;
					}

					if (g_nSpeedlvel == PLAYERSPEED_LEVEL3)
					{
						g_nSpeedlvel = PLAYERSPEED_LEVEL4;

						if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
						{
							if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
							{// ����ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.75f) * -1.75f;
								g_Player.move.y += cosf(D3DX_PI * 0.75f) * 1.75f;
								// �ړ��ʂ��X�V
							}
							else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
							{// �����ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * -1.75f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.75f;
							}
							else
							{// A�L�[�������ꂽ
								g_Player.move.x += sinf(D3DX_PI * 0.5f) * -1.75f;
								// �ړ��ʂ��X�V
								g_Player.move.x += -1;
								// ���_���W���X�V
							}

						}

						else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
						{
							if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
							{// �E��ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.75f) * 1.75f;
								g_Player.move.y += cosf(D3DX_PI * 0.75f) * 1.75f;
							}
							else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
							{// �E���ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.75f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.75f;
							}
							else
							{// D�L�[�������ꂽ
								g_Player.move.x += sinf(D3DX_PI * 0.5f) * 1.75f;				//���_���W���X�V
							}
						}

						else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || Stickmove(JOYKEY_L_UPSTICK) == true)
						{// W�L�[�������ꂽ

							if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
							{
								// A�L�[�������ꂽ
								g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -1.75f;
								// �ړ��ʂ��X�V
								g_Player.move.x += -1;
								// ���_���W���X�V

							}
							else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
							{// �E���ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.75f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * -1.75f;
							}
							else
							{
								g_Player.move.y += cosf(D3DX_PI * 1.0f) * 1.75f;		//���_���W���X�V
							}
						}

						else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || Stickmove(JOYKEY_L_DOWNSTICK) == true)
						{// S�L�[�������ꂽ
							if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || Stickmove(JOYKEY_L_LITESTICK) == true)
							{// A�L�[�������ꂽ
								g_Player.move.x += sinf(-D3DX_PI * 0.5f) * -1.75f;
								// �ړ��ʂ��X�V

							}
							else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || Stickmove(JOYKEY_L_RIGHTSTICK) == true)
							{// �E���ړ�
								g_Player.move.x += sinf(D3DX_PI * 0.25f) * 1.75f;
								g_Player.move.y += cosf(D3DX_PI * 0.25f) * 1.75f;
							}
							else
							{
								g_Player.move.y += cosf(-D3DX_PI * 1.0f) * -1.75f;					//���_���W���X�V
							}

						}

						/*g_nBulletlvel -= 10;
						if (g_nBulletlvel == PLAYERBULLET_NORMAL)
						{
							g_nBulletlvel = PLAYERBULLET_NORMAL;
						}*/
						g_nSpeedlvel = 0;
					}
				}
			}
		}
	

	}
#endif
}

//==================
//�p���[�A�b�v����
//==================
void PowerUpPlayer(void)
{
	Enemy* pEnemy = GetEnemy();							// �G�̎擾
	Bullet* pBullet = GetBullet();

	if (g_Player.state == PLAYERTYPE_SUCTION)		//�p���[�A�b�v�̕��@
	{
		if (pBullet->type == BULLETTYPE_PLAYER) return;

		for (int nCnt = 0; nCnt < 128; nCnt++)
		{
			// �e�̍��W�Ɏ��@���d�Ȃ���
			if (g_Player.pos.x - 35.0f <= pBullet[nCnt].pos.x &&
				g_Player.pos.x + 35.0f >= pBullet[nCnt].pos.x &&
				g_Player.pos.y - 35.0f <= pBullet[nCnt].pos.y &&
				g_Player.pos.y + 35.0f >= pBullet[nCnt].pos.y)
			{
				g_Player.nPowerupCountor++;

				if (g_Player.nPowerupCountor == 10)
				{
					if (g_nBulletlvel == PLAYERBULLET_NORMAL)
					{
						g_nBulletlvel = PLAYERBULLET_LEVEL1;

						// �T�E���h�̍Đ�
						PlaySound(SOUND_LABEL_SE_SHOT);

						g_nSpeedlvel -= 10;
						if (g_nSpeedlvel == PLAYERSPEED_NORMAL)
						{
							g_nSpeedlvel = PLAYERSPEED_NORMAL;
						}
						g_Player.nPowerupCountor = 0;

					}
					else if (g_nBulletlvel == PLAYERBULLET_LEVEL1)
					{
						g_nBulletlvel = PLAYERBULLET_LEVEL2;

						// �T�E���h�̍Đ�
						PlaySound(SOUND_LABEL_SE_SHOT);

						g_nSpeedlvel -= 10;
						if (g_nSpeedlvel == PLAYERSPEED_NORMAL)
						{
							g_nSpeedlvel = PLAYERSPEED_NORMAL;
						}
						g_Player.nPowerupCountor = 0;

					}
					else if (g_nBulletlvel == PLAYERBULLET_LEVEL2)
					{
						g_nBulletlvel = PLAYERBULLET_LEVEL3;

						// �T�E���h�̍Đ�
						PlaySound(SOUND_LABEL_SE_SHOT);

						g_nSpeedlvel -= 10;
						if (g_nSpeedlvel == PLAYERSPEED_NORMAL)
						{
							g_nSpeedlvel = PLAYERSPEED_NORMAL;
						}
						g_Player.nPowerupCountor = 0;

					}
					else if (g_nBulletlvel == PLAYERBULLET_LEVEL3)
					{
						g_nBulletlvel = PLAYERBULLET_LEVEL4;

						// �T�E���h�̍Đ�
						PlaySound(SOUND_LABEL_SE_SHOT);

						g_nSpeedlvel -= 10;
						if (g_nSpeedlvel == PLAYERSPEED_NORMAL)
						{
							g_nSpeedlvel = PLAYERSPEED_NORMAL;
						}
						g_Player.nPowerupCountor = 0;
					}

					if (g_nBulletlvel >= PLAYERBULLET_LEVEL4)
					{
						g_nBulletlvel = PLAYERBULLET_LEVEL4;
					}

				}
			}
		}
	}

}

//========================
// �v���C���[�̎擾
//========================
Player* GetPlayer(void)
{
	return &g_Player;
}