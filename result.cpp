/*==============================================================================

    result処理 [result.cpp]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  　ヘッダファイル
------------------------------------------------------------------------------*/
#include "result.h"
#include "main.h"
#include "polygon.h"
#include "input.h"
#include "score.h"
#include "game.h"

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define RESULT_TEXTURENAME "data/TEXTURE/result.png" // テクスチャファイル名
#define RESULTCLEAR_TEXTURENAME "data/TEXTURE/allclearedBG.png" // テクスチャファイル名
#define RESULTSCORE_TEXTURENAME "data/TEXTURE/number05.png" // テクスチャファイル名
#define RESULT_POS_X ( 0 )							 // Resultの表示位置X
#define RESULT_POS_Y ( 0 )						 	 // Resultの表示位置Y
#define RESULT_WIDTH ( 800 )						 // Resultの幅
#define RESULT_HEIGHT ( 600 )						 // Resultの高さ
#define RESULT_TEXTUER_U ( 0.0 )					 // ResultのテクスチャU値
#define RESULT_TEXTUER_V ( 0.0 )					 // ResultのテクスチャV値
#define RESULT_TEXTURE_WIDTH ( 1.0f )				 // Resultのテクスチャ幅
#define RESULT_TEXTURE_HEIGHT ( 1.0f )				 // Resultのテクスチャ高さ

#define RESULTSCORE_POS_X ( 600.0f )									// Scoreの表示位置X
#define RESULTSCORE_POS_Y ( 335.0f )									// Scoreの表示位置Y
#define RESULTSCORE_WIDTH ( 45 )										// Scoreの幅
#define RESULTSCORE_HEIGHT ( 55 )										// Scoreの高さ
#define RESULTSCORE_TEXTURE_WIDTH ( 0.1f )							// Scoreのテクスチャ幅
#define RESULTSCORE_TEXTURE_HEIGHT ( 1.0f )							// Scoreのテクスチャ高さ
#define RESULTSCORE_INTERVAL ( 10 )									// Scoreの間隔
#define RESULTSCORE_MAX_DIGIT ( 4 )									// Scoreの最大桁数

