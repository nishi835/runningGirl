/*==============================================================================

    複数ポリゴン [main.cpp]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  　ヘッダファイル
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
  　マクロ定義
------------------------------------------------------------------------------*/
#define CLASS_NAME	"hoge"			// ウィンドウクラス名
#define WINDOW_NAME	"タイトル名"	// ウィンドウタイトル
#define FIRST_MODE ( MODE_TITLE )   // 初期モード

/*------------------------------------------------------------------------------
  　関数ポインタ
------------------------------------------------------------------------------*/
typedef void( *ModeFunc )( void );

// 初期化用関数ポインタ
ModeFunc pInitFunc [] =
{
	InitTitle,
	InitDemo,
	InitGame,
	InitResult,
	InitTutorial,
	InitRanking
};

// 終了処理用関数ポインタ
ModeFunc pUninitFunc [] =
{
	UninitTitle,
	UninitDemo,
	UninitGame,
	UninitResult,
	UninitTutorial,
	UninitRanking
};

// 更新処理用関数ポインタ
ModeFunc pUpdateFunc [] =
{
	UpdateTitle,
	UpdateDemo,
	UpdateGame,
	UpdateResult,
	UpdateTutorial,
	UpdateRanking
};

// 描画処理用関数ポインタ
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
  　グローバル変数
------------------------------------------------------------------------------*/
LPDIRECT3D9 g_pD3D = NULL;				// Direct3Dインターフェース ( DXのバージョンを取得する )　※LP～はポインタ型！
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;  // Direct3Dデバイスへのポインタ ( グラフィックボードにアクセスするためのポインタ ) ※最重要
LPD3DXFONT g_pFont = NULL;				// フォントのインターフェースを格納
MODE g_mode = FIRST_MODE;				// ゲームモード管理用変数
MODE g_nextmode = FIRST_MODE;			// ゲームモード予約用変数

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
// ウィンドウズプログラムの処理
LRESULT CALLBACK WndProc( HWND hWnd,	 // ウィンドウの番号
						  UINT uMsg,		 // ウィンドウメッセージ
						  WPARAM wParam,   // ウィンドウの追加メッセージ
						  LPARAM lParam ); // ウィンドウの追加メッセージ

// DirectXの処理
HRESULT Init( HINSTANCE hInstance,  // インスタンス（このプログラムのメモリ上の実体）ハンドル
			  HWND hWnd,		    // メインウィンドウハンドル
			  BOOL bWindow );		// フルスクリーンかどうか
void Uninit( void );				// 終了処理
void Update( void );				// 更新処理
void Draw( void );					// 描画処理

