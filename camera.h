//==========================
//
// �J�������� [casmera.h]
// Author:shuuhei Ida
//
//==========================

#ifndef _CAMERA_H_					//
#define _CAMERA_H_					// ��d�̃C���N���[�h�h�~�̃}�N��4

#include "main.h"

#define WORLD_WIDTH	(2560)
#define WORLD_HEIGHT (720)

// �v���g�^�C�v�錾
void InitCamera(void);
void UpdateCamera(D3DXVECTOR3 pos);
D3DXVECTOR3 Screenposition(D3DXVECTOR3 pos);

#endif // !_CAMERA_H_
