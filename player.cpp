/*==============================================================================

    �|���S������ [player.cpp]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/5/31
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  �@�w�b�_�t�@�C��
------------------------------------------------------------------------------*/
#include "main.h"
#include "game.h"
#include "movebg.h"
#include "input.h"
#include "polygon.h"
#include "player.h"
#include "can.h"
#include "hurdle.h"
#include "goal.h"
#include "message.h"
#include "jumper.h"
#include "item.h"
#include "particle.h"
#include "hiteffect.h"
#include "dasheffect.h"
#include "fade.h"
#include <math.h>

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define PLAYER_TEXTURENAME_RUN    "data/TEXTURE/osanpo_8cut.png"			 // �e�N�X�`���t�@�C�����i���s���j
#define PLAYER_TEXTURENAME_TUMBLE "data/TEXTURE/tumble_4cut.png"			 // �e�N�X�`���t�@�C�����i�]�|���j
#define PLAYER_TEXTURENAME_CRUSH  "data/TEXTURE/crush_2cut.png"				 // �e�N�X�`���t�@�C�����i�Ԃ��ꎞ�j
#define PLAYER_TEXTURENAME_WHEEZE  "data/TEXTURE/wheeze.png"				 // �e�N�X�`���t�@�C�����i���؂ꎞ�j

#define VALUE_GRAVITY			(0.6f)								     // �d��

#define PLAYER_POS_X			(200)							 		 // �\���ʒuX
#define PLAYER_POS_Y			(150)									 // �\���ʒuY
#define PLAYER_MOVE_SPEED		(50.0f)									 // �ړ����x
#define PLAYER_TEX_MAX			(4)										 // �e�N�X�`���摜��
#define PLAYER_RUN_TEX_SCROLL_SPEED (0.2f)								 // �e�N�X�`���X�N���[���̑��x
#define PLAYER_CRUSH_TEX_SCROLL_SPEED (0.06f)							 // �e�N�X�`���X�N���[���̑��x
#define PLAYER_WHEEZE_TEX_SCROLL_SPEED (0.08f)							 // �e�N�X�`���X�N���[���̑��x
#define PLAYER_RUN_TEX_WIDTH		(0.25f)								 // ���s���̃e�N�X�`����
#define PLAYER_RUN_TEX_HEIGHT		(0.5f)								 // ���s���̃e�N�X�`������
#define PLAYER_RUN_MAX_NUMTEX_X     (4)									 // ���s���̃e�N�X�`����ԍ��̍ő吔
#define PLAYER_RUN_MAX_NUMTEX_Y     (2)									 // ���s���̃e�N�X�`���s�ԍ��̍ő吔
#define PLAYER_CRUSH_NUMTEX     (2)										 // �e�N�X�`����ԍ��̍ő吔�i�Ԃ��ꎞ�j
#define PLAYER_WHEEZE_NUMTEX     (2)									 // �e�N�X�`����ԍ��̍ő吔�i���؂ꎞ�j
#define PLAYER_WHEEZE_TIME      (120)									 // ���؂�̕\������
#define PLAYER_JUMPER_VALUE		(24.0f)									 // �W�����v��g�p���̃W�����v��
#define PLAYER_CRUSH_TIME       (60)									 // �Ԃ��ꎞ��		
#define PLAYER_TUMBLE_TIME      (80)									 // �]�|����
#define PLAYER_DASH_TIME        (180)									 // �v���C���[�̃_�b�V������
#define PLAYER_NUM_SHADOW       (3)                                      // �_�b�V�����̎c���̐�
#define PLAYER_SHADOW_INTERVAL  (20.0f)									 // �_�b�V�����̎c���̊Ԋu

