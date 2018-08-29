/*==============================================================================

    �|���S������ [item.cpp]
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
#include "item.h"
#include "player.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define ITEM_TEXTURENAME      "data/TEXTURE/sneaker03.png"		 // �e�N�X�`���t�@�C�����i�ʏ펞�j
#define ITEM_POS_X ( 0 )							 		 // �A�C�e���̕\���ʒuX
#define ITEM_POS_Y ( 0 )									 // �A�C�e���̕\���ʒuY

#define ITEM_TEX_WIDTH ( 1.0f )							 // �A�C�e���̃e�N�X�`����
#define ITEM_TEX_HEIGHT ( 1.0f )							 // �A�C�e���̃e�N�X�`������
#define ITEM_MOVE_SPEED ( 3.2f )							 // �A�C�e���̈ړ����x
#define ITEM_SCROLL_COEFFICIENT (800.0f)					 // �A�C�e���̈ړ���w�i�ɍ��킹��W��

/*------------------------------------------------------------------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTextureItem	= NULL;	// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferItem	= NULL; // ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

ITEM Item[ ITEM_MAX ];  // �A�C�e���̍\����

/*------------------------------------------------------------------------------
  �@����������
------------------------------------------------------------------------------*/
void InitItem( void )
{
	srand( ( unsigned )time ( 0 ) );	// �����ݒ�

	// �p�����[�^������
	for( int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++ )
	{
		Item[ nCntItem ].fPos.x	 = SCREEN_WIDTH * 1.5f;
		Item[ nCntItem ].fPos.y	 = ( rand() % ( int )( LIMIT_BOTTOM - LIMIT_TOP ) ) + LIMIT_TOP - ITEM_HEIGHT / 2.0f;
		Item[ nCntItem ].fPosTex.x = 0.0f;
		Item[ nCntItem ].fPosTex.y = 0.0f;
	}
	
	// ���_�̐ݒ�
	MakeVertexPolygon( &g_pVtxBufferItem, ITEM_MAX );
		
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					 // �f�o�C�X�̃|�C���^��n��
											ITEM_TEXTURENAME,			 // �e�N�X�`���̃t�@�C����
											&g_pTextureItem ) ) )		 // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK ); // �G���[�`�F�b�N
	}	
}

/*------------------------------------------------------------------------------
  �@�I������
------------------------------------------------------------------------------*/	
void UninitItem( void )
{
	// �e�N�X�`���C���^�[�t�F�[�X�̊J��
	if( g_pTextureItem != NULL )
	{
		g_pTextureItem->Release();
		g_pTextureItem = NULL;
	}
	
	// ���_�o�b�t�@�C���^�[�t�F�[�X�̊J��
	if( g_pVtxBufferItem != NULL )
	{
		g_pVtxBufferItem->Release();
		g_pVtxBufferItem = NULL;
	}
}

/*------------------------------------------------------------------------------
  �@�X�V����
------------------------------------------------------------------------------*/
void UpdateItem( void )
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
	g_pVtxBufferItem->Lock( 0,
							0,				// ���ׂĂ����b�N
							( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							0 );	

	// �X�V
	for( int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++ )
	{
		if( !Item[ nCntItem ].bUse ) { continue; }

		// ���W�̍X�V
		Item[ nCntItem ].fPos.x -= ( pMoveBg->fTexScrollSpeed * ITEM_SCROLL_COEFFICIENT );
		
		// �l���̍��W�̎Z�o
		Item[ nCntItem ].fLeftUp.x    = Item[ nCntItem ].fPos.x;
		Item[ nCntItem ].fLeftUp.y    = Item[ nCntItem ].fPos.y;
		Item[ nCntItem ].fRightUp.x   = Item[ nCntItem ].fPos.x + ITEM_WIDTH;
		Item[ nCntItem ].fRightUp.y   = Item[ nCntItem ].fPos.y;
		Item[ nCntItem ].fLeftDown.x  = Item[ nCntItem ].fPos.x;
		Item[ nCntItem ].fLeftDown.y  = Item[ nCntItem ].fPos.y + ITEM_HEIGHT;
		Item[ nCntItem ].fRightDown.x = Item[ nCntItem ].fPos.x + ITEM_WIDTH;
		Item[ nCntItem ].fRightDown.y = Item[ nCntItem ].fPos.y + ITEM_HEIGHT;
		
		// ���_���W�̐ݒ�
		pVtx[ 0 ].pos.x = Item[ nCntItem ].fLeftUp.x;   	// ����̒��_X
		pVtx[ 0 ].pos.y = Item[ nCntItem ].fLeftUp.y;   	// ����̒��_Y		
		pVtx[ 1 ].pos.x = Item[ nCntItem ].fRightUp.x;  	// �E��̒��_X
		pVtx[ 1 ].pos.y = Item[ nCntItem ].fRightUp.y;  	// �E��̒��_Y
		pVtx[ 2 ].pos.x = Item[ nCntItem ].fLeftDown.x; 	// �����̒��_X
		pVtx[ 2 ].pos.y = Item[ nCntItem ].fLeftDown.y; 	// �����̒��_Y
		pVtx[ 3 ].pos.x = Item[ nCntItem ].fRightDown.x;	// �E���̒��_X
		pVtx[ 3 ].pos.y = Item[ nCntItem ].fRightDown.y;	// �E���̒��_Y	

		// UV���W�̐ݒ�
		pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );					    // ����̒��_
		pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );					    // �E��̒��_
		pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );	// �����̒��_		
		pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );	// �����̒��_	

		pVtx += 4;
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferItem->Unlock();	
}

/*------------------------------------------------------------------------------
  �@�`�揈��
------------------------------------------------------------------------------*/
void DrawItem( void )
{
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �p�C�v���C���i�X�g���[���j��ݒ�
	pDevice->SetStreamSource( 0,						// �p�C�v���C���ԍ�
							  g_pVtxBufferItem,			// �p�C�v���C���̌��ɂȂ�̈���w��
							  0,						// �I�t�Z�b�g�i�P�ʂ�byte�j���X�g���[���̊J�n�ʒu���w��ł���
							  sizeof( VERTEX_2D ) );	// �����f�[�^�̒P�ʃT�C�Y�i�X�g���C�h�ʁj

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0, g_pTextureItem );

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF( FVF_VERTEX_2D );

	// �v���~�e�B�u�iPolygon�j�̕`��
	for( int nCnt = 0; nCnt < ITEM_MAX; nCnt++ )
	{
		if( !Item[ nCnt ].bUse ) { continue; }

		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, // �v���~�e�B�u�̎��
								nCnt * 4,			 // �`��J�n�ʒu�̃I�t�Z�b�g�i���_���j
								NUM_POLYGON );		 // �`�悷��v���~�e�B�u�̐�
	}
}

/*------------------------------------------------------------------------------
  �@�Q�b�^�[
------------------------------------------------------------------------------*/
ITEM* GetItem( void )
{
	return Item;
}

