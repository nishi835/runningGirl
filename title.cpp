/*==============================================================================

    Title���� [title.cpp]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/5/31
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  �@�w�b�_�t�@�C��
------------------------------------------------------------------------------*/
#include "title.h"
#include "main.h"
#include "polygon.h"
#include "input.h"
#include "fade.h"

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define TITLE_TEXTURENAME "data/TEXTURE/runninggirltitle.png"  // �e�N�X�`���t�@�C����
#define TITLE_POS_X ( 0 )							// Title�̕\���ʒuX
#define TITLE_POS_Y ( 0 )							// Title�̕\���ʒuY
#define TITLE_WIDTH ( 800 )							// Title�̕�
#define TITLE_HEIGHT ( 600 )						// Title�̍���
#define TITLE_TEXTUER_U ( 0.0 )						// Title�̃e�N�X�`��U�l
#define TITLE_TEXTUER_V ( 0.0 )						// Title�̃e�N�X�`��V�l
#define TITLE_TEXTURE_WIDTH ( 1.0f )				// Title�̃e�N�X�`����
#define TITLE_TEXTURE_HEIGHT ( 1.0f )				// Title�̃e�N�X�`������

#define TITLE_MESSAGE_TEXTURENAME "data/TEXTURE/pressenter.png" // �e�N�X�`���t�@�C����
#define TITLE_MESSAGE_POS_X ( 300 )								// TitleMessage�̕\���ʒuX
#define TITLE_MESSAGE_POS_Y ( 500 )								// TitleMessage�̕\���ʒuY
#define TITLE_MESSAGE_WIDTH ( 200 )								// TitleMessage�̕�
#define TITLE_MESSAGE_HEIGHT ( 50 )								// TitleMessage�̍���
#define TITLE_MESSAGE_TEXTUER_U ( 0.0 )							// TitleMessage�̃e�N�X�`��U�l
#define TITLE_MESSAGE_TEXTUER_V ( 0.0 )							// TitleMessage�̃e�N�X�`��V�l
#define TITLE_MESSAGE_TEXTURE_WIDTH ( 1.0f )					// TitleMessage�̃e�N�X�`����
#define TITLE_MESSAGE_TEXTURE_HEIGHT ( 1.0f )					// TitleMessage�̃e�N�X�`������


#define TITLE_RUNNING_TEXTURENAME "data/TEXTURE/running.png"    // �e�N�X�`���t�@�C����
#define TITLE_RUNNING_POS_X ( SCREEN_WIDTH )					// TitleMessage�̕\���ʒuX
#define TITLE_RUNNING_POS_Y ( 170 )								// TitleMessage�̕\���ʒuY
#define TITLE_RUNNING_WIDTH ( 600 )								// TitleMessage�̕�
#define TITLE_RUNNING_HEIGHT ( 100 )							// TitleMessage�̍���
#define TITLE_RUNNING_TEXTUER_U ( 0.0 )							// TitleMessage�̃e�N�X�`��U�l
#define TITLE_RUNNING_TEXTUER_V ( 0.0 )							// TitleMessage�̃e�N�X�`��V�l
#define TITLE_RUNNING_TEXTURE_WIDTH ( 1.0f )					// TitleMessage�̃e�N�X�`����
#define TITLE_RUNNING_TEXTURE_HEIGHT ( 1.0f )					// TitleMessage�̃e�N�X�`������

#define TITLE_GIRL_TEXTURENAME "data/TEXTURE/girl.png"			// �e�N�X�`���t�@�C����
#define TITLE_GIRL_POS_X ( -500 )						// TitleMessage�̕\���ʒuX
#define TITLE_GIRL_POS_Y ( 300 )								// TitleMessage�̕\���ʒuY
#define TITLE_GIRL_WIDTH ( 600 )								// TitleMessage�̕�
#define TITLE_GIRL_HEIGHT ( 100 )								// TitleMessage�̍���

#define TITLE_CURSOR_TEXTURENAME "data/TEXTURE/sneaker03.png"			// �e�N�X�`���t�@�C����
#define TITLE_CURSOR_POS_Y (200)
#define TITLE_TIME (300)						    // �^�C�g���̕\������

#define MENU_MAX (3)

