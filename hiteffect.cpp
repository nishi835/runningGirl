/*==============================================================================

    �p�[�e�B�N���̏��� [hiteffect.cpp]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/5/31
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  �@�w�b�_�t�@�C��
------------------------------------------------------------------------------*/
#include "main.h"
#include "hiteffect.h"
#include "polygon.h"
#include "player.h"

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define HITEFFECT_TEXTURENAME	 "data/TEXTURE/line.png"				// �e�N�X�`����(.png �� / .jpg / .tga �� / .bmp / .dds ��) ���͔��������g�p��

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTextureHitEffect   = NULL;		// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^�ϐ�
LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferHitEffect = NULL;		// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

HITEFFECT				HitEffect[ HITEFFECT_MAX ];		// �G�t�F�N�g�̍\����
int						g_nCntHitEffectPolygon = 0;		// �G�t�F�N�g�̃|���S�����i�`�悷��G�t�F�N�g�̐��~2�j

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
HRESULT MakeVertexHitEffect( LPDIRECT3DDEVICE9 pDevice );

/*------------------------------------------------------------------------------
  �@�p�[�e�B�N���̏�����
------------------------------------------------------------------------------*/
void InitHitEffect( void )
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �����l�̐ݒ�
	for( int nCntHitEffect = 0; nCntHitEffect < HITEFFECT_MAX; nCntHitEffect++ )
	{
		HitEffect[ nCntHitEffect ].bUse			= false;
		HitEffect[ nCntHitEffect ].fPos.x		= 0.0f;
		HitEffect[ nCntHitEffect ].fPos.y		= 0.0f;
		HitEffect[ nCntHitEffect ].fSize.x		= HITEFFECT_WIDTH;
		HitEffect[ nCntHitEffect ].fSize.y		= HITEFFECT_HEIGHT;
		HitEffect[ nCntHitEffect ].fRot			= 0.0f;
		HitEffect[ nCntHitEffect ].fLength		= 0.0f;
		HitEffect[ nCntHitEffect ].fRadius		= 0.0f;
		HitEffect[ nCntHitEffect ].fAngle		= 0.0f;
		HitEffect[ nCntHitEffect ].fScale		= 1.0f;
		HitEffect[ nCntHitEffect ].fSpeed		= 0.0f;
		HitEffect[ nCntHitEffect ].fVelocity.x	= 0.0f;
		HitEffect[ nCntHitEffect ].fVelocity.y	= 0.0f;
		HitEffect[ nCntHitEffect ].nLife		= 0;
	}

	// ���_�̍쐬
	MakeVertexPolygon( &g_pVtxBufferHitEffect, HITEFFECT_MAX );

	// �e�N�X�`���̓ǂݍ���
	if( FAILED( D3DXCreateTextureFromFile( pDevice, HITEFFECT_TEXTURENAME, &g_pTextureHitEffect ) ) )
	{
		MessageBox( NULL, "HitEffect�̃e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B", "�G���[���b�Z�[�W", MB_OK | MB_ICONEXCLAMATION );
	}
}

/*------------------------------------------------------------------------------
  �@�p�[�e�B�N���̏I������
------------------------------------------------------------------------------*/
void UninitHitEffect( void )
{
	// �e�N�X�`���C���^�[�t�F�[�X�̏I������
	if( g_pTextureHitEffect )
	{
		g_pTextureHitEffect->Release();
		g_pTextureHitEffect = NULL;
	}

	// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^�̏I������
	if( g_pVtxBufferHitEffect )
	{
		g_pVtxBufferHitEffect->Release();
		g_pVtxBufferHitEffect = NULL;
	}
}

