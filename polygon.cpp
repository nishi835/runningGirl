/*==============================================================================

    �|���S������ [polygon.cpp]
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

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
  �@�I������
------------------------------------------------------------------------------*/
void UninitPolygon( LPDIRECT3DTEXTURE9* pTexture, LPDIRECT3DVERTEXBUFFER9* pVtxBuffer )
{
	// �e�N�X�`���C���^�[�t�F�[�X�̏I������
	SAFE_RELEASE( *pTexture );
	*pTexture = NULL;

	// ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�̏I������
	SAFE_RELEASE( *pVtxBuffer );
	*pVtxBuffer = NULL;
}

/*------------------------------------------------------------------------------
  �@�X�V����
------------------------------------------------------------------------------*/
void UpdatePolygon( LPDIRECT3DVERTEXBUFFER9 pVtxBuffer, float fPosX, float fPosY, float fWidth, float fHeight, float fTexX, float fTexY, float fTexWidth, float fTexHeight )
{
	VERTEX_2D* pVtx = NULL; // VRAM�̉��z�A�h���X���i�[
	
	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	pVtxBuffer->Lock( 0,
					  0,				// ���ׂĂ����b�N
					  ( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
					  0 );			
	
	// ���_���W�̐ݒ�( 2D���W�͉E���ɒ��_��p�ӂ��� )
	pVtx[ 0 ].pos = D3DXVECTOR3( fPosX, fPosY, 0.0f );						// ����̒��_
	pVtx[ 1 ].pos = D3DXVECTOR3( fPosX + fWidth, fPosY, 0.0f );				// �E��̒��_
	pVtx[ 2 ].pos = D3DXVECTOR3( fPosX, fPosY + fHeight, 0.0f );			// �����̒��_
	pVtx[ 3 ].pos = D3DXVECTOR3( fPosX + fWidth, fPosY + fHeight, 0.0f );	// �E���̒��_

	// UV���W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( fTexX, fTexY );							// ����̒��_
	pVtx[ 1 ].tex = D3DXVECTOR2( fTexX + fTexWidth, fTexY );				// �E��̒��_
	pVtx[ 2 ].tex = D3DXVECTOR2( fTexX, fTexY + fTexHeight );				// �����̒��_	
	pVtx[ 3 ].tex = D3DXVECTOR2( fTexX + fTexWidth, fTexY + fTexHeight );	// �E���̒��_

	// ���_�o�b�t�@�̃A�����b�N
	pVtxBuffer->Unlock();
}

/*------------------------------------------------------------------------------
  �@�`�揈��
------------------------------------------------------------------------------*/
void DrawPolygon( LPDIRECT3DTEXTURE9 pTexture, LPDIRECT3DVERTEXBUFFER9 pVtxBuffer, int nNumPolygon )
{
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �p�C�v���C���i�X�g���[���j��ݒ�
	pDevice->SetStreamSource( 0,						// �p�C�v���C���ԍ�
							  pVtxBuffer,				// �p�C�v���C���̌��ɂȂ�̈���w��
							  0,						// �I�t�Z�b�g�i�P�ʂ�byte�j���X�g���[���̊J�n�ʒu���w��ł���
							  sizeof( VERTEX_2D ) );	// �����f�[�^�̒P�ʃT�C�Y�i�X�g���C�h�ʁj

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0, pTexture );

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF( FVF_VERTEX_2D );

	// �v���~�e�B�u�iPolygon�j�̕`��
	for( int nCnt = 0; nCnt < nNumPolygon; nCnt++ )
	{
		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, // �v���~�e�B�u�̎��
								nCnt * 4,			 // �`��J�n�ʒu�̃I�t�Z�b�g�i���_���j
								NUM_POLYGON );		 // �`�悷��v���~�e�B�u�̐�
	}
}

/*------------------------------------------------------------------------------
  �@���_�̍쐬
------------------------------------------------------------------------------*/
HRESULT MakeVertexPolygon( LPDIRECT3DVERTEXBUFFER9* pVtxBuffer, int nNumPolygon )
{
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// *** ���_�o�b�t�@�̍쐬 ***
	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_2D ) * NUM_VERTEX * nNumPolygon,  // �쐬���������_�o�b�t�@�̍��v�f�[�^�T�C�Y
											 D3DUSAGE_WRITEONLY,						 	  // ���_�o�b�t�@�̎g�p���@�i�������݂̂݁j
											 FVF_VERTEX_2D,								      // �H
											 D3DPOOL_MANAGED,								  // ���_�o�b�t�@�̒u����ƂȂ郁�������w��i�f�o�C�X�Ɉϑ��j
											 pVtxBuffer,									  // ���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�̃|�C���^�i�C���^�[�t�F�[�X�̊i�[�j
											 NULL ) ) )										  // �g��Ȃ�
											 {
												 return E_FAIL;
											 }											
	
	// *** ���_�o�b�t�@�̏����� ***										

	VERTEX_2D* pVtx; // VRAM�̉��z�A�h���X���i�[

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	if( FAILED ( (*pVtxBuffer)->Lock( 0,
									  0,				// ���ׂĂ����b�N
								      ( void** )&pVtx,  // ���z�A�h���X�p�|�C���^�̃A�h���X��n��
									  0 ) ) )			// ���b�N�t���O�H
								      {	
										  return E_FAIL;
									  }					

	for( int nCnt = 0; nCnt < nNumPolygon; nCnt++ )
	{
		// ���_���W�̐ݒ�( 2D���W�E�E��� )
		pVtx[ 0 ].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );	// ����̍��W
		pVtx[ 1 ].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );	// �E��̍��W
		pVtx[ 2 ].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );	// �����̍��W
		pVtx[ 3 ].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );	// �����̍��W

		// rhw�̐ݒ�i�K��1.0f�j����O�����i�o�b�t�@�j�̏��H
		pVtx[ 0 ].rhw = 1.0f;	// ����̒��_
		pVtx[ 1 ].rhw = 1.0f;	// �E��̒��_
		pVtx[ 2 ].rhw = 1.0f;	// �����̒��_
		pVtx[ 3 ].rhw = 1.0f;	// �E���̒��_

		// ���_�J���[�̐ݒ�i 0 �` 255�̐����l )
		pVtx[ 0 ].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );	// ����̒��_
		pVtx[ 1 ].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );	// �E��̒��_
		pVtx[ 2 ].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );	// �����̒��_
		pVtx[ 3 ].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );	// �E���̒��_
	
		// �e�N�X�`�����W�̐ݒ�( 0.0f�`1.0f )
		pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );	// ����̍��W
		pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );	// �E��̍��W
		pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );	// �����̍��W
		pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );	// �����̍��W

		pVtx += 4;
	}

	// ���_�o�b�t�@�̃��b�N������
	( *pVtxBuffer )->Unlock();

	return S_OK;
	}