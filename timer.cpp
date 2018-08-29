/*==============================================================================

    Timer���� [Timer.cpp]
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
#include "polygon.h"
#include "timer.h"

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define TIMER_TEXTURENAME "data/TEXTURE/number.png" // �e�N�X�`���t�@�C����
#define TIMER_POS_X ( 300.0f )							// Timer�̕\���ʒuX
#define TIMER_POS_Y ( 0.0f )							// Timer�̕\���ʒuY
#define TIMER_WIDTH ( 50 )							// Timer�̕�
#define TIMER_HEIGHT ( 75 )							// Timer�̍���
#define TIMER_INTERVAL ( 10 )						// Timer�̊Ԋu
#define TIMER_MAX_DIGIT ( 4 )						// Timer�̍ő包��
#define TIMER_TEXTURE_WIDTH ( 0.1f )				// Timer�̃e�N�X�`����
#define TIMER_TEXTURE_HEIGHT ( 1.0f )				// Timer�̃e�N�X�`������
#define TIMER_STAGE1 (36000)                         // Stage1�̃N���A�^�C��
#define TIMER_STAGE2 (3600)                         // Stage2�̃N���A�^�C��
#define TIMER_STAGE3 (3600)                         // Stage3�̃N���A�^�C��

/*------------------------------------------------------------------------------
  �@�\���̂̐錾
------------------------------------------------------------------------------*/
// Timer�̍\����
typedef struct
{
	float fTimer;		// �^�C�}�[�i�[
}TIMER;

/*------------------------------------------------------------------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureTimer = NULL;			// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTimer = NULL;  // ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

TIMER Timer;											// Timer�̍\����

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
HRESULT MakeVertexTimer( LPDIRECT3DDEVICE9 pDevice );

/*------------------------------------------------------------------------------
  �@����������
------------------------------------------------------------------------------*/
void InitTimer( void )
{
	// Timer�̏�����
	switch( GetStage() )
	{
	case 1:
		Timer.fTimer = TIMER_STAGE1;
		break;
	case 2:
		Timer.fTimer = TIMER_STAGE2;
		break;
	case 3:
		Timer.fTimer = TIMER_STAGE3;
		break;
	}

	// ���_�̐ݒ�
	MakeVertexPolygon( &g_pVtxBufferTimer, TIMER_MAX_DIGIT );
	
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					// �f�o�C�X�̃|�C���^��n��
											TIMER_TEXTURENAME,			// �e�N�X�`���̃t�@�C����
											&g_pTextureTimer ) ) )		// �e�N�X�`���C���^�[�t�F�[�X�̊i�[
	{
		MessageBox( NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK );  // �G���[�`�F�b�N
	}
}

/*------------------------------------------------------------------------------
  �@�I������
------------------------------------------------------------------------------*/
void UninitTimer( void )
{
	UninitPolygon( &g_pTextureTimer, &g_pVtxBufferTimer );
}

/*------------------------------------------------------------------------------
  �@�X�V����
------------------------------------------------------------------------------*/
void UpdateTimer( void )
{
	int value = ( int )Timer.fTimer; // ����p�^�C�}�[�i�[
	int number;				  // �^�C�}�[�̐���

	VERTEX_2D* pVtx = NULL;   // VRAM�̉��z�A�h���X���i�[
	
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferTimer->Lock( 0,
							 0,				   // ���ׂĂ����b�N
							 ( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
							 0 );			

	for( int nCnt = 0; nCnt < TIMER_MAX_DIGIT; nCnt++ )
	{
		// �^�C�}�[����
		number = value % 10;			// �ŉ��ʌ��̎��o��
		value /= 10;					// �ŉ��ʌ��̍X�V
			
		// �^�C�}�[�����̃Z�b�g
		SetTimerNumber( pVtx, number, nCnt );
			
		pVtx += 4;
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBufferTimer->Unlock();
	
	// �^�C�}�[�̃C���N�������g
	Timer.fTimer++;
}

/*------------------------------------------------------------------------------
  �@�`�揈��
------------------------------------------------------------------------------*/
void DrawTimer( void )
{
	DrawPolygon( g_pTextureTimer, g_pVtxBufferTimer, TIMER_MAX_DIGIT );
}

/*------------------------------------------------------------------------------
  �@�^�C�}�[�����̐ݒ菈��
------------------------------------------------------------------------------*/
void SetTimerNumber( VERTEX_2D* pVtx, int number, int index )
{
	// ���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( TIMER_POS_X - ( TIMER_WIDTH + TIMER_INTERVAL ) * index,			   TIMER_POS_Y,				   0.0f );	// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( TIMER_POS_X - ( TIMER_WIDTH + TIMER_INTERVAL ) * index + TIMER_WIDTH, TIMER_POS_Y,				   0.0f );	// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( TIMER_POS_X - ( TIMER_WIDTH + TIMER_INTERVAL ) * index,			   TIMER_POS_Y + TIMER_HEIGHT, 0.0f );	// �����̒��_	
	pVtx[ 3 ].pos = D3DXVECTOR3( TIMER_POS_X - ( TIMER_WIDTH + TIMER_INTERVAL ) * index + TIMER_WIDTH, TIMER_POS_Y + TIMER_HEIGHT, 0.0f );	// �E���̒��_

	// UV���W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( number * TIMER_TEXTURE_WIDTH,						 0.0f );					// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( number * TIMER_TEXTURE_WIDTH + TIMER_TEXTURE_WIDTH, 0.0f );					// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( number * TIMER_TEXTURE_WIDTH,						 TIMER_TEXTURE_HEIGHT );	// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( number * TIMER_TEXTURE_WIDTH + TIMER_TEXTURE_WIDTH, TIMER_TEXTURE_HEIGHT );	// �E���̒��_
}

/*------------------------------------------------------------------------------
  �@�^�C�}�[�̃Z�b�^�[
------------------------------------------------------------------------------*/
void SetTimer( float fTimer )
{
	Timer.fTimer = fTimer;
}

/*------------------------------------------------------------------------------
  �@�^�C�}�[�̃Q�b�^�[
------------------------------------------------------------------------------*/
int GetTimer( void )
{
	return Timer.fTimer;
}