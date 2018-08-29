/*==============================================================================

    ポリゴン処理 [hurdle.cpp]
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
#include "movebg.h"
#include "hurdle.h"
#include "player.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define HURDLE_TEXTURENAME      "data/TEXTURE/hurdle.png"		 // テクスチャファイル名（通常時）
#define HURDLE_TEXTURENAME_FALL "data/TEXTURE/hurdle_fall.png"	 // テクスチャファイル名（倒れ状態）
#define HURDLE_POS_X ( 0 )							 			 // ハードルの表示位置X
#define HURDLE_POS_Y ( 0 )										 // ハードルの表示位置Y
#define HURDLE_TEX_MAX (2)										 // ハードルのテクスチャ画像数

#define HURDLE_FALL_WIDTH ( 50.0f )								 // 倒れハードルの幅
#define HURDLE_FALL_HEIGHT ( 100.0f )							 // 倒れハードルの高さ
#define HURDLE_TEX_WIDTH ( 1.0f )								 // ハードルのテクスチャ幅
#define HURDLE_TEX_HEIGHT ( 1.0f )								 // ハードルのテクスチャ高さ
#define HURDLE_MOVE_SPEED ( 3.2f )								 // ハードルの移動速度
#define HURDLE_SCROLL_COEFFICIENT (800.0f)						 // ハードルの移動を背景に合わせる係数

/*------------------------------------------------------------------------------
  　グローバル変数
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTextureHurdle[ HURDLE_TEX_MAX ]	= {};	// テクスチャインターフェースのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferHurdle					= NULL; // 頂点バッファ管理インターフェースポインタ

HURDLE Hurdle[ HURDLE_MAX ];  // ハードルの構造体

// 回転関係
float		g_fLengthHurdle;  // ポリゴンの対角線の長さ
float	    g_fAngleHurdle;   // ポリゴンの対角線の角度

/*------------------------------------------------------------------------------
  　初期化処理
------------------------------------------------------------------------------*/
void InitHurdle( void )
{
	srand( ( unsigned )time ( 0 ) );	// 乱数設定

	// ハードルの初期化
	for( int nCntHurdle = 0; nCntHurdle < HURDLE_MAX; nCntHurdle++ )
	{
		Hurdle[ nCntHurdle ].fPos.x	 = SCREEN_WIDTH;
		Hurdle[ nCntHurdle ].fPos.y	 = ( rand() % ( int )( LIMIT_BOTTOM - LIMIT_TOP ) ) + LIMIT_TOP - HURDLE_HEIGHT / 2.0f;
		Hurdle[ nCntHurdle ].fPosTex.x = 0.0f;
		Hurdle[ nCntHurdle ].fPosTex.y = 0.0f;
		Hurdle[ nCntHurdle ].bUse     = false;
	}
	
	// 頂点の設定
	MakeVertexPolygon( &g_pVtxBufferHurdle, HURDLE_MAX );
	
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					 // デバイスのポインタを渡す
											HURDLE_TEXTURENAME,			 // テクスチャのファイル名
											&g_pTextureHurdle[ 0 ] ) ) ) // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK ); // エラーチェック
	}
	
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					 // デバイスのポインタを渡す
											HURDLE_TEXTURENAME_FALL,	 // テクスチャのファイル名
											&g_pTextureHurdle[ 1 ] ) ) ) // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK ); // エラーチェック
	}
}

/*------------------------------------------------------------------------------
  　終了処理
------------------------------------------------------------------------------*/	
void UninitHurdle( void )
{
	// テクスチャインターフェースの開放
	for( int nCnt = 0; nCnt < HURDLE_TEX_MAX; nCnt++ )
	{
		if( g_pTextureHurdle[ nCnt ] != NULL )
		{
			g_pTextureHurdle[ nCnt ]->Release();
			g_pTextureHurdle[ nCnt ] = NULL;
		}
	}

	// 頂点バッファインターフェースの開放
	if( g_pVtxBufferHurdle != NULL )
	{
		g_pVtxBufferHurdle->Release();
		g_pVtxBufferHurdle = NULL;
	}
}