/*------------------------------------------------------------------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTexturePlayer[ PLAYER_TEX_MAX ] = {};   // �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPlayer				   = NULL; // ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

PLAYER Player; // �v���C���[�̍\����

// ��]�֌W
float g_fLengthPlayer;		// �|���S���̑Ίp���̒���
float g_fAnglePlayer;		// �|���S���̑Ίp���̊p�x

/*------------------------------------------------------------------------------
  �@����������
------------------------------------------------------------------------------*/
void InitPlayer( void )
{
	// --- �v���C���[�̏����� --- //////////////////////////////////////////////

	Player.fAxis	    = D3DXVECTOR2( 160.0f, 240.0f + PLAYER_HEIGHT * 0.5f );
	Player.fMove	    = D3DXVECTOR2( 0.0f, 0.0f );
	Player.fRotA        = 0.0f;
	Player.fPosTex.x    = 0.0f;
	Player.fPosTex.y    = 0.0f;
	Player.fLandingPosY = 0.0f;
	Player.nNumTexX	    = 0;
	Player.nNumTexY	    = 0;
	Player.nCount       = 0;
	Player.nDashCount   = 0;
	Player.State        = STATE_STOP;
	
	// �\�����W�̐ݒ�
	Player.fPos.x = Player.fAxis.x + cosf( Player.fRotA - D3DX_PI * 0.5f ) * PLAYER_HEIGHT * 0.5f;
	Player.fPos.y = Player.fAxis.y + sinf( Player.fRotA - D3DX_PI * 0.5f ) * PLAYER_HEIGHT * 0.5f;

	// �l���̍��W�̐ݒ�
	Player.fLeftUp.x    = Player.fPos.x + cos( Player.fRotC + D3DX_PI + g_fAnglePlayer ) * g_fLengthPlayer;		// ����̒��_X
	Player.fLeftUp.y    = Player.fPos.y + sin( Player.fRotC + D3DX_PI + g_fAnglePlayer ) * g_fLengthPlayer;		// ����̒��_Y		
	Player.fRightUp.x   = Player.fPos.x + cos( Player.fRotC			  - g_fAnglePlayer ) * g_fLengthPlayer;		// �E��̒��_X
	Player.fRightUp.y   = Player.fPos.y + sin( Player.fRotC			  - g_fAnglePlayer ) * g_fLengthPlayer;		// �E��̒��_Y
	Player.fLeftDown.x  = Player.fPos.x + cos( Player.fRotC + D3DX_PI - g_fAnglePlayer ) * g_fLengthPlayer;		// �����̒��_X
	Player.fLeftDown.y  = Player.fPos.y + sin( Player.fRotC + D3DX_PI - g_fAnglePlayer ) * g_fLengthPlayer;		// �����̒��_Y
	Player.fRightDown.x = Player.fPos.x + cos( Player.fRotC			  + g_fAnglePlayer ) * g_fLengthPlayer;		// �E���̒��_X
	Player.fRightDown.y = Player.fPos.y + sin( Player.fRotC			  + g_fAnglePlayer ) * g_fLengthPlayer;		// �E���̒��_Y

	// --- ��]���� ---	////////////////////////////////////////////////////////

	Player.fPos		= D3DXVECTOR2( PLAYER_POS_X, PLAYER_POS_Y );									// ���S���W�̐ݒ�
	Player.fRotC	= 0.0f;																			// �|���S���̉�]�ʂ̐ݒ�
	g_fLengthPlayer = sqrt( PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HEIGHT * PLAYER_HEIGHT ) / 2.0f;   // �Ίp���̒����̎Z�o
	g_fAnglePlayer  = atan2( PLAYER_HEIGHT, PLAYER_WIDTH );											// �Ίp���̊p�x�̎Z�o

	// --- ���_�̐ݒ� --- //////////////////////////////////////////////////////

	MakeVertexPolygon( &g_pVtxBufferPlayer, PLAYER_NUM_SHADOW + 1 );
	
	VERTEX_2D* pVtx = NULL; // VRAM�̉��z�A�h���X���i�[

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferPlayer->Lock( 0,
							  0,				// ���ׂĂ����b�N
							  ( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							 
							  0 );	
		
	// ���_���W�̐ݒ�
	pVtx[ 0 ].pos.x = Player.fLeftUp.x;		// ����̒��_X
	pVtx[ 0 ].pos.y = Player.fLeftUp.y;		// ����̒��_Y		
	pVtx[ 1 ].pos.x = Player.fRightUp.x;	// �E��̒��_X
	pVtx[ 1 ].pos.y = Player.fRightUp.y;	// �E��̒��_Y
	pVtx[ 2 ].pos.x = Player.fLeftDown.x;	// �����̒��_X
	pVtx[ 2 ].pos.y = Player.fLeftDown.y;	// �����̒��_Y
	pVtx[ 3 ].pos.x = Player.fRightDown.x;	// �E���̒��_X
	pVtx[ 3 ].pos.y = Player.fRightDown.y;	// �E���̒��_Y
	
	// UV���W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  Player.fPosTex.y );						// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, Player.fPosTex.y );						// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  Player.fPosTex.y + PLAYER_RUN_TEX_HEIGHT );	// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, Player.fPosTex.y + PLAYER_RUN_TEX_HEIGHT );	// �E���̒��_	

	pVtx += 4;
	
	pVtx[ 0 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  Player.fPosTex.y );						// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, Player.fPosTex.y );						// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  Player.fPosTex.y + PLAYER_RUN_TEX_HEIGHT );	// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, Player.fPosTex.y + PLAYER_RUN_TEX_HEIGHT );	// �E���̒��_	

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferPlayer->Unlock();	

	// --- �e�N�X�`���̓ǂݍ��� ////////////////////////////////////////////////

	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					  // �f�o�C�X�̃|�C���^��n��
											PLAYER_TEXTURENAME_RUN,		  // �e�N�X�`���̃t�@�C����
											&g_pTexturePlayer[ 0 ] ) ) )  // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );    // �G���[�`�F�b�N
	}

	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					  // �f�o�C�X�̃|�C���^��n��
											PLAYER_TEXTURENAME_TUMBLE,	  // �e�N�X�`���̃t�@�C����
											&g_pTexturePlayer[ 1 ] ) ) )  // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );    // �G���[�`�F�b�N
	}

	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					  // �f�o�C�X�̃|�C���^��n��
											PLAYER_TEXTURENAME_CRUSH,	  // �e�N�X�`���̃t�@�C����
											&g_pTexturePlayer[ 2 ] ) ) )  // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );    // �G���[�`�F�b�N
	}

	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					  // �f�o�C�X�̃|�C���^��n��
											PLAYER_TEXTURENAME_WHEEZE,	  // �e�N�X�`���̃t�@�C����
											&g_pTexturePlayer[ 3 ] ) ) )  // �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );    // �G���[�`�F�b�N
	}
}

