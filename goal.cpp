/*==============================================================================

    �|���S������ [goal.cpp]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/5/31
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  �@�w�b�_�t�@�C��
------------------------------------------------------------------------------*/
#include "main.h"
#include "polygon.h"
#include "movebg.h"
#include "goal.h"
#include "player.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define GOAL_TEXTURENAME      "data/TEXTURE/goalline.png"		 // �e�N�X�`���t�@�C����
#define GOAL_POS_X ( 24000.0f )							 		 // �S�[���̕\���ʒuX
#define GOAL_POS_Y ( 150.0f )									 // �S�[���̕\���ʒuY
#define GOAL_TEX_MAX (1)									 // �S�[���̃e�N�X�`���摜��

#define GOAL_TEX_WIDTH ( 1.0f )								 // �S�[���̃e�N�X�`����
#define GOAL_TEX_HEIGHT ( 1.0f )							 // �S�[���̃e�N�X�`������
#define GOAL_MOVE_SPEED ( 3.2f )							 // �S�[���̈ړ����x
#define GOAL_SCROLL_COEFFICIENT (800.0f)					 // �S�[���̈ړ���w�i�ɍ��킹��W��

/*------------------------------------------------------------------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTextureGoal = NULL;	// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferGoal				= NULL; // ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

GOAL Goal;  // �S�[���̍\����

// ��]�֌W
float		g_fLengthGoal;  // �|���S���̑Ίp���̒���
float	    g_fAngleGoal;   // �|���S���̑Ίp���̊p�x

/*------------------------------------------------------------------------------
  �@����������
------------------------------------------------------------------------------*/
void InitGoal( void )
{
	srand( ( unsigned )time ( 0 ) );	// �����ݒ�

	// �S�[���̏�����
	Goal.fPos.x	 = GOAL_POS_X;
	Goal.fPos.y	 = GOAL_POS_Y;
	Goal.fPosTex.x = 0.0f;
	Goal.fPosTex.y = 0.0f;
	Goal.bUse      = false;
	
	// �l���̍��W�̎Z�o
	Goal.fLeftUp.x    = Goal.fPos.x;
	Goal.fLeftUp.y    = Goal.fPos.y;
	Goal.fRightUp.x   = Goal.fPos.x + GOAL_WIDTH;
	Goal.fRightUp.y   = Goal.fPos.y;
	Goal.fLeftDown.x  = Goal.fPos.x;
	Goal.fLeftDown.y  = Goal.fPos.y + GOAL_HEIGHT;
	Goal.fRightDown.x = Goal.fPos.x + GOAL_WIDTH;
	Goal.fRightDown.y = Goal.fPos.y + GOAL_HEIGHT;

	// *** ��]���� ***																				// �|���S���̉�]�ʂ̐ݒ�
	g_fLengthGoal = sqrt( GOAL_WIDTH * GOAL_WIDTH + GOAL_HEIGHT * GOAL_HEIGHT ) / 2.0f;   // �Ίp���̒����̎Z�o
	g_fAngleGoal  = atan2( GOAL_HEIGHT, GOAL_WIDTH );											// �Ίp���̊p�x�̎Z�o

	// ���_�̐ݒ�
	MakeVertexPolygon( &g_pVtxBufferGoal, 1 );
	
	VERTEX_2D* pVtx = NULL; // VRAM�̉��z�A�h���X���i�[
	
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferGoal->Lock( 0,
							0,				// ���ׂĂ����b�N
							( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							0 );	


	// ���_�o�b�t�@�ݒ�
	SetVertexGoal( pVtx );		

	// UV���W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );					    // ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );					    // �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );	// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );	// �E���̒��_
	
	// ���_���W�̐ݒ�
	pVtx[ 0 ].pos.x = Goal.fLeftUp.x;   	// ����̒��_X
	pVtx[ 0 ].pos.y = Goal.fLeftUp.y;   	// ����̒��_Y		
	pVtx[ 1 ].pos.x = Goal.fRightUp.x;  	// �E��̒��_X
	pVtx[ 1 ].pos.y = Goal.fRightUp.y;  	// �E��̒��_Y
	pVtx[ 2 ].pos.x = Goal.fLeftDown.x; 	// �����̒��_X
	pVtx[ 2 ].pos.y = Goal.fLeftDown.y; 	// �����̒��_Y
	pVtx[ 3 ].pos.x = Goal.fRightDown.x;	// �E���̒��_X
	pVtx[ 3 ].pos.y = Goal.fRightDown.y;	// �E���̒��_Y

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferGoal->Unlock();	

	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					 // �f�o�C�X�̃|�C���^��n��
											GOAL_TEXTURENAME,			 // �e�N�X�`���̃t�@�C����
											&g_pTextureGoal ) ) ) // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK ); // �G���[�`�F�b�N
	}	
}

