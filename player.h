//==========================
//
//�v���C���[���� [player.h]
//Author shuuhei Ida
//
//==========================

#ifndef _PLAYER_H_					//
#define _PLAYER_H_					//��d�̃C���N���[�h�h�~�̃}�N��

#define PLAYER_SIZEX (150)
#define PLAYER_SIZEY (150)

//�v���C���[�̏��
typedef enum
{
	PLAYERTYPE_APPEEAR = 0,			// �o���҂�
	PLAYERTYPE_NORMAL,				// �ʏ�
	PLAYERTYPE_DAMAGE,				// �_���[�W
	PLAYERTYPE_WAIT,				// �o�����
	PLAYERTYPE_DEATH,				// ���S
	PLAYERTYPE_SUCTION,				// �z�����
	PLAYERTYPE_UNRIVALED,			// ���G���
	PLAYERTYPE_MAX,
}PLAYERTYPE;

//�v���C���[�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posold;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	int nType;
	int nLife;
	int nCnterState;
	int nUnrivaledCnter;
	bool bDisp;
	PLAYERTYPE state;
	int nSpeed;
	int nPowerupCountor;	// �e�̃p���[�A�b�v�̃J�E���^�[
	int nSpeedupCounter;	// ���@�̃X�s�[�h�A�b�v�J�E���^�[
	int nLevel;
	int nStock;

}Player;

//���˂���e�̎��
typedef enum
{
	PLAYERBULLET_NORMAL = 0,
	PLAYERBULLET_LEVEL1,
	PLAYERBULLET_LEVEL2,
	PLAYERBULLET_LEVEL3,
	PLAYERBULLET_LEVEL4,
	PLAYERBULLET_MAX,
}PLAYERBULLET;

//�v���C���[�̃X�s�[�h�̎��
typedef enum
{
	PLAYERSPEED_NORMAL = 0,
	PLAYERSPEED_LEVEL1,
	PLAYERSPEED_LEVEL2,
	PLAYERSPEED_LEVEL3,
	PLAYERSPEED_LEVEL4,
	PLAYERSPEED_MAX
}PLAYERSPEED;

//�v���g�^�C�v�錾
void InitPlayer(void);

void UninitPlayer(void);

void UpdatePlayer(void);

void DrawPlayer(void);

void HitPlayer(int nDamage);

void SpeedUpPrlayer(void);

void PowerUpPlayer(void);

Player* GetPlayer(void);

#endif // !_PLAYER_H_