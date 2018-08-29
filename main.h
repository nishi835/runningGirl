/*==============================================================================

    main�̃w�b�_�[ [main.h]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/4/17
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/
#ifndef _MAIN_H_
#define _MAIN_H_

/*------------------------------------------------------------------------------
  �@�w�b�_�t�@�C��
------------------------------------------------------------------------------*/
// �w�b�_�̃C���N���[�h
#include <d3dx9.h>
#define DIRECTINPUT_VERSION ( 0x0800 )
#include <dinput.h>							// �����Ԃ�K�����

// ���C�u�����̃����N
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "winmm.lib" )		// �}���`���f�B�A���߂̃��C�u�����i�V�X�e�������擾�ɕK�v�j
#pragma comment( lib, "dinput8.lib" )   // DirectInput8�̃����N

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define SCREEN_WIDTH ( 800 ) //�E�B���h�E�̕�
#define SCREEN_HEIGHT ( 600 ) //�E�B���h�E�̍���

#define NUM_VERTEX ( 4 ) // ���_�̐�
#define NUM_POLYGON ( 2 ) // �|���S���̖���

// ���_�t�H�[�}�b�g
#define FVF_VERTEX_2D ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )  // ( ���W�ϊ��ςݒ��_���W | ���_�J���[ )

// �Z�[�t�����[�X�}�N��
#define SAFE_RELEASE( x ) { if( x ) { ( x )->Release(); ( x ) = NULL; } }

/*------------------------------------------------------------------------------
  �@�񋓑�
------------------------------------------------------------------------------*/
typedef enum
{
	MODE_TITLE = 0,
	MODE_DEMO,
	MODE_GAME, 
	MODE_RESULT, 
	MODE_TUTORIAL,
	MODE_RANKING,
	MODE_MAX
}MODE;

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
LPDIRECT3DDEVICE9 GetDevice( void ); // �f�o�C�X�̃|�C���^�擾
void DrawControl( void );
void SetMode( MODE mode );
MODE GetMode( void );
void ChangeMode( void );

#endif

