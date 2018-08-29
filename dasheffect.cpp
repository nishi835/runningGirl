/*==============================================================================

    �p�[�e�B�N���̏��� [dasheffect.cpp]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/5/31
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  �@�w�b�_�t�@�C��
------------------------------------------------------------------------------*/
#include "main.h"
#include "dasheffect.h"
#include "polygon.h"
#include "player.h"

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define DASHEFFECT_TEXTURENAME	 "data/TEXTURE/effect000.jpg"				// �e�N�X�`����(.png �� / .jpg / .tga �� / .bmp / .dds ��) ���͔��������g�p��
#define DASHEFFECT_SIZE_VALUE    (0.1f)                                 // �G�t�F�N�g�̃T�C�Y�̕ω���

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTextureDashEffect   = NULL;		// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^�ϐ�
LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferDashEffect = NULL;		// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

DASHEFFECT				DashEffect[ DASHEFFECT_MAX ];					// �G�t�F�N�g�̍\����
DASHEFFECTEMITTER		DashEffectEmitter[ DASHEFFECT_EMITTER_MAX ];	// �G�t�F�N�g�̔������̍\����
int						g_nCntDashEffectPolygon = 0;					// �G�t�F�N�g�̃|���S�����i�`�悷��G�t�F�N�g�̐��~2�j

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
HRESULT MakeVertexDashEffect( LPDIRECT3DDEVICE9 pDevice );

/*------------------------------------------------------------------------------
  �@�p�[�e�B�N���̏�����
------------------------------------------------------------------------------*/
void InitDashEffect( void )
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �����l�̐ݒ�
	for( int nCntDashEffect = 0; nCntDashEffect < DASHEFFECT_MAX; nCntDashEffect++ )
	{
		DashEffect[ nCntDashEffect ].bUse		= false;
		DashEffect[ nCntDashEffect ].fPos.x		= 0.0f;
		DashEffect[ nCntDashEffect ].fPos.y		= 0.0f;
		DashEffect[ nCntDashEffect ].fVelocity	= D3DXVECTOR2( 0.0f, 0.0f );
		DashEffect[ nCntDashEffect ].fSize.x	= DASHEFFECT_WIDTH;
		DashEffect[ nCntDashEffect ].fSize.y	= DASHEFFECT_HEIGHT;
		DashEffect[ nCntDashEffect ].fColor		= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		DashEffect[ nCntDashEffect ].fLength	= 0.0f;
		DashEffect[ nCntDashEffect ].fAngle		= 0.0f;
		DashEffect[ nCntDashEffect ].fRot		= 0.0f;
		DashEffect[ nCntDashEffect ].fScale		= 1.0f;
		DashEffect[ nCntDashEffect ].nLife		= 0;
	}

	// ���_�̍쐬
	MakeVertexPolygon( &g_pVtxBufferDashEffect, DASHEFFECT_MAX );

	// �e�N�X�`���̓ǂݍ���
	if( FAILED( D3DXCreateTextureFromFile( pDevice, DASHEFFECT_TEXTURENAME, &g_pTextureDashEffect ) ) )
	{
		MessageBox( NULL, "DashEffect�̃e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B", "�G���[���b�Z�[�W", MB_OK | MB_ICONEXCLAMATION );
	}
}

/*------------------------------------------------------------------------------
  �@�p�[�e�B�N���̏I������
------------------------------------------------------------------------------*/
void UninitDashEffect( void )
{
	// �e�N�X�`���C���^�[�t�F�[�X�̏I������
	if( g_pTextureDashEffect )
	{
		g_pTextureDashEffect->Release();
		g_pTextureDashEffect = NULL;
	}

	// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^�̏I������
	if( g_pVtxBufferDashEffect )
	{
		g_pVtxBufferDashEffect->Release();
		g_pVtxBufferDashEffect = NULL;
	}
}

