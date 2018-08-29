/*==============================================================================

    �����|���S�� [main.cpp]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/5/31
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  �@�w�b�_�t�@�C��
------------------------------------------------------------------------------*/
#include "main.h"
#include "fade.h"
#include "title.h"
#include "game.h"
#include "tutorial.h"
#include "demo.h"
#include "result.h"
#include "input.h"
#include "sound.h"
#include "player.h"
#include "ranking.h"
#include <Windows.h>
#include <stdio.h>

/*------------------------------------------------------------------------------
  �@�}�N����`
------------------------------------------------------------------------------*/
#define CLASS_NAME	"hoge"			// �E�B���h�E�N���X��
#define WINDOW_NAME	"�^�C�g����"	// �E�B���h�E�^�C�g��
#define FIRST_MODE ( MODE_TITLE )   // �������[�h

/*------------------------------------------------------------------------------
  �@�֐��|�C���^
------------------------------------------------------------------------------*/
typedef void( *ModeFunc )( void );

// �������p�֐��|�C���^
ModeFunc pInitFunc [] =
{
	InitTitle,
	InitDemo,
	InitGame,
	InitResult,
	InitTutorial,
	InitRanking
};

// �I�������p�֐��|�C���^
ModeFunc pUninitFunc [] =
{
	UninitTitle,
	UninitDemo,
	UninitGame,
	UninitResult,
	UninitTutorial,
	UninitRanking
};

// �X�V�����p�֐��|�C���^
ModeFunc pUpdateFunc [] =
{
	UpdateTitle,
	UpdateDemo,
	UpdateGame,
	UpdateResult,
	UpdateTutorial,
	UpdateRanking
};

// �`�揈���p�֐��|�C���^
ModeFunc pDrawFunc [] =
{
	DrawTitle,
	DrawDemo,
	DrawGame,
	DrawResult,
	DrawTutorial,
	DrawRanking
};

/*------------------------------------------------------------------------------
  �@�O���[�o���ϐ�
------------------------------------------------------------------------------*/
LPDIRECT3D9 g_pD3D = NULL;				// Direct3D�C���^�[�t�F�[�X ( DX�̃o�[�W�������擾���� )�@��LP�`�̓|�C���^�^�I
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;  // Direct3D�f�o�C�X�ւ̃|�C���^ ( �O���t�B�b�N�{�[�h�ɃA�N�Z�X���邽�߂̃|�C���^ ) ���ŏd�v
LPD3DXFONT g_pFont = NULL;				// �t�H���g�̃C���^�[�t�F�[�X���i�[
MODE g_mode = FIRST_MODE;				// �Q�[�����[�h�Ǘ��p�ϐ�
MODE g_nextmode = FIRST_MODE;			// �Q�[�����[�h�\��p�ϐ�

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
// �E�B���h�E�Y�v���O�����̏���
LRESULT CALLBACK WndProc( HWND hWnd,	 // �E�B���h�E�̔ԍ�
						  UINT uMsg,		 // �E�B���h�E���b�Z�[�W
						  WPARAM wParam,   // �E�B���h�E�̒ǉ����b�Z�[�W
						  LPARAM lParam ); // �E�B���h�E�̒ǉ����b�Z�[�W

// DirectX�̏���
HRESULT Init( HINSTANCE hInstance,  // �C���X�^���X�i���̃v���O�����̃�������̎��́j�n���h��
			  HWND hWnd,		    // ���C���E�B���h�E�n���h��
			  BOOL bWindow );		// �t���X�N���[�����ǂ���
void Uninit( void );				// �I������
void Update( void );				// �X�V����
void Draw( void );					// �`�揈��

