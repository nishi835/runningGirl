/*==============================================================================

    BG���� [BG.cpp]
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
#include "BG.h"
#include "movebg.h"

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define BG_TEXTURENAME "data/TEXTURE/sky04.png"					// �e�N�X�`���t�@�C����
#define BG_TEXTURENAME_AUDIENCE "data/TEXTURE/audience03.png" 
#define BG_TEXTURENAME_AUDIENCE02 "data/TEXTURE/audience01.png" 
#define BG_TEXTURENAME_NUMBERS "data/TEXTURE/numbers.png" 

#define BG_POS_X ( 0.0f )							// BG�̕\���ʒuX
#define BG_POS_Y ( 0.0f )							// BG�̕\���ʒuY
#define BG_WIDTH ( 800.0f )							// BG�̕�
#define BG_HEIGHT ( 210.0f )						// BG�̍���

#define BG_AUDIENCE_POS_X ( 0.0f )							// BG�̕\���ʒuX
#define BG_AUDIENCE_POS_Y ( 450.0f )						// BG�̕\���ʒuY
#define BG_AUDIENCE_WIDTH ( 800.0f )						// BG�̕�
#define BG_AUDIENCE_HEIGHT ( 150.0f )						// BG�̍���

#define BG_AUDIENCE02_POS_X ( 0.0f )						// BG�̕\���ʒuX
#define BG_AUDIENCE02_POS_Y ( 70.0f )						// BG�̕\���ʒuY
#define BG_AUDIENCE02_WIDTH ( 800.0f )						// BG�̕�
#define BG_AUDIENCE02_HEIGHT ( 80.0f )						// BG�̍���

#define BG_TEX_MAX (4)                          // �e�N�X�`���̍ő吔
#define BG_TEXTURE_WIDTH ( 1.0f )				// BG�̕�
#define BG_TEXTURE_HEIGHT ( 1.0f )				// BG�̍���
#define BG_TEXTURE_SCROLL_SPEED ( 0.002f )		// BG�̃e�N�X�`���X�N���[�����x
#define BG_SCROLL_COEFFICIENT (800.0f)			// BG�̃X�N���[���̌W��

#define BG_NUMBERS_POS_X  ( 0.0f )							   // numbers�̕\���ʒuX
#define BG_NUMBERS_POS_Y  ( 210.0f )						   // numbers�̕\���ʒuY
#define BG_NUMBERS_WIDTH  ( 800.0f )							   // numbers�̕�
#define BG_NUMBERS_HEIGHT ( 240.0f )						   // numbers�̍���

/*------------------------------------------------------------------------------
  �@�\���̂̐錾
------------------------------------------------------------------------------*/
// BG�̍\����
typedef struct
{
	D3DXVECTOR2 fPos;   // ���W
	float fPosTexX;		// BG�̃e�N�X�`��X���W
	float fPosTexY;		// BG�̃e�N�X�`��Y���W
}BG;

