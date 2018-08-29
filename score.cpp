/*==============================================================================

    Score���� [Score.cpp]
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
#include "score.h"
#include "message.h"
#include "player.h"

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define SCORE_TEXTURENAME "data/TEXTURE/number05.png"			// �e�N�X�`���t�@�C����
#define SCORE_RED_TEXTURENAME "data/TEXTURE/number06.png"			// �e�N�X�`���t�@�C����
#define SCORE_TITLE_TEXTURENAME "data/TEXTURE/scoretitle.png"   // �e�N�X�`���t�@�C����
#define TIME_TITLE_TEXTURENAME "data/TEXTURE/timetitle.png"   // �e�N�X�`���t�@�C����
#define SCORE_POS_X ( 280.0f )									// Score�̕\���ʒuX
#define SCORE_POS_Y ( 0.0f )									// Score�̕\���ʒuY
#define SCORE_WIDTH ( 25 )										// Score�̕�
#define SCORE_HEIGHT ( 35 )										// Score�̍���
#define SCORE_TEXTURE_WIDTH ( 0.1f )							// Score�̃e�N�X�`����
#define SCORE_TEXTURE_HEIGHT ( 1.0f )							// Score�̃e�N�X�`������
#define SCORE_INTERVAL ( 10 )									// Score�̊Ԋu
#define SCORE_MAX_DIGIT ( 4 )									// Score�̍ő包��
#define SCORE_TITLE_POS_X ( 0.0f )								// Score���o���̕\���ʒuX
#define SCORE_TITLE_POS_Y ( 0.0f )								// Score���o���̕\���ʒuY
#define SCORE_TITLE_WIDTH ( 150 )								// Score���o���̕�
#define SCORE_TITLE_HEIGHT ( 40 )								// Score���o���̍���
#define TIME_POS_X ( 240.0f )									// Time�̕\���ʒuX
#define TIME_POS_Y ( 450.0f )									// Time�̕\���ʒuY
#define TIME_TITLE_POS_X ( 0.0f )								// Time���o���̕\���ʒuX
#define TIME_TITLE_POS_Y ( 450.0f )								// Time���o���̕\���ʒuY
#define TIME_MAX_DIGIT (3)										// Time�̍ő包��
#define LIMIT_TIME ( 130 )										// �����^�C��

/*------------------------------------------------------------------------------
  �@�\���̂̐錾
------------------------------------------------------------------------------*/
// Score�̍\����
typedef struct
{
	float fScore;		// �X�R�A�i�[
	int   nTime;        // �c�莞�Ԋi�[
	float fSizeVar;     // �����̃T�C�Y�̕ω���
}SCORE;

/*------------------------------------------------------------------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;			// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureScoreRed = NULL;			// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureScoreTitle = NULL;		// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTimeTitle  = NULL;		// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferScore = NULL;   // ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

SCORE Score;										// Score�̍\����
int   g_nTimer = 0; // �ėp�^�C�}�[

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
HRESULT MakeVertexScore( LPDIRECT3DDEVICE9 pDevice );

/*------------------------------------------------------------------------------
  �@����������
------------------------------------------------------------------------------*/
void InitScore( void )
{
	// Score�̏�����
	Score.fScore = 0.0f;
	Score.nTime  = LIMIT_TIME;
	Score.fSizeVar = 0.0f;

	// ���_�̐ݒ�
	MakeVertexPolygon( &g_pVtxBufferScore, SCORE_MAX_DIGIT + TIME_MAX_DIGIT + 2 );
	
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					// �f�o�C�X�̃|�C���^��n��
											SCORE_TEXTURENAME,			// �e�N�X�`���̃t�@�C����
											&g_pTextureScore ) ) )		// �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );  // �G���[�`�F�b�N
	}
	
	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					// �f�o�C�X�̃|�C���^��n��
											SCORE_RED_TEXTURENAME,			// �e�N�X�`���̃t�@�C����
											&g_pTextureScoreRed ) ) )		// �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );  // �G���[�`�F�b�N
	}

	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					// �f�o�C�X�̃|�C���^��n��
											SCORE_TITLE_TEXTURENAME,			// �e�N�X�`���̃t�@�C����
											&g_pTextureScoreTitle ) ) )		// �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );  // �G���[�`�F�b�N
	}

	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					// �f�o�C�X�̃|�C���^��n��
											TIME_TITLE_TEXTURENAME,			// �e�N�X�`���̃t�@�C����
											&g_pTextureTimeTitle ) ) )		// �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );  // �G���[�`�F�b�N
	}

	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					// �f�o�C�X�̃|�C���^��n��
											TIME_TITLE_TEXTURENAME,			// �e�N�X�`���̃t�@�C����
											&g_pTextureTimeTitle ) ) )		// �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );  // �G���[�`�F�b�N
	}
}
/*------------------------------------------------------------------------------
  �@�I������
------------------------------------------------------------------------------*/
void UninitScore( void )
{
	// �e�N�X�`���C���^�[�t�F�[�X�̉��
	if( g_pTextureScore != NULL )
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	// �e�N�X�`���C���^�[�t�F�[�X�̉��
	if( g_pTextureScore != NULL )
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
		
	// �e�N�X�`���C���^�[�t�F�[�X�̉��
	if( g_pTextureScoreTitle != NULL )
	{
		g_pTextureScoreTitle->Release();
		g_pTextureScoreTitle = NULL;
	}
	
	// �e�N�X�`���C���^�[�t�F�[�X�̉��
	if( g_pTextureTimeTitle != NULL )
	{
		g_pTextureTimeTitle->Release();
		g_pTextureTimeTitle = NULL;
	}

	// ���_�o�b�t�@�C���^�[�t�F�[�X�̉��
	if( g_pVtxBufferScore != NULL )
	{
		g_pVtxBufferScore->Release();
		g_pVtxBufferScore = NULL;
	}
}