/*------------------------------------------------------------------------------
  �@�p�[�e�B�N���̍X�V
------------------------------------------------------------------------------*/
void UpdateDashEffect( void )
{
	// �J�E���^�[�̏���
	static int nFrameCount = 0;

	// �v���C���[���̎擾
	PLAYER* pPlayer = GetPlayer();

	// ���W�X�V
	for( int nCntDashEffect = 0; nCntDashEffect < DASHEFFECT_MAX; nCntDashEffect++ )
	{
		// ���g�p���Ȃ�ȉ��̃v���O�������΂�
		if( !DashEffect[ nCntDashEffect ].bUse ) { continue; }
	}

	// ����������G�t�F�N�g�𐶐�	
	for( int nCntDashEffectEmitter = 0; nCntDashEffectEmitter < DASHEFFECT_EMITTER_MAX; nCntDashEffectEmitter++ )
	{
		// ���������g���Ă�����
		if( DashEffectEmitter[ nCntDashEffectEmitter ].bUse )
		{	
			// �G�t�F�N�g�̐���
			for( int nCntDashEffect = 0; nCntDashEffect < DASHEFFECT_MAX; nCntDashEffect++ )
			{
				// �G�t�F�N�g���g���Ă�����
				if( DashEffect[ nCntDashEffect ].bUse ) { continue; }
 
				DashEffect[ nCntDashEffect ].fPos	   = DashEffectEmitter[ nCntDashEffectEmitter ].fPos;
				DashEffect[ nCntDashEffect ].fSize	   = DashEffectEmitter[ nCntDashEffectEmitter ].fSize;
				DashEffect[ nCntDashEffect ].fVelocity = DashEffectEmitter[ nCntDashEffectEmitter ].fVelocity;
				DashEffect[ nCntDashEffect ].fAngle	   = DashEffectEmitter[ nCntDashEffectEmitter ].fAngle;
				DashEffect[ nCntDashEffect ].fLength   = DashEffectEmitter[ nCntDashEffectEmitter ].fLength;
				DashEffect[ nCntDashEffect ].nLife	   = DashEffectEmitter[ nCntDashEffectEmitter ].nLife;
				DashEffect[ nCntDashEffect ].nMaxLife  = DashEffectEmitter[ nCntDashEffectEmitter ].nLife;
				DashEffect[ nCntDashEffect ].bUse	   = true;

				break;
			}
		}
	}

	// ���_�o�b�t�@�i�[�ϐ�
	VERTEX_2D* pVtx = NULL;

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferDashEffect->Lock( 0, 0, ( void** )&pVtx, 0 );
	
	// �G�t�F�N�g�̍X�V
	for( int nCntDashEffect = 0; nCntDashEffect < DASHEFFECT_MAX; nCntDashEffect++ )
	{
		if( !DashEffect[ nCntDashEffect ].bUse ) { continue; }
		
		// ���S���W�A�̗͂̍X�V		
		DashEffect[ nCntDashEffect ].fPos += DashEffect[ nCntDashEffect ].fVelocity; 
		DashEffect[ nCntDashEffect ].nLife--;

		// ���W�̍X�V
		//DashEffectEmitter[ nCntDashEffect ].fPos		 = pPlayer->fPos;

		// �J�E���^�[�̃C���N�������g
		DashEffect[ nCntDashEffect ].nCounter++;

		// �T�C�Y�̍X�V
		if( DashEffect[ nCntDashEffect ].nCounter < DashEffect[ nCntDashEffect ].nMaxLife * 0.5f )
		{
			DashEffect[ nCntDashEffect ].fSize.x += DASHEFFECT_SIZE_VALUE;
			DashEffect[ nCntDashEffect ].fSize.y += DASHEFFECT_SIZE_VALUE;
		}
		else
		{
			DashEffect[ nCntDashEffect ].fSize.x -= DASHEFFECT_SIZE_VALUE;
			DashEffect[ nCntDashEffect ].fSize.y -= DASHEFFECT_SIZE_VALUE;
		}

		// ��]�֌W�̍X�V
		DashEffect[ nCntDashEffect ].fLength = sqrt( DashEffect[ nCntDashEffect ].fSize.x * DashEffect[ nCntDashEffect ].fSize.x + DashEffect[ nCntDashEffect ].fSize.y * DashEffect[ nCntDashEffect ].fSize.y );
		DashEffect[ nCntDashEffect ].fAngle	 = atan2( DashEffect[ nCntDashEffect ].fSize.y, DashEffect[ nCntDashEffect ].fSize.x );

		// �̗͂��Ȃ��Ȃ��������
		if( DashEffect[ nCntDashEffect ].nLife <= 0 )
		{
			DashEffect[ nCntDashEffect ].bUse		= false;
			DashEffect[ nCntDashEffect ].fPos.x		= 0.0f;
			DashEffect[ nCntDashEffect ].fPos.y		= 0.0f;
			DashEffect[ nCntDashEffect ].fVelocity	= D3DXVECTOR2( 0.0f, 0.0f );
			DashEffect[ nCntDashEffect ].nCounter	= 0;
		}

		// �l���̍��W�̍X�V
		DashEffect[ nCntDashEffect ].fLeftUp.x    = DashEffect[ nCntDashEffect ].fPos.x + cos( DashEffect[ nCntDashEffect ].fRot + D3DX_PI + DashEffect[ nCntDashEffect ].fAngle ) * DashEffect[ nCntDashEffect ].fLength;		// ����̒��_X
		DashEffect[ nCntDashEffect ].fLeftUp.y    = DashEffect[ nCntDashEffect ].fPos.y + sin( DashEffect[ nCntDashEffect ].fRot + D3DX_PI + DashEffect[ nCntDashEffect ].fAngle ) * DashEffect[ nCntDashEffect ].fLength;		// ����̒��_Y		
		DashEffect[ nCntDashEffect ].fRightUp.x   = DashEffect[ nCntDashEffect ].fPos.x + cos( DashEffect[ nCntDashEffect ].fRot		   - DashEffect[ nCntDashEffect ].fAngle ) * DashEffect[ nCntDashEffect ].fLength;		// �E��̒��_X
		DashEffect[ nCntDashEffect ].fRightUp.y   = DashEffect[ nCntDashEffect ].fPos.y + sin( DashEffect[ nCntDashEffect ].fRot		   - DashEffect[ nCntDashEffect ].fAngle ) * DashEffect[ nCntDashEffect ].fLength;		// �E��̒��_Y
		DashEffect[ nCntDashEffect ].fLeftDown.x  = DashEffect[ nCntDashEffect ].fPos.x + cos( DashEffect[ nCntDashEffect ].fRot + D3DX_PI - DashEffect[ nCntDashEffect ].fAngle ) * DashEffect[ nCntDashEffect ].fLength;		// �����̒��_X
		DashEffect[ nCntDashEffect ].fLeftDown.y  = DashEffect[ nCntDashEffect ].fPos.y + sin( DashEffect[ nCntDashEffect ].fRot + D3DX_PI - DashEffect[ nCntDashEffect ].fAngle ) * DashEffect[ nCntDashEffect ].fLength;		// �����̒��_Y
		DashEffect[ nCntDashEffect ].fRightDown.x = DashEffect[ nCntDashEffect ].fPos.x + cos( DashEffect[ nCntDashEffect ].fRot		   + DashEffect[ nCntDashEffect ].fAngle ) * DashEffect[ nCntDashEffect ].fLength;		// �E���̒��_X
		DashEffect[ nCntDashEffect ].fRightDown.y = DashEffect[ nCntDashEffect ].fPos.y + sin( DashEffect[ nCntDashEffect ].fRot		   + DashEffect[ nCntDashEffect ].fAngle ) * DashEffect[ nCntDashEffect ].fLength;		// �E���̒��_Y
		
		// ���_���W�̐ݒ�
		pVtx[ 0 ].pos.x = DashEffect[ nCntDashEffect ].fLeftUp.x;		// ����̒��_X
		pVtx[ 0 ].pos.y = DashEffect[ nCntDashEffect ].fLeftUp.y;		// ����̒��_Y		
		pVtx[ 1 ].pos.x = DashEffect[ nCntDashEffect ].fRightUp.x;		// �E��̒��_X
		pVtx[ 1 ].pos.y = DashEffect[ nCntDashEffect ].fRightUp.y;		// �E��̒��_Y
		pVtx[ 2 ].pos.x = DashEffect[ nCntDashEffect ].fLeftDown.x;		// �����̒��_X
		pVtx[ 2 ].pos.y = DashEffect[ nCntDashEffect ].fLeftDown.y;		// �����̒��_Y
		pVtx[ 3 ].pos.x = DashEffect[ nCntDashEffect ].fRightDown.x;	// �E���̒��_X
		pVtx[ 3 ].pos.y = DashEffect[ nCntDashEffect ].fRightDown.y;	// �E���̒��_Y
		
		// �e�N�X�`�����W�̐ݒ�( 0.0f�`1.0f )
		pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );	// ����̍��W
		pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );	// �E��̍��W
		pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );	// �����̍��W
		pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );	// �����̍��W
		
		// ���_�F�̐ݒ�
		pVtx[ 0 ].color = DashEffect[ nCntDashEffect ].fColor;
		pVtx[ 1 ].color = DashEffect[ nCntDashEffect ].fColor;
		pVtx[ 2 ].color = DashEffect[ nCntDashEffect ].fColor;
		pVtx[ 3 ].color = DashEffect[ nCntDashEffect ].fColor;

		// ���̃|���S���̒��_��
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBufferDashEffect->Unlock();

	// �J�E���^�[�̃C���N�������g
	nFrameCount++;
}

