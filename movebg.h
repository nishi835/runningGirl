/*==============================================================================

    MOVEBG�����̃w�b�_ [movebg.h]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/5/31
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/
#ifndef _MOVEBG_H_
#define _MOVEBG_H_

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define MOVEBG_TEXTURE_SCROLL_ACCELERATION		(0.0002f)		// MoveBG�̃e�N�X�`���X�N���[�������x
#define MOVEBG_TEXTURE_SCROLL_ACCELERATION_DASH	(0.0004f)		// MoveBG�̃e�N�X�`���X�N���[�������x(�_�b�V�����j
#define MOVEBG_TEXTURE_LIMIT_SCROOL_SPEED		(0.0075f)		// MoveBG�̃e�N�X�`���X�N���[���X�s�[�h�̌��E�l
#define MOVEBG_TEXTURE_LIMIT_SCROOL_SPEED_DASH  (0.015f)		// MoveBG�̃e�N�X�`���X�N���[���X�s�[�h�̌��E�l(�_�b�V�����j

/*------------------------------------------------------------------------------
  �@�\���̂̐錾
------------------------------------------------------------------------------*/
// MoveBG�̍\����
typedef struct
{
	float fPosTexX;		    // MoveBG�̃e�N�X�`��X���W
	float fPosTexY;		    // MoveBG�̃e�N�X�`��Y���W
	float fTexScrollSpeed;  // MoveBG�̃e�N�X�`���X�N���[���X�s�[�h
}MOVEBG;

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
void InitMoveBG( void );
void UninitMoveBG( void );
void UpdateMoveBG( void );
void DrawMoveBG( void );
MOVEBG* GetMoveBG( void );

#endif