/*------------------------------------------------------------------------------
  �@���C���֐�
------------------------------------------------------------------------------*/
int APIENTRY WinMain( HINSTANCE hInstance,		// �C���X�^���X�n���h��
					  HINSTANCE hPrevInstance,  // �g��Ȃ�
					  LPSTR lpCmdLine,			// �R�}���h���C������̕�������i�[
					  int nCmdShow )			// �E�B���h�E�̕\���̎d��
{
	// ���[�j���O���
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	WNDCLASSEX wcex; // �E�B���h�E�N���X�\���́i�E�B���h�E�̐��`�j

	wcex.cbSize			= sizeof( WNDCLASSEX );			  // �\���̂̃T�C�Y
	wcex.style			= CS_CLASSDC;					  // �E�B���h�E�N���X�̃X�^�C����ݒ�i�����N���X�̃E�B���h�E�ɁA�����f�o�C�X�R���e�L�X�g�����蓖�āj
	wcex.lpfnWndProc	= WndProc;						  // �E�B���h�E�v���V�[�W���̊֐��|�C���^�i�A�h���X�j���w��
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;					  // WinMain�̃C���X�^���X�n���h�����w��
	wcex.hIcon			= NULL;							  // �E�B���h�E�A�C�R���̎w��
	wcex.hCursor		= LoadCursor( NULL, IDC_ARROW );  // �}�E�X�J�[�\���̎w��
	wcex.hbrBackground  = ( HBRUSH )( COLOR_WINDOW + 1 ); // �E�B���h�E�̃N���C�A���g�̈�̔w�i�F�̎w��
	wcex.lpszMenuName   = NULL;							  // �E�B���h�E���j���[�̎w��
	wcex.lpszClassName  = CLASS_NAME;					  // ���̃E�B���h�E�N���X�̖��O
	wcex.hIconSm	    = NULL;							  // �~�j�A�C�R���̎w��

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx( &wcex );

	HWND hWnd; // �E�B���h�E�n���h���i�|�C���^�j
	MSG msg;   // ���b�Z�[�W�\���̂̒�`

	// �^�C�}�[�p�ϐ�
	DWORD dwFrameCount = 0;	   //
	DWORD dwCurrentTime = 0;   // ���ݎ���
	DWORD dwExecLastTime = 0;  // ���s�I�����̎���
	DWORD dwFPSLastTime = 0;   // 

	// �E�B���h�E���쐬
	RECT cr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; // �N���C�A���g��`�̈�̍��W�w��
	DWORD style = WS_OVERLAPPEDWINDOW;				 // �E�B���h�E�X�^�C���̎w��
	AdjustWindowRect( &cr,							 // �N���C�A���g��`�̈�ɘg�����������E�B���h�E���W���i�[�����
					  // �E�B���h�E�X�^�C��
					  style ^ ( WS_MAXIMIZEBOX |	 // �ő剻�{�^���̎g�p�t���O
								WS_THICKFRAME ),	 // �E�B���h�E�T�C�Y�ύX�\�g�̎g�p�t���O						 
					  FALSE );						 // ���j���[�������ǂ���

	int ww = cr.right - cr.left;					 // �E�B���h�E�̕�
	int wh = cr.bottom - cr.top;					 // �E�B���h�E�̍���
	
	// �f�X�N�g�b�v�̋�`���擾
	RECT dr;
	GetWindowRect( GetDesktopWindow(),  // �f�X�N�g�b�v�E�B���h�E�̃n���h��
				   &dr );				// �f�X�N�g�b�v�E�B���h�E�̋�`���i�[�����
	int dw = dr.right - dr.left;		// �f�X�N�g�b�v�E�B���h�E�̕�
	int dh = dr.bottom - dr.top;		// �f�X�N�g�b�v�E�B���h�E�̍���

	// �E�B���h�E�̏����ʒu����ʒ����ɐݒ�
	int wx = ww > dw ? 0 : ( dw / 2 ) - ( ww / 2 );
	int wy = wh > dh ? 0 : ( dh / 2 ) - ( wh / 2 );

	// ���C���E�B���h�E�̍쐬
	hWnd = CreateWindowEx( 0,						  // �g���E�B���h�E�X�^�C��
			 		       CLASS_NAME,				  // �E�B���h�E�N���X�̖��O
					   	   WINDOW_NAME,				  // �E�B���h�E�̖��O
						   // �E�B���h�E�̃X�^�C���i�^�C�v�j���d�v
						   style ^ ( WS_MAXIMIZEBOX | // �ő剻�{�^���̎g�p�t���O
									 WS_THICKFRAME ), // �E�B���h�E�T�C�Y�ύX�\�g�̎g�p�t���O
						   wx,						  // �E�B���h�E�̏���X���W
						   wy,						  // �E�B���h�E�̏���Y���W
						   ww,						  // �E�B���h�E�̕��i�E�B���h�E�̃t���[�����܂ށj
						   wh,						  // �E�B���h�E�̍����i�E�B���h�E�̃t���[�����܂ށj
						   NULL,					  // �e�E�B���h�E�̃n���h��
						   NULL,					  // ���j���[�o�[�̃n���h�����͎q�E�B���h�EID
						   hInstance,				  // �C���X�^���X
						   NULL );					  // �E�B���h�E�쐬�f�[�^
	
	//�E�B���h�E�̕\����Ԃ̐ݒ�
	ShowWindow( hWnd,		// �E�B���h�E�n���h��
				nCmdShow ); // �E�B���h�E�̕\���̎d��

	// �E�B���h�E�̃N���C�A���g�̈�̍X�V
	UpdateWindow( hWnd );
	
	// DirectX�̏�����
	Init( hInstance,
		  hWnd,
		  TRUE );		// �E�B���h�E���[�h�Ȃ�TRUE�A�t���X�N���[�����[�h�Ȃ�FALSE

	// �|���S�������̏�����
	SetMode( FIRST_MODE );
	
	// ����\�̐ݒ�
	timeBeginPeriod( 1 ); // �g���I������猳�ɖ߂�

	// �e�J�E���^�[�̏�����
	dwCurrentTime = 0; 
	dwFrameCount = 0;
	dwExecLastTime = dwFPSLastTime = timeGetTime(); // �V�X�e���������~���b�P�ʂŎ擾
	
	//���b�Z�[�W���[�v
	for( ;; )
	{
		if( PeekMessage( &msg,		 // ���b�Z�[�W���i�[
						 NULL,		 // ���b�Z�[�W�擾���̃n���h���iNULL�͏����X���b�h�̃E�B���h�E���b�Z�[�W���擾�j
						 0,			 // ���b�Z�[�W�̃t�B���^�����O�i�ŏ��l�j
						 0,			 // ���b�Z�[�W�̃t�B���^�����O�i�ő�l�j
						 PM_REMOVE ) // �L���[���烁�b�Z�[�W���폜���邩�ǂ���
						 != 0) 
									 // ��GetMessage�ƈقȂ�APeekMessage�iPeek�́u�`���v�j�̓��b�Z�[�W�̃|�X�g��҂��Ȃ�
									 // �߂�l�̓��b�Z�[�W�擾����0�ȊO�A��擾����0���Ԃ�
		{
			// Windows�̏���
			if( msg.message == WM_QUIT )
			{
				break;
			}
			else
			{
				//���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage( &msg ); // ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage( &msg );  // �E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗏o
			}
		}
		else
		{
			// �b��60�t���[����ۂ����i60�t���[����葬���ꍇ����)
			dwCurrentTime = timeGetTime(); // ���݂̎��Ԃ��擾
			if( ( dwCurrentTime - dwExecLastTime ) >= ( 1000 / 60 ) )
			{
				dwExecLastTime = dwCurrentTime; // �����������Ԃ��i�[

				// DirectX�̏���
				Update(); // �X�V����
				Draw(); // �`�揈��
				dwFrameCount++;
			}
		}
	}

	// DirectX�̏I������
	Uninit(); 

	//�E�B���h�E�N���X�̓o�^�����i�E�B���h�E�̏I�������j���{���͎����I�ɂ���Ă����
	UnregisterClass( CLASS_NAME, wcex.hInstance );
	
	return ( int )msg.wParam;
}

