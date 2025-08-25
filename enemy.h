//==========================
//
//敵処理 [enemy.h]
//Author shuuhei Ida
//
//==========================
#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"

//マクロ定義
#define MAX_ENEMY (128)		// 敵の最大数

//敵の状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_MAX,
}ENEMYSTATE;

// 敵構造体の定義
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

// プロトタイプ宣言
void InitEnemy(void);

void UninitEnemy(void);

void UpdateEnemy(void);

void DrawEnemy(void);

void SetEnemy(D3DXVECTOR3 pos, int nType);

void HitEnemy(int nCntEnemy, int nDamage);

Enemy* GetEnemy(void);

int Enemycount(void);

#endif // !_ENEMY_H_

