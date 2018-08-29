/*==============================================================================

    tutorial�̏��� [tutorial.cpp]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/5/31
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  �@�w�b�_�t�@�C��
------------------------------------------------------------------------------*/
#include "tutorial.h"
#include "main.h"
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
#include "polygon.h"
#include "input.h"
#include "fade.h"
#include <stdio.h>

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define TUTORIAL_WINDOW_TEXTURENAME "data/TEXTURE/tutorialwindows.png" // �e�N�X�`���t�@�C����
#define TUTORIAL_WINDOW_POS_X ( 100 )								// TutorialMessage�̕\���ʒuX
#define TUTORIAL_WINDOW_POS_Y ( 300 )								// TutorialMessage�̕\���ʒuY
#define TUTORIAL_WINDOW_WIDTH ( 600 )								// TutorialMessage�̕�
#define TUTORIAL_WINDOW_HEIGHT ( 250 )								// TutorialMessage�̍���
#define TUTORIAL_WINDOW_TEXTUER_U ( 0.0 )							// TutorialMessage�̃e�N�X�`��U�l
#define TUTORIAL_WINDOW_TEXTUER_V ( 0.0 )							// TutorialMessage�̃e�N�X�`��V�l
#define TUTORIAL_WINDOW_TEXTURE_WIDTH ( 1.0f )						// TutorialMessage�̃e�N�X�`����
#define TUTORIAL_WINDOW_TEXTURE_HEIGHT ( 0.33f )					// TutorialMessage�̃e�N�X�`������

#define TUTORIAL_WINDOW_TIME (300)						    // �^�C�g���̕\������

/*------------------------------------------------------------------------------
  �@�\����
------------------------------------------------------------------------------*/
typedef struct
{
	int Phase;
	int Timer;
	int StopTimer;
	bool Move;
}TUTORIAL;

/*------------------------------------------------------------------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureTutorial = NULL;					// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTutorial = NULL;			// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

TUTORIAL Tutorial;

/*------------------------------------------------------------------------------
  �@����������
------------------------------------------------------------------------------*/
void InitTutorial( void )
{
	// ������
	Tutorial.Phase = 0;
	Tutorial.Timer = 0;
	Tutorial.StopTimer = 0;
	Tutorial.Move = false;

	InitBG();
	InitMoveBG();
	InitScore();	
	InitShadow();
	InitPlayer();

	InitCan();
	InitJumper();
	InitHurdle();
	InitGoal();
	InitItem();

	InitParticle();
	InitHitEffect();
	InitDashEffect();

	// ���[�h
	LoadTutorial();

	// ���_�̐ݒ�
	MakeVertexPolygon( &g_pVtxBufferTutorial, 1 );
	
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,						 // �f�o�C�X�̃|�C���^��n��
											TUTORIAL_WINDOW_TEXTURENAME,			     // �e�N�X�`���̃t�@�C����
											&g_pTextureTutorial ) ) )			 // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );		 // �G���[�`�F�b�N
	}

	VERTEX_2D* pVtx = NULL; // VRAM�̉��z�A�h���X���i�[
	
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferTutorial->Lock( 0,
							 0,					// ���ׂĂ����b�N
						     ( void** )&pVtx,   // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							 0 );			

	// ���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( TUTORIAL_WINDOW_POS_X,  TUTORIAL_WINDOW_POS_Y, 0.0f );									// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( TUTORIAL_WINDOW_POS_X + TUTORIAL_WINDOW_WIDTH,  TUTORIAL_WINDOW_POS_Y, 0.0f );					// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( TUTORIAL_WINDOW_POS_X,  TUTORIAL_WINDOW_POS_Y + TUTORIAL_WINDOW_HEIGHT, 0.0f );					// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( TUTORIAL_WINDOW_POS_X + TUTORIAL_WINDOW_WIDTH,  TUTORIAL_WINDOW_POS_Y + TUTORIAL_WINDOW_HEIGHT, 0.0f );	// �E���̒��_
	
	// UV���W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( TUTORIAL_WINDOW_TEXTUER_U,									TUTORIAL_WINDOW_TEXTUER_V );												// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( TUTORIAL_WINDOW_TEXTUER_U + TUTORIAL_WINDOW_TEXTURE_WIDTH, TUTORIAL_WINDOW_TEXTUER_V );							// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( TUTORIAL_WINDOW_TEXTUER_U,									TUTORIAL_WINDOW_TEXTUER_V + TUTORIAL_WINDOW_TEXTURE_HEIGHT );						// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( TUTORIAL_WINDOW_TEXTUER_U + TUTORIAL_WINDOW_TEXTURE_WIDTH, TUTORIAL_WINDOW_TEXTUER_V + TUTORIAL_WINDOW_TEXTURE_HEIGHT );	// �E���̒��_

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferTutorial->Unlock();	
}

/*------------------------------------------------------------------------------
  �@�I������
------------------------------------------------------------------------------*/
void UninitTutorial( void )
{
	UninitPolygon( &g_pTextureTutorial, &g_pVtxBufferTutorial );
	
	UninitBG();
	UninitMoveBG();
	UninitPlayer();
	UninitScore();

	UninitCan();
	UninitJumper();
	UninitHurdle();
	UninitGoal();
	UninitItem();
}

