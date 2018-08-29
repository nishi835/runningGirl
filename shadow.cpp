/*==============================================================================

    �ۉe�̏��� [shadow.cpp]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/4/17
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  �@�w�b�_�t�@�C��
------------------------------------------------------------------------------*/
#include "main.h"
#include "input.h"
#include "shadow.h"
#include "polygon.h"
#include "player.h"

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
// �Z�[�t�����[�X�}�N��
#define SAFE_RELEASE( x ) { if( x ) { ( x )->Release(); ( x ) = NULL; } }

#define SHADOW_TEXTURENAME "data/TEXTURE/effect000.jpg"
#define NUM_SHADOW (1)	// ��

/*-----------------------------------------------------------t-------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9      g_pTextureShadow   = NULL; // �e�N�X�`���C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferShadow = NULL; // ���_�o�b�t�@�C���^�[�t�F�[�X�̊i�[

SHADOW Shadow[ NUM_SHADOW ];

/*------------------------------------------------------------------------------
  �@�|���S���̏�����
------------------------------------------------------------------------------*/
HRESULT InitShadow( void )
{
	// �ۉe�̃p�����[�^������
	Shadow[ 0 ].fPos.x   = 0.0f;
	Shadow[ 0 ].fPos.y   = 0.0f;
	Shadow[ 0 ].fSize.x  = PLAYER_WIDTH * 0.6f;
	Shadow[ 0 ].fSize.y  = PLAYER_HEIGHT * 0.2f;
	Shadow[ 0 ].fColor   = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 0.7f );

	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// *** ���_�o�b�t�@�̍쐬 ***
	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_2D ) * NUM_SHADOW * 4,			  // �쐬���������_�o�b�t�@�̍��v�f�[�^�T�C�Y
											 D3DUSAGE_WRITEONLY,						 	  // ���_�o�b�t�@�̎g�p���@�i�������݂̂݁j
											 FVF_VERTEX_2D,								      // �H
											 D3DPOOL_MANAGED,								  // ���_�o�b�t�@�̒u����ƂȂ郁�������w��i�f�o�C�X�Ɉϑ��j
											 &g_pVtxBufferShadow,							  // ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�̃|�C���^�i�C���^�[�t�F�[�X�̊i�[�j
											 NULL ) ) )										  // �g��Ȃ�
											 {
												 return E_FAIL;
											 }	

	// *** ���_�o�b�t�@�̏����� ***										

	VERTEX_2D* pVtx; // VRAM�̉��z���_�A�h���X���i�[

	// ���_�o�b�t�@�����b�N���āA���z���_�A�h���X���擾����
	if( FAILED ( ( g_pVtxBufferShadow )->Lock( 0,
											   0,				 // ���ׂĂ����b�N
											   ( void** )&pVtx,  // ���z���_�A�h���X�p�|�C���^�̃A�h���X��n��
											   0 ) ) )			 // ���b�N�t���O�H
											   {	
										   	    return E_FAIL;
											   }		
	// ���_�f�[�^�̐ݒ�
	for( int nCnt = 0; nCnt < NUM_SHADOW; nCnt++ )
	{
			// ���_�J���[�̐ݒ�i 0 �` 255�̐����l )
			pVtx[ 0 ].color = Shadow[ 0 ].fColor;
			pVtx[ 1 ].color = Shadow[ 0 ].fColor;
			pVtx[ 2 ].color = Shadow[ 0 ].fColor;
			pVtx[ 3 ].color = Shadow[ 0 ].fColor;
		
			// rhw�̐ݒ�i�K��1.0f�j����O�����i�o�b�t�@�j�̏��H
			pVtx[ 0 ].rhw = 1.0f;	// ����̒��_
			pVtx[ 1 ].rhw = 1.0f;	// �E��̒��_
			pVtx[ 2 ].rhw = 1.0f;	// �����̒��_
			pVtx[ 3 ].rhw = 1.0f;	// �E���̒��_

			// ���_�f�[�^��UV�f�[�^��ǉ�
			pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
			pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
			pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
			pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );
		
			// ���_���W
			pVtx[ 0 ].pos  = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			pVtx[ 1 ].pos  = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			pVtx[ 2 ].pos  = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			pVtx[ 3 ].pos  = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			
			pVtx += 4;			
	}
	
	// ���_�o�b�t�@�̃��b�N������
	g_pVtxBufferShadow->Unlock();
		
	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice, SHADOW_TEXTURENAME, &g_pTextureShadow ) ) )
	{
		MessageBox( NULL, "�ǂݍ��ݎ��s", "�G���[", MB_OK ); 
	}
	
	return S_OK;
}

/*------------------------------------------------------------------------------
  �@�|���S���̏I������
------------------------------------------------------------------------------*/
void UninitShadow( void )
{
	// �I������
	// �e�N�X�`���C���^�[�t�F�[�X�̏I������
	if( g_pTextureShadow )
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^�̏I������
	if( g_pVtxBufferShadow )
	{
		g_pVtxBufferShadow->Release();
		g_pVtxBufferShadow = NULL;
	}
}

