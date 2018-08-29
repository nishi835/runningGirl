/*==============================================================================

    Title処理 [title.cpp]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  　ヘッダファイル
------------------------------------------------------------------------------*/
#include "title.h"
#include "main.h"
#include "polygon.h"
#include "input.h"
#include "fade.h"

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define TITLE_TEXTURENAME "data/TEXTURE/runninggirltitle.png"  // テクスチャファイル名
#define TITLE_POS_X ( 0 )							// Titleの表示位置X
#define TITLE_POS_Y ( 0 )							// Titleの表示位置Y
#define TITLE_WIDTH ( 800 )							// Titleの幅
#define TITLE_HEIGHT ( 600 )						// Titleの高さ
#define TITLE_TEXTUER_U ( 0.0 )						// TitleのテクスチャU値
#define TITLE_TEXTUER_V ( 0.0 )						// TitleのテクスチャV値
#define TITLE_TEXTURE_WIDTH ( 1.0f )				// Titleのテクスチャ幅
#define TITLE_TEXTURE_HEIGHT ( 1.0f )				// Titleのテクスチャ高さ

#define TITLE_MESSAGE_TEXTURENAME "data/TEXTURE/pressenter.png" // テクスチャファイル名
#define TITLE_MESSAGE_POS_X ( 300 )								// TitleMessageの表示位置X
#define TITLE_MESSAGE_POS_Y ( 500 )								// TitleMessageの表示位置Y
#define TITLE_MESSAGE_WIDTH ( 200 )								// TitleMessageの幅
#define TITLE_MESSAGE_HEIGHT ( 50 )								// TitleMessageの高さ
#define TITLE_MESSAGE_TEXTUER_U ( 0.0 )							// TitleMessageのテクスチャU値
#define TITLE_MESSAGE_TEXTUER_V ( 0.0 )							// TitleMessageのテクスチャV値
#define TITLE_MESSAGE_TEXTURE_WIDTH ( 1.0f )					// TitleMessageのテクスチャ幅
#define TITLE_MESSAGE_TEXTURE_HEIGHT ( 1.0f )					// TitleMessageのテクスチャ高さ


#define TITLE_RUNNING_TEXTURENAME "data/TEXTURE/running.png"    // テクスチャファイル名
#define TITLE_RUNNING_POS_X ( SCREEN_WIDTH )					// TitleMessageの表示位置X
#define TITLE_RUNNING_POS_Y ( 170 )								// TitleMessageの表示位置Y
#define TITLE_RUNNING_WIDTH ( 600 )								// TitleMessageの幅
#define TITLE_RUNNING_HEIGHT ( 100 )							// TitleMessageの高さ
#define TITLE_RUNNING_TEXTUER_U ( 0.0 )							// TitleMessageのテクスチャU値
#define TITLE_RUNNING_TEXTUER_V ( 0.0 )							// TitleMessageのテクスチャV値
#define TITLE_RUNNING_TEXTURE_WIDTH ( 1.0f )					// TitleMessageのテクスチャ幅
#define TITLE_RUNNING_TEXTURE_HEIGHT ( 1.0f )					// TitleMessageのテクスチャ高さ

#define TITLE_GIRL_TEXTURENAME "data/TEXTURE/girl.png"			// テクスチャファイル名
#define TITLE_GIRL_POS_X ( -500 )						// TitleMessageの表示位置X
#define TITLE_GIRL_POS_Y ( 300 )								// TitleMessageの表示位置Y
#define TITLE_GIRL_WIDTH ( 600 )								// TitleMessageの幅
#define TITLE_GIRL_HEIGHT ( 100 )								// TitleMessageの高さ

#define TITLE_CURSOR_TEXTURENAME "data/TEXTURE/sneaker03.png"			// テクスチャファイル名
#define TITLE_CURSOR_POS_Y (200)
#define TITLE_TIME (300)						    // タイトルの表示時間

#define MENU_MAX (3)

/*------------------------------------------------------------------------------
  　構造体
------------------------------------------------------------------------------*/
typedef struct{
	D3DXVECTOR2 pos;
	float move;
	bool dodraw;
}TITLE;

typedef struct{
	D3DXVECTOR2 pos;
	D3DXVECTOR2 size;
	int poscur;
}CURSOR;

