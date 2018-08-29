/*==============================================================================

    �|���S������ [hurdle.cpp]
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
#include "hurdle.h"
#include "player.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define HURDLE_TEXTURENAME      "data/TEXTURE/hurdle.png"		 // �e�N�X�`���t�@�C�����i�ʏ펞�j
#define HURDLE_TEXTURENAME_FALL "data/TEXTURE/hurdle_fall.png"	 // �e�N�X�`���t�@�C�����i�|���ԁj
#define HURDLE_POS_X ( 0 )							 			 // �n�[�h���̕\���ʒuX
#define HURDLE_POS_Y ( 0 )										 // �n�[�h���̕\���ʒuY
#define HURDLE_TEX_MAX (2)										 // �n�[�h���̃e�N�X�`���摜��

#define HURDLE_FALL_WIDTH ( 50.0f )								 // �|��n�[�h���̕�
#define HURDLE_FALL_HEIGHT ( 100.0f )							 // �|��n�[�h���̍���
#define HURDLE_TEX_WIDTH ( 1.0f )								 // �n�[�h���̃e�N�X�`����
#define HURDLE_TEX_HEIGHT ( 1.0f )								 // �n�[�h���̃e�N�X�`������
#define HURDLE_MOVE_SPEED ( 3.2f )								 // �n�[�h���̈ړ����x
#define HURDLE_SCROLL_COEFFICIENT (800.0f)						 // �n�[�h���̈ړ���w�i�ɍ��킹��W��

/*------------------------------------------------------------------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTextureHurdle[ HURDLE_TEX_MAX ]	= {};	// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferHurdle					= NULL; // ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

HURDLE Hurdle[ HURDLE_MAX ];  // �n�[�h���̍\����

// ��]�֌W
float		g_fLengthHurdle;  // �|���S���̑Ίp���̒���
float	    g_fAngleHurdle;   // �|���S���̑Ίp���̊p�x

/*------------------------------------------------------------------------------
  �@����������
------------------------------------------------------------------------------*/
void InitHurdle( void )
{
	srand( ( unsigned )time ( 0 ) );	// �����ݒ�

	// �n�[�h���̏�����
	for( int nCntHurdle = 0; nCntHurdle < HURDLE_MAX; nCntHurdle++ )
	{
		Hurdle[ nCntHurdle ].fPos.x	 = SCREEN_WIDTH;
		Hurdle[ nCntHurdle ].fPos.y	 = ( rand() % ( int )( LIMIT_BOTTOM - LIMIT_TOP ) ) + LIMIT_TOP - HURDLE_HEIGHT / 2.0f;
		Hurdle[ nCntHurdle ].fPosTex.x = 0.0f;
		Hurdle[ nCntHurdle ].fPosTex.y = 0.0f;
		Hurdle[ nCntHurdle ].bUse     = false;
	}
	
	// ���_�̐ݒ�
	MakeVertexPolygon( &g_pVtxBufferHurdle, HURDLE_MAX );
	
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					 // �f�o�C�X�̃|�C���^��n��
											HURDLE_TEXTURENAME,			 // �e�N�X�`���̃t�@�C����
											&g_pTextureHurdle[ 0 ] ) ) ) // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK ); // �G���[�`�F�b�N
	}
	
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					 // �f�o�C�X�̃|�C���^��n��
											HURDLE_TEXTURENAME_FALL,	 // �e�N�X�`���̃t�@�C����
											&g_pTextureHurdle[ 1 ] ) ) ) // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK ); // �G���[�`�F�b�N
	}
}

/*------------------------------------------------------------------------------
  �@�I������
------------------------------------------------------------------------------*/	
void UninitHurdle( void )
{
	// �e�N�X�`���C���^�[�t�F�[�X�̊J��
	for( int nCnt = 0; nCnt < HURDLE_TEX_MAX; nCnt++ )
	{
		if( g_pTextureHurdle[ nCnt ] != NULL )
		{
			g_pTextureHurdle[ nCnt ]->Release();
			g_pTextureHurdle[ nCnt ] = NULL;
		}
	}

	// ���_�o�b�t�@�C���^�[�t�F�[�X�̊J��
	if( g_pVtxBufferHurdle != NULL )
	{
		g_pVtxBufferHurdle->Release();
		g_pVtxBufferHurdle = NULL;
	}
}

