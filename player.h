//==========================
//
//プレイヤー処理 [player.h]
//Author shuuhei Ida
//
//==========================

#ifndef _PLAYER_H_					//
#define _PLAYER_H_					//二重のインクルード防止のマクロ

#define PLAYER_SIZEX (150)
#define PLAYER_SIZEY (150)

//プレイヤーの状態
typedef enum
{
	PLAYERTYPE_APPEEAR = 0,			// 出現待ち
	PLAYERTYPE_NORMAL,				// 通常
	PLAYERTYPE_DAMAGE,				// ダメージ
	PLAYERTYPE_WAIT,				// 出現状態
	PLAYERTYPE_DEATH,				// 死亡
	PLAYERTYPE_SUCTION,				// 吸収状態
	PLAYERTYPE_UNRIVALED,			// 無敵状態
	PLAYERTYPE_MAX,
}PLAYERTYPE;

//プレイヤー構造体の定義
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
	int nPowerupCountor;	// 弾のパワーアップのカウンター
	int nSpeedupCounter;	// 自機のスピードアップカウンター
	int nLevel;
	int nStock;

}Player;

//発射する弾の種類
typedef enum
{
	PLAYERBULLET_NORMAL = 0,
	PLAYERBULLET_LEVEL1,
	PLAYERBULLET_LEVEL2,
	PLAYERBULLET_LEVEL3,
	PLAYERBULLET_LEVEL4,
	PLAYERBULLET_MAX,
}PLAYERBULLET;

//プレイヤーのスピードの種類
typedef enum
{
	PLAYERSPEED_NORMAL = 0,
	PLAYERSPEED_LEVEL1,
	PLAYERSPEED_LEVEL2,
	PLAYERSPEED_LEVEL3,
	PLAYERSPEED_LEVEL4,
	PLAYERSPEED_MAX
}PLAYERSPEED;

//プロトタイプ宣言
void InitPlayer(void);

void UninitPlayer(void);

void UpdatePlayer(void);

void DrawPlayer(void);

void HitPlayer(int nDamage);

void SpeedUpPrlayer(void);

void PowerUpPlayer(void);

Player* GetPlayer(void);

#endif // !_PLAYER_H_