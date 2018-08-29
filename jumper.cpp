/*==============================================================================

    ポリゴン処理 [jumper.cpp]
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
#include "jumper.h"
#include "player.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define JUMPER_TEXTURENAME "data/TEXTURE/jumper_2cut.png"		 // テクスチャファイル名（通常時）
#define JUMPER_POS_X (0)							 		 // ハードルの表示位置X
#define JUMPER_POS_Y (0)									 // ハードルの表示位置Y

#define JUMPER_TEX_WIDTH (1.0f)							 // ハードルのテクスチャ幅
#define JUMPER_TEX_HEIGHT (1.0f)							 // ハードルのテクスチャ高さ
#define JUMPER_MOVE_SPEED (3.2f)							 // ハードルの移動速度
#define JUMPER_SCROLL_COEFFICIENT (800.0f)					 // ハードルの移動を背景に合わせる係数

/*------------------------------------------------------------------------------
  　グローバル変数
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTextureJumper	= NULL;	// テクスチャインターフェースのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferJumper	= NULL; // 頂点バッファ管理インターフェースポインタ

JUMPER Jumper[ JUMPER_MAX ];  // ハードルの構造体

/*------------------------------------------------------------------------------
  　初期化処理
------------------------------------------------------------------------------*/
void InitJumper( void )
{
	srand( ( unsigned )time ( 0 ) );	// 乱数設定

	// パラメータ初期化
	for( int nCntJumper = 0; nCntJumper < JUMPER_MAX; nCntJumper++ )
	{
		Jumper[ nCntJumper ].fPos.x	 = SCREEN_WIDTH * 1.5f;
		Jumper[ nCntJumper ].fPos.y	 = ( rand() % ( int )( LIMIT_BOTTOM - LIMIT_TOP ) ) + LIMIT_TOP - JUMPER_HEIGHT / 2.0f;
		Jumper[ nCntJumper ].fPosTex.x = 0.0f;
		Jumper[ nCntJumper ].fPosTex.y = 0.0f;
	}

	// 頂点の設定
	MakeVertexPolygon( &g_pVtxBufferJumper, JUMPER_MAX );
	
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					 // デバイスのポインタを渡す
											JUMPER_TEXTURENAME,			 // テクスチャのファイル名
											&g_pTextureJumper ) ) )		 // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK ); // エラーチェック
	}	
}

/*------------------------------------------------------------------------------
  　終了処理
------------------------------------------------------------------------------*/	
void UninitJumper( void )
{
	// テクスチャインターフェースの開放
	if( g_pTextureJumper != NULL )
	{
		g_pTextureJumper->Release();
		g_pTextureJumper = NULL;
	}
	
	// 頂点バッファインターフェースの開放
	if( g_pVtxBufferJumper != NULL )
	{
		g_pVtxBufferJumper->Release();
		g_pVtxBufferJumper = NULL;
	}
}

