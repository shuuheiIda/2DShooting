//==========================
//
//エフェクト処理 [effect.h]
//Author shuuhei Ida
//
//==========================
#ifndef _EFFECT_H_
#define _EFFECT_H_
#include "main.h"


//エフェクト構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR col;
	float fRadius;
	int nLife;
	bool bUse;

}Effect;

//グローバル変数
void InitEffect(void);

void UninitEffect(void);

void UpdateEffect(void);

void DrawEffect(void);

void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col,float fRadius,int nLife);

#endif // !_Effect_H_

