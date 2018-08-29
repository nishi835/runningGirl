/*==============================================================================

    ���b�Z�[�W�̏��� [message.cpp]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/5/31
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  �@�w�b�_�t�@�C��
------------------------------------------------------------------------------*/
#include "main.h"
#include "message.h"
#include "polygon.h"
#include "player.h"
#include "input.h"
#include "game.h"
#include "fade.h"

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define MESSAGE_TEXTURENAME_STAGE	 "data/TEXTURE/stage.png"	// �e�N�X�`����(.png �� / .jpg / .tga �� / .bmp / .dds ��) ���͔��������g�p��
#define MESSAGE_TEXTURENAME_READY	 "data/TEXTURE/ready.png"	// �e�N�X�`����(.png �� / .jpg / .tga �� / .bmp / .dds ��) ���͔��������g�p��
#define MESSAGE_TEXTURENAME_GO       "data/TEXTURE/go.png"	    // �e�N�X�`����
#define MESSAGE_TEXTURENAME_GOAL     "data/TEXTURE/goal.png"	    // �e�N�X�`����
#define MESSAGE_TEXTURENAME_GAMEOVER "data/TEXTURE/gameover.png"
#define MESSAGE_TEXTURENAME_TIMEUP   "data/TEXTURE/timeup.png"
#define MESSAGE_MAX					(1)					        // ���b�Z�[�W�̍ő吔
#define MESSAGE_TEX_MAX				(6)					        // �e�N�X�`���̍ő吔

/*------------------------------------------------------------------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTextureMessage[ MESSAGE_TEX_MAX ] = {};		// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^�ϐ�
LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferMessage					 = NULL;	// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

MESSAGE					Message[ MESSAGE_MAX ];		// ���b�Z�[�W�̍\����
int						g_nCntMessagePolygon = 0;	// ���b�Z�[�W�̃|���S�����i�`�悷�郁�b�Z�[�W�̐��~2�j
int						g_nFrameCount = 0;			// �^�C�}�[

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
HRESULT MakeVertexMessage( LPDIRECT3DDEVICE9 pDevice );

/*------------------------------------------------------------------------------
  �@���b�Z�[�W�̏�����
------------------------------------------------------------------------------*/
void InitMessage( void )
{
	// �J�E���^�[�̏�����
	g_nFrameCount = 0;

	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �����l�̐ݒ�
	for( int nCntMessage = 0; nCntMessage < MESSAGE_MAX; nCntMessage++ )
	{
		Message[ nCntMessage ].bUse			= false;
		Message[ nCntMessage ].fPos.x		= 0.0f;
		Message[ nCntMessage ].fPos.y		= 0.0f;
		Message[ nCntMessage ].fSize.x		= MESSAGE_WIDTH;
		Message[ nCntMessage ].fSize.y		= MESSAGE_HEIGHT;
		Message[ nCntMessage ].fRot			= 0.0f;
		Message[ nCntMessage ].fLength		= 0.0f;
		Message[ nCntMessage ].fAngle		= 0.0f;
		Message[ nCntMessage ].fScale		= 1.0f;
		Message[ nCntMessage ].fSpeed		= 0.0f;
		Message[ nCntMessage ].fVelocity.x	= 0.0f;
		Message[ nCntMessage ].fVelocity.y	= 0.0f;
		Message[ nCntMessage ].nCounter		= 0;
		Message[ nCntMessage ].nLife		= 0;
		Message[ nCntMessage ].nType		= MESSAGE_TYPE_STAGE;
	}

	// ���_�̍쐬
	MakeVertexPolygon( &g_pVtxBufferMessage, MESSAGE_MAX );

	// �e�N�X�`���̓ǂݍ���
	if( FAILED( D3DXCreateTextureFromFile( pDevice, MESSAGE_TEXTURENAME_STAGE, &g_pTextureMessage[ 0 ] ) ) )
	{
		MessageBox( NULL, "Message�̃e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B", "�G���[���b�Z�[�W", MB_OK | MB_ICONEXCLAMATION );
	}

	if( FAILED( D3DXCreateTextureFromFile( pDevice, MESSAGE_TEXTURENAME_READY, &g_pTextureMessage[ 1 ] ) ) )
	{
		MessageBox( NULL, "Message�̃e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B", "�G���[���b�Z�[�W", MB_OK | MB_ICONEXCLAMATION );
	}

	if( FAILED( D3DXCreateTextureFromFile( pDevice, MESSAGE_TEXTURENAME_GO, &g_pTextureMessage[ 2 ] ) ) )
	{
		MessageBox( NULL, "Message�̃e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B", "�G���[���b�Z�[�W", MB_OK | MB_ICONEXCLAMATION );
	}

	if( FAILED( D3DXCreateTextureFromFile( pDevice, MESSAGE_TEXTURENAME_GOAL, &g_pTextureMessage[ 3 ] ) ) )
	{
		MessageBox( NULL, "Message�̃e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B", "�G���[���b�Z�[�W", MB_OK | MB_ICONEXCLAMATION );
	}
	
	if( FAILED( D3DXCreateTextureFromFile( pDevice, MESSAGE_TEXTURENAME_TIMEUP, &g_pTextureMessage[ 4 ] ) ) )
	{
		MessageBox( NULL, "Message�̃e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B", "�G���[���b�Z�[�W", MB_OK | MB_ICONEXCLAMATION );
	}

	if( FAILED( D3DXCreateTextureFromFile( pDevice, MESSAGE_TEXTURENAME_GAMEOVER, &g_pTextureMessage[ 5 ] ) ) )
	{
		MessageBox( NULL, "Message�̃e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B", "�G���[���b�Z�[�W", MB_OK | MB_ICONEXCLAMATION );
	}
}

