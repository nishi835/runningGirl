/*==============================================================================

    ポリゴン処理 [Player.cpp]
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
#include "can.h"
#include "player.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define CAN_TEXTURENAME "data/TEXTURE/tire.png"			 // テクスチャファイル名
#define CAN_TEX_WIDTH		   (1.0f)					 // 缶のテクスチャ幅
#define CAN_TEX_HEIGHT		   (1.0f)					 // 缶のテクスチャ高さ
#define CAN_MOVE_SPEED		   (3.2f)					 // 缶の移動速度
#define CAN_SCROLL_COEFFICIENT (800.0f)					 // 缶の移動を背景に合わせる係数

/*------------------------------------------------------------------------------
  　グローバル変数
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTextureCan	= NULL;	// テクスチャインターフェースのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferCan = NULL; // 頂点バッファ管理インターフェースポインタ

CAN Can[ CAN_MAX ];		  // 缶の構造体配列

// 回転関係
D3DXVECTOR2 g_posCan;	  // ポリゴンの中心座標
float		g_fLengthCan; // ポリゴンの対角線の長さ
float	    g_fAngleCan;  // ポリゴンの対角線の角度

/*------------------------------------------------------------------------------
  　初期化処理
------------------------------------------------------------------------------*/
void InitCan( void )
{
	srand( ( unsigned )time ( 0 ) );	// 乱数設定

	// 缶の初期化	
	for( int nCntCan = 0; nCntCan < CAN_MAX; nCntCan++ )
	{
		Can[ nCntCan ].fPos		  = D3DXVECTOR2( SCREEN_WIDTH, LIMIT_BOTTOM - CAN_HEIGHT );
		Can[ nCntCan ].fMove	  = D3DXVECTOR2( - CAN_MOVE_SPEED, 0.0f );
		Can[ nCntCan ].fPosTex.x  = 0.0f;
		Can[ nCntCan ].fPosTex.y  = 0.0f;
		Can[ nCntCan ].fRot       = 0.0f;
		Can[ nCntCan ].bFly		  = false;
		Can[ nCntCan ].bUse		  = false;
	}
	
	// --- 回転準備 ---	////////////////////////////////////////////////////////

	g_fLengthCan = sqrt( CAN_WIDTH * CAN_WIDTH + CAN_HEIGHT * CAN_HEIGHT ) / 2.0f;   // 対角線の長さの算出
	g_fAngleCan  = atan2( CAN_HEIGHT, CAN_WIDTH );									 // 対角線の角度の算出

	// 頂点の設定
	MakeVertexPolygon( &g_pVtxBufferCan, CAN_MAX );
	
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,			 // デバイスのポインタを渡す
											CAN_TEXTURENAME,     // テクスチャのファイル名
											&g_pTextureCan ) ) ) // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK ); // エラーチェック
	}
}

/*------------------------------------------------------------------------------
  　終了処理
------------------------------------------------------------------------------*/	
void UninitCan( void )
{
	// テクスチャインターフェース、頂点バッファインターフェースの開放
	UninitPolygon( &g_pTextureCan, &g_pVtxBufferCan );
}