/*------------------------------------------------------------------------------
�@  �E�B���h�E�v���V�[�W��
    (CALLBACK : Win32API�֐����Ăяo���Ƃ��̋K��
    hWnd :�E�B���h�E�̃n���h��
    uMsg :���b�Z�[�W�̎��ʎq
    wParam : ���b�Z�[�W�̍ŏ��̃p�����[�^(�⑫���) ���|�C���^�^�����A�p�r�͉��ł��悢
�@  lParam : ���b�Z�[�W�̂Q�Ԗڂ̃p�����[�^(�⑫���) ���|�C���^�^�����A�p�r�͉��ł��悢
------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	const RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	switch( uMsg )
	{
	// �E�B���h�E���폜���ꂽ�Ƃ�
	case WM_DESTROY:
		PostQuitMessage( 0 ); // WM_QUIT���b�Z�[�W�𑗐M
		break;
	
	// �L�[�������ꂽ�Ƃ�
	case WM_KEYDOWN:
		switch( wParam )
		{
		// �I������
		case VK_ESCAPE:
			if( MessageBox( NULL, "�I�����܂���?",  "�I��", MB_OKCANCEL ) == IDOK )
			{
				DestroyWindow( hWnd ); // �E�B���h�E�̍폜
			}
		}
		break;

	// �E�B���h�E�����ꂽ�Ƃ�
	case WM_CREATE:
		break;

	// �{�^���������ꂽ�Ƃ�
	case WM_COMMAND:
		break;

	// �`�惁�b�Z�[�W�̎�M
	case WM_PAINT:		
		break;

	// �^�C�}�[����
	case WM_TIMER:
		break;

	// ���鑀�삪���ꂽ�Ƃ�
	case WM_CLOSE:		
		if( MessageBox( NULL, "�I�����܂���?",  "�I��", MB_OKCANCEL ) == IDOK )
		{
			DestroyWindow( hWnd );
		}
		else
		{
			return 0;
		}

		break;

	default:
		break;
	}

	// ����̃��b�Z�[�W�̏���
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

/*------------------------------------------------------------------------------
  �@������
------------------------------------------------------------------------------*/
HRESULT Init( HINSTANCE hInstance, HWND hWnd, BOOL bWindow ) 
{
	D3DPRESENT_PARAMETERS d3dpp; // �f�o�C�X����邽�߂̐ݒ�̊i�[
	D3DDISPLAYMODE d3ddm;		 // �f�B�X�v���C���[�h�̊i�[

	// Direct3D�C���^�[�t�F�C�X�̎擾
	g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if( g_pD3D == NULL )
	{
		return E_FAIL; // ��HRESULT�^�̖߂�l�̓G���[�̓��e������
	}

	// �f�B�X�v���C���[�h�̎擾
	if( FAILED( g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, // �f�B�X�v���C�A�_�v�^�̑I���i�}���`���j�^���j
											   &d3ddm ) ) )		   // �f�B�X�v���C���[�h�̊i�[ 
	// ��FAILED�}�N���ł�����ƁAHRESULT�^��BOOL�^�ɃL���X�g�����
	{
		return E_FAIL;
	}

	// --- �f�o�C�X�̃v���[���g�p�����[�^�̐ݒ�i �f�o�C�X�ݒ�p�p�����[�^�̐ݒ� ) ---
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );  // ��������Ԃ�0�Ŗ��߂�( d3dpp�̃f�t�H���g�ݒ� )

	// �o�b�N�o�b�t�@�̐ݒ�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;	// �o�b�N�o�b�t�@�̕�
	d3dpp.BackBufferHeight = SCREEN_HEIGHT; // �o�b�N�o�b�t�@�̍���
	d3dpp.BackBufferFormat = d3ddm.Format;  // �o�b�N�o�b�t�@�̃T�[�t�F�X�i2D�`����j�t�H�[�}�b�g
	d3dpp.BackBufferCount = 1;				// �o�b�N�o�b�t�@�̐�

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �X���b�v�G�t�F�N�g(��ʍX�V�̕��@�j�̐ݒ�
	d3dpp.EnableAutoDepthStencil = TRUE;						// �[�x/�X�e���V���o�b�t�@�̗L��
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �X�e���V���o�b�t�@���g���Ƃ��ɕύX
	d3dpp.Windowed = bWindow;									// �t���X�N���[���ɂ���Ƃ���FALSE�ɂ���
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // �t���X�N���[���ɂ���Ƃ��ɓK�X���l������
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;   // 

	// Direct3D�f�o�C�X�̍쐬
	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT,					// �O���t�B�b�N�{�[�h�̑I�� ���f�B�X�v���C�A�_�v�^�̎w��
									  D3DDEVTYPE_HAL,						// �n�[�h�E�F�A���`��
									  hWnd,									// �E�B���h�E�w��
									  D3DCREATE_HARDWARE_VERTEXPROCESSING,  // �`��A���_�����̓n�[�h�E�F�A�ɔC����
									  &d3dpp,
									  &g_pD3DDevice ) ) )					// �f�o�C�X���i�[
	{
		if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT,					// �O���t�B�b�N�{�[�h�̑I��
										  D3DDEVTYPE_HAL,						// �n�[�h�E�F�A���`��
										  hWnd,									// �E�B���h�E�w��
										  D3DCREATE_SOFTWARE_VERTEXPROCESSING,  // �`��̓n�[�h�E�F�A�A���_������CPU���s��
										  &d3dpp,
										  &g_pD3DDevice ) ) )					// �f�o�C�X���i�[
		{
			if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT,					// �O���t�B�b�N�{�[�h�̑I��
											  D3DDEVTYPE_REF,						// �\�t�g�E�F�A( CPU )���`��
											  hWnd,									// �E�B���h�E�w��
											  D3DCREATE_SOFTWARE_VERTEXPROCESSING,  // �`��A���_������CPU���s��
											  &d3dpp,
											  &g_pD3DDevice ) ) )					// �f�o�C�X���i�[
			{
				return E_FAIL; // �f�o�C�X�擾�s��
			}
		}
	}

	// *** �����_�[�X�e�[�g�i�f�o�C�X�ݒ�j�̐ݒ� ***

	// �J�����O�i�Жʕ`��j�̐ݒ� 
	g_pD3DDevice->SetRenderState( D3DRS_CULLMODE,						  // �J�����O�ݒ�
								  D3DCULL_CCW );						  // �����ŃJ�����O

	// �|���S���̓h��Ԃ��ݒ�
	g_pD3DDevice->SetRenderState( D3DRS_FILLMODE,						  // �h��Ԃ��ݒ�
								  D3DFILL_SOLID );						  // �ʂ�h��Ԃ�

	// �A���t�@�u�����h�i�������j�̐ݒ�
	g_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );		  // �A���t�@�u�����h���L�����ǂ���
	g_pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );	  // �`�挳�i�X�v���C�g�j�̃A���t�@�u�����h�ݒ�
	g_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // �`���i�w�i�j�̃A���t�@�u�����h�ݒ�
	g_pD3DDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 ); // ���_�A���t�@��L����

	// *** �T���v���[�X�e�[�g�̐ݒ� ***
	
	// �e�N�X�`���u�����f�B���O�̐ݒ�
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );   // �����P,�e�N�X�`���̃A���t�@�l
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );   // ���Z�q,��Z
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );   // �����Q,�f�B�t���[�Y�i�|���S���j�̃A���t�@�l

	g_pD3DDevice->SetSamplerState( 0,
								   D3DSAMP_MINFILTER,    // �k����
								   D3DTEXF_LINEAR );     // ���j�A�t�B���^�[���w��
	
	g_pD3DDevice->SetSamplerState( 0,
								   D3DSAMP_MAGFILTER,	 // �g�厞
								   D3DTEXF_LINEAR );	 // ���j�A�t�B���^�[���w��
	
	g_pD3DDevice->SetSamplerState( 0,
								   D3DSAMP_ADDRESSU,	 // u���ix���j���A�h���b�V���O���[�h�ɐݒ�
								   D3DTADDRESS_WRAP );   // ���b�v���[�h
	
	// *** �t�H���g�̍쐬 ***
	D3DXCreateFont( g_pD3DDevice,			// �f�o�C�X
					24,						// �����̍���
					0,						// �����̕��i0������ƁA�����̍����ɂ��킹����������j
					0,						// �����̑����i0���f�t�H���g�j
					0,						// �~�b�v�}�b�v���x����
					FALSE,					// �C�^���b�N���ǂ���
					SHIFTJIS_CHARSET,		// �����Z�b�g
					OUT_DEFAULT_PRECIS,
					DEFAULT_QUALITY, 
					DEFAULT_PITCH,			
					"Terminal",				// �t�H���g��
					&g_pFont );				// �t�H���g�C���^�[�t�F�[�X�̊i�[

	// �e�평����
	InitTitle();
	InitFade();
	InitSound( hWnd );
	InitKeyboard( hInstance, hWnd );
	InitGame();
	InitResult();

	return S_OK;
}