/*------------------------------------------------------------------------------
  �@�`�揈��
------------------------------------------------------------------------------*/
void DrawDashEffect( void )
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// ���Z�����ɐ؂�ւ�
	pDevice->SetRenderState( D3DRS_DESTBLEND,		// �ݒ薼(�C���f�b�N�X)	D3DRS_DESTBLEND�F�w�i�̃A���t�@�ݒ�
							   D3DBLEND_ONE );		// �ݒ�l

	// �X�g���[��(�p�C�v���C��)�ݒ�
	pDevice->SetStreamSource( 0,						// �q����p�C�v�̔ԍ�
							  g_pVtxBufferDashEffect,		// �X�g���[���̌��ƂȂ钸�_�o�b�t�@�̎w��
							  0,						// �I�t�Z�b�g(�o�C�g)
							  sizeof( VERTEX_2D ) );		// �X�g���C�h�l(�X�g���C�h��)

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF( FVF_VERTEX_2D );

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0, g_pTextureDashEffect );
	
	for( int nCntDashEffect = 0; nCntDashEffect < DASHEFFECT_MAX; nCntDashEffect++ )
	{
		if( !DashEffect[ nCntDashEffect ].bUse ) { continue; }
				
		// �v���~�e�B�u�̕`��
		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
								nCntDashEffect * 4,			// �I�t�Z�b�g(���_��)
								2 );						// �v���~�e�B�u��
	}

	// ��Z�����ɐ؂�ւ�
	pDevice->SetRenderState( D3DRS_DESTBLEND,			// �ݒ薼(�C���f�b�N�X)	D3DRS_DESTBLEND�F�w�i�̃A���t�@�ݒ�
						     D3DBLEND_INVSRCALPHA );	// �ݒ�l	D3DBLEND_INVSRCALPHA�F�P�|��

}