/*------------------------------------------------------------------------------
  �@���b�Z�[�W�̏I������
------------------------------------------------------------------------------*/
void UninitMessage( void )
{
	// �e�N�X�`���C���^�[�t�F�[�X�̏I������
	for( int nCnt = 0; nCnt < MESSAGE_TEX_MAX; nCnt++ )
	{
		if( g_pTextureMessage[ nCnt ] != NULL )
		{
			g_pTextureMessage[ nCnt ]->Release();
			g_pTextureMessage[ nCnt ] = NULL;
		}
	}

	// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^�̏I������
	if( g_pVtxBufferMessage )
	{
		g_pVtxBufferMessage->Release();
		g_pVtxBufferMessage = NULL;
	}
}

/*------------------------------------------------------------------------------
  �@���b�Z�[�W�̍X�V
------------------------------------------------------------------------------*/
void UpdateMessage( void )
{
	// �X�e�[�W���b�Z�[�W
	if( g_nFrameCount == 0 )
	{
		CreateMessage( D3DXVECTOR2( 800.0f, 250.0f ), D3DXVECTOR2( MESSAGE_READY_WIDTH, 80.0f ), 2.0f, D3DX_PI, 0.0f, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0 ), D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0 ), MESSAGE_TYPE_STAGE );
	}

	// ���W�X�V
	for( int nCntMessage = 0; nCntMessage < MESSAGE_MAX; nCntMessage++ )
	{
		// ���g�p���Ȃ�ȉ��̃v���O�������΂�
		if( !Message[ nCntMessage ].bUse ) { continue; }

		if( Message[ 0 ].nType == MESSAGE_TYPE_TIMEUP || Message[ 0 ].nType == MESSAGE_TYPE_GAMEOVER ) { continue; }

		if( Message[ nCntMessage ].fPos.x > SCREEN_WIDTH * 0.5f )
		{
			if( GetKeyboardTrigger( DIK_SPACE ) )
			{
				Message[ nCntMessage ].fPos.x = SCREEN_WIDTH * 0.5f;
			}

			Message[ nCntMessage ].fVelocity.x += 1.0f * cosf( Message[ nCntMessage ].fDirect );
			Message[ nCntMessage ].fPos.x += Message[ nCntMessage ].fVelocity.x;
			Message[ nCntMessage ].fPos.y += Message[ nCntMessage ].fVelocity.y;
		}
		else
		{						
			if( GetKeyboardTrigger( DIK_SPACE ) && Message[ nCntMessage ].nCounter && Message[ nCntMessage ].nType != MESSAGE_TYPE_GO )
			{
				Message[ nCntMessage ].nType++;
				Message[ nCntMessage ].nCounter = 0;
			}

			Message[ nCntMessage ].nCounter++;

			// ��ʒ����Ń��b�Z�[�W���~�߂�
			if( Message[ nCntMessage ].nCounter < 60 || Message[ nCntMessage ].nType == MESSAGE_TYPE_GOAL )
			{
				Message[ nCntMessage ].fVelocity.x = 0;
			}
			else
			{
				// ���b�Z�[�W���΂�
				if( GetKeyboardTrigger( DIK_SPACE ) && Message[ nCntMessage ].nType != MESSAGE_TYPE_GO )
				{
					Message[ nCntMessage ].fPos.x = SCREEN_WIDTH * 0.5;
					Message[ nCntMessage ].nType++;
					Message[ nCntMessage ].nCounter = 0;
				}
				// ���b�Z�[�W���X�N���[��������
				else
				{
					Message[ nCntMessage ].fVelocity.x += 1.0f * cosf( Message[ nCntMessage ].fDirect );
					Message[ nCntMessage ].fPos.x += Message[ nCntMessage ].fVelocity.x;
					Message[ nCntMessage ].fPos.y += Message[ nCntMessage ].fVelocity.y;
				}
			}
		}

		Message[ nCntMessage ].fColor += Message[ nCntMessage ].fColorValue;

		// ��ʊO(���E)�ɏo������
		if( Message[ nCntMessage ].fPos.x + MESSAGE_READY_WIDTH < 0.0f )
		{
			Message[ nCntMessage ].fPos.x = SCREEN_WIDTH;
			Message[ nCntMessage ].fVelocity.x = 0.0f;
			Message[ nCntMessage ].nCounter = 0;	
			
			if( Message[ nCntMessage ].nType == MESSAGE_TYPE_STAGE )
			{
				Message[ nCntMessage ].nType = MESSAGE_TYPE_READY;
			}
			else if( Message[ nCntMessage ].nType == MESSAGE_TYPE_READY )
			{
				Message[ nCntMessage ].nType = MESSAGE_TYPE_GO;
			}
			else if( Message[ nCntMessage ].nType == MESSAGE_TYPE_GO )
			{
				Message[ nCntMessage ].bUse = false;
			}
		}
	}

	// ���Ԑ؂ꃁ�b�Z�[�W
	if( Message[ 0 ].nType == MESSAGE_TYPE_TIMEUP )
	{
		if( Message[ 0 ].nCounter == 0 )
		{
			Message[ 0 ].bUse = true;
			Message[ 0 ].fPos.x = SCREEN_WIDTH * 0.5f;
			Message[ 0 ].fPos.y = SCREEN_HEIGHT * 0.5f;
		}

		Message[ 0 ].nCounter++;

		if( Message[ 0 ].nCounter > 180 )
		{
			Message[ 0 ].bUse = false;
			Message[ 0 ].nCounter = 0;
			Message[ 0 ].nType = MESSAGE_TYPE_GAMEOVER;
		}
	}
	
	// �Q�[���I�[�o�[���b�Z�[�W
	if( Message[ 0 ].nType == MESSAGE_TYPE_GAMEOVER )
	{
		if( Message[ 0 ].nCounter == 0 )
		{
			Message[ 0 ].bUse = true;
			Message[ 0 ].fPos.x = SCREEN_WIDTH * 0.5f;
			Message[ 0 ].fPos.y = 0.0f;
			Message[ 0 ].fVelocity.y = 3.0f;
		}

		Message[ 0 ].nCounter++;
		Message[ 0 ].fVelocity.y += 1.0f;
		Message[ 0 ].fPos.y += Message[ 0 ].fVelocity.y;

		if( Message[ 0 ].fPos.y >= SCREEN_HEIGHT * 0.5f )
		{
			Message[ 0 ].fPos.y = SCREEN_HEIGHT * 0.5f;
			Message[ 0 ].fVelocity.y *= - 0.8f;
		}

		if( Message[ 0 ].nCounter > 360 )
		{
			Message[ 0 ].bUse = false;
			SetFade( FADE_OUT, MODE_RESULT );
		}
	}
	
	// ���_�o�b�t�@�i�[�ϐ�
	VERTEX_2D* pVtx = NULL;

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferMessage->Lock( 0, 0, ( void** )&pVtx, 0 );
	
	// �X�V
	for( int nCntMessage = 0; nCntMessage < MESSAGE_MAX; nCntMessage++ )
	{
		if( !Message[ nCntMessage ].bUse ) { continue; }
				
		// �l���̍��W�̍X�V
		Message[ nCntMessage ].fLeftUp.x    = Message[ nCntMessage ].fPos.x + cos( Message[ nCntMessage ].fRot + D3DX_PI + Message[ nCntMessage ].fAngle ) * Message[ nCntMessage ].fLength;		// ����̒��_X
		Message[ nCntMessage ].fLeftUp.y    = Message[ nCntMessage ].fPos.y + sin( Message[ nCntMessage ].fRot + D3DX_PI + Message[ nCntMessage ].fAngle ) * Message[ nCntMessage ].fLength;		// ����̒��_Y		
		Message[ nCntMessage ].fRightUp.x   = Message[ nCntMessage ].fPos.x + cos( Message[ nCntMessage ].fRot		     - Message[ nCntMessage ].fAngle ) * Message[ nCntMessage ].fLength;		// �E��̒��_X
		Message[ nCntMessage ].fRightUp.y   = Message[ nCntMessage ].fPos.y + sin( Message[ nCntMessage ].fRot		     - Message[ nCntMessage ].fAngle ) * Message[ nCntMessage ].fLength;		// �E��̒��_Y
		Message[ nCntMessage ].fLeftDown.x  = Message[ nCntMessage ].fPos.x + cos( Message[ nCntMessage ].fRot + D3DX_PI - Message[ nCntMessage ].fAngle ) * Message[ nCntMessage ].fLength;		// �����̒��_X
		Message[ nCntMessage ].fLeftDown.y  = Message[ nCntMessage ].fPos.y + sin( Message[ nCntMessage ].fRot + D3DX_PI - Message[ nCntMessage ].fAngle ) * Message[ nCntMessage ].fLength;		// �����̒��_Y
		Message[ nCntMessage ].fRightDown.x = Message[ nCntMessage ].fPos.x + cos( Message[ nCntMessage ].fRot		     + Message[ nCntMessage ].fAngle ) * Message[ nCntMessage ].fLength;		// �E���̒��_X
		Message[ nCntMessage ].fRightDown.y = Message[ nCntMessage ].fPos.y + sin( Message[ nCntMessage ].fRot		     + Message[ nCntMessage ].fAngle ) * Message[ nCntMessage ].fLength;		// �E���̒��_Y
		
		// ���_���W�̐ݒ�
		pVtx[ 0 ].pos.x = Message[ nCntMessage ].fLeftUp.x;		// ����̒��_X
		pVtx[ 0 ].pos.y = Message[ nCntMessage ].fLeftUp.y;		// ����̒��_Y		
		pVtx[ 1 ].pos.x = Message[ nCntMessage ].fRightUp.x;	// �E��̒��_X
		pVtx[ 1 ].pos.y = Message[ nCntMessage ].fRightUp.y;	// �E��̒��_Y
		pVtx[ 2 ].pos.x = Message[ nCntMessage ].fLeftDown.x;	// �����̒��_X
		pVtx[ 2 ].pos.y = Message[ nCntMessage ].fLeftDown.y;	// �����̒��_Y
		pVtx[ 3 ].pos.x = Message[ nCntMessage ].fRightDown.x;	// �E���̒��_X
		pVtx[ 3 ].pos.y = Message[ nCntMessage ].fRightDown.y;	// �E���̒��_Y
		
		if( Message[ nCntMessage ].nType == MESSAGE_TYPE_STAGE )
		{
			int temp = GetStage();
			pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.33f * ( temp - 1 ) );	// ����̍��W
			pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.33f * ( temp - 1 ) );	// �E��̍��W
			pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 0.33f * temp );			// �����̍��W
			pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 0.33f * temp );			// �E���̍��W
		}
		else
		{
			// �e�N�X�`�����W�̐ݒ�( 0.0f�`1.0f )
			pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );	// ����̍��W
			pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );	// �E��̍��W
			pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );	// �����̍��W
			pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );	// �E���̍��W
		}
		
		// ���_�F�̐ݒ�
		pVtx[ 0 ].color = Message[ nCntMessage ].fColor;
		pVtx[ 1 ].color = Message[ nCntMessage ].fColor;
		pVtx[ 2 ].color = Message[ nCntMessage ].fColor;
		pVtx[ 3 ].color = Message[ nCntMessage ].fColor;

		// ���̃|���S���̒��_��
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBufferMessage->Unlock();

	// �J�E���^�[�̃C���N�������g
	g_nFrameCount++;
}

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
void DrawMessage( void )
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �X�g���[��(�p�C�v���C��)�ݒ�
	pDevice->SetStreamSource( 0,						// �q����p�C�v�̔ԍ�
							  g_pVtxBufferMessage,		// �X�g���[���̌��ƂȂ钸�_�o�b�t�@�̎w��
							  0,						// �I�t�Z�b�g(�o�C�g)
							  sizeof( VERTEX_2D ) );		// �X�g���C�h�l(�X�g���C�h��)

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF( FVF_VERTEX_2D );

	// �e�N�X�`���̐ݒ�
	switch( Message[ 0 ].nType )
	{
	case MESSAGE_TYPE_STAGE:
		pDevice->SetTexture( 0, g_pTextureMessage[ 0 ] );
		break;
	case MESSAGE_TYPE_READY:
		pDevice->SetTexture( 0, g_pTextureMessage[ 1 ] );
		break;
	case MESSAGE_TYPE_GO:
		pDevice->SetTexture( 0, g_pTextureMessage[ 2 ] );
		break;
	case MESSAGE_TYPE_GOAL:
		pDevice->SetTexture( 0, g_pTextureMessage[ 3 ] );
		break;
	case MESSAGE_TYPE_TIMEUP:
		pDevice->SetTexture( 0, g_pTextureMessage[ 4 ] );
		break;
	case MESSAGE_TYPE_GAMEOVER:
		pDevice->SetTexture( 0, g_pTextureMessage[ 5 ] );
		break;
	}
	
	for( int nCntMessage = 0; nCntMessage < MESSAGE_MAX; nCntMessage++ )
	{
		if( !Message[ nCntMessage ].bUse ) { continue; }
				
		// �v���~�e�B�u�̕`��
		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
								nCntMessage * 4,			// �I�t�Z�b�g(���_��)
								2 );						// �v���~�e�B�u��
	}
}

