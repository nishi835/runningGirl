/*==============================================================================

    Ranking���� [Ranking.cpp]
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
#include "ranking.h"
#include "player.h"
#include "input.h"
#include "fade.h"
#include "score.h"
#include "game.h"
#include <stdio.h>

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define RANKING_TEXTURENAME "data/TEXTURE/number05.png"		 // �e�N�X�`���t�@�C����
#define RANKING_TEXTURENAME02 "data/TEXTURE/ranking.jpg"	 // �e�N�X�`���t�@�C����
#define RANKING_TEXTURENAME03 "data/TEXTURE/all.png"	 // �e�N�X�`���t�@�C����
#define RANKING_MAX (5)									     // Ranking�̑���
#define RANKING_POS_X ( 450 )							     // Ranking�̕\���ʒuX
#define RANKING_POS_Y ( 230 )							     // Ranking�̕\���ʒuY
#define RANKING_STAGE_POS_X ( RANKING_POS_X + 180 )			 // Ranking�̃X�e�[�W���̕\���ʒuX
#define RANKING_STAGE_POS_Y ( 180 )							 // Ranking�̃X�e�[�W���̕\���ʒuY
#define RANKING_HEIGHT (68)									 // Ranking�̍���
#define RANKING_NUM_WIDTH ( 30 )							 // Ranking�̐����̕�
#define RANKING_NUM_HEIGHT ( 35 )							 // Ranking�̐����̍���
#define RANKING_ALL_WIDTH ( 70 )							 // Ranking��ALL�̕�
#define RANKING_ALL_HEIGHT ( 30 )							 // Ranking��ALL�̍���
#define RANKING_INTERVAL ( 10 )								 // Ranking�̊Ԋu
#define RANKING_MAX_DIGIT ( 4 )								 // Ranking�̍ő包��
#define RANKING_TEXTURE_WIDTH ( 0.1f )						 // Ranking�̃e�N�X�`����
#define RANKING_TEXTURE_HEIGHT ( 1.0f )						 // Ranking�̃e�N�X�`������

/*------------------------------------------------------------------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureRanking = NULL;		  	 // �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureRankingBG = NULL;			 // �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureRankingAll = NULL;			 // �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferRanking = NULL;		 // ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferRankingBG = NULL;    // ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferRankingAll = NULL;		 // ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

RANKING Ranking[5];										 // Ranking�̍\����
bool g_bRankIn = true;									 // �����N�C���������ǂ���
int g_Timer = 0;										 // �^�C�}�[

/*------------------------------------------------------------------------------
  �@����������
------------------------------------------------------------------------------*/
void InitRanking( void )
{
	for( int nCnt = 0; nCnt < 5; nCnt++ )
	{
		Ranking[ nCnt ].nStage = 0;
	}
	g_Timer = 0;

	// ���_�̐ݒ�
	MakeVertexPolygon( &g_pVtxBufferRanking, ( RANKING_MAX_DIGIT + 1 ) * RANKING_MAX );
	MakeVertexPolygon( &g_pVtxBufferRankingBG, 1 );
	MakeVertexPolygon( &g_pVtxBufferRankingAll, RANKING_MAX );
	
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					// �f�o�C�X�̃|�C���^��n��
											RANKING_TEXTURENAME,			// �e�N�X�`���̃t�@�C����
											&g_pTextureRanking ) ) )		// �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );  // �G���[�`�F�b�N
	}
	
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,						// �f�o�C�X�̃|�C���^��n��
											RANKING_TEXTURENAME02,			// �e�N�X�`���̃t�@�C����
											&g_pTextureRankingBG ) ) )		// �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );  // �G���[�`�F�b�N
	}
	
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,						// �f�o�C�X�̃|�C���^��n��
											RANKING_TEXTURENAME03,			// �e�N�X�`���̃t�@�C����
											&g_pTextureRankingAll ) ) )		// �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );  // �G���[�`�F�b�N
	}
	
	// �����L���O�̍쐬
	g_bRankIn = false;
	LoadRanking();
}

