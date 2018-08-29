/*==============================================================================

    ポリゴン処理 [goal.cpp]
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
#include "goal.h"
#include "player.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define GOAL_TEXTURENAME      "data/TEXTURE/goalline.png"		 // テクスチャファイル名
#define GOAL_POS_X ( 24000.0f )							 		 // ゴールの表示位置X
#define GOAL_POS_Y ( 150.0f )									 // ゴールの表示位置Y
#define GOAL_TEX_MAX (1)									 // ゴールのテクスチャ画像数

#define GOAL_TEX_WIDTH ( 1.0f )								 // ゴールのテクスチャ幅
#define GOAL_TEX_HEIGHT ( 1.0f )							 // ゴールのテクスチャ高さ
#define GOAL_MOVE_SPEED ( 3.2f )							 // ゴールの移動速度
#define GOAL_SCROLL_COEFFICIENT (800.0f)					 // ゴールの移動を背景に合わせる係数

/*------------------------------------------------------------------------------
  　グローバル変数
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTextureGoal = NULL;	// テクスチャインターフェースのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferGoal				= NULL; // 頂点バッファ管理インターフェースポインタ

GOAL Goal;  // ゴールの構造体

// 回転関係
float		g_fLengthGoal;  // ポリゴンの対角線の長さ
float	    g_fAngleGoal;   // ポリゴンの対角線の角度

/*------------------------------------------------------------------------------
  　初期化処理
------------------------------------------------------------------------------*/
void InitGoal( void )
{
	srand( ( unsigned )time ( 0 ) );	// 乱数設定

	// ゴールの初期化
	Goal.fPos.x	 = GOAL_POS_X;
	Goal.fPos.y	 = GOAL_POS_Y;
	Goal.fPosTex.x = 0.0f;
	Goal.fPosTex.y = 0.0f;
	Goal.bUse      = false;
	
	// 四隅の座標の算出
	Goal.fLeftUp.x    = Goal.fPos.x;
	Goal.fLeftUp.y    = Goal.fPos.y;
	Goal.fRightUp.x   = Goal.fPos.x + GOAL_WIDTH;
	Goal.fRightUp.y   = Goal.fPos.y;
	Goal.fLeftDown.x  = Goal.fPos.x;
	Goal.fLeftDown.y  = Goal.fPos.y + GOAL_HEIGHT;
	Goal.fRightDown.x = Goal.fPos.x + GOAL_WIDTH;
	Goal.fRightDown.y = Goal.fPos.y + GOAL_HEIGHT;

	// *** 回転準備 ***																				// ポリゴンの回転量の設定
	g_fLengthGoal = sqrt( GOAL_WIDTH * GOAL_WIDTH + GOAL_HEIGHT * GOAL_HEIGHT ) / 2.0f;   // 対角線の長さの算出
	g_fAngleGoal  = atan2( GOAL_HEIGHT, GOAL_WIDTH );											// 対角線の角度の算出

	// 頂点の設定
	MakeVertexPolygon( &g_pVtxBufferGoal, 1 );
	
	VERTEX_2D* pVtx = NULL; // VRAMの仮想アドレスを格納
	
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferGoal->Lock( 0,
							0,				// すべてをロック
							( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
							0 );	


	// 頂点バッファ設定
	SetVertexGoal( pVtx );		

	// UV座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );					    // 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );					    // 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );	// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );	// 右下の頂点
	
	// 頂点座標の設定
	pVtx[ 0 ].pos.x = Goal.fLeftUp.x;   	// 左上の頂点X
	pVtx[ 0 ].pos.y = Goal.fLeftUp.y;   	// 左上の頂点Y		
	pVtx[ 1 ].pos.x = Goal.fRightUp.x;  	// 右上の頂点X
	pVtx[ 1 ].pos.y = Goal.fRightUp.y;  	// 右上の頂点Y
	pVtx[ 2 ].pos.x = Goal.fLeftDown.x; 	// 左下の頂点X
	pVtx[ 2 ].pos.y = Goal.fLeftDown.y; 	// 左下の頂点Y
	pVtx[ 3 ].pos.x = Goal.fRightDown.x;	// 右下の頂点X
	pVtx[ 3 ].pos.y = Goal.fRightDown.y;	// 右下の頂点Y

	// 頂点バッファのアンロック
	g_pVtxBufferGoal->Unlock();	

	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					 // デバイスのポインタを渡す
											GOAL_TEXTURENAME,			 // テクスチャのファイル名
											&g_pTextureGoal ) ) ) // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK ); // エラーチェック
	}	
}