/*------------------------------------------------------------------------------
  �@���b�Z�[�W�̍쐬
------------------------------------------------------------------------------*/
void CreateMessage( D3DXVECTOR2 pos, D3DXVECTOR2 size, float speed, float direct, float angleOfFire, D3DXCOLOR color, D3DXCOLOR colorValue,  MESSAGETYPE type )
{
	for( int nCntMessage = 0; nCntMessage < MESSAGE_MAX; nCntMessage++ )
	{
		// �G�t�F�N�g���g���Ă��Ȃ�������
		if( !Message[ nCntMessage ].bUse )
		{
			Message[ nCntMessage ].bUse			= true;
			Message[ nCntMessage ].fPos			= pos;
			Message[ nCntMessage ].fLength		= sqrt( ( size.y * size.y ) + ( size.x * size.x  ) );
			Message[ nCntMessage ].fAngle		= atan2( size.y, size.x );
			Message[ nCntMessage ].fDirect		= direct;
			Message[ nCntMessage ].fRot			= angleOfFire;
			Message[ nCntMessage ].fVelocity.x	= speed * cos( direct );
			Message[ nCntMessage ].fVelocity.y	= speed * sin( direct );
			Message[ nCntMessage ].fColor		= color;
			Message[ nCntMessage ].fColorValue	= colorValue;
			Message[ nCntMessage ].nType		= type;

			break;	// for���𔲂���
		}
	}
}

/*------------------------------------------------------------------------------
  �@�Q�b�^�[
------------------------------------------------------------------------------*/
MESSAGE* GetMessage( void )
{
	return Message;
}

/*------------------------------------------------------------------------------
  �@�Z�b�^�[
------------------------------------------------------------------------------*/
void SetMessageType( MESSAGETYPE type )
{
	Message[ 0 ].nType = type;
}

//******************************* EOF **************************************