/*------------------------------------------------------------------------------
  �@�I������
------------------------------------------------------------------------------*/	
void UninitGoal( void )
{
	// �e�N�X�`���C���^�[�t�F�[�X�̊J��
	if( g_pTextureGoal != NULL )
	{
		g_pTextureGoal->Release();
		g_pTextureGoal = NULL;
	}

	// ���_�o�b�t�@�C���^�[�t�F�[�X�̊J��
	if( g_pVtxBufferGoal != NULL )
	{
		g_pVtxBufferGoal->Release();
		g_pVtxBufferGoal = NULL;
	}
}

/*------------------------------------------------------------------------------
  �@�X�V����
------------------------------------------------------------------------------*/
void UpdateGoal( void )
{	
	// �X�N���[���w�i�̎擾
	MOVEBG* pMoveBg = NULL;
	pMoveBg			= GetMoveBG();
	
	// �S�[���̈ړ� ///////////////////////////////////////////////////////////////

	// ���W�̍X�V
	Goal.fPos.x -= ( pMoveBg->fTexScrollSpeed * GOAL_SCROLL_COEFFICIENT );
	
	// �l���̍��W�̎Z�o
	Goal.fLeftUp.x    = Goal.fPos.x;
	Goal.fLeftUp.y    = Goal.fPos.y;
	Goal.fRightUp.x   = Goal.fPos.x + GOAL_WIDTH;
	Goal.fRightUp.y   = Goal.fPos.y;
	Goal.fLeftDown.x  = Goal.fPos.x;
	Goal.fLeftDown.y  = Goal.fPos.y + GOAL_HEIGHT;
	Goal.fRightDown.x = Goal.fPos.x + GOAL_WIDTH;
	Goal.fRightDown.y = Goal.fPos.y + GOAL_HEIGHT;

	VERTEX_2D* pVtx = NULL; // VRAM�̉��z�A�h���X���i�[
	
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferGoal->Lock( 0,
							  0,				// ���ׂĂ����b�N
							  ( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							  0 );	

	// ���_�o�b�t�@�ݒ�
	SetVertexGoal( pVtx );		
	
	// UV���W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );					    // ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );					    // �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );	// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );	// �E���̒��_
	
	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferGoal->Unlock();	
}

/*------------------------------------------------------------------------------
  �@�`�揈��
------------------------------------------------------------------------------*/
void DrawGoal( void )
{
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �p�C�v���C���i�X�g���[���j��ݒ�
	pDevice->SetStreamSource( 0,						// �p�C�v���C���ԍ�
							  g_pVtxBufferGoal,			// �p�C�v���C���̌��ɂȂ�̈���w��
							  0,						// �I�t�Z�b�g�i�P�ʂ�byte�j���X�g���[���̊J�n�ʒu���w��ł���
							  sizeof( VERTEX_2D ) );	// �����f�[�^�̒P�ʃT�C�Y�i�X�g���C�h�ʁj

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0, g_pTextureGoal );

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF( FVF_VERTEX_2D );

	// �v���~�e�B�u�iPolygon�j�̕`��	

	if( Goal.bUse )
	{
		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, // �v���~�e�B�u�̎��
								0,			 // �`��J�n�ʒu�̃I�t�Z�b�g�i���_���j
								NUM_POLYGON );		 // �`�悷��v���~�e�B�u�̐�
	}	
}

/*------------------------------------------------------------------------------
  �@���_�o�b�t�@�ݒ�
------------------------------------------------------------------------------*/
void SetVertexGoal( VERTEX_2D* pVtx )
{
	// ���_���W�̐ݒ�
	pVtx[ 0 ].pos.x = Goal.fLeftUp.x;   	// ����̒��_X
	pVtx[ 0 ].pos.y = Goal.fLeftUp.y;   	// ����̒��_Y		
	pVtx[ 1 ].pos.x = Goal.fRightUp.x;  	// �E��̒��_X
	pVtx[ 1 ].pos.y = Goal.fRightUp.y;  	// �E��̒��_Y
	pVtx[ 2 ].pos.x = Goal.fLeftDown.x; 	// �����̒��_X
	pVtx[ 2 ].pos.y = Goal.fLeftDown.y; 	// �����̒��_Y
	pVtx[ 3 ].pos.x = Goal.fRightDown.x;	// �E���̒��_X
	pVtx[ 3 ].pos.y = Goal.fRightDown.y;	// �E���̒��_Y
}

/*------------------------------------------------------------------------------
  �@�Q�b�^�[
------------------------------------------------------------------------------*/
GOAL* GetGoal( void )
{
	return &Goal;
}

