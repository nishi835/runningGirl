/*==============================================================================

    丸影の処理 [shadow.cpp]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/4/17
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  　ヘッダファイル
------------------------------------------------------------------------------*/
#include "main.h"
#include "input.h"
#include "shadow.h"
#include "polygon.h"
#include "player.h"

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
// セーフリリースマクロ
#define SAFE_RELEASE( x ) { if( x ) { ( x )->Release(); ( x ) = NULL; } }

#define SHADOW_TEXTURENAME "data/TEXTURE/effect000.jpg"
#define NUM_SHADOW (1)	// 数

/*-----------------------------------------------------------t-------------------
  　グローバル変数
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9      g_pTextureShadow   = NULL; // テクスチャインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferShadow = NULL; // 頂点バッファインターフェースの格納

SHADOW Shadow[ NUM_SHADOW ];

/*------------------------------------------------------------------------------
  　ポリゴンの初期化
------------------------------------------------------------------------------*/
HRESULT InitShadow( void )
{
	// 丸影のパラメータ初期化
	Shadow[ 0 ].fPos.x   = 0.0f;
	Shadow[ 0 ].fPos.y   = 0.0f;
	Shadow[ 0 ].fSize.x  = PLAYER_WIDTH * 0.6f;
	Shadow[ 0 ].fSize.y  = PLAYER_HEIGHT * 0.2f;
	Shadow[ 0 ].fColor   = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 0.7f );

	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// *** 頂点バッファの作成 ***
	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_2D ) * NUM_SHADOW * 4,			  // 作成したい頂点バッファの合計データサイズ
											 D3DUSAGE_WRITEONLY,						 	  // 頂点バッファの使用方法（書き込みのみ）
											 FVF_VERTEX_2D,								      // ？
											 D3DPOOL_MANAGED,								  // 頂点バッファの置き場となるメモリを指定（デバイスに委託）
											 &g_pVtxBufferShadow,							  // 頂点バッファ管理インターフェースのポインタ（インターフェースの格納）
											 NULL ) ) )										  // 使わない
											 {
												 return E_FAIL;
											 }	

	// *** 頂点バッファの初期化 ***										

	VERTEX_2D* pVtx; // VRAMの仮想頂点アドレスを格納

	// 頂点バッファをロックして、仮想頂点アドレスを取得する
	if( FAILED ( ( g_pVtxBufferShadow )->Lock( 0,
											   0,				 // すべてをロック
											   ( void** )&pVtx,  // 仮想頂点アドレス用ポインタのアドレスを渡す
											   0 ) ) )			 // ロックフラグ？
											   {	
										   	    return E_FAIL;
											   }		
	// 頂点データの設定
	for( int nCnt = 0; nCnt < NUM_SHADOW; nCnt++ )
	{
			// 頂点カラーの設定（ 0 ～ 255の整数値 )
			pVtx[ 0 ].color = Shadow[ 0 ].fColor;
			pVtx[ 1 ].color = Shadow[ 0 ].fColor;
			pVtx[ 2 ].color = Shadow[ 0 ].fColor;
			pVtx[ 3 ].color = Shadow[ 0 ].fColor;
		
			// rhwの設定（必ず1.0f）※手前か奥（バッファ）の情報？
			pVtx[ 0 ].rhw = 1.0f;	// 左上の頂点
			pVtx[ 1 ].rhw = 1.0f;	// 右上の頂点
			pVtx[ 2 ].rhw = 1.0f;	// 左下の頂点
			pVtx[ 3 ].rhw = 1.0f;	// 右下の頂点

			// 頂点データにUVデータを追加
			pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
			pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
			pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
			pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );
		
			// 頂点座標
			pVtx[ 0 ].pos  = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			pVtx[ 1 ].pos  = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			pVtx[ 2 ].pos  = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			pVtx[ 3 ].pos  = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			
			pVtx += 4;			
	}
	
	// 頂点バッファのロックを解除
	g_pVtxBufferShadow->Unlock();
		
	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice, SHADOW_TEXTURENAME, &g_pTextureShadow ) ) )
	{
		MessageBox( NULL, "読み込み失敗", "エラー", MB_OK ); 
	}
	
	return S_OK;
}

/*------------------------------------------------------------------------------
  　ポリゴンの終了処理
------------------------------------------------------------------------------*/
void UninitShadow( void )
{
	// 終了処理
	// テクスチャインターフェースの終了処理
	if( g_pTextureShadow )
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	// 頂点バッファ管理インターフェースポインタの終了処理
	if( g_pVtxBufferShadow )
	{
		g_pVtxBufferShadow->Release();
		g_pVtxBufferShadow = NULL;
	}
}

