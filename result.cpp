/*==============================================================================

    result���� [result.cpp]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/5/31
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  �@�w�b�_�t�@�C��
------------------------------------------------------------------------------*/
#include "result.h"
#include "main.h"
#include "polygon.h"
#include "input.h"
#include "score.h"
#include "game.h"

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define RESULT_TEXTURENAME "data/TEXTURE/result.png" // �e�N�X�`���t�@�C����
#define RESULTCLEAR_TEXTURENAME "data/TEXTURE/allclearedBG.png" // �e�N�X�`���t�@�C����
#define RESULTSCORE_TEXTURENAME "data/TEXTURE/number05.png" // �e�N�X�`���t�@�C����
#define RESULT_POS_X ( 0 )							 // Result�̕\���ʒuX
#define RESULT_POS_Y ( 0 )						 	 // Result�̕\���ʒuY
#define RESULT_WIDTH ( 800 )						 // Result�̕�
#define RESULT_HEIGHT ( 600 )						 // Result�̍���
#define RESULT_TEXTUER_U ( 0.0 )					 // Result�̃e�N�X�`��U�l
#define RESULT_TEXTUER_V ( 0.0 )					 // Result�̃e�N�X�`��V�l
#define RESULT_TEXTURE_WIDTH ( 1.0f )				 // Result�̃e�N�X�`����
#define RESULT_TEXTURE_HEIGHT ( 1.0f )				 // Result�̃e�N�X�`������

#define RESULTSCORE_POS_X ( 600.0f )									// Score�̕\���ʒuX
#define RESULTSCORE_POS_Y ( 335.0f )									// Score�̕\���ʒuY
#define RESULTSCORE_WIDTH ( 45 )										// Score�̕�
#define RESULTSCORE_HEIGHT ( 55 )										// Score�̍���
#define RESULTSCORE_TEXTURE_WIDTH ( 0.1f )							// Score�̃e�N�X�`����
#define RESULTSCORE_TEXTURE_HEIGHT ( 1.0f )							// Score�̃e�N�X�`������
#define RESULTSCORE_INTERVAL ( 10 )									// Score�̊Ԋu
#define RESULTSCORE_MAX_DIGIT ( 4 )									// Score�̍ő包��

/*------------------------------------------------------------------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureResult = NULL;					// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureResultScore = NULL;			// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferResult = NULL;			// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferResultScore = NULL;			// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
LPDIRECT3DTEXTURE9 g_pTextureAllClear = NULL;			// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferAllClear = NULL;			// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

int g_nResultTimer = 0;

/*------------------------------------------------------------------------------
  �@����������
------------------------------------------------------------------------------*/
void InitResult( void )
{
	g_nResultTimer = 0;

	// ���_�̐ݒ�
	MakeVertexPolygon( &g_pVtxBufferResult, 1 );
	
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,						 // �f�o�C�X�̃|�C���^��n��
											RESULT_TEXTURENAME,			     // �e�N�X�`���̃t�@�C����
											&g_pTextureResult ) ) )			 // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );		 // �G���[�`�F�b�N
	}

	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,						 // �f�o�C�X�̃|�C���^��n��
											RESULTSCORE_TEXTURENAME,			     // �e�N�X�`���̃t�@�C����
											&g_pTextureResultScore ) ) )			 // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );		 // �G���[�`�F�b�N
	}
	
	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,						 // �f�o�C�X�̃|�C���^��n��
											RESULTCLEAR_TEXTURENAME,			     // �e�N�X�`���̃t�@�C����
											&g_pTextureAllClear ) ) )			 // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );		 // �G���[�`�F�b�N
	}

	VERTEX_2D* pVtx = NULL; // VRAM�̉��z�A�h���X���i�[
	
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferResult->Lock( 0,
							 0,					// ���ׂĂ����b�N
						     ( void** )&pVtx,   // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							 0 );			

	// ���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( RESULT_POS_X,  RESULT_POS_Y, 0.0f );									// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( RESULT_POS_X + RESULT_WIDTH,  RESULT_POS_Y, 0.0f );					// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( RESULT_POS_X,  RESULT_POS_Y + RESULT_HEIGHT, 0.0f );					// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( RESULT_POS_X + RESULT_WIDTH,  RESULT_POS_Y + RESULT_HEIGHT, 0.0f );	// �E���̒��_
	
	// UV���W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( RESULT_TEXTUER_U,  RESULT_TEXTUER_V );												// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( RESULT_TEXTUER_U + RESULT_TEXTURE_WIDTH, RESULT_TEXTUER_V );							// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( RESULT_TEXTUER_U,  RESULT_TEXTUER_V + RESULT_TEXTURE_HEIGHT );						// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( RESULT_TEXTUER_U + RESULT_TEXTURE_WIDTH, RESULT_TEXTUER_V + RESULT_TEXTURE_HEIGHT );	// �E���̒��_

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferResult->Unlock();	
	
	// ���_�̐ݒ�
	MakeVertexPolygon( &g_pVtxBufferResultScore, RESULTSCORE_MAX_DIGIT );
	
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferResultScore->Lock( 0,
								   0,					// ���ׂĂ����b�N
								   ( void** )&pVtx,   // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
								   0 );			


	int value = ( int )GetScore(); // ����p�X�R�A�i�[
	int number;				  // �X�R�A�̐���

	for( int nCnt = 0; nCnt < RESULTSCORE_MAX_DIGIT; nCnt++ )
	{
		// �X�R�A����
		number = value % 10;			// �ŉ��ʌ��̎��o��
		value /= 10;					// �ŉ��ʌ��̍X�V
			
		// ���_���W�̐ݒ�
		pVtx[ 0 ].pos = D3DXVECTOR3( RESULTSCORE_POS_X - ( RESULTSCORE_WIDTH + RESULTSCORE_INTERVAL ) * nCnt,					  RESULTSCORE_POS_Y,				      0.0f );	// ����̒��_
		pVtx[ 1 ].pos = D3DXVECTOR3( RESULTSCORE_POS_X - ( RESULTSCORE_WIDTH + RESULTSCORE_INTERVAL ) * nCnt + RESULTSCORE_WIDTH, RESULTSCORE_POS_Y,				      0.0f );	// �E��̒��_
		pVtx[ 2 ].pos = D3DXVECTOR3( RESULTSCORE_POS_X - ( RESULTSCORE_WIDTH + RESULTSCORE_INTERVAL ) * nCnt,					  RESULTSCORE_POS_Y + RESULTSCORE_HEIGHT, 0.0f );	// �����̒��_	
		pVtx[ 3 ].pos = D3DXVECTOR3( RESULTSCORE_POS_X - ( RESULTSCORE_WIDTH + RESULTSCORE_INTERVAL ) * nCnt + RESULTSCORE_WIDTH, RESULTSCORE_POS_Y + RESULTSCORE_HEIGHT, 0.0f );	// �E���̒��_

		// UV���W�̐ݒ�
		pVtx[ 0 ].tex = D3DXVECTOR2( number * RESULTSCORE_TEXTURE_WIDTH,							 0.0f );					// ����̒��_
		pVtx[ 1 ].tex = D3DXVECTOR2( number * RESULTSCORE_TEXTURE_WIDTH + RESULTSCORE_TEXTURE_WIDTH, 0.0f );					// �E��̒��_
		pVtx[ 2 ].tex = D3DXVECTOR2( number * RESULTSCORE_TEXTURE_WIDTH,							 RESULTSCORE_TEXTURE_HEIGHT );	// �����̒��_	
		pVtx[ 3 ].tex = D3DXVECTOR2( number * RESULTSCORE_TEXTURE_WIDTH + RESULTSCORE_TEXTURE_WIDTH, RESULTSCORE_TEXTURE_HEIGHT );	// �E���̒��_
			
		pVtx += 4;
	}	
	
	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferResultScore->Unlock();	
}

/*------------------------------------------------------------------------------
  �@�I������
------------------------------------------------------------------------------*/
void UninitResult( void )
{
	UninitPolygon( &g_pTextureResult, &g_pVtxBufferResult );
	UninitPolygon( &g_pTextureResultScore, &g_pVtxBufferResultScore );
	UninitPolygon( &g_pTextureAllClear, &g_pVtxBufferAllClear );
}

/*------------------------------------------------------------------------------
  �@�X�V����
------------------------------------------------------------------------------*/
void UpdateResult( void )
{
	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();
	
	// ���[�h�ؑ�
  	if( GetKeyboardPress( DIK_SPACE ) )
	{
		SetMode( MODE_RANKING );
	}

	g_nResultTimer++;
}

/*------------------------------------------------------------------------------
  �@�`�揈��
------------------------------------------------------------------------------*/
void DrawResult( void )
{
	if( GetStage() >= STAGE_MAX && g_nResultTimer < 540 )
	{		
		DrawPolygon( g_pTextureAllClear, g_pVtxBufferResult, 1 );
	}
	else
	{
		DrawPolygon( g_pTextureResult, g_pVtxBufferResult, 1 );
		DrawPolygon( g_pTextureResultScore, g_pVtxBufferResultScore, RESULTSCORE_MAX_DIGIT );
	}
}