/*------------------------------------------------------------------------------
  �@�G�t�F�N�g�̔������̍쐬
------------------------------------------------------------------------------*/
void CreateDashEffectEmitter( D3DXVECTOR2 pos, D3DXVECTOR2 size, D3DXVECTOR2 velocity, int life )
{
	for( int nCntDashEffectEmitter = 0; nCntDashEffectEmitter < DASHEFFECT_EMITTER_MAX; nCntDashEffectEmitter++ )
	{
		// ���������g���Ă��Ȃ�������
		if( !DashEffectEmitter[ nCntDashEffectEmitter ].bUse )
		{
			DashEffectEmitter[ nCntDashEffectEmitter ].bUse		 = true;
			DashEffectEmitter[ nCntDashEffectEmitter ].fPos		 = pos;
			DashEffectEmitter[ nCntDashEffectEmitter ].fSize	 = size;
			DashEffectEmitter[ nCntDashEffectEmitter ].fLength	 = sqrt( size.x * size.x + size.y * size.y );
			DashEffectEmitter[ nCntDashEffectEmitter ].fAngle	 = atan2( size.y, size.x );
			DashEffectEmitter[ nCntDashEffectEmitter ].fVelocity = velocity;
			DashEffectEmitter[ nCntDashEffectEmitter ].nLife	 = life;

			break;	// for���𔲂���
		}
	}
}

/*------------------------------------------------------------------------------
  �@�G�t�F�N�g�̔������̏���
------------------------------------------------------------------------------*/
void UnuseDashEffect( void )
{
	for( int nCntDashEffectEmitter = 0; nCntDashEffectEmitter < DASHEFFECT_EMITTER_MAX; nCntDashEffectEmitter++ )
	{
		// ���������g���Ă�����
		if( DashEffectEmitter[ nCntDashEffectEmitter ].bUse )
		{
			DashEffectEmitter[ nCntDashEffectEmitter ].bUse		 = false;
			DashEffectEmitter[ nCntDashEffectEmitter ].fPos		 = D3DXVECTOR2( 0.0f, 0.0f );
			DashEffectEmitter[ nCntDashEffectEmitter ].fSize	 = D3DXVECTOR2( 0.0f, 0.0f );
			DashEffectEmitter[ nCntDashEffectEmitter ].fLength	 = 0.0f;
			DashEffectEmitter[ nCntDashEffectEmitter ].fAngle	 = 0.0f;
			DashEffectEmitter[ nCntDashEffectEmitter ].fVelocity = D3DXVECTOR2( 0.0f, 0.0f );
			DashEffectEmitter[ nCntDashEffectEmitter ].nLife	 = 0;
		}
	}
}

/*------------------------------------------------------------------------------
  �@�G�t�F�N�g�̔������̈ړ�
------------------------------------------------------------------------------*/
void MoveDashEffectEmitter( float movey )
{
	for( int nCntDashEffectEmitter = 0; nCntDashEffectEmitter < DASHEFFECT_EMITTER_MAX; nCntDashEffectEmitter++ ) 
	{
		DashEffectEmitter[ nCntDashEffectEmitter ].fPos.y += movey;
	}
}

//******************************* EOF **************************************