/*------------------------------------------------------------------------------
  　メイン関数
------------------------------------------------------------------------------*/
int APIENTRY WinMain( HINSTANCE hInstance,		// インスタンスハンドル
					  HINSTANCE hPrevInstance,  // 使わない
					  LPSTR lpCmdLine,			// コマンドラインからの文字列を格納
					  int nCmdShow )			// ウィンドウの表示の仕方
{
	// ワーニング回避
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	WNDCLASSEX wcex; // ウィンドウクラス構造体（ウィンドウの雛形）

	wcex.cbSize			= sizeof( WNDCLASSEX );			  // 構造体のサイズ
	wcex.style			= CS_CLASSDC;					  // ウィンドウクラスのスタイルを設定（同じクラスのウィンドウに、同じデバイスコンテキストを割り当て）
	wcex.lpfnWndProc	= WndProc;						  // ウィンドウプロシージャの関数ポインタ（アドレス）を指定
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;					  // WinMainのインスタンスハンドルを指定
	wcex.hIcon			= NULL;							  // ウィンドウアイコンの指定
	wcex.hCursor		= LoadCursor( NULL, IDC_ARROW );  // マウスカーソルの指定
	wcex.hbrBackground  = ( HBRUSH )( COLOR_WINDOW + 1 ); // ウィンドウのクライアント領域の背景色の指定
	wcex.lpszMenuName   = NULL;							  // ウィンドウメニューの指定
	wcex.lpszClassName  = CLASS_NAME;					  // このウィンドウクラスの名前
	wcex.hIconSm	    = NULL;							  // ミニアイコンの指定

	// ウィンドウクラスの登録
	RegisterClassEx( &wcex );

	HWND hWnd; // ウィンドウハンドル（ポインタ）
	MSG msg;   // メッセージ構造体の定義

	// タイマー用変数
	DWORD dwFrameCount = 0;	   //
	DWORD dwCurrentTime = 0;   // 現在時刻
	DWORD dwExecLastTime = 0;  // 実行終了時の時間
	DWORD dwFPSLastTime = 0;   // 

	// ウィンドウを作成
	RECT cr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; // クライアント矩形領域の座標指定
	DWORD style = WS_OVERLAPPEDWINDOW;				 // ウィンドウスタイルの指定
	AdjustWindowRect( &cr,							 // クライアント矩形領域に枠を加味したウィンドウ座標が格納される
					  // ウィンドウスタイル
					  style ^ ( WS_MAXIMIZEBOX |	 // 最大化ボタンの使用フラグ
								WS_THICKFRAME ),	 // ウィンドウサイズ変更可能枠の使用フラグ						 
					  FALSE );						 // メニューを持つかどうか

	int ww = cr.right - cr.left;					 // ウィンドウの幅
	int wh = cr.bottom - cr.top;					 // ウィンドウの高さ
	
	// デスクトップの矩形を取得
	RECT dr;
	GetWindowRect( GetDesktopWindow(),  // デスクトップウィンドウのハンドル
				   &dr );				// デスクトップウィンドウの矩形が格納される
	int dw = dr.right - dr.left;		// デスクトップウィンドウの幅
	int dh = dr.bottom - dr.top;		// デスクトップウィンドウの高さ

	// ウィンドウの初期位置を画面中央に設定
	int wx = ww > dw ? 0 : ( dw / 2 ) - ( ww / 2 );
	int wy = wh > dh ? 0 : ( dh / 2 ) - ( wh / 2 );

	// メインウィンドウの作成
	hWnd = CreateWindowEx( 0,						  // 拡張ウィンドウスタイル
			 		       CLASS_NAME,				  // ウィンドウクラスの名前
					   	   WINDOW_NAME,				  // ウィンドウの名前
						   // ウィンドウのスタイル（タイプ）※重要
						   style ^ ( WS_MAXIMIZEBOX | // 最大化ボタンの使用フラグ
									 WS_THICKFRAME ), // ウィンドウサイズ変更可能枠の使用フラグ
						   wx,						  // ウィンドウの初期X座標
						   wy,						  // ウィンドウの初期Y座標
						   ww,						  // ウィンドウの幅（ウィンドウのフレームも含む）
						   wh,						  // ウィンドウの高さ（ウィンドウのフレームも含む）
						   NULL,					  // 親ウィンドウのハンドル
						   NULL,					  // メニューバーのハンドル又は子ウィンドウID
						   hInstance,				  // インスタンス
						   NULL );					  // ウィンドウ作成データ
	
	//ウィンドウの表示状態の設定
	ShowWindow( hWnd,		// ウィンドウハンドル
				nCmdShow ); // ウィンドウの表示の仕方

	// ウィンドウのクライアント領域の更新
	UpdateWindow( hWnd );
	
	// DirectXの初期化
	Init( hInstance,
		  hWnd,
		  TRUE );		// ウィンドウモードならTRUE、フルスクリーンモードならFALSE

	// ポリゴン処理の初期化
	SetMode( FIRST_MODE );
	
	// 分解能の設定
	timeBeginPeriod( 1 ); // 使い終わったら元に戻す

	// 各カウンターの初期化
	dwCurrentTime = 0; 
	dwFrameCount = 0;
	dwExecLastTime = dwFPSLastTime = timeGetTime(); // システム時刻をミリ秒単位で取得
	
	//メッセージループ
	for( ;; )
	{
		if( PeekMessage( &msg,		 // メッセージを格納
						 NULL,		 // メッセージ取得元のハンドル（NULLは所属スレッドのウィンドウメッセージを取得）
						 0,			 // メッセージのフィルタリング（最小値）
						 0,			 // メッセージのフィルタリング（最大値）
						 PM_REMOVE ) // キューからメッセージを削除するかどうか
						 != 0) 
									 // ※GetMessageと異なり、PeekMessage（Peekは「覗く」）はメッセージのポストを待たない
									 // 戻り値はメッセージ取得時は0以外、非取得時は0が返る
		{
			// Windowsの処理
			if( msg.message == WM_QUIT )
			{
				break;
			}
			else
			{
				//メッセージの翻訳と送出
				TranslateMessage( &msg ); // 仮想キーメッセージを文字メッセージへ変換
				DispatchMessage( &msg );  // ウィンドウプロシージャにメッセージを送出
			}
		}
		else
		{
			// 秒間60フレームを保つ処理（60フレームより速い場合だけ)
			dwCurrentTime = timeGetTime(); // 現在の時間を取得
			if( ( dwCurrentTime - dwExecLastTime ) >= ( 1000 / 60 ) )
			{
				dwExecLastTime = dwCurrentTime; // 処理した時間を格納

				// DirectXの処理
				Update(); // 更新処理
				Draw(); // 描画処理
				dwFrameCount++;
			}
		}
	}

	// DirectXの終了処理
	Uninit(); 

	//ウィンドウクラスの登録解除（ウィンドウの終了処理）※本来は自動的にやってくれる
	UnregisterClass( CLASS_NAME, wcex.hInstance );
	
	return ( int )msg.wParam;
}

