/*==============================================================================

    �|���S������ [jumper.cpp]
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
#include "jumper.h"
#include "player.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define JUMPER_TEXTURENAME "data/TEXTURE/jumper_2cut.png"		 // �e�N�X�`���t�@�C�����i�ʏ펞�j
#define JUMPER_POS_X (0)							 		 // �n�[�h���̕\���ʒuX
#define JUMPER_POS_Y (0)									 // �n�[�h���̕\���ʒuY

#define JUMPER_TEX_WIDTH (1.0f)							 // �n�[�h���̃e�N�X�`����
#define JUMPER_TEX_HEIGHT (1.0f)							 // �n�[�h���̃e�N�X�`������
#define JUMPER_MOVE_SPEED (3.2f)							 // �n�[�h���̈ړ����x
#define JUMPER_SCROLL_COEFFICIENT (800.0f)					 // �n�[�h���̈ړ���w�i�ɍ��킹��W��

/*------------------------------------------------------------------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTextureJumper	= NULL;	// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferJumper	= NULL; // ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

JUMPER Jumper[ JUMPER_MAX ];  // �n�[�h���̍\����

/*------------------------------------------------------------------------------
  �@����������
------------------------------------------------------------------------------*/
void InitJumper( void )
{
	srand( ( unsigned )time ( 0 ) );	// �����ݒ�

	// �p�����[�^������
	for( int nCntJumper = 0; nCntJumper < JUMPER_MAX; nCntJumper++ )
	{
		Jumper[ nCntJumper ].fPos.x	 = SCREEN_WIDTH * 1.5f;
		Jumper[ nCntJumper ].fPos.y	 = ( rand() % ( int )( LIMIT_BOTTOM - LIMIT_TOP ) ) + LIMIT_TOP - JUMPER_HEIGHT / 2.0f;
		Jumper[ nCntJumper ].fPosTex.x = 0.0f;
		Jumper[ nCntJumper ].fPosTex.y = 0.0f;
	}

	// ���_�̐ݒ�
	MakeVertexPolygon( &g_pVtxBufferJumper, JUMPER_MAX );
	
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					 // �f�o�C�X�̃|�C���^��n��
											JUMPER_TEXTURENAME,			 // �e�N�X�`���̃t�@�C����
											&g_pTextureJumper ) ) )		 // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK ); // �G���[�`�F�b�N
	}	
}

/*------------------------------------------------------------------------------
  �@�I������
------------------------------------------------------------------------------*/	
void UninitJumper( void )
{
	// �e�N�X�`���C���^�[�t�F�[�X�̊J��
	if( g_pTextureJumper != NULL )
	{
		g_pTextureJumper->Release();
		g_pTextureJumper = NULL;
	}
	
	// ���_�o�b�t�@�C���^�[�t�F�[�X�̊J��
	if( g_pVtxBufferJumper != NULL )
	{
		g_pVtxBufferJumper->Release();
		g_pVtxBufferJumper = NULL;
	}
}