/*------------------------------------------------------------------------------
  �@�|���S���X�V����
------------------------------------------------------------------------------*/
void UpdateShadow( void )
{
	// �v���C���[�̏����擾
	PLAYER* pPlayer = GetPlayer();

	// �ۉe�̍��W�X�V
	Shadow[ 0 ].fPos.x = pPlayer->fAxis.x;
	if( pPlayer->State != STATE_JUMP && pPlayer->State != STATE_JUMPER )
	{
		Shadow[ 0 ].fPos.y = pPlayer->fAxis.y - 15.0f;
	}

	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	VERTEX_2D* pVtx; // VRAM�̉��z���_�A�h���X���i�[

	// ���_�o�b�t�@�����b�N���āA���z���_�A�h���X���擾����
	g_pVtxBufferShadow->Lock( 0,
							  0,				 // ���ׂĂ����b�N
							  ( void** )&pVtx,   // ���z���_�A�h���X�p�|�C���^�̃A�h���X��n��
							  0 );				 // ���b�N�t���O�H

	// ���_�f�[�^�̐ݒ�
	for( int nCnt = 0; nCnt < NUM_SHADOW; nCnt++ )
	{
			// ���_�J���[�̐ݒ�i 0 �` 255�̐����l )
			pVtx[ 0 ].color = Shadow[ 0 ].fColor;
			pVtx[ 1 ].color = Shadow[ 0 ].fColor;
			pVtx[ 2 ].color = Shadow[ 0 ].fColor;
			pVtx[ 3 ].color = Shadow[ 0 ].fColor;
		
			// ���_�f�[�^��UV�f�[�^��ǉ�
			pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
			pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
			pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
			pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );
		
			// ���_���W
			pVtx[ 0 ].pos  = D3DXVECTOR3( Shadow[ nCnt ].fPos.x - Shadow[ nCnt ].fSize.x * 0.5f, Shadow[ nCnt ].fPos.y						   , 0.0f );
			pVtx[ 1 ].pos  = D3DXVECTOR3( Shadow[ nCnt ].fPos.x + Shadow[ nCnt ].fSize.x * 0.5f, Shadow[ nCnt ].fPos.y						   , 0.0f );
			pVtx[ 2 ].pos  = D3DXVECTOR3( Shadow[ nCnt ].fPos.x - Shadow[ nCnt ].fSize.x * 0.5f, Shadow[ nCnt ].fPos.y + Shadow[ nCnt ].fSize.y, 0.0f );
			pVtx[ 3 ].pos  = D3DXVECTOR3( Shadow[ nCnt ].fPos.x + Shadow[ nCnt ].fSize.x * 0.5f, Shadow[ nCnt ].fPos.y + Shadow[ nCnt ].fSize.y, 0.0f );
			
			pVtx += 4;			
	}
	
	// ���_�o�b�t�@�̃��b�N������
	g_pVtxBufferShadow->Unlock();
}

/*------------------------------------------------------------------------------
  �@�|���S���`�揈��
------------------------------------------------------------------------------*/
void DrawShadow( void )
{
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// �A���t�@�u�����h�i�������j�̐ݒ�
	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );		  // �A���t�@�u�����h���L�����ǂ���
    pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT ); // �A���t�@�u�����h�̉��Z����ݒ�, �����Z�ɐݒ�
	pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );	  // �`�挳�i�X�v���C�g�j�̃A���t�@�u�����h�ݒ�
	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE);		  // �`���i�w�i�j�̃A���t�@�u�����h�ݒ�

	// �p�C�v���C���i�X�g���[���j��ݒ�
	pDevice->SetStreamSource( 0,						// �p�C�v���C���ԍ�
							  g_pVtxBufferShadow,		// �p�C�v���C���̌��ɂȂ�̈���w��
							  0,						// �I�t�Z�b�g�i�P�ʂ�byte�j���X�g���[���̊J�n�ʒu���w��ł���
							  sizeof( VERTEX_2D ) );	// �����f�[�^�̒P�ʃT�C�Y�i�X�g���C�h�ʁj

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0, g_pTextureShadow );

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF( FVF_VERTEX_2D );

	for( int nCnt = 0; nCnt < NUM_SHADOW; nCnt++ )
	{
		// �v���~�e�B�u�i�|���S���j�̕`��
		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
								nCnt * 4,				// �I�t�Z�b�g
								NUM_SHADOW * 2			// �v���~�e�B�u�̐�
								);
	}
	
	// �A���t�@�u�����h�i�������j�̐ݒ�����ɖ߂�
	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );		  // �A���t�@�u�����h���L�����ǂ���
	pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );		  // �A���t�@�u�����h�̉��Z����ݒ�, �����Z�ɐݒ�
	pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );	  // �`�挳�i�X�v���C�g�j�̃A���t�@�u�����h�ݒ�
	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);  // �`���i�w�i�j�̃A���t�@�u�����h�ݒ�
}