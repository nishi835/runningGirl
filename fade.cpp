/*==============================================================================

    Fade���� [fade.cpp]
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
#include "fade.h"

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define FADE_TEXTURENAME "data/TEXTURE/fade00.png"		 // �e�N�X�`���t�@�C����
#define FADE_POS_X ( 0 )								 // Fade�̕\���ʒuX
#define FADE_POS_Y ( 0 )								 // Fade�̕\���ʒuY
#define FADE_WIDTH ( 800 )								 // Fade�̕�
#define FADE_HEIGHT ( 600 )								 // Fade�̍���
#define FADE_TEXTURE_WIDTH ( 1.0f )						 // Fade�̕�
#define FADE_TEXTURE_HEIGHT ( 1.0f )					 // Fade�̍���
#define FADE_RATE ( 0.02f )								 // Fade�W��

/*------------------------------------------------------------------------------
  �@�\���̂̐錾
------------------------------------------------------------------------------*/
// Fade�̍\����
typedef struct
{
	float fPosTexX;		// Fade�̃e�N�X�`��X���W
	float fPosTexY;		// Fade�̃e�N�X�`��Y���W
}FADEHOGE;

/*------------------------------------------------------------------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;			// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferFade = NULL;    // ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

FADEHOGE Fade;										// Fade�̍\����
D3DXCOLOR g_colorFade;
FADE g_fade = FADE_IN;
MODE g_modeNext = MODE_TITLE;

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
HRESULT MakeVertexFade( LPDIRECT3DDEVICE9 pDevice );
HRESULT SetColorFade( D3DXCOLOR color );

/*------------------------------------------------------------------------------
  �@����������
------------------------------------------------------------------------------*/
void InitFade( void )
{
	// Fade�̏�����
	Fade.fPosTexX = 0.0f;
	Fade.fPosTexY = 0.0f;

	//
	g_colorFade = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );

	// ���_�̐ݒ�
	MakeVertexPolygon( &g_pVtxBufferFade, 1 );
	
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,				 // �f�o�C�X�̃|�C���^��n��
											FADE_TEXTURENAME,     // �e�N�X�`���̃t�@�C����
											&g_pTextureFade ) ) ) // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK ); // �G���[�`�F�b�N
	}
}

/*------------------------------------------------------------------------------
  �@�I������
------------------------------------------------------------------------------*/
void UninitFade( void )
{
	UninitPolygon( &g_pTextureFade, &g_pVtxBufferFade );
}

/*------------------------------------------------------------------------------
  �@�X�V����
------------------------------------------------------------------------------*/
void UpdateFade( void )
{
	if( g_fade == FADE_NONE )
	{
		return;
	}

	if( g_fade == FADE_IN )
	{
		g_colorFade.a -= FADE_RATE;
		
		if( g_colorFade.a <= 0.0f )
		{
			g_colorFade.a = 0.0f;
			g_fade = FADE_NONE;
		}
	}
	else if( g_fade == FADE_OUT )
	{
		g_colorFade.a += FADE_RATE;
		
		if( g_colorFade.a >= 1.0f )
		{
			g_colorFade.a = 1.0f;
			g_fade = FADE_IN;
			SetMode( g_modeNext );
		}
	}

	SetColorFade( g_colorFade );

	VERTEX_2D* pVtx = NULL; // VRAM�̉��z�A�h���X���i�[
	
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferFade->Lock( 0,
						    0,				// ���ׂĂ����b�N
						    ( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
						    0 );			

	// ���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( FADE_POS_X, FADE_POS_Y, 0.0f );							// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( FADE_POS_X + FADE_WIDTH, FADE_POS_Y, 0.0f );				// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( FADE_POS_X,  FADE_POS_Y + FADE_HEIGHT, 0.0f );				// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( FADE_POS_X + FADE_WIDTH,  FADE_POS_Y + FADE_HEIGHT, 0.0f );	// �E���̒��_
	
	// UV���W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( Fade.fPosTexX,  Fade.fPosTexY );							// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( Fade.fPosTexX + FADE_TEXTURE_WIDTH,  Fade.fPosTexY );				// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( Fade.fPosTexX,  Fade.fPosTexY		+ FADE_TEXTURE_HEIGHT );				// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( Fade.fPosTexX + FADE_TEXTURE_WIDTH,  Fade.fPosTexY + FADE_TEXTURE_HEIGHT );	// �E���̒��_

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferFade->Unlock();	

	// ���[�h�ؑ�
	//SetMode( g_modeNext );
}

/*------------------------------------------------------------------------------
  �@�`�揈��
------------------------------------------------------------------------------*/
void DrawFade( void )
{
	DrawPolygon( NULL, g_pVtxBufferFade, 1 );
}

/*------------------------------------------------------------------------------
  �@�Z�b�^�[
------------------------------------------------------------------------------*/
void SetFade( FADE fade, MODE modenext )
{
	g_fade = fade;
	g_modeNext = modenext;
}

/*------------------------------------------------------------------------------
  �@�Q�b�^�[
------------------------------------------------------------------------------*/
FADE GetFade( void )
{
	return g_fade;
}

/*------------------------------------------------------------------------------
  �@�F�ݒ菈��
------------------------------------------------------------------------------*/
HRESULT SetColorFade( D3DXCOLOR color )
{
	VERTEX_2D* pVtx; // VRAM�̉��z�A�h���X���i�[

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	if( FAILED ( g_pVtxBufferFade->Lock( 0,
										 0,				// ���ׂĂ����b�N
										 ( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
										 0 ) ) )			// ���b�N�t���O�H
										 {	
										 	  return E_FAIL;
										 }					
 
	// ���_�J���[�̐ݒ�i 0 �` 1�̐����l )
	pVtx[ 0 ].color = color;	// ����̒��_
	pVtx[ 1 ].color = color;	// �E��̒��_
	pVtx[ 2 ].color = color;	// �����̒��_
	pVtx[ 3 ].color = color;	// �E���̒��_

	// ���_�o�b�t�@�̃��b�N������
	( g_pVtxBufferFade )->Unlock();

	return S_OK;
}
