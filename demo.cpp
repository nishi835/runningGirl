/*==============================================================================

    demo���� [demo.cpp]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/5/31
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  �@�w�b�_�t�@�C��
------------------------------------------------------------------------------*/
#include "demo.h"
#include "main.h"
#include "polygon.h"
#include "input.h"
#include "bg.h"
#include "input.h"
#include "movebg.h"
#include "player.h"
#include "can.h"
#include "hurdle.h"
#include "goal.h"
#include "jumper.h"
#include "shadow.h"
#include "hiteffect.h"
#include "dasheffect.h"
#include "particle.h"
#include "score.h"
#include "message.h"
#include "item.h"
#include "game.h"
#include "sound.h"

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define DEMO_TEXTURENAME	  "data/TEXTURE/demoplay.png"					 // �e�N�X�`���t�@�C����
#define DEMO_WIDTH			  ( 200 )										 // Demo�̕�
#define DEMO_HEIGHT			  ( 50 )										 // Demo�̍���
#define DEMO_POS_X			  ( SCREEN_WIDTH * 0.5f - DEMO_WIDTH * 0.5f )	 // Demo�̕\���ʒuX
#define DEMO_POS_Y			  ( SCREEN_HEIGHT * 0.5f - DEMO_HEIGHT * 0.5f )	 // Demo�̕\���ʒuY
#define DEMO_TEXTUER_U		  ( 0.0 )										 // Demo�̃e�N�X�`��U�l
#define DEMO_TEXTUER_V		  ( 0.0 )										 // Demo�̃e�N�X�`��V�l
#define DEMO_TEXTURE_WIDTH    ( 1.0f )										 // Demo�̃e�N�X�`����
#define DEMO_TEXTURE_HEIGHT   ( 1.0f )										 // Demo�̃e�N�X�`������
#define DEMO_TIME			  (900)											 // Demo�̎���
#define DEMO_MESSAGE_INTERVAL (60)											 // Demo���b�Z�[�W�̓_�ŊԊu

/*------------------------------------------------------------------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureDemo = NULL;					// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferDemo = NULL;			// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

/*------------------------------------------------------------------------------
  �@����������
------------------------------------------------------------------------------*/
void InitDemo( void )
{
	// ���_�̐ݒ�
	MakeVertexPolygon( &g_pVtxBufferDemo, 1 );
	
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,						 // �f�o�C�X�̃|�C���^��n��
											DEMO_TEXTURENAME,			     // �e�N�X�`���̃t�@�C����
											&g_pTextureDemo ) ) )			 // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );		 // �G���[�`�F�b�N
	}

	// �w�i�̏�����
	InitBG();

	// �X�N���[���w�i�̏�����
	InitMoveBG();

	// �X�R�A�̏�����
	InitScore();
	
	// ���b�Z�[�W�̏�����
	InitMessage();
	
	// �v���C���[�̏�����
	InitPlayer();

	// �ۉe�̏�����
	InitShadow();

	// �ʂ̏�����
	InitCan();

	// �W�����v��̏�����
	InitJumper();

	// �n�[�h���̏�����
	InitHurdle();
	
	// �S�[���̏�����
	InitGoal();
	
	// �A�C�e���̏�����
	InitItem();
	
	// �p�[�e�B�N���̏�����
	InitParticle();
	
	// �q�b�g�G�t�F�N�g�̏�����
	InitHitEffect();
	
	// �_�b�V���G�t�F�N�g�̏�����
	InitDashEffect();
}

/*------------------------------------------------------------------------------
  �@�I������
------------------------------------------------------------------------------*/
void UninitDemo( void )
{
	UninitPolygon( &g_pTextureDemo, &g_pVtxBufferDemo );

	// BGM��off
    StopSound( SOUND_LABEL_BGM000 );

	// �w�i�̏I������
	UninitBG();
	
	// �X�N���[���w�i�̏I������
	UninitMoveBG();

	// �X�R�A�̏I������
	UninitScore();
	
	// ���b�Z�[�W�̏I������
	UninitMessage();

	// �v���C���[�̏I������
	UninitPlayer();

	// �ۉe�̏I������
	UninitShadow();

	// �ʂ̏I������
	UninitCan();

	// �W�����v��̏I������
	UninitJumper();

	// �n�[�h���̏I������
	UninitHurdle();	
	
	// �S�[���̏I������
	UninitGoal();	

	// �A�C�e���̏I������
	UninitItem();
	
	// �p�[�e�B�N���̏I������
	UninitParticle();

	// �q�b�g�G�t�F�N�g�̏I������
	UninitHitEffect();
	
	// �_�b�V���G�t�F�N�g�̏I������
	UninitDashEffect();
}

