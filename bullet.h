//==========================
//
// 弾処理 [bullet.h]
// Author shuuhei Ida
//
//==========================

#ifndef _BULLET_H_
#define _BULLET_H_
#include "main.h"

// 弾の状態
typedef enum
{
	BULLETTYPE_PLAYER=0,
	BULLETTYPE_ENEMY,
	BULLETTYPE_MAX
}BULLETTYPE;

// 弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	int nLife;
	BULLETTYPE type;
	bool bUse;

}Bullet;

// グローバル変数
void InitBullet(void);

void UninitBullet(void);

void UpdateBullet(void);

void DrawBullet(void);

void SetBullet(D3DXVECTOR3 pos,D3DXVECTOR3 move,int nLife,BULLETTYPE type);

void CollisionEnemy(Bullet* pBullet);

void CollisionPlayer(Bullet* pBullet);

Bullet* GetBullet(void);

#endif // !_BULLET_H_

