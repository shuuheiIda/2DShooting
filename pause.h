//==========================
//
//�|�[�Y���� [pause.h]
//Author:shuuhei Ida
//
//==========================

#ifndef _PAUSE_H_					//
#define _PAUSE_H_					// ��d�̃C���N���[�h�h�~�̃}�N��

#include "main.h"

typedef enum
{
	PAUSE_CONTINUE =0,
	PAUSE_RETRY,
	PAUSE_QUIT,
	PAUSE_MAX
}PAUSE;

// �v���g�^�C�v�錾
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

#endif // !_PAUSE_H_