/*------------------------------------------------------------------------------
  �@�I������
------------------------------------------------------------------------------*/
void UninitRanking( void )
{
	UninitPolygon( &g_pTextureRanking, &g_pVtxBufferRanking );
	UninitPolygon( &g_pTextureRankingBG, &g_pVtxBufferRankingBG );
	UninitPolygon( &g_pTextureRankingAll, &g_pVtxBufferRankingAll );
}

/*------------------------------------------------------------------------------
  �@�X�V����
------------------------------------------------------------------------------*/
void UpdateRanking( void )
{
	// �L�[�{�[�h�X�V
	UpdateKeyboard();

	// �����L���O�����̃Z�b�g
	SetRankingNumber();

	// ���[�h�ؑ�
   	if( GetKeyboardTrigger( DIK_SPACE ) && g_Timer > 120 )
	{
		SetFade( FADE_OUT, MODE_TITLE );
	}

	g_Timer++;
}

/*------------------------------------------------------------------------------
  �@�����L���O�`�揈��
------------------------------------------------------------------------------*/
void DrawRanking( void )
{
	DrawPolygon( g_pTextureRankingBG, g_pVtxBufferRankingBG, 1 );
	DrawPolygon( g_pTextureRanking, g_pVtxBufferRanking, ( RANKING_MAX_DIGIT + 1 ) * RANKING_MAX );
	DrawPolygon( g_pTextureRankingAll, g_pVtxBufferRankingAll, RANKING_MAX );
} 

