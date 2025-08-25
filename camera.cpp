//==========================
//
// �J�������� [casmera.h]
// Author:shuuhei Ida
//
//==========================

#include "camera.h"

D3DXVECTOR3 g_Camerapos;

//==========================
// �J�����̏���������
//==========================
void InitCamera(void)
{
	 g_Camerapos ={ (float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f ,0.0f}; 
}
//==========================
// �J�����̍X�V����
//==========================
void UpdateCamera(D3DXVECTOR3 pos)
{
	g_Camerapos = pos;


	if (g_Camerapos.x - (float)SCREEN_WIDTH * 0.5f <= 0.0f)		//��ʍ��[�ɍs�����Ƃ�
	{
		g_Camerapos.x = (float)SCREEN_WIDTH * 0.5f;
	}
	else if (g_Camerapos.x + (float)SCREEN_WIDTH * 0.5f >= WORLD_WIDTH)		//��ʉE�[�ɍs�����Ƃ�
	{
		g_Camerapos.x = WORLD_WIDTH - (float)SCREEN_WIDTH * 0.5f;
	}
	
	if (g_Camerapos.y - (float)SCREEN_HEIGHT * 0.5f <= 0.0f)
	{
		g_Camerapos.y = (float)SCREEN_HEIGHT * 0.5f;
	}
	else if (g_Camerapos.y + (float)SCREEN_HEIGHT * 0.5f >= WORLD_HEIGHT)
	{
		g_Camerapos.y = WORLD_HEIGHT - (float)SCREEN_HEIGHT * 0.5f;
	}
}
//==========================
// �J�������W�ϊ�����
//==========================
D3DXVECTOR3 Screenposition(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 screen_origin_position = D3DXVECTOR3(g_Camerapos.x - (float)SCREEN_WIDTH * 0.5f, g_Camerapos.y - (float)SCREEN_HEIGHT * 0.5f, 0.0f);

	// ���[���h���W����X�N���[�����W�ɕϊ�����
	D3DXVECTOR3 screen_position = D3DXVECTOR3(pos.x - screen_origin_position.x, pos.y - screen_origin_position.y, 0.0f);

	return screen_position;
}


