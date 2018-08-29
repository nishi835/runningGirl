/*==============================================================================

    �_�b�V���G�t�F�N�g�̃w�b�_ [dasheffect.h]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/5/31
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/
#ifndef _DASHEFFECT_H_
#define _DASHEFFECT_H_

/*------------------------------------------------------------------------------
  �@�w�b�_�t�@�C��
------------------------------------------------------------------------------*/
#include "main.h"

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define DASHEFFECT_WIDTH		 (0.8f)											// �|���S���̕�
#define DASHEFFECT_HEIGHT		 (0.8f)											// �|���S���̍���
#define DASHEFFECT_VELOCITY_X	 (-5.0f)										// �G�t�F�N�g�̉��ړ����x
#define DASHEFFECT_EMITTER_MAX	 (43)											// �G�t�F�N�g�̔������̍ő吔
#define DASHEFFECT_MAX			 (2048)											// �G�t�F�N�g�̍ő吔
#define DASHEFFECT_LIFE			 (20)											// �G�t�F�N�g�̗̑�

/*------------------------------------------------------------------------------
  �@�\���̂̐錾
------------------------------------------------------------------------------*/
// �G�t�F�N�g�̍\����
typedef struct
{
	D3DXVECTOR2	fPos;			// ���W
	D3DXVECTOR2	fSize;			// ��T�C�Y
	D3DXVECTOR2	fVelocity;		// ���x
	D3DXVECTOR2 fLeftUp;		// ����̒��_
	D3DXVECTOR2 fRightUp;		// �E��̒��_
	D3DXVECTOR2 fLeftDown;		// �����̒��_
	D3DXVECTOR2 fRightDown;		// �E���̒��_
	D3DXCOLOR	fColor;			// �t�F�[�h����F
	D3DXCOLOR	fColorValue;	// �F�̕ω���
	float		fRot;			// ��]�ʁi���W�A���j
	float		fLength;		// ���a
	float		fAngle;			// �Ίp���̊p�x
	float		fScale;			// �傫���̔{��
	int			nCounter;		// �J�E���^�[
	int			nLife;			// �̗�
	int         nMaxLife;       // �ő�̗�
	bool		bUse;			// �g�p�����ۂ�
}DASHEFFECT;

// �G�t�F�N�g�̔������̍\����
typedef struct
{
	D3DXVECTOR2	fPos;			// ���W
	D3DXVECTOR2	fSize;			// �T�C�Y
	D3DXVECTOR2	fVelocity;		// ���x
	float		fLength;		// ���a
	float		fAngle;			// �Ίp���̊p�x
	int         nLife;			// �̗�
	bool		bUse;           // �g�p�t���O
}DASHEFFECTEMITTER;

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
void InitDashEffect( void );
void UninitDashEffect( void );
void UpdateDashEffect( void );
void DrawDashEffect( void );
void CreateDashEffectEmitter( D3DXVECTOR2 pos, D3DXVECTOR2 size, D3DXVECTOR2 velocity, int life );
void MoveDashEffectEmitter( float movey );
void UnuseDashEffect( void );

#endif