/*------------------------------------------------------------------------------
　  ウィンドウプロシージャ
    (CALLBACK : Win32API関数を呼び出すときの規約
    hWnd :ウィンドウのハンドル
    uMsg :メッセージの識別子
    wParam : メッセージの最初のパラメータ(補足情報) ※ポインタ型だが、用途は何でもよい
　  lParam : メッセージの２番目のパラメータ(補足情報) ※ポインタ型だが、用途は何でもよい
------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	const RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	switch( uMsg )
	{
	// ウィンドウが削除されたとき
	case WM_DESTROY:
		PostQuitMessage( 0 ); // WM_QUITメッセージを送信
		break;
	
	// キーが押されたとき
	case WM_KEYDOWN:
		switch( wParam )
		{
		// 終了処理
		case VK_ESCAPE:
			if( MessageBox( NULL, "終了しますか?",  "終了", MB_OKCANCEL ) == IDOK )
			{
				DestroyWindow( hWnd ); // ウィンドウの削除
			}
		}
		break;

	// ウィンドウが作られたとき
	case WM_CREATE:
		break;

	// ボタンが押されたとき
	case WM_COMMAND:
		break;

	// 描画メッセージの受信
	case WM_PAINT:		
		break;

	// タイマー処理
	case WM_TIMER:
		break;

	// 閉じる操作がされたとき
	case WM_CLOSE:		
		if( MessageBox( NULL, "終了しますか?",  "終了", MB_OKCANCEL ) == IDOK )
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

	// 既定のメッセージの処理
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

/*------------------------------------------------------------------------------
  　初期化
------------------------------------------------------------------------------*/
HRESULT Init( HINSTANCE hInstance, HWND hWnd, BOOL bWindow ) 
{
	D3DPRESENT_PARAMETERS d3dpp; // デバイスを作るための設定の格納
	D3DDISPLAYMODE d3ddm;		 // ディスプレイモードの格納

	// Direct3Dインターフェイスの取得
	g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if( g_pD3D == NULL )
	{
		return E_FAIL; // ※HRESULT型の戻り値はエラーの内容を示す
	}

	// ディスプレイモードの取得
	if( FAILED( g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, // ディスプレイアダプタの選択（マルチモニタ時）
											   &d3ddm ) ) )		   // ディスプレイモードの格納 
	// ※FAILEDマクロでくくると、HRESULT型がBOOL型にキャストされる
	{
		return E_FAIL;
	}

	// --- デバイスのプレゼントパラメータの設定（ デバイス設定用パラメータの設定 ) ---
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );  // メモリ空間を0で埋める( d3dppのデフォルト設定 )

	// バックバッファの設定
	d3dpp.BackBufferWidth = SCREEN_WIDTH;	// バックバッファの幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT; // バックバッファの高さ
	d3dpp.BackBufferFormat = d3ddm.Format;  // バックバッファのサーフェス（2D描画情報）フォーマット
	d3dpp.BackBufferCount = 1;				// バックバッファの数

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// スワップエフェクト(画面更新の方法）の設定
	d3dpp.EnableAutoDepthStencil = TRUE;						// 深度/ステンシルバッファの有無
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// ステンシルバッファを使うときに変更
	d3dpp.Windowed = bWindow;									// フルスクリーンにするときはFALSEにする
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // フルスクリーンにするときに適宜数値を入れる
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;   // 

	// Direct3Dデバイスの作成
	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT,					// グラフィックボードの選択 ※ディスプレイアダプタの指定
									  D3DDEVTYPE_HAL,						// ハードウェアが描画
									  hWnd,									// ウィンドウ指定
									  D3DCREATE_HARDWARE_VERTEXPROCESSING,  // 描画、頂点処理はハードウェアに任せる
									  &d3dpp,
									  &g_pD3DDevice ) ) )					// デバイスを格納
	{
		if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT,					// グラフィックボードの選択
										  D3DDEVTYPE_HAL,						// ハードウェアが描画
										  hWnd,									// ウィンドウ指定
										  D3DCREATE_SOFTWARE_VERTEXPROCESSING,  // 描画はハードウェア、頂点処理はCPUが行う
										  &d3dpp,
										  &g_pD3DDevice ) ) )					// デバイスを格納
		{
			if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT,					// グラフィックボードの選択
											  D3DDEVTYPE_REF,						// ソフトウェア( CPU )が描画
											  hWnd,									// ウィンドウ指定
											  D3DCREATE_SOFTWARE_VERTEXPROCESSING,  // 描画、頂点処理はCPUが行う
											  &d3dpp,
											  &g_pD3DDevice ) ) )					// デバイスを格納
			{
				return E_FAIL; // デバイス取得不可
			}
		}
	}

	// *** レンダーステート（デバイス設定）の設定 ***

	// カリング（片面描画）の設定 
	g_pD3DDevice->SetRenderState( D3DRS_CULLMODE,						  // カリング設定
								  D3DCULL_CCW );						  // 左回りでカリング

	// ポリゴンの塗りつぶし設定
	g_pD3DDevice->SetRenderState( D3DRS_FILLMODE,						  // 塗りつぶし設定
								  D3DFILL_SOLID );						  // 面を塗りつぶす

	// アルファブレンド（透明化）の設定
	g_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );		  // アルファブレンドが有効かどうか
	g_pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );	  // 描画元（スプライト）のアルファブレンド設定
	g_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // 描画先（背景）のアルファブレンド設定
	g_pD3DDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 ); // 頂点アルファを有効に

	// *** サンプラーステートの設定 ***
	
	// テクスチャブレンディングの設定
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );   // 引数１,テクスチャのアルファ値
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );   // 演算子,乗算
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );   // 引数２,ディフューズ（ポリゴン）のアルファ値

	g_pD3DDevice->SetSamplerState( 0,
								   D3DSAMP_MINFILTER,    // 縮小時
								   D3DTEXF_LINEAR );     // リニアフィルターを指定
	
	g_pD3DDevice->SetSamplerState( 0,
								   D3DSAMP_MAGFILTER,	 // 拡大時
								   D3DTEXF_LINEAR );	 // リニアフィルターを指定
	
	g_pD3DDevice->SetSamplerState( 0,
								   D3DSAMP_ADDRESSU,	 // u軸（x軸）をアドレッシングモードに設定
								   D3DTADDRESS_WRAP );   // ラップモード
	
	// *** フォントの作成 ***
	D3DXCreateFont( g_pD3DDevice,			// デバイス
					24,						// 文字の高さ
					0,						// 文字の幅（0を入れると、文字の高さにあわせた幅が入る）
					0,						// 文字の太さ（0がデフォルト）
					0,						// ミップマップレベル数
					FALSE,					// イタリックかどうか
					SHIFTJIS_CHARSET,		// 文字セット
					OUT_DEFAULT_PRECIS,
					DEFAULT_QUALITY, 
					DEFAULT_PITCH,			
					"Terminal",				// フォント名
					&g_pFont );				// フォントインターフェースの格納

	// 各種初期化
	InitTitle();
	InitFade();
	InitSound( hWnd );
	InitKeyboard( hInstance, hWnd );
	InitGame();
	InitResult();

	return S_OK;
}