/*------------------------------------------------------------------------------
  �@�\����
------------------------------------------------------------------------------*/
typedef struct{
	D3DXVECTOR2 pos;
	float move;
	bool dodraw;
}TITLE;

typedef struct{
	D3DXVECTOR2 pos;
	D3DXVECTOR2 size;
	int poscur;
}CURSOR;

/*------------------------------------------------------------------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureTitle = NULL;					// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTitleMessage = NULL;			// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureRunning = NULL;				// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureGirl = NULL;					// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureCursor = NULL;					// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTitle = NULL;			// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTitleMessage = NULL;    // ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferRunning = NULL;			// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferGirl = NULL;			// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferCursor = NULL;			// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

TITLE Title[ 2 ];
CURSOR Cursor;

/*------------------------------------------------------------------------------
  �@����������
------------------------------------------------------------------------------*/
void InitTitle( void )
{
	// �ϐ�������
	Title[ 0 ].pos.x = TITLE_RUNNING_POS_X;
	Title[ 0 ].pos.y = TITLE_RUNNING_POS_Y;
	Title[ 1 ].pos.x = TITLE_GIRL_POS_X;
	Title[ 1 ].pos.y = TITLE_GIRL_POS_Y;
	Title[ 0 ].move  = 0.0f;
	Title[ 2 ].dodraw = false;

	// �J�[�\��������
	Cursor.pos.x = 300.0f;
	Cursor.pos.y = TITLE_CURSOR_POS_Y;
	Cursor.size.x = 50.0f;
	Cursor.size.y = 50.0f;
	Cursor.poscur = 0;
	
	// ���_�̐ݒ�
	MakeVertexPolygon( &g_pVtxBufferTitle, 1 );
	MakeVertexPolygon( &g_pVtxBufferTitleMessage, 1 );
	MakeVertexPolygon( &g_pVtxBufferRunning, 1 );
	MakeVertexPolygon( &g_pVtxBufferGirl, 1 );
	MakeVertexPolygon( &g_pVtxBufferCursor, 1 );
	
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,						 // �f�o�C�X�̃|�C���^��n��
											TITLE_TEXTURENAME,			     // �e�N�X�`���̃t�@�C����
											&g_pTextureTitle ) ) )			 // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );		 // �G���[�`�F�b�N
	}
	
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					 	 // �f�o�C�X�̃|�C���^��n��
											TITLE_MESSAGE_TEXTURENAME,       // �e�N�X�`���̃t�@�C����
											&g_pTextureTitleMessage ) ) )    // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );		 // �G���[�`�F�b�N
	}

	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					 	 // �f�o�C�X�̃|�C���^��n��
											TITLE_RUNNING_TEXTURENAME,       // �e�N�X�`���̃t�@�C����
											&g_pTextureRunning ) ) )    // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );		 // �G���[�`�F�b�N
	}
	
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					 	 // �f�o�C�X�̃|�C���^��n��
											TITLE_GIRL_TEXTURENAME,       // �e�N�X�`���̃t�@�C����
											&g_pTextureGirl ) ) )    // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );		 // �G���[�`�F�b�N
	}
	
	
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					 	 // �f�o�C�X�̃|�C���^��n��
											TITLE_CURSOR_TEXTURENAME,       // �e�N�X�`���̃t�@�C����
											&g_pTextureCursor ) ) )    // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );		 // �G���[�`�F�b�N
	}

	VERTEX_2D* pVtx = NULL; // VRAM�̉��z�A�h���X���i�[
	
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferTitle->Lock( 0,
							 0,					// ���ׂĂ����b�N
						     ( void** )&pVtx,   // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							 0 );			

	// ���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( TITLE_POS_X,  TITLE_POS_Y, 0.0f );									// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( TITLE_POS_X + TITLE_WIDTH,  TITLE_POS_Y, 0.0f );					// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( TITLE_POS_X,  TITLE_POS_Y + TITLE_HEIGHT, 0.0f );					// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( TITLE_POS_X + TITLE_WIDTH,  TITLE_POS_Y + TITLE_HEIGHT, 0.0f );	// �E���̒��_
	
	// UV���W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( TITLE_TEXTUER_U,  TITLE_TEXTUER_V );												// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( TITLE_TEXTUER_U + TITLE_TEXTURE_WIDTH, TITLE_TEXTUER_V );							// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( TITLE_TEXTUER_U,  TITLE_TEXTUER_V + TITLE_TEXTURE_HEIGHT );						// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( TITLE_TEXTUER_U + TITLE_TEXTURE_WIDTH, TITLE_TEXTUER_V + TITLE_TEXTURE_HEIGHT );	// �E���̒��_

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferTitle->Unlock();	
	
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferTitleMessage->Lock( 0,
							  	    0,				   // ���ׂĂ����b�N
								    ( void** )&pVtx,   // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
									0 );	
	
	pVtx[ 0 ].pos = D3DXVECTOR3( TITLE_MESSAGE_POS_X,  TITLE_MESSAGE_POS_Y, 0.0f );													// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( TITLE_MESSAGE_POS_X + TITLE_MESSAGE_WIDTH,  TITLE_MESSAGE_POS_Y, 0.0f );							// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( TITLE_MESSAGE_POS_X,  TITLE_MESSAGE_POS_Y + TITLE_MESSAGE_HEIGHT, 0.0f );							// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( TITLE_MESSAGE_POS_X + TITLE_MESSAGE_WIDTH,  TITLE_MESSAGE_POS_Y + TITLE_MESSAGE_HEIGHT, 0.0f );	// �E���̒��_

	pVtx[ 0 ].tex = D3DXVECTOR2( TITLE_MESSAGE_TEXTUER_U,  TITLE_MESSAGE_TEXTUER_V );																// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( TITLE_MESSAGE_TEXTUER_U + TITLE_MESSAGE_TEXTURE_WIDTH, TITLE_MESSAGE_TEXTUER_V );									// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( TITLE_MESSAGE_TEXTUER_U,  TITLE_MESSAGE_TEXTUER_V + TITLE_MESSAGE_TEXTURE_HEIGHT );								// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( TITLE_MESSAGE_TEXTUER_U + TITLE_MESSAGE_TEXTURE_WIDTH, TITLE_MESSAGE_TEXTUER_V + TITLE_MESSAGE_TEXTURE_HEIGHT );	// �E���̒��_

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferTitleMessage->Unlock();

		// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferRunning->Lock( 0,
							   0,				   // ���ׂĂ����b�N
							   ( void** )&pVtx,   // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							   0 );	
	
	pVtx[ 0 ].pos = D3DXVECTOR3( TITLE_RUNNING_POS_X,  TITLE_RUNNING_POS_Y, 0.0f );													// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( TITLE_RUNNING_POS_X + TITLE_RUNNING_WIDTH,  TITLE_RUNNING_POS_Y, 0.0f );							// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( TITLE_RUNNING_POS_X,  TITLE_RUNNING_POS_Y + TITLE_RUNNING_HEIGHT, 0.0f );							// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( TITLE_RUNNING_POS_X + TITLE_RUNNING_WIDTH,  TITLE_RUNNING_POS_Y + TITLE_RUNNING_HEIGHT, 0.0f );	// �E���̒��_

	pVtx[ 0 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U,  TITLE_RUNNING_TEXTUER_V );																// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U + TITLE_RUNNING_TEXTURE_WIDTH, TITLE_RUNNING_TEXTUER_V );									// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U,  TITLE_RUNNING_TEXTUER_V +    TITLE_RUNNING_TEXTURE_HEIGHT );								// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U + TITLE_RUNNING_TEXTURE_WIDTH, TITLE_RUNNING_TEXTUER_V + TITLE_RUNNING_TEXTURE_HEIGHT );	// �E���̒��_
		
	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferRunning->Unlock();
	
		// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferGirl->Lock( 0,
							   0,				   // ���ׂĂ����b�N
							   ( void** )&pVtx,   // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							   0 );	
	
	pVtx[ 0 ].pos = D3DXVECTOR3( TITLE_GIRL_POS_X,  TITLE_GIRL_POS_Y, 0.0f );											// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( TITLE_GIRL_POS_X + TITLE_GIRL_WIDTH,  TITLE_GIRL_POS_Y, 0.0f );						// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( TITLE_GIRL_POS_X,  TITLE_GIRL_POS_Y + TITLE_GIRL_HEIGHT, 0.0f );						// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( TITLE_GIRL_POS_X + TITLE_GIRL_WIDTH,  TITLE_GIRL_POS_Y + TITLE_GIRL_HEIGHT, 0.0f );	// �E���̒��_

	pVtx[ 0 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U,  TITLE_RUNNING_TEXTUER_V );																// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U + TITLE_RUNNING_TEXTURE_WIDTH, TITLE_RUNNING_TEXTUER_V );									// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U,  TITLE_RUNNING_TEXTUER_V +    TITLE_RUNNING_TEXTURE_HEIGHT );								// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U + TITLE_RUNNING_TEXTURE_WIDTH, TITLE_RUNNING_TEXTUER_V + TITLE_RUNNING_TEXTURE_HEIGHT );	// �E���̒��_
		
	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferGirl->Unlock();
	
		// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferCursor->Lock( 0,
							   0,				   // ���ׂĂ����b�N
							   ( void** )&pVtx,   // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							   0 );	
	
	pVtx[ 0 ].pos = D3DXVECTOR3( Cursor.pos.x,					Cursor.pos.y,				  0.0f );	// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( Cursor.pos.x + Cursor.size.x,	Cursor.pos.y,				  0.0f );	// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( Cursor.pos.x,					Cursor.pos.y + Cursor.size.y, 0.0f );	// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( Cursor.pos.x + Cursor.size.x,  Cursor.pos.y + Cursor.size.y, 0.0f );	// �E���̒��_

	pVtx[ 0 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U,  TITLE_RUNNING_TEXTUER_V );																// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U + TITLE_RUNNING_TEXTURE_WIDTH, TITLE_RUNNING_TEXTUER_V );									// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U,  TITLE_RUNNING_TEXTUER_V +    TITLE_RUNNING_TEXTURE_HEIGHT );								// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U + TITLE_RUNNING_TEXTURE_WIDTH, TITLE_RUNNING_TEXTUER_V + TITLE_RUNNING_TEXTURE_HEIGHT );	// �E���̒��_
		
	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferCursor->Unlock();
}