/*------------------------------------------------------------------------------
  �@�X�V����
------------------------------------------------------------------------------*/
void UpdateTutorial( void )
{
	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();
	
	if( Tutorial.Move )
	{
		UpdateBG();
		UpdateMoveBG();
		UpdatePlayer();
		UpdateScore();

		UpdateCan();
		UpdateJumper();
		UpdateHurdle();
		UpdateGoal();
		UpdateItem();
	}
	
	if( Tutorial.Phase == 0 )
	{
		Tutorial.StopTimer++;
		if( Tutorial.StopTimer > 180 )
		{
			Tutorial.Phase++;
			Tutorial.StopTimer = 0;
		}
	}

	if( Tutorial.Phase == 1 )
	{
		Tutorial.StopTimer++;
		if( Tutorial.StopTimer > 180 )
		{
			Tutorial.Phase++;
			Tutorial.StopTimer = 0;
			Tutorial.Move = true;
		}
	}

	if( Tutorial.Timer > 540 && Tutorial.Phase == 2 )
	{
		Tutorial.Move = false;
	}
	
	if( Tutorial.Phase == 2 && !Tutorial.Move )
	{
		Tutorial.StopTimer++;
		if( Tutorial.StopTimer > 180 )
		{
			Tutorial.Phase++;
			Tutorial.StopTimer = 0;
			Tutorial.Move = true;
		}
	}

	if( Tutorial.Timer == 600 )
	{
		MovePlayerPos( D3DXVECTOR2( 0.0f, 60.0f ) );
	}	
	
	if( Tutorial.Timer == 950 )
	{
		MovePlayerPos( D3DXVECTOR2( 0.0f, -60.0f ) );
	}	

	VERTEX_2D* pVtx = NULL; // VRAM�̉��z�A�h���X���i�[
	
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferTutorial->Lock( 0,
								0,					// ���ׂĂ����b�N
								( void** )&pVtx,   // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
								0 );			
	
	// UV���W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( TUTORIAL_WINDOW_TEXTUER_U,									TUTORIAL_WINDOW_TEXTUER_V + Tutorial.Phase	       * TUTORIAL_WINDOW_TEXTURE_HEIGHT );	// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( TUTORIAL_WINDOW_TEXTUER_U + TUTORIAL_WINDOW_TEXTURE_WIDTH, TUTORIAL_WINDOW_TEXTUER_V + Tutorial.Phase		   * TUTORIAL_WINDOW_TEXTURE_HEIGHT );	// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( TUTORIAL_WINDOW_TEXTUER_U,									TUTORIAL_WINDOW_TEXTUER_V + ( Tutorial.Phase + 1 ) * TUTORIAL_WINDOW_TEXTURE_HEIGHT );	// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( TUTORIAL_WINDOW_TEXTUER_U + TUTORIAL_WINDOW_TEXTURE_WIDTH, TUTORIAL_WINDOW_TEXTUER_V + ( Tutorial.Phase + 1 ) * TUTORIAL_WINDOW_TEXTURE_HEIGHT );	// �E���̒��_

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferTutorial->Unlock();	
	
	// �^�C�}�[�̃C���N�������g
	Tutorial.Timer++;
}

/*------------------------------------------------------------------------------
  �@�`�揈��
------------------------------------------------------------------------------*/
void DrawTutorial( void )
{
	DrawBG();
	DrawMoveBG();
	DrawPlayer();	

	DrawCan();
	DrawJumper();
	DrawHurdle();
	DrawGoal();
	DrawItem();

	if( !Tutorial.Move )
	{
		DrawPolygon( g_pTextureTutorial, g_pVtxBufferTutorial, 1 );
	}

	DrawControl();
}

void LoadTutorial( void )
{
	// ���擾
	CAN* pCan = GetCan();
	HURDLE* pHurdle = GetHurdle();
	JUMPER* pJumper = GetJumper();
	GOAL* pGoal = GetGoal();
	ITEM* pItem = GetItem();

	// NULL�`�F�b�N
	if( pCan != NULL && pHurdle != NULL && pJumper != NULL && pGoal != NULL && pItem != NULL )
	{
		char aTemp[ 256 ] = {};
		sprintf( aTemp, "data\\tutorial.bin" );
		FILE* fp;
		fp = fopen( aTemp, "rb" );
		if( fp == NULL )
		{
			MessageBox( NULL, "�o�C�i���t�@�C���I�[�v�����s", "�G���[", MB_OK );
			return;
		}

		fread( pCan, sizeof( CAN ), CAN_MAX, fp );
		fread( pHurdle, sizeof( HURDLE ), HURDLE_MAX, fp );
		fread( pJumper, sizeof( JUMPER ), JUMPER_MAX, fp );
		fread( pGoal, sizeof( GOAL ), 1, fp );
		fread( pItem, sizeof( HURDLE ), HURDLE_MAX, fp );

		fclose( fp );
	}
}

// �Q�b�^�[
int GetTutorialTimer( void )
{
	return Tutorial.Timer;
}