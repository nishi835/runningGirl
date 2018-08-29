/*==============================================================================

    MoveBG���� [MoveBG.cpp]
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
#include "input.h"
#include "score.h"

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define MOVEBG_TEXTURENAME "data/TEXTURE/movebg07.png" // �e�N�X�`���t�@�C����
#define MOVEBG_POS_X  ( 0 )							   // MoveBG�̕\���ʒuX
#define MOVEBG_POS_Y  ( 210 )						   // MoveBG�̕\���ʒuY
#define MOVEBG_WIDTH  ( 800 )						   // MoveBG�̕�
#define MOVEBG_HEIGHT ( 250 )						   // MoveBG�̍���
#define MOVEBG_TEXTURE_WIDTH  ( 1.0f )				   // MoveBG�̕�
#define MOVEBG_TEXTURE_HEIGHT ( 1.0f )				   // MoveBG�̍���
#define MOVEBG_MAX (1)								   // MoveBG�̐�

/*------------------------------------------------------------------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9      g_pTextureMoveBG   = NULL;	// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferMoveBG = NULL;  // ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

MOVEBG MoveBG[ 2 ];										// MoveBG�̍\����

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
HRESULT MakeVertexMoveBG( LPDIRECT3DDEVICE9 pDevice );

/*------------------------------------------------------------------------------
  �@����������
------------------------------------------------------------------------------*/
void InitMoveBG( void )
{
	// MoveBG�̏�����
	for( int nCntMoveBG = 0; nCntMoveBG < MOVEBG_MAX; nCntMoveBG++ )
	{
		MoveBG[ nCntMoveBG ].fPosTexX        = 0.0f;
		MoveBG[ nCntMoveBG ].fPosTexY		 = 0.0f;
		MoveBG[ nCntMoveBG ].fTexScrollSpeed = 0.0f;
	}

	// ���_�̐ݒ�
	MakeVertexPolygon( &g_pVtxBufferMoveBG, MOVEBG_MAX );
	
	VERTEX_2D* pVtx = NULL; // VRAM�̉��z�A�h���X���i�[
	
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferMoveBG->Lock( 0,
							  0,				// ���ׂĂ����b�N
							  ( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							  0 );			
		
	for( int nCntMoveBG = 0; nCntMoveBG < MOVEBG_MAX; nCntMoveBG++ )
	{
		// ���_���W�̐ݒ�
		pVtx[ 0 ].pos = D3DXVECTOR3( MOVEBG_POS_X,				  MOVEBG_POS_Y,					0.0f );				// ����̒��_
		pVtx[ 1 ].pos = D3DXVECTOR3( MOVEBG_POS_X + MOVEBG_WIDTH, MOVEBG_POS_Y,					0.0f );				// �E��̒��_
		pVtx[ 2 ].pos = D3DXVECTOR3( MOVEBG_POS_X,				  MOVEBG_POS_Y + MOVEBG_HEIGHT, 0.0f );				// �����̒��_	
		pVtx[ 3 ].pos = D3DXVECTOR3( MOVEBG_POS_X + MOVEBG_WIDTH, MOVEBG_POS_Y + MOVEBG_HEIGHT, 0.0f );				// �E���̒��_
	
		// UV���W�̐ݒ�
		pVtx[ 0 ].tex = D3DXVECTOR2( MoveBG[ nCntMoveBG ].fPosTexX,						   MoveBG[ nCntMoveBG ].fPosTexY );							// ����̒��_
		pVtx[ 1 ].tex = D3DXVECTOR2( MoveBG[ nCntMoveBG ].fPosTexX + MOVEBG_TEXTURE_WIDTH, MoveBG[ nCntMoveBG ].fPosTexY );							// �E��̒��_
		pVtx[ 2 ].tex = D3DXVECTOR2( MoveBG[ nCntMoveBG ].fPosTexX,						   MoveBG[ nCntMoveBG ].fPosTexY + MOVEBG_TEXTURE_HEIGHT );	// �����̒��_	
		pVtx[ 3 ].tex = D3DXVECTOR2( MoveBG[ nCntMoveBG ].fPosTexX + MOVEBG_TEXTURE_WIDTH, MoveBG[ nCntMoveBG ].fPosTexY + MOVEBG_TEXTURE_HEIGHT );	// �E���̒��_

		pVtx += 4;
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferMoveBG->Unlock();

	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if( FAILED( D3DXCreateTextureFromFile( pDevice,				 // �f�o�C�X�̃|�C���^��n��
										   MOVEBG_TEXTURENAME,      // �e�N�X�`���̃t�@�C����
										   &g_pTextureMoveBG ) ) )  // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK ); // �G���[�`�F�b�N
	}
	

}

/*------------------------------------------------------------------------------
  �@�I������
------------------------------------------------------------------------------*/
void UninitMoveBG( void )
{
	UninitPolygon( &g_pTextureMoveBG, &g_pVtxBufferMoveBG );
}

/*------------------------------------------------------------------------------
  �@�X�V����
------------------------------------------------------------------------------*/
void UpdateMoveBG( void )
{	
	// �e�N�X�`���̈ʒu�X�V
	MoveBG[ 0 ].fPosTexX += MoveBG[ 0 ].fTexScrollSpeed;
	
	// �X�R�A�̍X�V����
	SetScore( MoveBG[ 0 ].fTexScrollSpeed * 10 ); // �X�R�A�̃Z�b�g

	VERTEX_2D* pVtx = NULL; // VRAM�̉��z�A�h���X���i�[
	
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferMoveBG->Lock( 0,
							  0,				// ���ׂĂ����b�N
							  ( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							  0 );			
		
	for( int nCntMoveBG = 0; nCntMoveBG < MOVEBG_MAX; nCntMoveBG++ )
	{
		// ���_���W�̐ݒ�
		pVtx[ 0 ].pos = D3DXVECTOR3( MOVEBG_POS_X,				  MOVEBG_POS_Y,					0.0f );				// ����̒��_
		pVtx[ 1 ].pos = D3DXVECTOR3( MOVEBG_POS_X + MOVEBG_WIDTH, MOVEBG_POS_Y,					0.0f );				// �E��̒��_
		pVtx[ 2 ].pos = D3DXVECTOR3( MOVEBG_POS_X,				  MOVEBG_POS_Y + MOVEBG_HEIGHT, 0.0f );				// �����̒��_	
		pVtx[ 3 ].pos = D3DXVECTOR3( MOVEBG_POS_X + MOVEBG_WIDTH, MOVEBG_POS_Y + MOVEBG_HEIGHT, 0.0f );				// �E���̒��_
	
		// UV���W�̐ݒ�
		pVtx[ 0 ].tex = D3DXVECTOR2( MoveBG[ nCntMoveBG ].fPosTexX,						   MoveBG[ nCntMoveBG ].fPosTexY );							// ����̒��_
		pVtx[ 1 ].tex = D3DXVECTOR2( MoveBG[ nCntMoveBG ].fPosTexX + MOVEBG_TEXTURE_WIDTH, MoveBG[ nCntMoveBG ].fPosTexY );							// �E��̒��_
		pVtx[ 2 ].tex = D3DXVECTOR2( MoveBG[ nCntMoveBG ].fPosTexX,						   MoveBG[ nCntMoveBG ].fPosTexY + MOVEBG_TEXTURE_HEIGHT );	// �����̒��_	
		pVtx[ 3 ].tex = D3DXVECTOR2( MoveBG[ nCntMoveBG ].fPosTexX + MOVEBG_TEXTURE_WIDTH, MoveBG[ nCntMoveBG ].fPosTexY + MOVEBG_TEXTURE_HEIGHT );	// �E���̒��_

		pVtx += 4;
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferMoveBG->Unlock();
}

/*------------------------------------------------------------------------------
  �@�`�揈��
------------------------------------------------------------------------------*/
void DrawMoveBG( void )
{
	DrawPolygon( g_pTextureMoveBG, g_pVtxBufferMoveBG, 1 );
}

/*------------------------------------------------------------------------------
  �@�Q�b�^�[
------------------------------------------------------------------------------*/
MOVEBG* GetMoveBG( void )
{
	return &MoveBG[ 0 ];
}