/*------------------------------------------------------------------------------
  �@�X�V����
------------------------------------------------------------------------------*/
void UpdateDemo( void )
{
	// �^�C�}�[�̐ݒ�
	static int nTimer = 0;

	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();

	VERTEX_2D* pVtx = NULL; // VRAM�̉��z�A�h���X���i�[
	
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferDemo->Lock( 0,
							0,					// ���ׂĂ����b�N
						    ( void** )&pVtx,    // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							0 );			

	// ���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( DEMO_POS_X,			   DEMO_POS_Y,				 0.0f );	// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( DEMO_POS_X + DEMO_WIDTH,  DEMO_POS_Y,				 0.0f );	// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( DEMO_POS_X,			   DEMO_POS_Y + DEMO_HEIGHT, 0.0f );	// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( DEMO_POS_X + DEMO_WIDTH,  DEMO_POS_Y + DEMO_HEIGHT, 0.0f );	// �E���̒��_
	
	// UV���W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( DEMO_TEXTUER_U,					  DEMO_TEXTUER_V );							// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( DEMO_TEXTUER_U + DEMO_TEXTURE_WIDTH, DEMO_TEXTUER_V );							// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( DEMO_TEXTUER_U,					  DEMO_TEXTUER_V + DEMO_TEXTURE_HEIGHT );	// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( DEMO_TEXTUER_U + DEMO_TEXTURE_WIDTH, DEMO_TEXTUER_V + DEMO_TEXTURE_HEIGHT );	// �E���̒��_

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferDemo->Unlock();	

	// ���b�Z�[�W�̏��擾
	MESSAGE* pMessage = GetMessage();

	switch( pMessage->nType )
	{ 
	case MESSAGE_TYPE_STAGE:
	case MESSAGE_TYPE_READY:
		// �w�i�̍X�V
		//UpdateBG();

		// �X�R�A�̍X�V
		UpdateScore();

		// ���b�Z�[�W�̍X�V
		UpdateMessage();

		// �ۉe�̍X�V
		UpdateShadow();
	
		// �L�[�{�[�h�̍X�V
		UpdateKeyboard();

		break;

	case MESSAGE_TYPE_GO:
	case MESSAGE_TYPE_GOAL:
		// �w�i�̍X�V
		UpdateBG();

		// �X�N���[���w�i�̍X�V
		UpdateMoveBG();

		// �X�R�A�̍X�V
		UpdateScore();
	
		// ���b�Z�[�W�̍X�V
		UpdateMessage();

		// �v���C���[�̍X�V
		UpdatePlayer();

		// �ۉe�̍X�V
		UpdateShadow();

		// �ʂ̍X�V
		UpdateCan();

		// �n�[�h���̍X�V
		UpdateHurdle();
	
		// �S�[���̍X�V
		UpdateGoal();
	
		// �W�����v��̍X�V
		UpdateJumper();
	
		// �A�C�e���̍X�V
		UpdateItem();
	
		// �p�[�e�B�N���̍X�V
		UpdateParticle();

		// �q�b�g�G�t�F�N�g�̍X�V
		UpdateHitEffect();
		
		// �_�b�V���G�t�F�N�g�̍X�V
		UpdateDashEffect();

		// �L�[�{�[�h�̍X�V
		UpdateKeyboard();

		break;
	}

	// ���[�h�ؑ�
  	if( GetKeyboardPress( DIK_SPACE ) || nTimer > DEMO_TIME )
	{
		SetMode( MODE_TITLE );
		nTimer = 0;
	}

	// �^�C�}�[�̃C���N�������g
	nTimer++;
}

/*------------------------------------------------------------------------------
  �@�`�揈��
------------------------------------------------------------------------------*/
void DrawDemo( void )
{
	// �^�C�}�[�̐ݒ�
	static int nTimer = 0;

	// �X�N���[���w�i�̕`��
	DrawMoveBG();

	// �w�i�̕`��
	DrawBG();

	// �X�R�A�̕`��
	DrawScore();
	
	// ���b�Z�[�W�̕`��
	DrawMessage();

	// ������@�̕`��
	DrawControl();
	
	// �ۉe�̕`��
	DrawShadow();

	// �n�[�h���̕`��
	DrawHurdle();
	
	// �S�[���̕`��
	DrawGoal();
	
	// �W�����v��̕`��
	DrawJumper();
	
	// �A�C�e���̕`��
	DrawItem();
	
	// �p�[�e�B�N���̕`��
	DrawParticle();

	// �q�b�g�G�t�F�N�g�̕`��
	DrawHitEffect();
	
	// �_�b�V���G�t�F�N�g�̕`��
	DrawDashEffect();

	// �v���C���[�̕`��
	DrawPlayer();

	// �ʂ̕`��
	DrawCan();
	
	// �f�����b�Z�[�W�̕\��
	if( nTimer < DEMO_MESSAGE_INTERVAL )
	{
		DrawPolygon( g_pTextureDemo, g_pVtxBufferDemo, 1 );
	}

	if( nTimer > DEMO_MESSAGE_INTERVAL * 2 )
	{
		nTimer = 0;
	}
	
	// �^�C�}�[�̃C���N�������g
	nTimer++;
}