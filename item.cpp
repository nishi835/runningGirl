/*==============================================================================

    ポリゴン処理 [item.cpp]
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
#include "item.h"
#include "player.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define ITEM_TEXTURENAME      "data/TEXTURE/sneaker03.png"		 // テクスチャファイル名（通常時）
#define ITEM_POS_X ( 0 )							 		 // アイテムの表示位置X
#define ITEM_POS_Y ( 0 )									 // アイテムの表示位置Y

#define ITEM_TEX_WIDTH ( 1.0f )							 // アイテムのテクスチャ幅
#define ITEM_TEX_HEIGHT ( 1.0f )							 // アイテムのテクスチャ高さ
#define ITEM_MOVE_SPEED ( 3.2f )							 // アイテムの移動速度
#define ITEM_SCROLL_COEFFICIENT (800.0f)					 // アイテムの移動を背景に合わせる係数

/*------------------------------------------------------------------------------
  　グローバル変数
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTextureItem	= NULL;	// テクスチャインターフェースのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferItem	= NULL; // 頂点バッファ管理インターフェースポインタ

ITEM Item[ ITEM_MAX ];  // アイテムの構造体

/*------------------------------------------------------------------------------
  　初期化処理
------------------------------------------------------------------------------*/
void InitItem( void )
{
	srand( ( unsigned )time ( 0 ) );	// 乱数設定

	// パラメータ初期化
	for( int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++ )
	{
		Item[ nCntItem ].fPos.x	 = SCREEN_WIDTH * 1.5f;
		Item[ nCntItem ].fPos.y	 = ( rand() % ( int )( LIMIT_BOTTOM - LIMIT_TOP ) ) + LIMIT_TOP - ITEM_HEIGHT / 2.0f;
		Item[ nCntItem ].fPosTex.x = 0.0f;
		Item[ nCntItem ].fPosTex.y = 0.0f;
	}
	
	// 頂点の設定
	MakeVertexPolygon( &g_pVtxBufferItem, ITEM_MAX );
		
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					 // デバイスのポインタを渡す
											ITEM_TEXTURENAME,			 // テクスチャのファイル名
											&g_pTextureItem ) ) )		 // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK ); // エラーチェック
	}	
}

/*------------------------------------------------------------------------------
  　終了処理
------------------------------------------------------------------------------*/	
void UninitItem( void )
{
	// テクスチャインターフェースの開放
	if( g_pTextureItem != NULL )
	{
		g_pTextureItem->Release();
		g_pTextureItem = NULL;
	}
	
	// 頂点バッファインターフェースの開放
	if( g_pVtxBufferItem != NULL )
	{
		g_pVtxBufferItem->Release();
		g_pVtxBufferItem = NULL;
	}
}

/*------------------------------------------------------------------------------
  　更新処理
------------------------------------------------------------------------------*/
void UpdateItem( void )
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
	g_pVtxBufferItem->Lock( 0,
							0,				// すべてをロック
							( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
							0 );	

	// 更新
	for( int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++ )
	{
		if( !Item[ nCntItem ].bUse ) { continue; }

		// 座標の更新
		Item[ nCntItem ].fPos.x -= ( pMoveBg->fTexScrollSpeed * ITEM_SCROLL_COEFFICIENT );
		
		// 四隅の座標の算出
		Item[ nCntItem ].fLeftUp.x    = Item[ nCntItem ].fPos.x;
		Item[ nCntItem ].fLeftUp.y    = Item[ nCntItem ].fPos.y;
		Item[ nCntItem ].fRightUp.x   = Item[ nCntItem ].fPos.x + ITEM_WIDTH;
		Item[ nCntItem ].fRightUp.y   = Item[ nCntItem ].fPos.y;
		Item[ nCntItem ].fLeftDown.x  = Item[ nCntItem ].fPos.x;
		Item[ nCntItem ].fLeftDown.y  = Item[ nCntItem ].fPos.y + ITEM_HEIGHT;
		Item[ nCntItem ].fRightDown.x = Item[ nCntItem ].fPos.x + ITEM_WIDTH;
		Item[ nCntItem ].fRightDown.y = Item[ nCntItem ].fPos.y + ITEM_HEIGHT;
		
		// 頂点座標の設定
		pVtx[ 0 ].pos.x = Item[ nCntItem ].fLeftUp.x;   	// 左上の頂点X
		pVtx[ 0 ].pos.y = Item[ nCntItem ].fLeftUp.y;   	// 左上の頂点Y		
		pVtx[ 1 ].pos.x = Item[ nCntItem ].fRightUp.x;  	// 右上の頂点X
		pVtx[ 1 ].pos.y = Item[ nCntItem ].fRightUp.y;  	// 右上の頂点Y
		pVtx[ 2 ].pos.x = Item[ nCntItem ].fLeftDown.x; 	// 左下の頂点X
		pVtx[ 2 ].pos.y = Item[ nCntItem ].fLeftDown.y; 	// 左下の頂点Y
		pVtx[ 3 ].pos.x = Item[ nCntItem ].fRightDown.x;	// 右下の頂点X
		pVtx[ 3 ].pos.y = Item[ nCntItem ].fRightDown.y;	// 右下の頂点Y	

		// UV座標の設定
		pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );					    // 左上の頂点
		pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );					    // 右上の頂点
		pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );	// 左下の頂点		
		pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );	// 左下の頂点	

		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_pVtxBufferItem->Unlock();	
}

/*------------------------------------------------------------------------------
  　描画処理
------------------------------------------------------------------------------*/
void DrawItem( void )
{
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// パイプライン（ストリーム）を設定
	pDevice->SetStreamSource( 0,						// パイプライン番号
							  g_pVtxBufferItem,			// パイプラインの元になる領域を指定
							  0,						// オフセット（単位はbyte）※ストリームの開始位置を指定できる
							  sizeof( VERTEX_2D ) );	// 流すデータの単位サイズ（ストライド量）

	// テクスチャの設定
	pDevice->SetTexture( 0, g_pTextureItem );

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_2D );

	// プリミティブ（Polygon）の描画
	for( int nCnt = 0; nCnt < ITEM_MAX; nCnt++ )
	{
		if( !Item[ nCnt ].bUse ) { continue; }

		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, // プリミティブの種類
								nCnt * 4,			 // 描画開始位置のオフセット（頂点数）
								NUM_POLYGON );		 // 描画するプリミティブの数
	}
}

/*------------------------------------------------------------------------------
  　ゲッター
------------------------------------------------------------------------------*/
ITEM* GetItem( void )
{
	return Item;
}