/*------------------------------------------------------------------------------
  �@�p�[�e�B�N���̍X�V
------------------------------------------------------------------------------*/
void UpdateHitEffect( void )
{
	// �v���C���[���̎擾
	PLAYER* pPlayer = GetPlayer();

	// ���W�X�V
	for( int nCntHitEffect = 0; nCntHitEffect < HITEFFECT_MAX; nCntHitEffect++ )
	{
		// ���g�p���Ȃ�ȉ��̃v���O�������΂�
		if( !HitEffect[ nCntHitEffect ].bUse ) { continue; }

		HitEffect[ nCntHitEffect ].fPos.x += HitEffect[ nCntHitEffect ].fVelocity.x;
		HitEffect[ nCntHitEffect ].fPos.y += HitEffect[ nCntHitEffect ].fVelocity.y;
		HitEffect[ nCntHitEffect ].fColor += HitEffect[ nCntHitEffect ].fColorValue;
		HitEffect[ nCntHitEffect ].nLife--;

		// �̗͂��Ȃ��Ȃ��������
		if( HitEffect[ nCntHitEffect ].nLife <= 0 )
		{
			HitEffect[ nCntHitEffect ].bUse		= false;
			HitEffect[ nCntHitEffect ].fPos.x		= 0.0f;
			HitEffect[ nCntHitEffect ].fPos.y		= 0.0f;
			HitEffect[ nCntHitEffect ].fRadius		= 0.0f;
			HitEffect[ nCntHitEffect ].nLife		= 0;

			continue;
		}
	}
	
	// ���_�o�b�t�@�i�[�ϐ�
	VERTEX_2D* pVtx = NULL;

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferHitEffect->Lock( 0, 0, ( void** )&pVtx, 0 );
	
	// �X�V
	for( int nCntHitEffect = 0; nCntHitEffect < HITEFFECT_MAX; nCntHitEffect++ )
	{
		if( !HitEffect[ nCntHitEffect ].bUse ) { continue; }
		
		// �\�����W�̍X�V		
		HitEffect[ nCntHitEffect ].fRadius += HITEFFECT_RADIUS_VALUE;
		HitEffect[ nCntHitEffect ].fPos.x   = HitEffect[ nCntHitEffect ].fAxis.x + cosf( HitEffect[ nCntHitEffect ].fRot /*- D3DX_PI * 0.5f*/ ) * HitEffect[ nCntHitEffect ].fRadius;
		HitEffect[ nCntHitEffect ].fPos.y   = HitEffect[ nCntHitEffect ].fAxis.y + sinf( HitEffect[ nCntHitEffect ].fRot /*- D3DX_PI * 0.5f*/ ) * HitEffect[ nCntHitEffect ].fRadius;	

		// �l���̍��W�̍X�V
		HitEffect[ nCntHitEffect ].fLeftUp.x    = HitEffect[ nCntHitEffect ].fPos.x + cos( HitEffect[ nCntHitEffect ].fRot + D3DX_PI + HitEffect[ nCntHitEffect ].fAngle ) * HitEffect[ nCntHitEffect ].fLength;		// ����̒��_X
		HitEffect[ nCntHitEffect ].fLeftUp.y    = HitEffect[ nCntHitEffect ].fPos.y + sin( HitEffect[ nCntHitEffect ].fRot + D3DX_PI + HitEffect[ nCntHitEffect ].fAngle ) * HitEffect[ nCntHitEffect ].fLength;		// ����̒��_Y		
		HitEffect[ nCntHitEffect ].fRightUp.x   = HitEffect[ nCntHitEffect ].fPos.x + cos( HitEffect[ nCntHitEffect ].fRot		     - HitEffect[ nCntHitEffect ].fAngle ) * HitEffect[ nCntHitEffect ].fLength;		// �E��̒��_X
		HitEffect[ nCntHitEffect ].fRightUp.y   = HitEffect[ nCntHitEffect ].fPos.y + sin( HitEffect[ nCntHitEffect ].fRot		     - HitEffect[ nCntHitEffect ].fAngle ) * HitEffect[ nCntHitEffect ].fLength;		// �E��̒��_Y
		HitEffect[ nCntHitEffect ].fLeftDown.x  = HitEffect[ nCntHitEffect ].fPos.x + cos( HitEffect[ nCntHitEffect ].fRot + D3DX_PI - HitEffect[ nCntHitEffect ].fAngle ) * HitEffect[ nCntHitEffect ].fLength;		// �����̒��_X
		HitEffect[ nCntHitEffect ].fLeftDown.y  = HitEffect[ nCntHitEffect ].fPos.y + sin( HitEffect[ nCntHitEffect ].fRot + D3DX_PI - HitEffect[ nCntHitEffect ].fAngle ) * HitEffect[ nCntHitEffect ].fLength;		// �����̒��_Y
		HitEffect[ nCntHitEffect ].fRightDown.x = HitEffect[ nCntHitEffect ].fPos.x + cos( HitEffect[ nCntHitEffect ].fRot		     + HitEffect[ nCntHitEffect ].fAngle ) * HitEffect[ nCntHitEffect ].fLength;		// �E���̒��_X
		HitEffect[ nCntHitEffect ].fRightDown.y = HitEffect[ nCntHitEffect ].fPos.y + sin( HitEffect[ nCntHitEffect ].fRot		     + HitEffect[ nCntHitEffect ].fAngle ) * HitEffect[ nCntHitEffect ].fLength;		// �E���̒��_Y
		
		// ���_���W�̐ݒ�
		pVtx[ 0 ].pos.x = HitEffect[ nCntHitEffect ].fLeftUp.x;		// ����̒��_X
		pVtx[ 0 ].pos.y = HitEffect[ nCntHitEffect ].fLeftUp.y;		// ����̒��_Y		
		pVtx[ 1 ].pos.x = HitEffect[ nCntHitEffect ].fRightUp.x;	// �E��̒��_X
		pVtx[ 1 ].pos.y = HitEffect[ nCntHitEffect ].fRightUp.y;	// �E��̒��_Y
		pVtx[ 2 ].pos.x = HitEffect[ nCntHitEffect ].fLeftDown.x;	// �����̒��_X
		pVtx[ 2 ].pos.y = HitEffect[ nCntHitEffect ].fLeftDown.y;	// �����̒��_Y
		pVtx[ 3 ].pos.x = HitEffect[ nCntHitEffect ].fRightDown.x;	// �E���̒��_X
		pVtx[ 3 ].pos.y = HitEffect[ nCntHitEffect ].fRightDown.y;	// �E���̒��_Y
		
		// �e�N�X�`�����W�̐ݒ�( 0.0f�`1.0f )
		pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );	// ����̍��W
		pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );	// �E��̍��W
		pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );	// �����̍��W
		pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );	// �����̍��W
		
		// ���_�F�̐ݒ�
		pVtx[ 0 ].color = HitEffect[ nCntHitEffect ].fColor;
		pVtx[ 1 ].color = HitEffect[ nCntHitEffect ].fColor;
		pVtx[ 2 ].color = HitEffect[ nCntHitEffect ].fColor;
		pVtx[ 3 ].color = HitEffect[ nCntHitEffect ].fColor;

		// ���̃|���S���̒��_��
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBufferHitEffect->Unlock();
}

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
void DrawHitEffect( void )
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// ���Z�����ɐ؂�ւ�
	//pDevice->SetRenderState( D3DRS_DESTBLEND,		// �ݒ薼(�C���f�b�N�X)	D3DRS_DESTBLEND�F�w�i�̃A���t�@�ݒ�
	//						   D3DBLEND_ONE );		// �ݒ�l

	// �X�g���[��(�p�C�v���C��)�ݒ�
	pDevice->SetStreamSource( 0,						// �q����p�C�v�̔ԍ�
							  g_pVtxBufferHitEffect,		// �X�g���[���̌��ƂȂ钸�_�o�b�t�@�̎w��
							  0,						// �I�t�Z�b�g(�o�C�g)
							  sizeof( VERTEX_2D ) );		// �X�g���C�h�l(�X�g���C�h��)

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF( FVF_VERTEX_2D );

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0, g_pTextureHitEffect );
	
	for( int nCntHitEffect = 0; nCntHitEffect < HITEFFECT_MAX; nCntHitEffect++ )
	{
		if( !HitEffect[ nCntHitEffect ].bUse ) { continue; }
				
		// �v���~�e�B�u�̕`��
		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
								nCntHitEffect * 4,			// �I�t�Z�b�g(���_��)
								2 );						// �v���~�e�B�u��
	}

	// ��Z�����ɐ؂�ւ�
	pDevice->SetRenderState( D3DRS_DESTBLEND,			// �ݒ薼(�C���f�b�N�X)	D3DRS_DESTBLEND�F�w�i�̃A���t�@�ݒ�
						     D3DBLEND_INVSRCALPHA );	// �ݒ�l	D3DBLEND_INVSRCALPHA�F�P�|��

}

