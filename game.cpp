/*==============================================================================

    GAME���� [game.cpp]
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
#include <stdio.h>

/*------------------------------------------------------------------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
GAMEMODE g_GameMode = GAMEMODE_READY;
int      g_nStage   = 1;

/*------------------------------------------------------------------------------
  �@����������
------------------------------------------------------------------------------*/
void InitGame( void )
{
	g_nStage = 1;

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

	// BGM�̍Đ�
	/*if( FAILED( PlaySound( SOUND_LABEL_BGM000 ) ) )
	{
		MessageBox( 0, "�Đ��Ɏ��s���܂���", "�x��", MB_ICONWARNING );
	}*/

	// �X�e�[�W���̃��[�h
	LoadGame();
}

/*------------------------------------------------------------------------------
  �@�I������
------------------------------------------------------------------------------*/
void UninitGame( void )
{	
	g_nStage = 1;

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
void UpdateGame( void )
{
	// ���b�Z�[�W�̏��擾
	MESSAGE* pMessage = GetMessage();

	switch( pMessage->nType )
	{ 
	case MESSAGE_TYPE_STAGE:
	case MESSAGE_TYPE_READY:
	case MESSAGE_TYPE_GAMEOVER:
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
	case MESSAGE_TYPE_TIMEUP:
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
}

/*------------------------------------------------------------------------------
  �@�`�揈��
------------------------------------------------------------------------------*/
void DrawGame( void )
{
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
}

/*------------------------------------------------------------------------------
  �@���[�h����
------------------------------------------------------------------------------*/
void LoadGame( void )
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
		sprintf( aTemp, "data\\stage%d.bin", g_nStage );
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

/*------------------------------------------------------------------------------
  �@�Q�b�^�[
------------------------------------------------------------------------------*/
int GetStage( void )
{
	return g_nStage;
}

/*------------------------------------------------------------------------------
  �@�Z�b�^�[
------------------------------------------------------------------------------*/
void FowardStage( void )
{
	g_nStage++;
}