/*------------------------------------------------------------------------------
  �@�I������
------------------------------------------------------------------------------*/
void Uninit( void ) // �I�������́A�������Ƌt�̏��Ԃł��čs��
{
	UninitTitle();
	UninitFade();
	UninitGame();
	UninitResult();
	UninitSound();
	UninitKeyboard();

	SAFE_RELEASE( g_pD3DDevice )
	SAFE_RELEASE( g_pD3D )
}

/*------------------------------------------------------------------------------
  �@�X�V����
------------------------------------------------------------------------------*/
void Update( void )
{
	pUpdateFunc[ g_mode ]();
	UpdateFade();
}

/*------------------------------------------------------------------------------
  �@�`�揈��
------------------------------------------------------------------------------*/
void Draw( void )
{
	// ��ʁi�o�b�N�o�b�t�@�j�̃N���A�@
	g_pD3DDevice->Clear( 0, 
					     NULL,
						 D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // 
					     D3DCOLOR_RGBA( 255, 0, 0, 255 ),
						 1.0f,
						 0 );

	// �`��̊J�n
	if( SUCCEEDED ( g_pD3DDevice->BeginScene() ) ) // �����ŕ`��̃^�X�N��ςݏグ��
	{
		// �`��
		pDrawFunc[ g_mode ]();
		DrawFade();

		// �`��̏I��
		g_pD3DDevice->EndScene();
	}
	g_pD3DDevice->Present( NULL, NULL, NULL, NULL ); // �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ��@
	// �����ߍ��񂾃^�X�N�������ł܂Ƃ߂ď�������

	// ���[�h�̐؂�ւ�
	ChangeMode();
}

