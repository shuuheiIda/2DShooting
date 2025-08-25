//==========================
//
//�G���� [enemy.h]
//Author shuuhei Ida
//
//==========================
#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"

//�}�N����`
#define MAX_ENEMY (128)		// �G�̍ő吔

//�G�̏��
typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_MAX,
}ENEMYSTATE;

// �G�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;
	int nType;
	int nLife;
	ENEMYSTATE state;
	int nCnterState;
	bool bUse;
	int nScore;

}Enemy;

// �v���g�^�C�v�錾
void InitEnemy(void);

void UninitEnemy(void);

void UpdateEnemy(void);

void DrawEnemy(void);

void SetEnemy(D3DXVECTOR3 pos, int nType);

void HitEnemy(int nCntEnemy, int nDamage);

Enemy* GetEnemy(void);

int Enemycount(void);

#endif // !_ENEMY_H_

