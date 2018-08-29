/*==============================================================================

    �p�[�e�B�N���̃w�b�_ [particle.h]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/5/31
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

/*------------------------------------------------------------------------------
  �@�w�b�_�t�@�C��
------------------------------------------------------------------------------*/
#include "main.h"

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define MESSAGE_WIDTH			(40.0f)							// �|���S���̕��̔���
#define MESSAGE_HEIGHT			(40.0f)							// �|���S���̍����̔���
#define MESSAGE_READY_WIDTH         (250)                       // READY�̕�
#define MESSAGE_SPEED          (2.0f)							// �p�[�e�B�N���̊����
#define MESSAGE_LIFE           (60)							// �p�[�e�B�N���̊����

/*------------------------------------------------------------------------------
  �@�񋓑̂̐錾
------------------------------------------------------------------------------*/
typedef enum
{
	MESSAGE_TYPE_STAGE = 0,  // STAGE�̕\��
	MESSAGE_TYPE_READY,		 // READY�̕\��
	MESSAGE_TYPE_GO,		 // GO�̕\��
	MESSAGE_TYPE_GOAL,       // GOAL�̕\��
	MESSAGE_TYPE_TIMEUP,
	MESSAGE_TYPE_GAMEOVER,
	MESSAGE_TYPE_MAX
}MESSAGETYPE;

/*------------------------------------------------------------------------------
  �@�\���̂̐錾
------------------------------------------------------------------------------*/
// �@���̍\����
typedef struct
{
	D3DXVECTOR2  fPos;			// ������W
	D3DXVECTOR2  fSize;			// ��T�C�Y
	D3DXVECTOR2  fVelocity;		// ���x
	D3DXVECTOR2  fLeftUp;		// ����̒��_
	D3DXVECTOR2  fRightUp;		// �E��̒��_
	D3DXVECTOR2  fLeftDown;		// �����̒��_
	D3DXVECTOR2  fRightDown;	// �E���̒��_
	D3DXCOLOR	 fColor;		// �t�F�[�h����F
	D3DXCOLOR	 fColorValue;	// �F�̕ω���
	float		 fRot;			// ��]��(���W�A���p)
	float		 fDirect;       // �i�s�����i���W�A���p�j
	float		 fLength;		// ���a
	float		 fAngle;		// �Ίp���̊p�x
	float		 fScale;		// �傫���̔{��
	float		 fSpeed;		// �����i�X�J���[�j
	int			 nLife;			// �̗�
	int			 nCounter;      // �J�E���^�[
	bool		 bUse;			// �g�p�����ۂ�
	int			 nType;         // �^�C�v
}MESSAGE;

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
void InitMessage( void );
void UninitMessage( void );
void UpdateMessage( void );
void DrawMessage( void );
void CreateMessage( D3DXVECTOR2 pos, D3DXVECTOR2 size, float speed, float direct, float angleOfFire, D3DXCOLOR color, D3DXCOLOR colorValue,  MESSAGETYPE type );
MESSAGE* GetMessage( void );
void SetMessageType( MESSAGETYPE type );

#endif