/*------------------------------------------------------------------------------
  　更新処理
------------------------------------------------------------------------------*/
void UpdateCan( void )
{	
	// スクロール背景の取得
	MOVEBG* pMoveBg = NULL;
	pMoveBg			= GetMoveBG();
	
		
	VERTEX_2D* pVtx = NULL; // VRAMの仮想アドレスを格納
	
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferCan->Lock( 0,
						   0,				 // すべてをロック
						   ( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
						   0 );	

	for( int nCntCan = 0; nCntCan < CAN_MAX; nCntCan++ )
	{
		if( !Can[ nCntCan ].bUse ) { continue; } 

		// 缶の移動 ///////////////////////////////////////////////////////////////

		// 移動量の設定
		if( Can[ nCntCan ].bFly )
		{
			Can[ nCntCan ].fMove.y += 0.6f;
			Can[ nCntCan ].fRot    += 0.4f;
		}
		else if( Can[ nCntCan ].fPos.x < SCREEN_WIDTH )
		{
			Can[ nCntCan ].fMove.x = - ( pMoveBg[ 0 ].fTexScrollSpeed * CAN_SCROLL_COEFFICIENT + CAN_MOVE_SPEED );
		}
		else
		{
			Can[ nCntCan ].fMove.x = - ( pMoveBg[ 0 ].fTexScrollSpeed * CAN_SCROLL_COEFFICIENT );
		}

		// 座標の更新
		Can[ nCntCan ].fPos += Can[ nCntCan ].fMove;
	
		// 頂点座標の設定 /////////////////////////////////////////////////////////
		
		// 中心座標の算出
		Can[ nCntCan ].fAxis.y = Can[ nCntCan ].fPos.y + CAN_HEIGHT * 0.5f;
		Can[ nCntCan ].fAxis.x = Can[ nCntCan ].fPos.x + CAN_WIDTH * 0.5f;

		// 四隅の座標の算出
 		Can[ nCntCan ].fLeftUp.x    = Can[ nCntCan ].fAxis.x;
		Can[ nCntCan ].fLeftUp.y    = Can[ nCntCan ].fAxis.y;
		Can[ nCntCan ].fRightUp.x   = Can[ nCntCan ].fAxis.x + CAN_WIDTH;
		Can[ nCntCan ].fRightUp.y   = Can[ nCntCan ].fAxis.y;
		Can[ nCntCan ].fLeftDown.x  = Can[ nCntCan ].fAxis.x;
		Can[ nCntCan ].fLeftDown.y  = Can[ nCntCan ].fAxis.y + CAN_HEIGHT;
		Can[ nCntCan ].fRightDown.x = Can[ nCntCan ].fAxis.x + CAN_WIDTH;
		Can[ nCntCan ].fRightDown.y = Can[ nCntCan ].fAxis.y + CAN_HEIGHT;
		
		// 四隅の座標の更新
		Can[ nCntCan ].fLeftUp.x    = Can[ nCntCan ].fAxis.x + cos( Can[ nCntCan ].fRot + D3DX_PI + g_fAngleCan ) * g_fLengthCan;		// 左上の頂点X
		Can[ nCntCan ].fLeftUp.y    = Can[ nCntCan ].fAxis.y + sin( Can[ nCntCan ].fRot + D3DX_PI + g_fAngleCan ) * g_fLengthCan;		// 左上の頂点Y		
		Can[ nCntCan ].fRightUp.x   = Can[ nCntCan ].fAxis.x + cos( Can[ nCntCan ].fRot			 - g_fAngleCan ) * g_fLengthCan;		// 右上の頂点X
		Can[ nCntCan ].fRightUp.y   = Can[ nCntCan ].fAxis.y + sin( Can[ nCntCan ].fRot			 - g_fAngleCan ) * g_fLengthCan;		// 右上の頂点Y
		Can[ nCntCan ].fLeftDown.x  = Can[ nCntCan ].fAxis.x + cos( Can[ nCntCan ].fRot + D3DX_PI - g_fAngleCan ) * g_fLengthCan;		// 左下の頂点X
		Can[ nCntCan ].fLeftDown.y  = Can[ nCntCan ].fAxis.y + sin( Can[ nCntCan ].fRot + D3DX_PI - g_fAngleCan ) * g_fLengthCan;		// 左下の頂点Y
		Can[ nCntCan ].fRightDown.x = Can[ nCntCan ].fAxis.x + cos( Can[ nCntCan ].fRot			 + g_fAngleCan ) * g_fLengthCan;		// 右下の頂点X
		Can[ nCntCan ].fRightDown.y = Can[ nCntCan ].fAxis.y + sin( Can[ nCntCan ].fRot			 + g_fAngleCan ) * g_fLengthCan;		// 右下の頂点Y
		
		// 頂点座標の設定
		pVtx[ 0 ].pos.x = Can[ nCntCan ].fLeftUp.x;		// 左上の頂点X
		pVtx[ 0 ].pos.y = Can[ nCntCan ].fLeftUp.y;		// 左上の頂点Y		
		pVtx[ 1 ].pos.x = Can[ nCntCan ].fRightUp.x;	// 右上の頂点X
		pVtx[ 1 ].pos.y = Can[ nCntCan ].fRightUp.y;	// 右上の頂点Y
		pVtx[ 2 ].pos.x = Can[ nCntCan ].fLeftDown.x;	// 左下の頂点X
		pVtx[ 2 ].pos.y = Can[ nCntCan ].fLeftDown.y;	// 左下の頂点Y
		pVtx[ 3 ].pos.x = Can[ nCntCan ].fRightDown.x;	// 右下の頂点X
		pVtx[ 3 ].pos.y = Can[ nCntCan ].fRightDown.y;	// 右下の頂点Y
	
		// UV座標の設定
		pVtx[ 0 ].tex = D3DXVECTOR2( Can[ nCntCan ].fPosTex.x,				   Can[ nCntCan ].fPosTex.y );					// 左上の頂点
		pVtx[ 1 ].tex = D3DXVECTOR2( Can[ nCntCan ].fPosTex.x + CAN_TEX_WIDTH, Can[ nCntCan ].fPosTex.y );					// 右上の頂点
		pVtx[ 2 ].tex = D3DXVECTOR2( Can[ nCntCan ].fPosTex.x,				   Can[ nCntCan ].fPosTex.y + CAN_TEX_HEIGHT );	// 左下の頂点	
		pVtx[ 3 ].tex = D3DXVECTOR2( Can[ nCntCan ].fPosTex.x + CAN_TEX_WIDTH, Can[ nCntCan ].fPosTex.y + CAN_TEX_HEIGHT );	// 右下の頂点

		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_pVtxBufferCan->Unlock();	
}

/*------------------------------------------------------------------------------
  　描画処理
------------------------------------------------------------------------------*/
void DrawCan( void )
{
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// パイプライン（ストリーム）を設定
	pDevice->SetStreamSource( 0,						// パイプライン番号
							  g_pVtxBufferCan,			// パイプラインの元になる領域を指定
							  0,						// オフセット（単位はbyte）※ストリームの開始位置を指定できる
							  sizeof( VERTEX_2D ) );	// 流すデータの単位サイズ（ストライド量）

	// テクスチャの設定
	pDevice->SetTexture( 0, g_pTextureCan );

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_2D );

	// プリミティブ（Polygon）の描画
	for( int nCnt = 0; nCnt < CAN_MAX; nCnt++ )
	{
		if( !Can[ nCnt ].bUse ) { continue; }

		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, // プリミティブの種類
								nCnt * 4,			 // 描画開始位置のオフセット（頂点数）
								NUM_POLYGON );		 // 描画するプリミティブの数
	}	
}

/*------------------------------------------------------------------------------
  　ゲッター
------------------------------------------------------------------------------*/
CAN* GetCan( void )
{
	return Can;
}

