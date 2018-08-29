/*==============================================================================

    �p�[�e�B�N���̃w�b�_ [particle.h]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/5/31
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

/*------------------------------------------------------------------------------
  �@�w�b�_�t�@�C��
------------------------------------------------------------------------------*/
#include "main.h"

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define PARTICLE_WIDTH			(40.0f)							// �|���S���̕��̔���
#define PARTICLE_HEIGHT			(40.0f)							// �|���S���̍����̔���
#define PARTICLE_SPEED          (2.0f)							// �p�[�e�B�N���̊����
#define PARTICLE_LIFE           (60)							// �p�[�e�B�N���̊����

/*------------------------------------------------------------------------------
  �@�\���̂̐錾
------------------------------------------------------------------------------*/
// �@���̍\����
typedef struct
{
	D3DXVECTOR2	fPos;			// ������W
	D3DXVECTOR2	fSize;			// ��T�C�Y
	D3DXVECTOR2	fVelocity;		// ���x
	D3DXVECTOR2 fLeftUp;		// ����̒��_
	D3DXVECTOR2 fRightUp;		// �E��̒��_
	D3DXVECTOR2 fLeftDown;		// �����̒��_
	D3DXVECTOR2 fRightDown;		// �E���̒��_
	D3DXCOLOR	fColor;			// �t�F�[�h����F
	D3DXCOLOR	fColorValue;	// �F�̕ω���
	float		fRot;			// ��]��(���W�A���p)
	float		fLength;		// ���a
	float		fAngle;			// �Ίp���̊p�x
	float		fScale;			// �傫���̔{��
	float		fSpeed;			// �����i�X�J���[�j
	int			nLife;			// �̗�
	bool		bUse;			// �g�p�����ۂ�
}PARTICLE;

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
void InitParticle( void );
void UninitParticle( void );
void UpdateParticle( void );
void DrawParticle( void );
void CreateParticle( D3DXVECTOR2 pos, D3DXVECTOR2 size, float speed, float angleOfFire, D3DXCOLOR color, D3DXCOLOR colorValue,  int life );

#endif