/*------------------------------------------------------------------------------
  　グローバル変数
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureTitle = NULL;					// テクスチャインターフェースのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTitleMessage = NULL;			// テクスチャインターフェースのポインタ
LPDIRECT3DTEXTURE9 g_pTextureRunning = NULL;				// テクスチャインターフェースのポインタ
LPDIRECT3DTEXTURE9 g_pTextureGirl = NULL;					// テクスチャインターフェースのポインタ
LPDIRECT3DTEXTURE9 g_pTextureCursor = NULL;					// テクスチャインターフェースのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTitle = NULL;			// 頂点バッファ管理インターフェースポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTitleMessage = NULL;    // 頂点バッファ管理インターフェースポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferRunning = NULL;			// 頂点バッファ管理インターフェースポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferGirl = NULL;			// 頂点バッファ管理インターフェースポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferCursor = NULL;			// 頂点バッファ管理インターフェースポインタ

TITLE Title[ 2 ];
CURSOR Cursor;

/*------------------------------------------------------------------------------
  　初期化処理
------------------------------------------------------------------------------*/
void InitTitle( void )
{
	// 変数初期化
	Title[ 0 ].pos.x = TITLE_RUNNING_POS_X;
	Title[ 0 ].pos.y = TITLE_RUNNING_POS_Y;
	Title[ 1 ].pos.x = TITLE_GIRL_POS_X;
	Title[ 1 ].pos.y = TITLE_GIRL_POS_Y;
	Title[ 0 ].move  = 0.0f;
	Title[ 2 ].dodraw = false;

	// カーソル初期化
	Cursor.pos.x = 300.0f;
	Cursor.pos.y = TITLE_CURSOR_POS_Y;
	Cursor.size.x = 50.0f;
	Cursor.size.y = 50.0f;
	Cursor.poscur = 0;
	
	// 頂点の設定
	MakeVertexPolygon( &g_pVtxBufferTitle, 1 );
	MakeVertexPolygon( &g_pVtxBufferTitleMessage, 1 );
	MakeVertexPolygon( &g_pVtxBufferRunning, 1 );
	MakeVertexPolygon( &g_pVtxBufferGirl, 1 );
	MakeVertexPolygon( &g_pVtxBufferCursor, 1 );
	
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,						 // デバイスのポインタを渡す
											TITLE_TEXTURENAME,			     // テクスチャのファイル名
											&g_pTextureTitle ) ) )			 // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );		 // エラーチェック
	}
	
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					 	 // デバイスのポインタを渡す
											TITLE_MESSAGE_TEXTURENAME,       // テクスチャのファイル名
											&g_pTextureTitleMessage ) ) )    // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );		 // エラーチェック
	}

	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					 	 // デバイスのポインタを渡す
											TITLE_RUNNING_TEXTURENAME,       // テクスチャのファイル名
											&g_pTextureRunning ) ) )    // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );		 // エラーチェック
	}
	
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					 	 // デバイスのポインタを渡す
											TITLE_GIRL_TEXTURENAME,       // テクスチャのファイル名
											&g_pTextureGirl ) ) )    // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );		 // エラーチェック
	}
	
	
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					 	 // デバイスのポインタを渡す
											TITLE_CURSOR_TEXTURENAME,       // テクスチャのファイル名
											&g_pTextureCursor ) ) )    // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );		 // エラーチェック
	}

	VERTEX_2D* pVtx = NULL; // VRAMの仮想アドレスを格納
	
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferTitle->Lock( 0,
							 0,					// すべてをロック
						     ( void** )&pVtx,   // 仮想アドレス用ポインタのアドレスを渡す
							 0 );			

	// 頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( TITLE_POS_X,  TITLE_POS_Y, 0.0f );									// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( TITLE_POS_X + TITLE_WIDTH,  TITLE_POS_Y, 0.0f );					// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( TITLE_POS_X,  TITLE_POS_Y + TITLE_HEIGHT, 0.0f );					// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( TITLE_POS_X + TITLE_WIDTH,  TITLE_POS_Y + TITLE_HEIGHT, 0.0f );	// 右下の頂点
	
	// UV座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( TITLE_TEXTUER_U,  TITLE_TEXTUER_V );												// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( TITLE_TEXTUER_U + TITLE_TEXTURE_WIDTH, TITLE_TEXTUER_V );							// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( TITLE_TEXTUER_U,  TITLE_TEXTUER_V + TITLE_TEXTURE_HEIGHT );						// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( TITLE_TEXTUER_U + TITLE_TEXTURE_WIDTH, TITLE_TEXTUER_V + TITLE_TEXTURE_HEIGHT );	// 右下の頂点

	// 頂点バッファのアンロック
	g_pVtxBufferTitle->Unlock();	
	
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferTitleMessage->Lock( 0,
							  	    0,				   // すべてをロック
								    ( void** )&pVtx,   // 仮想アドレス用ポインタのアドレスを渡す
									0 );	
	
	pVtx[ 0 ].pos = D3DXVECTOR3( TITLE_MESSAGE_POS_X,  TITLE_MESSAGE_POS_Y, 0.0f );													// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( TITLE_MESSAGE_POS_X + TITLE_MESSAGE_WIDTH,  TITLE_MESSAGE_POS_Y, 0.0f );							// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( TITLE_MESSAGE_POS_X,  TITLE_MESSAGE_POS_Y + TITLE_MESSAGE_HEIGHT, 0.0f );							// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( TITLE_MESSAGE_POS_X + TITLE_MESSAGE_WIDTH,  TITLE_MESSAGE_POS_Y + TITLE_MESSAGE_HEIGHT, 0.0f );	// 右下の頂点

	pVtx[ 0 ].tex = D3DXVECTOR2( TITLE_MESSAGE_TEXTUER_U,  TITLE_MESSAGE_TEXTUER_V );																// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( TITLE_MESSAGE_TEXTUER_U + TITLE_MESSAGE_TEXTURE_WIDTH, TITLE_MESSAGE_TEXTUER_V );									// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( TITLE_MESSAGE_TEXTUER_U,  TITLE_MESSAGE_TEXTUER_V + TITLE_MESSAGE_TEXTURE_HEIGHT );								// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( TITLE_MESSAGE_TEXTUER_U + TITLE_MESSAGE_TEXTURE_WIDTH, TITLE_MESSAGE_TEXTUER_V + TITLE_MESSAGE_TEXTURE_HEIGHT );	// 右下の頂点

	// 頂点バッファのアンロック
	g_pVtxBufferTitleMessage->Unlock();

		// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferRunning->Lock( 0,
							   0,				   // すべてをロック
							   ( void** )&pVtx,   // 仮想アドレス用ポインタのアドレスを渡す
							   0 );	
	
	pVtx[ 0 ].pos = D3DXVECTOR3( TITLE_RUNNING_POS_X,  TITLE_RUNNING_POS_Y, 0.0f );													// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( TITLE_RUNNING_POS_X + TITLE_RUNNING_WIDTH,  TITLE_RUNNING_POS_Y, 0.0f );							// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( TITLE_RUNNING_POS_X,  TITLE_RUNNING_POS_Y + TITLE_RUNNING_HEIGHT, 0.0f );							// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( TITLE_RUNNING_POS_X + TITLE_RUNNING_WIDTH,  TITLE_RUNNING_POS_Y + TITLE_RUNNING_HEIGHT, 0.0f );	// 右下の頂点

	pVtx[ 0 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U,  TITLE_RUNNING_TEXTUER_V );																// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U + TITLE_RUNNING_TEXTURE_WIDTH, TITLE_RUNNING_TEXTUER_V );									// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U,  TITLE_RUNNING_TEXTUER_V +    TITLE_RUNNING_TEXTURE_HEIGHT );								// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U + TITLE_RUNNING_TEXTURE_WIDTH, TITLE_RUNNING_TEXTUER_V + TITLE_RUNNING_TEXTURE_HEIGHT );	// 右下の頂点
		
	// 頂点バッファのアンロック
	g_pVtxBufferRunning->Unlock();
	
		// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferGirl->Lock( 0,
							   0,				   // すべてをロック
							   ( void** )&pVtx,   // 仮想アドレス用ポインタのアドレスを渡す
							   0 );	
	
	pVtx[ 0 ].pos = D3DXVECTOR3( TITLE_GIRL_POS_X,  TITLE_GIRL_POS_Y, 0.0f );											// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( TITLE_GIRL_POS_X + TITLE_GIRL_WIDTH,  TITLE_GIRL_POS_Y, 0.0f );						// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( TITLE_GIRL_POS_X,  TITLE_GIRL_POS_Y + TITLE_GIRL_HEIGHT, 0.0f );						// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( TITLE_GIRL_POS_X + TITLE_GIRL_WIDTH,  TITLE_GIRL_POS_Y + TITLE_GIRL_HEIGHT, 0.0f );	// 右下の頂点

	pVtx[ 0 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U,  TITLE_RUNNING_TEXTUER_V );																// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U + TITLE_RUNNING_TEXTURE_WIDTH, TITLE_RUNNING_TEXTUER_V );									// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U,  TITLE_RUNNING_TEXTUER_V +    TITLE_RUNNING_TEXTURE_HEIGHT );								// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U + TITLE_RUNNING_TEXTURE_WIDTH, TITLE_RUNNING_TEXTUER_V + TITLE_RUNNING_TEXTURE_HEIGHT );	// 右下の頂点
		
	// 頂点バッファのアンロック
	g_pVtxBufferGirl->Unlock();
	
		// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferCursor->Lock( 0,
							   0,				   // すべてをロック
							   ( void** )&pVtx,   // 仮想アドレス用ポインタのアドレスを渡す
							   0 );	
	
	pVtx[ 0 ].pos = D3DXVECTOR3( Cursor.pos.x,					Cursor.pos.y,				  0.0f );	// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( Cursor.pos.x + Cursor.size.x,	Cursor.pos.y,				  0.0f );	// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( Cursor.pos.x,					Cursor.pos.y + Cursor.size.y, 0.0f );	// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( Cursor.pos.x + Cursor.size.x,  Cursor.pos.y + Cursor.size.y, 0.0f );	// 右下の頂点

	pVtx[ 0 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U,  TITLE_RUNNING_TEXTUER_V );																// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U + TITLE_RUNNING_TEXTURE_WIDTH, TITLE_RUNNING_TEXTUER_V );									// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U,  TITLE_RUNNING_TEXTUER_V +    TITLE_RUNNING_TEXTURE_HEIGHT );								// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U + TITLE_RUNNING_TEXTURE_WIDTH, TITLE_RUNNING_TEXTUER_V + TITLE_RUNNING_TEXTURE_HEIGHT );	// 右下の頂点
		
	// 頂点バッファのアンロック
	g_pVtxBufferCursor->Unlock();
}

