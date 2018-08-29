/*==============================================================================

    �|���S������ [Player.cpp]
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
#include "can.h"
#include "player.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define CAN_TEXTURENAME "data/TEXTURE/tire.png"			 // �e�N�X�`���t�@�C����
#define CAN_TEX_WIDTH		   (1.0f)					 // �ʂ̃e�N�X�`����
#define CAN_TEX_HEIGHT		   (1.0f)					 // �ʂ̃e�N�X�`������
#define CAN_MOVE_SPEED		   (3.2f)					 // �ʂ̈ړ����x
#define CAN_SCROLL_COEFFICIENT (800.0f)					 // �ʂ̈ړ���w�i�ɍ��킹��W��

/*------------------------------------------------------------------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTextureCan	= NULL;	// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferCan = NULL; // ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

CAN Can[ CAN_MAX ];		  // �ʂ̍\���̔z��

// ��]�֌W
D3DXVECTOR2 g_posCan;	  // �|���S���̒��S���W
float		g_fLengthCan; // �|���S���̑Ίp���̒���
float	    g_fAngleCan;  // �|���S���̑Ίp���̊p�x

/*------------------------------------------------------------------------------
  �@����������
------------------------------------------------------------------------------*/
void InitCan( void )
{
	srand( ( unsigned )time ( 0 ) );	// �����ݒ�

	// �ʂ̏�����	
	for( int nCntCan = 0; nCntCan < CAN_MAX; nCntCan++ )
	{
		Can[ nCntCan ].fPos		  = D3DXVECTOR2( SCREEN_WIDTH, LIMIT_BOTTOM - CAN_HEIGHT );
		Can[ nCntCan ].fMove	  = D3DXVECTOR2( - CAN_MOVE_SPEED, 0.0f );
		Can[ nCntCan ].fPosTex.x  = 0.0f;
		Can[ nCntCan ].fPosTex.y  = 0.0f;
		Can[ nCntCan ].fRot       = 0.0f;
		Can[ nCntCan ].bFly		  = false;
		Can[ nCntCan ].bUse		  = false;
	}
	
	// --- ��]���� ---	////////////////////////////////////////////////////////

	g_fLengthCan = sqrt( CAN_WIDTH * CAN_WIDTH + CAN_HEIGHT * CAN_HEIGHT ) / 2.0f;   // �Ίp���̒����̎Z�o
	g_fAngleCan  = atan2( CAN_HEIGHT, CAN_WIDTH );									 // �Ίp���̊p�x�̎Z�o

	// ���_�̐ݒ�
	MakeVertexPolygon( &g_pVtxBufferCan, CAN_MAX );
	
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,			 // �f�o�C�X�̃|�C���^��n��
											CAN_TEXTURENAME,     // �e�N�X�`���̃t�@�C����
											&g_pTextureCan ) ) ) // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK ); // �G���[�`�F�b�N
	}
}

/*------------------------------------------------------------------------------
  �@�I������
------------------------------------------------------------------------------*/	
void UninitCan( void )
{
	// �e�N�X�`���C���^�[�t�F�[�X�A���_�o�b�t�@�C���^�[�t�F�[�X�̊J��
	UninitPolygon( &g_pTextureCan, &g_pVtxBufferCan );
}