/*------------------------------------------------------------------------------
  �@����@�\��
------------------------------------------------------------------------------*/
void DrawControl( void )
{
	char aStr[ 256 ];
	sprintf( aStr, "D:�O�i W:��ړ� S:���ړ� X:�W�����v" );
	RECT rc = { 0, 50, SCREEN_WIDTH, SCREEN_HEIGHT };		// �t�H���g�\���̋�`
	g_pFont->DrawText( NULL,								// �f�o�C�X�R���e�L�X�g
					   aStr,								// �\��������
					   -1,									// �������i-1�͑S���j
					   &rc,									// �͈�
					   DT_LEFT,								// �\�����@�i���l�߁j
					   D3DCOLOR_RGBA( 128, 255, 128, 255 )  // �F
					 ); 

#ifdef _DEBUG
	
	PLAYER* pPlayer = GetPlayer();
	sprintf( aStr, "\nAxis.y:%f\nAxis.x:%f\n", pPlayer->fAxis.y, pPlayer->fAxis.x );
	RECT rc2 = { 0, 70, SCREEN_WIDTH, SCREEN_HEIGHT };		// �t�H���g�\���̋�`
	g_pFont->DrawText( NULL,								// �f�o�C�X�R���e�L�X�g
					   aStr,								// �\��������
					   -1,									// �������i-1�͑S���j
					   &rc2,									// �͈�
					   DT_LEFT,								// �\�����@�i���l�߁j
					   D3DCOLOR_RGBA( 128, 255, 128, 255 )  // �F
					 ); 
	
	// �^�C�}�[�\��
	sprintf( aStr, "\nTimer = %d\n", GetTutorialTimer() );
	RECT rc3 = { 0, 550, SCREEN_WIDTH, SCREEN_HEIGHT };		// �t�H���g�\���̋�`
	g_pFont->DrawText( NULL,								// �f�o�C�X�R���e�L�X�g
					   aStr,								// �\��������
					   -1,									// �������i-1�͑S���j
					   &rc3,									// �͈�
					   DT_LEFT,								// �\�����@�i���l�߁j
					   D3DCOLOR_RGBA( 128, 255, 128, 255 )  // �F
					 ); 
#endif

}

/*------------------------------------------------------------------------------
  �@�Q�b�^�[
------------------------------------------------------------------------------*/
LPDIRECT3DDEVICE9 GetDevice( void )
{
	return g_pD3DDevice;
}

/*------------------------------------------------------------------------------
  �@�Z�b�g���[�h(���[�h�\��j
------------------------------------------------------------------------------*/
void SetMode( MODE mode )
{
	g_nextmode = mode;
}

/*------------------------------------------------------------------------------
  �@���[�h�ύX
------------------------------------------------------------------------------*/
void ChangeMode( void )
{
	if( g_mode != g_nextmode )
	{
		// �I������
		pUninitFunc[ g_mode ](); 

		// ����������
		pInitFunc[ g_nextmode ]();

		g_mode = g_nextmode;
	}
}

/*------------------------------------------------------------------------------
  �@�Q�b�g���[�h
------------------------------------------------------------------------------*/
MODE GetMode( void )
{
	return g_mode;
}


