/*------------------------------------------------------------------------------
  　ポリゴン更新処理
------------------------------------------------------------------------------*/
void UpdateShadow( void )
{
	// プレイヤーの情報を取得
	PLAYER* pPlayer = GetPlayer();

	// 丸影の座標更新
	Shadow[ 0 ].fPos.x = pPlayer->fAxis.x;
	if( pPlayer->State != STATE_JUMP && pPlayer->State != STATE_JUMPER )
	{
		Shadow[ 0 ].fPos.y = pPlayer->fAxis.y - 15.0f;
	}

	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	VERTEX_2D* pVtx; // VRAMの仮想頂点アドレスを格納

	// 頂点バッファをロックして、仮想頂点アドレスを取得する
	g_pVtxBufferShadow->Lock( 0,
							  0,				 // すべてをロック
							  ( void** )&pVtx,   // 仮想頂点アドレス用ポインタのアドレスを渡す
							  0 );				 // ロックフラグ？

	// 頂点データの設定
	for( int nCnt = 0; nCnt < NUM_SHADOW; nCnt++ )
	{
			// 頂点カラーの設定（ 0 ～ 255の整数値 )
			pVtx[ 0 ].color = Shadow[ 0 ].fColor;
			pVtx[ 1 ].color = Shadow[ 0 ].fColor;
			pVtx[ 2 ].color = Shadow[ 0 ].fColor;
			pVtx[ 3 ].color = Shadow[ 0 ].fColor;
		
			// 頂点データにUVデータを追加
			pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
			pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
			pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
			pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );
		
			// 頂点座標
			pVtx[ 0 ].pos  = D3DXVECTOR3( Shadow[ nCnt ].fPos.x - Shadow[ nCnt ].fSize.x * 0.5f, Shadow[ nCnt ].fPos.y						   , 0.0f );
			pVtx[ 1 ].pos  = D3DXVECTOR3( Shadow[ nCnt ].fPos.x + Shadow[ nCnt ].fSize.x * 0.5f, Shadow[ nCnt ].fPos.y						   , 0.0f );
			pVtx[ 2 ].pos  = D3DXVECTOR3( Shadow[ nCnt ].fPos.x - Shadow[ nCnt ].fSize.x * 0.5f, Shadow[ nCnt ].fPos.y + Shadow[ nCnt ].fSize.y, 0.0f );
			pVtx[ 3 ].pos  = D3DXVECTOR3( Shadow[ nCnt ].fPos.x + Shadow[ nCnt ].fSize.x * 0.5f, Shadow[ nCnt ].fPos.y + Shadow[ nCnt ].fSize.y, 0.0f );
			
			pVtx += 4;			
	}
	
	// 頂点バッファのロックを解除
	g_pVtxBufferShadow->Unlock();
}

/*------------------------------------------------------------------------------
  　ポリゴン描画処理
------------------------------------------------------------------------------*/
void DrawShadow( void )
{
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// アルファブレンド（透明化）の設定
	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );		  // アルファブレンドが有効かどうか
    pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT ); // アルファブレンドの演算式を設定, 引き算に設定
	pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );	  // 描画元（スプライト）のアルファブレンド設定
	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE);		  // 描画先（背景）のアルファブレンド設定

	// パイプライン（ストリーム）を設定
	pDevice->SetStreamSource( 0,						// パイプライン番号
							  g_pVtxBufferShadow,		// パイプラインの元になる領域を指定
							  0,						// オフセット（単位はbyte）※ストリームの開始位置を指定できる
							  sizeof( VERTEX_2D ) );	// 流すデータの単位サイズ（ストライド量）

	// テクスチャの設定
	pDevice->SetTexture( 0, g_pTextureShadow );

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_2D );

	for( int nCnt = 0; nCnt < NUM_SHADOW; nCnt++ )
	{
		// プリミティブ（ポリゴン）の描画
		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,	// プリミティブの種類
								nCnt * 4,				// オフセット
								NUM_SHADOW * 2			// プリミティブの数
								);
	}
	
	// アルファブレンド（透明化）の設定を元に戻す
	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );		  // アルファブレンドが有効かどうか
	pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );		  // アルファブレンドの演算式を設定, 足し算に設定
	pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );	  // 描画元（スプライト）のアルファブレンド設定
	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);  // 描画先（背景）のアルファブレンド設定
}