/*==============================================================================

    �p�[�e�B�N���̏��� [particle.cpp]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/5/31
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  �@�w�b�_�t�@�C��
------------------------------------------------------------------------------*/
#include "main.h"
#include "particle.h"
#include "polygon.h"
#include "player.h"

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define PARTICLE_TEXTURENAME	"data/TEXTURE/explosion001.png"	// �e�N�X�`����(.png �� / .jpg / .tga �� / .bmp / .dds ��) ���͔��������g�p��
#define PARTICLE_MAX			(2048)							// �G�t�F�N�g�̍ő吔

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTextureParticle   = NULL;		// �e�N�X�`���C���^�[�t�F�[�X�̃|�C���^�ϐ�
LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferParticle = NULL;	// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

PARTICLE				Particle[ PARTICLE_MAX ];		// �G�t�F�N�g�̍\����
int						g_nCntParticlePolygon = 0;	// �G�t�F�N�g�̃|���S�����i�`�悷��G�t�F�N�g�̐��~2�j

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
HRESULT MakeVertexParticle( LPDIRECT3DDEVICE9 pDevice );

/*------------------------------------------------------------------------------
  �@�p�[�e�B�N���̏�����
------------------------------------------------------------------------------*/
void InitParticle( void )
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �����l�̐ݒ�
	for( int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++ )
	{
		Particle[ nCntParticle ].bUse			= false;
		Particle[ nCntParticle ].fPos.x			= 0.0f;
		Particle[ nCntParticle ].fPos.y			= 0.0f;
		Particle[ nCntParticle ].fSize.x		= PARTICLE_WIDTH;
		Particle[ nCntParticle ].fSize.y		= PARTICLE_HEIGHT;
		Particle[ nCntParticle ].fRot			= 0.0f;
		Particle[ nCntParticle ].fLength		= 0.0f;
		Particle[ nCntParticle ].fAngle			= 0.0f;
		Particle[ nCntParticle ].fScale			= 1.0f;
		Particle[ nCntParticle ].fSpeed			= 0.0f;
		Particle[ nCntParticle ].fVelocity.x	= 0.0f;
		Particle[ nCntParticle ].fVelocity.y	= 0.0f;
		Particle[ nCntParticle ].nLife			= 0;
	}

	// ���_�̍쐬
	MakeVertexPolygon( &g_pVtxBufferParticle, PARTICLE_MAX );

	// �e�N�X�`���̓ǂݍ���
	if( FAILED( D3DXCreateTextureFromFile( pDevice, PARTICLE_TEXTURENAME, &g_pTextureParticle ) ) )
	{
		MessageBox( NULL, "Particle�̃e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B", "�G���[���b�Z�[�W", MB_OK | MB_ICONEXCLAMATION );
	}
}

/*------------------------------------------------------------------------------
  �@�p�[�e�B�N���̏I������
------------------------------------------------------------------------------*/
void UninitParticle( void )
{
	// �e�N�X�`���C���^�[�t�F�[�X�̏I������
	if( g_pTextureParticle )
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}

	// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^�̏I������
	if( g_pVtxBufferParticle )
	{
		g_pVtxBufferParticle->Release();
		g_pVtxBufferParticle = NULL;
	}
}