/*------------------------------------------------------------------------------
  �@�X�V����
------------------------------------------------------------------------------*/
void UpdateCan( void )
{	
	// �X�N���[���w�i�̎擾
	MOVEBG* pMoveBg = NULL;
	pMoveBg			= GetMoveBG();
	
		
	VERTEX_2D* pVtx = NULL; // VRAM�̉��z�A�h���X���i�[
	
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferCan->Lock( 0,
						   0,				 // ���ׂĂ����b�N
						   ( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
						   0 );	

	for( int nCntCan = 0; nCntCan < CAN_MAX; nCntCan++ )
	{
		if( !Can[ nCntCan ].bUse ) { continue; } 

		// �ʂ̈ړ� ///////////////////////////////////////////////////////////////

		// �ړ��ʂ̐ݒ�
		if( Can[ nCntCan ].bFly )
		{
			Can[ nCntCan ].fMove.y += 0.6f;
			Can[ nCntCan ].fRot    += 0.4f;
		}
		else if( Can[ nCntCan ].fPos.x < SCREEN_WIDTH )
		{
			Can[ nCntCan ].fMove.x = - ( pMoveBg[ 0 ].fTexScrollSpeed * CAN_SCROLL_COEFFICIENT + CAN_MOVE_SPEED );
		}
		else
		{
			Can[ nCntCan ].fMove.x = - ( pMoveBg[ 0 ].fTexScrollSpeed * CAN_SCROLL_COEFFICIENT );
		}

		// ���W�̍X�V
		Can[ nCntCan ].fPos += Can[ nCntCan ].fMove;
	
		// ���_���W�̐ݒ� /////////////////////////////////////////////////////////
		
		// ���S���W�̎Z�o
		Can[ nCntCan ].fAxis.y = Can[ nCntCan ].fPos.y + CAN_HEIGHT * 0.5f;
		Can[ nCntCan ].fAxis.x = Can[ nCntCan ].fPos.x + CAN_WIDTH * 0.5f;

		// �l���̍��W�̎Z�o
 		Can[ nCntCan ].fLeftUp.x    = Can[ nCntCan ].fAxis.x;
		Can[ nCntCan ].fLeftUp.y    = Can[ nCntCan ].fAxis.y;
		Can[ nCntCan ].fRightUp.x   = Can[ nCntCan ].fAxis.x + CAN_WIDTH;
		Can[ nCntCan ].fRightUp.y   = Can[ nCntCan ].fAxis.y;
		Can[ nCntCan ].fLeftDown.x  = Can[ nCntCan ].fAxis.x;
		Can[ nCntCan ].fLeftDown.y  = Can[ nCntCan ].fAxis.y + CAN_HEIGHT;
		Can[ nCntCan ].fRightDown.x = Can[ nCntCan ].fAxis.x + CAN_WIDTH;
		Can[ nCntCan ].fRightDown.y = Can[ nCntCan ].fAxis.y + CAN_HEIGHT;
		
		// �l���̍��W�̍X�V
		Can[ nCntCan ].fLeftUp.x    = Can[ nCntCan ].fAxis.x + cos( Can[ nCntCan ].fRot + D3DX_PI + g_fAngleCan ) * g_fLengthCan;		// ����̒��_X
		Can[ nCntCan ].fLeftUp.y    = Can[ nCntCan ].fAxis.y + sin( Can[ nCntCan ].fRot + D3DX_PI + g_fAngleCan ) * g_fLengthCan;		// ����̒��_Y		
		Can[ nCntCan ].fRightUp.x   = Can[ nCntCan ].fAxis.x + cos( Can[ nCntCan ].fRot			 - g_fAngleCan ) * g_fLengthCan;		// �E��̒��_X
		Can[ nCntCan ].fRightUp.y   = Can[ nCntCan ].fAxis.y + sin( Can[ nCntCan ].fRot			 - g_fAngleCan ) * g_fLengthCan;		// �E��̒��_Y
		Can[ nCntCan ].fLeftDown.x  = Can[ nCntCan ].fAxis.x + cos( Can[ nCntCan ].fRot + D3DX_PI - g_fAngleCan ) * g_fLengthCan;		// �����̒��_X
		Can[ nCntCan ].fLeftDown.y  = Can[ nCntCan ].fAxis.y + sin( Can[ nCntCan ].fRot + D3DX_PI - g_fAngleCan ) * g_fLengthCan;		// �����̒��_Y
		Can[ nCntCan ].fRightDown.x = Can[ nCntCan ].fAxis.x + cos( Can[ nCntCan ].fRot			 + g_fAngleCan ) * g_fLengthCan;		// �E���̒��_X
		Can[ nCntCan ].fRightDown.y = Can[ nCntCan ].fAxis.y + sin( Can[ nCntCan ].fRot			 + g_fAngleCan ) * g_fLengthCan;		// �E���̒��_Y
		
		// ���_���W�̐ݒ�
		pVtx[ 0 ].pos.x = Can[ nCntCan ].fLeftUp.x;		// ����̒��_X
		pVtx[ 0 ].pos.y = Can[ nCntCan ].fLeftUp.y;		// ����̒��_Y		
		pVtx[ 1 ].pos.x = Can[ nCntCan ].fRightUp.x;	// �E��̒��_X
		pVtx[ 1 ].pos.y = Can[ nCntCan ].fRightUp.y;	// �E��̒��_Y
		pVtx[ 2 ].pos.x = Can[ nCntCan ].fLeftDown.x;	// �����̒��_X
		pVtx[ 2 ].pos.y = Can[ nCntCan ].fLeftDown.y;	// �����̒��_Y
		pVtx[ 3 ].pos.x = Can[ nCntCan ].fRightDown.x;	// �E���̒��_X
		pVtx[ 3 ].pos.y = Can[ nCntCan ].fRightDown.y;	// �E���̒��_Y
	
		// UV���W�̐ݒ�
		pVtx[ 0 ].tex = D3DXVECTOR2( Can[ nCntCan ].fPosTex.x,				   Can[ nCntCan ].fPosTex.y );					// ����̒��_
		pVtx[ 1 ].tex = D3DXVECTOR2( Can[ nCntCan ].fPosTex.x + CAN_TEX_WIDTH, Can[ nCntCan ].fPosTex.y );					// �E��̒��_
		pVtx[ 2 ].tex = D3DXVECTOR2( Can[ nCntCan ].fPosTex.x,				   Can[ nCntCan ].fPosTex.y + CAN_TEX_HEIGHT );	// �����̒��_	
		pVtx[ 3 ].tex = D3DXVECTOR2( Can[ nCntCan ].fPosTex.x + CAN_TEX_WIDTH, Can[ nCntCan ].fPosTex.y + CAN_TEX_HEIGHT );	// �E���̒��_

		pVtx += 4;
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferCan->Unlock();	
}

/*------------------------------------------------------------------------------
  �@�`�揈��
------------------------------------------------------------------------------*/
void DrawCan( void )
{
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �p�C�v���C���i�X�g���[���j��ݒ�
	pDevice->SetStreamSource( 0,						// �p�C�v���C���ԍ�
							  g_pVtxBufferCan,			// �p�C�v���C���̌��ɂȂ�̈���w��
							  0,						// �I�t�Z�b�g�i�P�ʂ�byte�j���X�g���[���̊J�n�ʒu���w��ł���
							  sizeof( VERTEX_2D ) );	// �����f�[�^�̒P�ʃT�C�Y�i�X�g���C�h�ʁj

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0, g_pTextureCan );

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF( FVF_VERTEX_2D );

	// �v���~�e�B�u�iPolygon�j�̕`��
	for( int nCnt = 0; nCnt < CAN_MAX; nCnt++ )
	{
		if( !Can[ nCnt ].bUse ) { continue; }

		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, // �v���~�e�B�u�̎��
								nCnt * 4,			 // �`��J�n�ʒu�̃I�t�Z�b�g�i���_���j
								NUM_POLYGON );		 // �`�悷��v���~�e�B�u�̐�
	}	
}

/*------------------------------------------------------------------------------
  �@�Q�b�^�[
------------------------------------------------------------------------------*/
CAN* GetCan( void )
{
	return Can;
}