/*------------------------------------------------------------------------------
  　終了処理
------------------------------------------------------------------------------*/	
void UninitGoal( void )
{
	// テクスチャインターフェースの開放
	if( g_pTextureGoal != NULL )
	{
		g_pTextureGoal->Release();
		g_pTextureGoal = NULL;
	}

	// 頂点バッファインターフェースの開放
	if( g_pVtxBufferGoal != NULL )
	{
		g_pVtxBufferGoal->Release();
		g_pVtxBufferGoal = NULL;
	}
}

/*------------------------------------------------------------------------------
  　更新処理
------------------------------------------------------------------------------*/
void UpdateGoal( void )
{	
	// スクロール背景の取得
	MOVEBG* pMoveBg = NULL;
	pMoveBg			= GetMoveBG();
	
	// ゴールの移動 ///////////////////////////////////////////////////////////////

	// 座標の更新
	Goal.fPos.x -= ( pMoveBg->fTexScrollSpeed * GOAL_SCROLL_COEFFICIENT );
	
	// 四隅の座標の算出
	Goal.fLeftUp.x    = Goal.fPos.x;
	Goal.fLeftUp.y    = Goal.fPos.y;
	Goal.fRightUp.x   = Goal.fPos.x + GOAL_WIDTH;
	Goal.fRightUp.y   = Goal.fPos.y;
	Goal.fLeftDown.x  = Goal.fPos.x;
	Goal.fLeftDown.y  = Goal.fPos.y + GOAL_HEIGHT;
	Goal.fRightDown.x = Goal.fPos.x + GOAL_WIDTH;
	Goal.fRightDown.y = Goal.fPos.y + GOAL_HEIGHT;

	VERTEX_2D* pVtx = NULL; // VRAMの仮想アドレスを格納
	
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferGoal->Lock( 0,
							  0,				// すべてをロック
							  ( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
							  0 );	

	// 頂点バッファ設定
	SetVertexGoal( pVtx );		
	
	// UV座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );					    // 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );					    // 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );	// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );	// 右下の頂点
	
	// 頂点バッファのアンロック
	g_pVtxBufferGoal->Unlock();	
}

/*------------------------------------------------------------------------------
  　描画処理
------------------------------------------------------------------------------*/
void DrawGoal( void )
{
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// パイプライン（ストリーム）を設定
	pDevice->SetStreamSource( 0,						// パイプライン番号
							  g_pVtxBufferGoal,			// パイプラインの元になる領域を指定
							  0,						// オフセット（単位はbyte）※ストリームの開始位置を指定できる
							  sizeof( VERTEX_2D ) );	// 流すデータの単位サイズ（ストライド量）

	// テクスチャの設定
	pDevice->SetTexture( 0, g_pTextureGoal );

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_2D );

	// プリミティブ（Polygon）の描画	

	if( Goal.bUse )
	{
		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, // プリミティブの種類
								0,			 // 描画開始位置のオフセット（頂点数）
								NUM_POLYGON );		 // 描画するプリミティブの数
	}	
}

/*------------------------------------------------------------------------------
  　頂点バッファ設定
------------------------------------------------------------------------------*/
void SetVertexGoal( VERTEX_2D* pVtx )
{
	// 頂点座標の設定
	pVtx[ 0 ].pos.x = Goal.fLeftUp.x;   	// 左上の頂点X
	pVtx[ 0 ].pos.y = Goal.fLeftUp.y;   	// 左上の頂点Y		
	pVtx[ 1 ].pos.x = Goal.fRightUp.x;  	// 右上の頂点X
	pVtx[ 1 ].pos.y = Goal.fRightUp.y;  	// 右上の頂点Y
	pVtx[ 2 ].pos.x = Goal.fLeftDown.x; 	// 左下の頂点X
	pVtx[ 2 ].pos.y = Goal.fLeftDown.y; 	// 左下の頂点Y
	pVtx[ 3 ].pos.x = Goal.fRightDown.x;	// 右下の頂点X
	pVtx[ 3 ].pos.y = Goal.fRightDown.y;	// 右下の頂点Y
}

/*------------------------------------------------------------------------------
  　ゲッター
------------------------------------------------------------------------------*/
GOAL* GetGoal( void )
{
	return &Goal;
}