/*------------------------------------------------------------------------------
  �@�p�[�e�B�N���̍X�V
------------------------------------------------------------------------------*/
void UpdateParticle( void )
{
	// �v���C���[���̎擾
	PLAYER* pPlayer = GetPlayer();

	// ���W�X�V
	for( int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++ )
	{
		// ���g�p���Ȃ�ȉ��̃v���O�������΂�
		if( !Particle[ nCntParticle ].bUse ) { continue; }

		Particle[ nCntParticle ].fPos.x += Particle[ nCntParticle ].fVelocity.x;
		Particle[ nCntParticle ].fPos.y += Particle[ nCntParticle ].fVelocity.y;
		Particle[ nCntParticle ].fColor += Particle[ nCntParticle ].fColorValue;
		Particle[ nCntParticle ].nLife--;

		// �̗͂��Ȃ��Ȃ��������
		if( Particle[ nCntParticle ].nLife <= 0 )
		{
			Particle[ nCntParticle ].bUse		= false;
			Particle[ nCntParticle ].fPos.x		= 0.0f;
			Particle[ nCntParticle ].fPos.y		= 0.0f;
			Particle[ nCntParticle ].nLife		= 0;

			continue;
		}

		// ��ʊO(���E)�ɏo�������
		if( Particle[ nCntParticle ].fPos.x - Particle[ nCntParticle ].fLength < 0.0f ||
			Particle[ nCntParticle ].fPos.x + Particle[ nCntParticle ].fLength > SCREEN_WIDTH )
		{
			Particle[ nCntParticle ].bUse		= false;
			Particle[ nCntParticle ].fPos.x		= 0.0f;
			Particle[ nCntParticle ].fPos.y		= 0.0f;
			Particle[ nCntParticle ].nLife		= 0;

			continue;
		}

		// ��ʊO(�㉺)�ɏo�������
		if( Particle[ nCntParticle ].fPos.y - Particle[ nCntParticle ].fLength < 0.0f ||
			Particle[ nCntParticle ].fPos.y + Particle[ nCntParticle ].fLength > SCREEN_HEIGHT )
		{
			Particle[ nCntParticle ].bUse		= false;
			Particle[ nCntParticle ].fPos.x		= 0.0f;
			Particle[ nCntParticle ].fPos.y		= 0.0f;
			Particle[ nCntParticle ].nLife		= 0;

			continue;
		}
	}
	
	// ���_�o�b�t�@�i�[�ϐ�
	VERTEX_2D* pVtx = NULL;

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferParticle->Lock( 0, 0, (void**)&pVtx, 0 );
	
	// �X�V
	for( int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++ )
	{
		if( !Particle[ nCntParticle ].bUse ) { continue; }
				
		// �l���̍��W�̍X�V
		Particle[ nCntParticle ].fLeftUp.x    = Particle[ nCntParticle ].fPos.x + cos( Particle[ nCntParticle ].fRot + D3DX_PI + Particle[ nCntParticle ].fAngle ) * Particle[ nCntParticle ].fLength;		// ����̒��_X
		Particle[ nCntParticle ].fLeftUp.y    = Particle[ nCntParticle ].fPos.y + sin( Particle[ nCntParticle ].fRot + D3DX_PI + Particle[ nCntParticle ].fAngle ) * Particle[ nCntParticle ].fLength;		// ����̒��_Y		
		Particle[ nCntParticle ].fRightUp.x   = Particle[ nCntParticle ].fPos.x + cos( Particle[ nCntParticle ].fRot		   - Particle[ nCntParticle ].fAngle ) * Particle[ nCntParticle ].fLength;		// �E��̒��_X
		Particle[ nCntParticle ].fRightUp.y   = Particle[ nCntParticle ].fPos.y + sin( Particle[ nCntParticle ].fRot		   - Particle[ nCntParticle ].fAngle ) * Particle[ nCntParticle ].fLength;		// �E��̒��_Y
		Particle[ nCntParticle ].fLeftDown.x  = Particle[ nCntParticle ].fPos.x + cos( Particle[ nCntParticle ].fRot + D3DX_PI - Particle[ nCntParticle ].fAngle ) * Particle[ nCntParticle ].fLength;		// �����̒��_X
		Particle[ nCntParticle ].fLeftDown.y  = Particle[ nCntParticle ].fPos.y + sin( Particle[ nCntParticle ].fRot + D3DX_PI - Particle[ nCntParticle ].fAngle ) * Particle[ nCntParticle ].fLength;		// �����̒��_Y
		Particle[ nCntParticle ].fRightDown.x = Particle[ nCntParticle ].fPos.x + cos( Particle[ nCntParticle ].fRot		   + Particle[ nCntParticle ].fAngle ) * Particle[ nCntParticle ].fLength;		// �E���̒��_X
		Particle[ nCntParticle ].fRightDown.y = Particle[ nCntParticle ].fPos.y + sin( Particle[ nCntParticle ].fRot		   + Particle[ nCntParticle ].fAngle ) * Particle[ nCntParticle ].fLength;		// �E���̒��_Y
		
		// ���_���W�̐ݒ�
		pVtx[ 0 ].pos.x = Particle[ nCntParticle ].fLeftUp.x;		// ����̒��_X
		pVtx[ 0 ].pos.y = Particle[ nCntParticle ].fLeftUp.y;		// ����̒��_Y		
		pVtx[ 1 ].pos.x = Particle[ nCntParticle ].fRightUp.x;		// �E��̒��_X
		pVtx[ 1 ].pos.y = Particle[ nCntParticle ].fRightUp.y;		// �E��̒��_Y
		pVtx[ 2 ].pos.x = Particle[ nCntParticle ].fLeftDown.x;		// �����̒��_X
		pVtx[ 2 ].pos.y = Particle[ nCntParticle ].fLeftDown.y;		// �����̒��_Y
		pVtx[ 3 ].pos.x = Particle[ nCntParticle ].fRightDown.x;	// �E���̒��_X
		pVtx[ 3 ].pos.y = Particle[ nCntParticle ].fRightDown.y;	// �E���̒��_Y
		
		// �e�N�X�`�����W�̐ݒ�( 0.0f�`1.0f )
		pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );		// ����̍��W
		pVtx[ 1 ].tex = D3DXVECTOR2( 0.125f, 0.0f );	// �E��̍��W
		pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );		// �����̍��W
		pVtx[ 3 ].tex = D3DXVECTOR2( 0.125f, 1.0f );	// �����̍��W
		
		// ���_�F�̐ݒ�
		pVtx[ 0 ].color = Particle[ nCntParticle ].fColor;
		pVtx[ 1 ].color = Particle[ nCntParticle ].fColor;
		pVtx[ 2 ].color = Particle[ nCntParticle ].fColor;
		pVtx[ 3 ].color = Particle[ nCntParticle ].fColor;

		// ���̃|���S���̒��_��
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBufferParticle->Unlock();
}

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
void DrawParticle( void )
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// ���Z�����ɐ؂�ւ�
	//pDevice->SetRenderState( D3DRS_DESTBLEND,		// �ݒ薼(�C���f�b�N�X)	D3DRS_DESTBLEND�F�w�i�̃A���t�@�ݒ�
	//						   D3DBLEND_ONE );		// �ݒ�l

	// �X�g���[��(�p�C�v���C��)�ݒ�
	pDevice->SetStreamSource( 0,						// �q����p�C�v�̔ԍ�
							  g_pVtxBufferParticle,		// �X�g���[���̌��ƂȂ钸�_�o�b�t�@�̎w��
							  0,						// �I�t�Z�b�g(�o�C�g)
							  sizeof( VERTEX_2D ) );		// �X�g���C�h�l(�X�g���C�h��)

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF( FVF_VERTEX_2D );

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0, g_pTextureParticle );
	
	for( int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++ )
	{
		if( !Particle[ nCntParticle ].bUse ) { continue; }
				
		// �v���~�e�B�u�̕`��
		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
								nCntParticle * 4,			// �I�t�Z�b�g(���_��)
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
void CreateParticle( D3DXVECTOR2 pos, D3DXVECTOR2 size, float speed, float angleOfFire, D3DXCOLOR color, D3DXCOLOR colorValue,  int life )
{
	for( int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++ )
	{
		// �G�t�F�N�g���g���Ă��Ȃ�������
		if( !Particle[ nCntParticle ].bUse )
		{
			Particle[ nCntParticle ].bUse			= true;
			Particle[ nCntParticle ].fPos			= pos;
			Particle[ nCntParticle ].fLength		= sqrt( ( size.y * size.y ) + ( size.x * size.x  ) );
			Particle[ nCntParticle ].fAngle			= atan2( size.y, size.x );
			Particle[ nCntParticle ].fVelocity.x	= speed * cos( angleOfFire );
			Particle[ nCntParticle ].fVelocity.y	= speed * sin( angleOfFire );
			Particle[ nCntParticle ].fColor			= color;
			Particle[ nCntParticle ].fColorValue	= colorValue;
			Particle[ nCntParticle ].nLife			= life;

			break;	// for���𔲂���
		}
	}
}

//******************************* EOF **************************************