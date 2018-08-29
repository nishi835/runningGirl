/*==============================================================================

    �p�[�e�B�N���̃w�b�_ [particle.h]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/5/31
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/
#ifndef _HITEFFECT_H_
#define _HITEFFECT_H_

/*------------------------------------------------------------------------------
  �@�w�b�_�t�@�C��
------------------------------------------------------------------------------*/
#include "main.h"

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define HITEFFECT_WIDTH			 (4.0f)											// �|���S���̕�
#define HITEFFECT_HEIGHT		 (2.0f)										// �|���S���̍���
#define HITEFFECT_RADIUS		 (70.0f)										// �G�t�F�N�g�̕��˔��a
#define HITEFFECT_SPEED          (2.0f)											// �p�[�e�B�N���̊����
#define HITEFFECT_LIFE           (60)											// �p�[�e�B�N���̊����
#define HITEFFECT_RADIUS_VALUE   (HITEFFECT_RADIUS / ( HITEFFECT_LIFE * 0.2f ))	// �G�t�F�N�g�̕��˔��a�̕ω���
#define HITEFFECT_MAX			 (72)											// �G�t�F�N�g�̍ő吔

/*------------------------------------------------------------------------------
  �@�\���̂̐錾
------------------------------------------------------------------------------*/
// �G�t�F�N�g�̍\����
typedef struct
{
	D3DXVECTOR2	fPos;			// ���W
	D3DXVECTOR2	fAxis;			// ���˂̒��S���W
	D3DXVECTOR2	fSize;			// ��T�C�Y
	D3DXVECTOR2	fVelocity;		// ���x
	D3DXVECTOR2 fLeftUp;		// ����̒��_
	D3DXVECTOR2 fRightUp;		// �E��̒��_
	D3DXVECTOR2 fLeftDown;		// �����̒��_
	D3DXVECTOR2 fRightDown;		// �E���̒��_
	D3DXCOLOR	fColor;			// �t�F�[�h����F
	D3DXCOLOR	fColorValue;	// �F�̕ω���
	float		fRot;			// ��]��(���W�A���p)
	float       fRadius;        // ���˂̒��S���W����̔��a
	float		fLength;		// ���a
	float		fAngle;			// �Ίp���̊p�x
	float		fScale;			// �傫���̔{��
	float		fSpeed;			// �����i�X�J���[�j
	int			nLife;			// �̗�
	bool		bUse;			// �g�p�����ۂ�
}HITEFFECT;

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
void InitHitEffect( void );
void UninitHitEffect( void );
void UpdateHitEffect( void );
void DrawHitEffect( void );
void CreateHitEffect( D3DXVECTOR2 pos, D3DXVECTOR2 size, float speed, float angleOfFire, D3DXCOLOR color, D3DXCOLOR colorValue,  int life );

#endif