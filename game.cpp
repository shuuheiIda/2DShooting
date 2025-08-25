//==========================
//
// �V���[�e�B���O�Q�[������
// Author shuuhei Ida
//
//==========================
#include "game.h"
#include"player.h"
#include"bg.h"
#include "input.h"
#include"bullet.h"
#include "explosion .h"
#include"enemy.h"
#include"score.h"
#include"fade.h"
#include"effect.h"
#include"sound.h"
#include"pause.h"
#include"camera.h"
#include"time.h"

bool g_bPause = false;		// �|�[�Y��Ԃ̃I��/�I�t

//===================
// �Q�[���̏���������
//===================
void InitGame(void)
{

	InitCamera();

	InitBG();
	
	InitExplosion();
	
	InitBullet();
	
	InitPlayer();
	
	InitScore();

	InitEffect();

	// �G�̏���������
	InitEnemy();
	SetEnemy(D3DXVECTOR3(1000.0f, 150.0f, 0.0f), 0);
	SetEnemy(D3DXVECTOR3(1000.0f, 250.0f, 0.0f), 1);
	SetEnemy(D3DXVECTOR3(1000.0f, 350.0f, 0.0f), 2);
	SetEnemy(D3DXVECTOR3(1000.0f, 450.0f, 0.0f), 3);
	SetScore(0);
	InitTime();

	// �T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM001);

	InitPause();

	g_bPause = false;
}

//==================
// �Q�[���̏I������
//==================
void UninitGame(void)
{
	StopSound();

	UninitBG();
	
	UninitPlayer();
	
	UninitExplosion();
	
	UninitEnemy();
	
	UninitScore();

	UninitTime();

	UninitEffect();

	UninitPause();


}

//=================
// �Q�[���̍X�V����
//=================
void UpdateGame(void)
{
	Player* pPlayer = GetPlayer();

	int nNoDeadenemy;
	// �G�̎擾
	nNoDeadenemy = Enemycount();

	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = (g_bPause) ? false : true;
	}

	if (g_bPause == false)
	{
		UpdateCamera(pPlayer->pos);

		UpdateBG();
	
		UpdateBullet();
	
		UpdatePlayer();
	
		UpdateExplosion();
	
		UpdateEnemy();

		UpdateEffect();

		UpdateTime();

	}
	else
	{
		// �|�[�Y�̂ݍX�V
		UpdatePause();
	}

	if (nNoDeadenemy == 0)
	{
		SetFade(MODE_RESULT);
	}

	//if (GetKeyboardTrigger(DIK_RETURN) == true)
	//{// ����L�[�������ꂽ
	//	// ���[�h�ݒ�(�Q�[����ʂɈڍs)
	//	SetFade(MODE_RESULT);
	//}

}

//==================
// �Q�[���̕`�ʏ���
//==================
void DrawGame(void)
{
	DrawBG();

	DrawPlayer();

	DrawBullet();

	DrawExplosion();

	DrawEnemy();

	DrawScore();

	DrawTime();

	DrawEffect();
	
	if (g_bPause == true)
	{
		// �|�[�Y��`��
		DrawPause();
	}
}

//==================
// �|�[�Y�̐ݒ�
//==================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}