/*------------------------------------------------------------------------------
  �@�����L���O�����̐ݒ菈��
------------------------------------------------------------------------------*/
void SetRankingNumber( void )
{
	// �w�i�̏���
	VERTEX_2D* pVtx = NULL; // VRAM�̉��z�A�h���X���i�[
	
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferRankingBG->Lock( 0,
							     0,				// ���ׂĂ����b�N
							     ( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							     0 );			
	
	// ���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );		// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( 800.0f, 0.0f, 0.0f );		// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( 0.0f, 600.0f, 0.0f );		// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( 800.0f, 600.0f, 0.0f );	// �E���̒��_
	
	// UV���W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );				// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );				// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );				// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );				// �E���̒��_

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferRankingBG->Unlock();	
	
	// �����L���O�����̏���

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferRanking->Lock( 0,
							   0,				 // ���ׂĂ����b�N
							   ( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							   0 );			
		
	for( int nCntRank = 0; nCntRank < RANKING_MAX; nCntRank++ )
	{
		int value = ( int )Ranking[ nCntRank ].nScore; // ����p�����L���O�i�[
		int number;									   // �����L���O�̐���

		for( int nCnt = 0; nCnt < RANKING_MAX_DIGIT; nCnt++ )
		{
			// �����L���O����
			number = value % 10;			// �ŉ��ʌ��̎��o��
			value *= 0.1;					// �ŉ��ʌ��̍X�V

			// ���_���W�̐ݒ�			  
			pVtx[ 0 ].pos = D3DXVECTOR3( RANKING_POS_X - ( RANKING_NUM_WIDTH + RANKING_INTERVAL ) * nCnt,					  RANKING_POS_Y 					 + RANKING_HEIGHT * nCntRank, 0.0f );	// ����̒��_
			pVtx[ 1 ].pos = D3DXVECTOR3( RANKING_POS_X - ( RANKING_NUM_WIDTH + RANKING_INTERVAL ) * nCnt + RANKING_NUM_WIDTH, RANKING_POS_Y 					 + RANKING_HEIGHT * nCntRank, 0.0f );	// �E��̒��_
			pVtx[ 2 ].pos = D3DXVECTOR3( RANKING_POS_X - ( RANKING_NUM_WIDTH + RANKING_INTERVAL ) * nCnt,					  RANKING_POS_Y + RANKING_NUM_HEIGHT + RANKING_HEIGHT * nCntRank, 0.0f );	// �����̒��_	
			pVtx[ 3 ].pos = D3DXVECTOR3( RANKING_POS_X - ( RANKING_NUM_WIDTH + RANKING_INTERVAL ) * nCnt + RANKING_NUM_WIDTH, RANKING_POS_Y + RANKING_NUM_HEIGHT + RANKING_HEIGHT * nCntRank, 0.0f );	// �E���̒��_

			// UV���W�̐ݒ�
			pVtx[ 0 ].tex = D3DXVECTOR2( number * RANKING_TEXTURE_WIDTH,						 0.0f );					// ����̒��_
			pVtx[ 1 ].tex = D3DXVECTOR2( number * RANKING_TEXTURE_WIDTH + RANKING_TEXTURE_WIDTH, 0.0f );					// �E��̒��_
			pVtx[ 2 ].tex = D3DXVECTOR2( number * RANKING_TEXTURE_WIDTH,						 RANKING_TEXTURE_HEIGHT );	// �����̒��_	
			pVtx[ 3 ].tex = D3DXVECTOR2( number * RANKING_TEXTURE_WIDTH + RANKING_TEXTURE_WIDTH, RANKING_TEXTURE_HEIGHT );	// �E���̒��_

			// ���_�J���[�̐ݒ�i 0 �` 255�̐����l )
			if( /*g_bRankIn*/ 1)
			{
				static int nAlpha = 0;
				nAlpha++;

				pVtx[ 0 ].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );	// ����̒��_
				pVtx[ 1 ].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );	// �E��̒��_
				pVtx[ 2 ].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );	// �����̒��_
				pVtx[ 3 ].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );	// �E���̒��_
			}

			pVtx += 4;
		}

		// ���_���W�̐ݒ�			  
		pVtx[ 0 ].pos = D3DXVECTOR3( RANKING_STAGE_POS_X,					  RANKING_POS_Y 		  			 + RANKING_HEIGHT * nCntRank, 0.0f );	// ����̒��_
		pVtx[ 1 ].pos = D3DXVECTOR3( RANKING_STAGE_POS_X + RANKING_NUM_WIDTH, RANKING_POS_Y 					 + RANKING_HEIGHT * nCntRank, 0.0f );	// �E��̒��_
		pVtx[ 2 ].pos = D3DXVECTOR3( RANKING_STAGE_POS_X,					  RANKING_POS_Y + RANKING_NUM_HEIGHT + RANKING_HEIGHT * nCntRank, 0.0f );	// �����̒��_	
		pVtx[ 3 ].pos = D3DXVECTOR3( RANKING_STAGE_POS_X + RANKING_NUM_WIDTH, RANKING_POS_Y + RANKING_NUM_HEIGHT + RANKING_HEIGHT * nCntRank, 0.0f );	// �E���̒��_

		// UV���W�̐ݒ�
		pVtx[ 0 ].tex = D3DXVECTOR2( Ranking[ nCntRank ].nStage * RANKING_TEXTURE_WIDTH,						 0.0f );					// ����̒��_
		pVtx[ 1 ].tex = D3DXVECTOR2( Ranking[ nCntRank ].nStage * RANKING_TEXTURE_WIDTH + RANKING_TEXTURE_WIDTH, 0.0f );					// �E��̒��_
		pVtx[ 2 ].tex = D3DXVECTOR2( Ranking[ nCntRank ].nStage * RANKING_TEXTURE_WIDTH,						 RANKING_TEXTURE_HEIGHT );	// �����̒��_	
		pVtx[ 3 ].tex = D3DXVECTOR2( Ranking[ nCntRank ].nStage * RANKING_TEXTURE_WIDTH + RANKING_TEXTURE_WIDTH, RANKING_TEXTURE_HEIGHT );	// �E���̒��_

		pVtx += 4;
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferRanking->Unlock();
		
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferRankingAll->Lock( 0,
							   0,				 // ���ׂĂ����b�N
							   ( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							   0 );			

	for( int nCntRank = 0; nCntRank < RANKING_MAX; nCntRank++ )
	{	
		if( Ranking[ nCntRank ].nStage >= STAGE_MAX )
		{
			// ���_���W�̐ݒ�			  
			pVtx[ 0 ].pos = D3DXVECTOR3( RANKING_STAGE_POS_X,					  RANKING_POS_Y 		  			 + RANKING_HEIGHT * nCntRank, 0.0f );	// ����̒��_
			pVtx[ 1 ].pos = D3DXVECTOR3( RANKING_STAGE_POS_X + RANKING_ALL_WIDTH, RANKING_POS_Y 					 + RANKING_HEIGHT * nCntRank, 0.0f );	// �E��̒��_
			pVtx[ 2 ].pos = D3DXVECTOR3( RANKING_STAGE_POS_X,					  RANKING_POS_Y + RANKING_NUM_HEIGHT + RANKING_HEIGHT * nCntRank, 0.0f );	// �����̒��_	
			pVtx[ 3 ].pos = D3DXVECTOR3( RANKING_STAGE_POS_X + RANKING_ALL_WIDTH, RANKING_POS_Y + RANKING_NUM_HEIGHT + RANKING_HEIGHT * nCntRank, 0.0f );	// �E���̒��_

			// UV���W�̐ݒ�
			pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );	// ����̒��_
			pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );	// �E��̒��_
			pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );	// �����̒��_	
			pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );	// �E���̒��_

			pVtx += 4;
		}
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferRankingAll->Unlock();
}