/*------------------------------------------------------------------------------
  �@�I������
------------------------------------------------------------------------------*/	
void UninitPlayer( void )
{
	// �e�N�X�`���C���^�[�t�F�[�X�̊J��
	for( int nCnt = 0; nCnt < PLAYER_TEX_MAX; nCnt++ )
	{
		if( g_pTexturePlayer[ nCnt ] != NULL )
		{
			g_pTexturePlayer[ nCnt ]->Release();
			g_pTexturePlayer[ nCnt ] = NULL;
		}
	}

	// ���_�o�b�t�@�C���^�[�t�F�[�X�̊J��
	if( g_pVtxBufferPlayer != NULL )
	{
		g_pVtxBufferPlayer->Release();
		g_pVtxBufferPlayer = NULL;
	}
}

/*------------------------------------------------------------------------------
  �@�X�V����
------------------------------------------------------------------------------*/
void UpdatePlayer( void )
{	
	// �J�E���^�[�̏��� ////////////////////////////////////////////////////////

	// �J�E���^�[�̏�����
	static int nFrameCount = 0;	// �^�C�}�[

	// �Ԃ���A�]�|�A�W�����v���� ////////////////////////////////////////////

	// �ړ��w�i�̎擾
	MOVEBG* pMoveBg = NULL;
	pMoveBg			= GetMoveBG();

	// �Ԃ��ꏈ��
	if( Player.State == STATE_CRUSH )
	{
		pMoveBg->fTexScrollSpeed  *= 0.7f;					// �X�N���[��������
		Player.fRotA			   = D3DX_PI + D3DX_PI / 2; // �v���C���[��|��
		Player.fRotC			   = D3DX_PI + D3DX_PI / 2; 
		Player.nStunCount++;								// �X�^���J�E���^���C���N�������g

		// �X�^���J�E���^���Ԃ��ꎞ�Ԃ𒴂�����
		if( Player.nStunCount > PLAYER_CRUSH_TIME )
		{
			Player.State	  = STATE_STOP;		 // ��~��ԂɈڍs
			Player.fRotA	  = 0.0f;			 // �v���C���[���N����
			Player.fRotC	  = 0.0f;			 // �v���C���[���N����
			Player.nStunCount = 0;				 // �X�^���J�E���^�����Z�b�g
		}
	}
	
	// �]�|����
	if( Player.State == STATE_TUMBLE )
	{
		pMoveBg->fTexScrollSpeed *= 0.98f;					// �X�N���[��������
		Player.nStunCount++;								// �X�^���J�E���^���C���N�������g
		
		// �X�^���J�E���^���]�|���Ԃ̔����𒴂�����
		if( Player.nStunCount > PLAYER_TUMBLE_TIME * 0.5f && Player.fScrollTex >= 3.0f )
		{
			pMoveBg->fTexScrollSpeed  *= 0.7f;				// �X�N���[��������
			Player.fScrollTex		   = 3.0f;				// �A�j���[�V������������Ԃ̃J�b�g��
		}

		// �X�^���J�E���^���]�|���Ԃ𒴂�����
		if( Player.nStunCount > PLAYER_TUMBLE_TIME )
		{
			Player.State	  = STATE_STOP;  // ��~��ԂɈڍs
			Player.nStunCount = 0;			 // �X�^���J�E���^�����Z�b�g
		}
	}

	// �W�����v�̑؋󎞊Ԑ���
	if( Player.State == STATE_JUMP || Player.State == STATE_JUMPER )
	{
		Player.fMove.y += VALUE_GRAVITY;		 // �d�͉���

		// �v���C���[��Y���W�����n���W�ȉ��ɂȂ�����
		if( Player.fAxis.y >= Player.fLandingPosY )
		{
			Player.fAxis.y		= Player.fLandingPosY;  // �v���C���[��Y���W��␳
			Player.State	    = STATE_RUN;			// ���s��Ԃɖ߂�
			Player.fMove.y	    = 0.0f;					// Y�ړ��ʂ�0��
			Player.fLandingPosY = 0.0f;					// ���n���W��������
		}
	}
	
	// ���ڂ���p�[�e�B�N���̐����@////////////////////////////////////////////////
	
	if( Player.State == STATE_RUN && nFrameCount % 15 == 0 )
	{
		int temp = rand() % 21 - 10; // �T�C�Y�����p
		CreateParticle( D3DXVECTOR2( Player.fAxis.x + rand() % 11 - 5, Player.fAxis.y + rand() % 11 - 15 ), // ���W
						D3DXVECTOR2( PARTICLE_WIDTH + temp, PARTICLE_HEIGHT + temp ),					    // �T�C�Y
						PARTICLE_SPEED/* + rand() % 3 - 1*/,											    // ����
						D3DX_PI/*( 376 + rand() % 128 - 64 ) * 0.01f*/,									    // �p�x�i���W�A���j
						D3DXCOLOR( 1.0, 1.0, 1.0, 1.0f ),   // �F
						D3DXCOLOR( 0.0f, 0.0f, 0.0f, -(1.0f / PARTICLE_LIFE ) ),						    // �F�̕ω���
						PARTICLE_LIFE - rand() % 20 );													    // ����
	}

	// �ʂƂ̓����蔻�� ///////////////////////////////////////////////////////////

	// �ʂ̎擾
	CAN* pCan = NULL;
	pCan	  = GetCan();

	// �����蔻��
	for( int nCntCan = 0; nCntCan < CAN_MAX; nCntCan++ )
	{
		if( !pCan[ nCntCan ].bUse ) { continue; }

		if( ( Player.State == STATE_RUN || Player.State == STATE_STOP ) &&
			HitObject( Player.fLeftUp.x, Player.fLeftUp.y, PLAYER_WIDTH, PLAYER_HEIGHT, pCan[ nCntCan ].fLeftUp.x, pCan[ nCntCan ].fLeftUp.y, CAN_WIDTH, CAN_HEIGHT ) )
		{
			if( Player.bDash && pCan[ nCntCan ].bFly == false )
			{
				pCan[ nCntCan ].bFly    = true;
				pCan[ nCntCan ].fMove.x = 3.0f;
				pCan[ nCntCan ].fMove.y = - 8.0f;
			}
			else if( !Player.bDash ) 
			{
				Player.State = STATE_CRUSH;
				Player.fScrollTex = 0.0f;
			}
		}
	}

	// �n�[�h���Ƃ̓����蔻�� /////////////////////////////////////////////////////

	// �n�[�h���̎擾
	HURDLE* pHurdle = NULL;
	pHurdle	        = GetHurdle();

	// �����蔻��
	for( int nCntHurdle = 0; nCntHurdle < HURDLE_MAX; nCntHurdle++ )
	{
		if( !pHurdle[ nCntHurdle ].bUse ) { continue; }

		if( Player.State == STATE_RUN && !pHurdle[ nCntHurdle ].bFall &&
			HitObject( Player.fLeftUp.x, Player.fLeftUp.y, PLAYER_WIDTH, PLAYER_HEIGHT, pHurdle[ nCntHurdle ].fLeftUp.x, pHurdle[ nCntHurdle ].fLeftUp.y, HURDLE_WIDTH * 0.5f, HURDLE_HEIGHT ) )
		{
			Player.State   = STATE_TUMBLE;
			pHurdle[ nCntHurdle ].bFall = true;
		
			// �q�b�g�G�t�F�N�g�̍쐬
			for( int nCntHitEffect = 0; nCntHitEffect < HITEFFECT_MAX; nCntHitEffect++ )
			{
				if( nCntHitEffect % 6 == 0 )
				{
					CreateHitEffect( Player.fPos, D3DXVECTOR2( HITEFFECT_WIDTH * 3.0f, HITEFFECT_HEIGHT ), 0.0f, ( D3DX_PI * 2.0f / HITEFFECT_MAX ) * nCntHitEffect, D3DXCOLOR( 1.0f, 0.0f, 0.0f, 0.4f ), D3DXCOLOR( 0.0f, 0.0f, 0.0f, - ( 1.0f / HITEFFECT_LIFE * 1.0f) ), HITEFFECT_LIFE );
				}
				else
				{
					CreateHitEffect( Player.fPos, D3DXVECTOR2( HITEFFECT_WIDTH, HITEFFECT_HEIGHT ), 0.0f, ( D3DX_PI * 2.0f / HITEFFECT_MAX ) * nCntHitEffect, D3DXCOLOR( 1.0f, 0.0f, 0.0f, 0.4f ), D3DXCOLOR( 0.0f, 0.0f, 0.0f, - ( 1.0f / HITEFFECT_LIFE * 1.0f ) ), HITEFFECT_LIFE );
				}
			}
		}
	}
	
	// �W�����v��Ƃ̓����蔻�� ///////////////////////////////////////////////////
	
	// �W�����v��̎擾
	JUMPER* pJumper = NULL;
	pJumper	        = GetJumper();
	
	// �����蔻��	
	for( int nCntJumper = 0; nCntJumper < HURDLE_MAX; nCntJumper++ )
	{
		if( !pJumper[ nCntJumper ].bUse ) { continue; }

		if( Player.State == STATE_RUN && 
			HitObject( Player.fLeftUp.x, Player.fLeftUp.y, PLAYER_WIDTH, PLAYER_HEIGHT, pJumper[ nCntJumper ].fLeftUp.x + JUMPER_WIDTH * 0.5f, pJumper[ nCntJumper ].fLeftUp.y, JUMPER_WIDTH * 0.1f, JUMPER_HEIGHT ) )
		{
			Player.State		= STATE_JUMPER;				// �W�����v��Ԃɂ���
			Player.fMove.y	    = - PLAYER_JUMPER_VALUE;	// �c�ړ��ʂ�ݒ�
			Player.fLandingPosY = Player.fAxis.y;           // �W�����v���̍��W�𒅒n���W�ɂ���
		}
	}

	// �A�C�e���Ƃ̓����蔻�� ////////////////////////////////////////////////////

	// �A�C�e���̏��̎擾
	ITEM* pItem = GetItem();

	// �����蔻��	
	for( int nCntItem = 0; nCntItem < HURDLE_MAX; nCntItem++ )
	{
		if( !pItem[ nCntItem ].bUse ) { continue; }
	
		if( HitObject( Player.fLeftUp.x, Player.fLeftUp.y, PLAYER_WIDTH, PLAYER_HEIGHT, pItem[ nCntItem ].fLeftUp.x, pItem[ nCntItem ].fLeftUp.y, JUMPER_WIDTH, JUMPER_HEIGHT ) )
		{
			Player.bDash = true;    // �_�b�V����Ԃɂ���
		}
	}

	if( Player.bDash )
	{
		// �_�b�V���G�t�F�N�g�̔������̍쐬
		for( int nCntDashEffect = 0; nCntDashEffect < DASHEFFECT_EMITTER_MAX; nCntDashEffect++ )
		{
			CreateDashEffectEmitter( D3DXVECTOR2( Player.fPos.x - DASHEFFECT_WIDTH - rand() % 100 * 0.1f, Player.fPos.y + rand() % ( int )PLAYER_HEIGHT - PLAYER_HEIGHT * 0.5f ),
									 D3DXVECTOR2( DASHEFFECT_WIDTH, DASHEFFECT_HEIGHT ),
									 D3DXVECTOR2( DASHEFFECT_VELOCITY_X, 0.0f ),
									 DASHEFFECT_LIFE + rand() % 20 );
		}
	}	
	
	// �S�[���̏��� //////////////////////////////////////////////////////////////

	// �S�[���̏��̎擾
	GOAL* pGoal = GetGoal();
	
	// �����蔻��
	if( pGoal->bUse )
	{
		if( HitObject( Player.fLeftUp.x, Player.fLeftUp.y, PLAYER_WIDTH, PLAYER_HEIGHT, pGoal->fPos.x + GOAL_WIDTH * 0.5f, 0.0f, GOAL_WIDTH, SCREEN_HEIGHT ) && ( Player.State == STATE_RUN || Player.State == STATE_JUMPER ) )
		{
			CreateMessage( D3DXVECTOR2( 800.0f, 250.0f ), D3DXVECTOR2( MESSAGE_READY_WIDTH, 80.0f ), 2.0f, D3DX_PI, 0.0f, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0 ), D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0 ), MESSAGE_TYPE_GOAL );
			Player.State = STATE_GOAL;
			Player.bDash = false;
		}
	}

	// ���̃X�e�[�W�֐i��
	if( Player.State == STATE_WHEEZE )
	{
		Player.nCount++;

		if( Player.nCount > PLAYER_WHEEZE_TIME )
		{
			FowardStage();
			if( GetStage() <= STAGE_MAX )
			{
				InitGame();
			}
			else
			{
				SetFade( FADE_OUT, MODE_RESULT );
			}
		}
	}

	// �Q�[���I�[�o�[�̏��� ////////////////////////////////////////////////////////////
	if( Player.State == STATE_GAMEOVER )
	{
		//pMoveBg->fTexScrollSpeed = 0.0f;
	}

	// ����̏��� ////////////////////////////////////////////////////////////////
		
	// �W�����v
	if( GetKeyboardTrigger( DIK_X ) && ( Player.State == STATE_STOP || Player.State == STATE_RUN ) ) 
	{
		Player.fMove.y	    = - PLAYER_JUMP_VALUE;	// �c�ړ��ʂ�ݒ�
		Player.fLandingPosY = Player.fAxis.y;       // �W�����v���̍��W�𒅒n���W�ɂ���
		Player.State	    = STATE_JUMP;			// �v���C���[���W�����v��Ԃɂ���
	}
	
	// �����O�i�̏��� ////////////////////////////////////////////////////////////

	// �_�b�V����Ԃ̏ꍇ
	if( Player.bDash )
	{
		pMoveBg->fTexScrollSpeed += MOVEBG_TEXTURE_SCROLL_ACCELERATION_DASH;	
		Player.nDashCount++;
		if( Player.nDashCount > PLAYER_DASH_TIME )
		{
			Player.bDash = false;
			Player.nDashCount = 0;
			UnuseDashEffect();
		}
	}
	// �S�[����Ԃ̏ꍇ
	else if( Player.State == STATE_GOAL || Player.State == STATE_TIMEUP )
	{
		pMoveBg->fTexScrollSpeed -= MOVEBG_TEXTURE_SCROLL_ACCELERATION * 0.5f;
		
		if( pMoveBg->fTexScrollSpeed < 0.0f )
		{
			pMoveBg->fTexScrollSpeed = 0.0f;

			if( Player.State == STATE_GOAL )
			{
				Player.State = STATE_WHEEZE;
			}
			else 
			{
				Player.State = STATE_GAMEOVER;
			}
		}
	}
	// ��~�܂��͑��s��Ԃ̏ꍇ
	else
	{
		if( Player.State == STATE_STOP || Player.State == STATE_RUN )
		{
			pMoveBg->fTexScrollSpeed += MOVEBG_TEXTURE_SCROLL_ACCELERATION;
			Player.State = STATE_RUN;
		}
	}

	// �O�i���x����
	if( Player.bDash )
	{
		if( pMoveBg->fTexScrollSpeed > MOVEBG_TEXTURE_LIMIT_SCROOL_SPEED_DASH )
		{
			pMoveBg->fTexScrollSpeed = MOVEBG_TEXTURE_LIMIT_SCROOL_SPEED_DASH;
		}
	}
	else
	{
		if( pMoveBg->fTexScrollSpeed > MOVEBG_TEXTURE_LIMIT_SCROOL_SPEED )
		{
			pMoveBg->fTexScrollSpeed = MOVEBG_TEXTURE_LIMIT_SCROOL_SPEED;
		}
	}

	// ���ړ�
	if( GetKeyboardTrigger( DIK_S ) && ( Player.State == STATE_STOP || Player.State == STATE_RUN ) )
	{
		Player.fAxis.y += PLAYER_MOVE_SPEED;
		MoveDashEffectEmitter( PLAYER_MOVE_SPEED );
	}
	
	// ��ړ�
	if( GetKeyboardTrigger( DIK_W ) && ( Player.State == STATE_STOP || Player.State == STATE_RUN ) )
	{
		Player.fAxis.y -= PLAYER_MOVE_SPEED;
		MoveDashEffectEmitter( - PLAYER_MOVE_SPEED );
	} 

	// ��ړ�����
	if( ( Player.fPos.y + PLAYER_HEIGHT / 2 < LIMIT_TOP ) && Player.State != STATE_JUMP )
	{
		Player.fPos.y = LIMIT_TOP - PLAYER_HEIGHT / 2;
	}
	
	// ���ړ�����
	if( ( Player.fPos.y + PLAYER_HEIGHT / 2  > LIMIT_BOTTOM ) && Player.State != STATE_JUMP )
	{
		Player.fPos.y = LIMIT_BOTTOM - PLAYER_HEIGHT / 2;
	}

	// ���W�̍X�V ///////////////////////////////////////////////////////////////

	// ��]���̍X�V
	Player.fAxis.y += Player.fMove.y;
	
	// �\�����W�̍X�V
	Player.fPos.x = Player.fAxis.x + cosf( Player.fRotA - D3DX_PI * 0.5f ) * PLAYER_HEIGHT * 0.5f;
	Player.fPos.y = Player.fAxis.y + sinf( Player.fRotA - D3DX_PI * 0.5f ) * PLAYER_HEIGHT * 0.5f;
	
	// �l���̍��W�̍X�V
	Player.fLeftUp.x    = Player.fPos.x + cos( Player.fRotC + D3DX_PI + g_fAnglePlayer ) * g_fLengthPlayer;		// ����̒��_X
	Player.fLeftUp.y    = Player.fPos.y + sin( Player.fRotC + D3DX_PI + g_fAnglePlayer ) * g_fLengthPlayer;		// ����̒��_Y		
	Player.fRightUp.x   = Player.fPos.x + cos( Player.fRotC			  - g_fAnglePlayer ) * g_fLengthPlayer;		// �E��̒��_X
	Player.fRightUp.y   = Player.fPos.y + sin( Player.fRotC			  - g_fAnglePlayer ) * g_fLengthPlayer;		// �E��̒��_Y
	Player.fLeftDown.x  = Player.fPos.x + cos( Player.fRotC + D3DX_PI - g_fAnglePlayer ) * g_fLengthPlayer;		// �����̒��_X
	Player.fLeftDown.y  = Player.fPos.y + sin( Player.fRotC + D3DX_PI - g_fAnglePlayer ) * g_fLengthPlayer;		// �����̒��_Y
	Player.fRightDown.x = Player.fPos.x + cos( Player.fRotC			  + g_fAnglePlayer ) * g_fLengthPlayer;		// �E���̒��_X
	Player.fRightDown.y = Player.fPos.y + sin( Player.fRotC			  + g_fAnglePlayer ) * g_fLengthPlayer;		// �E���̒��_Y
	
	// �e�N�X�`���̍X�V //////////////////////////////////////////////////////////

	switch( Player.State )
	{
	case STATE_RUN:
	case STATE_TUMBLE:
	case STATE_GOAL:
	case STATE_TIMEUP:
		// �e�N�X�`���ԍ��̍X�V
		Player.nNumTexX = ( int )Player.fScrollTex % PLAYER_RUN_MAX_NUMTEX_X;
		Player.nNumTexY = ( int )Player.fScrollTex / PLAYER_RUN_MAX_NUMTEX_X % PLAYER_RUN_MAX_NUMTEX_Y;

		// �e�N�X�`�����W�̍X�V
		Player.fPosTex.x = Player.nNumTexX * ( 1.0f / PLAYER_RUN_MAX_NUMTEX_X );
		Player.fPosTex.y = Player.nNumTexY * ( 1.0f / PLAYER_RUN_MAX_NUMTEX_Y );
		
		// �e�N�X�`���X�N���[���ʒu�̍X�V
		Player.fScrollTex += PLAYER_RUN_TEX_SCROLL_SPEED;
		break;
	case STATE_CRUSH:		
		// �e�N�X�`�����W�̍X�V
		if( Player.fScrollTex < 1.0f )
		{
			Player.fPosTex.x = 0.0f;
		}
		else
		{
			Player.fPosTex.x = 0.5f;
		}

		// �e�N�X�`���X�N���[���ʒu�̍X�V
		Player.fScrollTex += PLAYER_CRUSH_TEX_SCROLL_SPEED;
		break;
	case STATE_WHEEZE:
		// �e�N�X�`���ԍ��̍X�V
		Player.nNumTexX = ( int )Player.fScrollTex % PLAYER_WHEEZE_NUMTEX;

		// �e�N�X�`�����W�̍X�V
		Player.fPosTex.x = Player.nNumTexX * ( 1.0f / PLAYER_WHEEZE_NUMTEX );
		
		// �e�N�X�`���X�N���[���ʒu�̍X�V
		Player.fScrollTex += PLAYER_WHEEZE_TEX_SCROLL_SPEED;
		break;
	case STATE_GAMEOVER:
		// �e�N�X�`�����W�̐ݒ�
		Player.fPosTex.x = 0.5f;
		break;
	case STATE_STOP:
		break;
	}	

	// ���_�o�b�t�@�̍X�V ////////////////////////////////////////////////////////

	VERTEX_2D* pVtx = NULL; // VRAM�̉��z�A�h���X���i�[

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferPlayer->Lock( 0,
							  0,				// ���ׂĂ����b�N
							  ( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							 
							  0 );	

	// ��]�A���_�o�b�t�@�ݒ�
	RotationPlayer( pVtx );
	
	// UV���W�̐ݒ�
	if( Player.State == STATE_TUMBLE || Player.State == STATE_JUMPER )
	{
		pVtx[ 0 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  0.0f );						// ����̒��_
		pVtx[ 1 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, 0.0f );						// �E��̒��_
		pVtx[ 2 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  1.0f );	// �����̒��_	
		pVtx[ 3 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, 1.0f );	// �E���̒��_

		pVtx += 4;

		pVtx[ 0 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  0.0f );						// ����̒��_
		pVtx[ 1 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, 0.0f );						// �E��̒��_
		pVtx[ 2 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  1.0f );	// �����̒��_	
		pVtx[ 3 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, 1.0f );	// �E���̒��_
	}
	else if( Player.State == STATE_CRUSH || Player.State == STATE_WHEEZE || Player.State == STATE_GAMEOVER )
	{
		pVtx[ 0 ].tex = D3DXVECTOR2( Player.fPosTex.x,					  0.0f );						// ����̒��_
		pVtx[ 1 ].tex = D3DXVECTOR2( Player.fPosTex.x + 0.5f			, 0.0f );						// �E��̒��_
		pVtx[ 2 ].tex = D3DXVECTOR2( Player.fPosTex.x,					  1.0f );	// �����̒��_	
		pVtx[ 3 ].tex = D3DXVECTOR2( Player.fPosTex.x + 0.5f			, 1.0f );	// �E���̒��_
	}
	else
	{
		if( Player.bDash )
		{
			for( int nCnt = 0; nCnt < PLAYER_NUM_SHADOW + 1; nCnt++ )
			{
				if( nCnt > 0 )
				{
					// ���_�J���[�̐ݒ�i 0 �` 255�̐����l )
					pVtx[ 0 ].color = D3DCOLOR_RGBA( 255, 255, 255, 64 );	// ����̒��_
					pVtx[ 1 ].color = D3DCOLOR_RGBA( 255, 255, 255, 64 );	// �E��̒��_
					pVtx[ 2 ].color = D3DCOLOR_RGBA( 255, 255, 255, 64 );	// �����̒��_
					pVtx[ 3 ].color = D3DCOLOR_RGBA( 255, 255, 255, 64 );	// �E���̒��_
				}

				pVtx[ 0 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  Player.fPosTex.y );						// ����̒��_
				pVtx[ 1 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, Player.fPosTex.y );						// �E��̒��_
				pVtx[ 2 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  Player.fPosTex.y + PLAYER_RUN_TEX_HEIGHT );	// �����̒��_	
				pVtx[ 3 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, Player.fPosTex.y + PLAYER_RUN_TEX_HEIGHT );	// �E���̒��_
																																														
				pVtx += 4;
			}
		}
		else
		{
			pVtx[ 0 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  Player.fPosTex.y );						// ����̒��_
			pVtx[ 1 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, Player.fPosTex.y );						// �E��̒��_
			pVtx[ 2 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  Player.fPosTex.y + PLAYER_RUN_TEX_HEIGHT );	// �����̒��_	
			pVtx[ 3 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, Player.fPosTex.y + PLAYER_RUN_TEX_HEIGHT );	// �E���̒��_
		}
	}																		

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferPlayer->Unlock();	

	// �J�E���^�[�̏��� ////////////////////////////////////////////////////////

	// �J�E���^�[�̃C���N�������g
	nFrameCount++;
}