/*------------------------------------------------------------------------------
  　終了処理
------------------------------------------------------------------------------*/
void Uninit( void ) // 終了処理は、初期化と逆の順番でして行く
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
  　更新処理
------------------------------------------------------------------------------*/
void Update( void )
{
	pUpdateFunc[ g_mode ]();
	UpdateFade();
}

/*------------------------------------------------------------------------------
  　描画処理
------------------------------------------------------------------------------*/
void Draw( void )
{
	// 画面（バックバッファ）のクリア　
	g_pD3DDevice->Clear( 0, 
					     NULL,
						 D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // 
					     D3DCOLOR_RGBA( 255, 0, 0, 255 ),
						 1.0f,
						 0 );

	// 描画の開始
	if( SUCCEEDED ( g_pD3DDevice->BeginScene() ) ) // ここで描画のタスクを積み上げる
	{
		// 描画
		pDrawFunc[ g_mode ]();
		DrawFade();

		// 描画の終了
		g_pD3DDevice->EndScene();
	}
	g_pD3DDevice->Present( NULL, NULL, NULL, NULL ); // バックバッファとフロントバッファの入れ替え　
	// ※溜め込んだタスクをここでまとめて処理する

	// モードの切り替え
	ChangeMode();
}

/*------------------------------------------------------------------------------
  　操作法表示
------------------------------------------------------------------------------*/
void DrawControl( void )
{
	char aStr[ 256 ];
	sprintf( aStr, "D:前進 W:上移動 S:下移動 X:ジャンプ" );
	RECT rc = { 0, 50, SCREEN_WIDTH, SCREEN_HEIGHT };		// フォント表示の矩形
	g_pFont->DrawText( NULL,								// デバイスコンテキスト
					   aStr,								// 表示文字列
					   -1,									// 文字数（-1は全部）
					   &rc,									// 範囲
					   DT_LEFT,								// 表示方法（左詰め）
					   D3DCOLOR_RGBA( 128, 255, 128, 255 )  // 色
					 ); 

#ifdef _DEBUG
	
	PLAYER* pPlayer = GetPlayer();
	sprintf( aStr, "\nAxis.y:%f\nAxis.x:%f\n", pPlayer->fAxis.y, pPlayer->fAxis.x );
	RECT rc2 = { 0, 70, SCREEN_WIDTH, SCREEN_HEIGHT };		// フォント表示の矩形
	g_pFont->DrawText( NULL,								// デバイスコンテキスト
					   aStr,								// 表示文字列
					   -1,									// 文字数（-1は全部）
					   &rc2,									// 範囲
					   DT_LEFT,								// 表示方法（左詰め）
					   D3DCOLOR_RGBA( 128, 255, 128, 255 )  // 色
					 ); 
	
	// タイマー表示
	sprintf( aStr, "\nTimer = %d\n", GetTutorialTimer() );
	RECT rc3 = { 0, 550, SCREEN_WIDTH, SCREEN_HEIGHT };		// フォント表示の矩形
	g_pFont->DrawText( NULL,								// デバイスコンテキスト
					   aStr,								// 表示文字列
					   -1,									// 文字数（-1は全部）
					   &rc3,									// 範囲
					   DT_LEFT,								// 表示方法（左詰め）
					   D3DCOLOR_RGBA( 128, 255, 128, 255 )  // 色
					 ); 
#endif

}

/*------------------------------------------------------------------------------
  　ゲッター
------------------------------------------------------------------------------*/
LPDIRECT3DDEVICE9 GetDevice( void )
{
	return g_pD3DDevice;
}

/*------------------------------------------------------------------------------
  　セットモード(モード予約）
------------------------------------------------------------------------------*/
void SetMode( MODE mode )
{
	g_nextmode = mode;
}

/*------------------------------------------------------------------------------
  　モード変更
------------------------------------------------------------------------------*/
void ChangeMode( void )
{
	if( g_mode != g_nextmode )
	{
		// 終了処理
		pUninitFunc[ g_mode ](); 

		// 初期化処理
		pInitFunc[ g_nextmode ]();

		g_mode = g_nextmode;
	}
}

/*------------------------------------------------------------------------------
  　ゲットモード
------------------------------------------------------------------------------*/
MODE GetMode( void )
{
	return g_mode;
}


