/*------------------------------------------------------------------------------
  �@�I������
------------------------------------------------------------------------------*/
void UninitTitle( void )
{
	UninitPolygon( &g_pTextureTitle, &g_pVtxBufferTitle );
	UninitPolygon( &g_pTextureTitleMessage, &g_pVtxBufferTitleMessage );
	UninitPolygon( &g_pTextureRunning, &g_pVtxBufferRunning );
	UninitPolygon( &g_pTextureGirl, &g_pVtxBufferGirl );
	UninitPolygon( &g_pTextureCursor, &g_pVtxBufferCursor );
}

/*------------------------------------------------------------------------------
  �@�X�V����
------------------------------------------------------------------------------*/
void UpdateTitle( void )
{
	// �^�C�}�[�̐ݒ�
	static int nTimer = 0;

	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();

	// �^�C�g���̈ړ�
	Title[ 0 ].move += 0.3f;
	if( Title[ 0 ].pos.x > 150.0f )
	{
		Title[ 0 ].pos.x -= Title[ 0 ].move;
		Title[ 1 ].pos.x += Title[ 0 ].move;
	}
	else if( nTimer % 30 == 0 )
	{		
		Title[ 2 ].dodraw =! Title[ 2 ].dodraw;
	}

	VERTEX_2D* pVtx = NULL; // VRAM�̉��z�A�h���X���i�[

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferRunning->Lock( 0,
							   0,				  // ���ׂĂ����b�N
							   ( void** )&pVtx,   // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							   0 );	
	
	pVtx[ 0 ].pos = D3DXVECTOR3( Title[ 0 ].pos.x,  Title[ 0 ].pos.y, 0.0f );												// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( Title[ 0 ].pos.x + TITLE_RUNNING_WIDTH,  Title[ 0 ].pos.y, 0.0f );							// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( Title[ 0 ].pos.x,  Title[ 0 ].pos.y + TITLE_RUNNING_HEIGHT, 0.0f );						// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( Title[ 0 ].pos.x + TITLE_RUNNING_WIDTH,  Title[ 0 ].pos.y + TITLE_RUNNING_HEIGHT, 0.0f );	// �E���̒��_
		
	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferRunning->Unlock();
	
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferGirl->Lock( 0,
							0,				  // ���ׂĂ����b�N
							( void** )&pVtx,   // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							0 );	
	
	pVtx[ 0 ].pos = D3DXVECTOR3( Title[ 1 ].pos.x,  Title[ 1 ].pos.y, 0.0f );											// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( Title[ 1 ].pos.x + TITLE_GIRL_WIDTH,  Title[ 1 ].pos.y, 0.0f );						// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( Title[ 1 ].pos.x,  Title[ 1 ].pos.y + TITLE_GIRL_HEIGHT, 0.0f );						// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( Title[ 1 ].pos.x + TITLE_GIRL_WIDTH,  Title[ 1 ].pos.y + TITLE_GIRL_HEIGHT, 0.0f );	// �E���̒��_
		
	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferGirl->Unlock();
		
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferCursor->Lock( 0,
							  0,				   // ���ׂĂ����b�N
							  ( void** )&pVtx,   // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							  0 );	
	
	pVtx[ 0 ].pos = D3DXVECTOR3( Cursor.pos.x,					Cursor.pos.y,				  0.0f );	// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( Cursor.pos.x + Cursor.size.x,	Cursor.pos.y,				  0.0f );	// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( Cursor.pos.x,					Cursor.pos.y + Cursor.size.y, 0.0f );	// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( Cursor.pos.x + Cursor.size.x,  Cursor.pos.y + Cursor.size.y, 0.0f );	// �E���̒��_

	pVtx[ 0 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U,  TITLE_RUNNING_TEXTUER_V );																// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U + TITLE_RUNNING_TEXTURE_WIDTH, TITLE_RUNNING_TEXTUER_V );									// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U,  TITLE_RUNNING_TEXTUER_V +    TITLE_RUNNING_TEXTURE_HEIGHT );								// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U + TITLE_RUNNING_TEXTURE_WIDTH, TITLE_RUNNING_TEXTUER_V + TITLE_RUNNING_TEXTURE_HEIGHT );	// �E���̒��_
		
	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferCursor->Unlock();

	// ���[�h�ؑ�
   	if( GetKeyboardTrigger( DIK_SPACE ) )
	{
		switch( Cursor.poscur )
		{
		case 0:
			SetFade( FADE_OUT, MODE_GAME );
			break;
		case 1:
			SetFade( FADE_OUT, MODE_RANKING );
			break;
		case 2:
			SetFade( FADE_OUT, MODE_TUTORIAL );
			break;
		}
	}

	// �J�[�\������
	if( GetKeyboardTrigger( DIK_S ) )
	{
		Cursor.poscur++;
		Cursor.pos.y += 75.0f;

		if( Cursor.poscur >= MENU_MAX )
		{
			Cursor.poscur = 0;
			Cursor.pos.y = TITLE_CURSOR_POS_Y;
		}

		nTimer = 0;
	}
	
	if( GetKeyboardTrigger( DIK_W ) )
	{
		Cursor.poscur--;
		Cursor.pos.y -= 75.0f;

		if( Cursor.poscur < 0 )
		{
			Cursor.poscur = MENU_MAX - 1;
			Cursor.pos.y = ( MENU_MAX - 1 ) * 75.0f + TITLE_CURSOR_POS_Y;
		}

		nTimer = 0;
	}

	if( nTimer > TITLE_TIME && GetFade() == FADE_NONE )
	{ 
		SetMode( MODE_DEMO );
		nTimer = 0;
	}

	// �^�C�}�[�̃C���N�������g
	nTimer++;
}

/*------------------------------------------------------------------------------
  �@�`�揈��
------------------------------------------------------------------------------*/
void DrawTitle( void )
{
	DrawPolygon( g_pTextureTitle, g_pVtxBufferTitle, 1 );
	if( Title[ 2 ].dodraw )
	{
		DrawPolygon( g_pTextureTitleMessage, g_pVtxBufferTitleMessage, 1 );
	}
	DrawPolygon( g_pTextureRunning, g_pVtxBufferRunning, 1 );
	DrawPolygon( g_pTextureGirl, g_pVtxBufferGirl, 1 );
	DrawPolygon( g_pTextureCursor, g_pVtxBufferCursor, 1 );
}