/*------------------------------------------------------------------------------
  �@�X�V����
------------------------------------------------------------------------------*/
void UpdateScore( void )
{
	VERTEX_2D* pVtx = NULL;   // VRAM�̉��z�A�h���X���i�[
	
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferScore->Lock( 0,
							 0,				   // ���ׂĂ����b�N
							 ( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							 0 );			
	// ���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( SCORE_TITLE_POS_X ,					SCORE_TITLE_POS_Y,				        0.0f );	// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( SCORE_TITLE_POS_X + SCORE_TITLE_WIDTH, SCORE_TITLE_POS_Y,						0.0f );	// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( SCORE_TITLE_POS_X ,					SCORE_TITLE_POS_Y + SCORE_TITLE_HEIGHT, 0.0f );	// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( SCORE_TITLE_POS_X + SCORE_TITLE_WIDTH, SCORE_TITLE_POS_Y + SCORE_TITLE_HEIGHT, 0.0f );	// �E���̒��_

	pVtx += 4;
	
	// ���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( TIME_TITLE_POS_X ,				       TIME_TITLE_POS_Y,				      0.0f );	// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( TIME_TITLE_POS_X + SCORE_TITLE_WIDTH, TIME_TITLE_POS_Y,				  	  0.0f );	// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( TIME_TITLE_POS_X ,					   TIME_TITLE_POS_Y + SCORE_TITLE_HEIGHT, 0.0f );	// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( TIME_TITLE_POS_X + SCORE_TITLE_WIDTH, TIME_TITLE_POS_Y + SCORE_TITLE_HEIGHT, 0.0f );	// �E���̒��_
	
	pVtx += 4;
	
	int value = ( int )Score.fScore; // ����p�X�R�A�i�[
	int number;				  // �X�R�A�̐���

	for( int nCnt = 0; nCnt < SCORE_MAX_DIGIT; nCnt++ )
	{
		// �X�R�A����
		number = value % 10;			// �ŉ��ʌ��̎��o��
		value /= 10;					// �ŉ��ʌ��̍X�V
			
		// �X�R�A�����̃Z�b�g
		SetScoreNumber( pVtx, number, nCnt );
			
		pVtx += 4;
	}
	
	value = Score.nTime; // ����p�^�C���i�[
	for( int nCnt = 0; nCnt < TIME_MAX_DIGIT; nCnt++ )
	{
		// �^�C������
		number = value % 10;			// �ŉ��ʌ��̎��o��
		value /= 10;					// �ŉ��ʌ��̍X�V
			
		// �^�C�������̃Z�b�g
		SetTimeNumber( pVtx, number, nCnt );
			
		pVtx += 4;
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferScore->Unlock();

	// �^�C�}�[�̃f�N�������g
	MESSAGE* pMessage = GetMessage();
	if( g_nTimer % 60 == 0 && pMessage->nType == MESSAGE_TYPE_GO )
	{
		Score.nTime--;
	}

	if( Score.nTime <= 10 && pMessage->nType == MESSAGE_TYPE_GO )
	{
		if( g_nTimer % 60 < 30 )
		{
			Score.fSizeVar += 0.3f;
		}
		else
		{
			Score.fSizeVar -= 0.3f;
		}
	}

	// �^�C�}�[��0�ɂȂ�����
	if( Score.nTime == 0 && ( pMessage->nType != MESSAGE_TYPE_TIMEUP && pMessage->nType != MESSAGE_TYPE_GAMEOVER ) )
	{
		SetPlayerState( STATE_TIMEUP );
		SetMessageType( MESSAGE_TYPE_TIMEUP );
	}

	// �ėp�^�C�}�[�̃C���N�������g
	g_nTimer++;
}

/*------------------------------------------------------------------------------
  �@�`�揈��
------------------------------------------------------------------------------*/
void DrawScore( void )
{
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �p�C�v���C���i�X�g���[���j��ݒ�
	pDevice->SetStreamSource( 0,						// �p�C�v���C���ԍ�
							  g_pVtxBufferScore,		// �p�C�v���C���̌��ɂȂ�̈���w��
							  0,						// �I�t�Z�b�g�i�P�ʂ�byte�j���X�g���[���̊J�n�ʒu���w��ł���
							  sizeof( VERTEX_2D ) );	// �����f�[�^�̒P�ʃT�C�Y�i�X�g���C�h�ʁj

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF( FVF_VERTEX_2D );

	// �v���~�e�B�u�iPolygon�j�̕`��
	for( int nCnt = 0; nCnt < SCORE_MAX_DIGIT + TIME_MAX_DIGIT + 2; nCnt++ )
	{
		// �e�N�X�`���̐ݒ�
		if( nCnt == 0 )
		{
			pDevice->SetTexture( 0, g_pTextureScoreTitle );
		}
		else if( nCnt == 1 )
		{
			pDevice->SetTexture( 0, g_pTextureTimeTitle );
		}
		else if( nCnt >= SCORE_MAX_DIGIT + 2 && Score.nTime < 10 )
		{
			pDevice->SetTexture( 0, g_pTextureScoreRed );
		}
		else
		{
			pDevice->SetTexture( 0, g_pTextureScore );
		}

		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, // �v���~�e�B�u�̎��
								nCnt * 4,			 // �`��J�n�ʒu�̃I�t�Z�b�g�i���_���j
								NUM_POLYGON );		 // �`�悷��v���~�e�B�u�̐�
	}
}

/*------------------------------------------------------------------------------
  �@�X�R�A�����̐ݒ菈��
------------------------------------------------------------------------------*/
void SetScoreNumber( VERTEX_2D* pVtx, int number, int index )
{
	// ���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( SCORE_POS_X - ( SCORE_WIDTH + SCORE_INTERVAL ) * index,			   SCORE_POS_Y,				   0.0f );	// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( SCORE_POS_X - ( SCORE_WIDTH + SCORE_INTERVAL ) * index + SCORE_WIDTH, SCORE_POS_Y,				   0.0f );	// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( SCORE_POS_X - ( SCORE_WIDTH + SCORE_INTERVAL ) * index,			   SCORE_POS_Y + SCORE_HEIGHT, 0.0f );	// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( SCORE_POS_X - ( SCORE_WIDTH + SCORE_INTERVAL ) * index + SCORE_WIDTH, SCORE_POS_Y + SCORE_HEIGHT, 0.0f );	// �E���̒��_

	// UV���W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( number * SCORE_TEXTURE_WIDTH,						 0.0f );					// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( number * SCORE_TEXTURE_WIDTH + SCORE_TEXTURE_WIDTH, 0.0f );					// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( number * SCORE_TEXTURE_WIDTH,						 SCORE_TEXTURE_HEIGHT );	// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( number * SCORE_TEXTURE_WIDTH + SCORE_TEXTURE_WIDTH, SCORE_TEXTURE_HEIGHT );	// �E���̒��_
}

/*------------------------------------------------------------------------------
  �@�^�C�������̐ݒ菈��
------------------------------------------------------------------------------*/
void SetTimeNumber( VERTEX_2D* pVtx, int number, int index )
{
	// ���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( TIME_POS_X - ( SCORE_WIDTH + SCORE_INTERVAL + Score.fSizeVar ) * index,							    TIME_POS_Y + Score.fSizeVar,			    0.0f );	// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( TIME_POS_X - ( SCORE_WIDTH + SCORE_INTERVAL + Score.fSizeVar ) * index + SCORE_WIDTH + Score.fSizeVar, TIME_POS_Y + Score.fSizeVar,				0.0f );	// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( TIME_POS_X - ( SCORE_WIDTH + SCORE_INTERVAL + Score.fSizeVar ) * index,								TIME_POS_Y + Score.fSizeVar + SCORE_HEIGHT, 0.0f );	// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( TIME_POS_X - ( SCORE_WIDTH + SCORE_INTERVAL + Score.fSizeVar ) * index + SCORE_WIDTH + Score.fSizeVar, TIME_POS_Y + Score.fSizeVar + SCORE_HEIGHT, 0.0f );	// �E���̒��_

	// UV���W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( number * SCORE_TEXTURE_WIDTH,						 0.0f );					// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( number * SCORE_TEXTURE_WIDTH + SCORE_TEXTURE_WIDTH, 0.0f );					// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( number * SCORE_TEXTURE_WIDTH,						 SCORE_TEXTURE_HEIGHT );	// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( number * SCORE_TEXTURE_WIDTH + SCORE_TEXTURE_WIDTH, SCORE_TEXTURE_HEIGHT );	// �E���̒��_
}

/*------------------------------------------------------------------------------
  �@�X�R�A�̃Z�b�^�[
------------------------------------------------------------------------------*/
void SetScore( float fScore )
{
	Score.fScore += fScore;
}

float GetScore( void )
{
	return Score.fScore;
}