/*------------------------------------------------------------------------------
  　グローバル変数
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureResult = NULL;					// テクスチャインターフェースのポインタ
LPDIRECT3DTEXTURE9 g_pTextureResultScore = NULL;			// テクスチャインターフェースのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferResult = NULL;			// 頂点バッファ管理インターフェースポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferResultScore = NULL;			// 頂点バッファ管理インターフェースポインタ
LPDIRECT3DTEXTURE9 g_pTextureAllClear = NULL;			// テクスチャインターフェースのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferAllClear = NULL;			// 頂点バッファ管理インターフェースポインタ

int g_nResultTimer = 0;

/*------------------------------------------------------------------------------
  　初期化処理
------------------------------------------------------------------------------*/
void InitResult( void )
{
	g_nResultTimer = 0;

	// 頂点の設定
	MakeVertexPolygon( &g_pVtxBufferResult, 1 );
	
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,						 // デバイスのポインタを渡す
											RESULT_TEXTURENAME,			     // テクスチャのファイル名
											&g_pTextureResult ) ) )			 // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );		 // エラーチェック
	}

	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,						 // デバイスのポインタを渡す
											RESULTSCORE_TEXTURENAME,			     // テクスチャのファイル名
											&g_pTextureResultScore ) ) )			 // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );		 // エラーチェック
	}
	
	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,						 // デバイスのポインタを渡す
											RESULTCLEAR_TEXTURENAME,			     // テクスチャのファイル名
											&g_pTextureAllClear ) ) )			 // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );		 // エラーチェック
	}

	VERTEX_2D* pVtx = NULL; // VRAMの仮想アドレスを格納
	
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferResult->Lock( 0,
							 0,					// すべてをロック
						     ( void** )&pVtx,   // 仮想アドレス用ポインタのアドレスを渡す
							 0 );			

	// 頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( RESULT_POS_X,  RESULT_POS_Y, 0.0f );									// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( RESULT_POS_X + RESULT_WIDTH,  RESULT_POS_Y, 0.0f );					// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( RESULT_POS_X,  RESULT_POS_Y + RESULT_HEIGHT, 0.0f );					// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( RESULT_POS_X + RESULT_WIDTH,  RESULT_POS_Y + RESULT_HEIGHT, 0.0f );	// 右下の頂点
	
	// UV座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( RESULT_TEXTUER_U,  RESULT_TEXTUER_V );												// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( RESULT_TEXTUER_U + RESULT_TEXTURE_WIDTH, RESULT_TEXTUER_V );							// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( RESULT_TEXTUER_U,  RESULT_TEXTUER_V + RESULT_TEXTURE_HEIGHT );						// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( RESULT_TEXTUER_U + RESULT_TEXTURE_WIDTH, RESULT_TEXTUER_V + RESULT_TEXTURE_HEIGHT );	// 右下の頂点

	// 頂点バッファのアンロック
	g_pVtxBufferResult->Unlock();	
	
	// 頂点の設定
	MakeVertexPolygon( &g_pVtxBufferResultScore, RESULTSCORE_MAX_DIGIT );
	
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferResultScore->Lock( 0,
								   0,					// すべてをロック
								   ( void** )&pVtx,   // 仮想アドレス用ポインタのアドレスを渡す
								   0 );			


	int value = ( int )GetScore(); // 分解用スコア格納
	int number;				  // スコアの数字

	for( int nCnt = 0; nCnt < RESULTSCORE_MAX_DIGIT; nCnt++ )
	{
		// スコア分解
		number = value % 10;			// 最下位桁の取り出し
		value /= 10;					// 最下位桁の更新
			
		// 頂点座標の設定
		pVtx[ 0 ].pos = D3DXVECTOR3( RESULTSCORE_POS_X - ( RESULTSCORE_WIDTH + RESULTSCORE_INTERVAL ) * nCnt,					  RESULTSCORE_POS_Y,				      0.0f );	// 左上の頂点
		pVtx[ 1 ].pos = D3DXVECTOR3( RESULTSCORE_POS_X - ( RESULTSCORE_WIDTH + RESULTSCORE_INTERVAL ) * nCnt + RESULTSCORE_WIDTH, RESULTSCORE_POS_Y,				      0.0f );	// 右上の頂点
		pVtx[ 2 ].pos = D3DXVECTOR3( RESULTSCORE_POS_X - ( RESULTSCORE_WIDTH + RESULTSCORE_INTERVAL ) * nCnt,					  RESULTSCORE_POS_Y + RESULTSCORE_HEIGHT, 0.0f );	// 左下の頂点	
		pVtx[ 3 ].pos = D3DXVECTOR3( RESULTSCORE_POS_X - ( RESULTSCORE_WIDTH + RESULTSCORE_INTERVAL ) * nCnt + RESULTSCORE_WIDTH, RESULTSCORE_POS_Y + RESULTSCORE_HEIGHT, 0.0f );	// 右下の頂点

		// UV座標の設定
		pVtx[ 0 ].tex = D3DXVECTOR2( number * RESULTSCORE_TEXTURE_WIDTH,							 0.0f );					// 左上の頂点
		pVtx[ 1 ].tex = D3DXVECTOR2( number * RESULTSCORE_TEXTURE_WIDTH + RESULTSCORE_TEXTURE_WIDTH, 0.0f );					// 右上の頂点
		pVtx[ 2 ].tex = D3DXVECTOR2( number * RESULTSCORE_TEXTURE_WIDTH,							 RESULTSCORE_TEXTURE_HEIGHT );	// 左下の頂点	
		pVtx[ 3 ].tex = D3DXVECTOR2( number * RESULTSCORE_TEXTURE_WIDTH + RESULTSCORE_TEXTURE_WIDTH, RESULTSCORE_TEXTURE_HEIGHT );	// 右下の頂点
			
		pVtx += 4;
	}	
	
	// 頂点バッファのアンロック
	g_pVtxBufferResultScore->Unlock();	
}

/*------------------------------------------------------------------------------
  　終了処理
------------------------------------------------------------------------------*/
void UninitResult( void )
{
	UninitPolygon( &g_pTextureResult, &g_pVtxBufferResult );
	UninitPolygon( &g_pTextureResultScore, &g_pVtxBufferResultScore );
	UninitPolygon( &g_pTextureAllClear, &g_pVtxBufferAllClear );
}

/*------------------------------------------------------------------------------
  　更新処理
------------------------------------------------------------------------------*/
void UpdateResult( void )
{
	// キーボードの更新
	UpdateKeyboard();
	
	// モード切替
  	if( GetKeyboardPress( DIK_SPACE ) )
	{
		SetMode( MODE_RANKING );
	}

	g_nResultTimer++;
}

/*------------------------------------------------------------------------------
  　描画処理
------------------------------------------------------------------------------*/
void DrawResult( void )
{
	if( GetStage() >= STAGE_MAX && g_nResultTimer < 540 )
	{		
		DrawPolygon( g_pTextureAllClear, g_pVtxBufferResult, 1 );
	}
	else
	{
		DrawPolygon( g_pTextureResult, g_pVtxBufferResult, 1 );
		DrawPolygon( g_pTextureResultScore, g_pVtxBufferResultScore, RESULTSCORE_MAX_DIGIT );
	}
}