/*------------------------------------------------------------------------------
  �@�X�V����
------------------------------------------------------------------------------*/
void UpdateJumper( void )
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
	g_pVtxBufferJumper->Lock( 0,
							  0,				// ���ׂĂ����b�N
							  ( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							  0 );	
	
	// �X�V
	for( int nCntJumper = 0; nCntJumper < JUMPER_MAX; nCntJumper++ )
	{
		if( !Jumper[ nCntJumper ].bUse ) { continue; }

		// ���W�̍X�V
		Jumper[ nCntJumper ].fPos.x -= ( pMoveBg->fTexScrollSpeed * JUMPER_SCROLL_COEFFICIENT );
	
		//// ��ʒ[�܂ōs������
		//if( Jumper[ nCntJumper ].fPos.x + JUMPER_WIDTH < 0 )
		//{
		//	Jumper[ nCntJumper ].fPos.x = SCREEN_WIDTH;
		//	Jumper[ nCntJumper ].fPos.y = ( rand() % ( int )( LIMIT_BOTTOM - LIMIT_TOP ) ) + LIMIT_TOP - JUMPER_HEIGHT / 2.0f;
		//}
	
		// �l���̍��W�̎Z�o
		Jumper[ nCntJumper ].fLeftUp.x    = Jumper[ nCntJumper ].fPos.x;
		Jumper[ nCntJumper ].fLeftUp.y    = Jumper[ nCntJumper ].fPos.y;
		Jumper[ nCntJumper ].fRightUp.x   = Jumper[ nCntJumper ].fPos.x + JUMPER_WIDTH;
		Jumper[ nCntJumper ].fRightUp.y   = Jumper[ nCntJumper ].fPos.y;
		Jumper[ nCntJumper ].fLeftDown.x  = Jumper[ nCntJumper ].fPos.x;
		Jumper[ nCntJumper ].fLeftDown.y  = Jumper[ nCntJumper ].fPos.y + JUMPER_HEIGHT;
		Jumper[ nCntJumper ].fRightDown.x = Jumper[ nCntJumper ].fPos.x + JUMPER_WIDTH;
		Jumper[ nCntJumper ].fRightDown.y = Jumper[ nCntJumper ].fPos.y + JUMPER_HEIGHT;
	
		// ���_���W�̐ݒ�
		pVtx[ 0 ].pos.x = Jumper[ nCntJumper ].fLeftUp.x;   	// ����̒��_X
		pVtx[ 0 ].pos.y = Jumper[ nCntJumper ].fLeftUp.y;   	// ����̒��_Y		
		pVtx[ 1 ].pos.x = Jumper[ nCntJumper ].fRightUp.x;  	// �E��̒��_X
		pVtx[ 1 ].pos.y = Jumper[ nCntJumper ].fRightUp.y;  	// �E��̒��_Y
		pVtx[ 2 ].pos.x = Jumper[ nCntJumper ].fLeftDown.x; 	// �����̒��_X
		pVtx[ 2 ].pos.y = Jumper[ nCntJumper ].fLeftDown.y; 	// �����̒��_Y
		pVtx[ 3 ].pos.x = Jumper[ nCntJumper ].fRightDown.x;	// �E���̒��_X
		pVtx[ 3 ].pos.y = Jumper[ nCntJumper ].fRightDown.y;	// �E���̒��_Y

		// UV���W�̐ݒ�
		PLAYER* pPlayer = GetPlayer();
	
		if( pPlayer->State == STATE_RUN && 
			HitObject( pPlayer->fLeftUp.x, pPlayer->fLeftUp.y, PLAYER_WIDTH, PLAYER_HEIGHT, Jumper[ nCntJumper ].fLeftUp.x + JUMPER_WIDTH * 0.8f, Jumper[ nCntJumper ].fLeftUp.y, JUMPER_WIDTH * 0.3f, JUMPER_HEIGHT ) )
		{
			pVtx[ 0 ].tex = D3DXVECTOR2( Jumper[ nCntJumper ].fPosTex.x + 0.5f, Jumper[ nCntJumper ].fPosTex.y );					    // ����̒��_
			pVtx[ 1 ].tex = D3DXVECTOR2( Jumper[ nCntJumper ].fPosTex.x + 1.0f, Jumper[ nCntJumper ].fPosTex.y );					    // �E��̒��_
			pVtx[ 2 ].tex = D3DXVECTOR2( Jumper[ nCntJumper ].fPosTex.x + 0.5f, Jumper[ nCntJumper ].fPosTex.y + JUMPER_TEX_HEIGHT );	// �����̒��_	
			pVtx[ 3 ].tex = D3DXVECTOR2( Jumper[ nCntJumper ].fPosTex.x + 1.0f, Jumper[ nCntJumper ].fPosTex.y + JUMPER_TEX_HEIGHT );	// �E���̒��_
		}
		else
		{
			pVtx[ 0 ].tex = D3DXVECTOR2( Jumper[ nCntJumper ].fPosTex.x       , Jumper[ nCntJumper ].fPosTex.y );					    // ����̒��_
			pVtx[ 1 ].tex = D3DXVECTOR2( Jumper[ nCntJumper ].fPosTex.x + 0.5f, Jumper[ nCntJumper ].fPosTex.y );					    // �E��̒��_
			pVtx[ 2 ].tex = D3DXVECTOR2( Jumper[ nCntJumper ].fPosTex.x       , Jumper[ nCntJumper ].fPosTex.y + JUMPER_TEX_HEIGHT );	// �����̒��_	
			pVtx[ 3 ].tex = D3DXVECTOR2( Jumper[ nCntJumper ].fPosTex.x + 0.5f, Jumper[ nCntJumper ].fPosTex.y + JUMPER_TEX_HEIGHT );	// �E���̒��_
		}

		pVtx += 4;
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferJumper->Unlock();	
}

/*------------------------------------------------------------------------------
  �@�`�揈��
------------------------------------------------------------------------------*/
void DrawJumper( void )
{
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �p�C�v���C���i�X�g���[���j��ݒ�
	pDevice->SetStreamSource( 0,						// �p�C�v���C���ԍ�
							  g_pVtxBufferJumper,		// �p�C�v���C���̌��ɂȂ�̈���w��
							  0,						// �I�t�Z�b�g�i�P�ʂ�byte�j���X�g���[���̊J�n�ʒu���w��ł���
							  sizeof( VERTEX_2D ) );	// �����f�[�^�̒P�ʃT�C�Y�i�X�g���C�h�ʁj

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0, g_pTextureJumper );

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF( FVF_VERTEX_2D );

	// �v���~�e�B�u�iPolygon�j�̕`��
	for( int nCnt = 0; nCnt < JUMPER_MAX; nCnt++ )
	{
		if( !Jumper[ nCnt ].bUse ) { continue; }

		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, // �v���~�e�B�u�̎��
								nCnt * 4,			 // �`��J�n�ʒu�̃I�t�Z�b�g�i���_���j
								NUM_POLYGON );		 // �`�悷��v���~�e�B�u�̐�
	}
}

/*------------------------------------------------------------------------------
  �@�Q�b�^�[
------------------------------------------------------------------------------*/
JUMPER* GetJumper( void )
{
	return Jumper;
}