/*------------------------------------------------------------------------------
  　更新処理
------------------------------------------------------------------------------*/
void UpdateHurdle( void )
{	
	// スクロール背景の取得
	MOVEBG* pMoveBg = NULL;
	pMoveBg			= GetMoveBG();
	if( pMoveBg == NULL )
	{
		printf( "データがありません\n" );
	}	

	VERTEX_2D* pVtx = NULL; // VRAMの仮想アドレスを格納
	
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferHurdle->Lock( 0,
							  0,				// すべてをロック
							  ( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
							  0 );	

	// 更新
	for( int nCntHurdle = 0; nCntHurdle < HURDLE_MAX; nCntHurdle++ )
	{
		if( !Hurdle[ nCntHurdle ].bUse ) { continue; }

		// 座標の更新
		Hurdle[ nCntHurdle ].fPos.x -= ( pMoveBg->fTexScrollSpeed * HURDLE_SCROLL_COEFFICIENT );
		
		// 四隅の座標の算出
		Hurdle[ nCntHurdle ].fLeftUp.x    = Hurdle[ nCntHurdle ].fPos.x;
		Hurdle[ nCntHurdle ].fLeftUp.y    = Hurdle[ nCntHurdle ].fPos.y;
		Hurdle[ nCntHurdle ].fRightUp.x   = Hurdle[ nCntHurdle ].fPos.x + HURDLE_WIDTH;
		Hurdle[ nCntHurdle ].fRightUp.y   = Hurdle[ nCntHurdle ].fPos.y;
		Hurdle[ nCntHurdle ].fLeftDown.x  = Hurdle[ nCntHurdle ].fPos.x;
		Hurdle[ nCntHurdle ].fLeftDown.y  = Hurdle[ nCntHurdle ].fPos.y + HURDLE_HEIGHT;
		Hurdle[ nCntHurdle ].fRightDown.x = Hurdle[ nCntHurdle ].fPos.x + HURDLE_WIDTH;
		Hurdle[ nCntHurdle ].fRightDown.y = Hurdle[ nCntHurdle ].fPos.y + HURDLE_HEIGHT;
			
		// 頂点座標の設定
		pVtx[ 0 ].pos.x = Hurdle[ nCntHurdle ].fLeftUp.x;   	// 左上の頂点X
		pVtx[ 0 ].pos.y = Hurdle[ nCntHurdle ].fLeftUp.y;   	// 左上の頂点Y		
		pVtx[ 1 ].pos.x = Hurdle[ nCntHurdle ].fRightUp.x;  	// 右上の頂点X
		pVtx[ 1 ].pos.y = Hurdle[ nCntHurdle ].fRightUp.y;  	// 右上の頂点Y
		pVtx[ 2 ].pos.x = Hurdle[ nCntHurdle ].fLeftDown.x; 	// 左下の頂点X
		pVtx[ 2 ].pos.y = Hurdle[ nCntHurdle ].fLeftDown.y; 	// 左下の頂点Y
		pVtx[ 3 ].pos.x = Hurdle[ nCntHurdle ].fRightDown.x;	// 右下の頂点X
		pVtx[ 3 ].pos.y = Hurdle[ nCntHurdle ].fRightDown.y;	// 右下の頂点Y

		// UV座標の設定
		pVtx[ 0 ].tex = D3DXVECTOR2( Hurdle[ nCntHurdle ].fPosTex.x,				    Hurdle[ nCntHurdle ].fPosTex.y );					    // 左上の頂点
		pVtx[ 1 ].tex = D3DXVECTOR2( Hurdle[ nCntHurdle ].fPosTex.x + HURDLE_TEX_WIDTH, Hurdle[ nCntHurdle ].fPosTex.y );					    // 右上の頂点
		pVtx[ 2 ].tex = D3DXVECTOR2( Hurdle[ nCntHurdle ].fPosTex.x,				    Hurdle[ nCntHurdle ].fPosTex.y + HURDLE_TEX_HEIGHT );	// 左下の頂点	
		pVtx[ 3 ].tex = D3DXVECTOR2( Hurdle[ nCntHurdle ].fPosTex.x + HURDLE_TEX_WIDTH, Hurdle[ nCntHurdle ].fPosTex.y + HURDLE_TEX_HEIGHT );	// 右下の頂点

 		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_pVtxBufferHurdle->Unlock();	
}

/*------------------------------------------------------------------------------
  　描画処理
------------------------------------------------------------------------------*/
void DrawHurdle( void )
{
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// パイプライン（ストリーム）を設定
	pDevice->SetStreamSource( 0,						// パイプライン番号
							  g_pVtxBufferHurdle,			// パイプラインの元になる領域を指定
							  0,						// オフセット（単位はbyte）※ストリームの開始位置を指定できる
							  sizeof( VERTEX_2D ) );	// 流すデータの単位サイズ（ストライド量）

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_2D );

	// プリミティブ（Polygon）の描画
	for( int nCnt = 0; nCnt < HURDLE_MAX; nCnt++ )
	{
		if( !Hurdle[ nCnt ].bUse ) { continue; }
		
		// テクスチャの設定
		if( !Hurdle[ nCnt ].bFall )
		{
			pDevice->SetTexture( 0, g_pTextureHurdle[ 0 ] );
		}
		else
		{
			pDevice->SetTexture( 0, g_pTextureHurdle[ 1 ] );
		}

		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, // プリミティブの種類
								nCnt * 4,			 // 描画開始位置のオフセット（頂点数）
								NUM_POLYGON );		 // 描画するプリミティブの数
	}	
}

/*------------------------------------------------------------------------------
  　ゲッター
------------------------------------------------------------------------------*/
HURDLE* GetHurdle( void )
{
	return Hurdle;
}