/*------------------------------------------------------------------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureBG[ BG_TEX_MAX ] = {};			// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DTEXTURE9      g_pTextureNumbers   = NULL;	// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBG = NULL;  // ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

BG Bg[ BG_TEX_MAX ];											// BG�̍\����

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
HRESULT MakeVertexBG( LPDIRECT3DDEVICE9 pDevice );

/*------------------------------------------------------------------------------
  �@����������
------------------------------------------------------------------------------*/
void InitBG( void )
{
	// BG�̏�����
	Bg[ 0 ].fPosTexX = 0.0f;
	Bg[ 0 ].fPosTexY = 0.0f;
	Bg[ 1 ].fPosTexX = 0.0f;
	Bg[ 1 ].fPosTexY = 0.0f;
	Bg[ 2 ].fPosTexX = 0.0f;
	Bg[ 2 ].fPosTexY = 0.0f;
	Bg[ 3 ].fPosTexX = 0.0f;
	Bg[ 3 ].fPosTexY = 0.0f;

	// ���_�̐ݒ�
	MakeVertexPolygon( &g_pVtxBufferBG, BG_TEX_MAX );
	
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,				 // �f�o�C�X�̃|�C���^��n��
											BG_TEXTURENAME,     // �e�N�X�`���̃t�@�C����
											&g_pTextureBG[ 0 ] ) ) ) // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK ); // �G���[�`�F�b�N
	}

	if( FAILED ( D3DXCreateTextureFromFile( pDevice,				 // �f�o�C�X�̃|�C���^��n��
											BG_TEXTURENAME_AUDIENCE,     // �e�N�X�`���̃t�@�C����
											&g_pTextureBG[ 1 ] ) ) ) // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK ); // �G���[�`�F�b�N
	}

	if( FAILED ( D3DXCreateTextureFromFile( pDevice,				 // �f�o�C�X�̃|�C���^��n��
											BG_TEXTURENAME_AUDIENCE02,     // �e�N�X�`���̃t�@�C����
											&g_pTextureBG[ 2 ] ) ) ) // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK ); // �G���[�`�F�b�N
	}
	
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,				 // �f�o�C�X�̃|�C���^��n��
											BG_TEXTURENAME_NUMBERS,     // �e�N�X�`���̃t�@�C����
											&g_pTextureBG[ 3 ] ) ) ) // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK ); // �G���[�`�F�b�N
	}
	
	VERTEX_2D* pVtx = NULL; // VRAM�̉��z�A�h���X���i�[

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferBG->Lock( 0,
						  0,				// ���ׂĂ����b�N
						  ( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
						  0 );			

	// ���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( BG_POS_X,			  BG_POS_Y,				0.0f );				// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( BG_POS_X + BG_WIDTH, BG_POS_Y,				0.0f );				// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( BG_POS_X,			  BG_POS_Y + BG_HEIGHT, 0.0f );				// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( BG_POS_X + BG_WIDTH, BG_POS_Y + BG_HEIGHT, 0.0f );				// �E���̒��_
	
	// ���_���W�̐ݒ�
	pVtx[ 4 ].pos = D3DXVECTOR3( BG_AUDIENCE_POS_X,					    BG_AUDIENCE_POS_Y,						0.0f );				// ����̒��_
	pVtx[ 5 ].pos = D3DXVECTOR3( BG_AUDIENCE_POS_X + BG_AUDIENCE_WIDTH, BG_AUDIENCE_POS_Y,						0.0f );				// �E��̒��_
	pVtx[ 6 ].pos = D3DXVECTOR3( BG_AUDIENCE_POS_X,					    BG_AUDIENCE_POS_Y + BG_AUDIENCE_HEIGHT, 0.0f );				// �����̒��_	
	pVtx[ 7 ].pos = D3DXVECTOR3( BG_AUDIENCE_POS_X + BG_AUDIENCE_WIDTH, BG_AUDIENCE_POS_Y + BG_AUDIENCE_HEIGHT, 0.0f );				// �E���̒��_
	
	// ���_���W�̐ݒ�
	pVtx[ 8 ].pos  = D3DXVECTOR3( BG_AUDIENCE02_POS_X,					   BG_AUDIENCE02_POS_Y,						   0.0f );				// ����̒��_
	pVtx[ 9 ].pos  = D3DXVECTOR3( BG_AUDIENCE02_POS_X + BG_AUDIENCE_WIDTH, BG_AUDIENCE02_POS_Y,						   0.0f );				// �E��̒��_
	pVtx[ 10 ].pos = D3DXVECTOR3( BG_AUDIENCE02_POS_X,					   BG_AUDIENCE02_POS_Y + BG_AUDIENCE02_HEIGHT, 0.0f );				// �����̒��_	
	pVtx[ 11 ].pos = D3DXVECTOR3( BG_AUDIENCE02_POS_X + BG_AUDIENCE_WIDTH, BG_AUDIENCE02_POS_Y + BG_AUDIENCE02_HEIGHT, 0.0f );				// �E���̒��_
	
	// ���_���W�̐ݒ�
	pVtx[ 12 ].pos = D3DXVECTOR3( Bg[ 3 ].fPos.x,				     BG_NUMBERS_POS_Y,					   0.0f );				// ����̒��_
	pVtx[ 13 ].pos = D3DXVECTOR3( Bg[ 3 ].fPos.x + BG_NUMBERS_WIDTH, BG_NUMBERS_POS_Y,					   0.0f );				// �E��̒��_
	pVtx[ 14 ].pos = D3DXVECTOR3( Bg[ 3 ].fPos.x,					 BG_NUMBERS_POS_Y + BG_NUMBERS_HEIGHT, 0.0f );				// �����̒��_	
	pVtx[ 15 ].pos = D3DXVECTOR3( Bg[ 3 ].fPos.x + BG_NUMBERS_WIDTH, BG_NUMBERS_POS_Y + BG_NUMBERS_HEIGHT, 0.0f );				// �E���̒��_

	// UV���W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( Bg[ 0 ].fPosTexX,					  Bg[ 0 ].fPosTexY );							// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( Bg[ 0 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 0 ].fPosTexY );							// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( Bg[ 0 ].fPosTexX,					  Bg[ 0 ].fPosTexY + BG_TEXTURE_HEIGHT );		// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( Bg[ 0 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 0 ].fPosTexY + BG_TEXTURE_HEIGHT );		// �E���̒��_
	
	// UV���W�̐ݒ�
	pVtx[ 4 ].tex = D3DXVECTOR2( Bg[ 1 ].fPosTexX,					  Bg[ 1 ].fPosTexY );							// ����̒��_
	pVtx[ 5 ].tex = D3DXVECTOR2( Bg[ 1 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 1 ].fPosTexY );							// �E��̒��_
	pVtx[ 6 ].tex = D3DXVECTOR2( Bg[ 1 ].fPosTexX,					  Bg[ 1 ].fPosTexY + BG_TEXTURE_HEIGHT );		// �����̒��_	
	pVtx[ 7 ].tex = D3DXVECTOR2( Bg[ 1 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 1 ].fPosTexY + BG_TEXTURE_HEIGHT );		// �E���̒��_
	
	pVtx[ 8 ].tex  = D3DXVECTOR2( Bg[ 2 ].fPosTexX,					   Bg[ 2 ].fPosTexY );							// ����̒��_
	pVtx[ 9 ].tex  = D3DXVECTOR2( Bg[ 2 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 2 ].fPosTexY );							// �E��̒��_
	pVtx[ 10 ].tex = D3DXVECTOR2( Bg[ 2 ].fPosTexX,					   Bg[ 2 ].fPosTexY + BG_TEXTURE_HEIGHT );		// �����̒��_	
	pVtx[ 11 ].tex = D3DXVECTOR2( Bg[ 2 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 2 ].fPosTexY + BG_TEXTURE_HEIGHT );		// �E���̒��_
	
	pVtx[ 12 ].tex = D3DXVECTOR2( Bg[ 3 ].fPosTexX,					   Bg[ 3 ].fPosTexY );							// ����̒��_
	pVtx[ 13 ].tex = D3DXVECTOR2( Bg[ 3 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 3 ].fPosTexY );							// �E��̒��_
	pVtx[ 14 ].tex = D3DXVECTOR2( Bg[ 3 ].fPosTexX,					   Bg[ 3 ].fPosTexY + BG_TEXTURE_HEIGHT );		// �����̒��_	
	pVtx[ 15 ].tex = D3DXVECTOR2( Bg[ 3 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 3 ].fPosTexY + BG_TEXTURE_HEIGHT );		// �E���̒��_
	
	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferBG->Unlock();	
}