/*------------------------------------------------------------------------------
  �@�p�[�e�B�N���̍쐬
--------------------------------------------------------------------------------
   ���� pos			 : ���W
        scale		 : �g��x
        color		 : �F
        alphaValue	 : ���̕ω���
        life		 : �̗�
        type		 : �G�t�F�N�g�̎��
------------------------------------------------------------------------------*/
void CreateHitEffect( D3DXVECTOR2 pos, D3DXVECTOR2 size, float speed, float angleOfFire, D3DXCOLOR color, D3DXCOLOR colorValue,  int life )
{
	for( int nCntHitEffect = 0; nCntHitEffect < HITEFFECT_MAX; nCntHitEffect++ )
	{
		// �G�t�F�N�g���g���Ă��Ȃ�������
		if( !HitEffect[ nCntHitEffect ].bUse )
		{
			HitEffect[ nCntHitEffect ].bUse			= true;
			HitEffect[ nCntHitEffect ].fAxis		= pos;
			HitEffect[ nCntHitEffect ].fLength		= sqrt( ( size.y * size.y ) + ( size.x * size.x  ) );
			HitEffect[ nCntHitEffect ].fAngle		= atan2( size.y, size.x );
			HitEffect[ nCntHitEffect ].fRot			= angleOfFire;
			HitEffect[ nCntHitEffect ].fVelocity.x	= speed * cos( angleOfFire );
			HitEffect[ nCntHitEffect ].fVelocity.y	= speed * sin( angleOfFire );
			HitEffect[ nCntHitEffect ].fColor		= color;
			HitEffect[ nCntHitEffect ].fColorValue	= colorValue;
			HitEffect[ nCntHitEffect ].nLife		= life;

			break;	// for���𔲂���
		}
	}
}

//******************************* EOF **************************************