/*------------------------------------------------------------------------------
  �@�����L���O�̏���
------------------------------------------------------------------------------*/
void MakeRanking( void )
{
	// �����N����
	int nScore = ( int )GetScore();
	int nRank = 0;
	bool bRankIn = false;
	int nCnt = RANKING_MAX - 1;
	while( 1 )
	{
		if( Ranking[ nCnt ].nScore > nScore )
		{
			nRank = nCnt + 1;
			break;
		}
		nCnt--;
		if( nCnt < 0 )
		{
			nRank = 0;
			break;
		}
	}

	// �����N�\�[�g
	if( nRank <= RANKING_MAX - 1 )
	{
		g_bRankIn = true;
		Ranking[ RANKING_MAX - 1 ].nScore = nScore;
		Ranking[ RANKING_MAX - 1 ].nStage = GetStage();

		for( int nCnt = RANKING_MAX - 1; nCnt > nRank; nCnt-- )
		{
			int work = Ranking[ nCnt - 1 ].nScore;
			Ranking[ nCnt - 1 ].nScore = Ranking[ nCnt ].nScore;
			Ranking[ nCnt ].nScore = work;

			work = Ranking[ nCnt - 1 ].nStage;
			Ranking[ nCnt - 1 ].nStage = Ranking[ nCnt ].nStage;
			Ranking[ nCnt ].nStage = work;
		}
	}
}

/*------------------------------------------------------------------------------
  �@�����L���O�̃Z�[�u
------------------------------------------------------------------------------*/
void SaveRanking( void )
{
	FILE* fp = NULL;
	if( ( fp = fopen( "data\\ranking.txt", "w" ) ) == NULL ) 
	{
		MessageBox( NULL, "�t�@�C���I�[�v���ł��܂���(�Z�[�u�j", "�G���[", MB_OK );
	}
	else
	{
		for( int nCnt = 0; nCnt < RANKING_MAX; nCnt++ )
		{
			fprintf( fp, "%d\n", Ranking[ nCnt ].nScore );
			fprintf( fp, "%d\n", Ranking[ nCnt ].nStage );
		}

		fclose( fp );
	}
}

/*------------------------------------------------------------------------------
  �@�����L���O�̃��[�h
------------------------------------------------------------------------------*/
void LoadRanking( void )
{	
	FILE* fp = NULL;
	if( ( fp = fopen( "data\\ranking.txt", "r" ) ) == NULL ) 
	{
		MessageBox( NULL, "�t�@�C���I�[�v���ł��܂���(���[�h�j", "�G���[", MB_OK );
	}
	else
	{
		for( int nCnt = 0; nCnt < RANKING_MAX; nCnt++ )
		{
			fscanf( fp, "%d", &Ranking[nCnt].nScore );
			fscanf( fp, "%d", &Ranking[nCnt].nStage );
		}

		fclose( fp );
	}
}

/*------------------------------------------------------------------------------
  �@�Q�b�^�[
------------------------------------------------------------------------------*/
bool GetRankIn( void )
{	
	return g_bRankIn;
}