/*------------------------------------------------------------------------------
  �@�`�揈��
------------------------------------------------------------------------------*/
void DrawPlayer( void )
{
	// �v���C���[�̕`��
	switch( Player.State )
	{
	case STATE_STOP:
	case STATE_RUN:
	case STATE_JUMP:
	case STATE_GOAL:
	case STATE_TIMEUP:
		DrawPolygon( g_pTexturePlayer[ 0 ], g_pVtxBufferPlayer, PLAYER_NUM_SHADOW + 1 );
		break;
	case STATE_TUMBLE:
	case STATE_JUMPER:
		DrawPolygon( g_pTexturePlayer[ 1 ], g_pVtxBufferPlayer, 1 );
		break;
	case STATE_CRUSH:
		DrawPolygon( g_pTexturePlayer[ 2 ], g_pVtxBufferPlayer, 1 );
		break;
	case STATE_WHEEZE:
	case STATE_GAMEOVER:
		DrawPolygon( g_pTexturePlayer[ 3 ], g_pVtxBufferPlayer, 1 );
		break;
	}
}

/*------------------------------------------------------------------------------
  �@�ʂ̉�]�A���_�o�b�t�@�ݒ�
------------------------------------------------------------------------------*/
void RotationPlayer( VERTEX_2D* pVtx )
{
	if( Player.bDash )
	{
		for( int nCnt = 0; nCnt < PLAYER_NUM_SHADOW + 1; nCnt++ )
		{
			// ���_���W�̐ݒ�
			pVtx[ 0 ].pos.x = Player.fLeftUp.x - PLAYER_SHADOW_INTERVAL * nCnt;		// ����̒��_X
			pVtx[ 0 ].pos.y = Player.fLeftUp.y;										// ����̒��_Y		
			pVtx[ 1 ].pos.x = Player.fRightUp.x - PLAYER_SHADOW_INTERVAL * nCnt;	// �E��̒��_X
			pVtx[ 1 ].pos.y = Player.fRightUp.y;									// �E��̒��_Y
			pVtx[ 2 ].pos.x = Player.fLeftDown.x - PLAYER_SHADOW_INTERVAL * nCnt;	// �����̒��_X
			pVtx[ 2 ].pos.y = Player.fLeftDown.y;									// �����̒��_Y
			pVtx[ 3 ].pos.x = Player.fRightDown.x - PLAYER_SHADOW_INTERVAL * nCnt;	// �E���̒��_X
			pVtx[ 3 ].pos.y = Player.fRightDown.y;									// �E���̒��_Y

			pVtx += 4;
		}
	}
	else
	{
		// ���_���W�̐ݒ�
		pVtx[ 0 ].pos.x = Player.fLeftUp.x;		// ����̒��_X
		pVtx[ 0 ].pos.y = Player.fLeftUp.y;						// ����̒��_Y		
		pVtx[ 1 ].pos.x = Player.fRightUp.x;		// �E��̒��_X
		pVtx[ 1 ].pos.y = Player.fRightUp.y;					// �E��̒��_Y
		pVtx[ 2 ].pos.x = Player.fLeftDown.x;	// �����̒��_X
		pVtx[ 2 ].pos.y = Player.fLeftDown.y;					// �����̒��_Y
		pVtx[ 3 ].pos.x = Player.fRightDown.x;	// �E���̒��_X
		pVtx[ 3 ].pos.y = Player.fRightDown.y;					// �E���̒��_Y
	}
}

/*------------------------------------------------------------------------------
  �@�I�u�W�F�N�g�Ƃ̓����蔻��
------------------------------------------------------------------------------*/
bool HitObject( float pos1x, float pos1y, float width1, float height1, float pos2x, float pos2y, float width2, float height2 )
{
	bool bHit = false;
	if( pos1x + width1 > pos2x && pos1x < pos2x + width2 )
	{
		if( pos1y + height1 > pos2y && pos1y + height1 < pos2y + height2 )
		{
			bHit = true;
		}
	}

	return bHit;
}

/*------------------------------------------------------------------------------
  �@�Q�b�^�[
------------------------------------------------------------------------------*/
PLAYER* GetPlayer( void )
{
	return &Player;
}

/*------------------------------------------------------------------------------
  �@�Z�b�^�[
------------------------------------------------------------------------------*/
void SetPlayerState( STATE state )
{
	Player.State = state;
}

void MovePlayerPos( D3DXVECTOR2 pos )
{
	Player.fAxis += pos;
}