/*------------------------------------------------------------------------------
  　更新処理
------------------------------------------------------------------------------*/
void UpdateJumper( void )
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
	g_pVtxBufferJumper->Lock( 0,
							  0,				// すべてをロック
							  ( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
							  0 );	
	
	// 更新
	for( int nCntJumper = 0; nCntJumper < JUMPER_MAX; nCntJumper++ )
	{
		if( !Jumper[ nCntJumper ].bUse ) { continue; }

		// 座標の更新
		Jumper[ nCntJumper ].fPos.x -= ( pMoveBg->fTexScrollSpeed * JUMPER_SCROLL_COEFFICIENT );
	
		//// 画面端まで行ったら
		//if( Jumper[ nCntJumper ].fPos.x + JUMPER_WIDTH < 0 )
		//{
		//	Jumper[ nCntJumper ].fPos.x = SCREEN_WIDTH;
		//	Jumper[ nCntJumper ].fPos.y = ( rand() % ( int )( LIMIT_BOTTOM - LIMIT_TOP ) ) + LIMIT_TOP - JUMPER_HEIGHT / 2.0f;
		//}
	
		// 四隅の座標の算出
		Jumper[ nCntJumper ].fLeftUp.x    = Jumper[ nCntJumper ].fPos.x;
		Jumper[ nCntJumper ].fLeftUp.y    = Jumper[ nCntJumper ].fPos.y;
		Jumper[ nCntJumper ].fRightUp.x   = Jumper[ nCntJumper ].fPos.x + JUMPER_WIDTH;
		Jumper[ nCntJumper ].fRightUp.y   = Jumper[ nCntJumper ].fPos.y;
		Jumper[ nCntJumper ].fLeftDown.x  = Jumper[ nCntJumper ].fPos.x;
		Jumper[ nCntJumper ].fLeftDown.y  = Jumper[ nCntJumper ].fPos.y + JUMPER_HEIGHT;
		Jumper[ nCntJumper ].fRightDown.x = Jumper[ nCntJumper ].fPos.x + JUMPER_WIDTH;
		Jumper[ nCntJumper ].fRightDown.y = Jumper[ nCntJumper ].fPos.y + JUMPER_HEIGHT;
	
		// 頂点座標の設定
		pVtx[ 0 ].pos.x = Jumper[ nCntJumper ].fLeftUp.x;   	// 左上の頂点X
		pVtx[ 0 ].pos.y = Jumper[ nCntJumper ].fLeftUp.y;   	// 左上の頂点Y		
		pVtx[ 1 ].pos.x = Jumper[ nCntJumper ].fRightUp.x;  	// 右上の頂点X
		pVtx[ 1 ].pos.y = Jumper[ nCntJumper ].fRightUp.y;  	// 右上の頂点Y
		pVtx[ 2 ].pos.x = Jumper[ nCntJumper ].fLeftDown.x; 	// 左下の頂点X
		pVtx[ 2 ].pos.y = Jumper[ nCntJumper ].fLeftDown.y; 	// 左下の頂点Y
		pVtx[ 3 ].pos.x = Jumper[ nCntJumper ].fRightDown.x;	// 右下の頂点X
		pVtx[ 3 ].pos.y = Jumper[ nCntJumper ].fRightDown.y;	// 右下の頂点Y

		// UV座標の設定
		PLAYER* pPlayer = GetPlayer();
	
		if( pPlayer->State == STATE_RUN && 
			HitObject( pPlayer->fLeftUp.x, pPlayer->fLeftUp.y, PLAYER_WIDTH, PLAYER_HEIGHT, Jumper[ nCntJumper ].fLeftUp.x + JUMPER_WIDTH * 0.8f, Jumper[ nCntJumper ].fLeftUp.y, JUMPER_WIDTH * 0.3f, JUMPER_HEIGHT ) )
		{
			pVtx[ 0 ].tex = D3DXVECTOR2( Jumper[ nCntJumper ].fPosTex.x + 0.5f, Jumper[ nCntJumper ].fPosTex.y );					    // 左上の頂点
			pVtx[ 1 ].tex = D3DXVECTOR2( Jumper[ nCntJumper ].fPosTex.x + 1.0f, Jumper[ nCntJumper ].fPosTex.y );					    // 右上の頂点
			pVtx[ 2 ].tex = D3DXVECTOR2( Jumper[ nCntJumper ].fPosTex.x + 0.5f, Jumper[ nCntJumper ].fPosTex.y + JUMPER_TEX_HEIGHT );	// 左下の頂点	
			pVtx[ 3 ].tex = D3DXVECTOR2( Jumper[ nCntJumper ].fPosTex.x + 1.0f, Jumper[ nCntJumper ].fPosTex.y + JUMPER_TEX_HEIGHT );	// 右下の頂点
		}
		else
		{
			pVtx[ 0 ].tex = D3DXVECTOR2( Jumper[ nCntJumper ].fPosTex.x       , Jumper[ nCntJumper ].fPosTex.y );					    // 左上の頂点
			pVtx[ 1 ].tex = D3DXVECTOR2( Jumper[ nCntJumper ].fPosTex.x + 0.5f, Jumper[ nCntJumper ].fPosTex.y );					    // 右上の頂点
			pVtx[ 2 ].tex = D3DXVECTOR2( Jumper[ nCntJumper ].fPosTex.x       , Jumper[ nCntJumper ].fPosTex.y + JUMPER_TEX_HEIGHT );	// 左下の頂点	
			pVtx[ 3 ].tex = D3DXVECTOR2( Jumper[ nCntJumper ].fPosTex.x + 0.5f, Jumper[ nCntJumper ].fPosTex.y + JUMPER_TEX_HEIGHT );	// 右下の頂点
		}

		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_pVtxBufferJumper->Unlock();	
}

/*------------------------------------------------------------------------------
  　描画処理
------------------------------------------------------------------------------*/
void DrawJumper( void )
{
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// パイプライン（ストリーム）を設定
	pDevice->SetStreamSource( 0,						// パイプライン番号
							  g_pVtxBufferJumper,		// パイプラインの元になる領域を指定
							  0,						// オフセット（単位はbyte）※ストリームの開始位置を指定できる
							  sizeof( VERTEX_2D ) );	// 流すデータの単位サイズ（ストライド量）

	// テクスチャの設定
	pDevice->SetTexture( 0, g_pTextureJumper );

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_2D );

	// プリミティブ（Polygon）の描画
	for( int nCnt = 0; nCnt < JUMPER_MAX; nCnt++ )
	{
		if( !Jumper[ nCnt ].bUse ) { continue; }

		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, // プリミティブの種類
								nCnt * 4,			 // 描画開始位置のオフセット（頂点数）
								NUM_POLYGON );		 // 描画するプリミティブの数
	}
}

/*------------------------------------------------------------------------------
  　ゲッター
------------------------------------------------------------------------------*/
JUMPER* GetJumper( void )
{
	return Jumper;
}

