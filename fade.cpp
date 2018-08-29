/*==============================================================================

    Fade処理 [fade.cpp]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  　ヘッダファイル
------------------------------------------------------------------------------*/
#include "main.h"
#include "polygon.h"
#include "fade.h"

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define FADE_TEXTURENAME "data/TEXTURE/fade00.png"		 // テクスチャファイル名
#define FADE_POS_X ( 0 )								 // Fadeの表示位置X
#define FADE_POS_Y ( 0 )								 // Fadeの表示位置Y
#define FADE_WIDTH ( 800 )								 // Fadeの幅
#define FADE_HEIGHT ( 600 )								 // Fadeの高さ
#define FADE_TEXTURE_WIDTH ( 1.0f )						 // Fadeの幅
#define FADE_TEXTURE_HEIGHT ( 1.0f )					 // Fadeの高さ
#define FADE_RATE ( 0.02f )								 // Fade係数

/*------------------------------------------------------------------------------
  　構造体の宣言
------------------------------------------------------------------------------*/
// Fadeの構造体
typedef struct
{
	float fPosTexX;		// FadeのテクスチャX座標
	float fPosTexY;		// FadeのテクスチャY座標
}FADEHOGE;

/*------------------------------------------------------------------------------
  　グローバル変数
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;			// テクスチャインターフェースのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferFade = NULL;    // 頂点バッファ管理インターフェースポインタ

FADEHOGE Fade;										// Fadeの構造体
D3DXCOLOR g_colorFade;
FADE g_fade = FADE_IN;
MODE g_modeNext = MODE_TITLE;

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
HRESULT MakeVertexFade( LPDIRECT3DDEVICE9 pDevice );
HRESULT SetColorFade( D3DXCOLOR color );

/*------------------------------------------------------------------------------
  　初期化処理
------------------------------------------------------------------------------*/
void InitFade( void )
{
	// Fadeの初期化
	Fade.fPosTexX = 0.0f;
	Fade.fPosTexY = 0.0f;

	//
	g_colorFade = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );

	// 頂点の設定
	MakeVertexPolygon( &g_pVtxBufferFade, 1 );
	
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,				 // デバイスのポインタを渡す
											FADE_TEXTURENAME,     // テクスチャのファイル名
											&g_pTextureFade ) ) ) // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK ); // エラーチェック
	}
}

/*------------------------------------------------------------------------------
  　終了処理
------------------------------------------------------------------------------*/
void UninitFade( void )
{
	UninitPolygon( &g_pTextureFade, &g_pVtxBufferFade );
}

/*------------------------------------------------------------------------------
  　更新処理
------------------------------------------------------------------------------*/
void UpdateFade( void )
{
	if( g_fade == FADE_NONE )
	{
		return;
	}

	if( g_fade == FADE_IN )
	{
		g_colorFade.a -= FADE_RATE;
		
		if( g_colorFade.a <= 0.0f )
		{
			g_colorFade.a = 0.0f;
			g_fade = FADE_NONE;
		}
	}
	else if( g_fade == FADE_OUT )
	{
		g_colorFade.a += FADE_RATE;
		
		if( g_colorFade.a >= 1.0f )
		{
			g_colorFade.a = 1.0f;
			g_fade = FADE_IN;
			SetMode( g_modeNext );
		}
	}

	SetColorFade( g_colorFade );

	VERTEX_2D* pVtx = NULL; // VRAMの仮想アドレスを格納
	
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferFade->Lock( 0,
						    0,				// すべてをロック
						    ( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
						    0 );			

	// 頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( FADE_POS_X, FADE_POS_Y, 0.0f );							// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( FADE_POS_X + FADE_WIDTH, FADE_POS_Y, 0.0f );				// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( FADE_POS_X,  FADE_POS_Y + FADE_HEIGHT, 0.0f );				// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( FADE_POS_X + FADE_WIDTH,  FADE_POS_Y + FADE_HEIGHT, 0.0f );	// 右下の頂点
	
	// UV座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( Fade.fPosTexX,  Fade.fPosTexY );							// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( Fade.fPosTexX + FADE_TEXTURE_WIDTH,  Fade.fPosTexY );				// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( Fade.fPosTexX,  Fade.fPosTexY		+ FADE_TEXTURE_HEIGHT );				// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( Fade.fPosTexX + FADE_TEXTURE_WIDTH,  Fade.fPosTexY + FADE_TEXTURE_HEIGHT );	// 右下の頂点

	// 頂点バッファのアンロック
	g_pVtxBufferFade->Unlock();	

	// モード切替
	//SetMode( g_modeNext );
}

/*------------------------------------------------------------------------------
  　描画処理
------------------------------------------------------------------------------*/
void DrawFade( void )
{
	DrawPolygon( NULL, g_pVtxBufferFade, 1 );
}

/*------------------------------------------------------------------------------
  　セッター
------------------------------------------------------------------------------*/
void SetFade( FADE fade, MODE modenext )
{
	g_fade = fade;
	g_modeNext = modenext;
}

/*------------------------------------------------------------------------------
  　ゲッター
------------------------------------------------------------------------------*/
FADE GetFade( void )
{
	return g_fade;
}

/*------------------------------------------------------------------------------
  　色設定処理
------------------------------------------------------------------------------*/
HRESULT SetColorFade( D3DXCOLOR color )
{
	VERTEX_2D* pVtx; // VRAMの仮想アドレスを格納

	// 頂点バッファをロックして、仮想アドレスを取得する
	if( FAILED ( g_pVtxBufferFade->Lock( 0,
										 0,				// すべてをロック
										 ( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
										 0 ) ) )			// ロックフラグ？
										 {	
										 	  return E_FAIL;
										 }					
 
	// 頂点カラーの設定（ 0 ～ 1の整数値 )
	pVtx[ 0 ].color = color;	// 左上の頂点
	pVtx[ 1 ].color = color;	// 右上の頂点
	pVtx[ 2 ].color = color;	// 左下の頂点
	pVtx[ 3 ].color = color;	// 右下の頂点

	// 頂点バッファのロックを解除
	( g_pVtxBufferFade )->Unlock();

	return S_OK;
}
