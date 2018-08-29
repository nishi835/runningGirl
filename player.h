/*==============================================================================

    �|���S�������̃w�b�_ [polygon.h]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/5/31
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/
#ifndef _PLAYER_H_
#define _PLAYER_H_

/*------------------------------------------------------------------------------
  �@�w�b�_�t�@�C��
------------------------------------------------------------------------------*/
#include "polygon.h"

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define PLAYER_WIDTH ( 130.0f )								 // �v���C���[�̕�
#define PLAYER_HEIGHT ( 180.0f )							 // �v���C���[�̍���
#define PLAYER_JUMP_VALUE ( 12.0f )							 // �v���C���[�̃W�����v��
#define LIMIT_BOTTOM ( 465.0f )								 // ���ړ�����
#define LIMIT_TOP ( 250.0f )								 // ��ړ�����

/*------------------------------------------------------------------------------
  �@�񋓑̂̐錾
------------------------------------------------------------------------------*/
// �v���C���[�̏�Ԃ̗񋓑�
typedef enum
{
	STATE_STOP = 0, // ��~���
	STATE_RUN,      // ���s���
	STATE_JUMP,     // �W�����v���
	STATE_JUMPER,   // �W�����v��ɂ��W�����v���
	STATE_CRUSH,    // �Ԃ�����
	STATE_TUMBLE,   // �]�|���
	STATE_GOAL,     // �S�[�����
	STATE_WHEEZE,   // �S�[����̑��؂���
	STATE_TIMEUP,   // �^�C���A�b�v���
	STATE_GAMEOVER, // �Q�[���I�[�o�[���
	STATE_MAX
}STATE;

/*------------------------------------------------------------------------------
  �@�\���̂̐錾
------------------------------------------------------------------------------*/
// �v���C���[�̍\����
typedef struct
{
	D3DXVECTOR2 fPos;			// ���S
	D3DXVECTOR2 fAxis;			// ��]���i�����j
	D3DXVECTOR2 fMove;			// ���x
	D3DXVECTOR2 fLeftUp;		// ����̒��_
	D3DXVECTOR2 fRightUp;		// �E��̒��_
	D3DXVECTOR2 fLeftDown;		// �����̒��_
	D3DXVECTOR2 fRightDown;		// �E���̒��_
	D3DXVECTOR2 fPosTex;		// �e�N�X�`���̍��W
	float		fRotC;			// ���S�����Ƃ�����]��
	float		fRotA;			// Axis�����Ƃ�����]��
	float		fScrollTex;		// �e�N�X�`���̃X�N���[���ʒu
	float		fLandingPosY;   // �W�����v�̒��n�_
	int			nStunCount;		// �X�^�����̃t���[�������J�E���g
	int			nDashCount;     // �_�b�V�����̃t���[�������J�E���g
	int         nCount;         // �ėp�J�E���^�[
	int			nNumTexX;		// �e�N�X�`���̗�ԍ�
	int			nNumTexY;		// �e�N�X�`���̍s�ԍ�
	STATE		State;          // �v���C���[�̏��
	bool		bDash;          // �_�b�V�����Ă��邩�ۂ�
}PLAYER;

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
void InitPlayer( void );
void UninitPlayer( void );
void UpdatePlayer( void );
void DrawPlayer( void );
void RotationPlayer( VERTEX_2D* pVtx );
bool HitObject( float pos1x, float pos1y, float width1, float height1, float pos2x, float pos2y, float width2, float height2 );
PLAYER* GetPlayer( void );
void SetPlayerState( STATE state );
void MovePlayerPos( D3DXVECTOR2 pos );

#endif