/*------------------------------------------------------------------------------
  �@�I������
------------------------------------------------------------------------------*/
void UninitBG( void )
{
	// �e�N�X�`���C���^�[�t�F�[�X�̏I������
	for( int nCnt = 0; nCnt < BG_TEX_MAX; nCnt++ )
	{
		if( g_pTextureBG[ nCnt ] != NULL )
		{
			g_pTextureBG[ nCnt ]->Release();
			g_pTextureBG[ nCnt ] = NULL;
		}
	}

	// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^�̏I������
	if( g_pVtxBufferBG )
	{
		g_pVtxBufferBG->Release();
		g_pVtxBufferBG = NULL;
	}
}

/*------------------------------------------------------------------------------
  �@�X�V����
------------------------------------------------------------------------------*/
void UpdateBG( void )
{
	// �X�N���[���w�i�̎擾
	MOVEBG* pMoveBg = NULL;
	pMoveBg			= GetMoveBG();

	VERTEX_2D* pVtx = NULL; // VRAM�̉��z�A�h���X���i�[

	// �e�N�X�`�����W�̍X�V
	Bg[ 0 ].fPosTexX += pMoveBg->fTexScrollSpeed * 0.1f;
	Bg[ 1 ].fPosTexX += pMoveBg->fTexScrollSpeed;
	Bg[ 2 ].fPosTexX += pMoveBg->fTexScrollSpeed * 0.3f;
	Bg[ 3 ].fPos.x   -= pMoveBg->fTexScrollSpeed * BG_SCROLL_COEFFICIENT;

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferBG->Lock( 0,
						  0,				// ���ׂĂ����b�N
						  ( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
						  0 );			

	// ���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( BG_POS_X,			  BG_POS_Y,				0.0f );				// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( BG_POS_X + BG_WIDTH, BG_POS_Y,				0.0f );				// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( BG_POS_X,			  BG_POS_Y + BG_HEIGHT, 0.0f );				// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( BG_POS_X + BG_WIDTH, BG_POS_Y + BG_HEIGHT, 0.0f );				// �E���̒��_
	
	// ���_���W�̐ݒ�
	pVtx[ 4 ].pos = D3DXVECTOR3( BG_AUDIENCE_POS_X,					    BG_AUDIENCE_POS_Y,						0.0f );				// ����̒��_
	pVtx[ 5 ].pos = D3DXVECTOR3( BG_AUDIENCE_POS_X + BG_AUDIENCE_WIDTH, BG_AUDIENCE_POS_Y,						0.0f );				// �E��̒��_
	pVtx[ 6 ].pos = D3DXVECTOR3( BG_AUDIENCE_POS_X,					    BG_AUDIENCE_POS_Y + BG_AUDIENCE_HEIGHT, 0.0f );				// �����̒��_	
	pVtx[ 7 ].pos = D3DXVECTOR3( BG_AUDIENCE_POS_X + BG_AUDIENCE_WIDTH, BG_AUDIENCE_POS_Y + BG_AUDIENCE_HEIGHT, 0.0f );				// �E���̒��_
	
	// ���_���W�̐ݒ�
	pVtx[ 8 ].pos  = D3DXVECTOR3( BG_AUDIENCE02_POS_X,					   BG_AUDIENCE02_POS_Y,						   0.0f );				// ����̒��_
	pVtx[ 9 ].pos  = D3DXVECTOR3( BG_AUDIENCE02_POS_X + BG_AUDIENCE_WIDTH, BG_AUDIENCE02_POS_Y,						   0.0f );				// �E��̒��_
	pVtx[ 10 ].pos = D3DXVECTOR3( BG_AUDIENCE02_POS_X,					   BG_AUDIENCE02_POS_Y + BG_AUDIENCE02_HEIGHT, 0.0f );				// �����̒��_	
	pVtx[ 11 ].pos = D3DXVECTOR3( BG_AUDIENCE02_POS_X + BG_AUDIENCE_WIDTH, BG_AUDIENCE02_POS_Y + BG_AUDIENCE02_HEIGHT, 0.0f );				// �E���̒��_
	
	// ���_���W�̐ݒ�
	pVtx[ 12 ].pos = D3DXVECTOR3( Bg[ 3 ].fPos.x,				     BG_NUMBERS_POS_Y,					   0.0f );				// ����̒��_
	pVtx[ 13 ].pos = D3DXVECTOR3( Bg[ 3 ].fPos.x + BG_NUMBERS_WIDTH, BG_NUMBERS_POS_Y,					   0.0f );				// �E��̒��_
	pVtx[ 14 ].pos = D3DXVECTOR3( Bg[ 3 ].fPos.x,					 BG_NUMBERS_POS_Y + BG_NUMBERS_HEIGHT, 0.0f );				// �����̒��_	
	pVtx[ 15 ].pos = D3DXVECTOR3( Bg[ 3 ].fPos.x + BG_NUMBERS_WIDTH, BG_NUMBERS_POS_Y + BG_NUMBERS_HEIGHT, 0.0f );				// �E���̒��_

	// UV���W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( Bg[ 0 ].fPosTexX,					  Bg[ 0 ].fPosTexY );							// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( Bg[ 0 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 0 ].fPosTexY );							// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( Bg[ 0 ].fPosTexX,					  Bg[ 0 ].fPosTexY + BG_TEXTURE_HEIGHT );		// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( Bg[ 0 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 0 ].fPosTexY + BG_TEXTURE_HEIGHT );		// �E���̒��_
	
	// UV���W�̐ݒ�
	pVtx[ 4 ].tex = D3DXVECTOR2( Bg[ 1 ].fPosTexX,					  Bg[ 1 ].fPosTexY );							// ����̒��_
	pVtx[ 5 ].tex = D3DXVECTOR2( Bg[ 1 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 1 ].fPosTexY );							// �E��̒��_
	pVtx[ 6 ].tex = D3DXVECTOR2( Bg[ 1 ].fPosTexX,					  Bg[ 1 ].fPosTexY + BG_TEXTURE_HEIGHT );		// �����̒��_	
	pVtx[ 7 ].tex = D3DXVECTOR2( Bg[ 1 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 1 ].fPosTexY + BG_TEXTURE_HEIGHT );		// �E���̒��_
	
	pVtx[ 8 ].tex  = D3DXVECTOR2( Bg[ 2 ].fPosTexX,					   Bg[ 2 ].fPosTexY );							// ����̒��_
	pVtx[ 9 ].tex  = D3DXVECTOR2( Bg[ 2 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 2 ].fPosTexY );							// �E��̒��_
	pVtx[ 10 ].tex = D3DXVECTOR2( Bg[ 2 ].fPosTexX,					   Bg[ 2 ].fPosTexY + BG_TEXTURE_HEIGHT );		// �����̒��_	
	pVtx[ 11 ].tex = D3DXVECTOR2( Bg[ 2 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 2 ].fPosTexY + BG_TEXTURE_HEIGHT );		// �E���̒��_
	
	pVtx[ 12 ].tex = D3DXVECTOR2( Bg[ 3 ].fPosTexX,					   Bg[ 3 ].fPosTexY );							// ����̒��_
	pVtx[ 13 ].tex = D3DXVECTOR2( Bg[ 3 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 3 ].fPosTexY );							// �E��̒��_
	pVtx[ 14 ].tex = D3DXVECTOR2( Bg[ 3 ].fPosTexX,					   Bg[ 3 ].fPosTexY + BG_TEXTURE_HEIGHT );		// �����̒��_	
	pVtx[ 15 ].tex = D3DXVECTOR2( Bg[ 3 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 3 ].fPosTexY + BG_TEXTURE_HEIGHT );		// �E���̒��_
	
	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferBG->Unlock();	
}

/*------------------------------------------------------------------------------
  �@�`�揈��
------------------------------------------------------------------------------*/
void DrawBG( void )
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �X�g���[��(�p�C�v���C��)�ݒ�
	pDevice->SetStreamSource( 0,						// �q����p�C�v�̔ԍ�
							  g_pVtxBufferBG,			// �X�g���[���̌��ƂȂ钸�_�o�b�t�@�̎w��
							  0,						// �I�t�Z�b�g(�o�C�g)
							  sizeof( VERTEX_2D ) );	// �X�g���C�h�l(�X�g���C�h��)

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF( FVF_VERTEX_2D );
		
	for( int nCntBG = 0; nCntBG < BG_TEX_MAX; nCntBG++ )
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture( 0, g_pTextureBG[ nCntBG ] );	

		// �v���~�e�B�u�̕`��
		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
								nCntBG * 4,					// �I�t�Z�b�g(���_��)
								2 );						// �v���~�e�B�u��
	}
}