/*------------------------------------------------------------------------------
  　終了処理
------------------------------------------------------------------------------*/
void UninitTitle( void )
{
	UninitPolygon( &g_pTextureTitle, &g_pVtxBufferTitle );
	UninitPolygon( &g_pTextureTitleMessage, &g_pVtxBufferTitleMessage );
	UninitPolygon( &g_pTextureRunning, &g_pVtxBufferRunning );
	UninitPolygon( &g_pTextureGirl, &g_pVtxBufferGirl );
	UninitPolygon( &g_pTextureCursor, &g_pVtxBufferCursor );
}

/*------------------------------------------------------------------------------
  　更新処理
------------------------------------------------------------------------------*/
void UpdateTitle( void )
{
	// タイマーの設定
	static int nTimer = 0;

	// キーボードの更新
	UpdateKeyboard();

	// タイトルの移動
	Title[ 0 ].move += 0.3f;
	if( Title[ 0 ].pos.x > 150.0f )
	{
		Title[ 0 ].pos.x -= Title[ 0 ].move;
		Title[ 1 ].pos.x += Title[ 0 ].move;
	}
	else if( nTimer % 30 == 0 )
	{		
		Title[ 2 ].dodraw =! Title[ 2 ].dodraw;
	}

	VERTEX_2D* pVtx = NULL; // VRAMの仮想アドレスを格納

	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferRunning->Lock( 0,
							   0,				  // すべてをロック
							   ( void** )&pVtx,   // 仮想アドレス用ポインタのアドレスを渡す
							   0 );	
	
	pVtx[ 0 ].pos = D3DXVECTOR3( Title[ 0 ].pos.x,  Title[ 0 ].pos.y, 0.0f );												// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( Title[ 0 ].pos.x + TITLE_RUNNING_WIDTH,  Title[ 0 ].pos.y, 0.0f );							// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( Title[ 0 ].pos.x,  Title[ 0 ].pos.y + TITLE_RUNNING_HEIGHT, 0.0f );						// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( Title[ 0 ].pos.x + TITLE_RUNNING_WIDTH,  Title[ 0 ].pos.y + TITLE_RUNNING_HEIGHT, 0.0f );	// 右下の頂点
		
	// 頂点バッファのアンロック
	g_pVtxBufferRunning->Unlock();
	
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferGirl->Lock( 0,
							0,				  // すべてをロック
							( void** )&pVtx,   // 仮想アドレス用ポインタのアドレスを渡す
							0 );	
	
	pVtx[ 0 ].pos = D3DXVECTOR3( Title[ 1 ].pos.x,  Title[ 1 ].pos.y, 0.0f );											// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( Title[ 1 ].pos.x + TITLE_GIRL_WIDTH,  Title[ 1 ].pos.y, 0.0f );						// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( Title[ 1 ].pos.x,  Title[ 1 ].pos.y + TITLE_GIRL_HEIGHT, 0.0f );						// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( Title[ 1 ].pos.x + TITLE_GIRL_WIDTH,  Title[ 1 ].pos.y + TITLE_GIRL_HEIGHT, 0.0f );	// 右下の頂点
		
	// 頂点バッファのアンロック
	g_pVtxBufferGirl->Unlock();
		
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferCursor->Lock( 0,
							  0,				   // すべてをロック
							  ( void** )&pVtx,   // 仮想アドレス用ポインタのアドレスを渡す
							  0 );	
	
	pVtx[ 0 ].pos = D3DXVECTOR3( Cursor.pos.x,					Cursor.pos.y,				  0.0f );	// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( Cursor.pos.x + Cursor.size.x,	Cursor.pos.y,				  0.0f );	// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( Cursor.pos.x,					Cursor.pos.y + Cursor.size.y, 0.0f );	// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( Cursor.pos.x + Cursor.size.x,  Cursor.pos.y + Cursor.size.y, 0.0f );	// 右下の頂点

	pVtx[ 0 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U,  TITLE_RUNNING_TEXTUER_V );																// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U + TITLE_RUNNING_TEXTURE_WIDTH, TITLE_RUNNING_TEXTUER_V );									// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U,  TITLE_RUNNING_TEXTUER_V +    TITLE_RUNNING_TEXTURE_HEIGHT );								// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( TITLE_RUNNING_TEXTUER_U + TITLE_RUNNING_TEXTURE_WIDTH, TITLE_RUNNING_TEXTUER_V + TITLE_RUNNING_TEXTURE_HEIGHT );	// 右下の頂点
		
	// 頂点バッファのアンロック
	g_pVtxBufferCursor->Unlock();

	// モード切替
   	if( GetKeyboardTrigger( DIK_SPACE ) )
	{
		switch( Cursor.poscur )
		{
		case 0:
			SetFade( FADE_OUT, MODE_GAME );
			break;
		case 1:
			SetFade( FADE_OUT, MODE_RANKING );
			break;
		case 2:
			SetFade( FADE_OUT, MODE_TUTORIAL );
			break;
		}
	}

	// カーソル操作
	if( GetKeyboardTrigger( DIK_S ) )
	{
		Cursor.poscur++;
		Cursor.pos.y += 75.0f;

		if( Cursor.poscur >= MENU_MAX )
		{
			Cursor.poscur = 0;
			Cursor.pos.y = TITLE_CURSOR_POS_Y;
		}

		nTimer = 0;
	}
	
	if( GetKeyboardTrigger( DIK_W ) )
	{
		Cursor.poscur--;
		Cursor.pos.y -= 75.0f;

		if( Cursor.poscur < 0 )
		{
			Cursor.poscur = MENU_MAX - 1;
			Cursor.pos.y = ( MENU_MAX - 1 ) * 75.0f + TITLE_CURSOR_POS_Y;
		}

		nTimer = 0;
	}

	if( nTimer > TITLE_TIME && GetFade() == FADE_NONE )
	{ 
		SetMode( MODE_DEMO );
		nTimer = 0;
	}

	// タイマーのインクリメント
	nTimer++;
}

/*------------------------------------------------------------------------------
  　描画処理
------------------------------------------------------------------------------*/
void DrawTitle( void )
{
	DrawPolygon( g_pTextureTitle, g_pVtxBufferTitle, 1 );
	if( Title[ 2 ].dodraw )
	{
		DrawPolygon( g_pTextureTitleMessage, g_pVtxBufferTitleMessage, 1 );
	}
	DrawPolygon( g_pTextureRunning, g_pVtxBufferRunning, 1 );
	DrawPolygon( g_pTextureGirl, g_pVtxBufferGirl, 1 );
	DrawPolygon( g_pTextureCursor, g_pVtxBufferCursor, 1 );
}