/*------------------------------------------------------------------------------
  �@�X�V����
------------------------------------------------------------------------------*/
void UpdateHurdle( void )
{	
	// �X�N���[���w�i�̎擾
	MOVEBG* pMoveBg = NULL;
	pMoveBg			= GetMoveBG();
	if( pMoveBg == NULL )
	{
		printf( "�f�[�^������܂���\n" );
	}	

	VERTEX_2D* pVtx = NULL; // VRAM�̉��z�A�h���X���i�[
	
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferHurdle->Lock( 0,
							  0,				// ���ׂĂ����b�N
							  ( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							  0 );	

	// �X�V
	for( int nCntHurdle = 0; nCntHurdle < HURDLE_MAX; nCntHurdle++ )
	{
		if( !Hurdle[ nCntHurdle ].bUse ) { continue; }

		// ���W�̍X�V
		Hurdle[ nCntHurdle ].fPos.x -= ( pMoveBg->fTexScrollSpeed * HURDLE_SCROLL_COEFFICIENT );
		
		// �l���̍��W�̎Z�o
		Hurdle[ nCntHurdle ].fLeftUp.x    = Hurdle[ nCntHurdle ].fPos.x;
		Hurdle[ nCntHurdle ].fLeftUp.y    = Hurdle[ nCntHurdle ].fPos.y;
		Hurdle[ nCntHurdle ].fRightUp.x   = Hurdle[ nCntHurdle ].fPos.x + HURDLE_WIDTH;
		Hurdle[ nCntHurdle ].fRightUp.y   = Hurdle[ nCntHurdle ].fPos.y;
		Hurdle[ nCntHurdle ].fLeftDown.x  = Hurdle[ nCntHurdle ].fPos.x;
		Hurdle[ nCntHurdle ].fLeftDown.y  = Hurdle[ nCntHurdle ].fPos.y + HURDLE_HEIGHT;
		Hurdle[ nCntHurdle ].fRightDown.x = Hurdle[ nCntHurdle ].fPos.x + HURDLE_WIDTH;
		Hurdle[ nCntHurdle ].fRightDown.y = Hurdle[ nCntHurdle ].fPos.y + HURDLE_HEIGHT;
			
		// ���_���W�̐ݒ�
		pVtx[ 0 ].pos.x = Hurdle[ nCntHurdle ].fLeftUp.x;   	// ����̒��_X
		pVtx[ 0 ].pos.y = Hurdle[ nCntHurdle ].fLeftUp.y;   	// ����̒��_Y		
		pVtx[ 1 ].pos.x = Hurdle[ nCntHurdle ].fRightUp.x;  	// �E��̒��_X
		pVtx[ 1 ].pos.y = Hurdle[ nCntHurdle ].fRightUp.y;  	// �E��̒��_Y
		pVtx[ 2 ].pos.x = Hurdle[ nCntHurdle ].fLeftDown.x; 	// �����̒��_X
		pVtx[ 2 ].pos.y = Hurdle[ nCntHurdle ].fLeftDown.y; 	// �����̒��_Y
		pVtx[ 3 ].pos.x = Hurdle[ nCntHurdle ].fRightDown.x;	// �E���̒��_X
		pVtx[ 3 ].pos.y = Hurdle[ nCntHurdle ].fRightDown.y;	// �E���̒��_Y

		// UV���W�̐ݒ�
		pVtx[ 0 ].tex = D3DXVECTOR2( Hurdle[ nCntHurdle ].fPosTex.x,				    Hurdle[ nCntHurdle ].fPosTex.y );					    // ����̒��_
		pVtx[ 1 ].tex = D3DXVECTOR2( Hurdle[ nCntHurdle ].fPosTex.x + HURDLE_TEX_WIDTH, Hurdle[ nCntHurdle ].fPosTex.y );					    // �E��̒��_
		pVtx[ 2 ].tex = D3DXVECTOR2( Hurdle[ nCntHurdle ].fPosTex.x,				    Hurdle[ nCntHurdle ].fPosTex.y + HURDLE_TEX_HEIGHT );	// �����̒��_	
		pVtx[ 3 ].tex = D3DXVECTOR2( Hurdle[ nCntHurdle ].fPosTex.x + HURDLE_TEX_WIDTH, Hurdle[ nCntHurdle ].fPosTex.y + HURDLE_TEX_HEIGHT );	// �E���̒��_

 		pVtx += 4;
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferHurdle->Unlock();	
}

/*------------------------------------------------------------------------------
  �@�`�揈��
------------------------------------------------------------------------------*/
void DrawHurdle( void )
{
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �p�C�v���C���i�X�g���[���j��ݒ�
	pDevice->SetStreamSource( 0,						// �p�C�v���C���ԍ�
							  g_pVtxBufferHurdle,			// �p�C�v���C���̌��ɂȂ�̈���w��
							  0,						// �I�t�Z�b�g�i�P�ʂ�byte�j���X�g���[���̊J�n�ʒu���w��ł���
							  sizeof( VERTEX_2D ) );	// �����f�[�^�̒P�ʃT�C�Y�i�X�g���C�h�ʁj

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF( FVF_VERTEX_2D );

	// �v���~�e�B�u�iPolygon�j�̕`��
	for( int nCnt = 0; nCnt < HURDLE_MAX; nCnt++ )
	{
		if( !Hurdle[ nCnt ].bUse ) { continue; }
		
		// �e�N�X�`���̐ݒ�
		if( !Hurdle[ nCnt ].bFall )
		{
			pDevice->SetTexture( 0, g_pTextureHurdle[ 0 ] );
		}
		else
		{
			pDevice->SetTexture( 0, g_pTextureHurdle[ 1 ] );
		}

		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, // �v���~�e�B�u�̎��
								nCnt * 4,			 // �`��J�n�ʒu�̃I�t�Z�b�g�i���_���j
								NUM_POLYGON );		 // �`�悷��v���~�e�B�u�̐�
	}	
}

/*------------------------------------------------------------------------------
  �@�Q�b�^�[
------------------------------------------------------------------------------*/
HURDLE* GetHurdle( void )
{
	return Hurdle;
}

