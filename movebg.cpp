/*==============================================================================

    MoveBG処理 [MoveBG.cpp]
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
#include "input.h"
#include "score.h"

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define MOVEBG_TEXTURENAME "data/TEXTURE/movebg07.png" // テクスチャファイル名
#define MOVEBG_POS_X  ( 0 )							   // MoveBGの表示位置X
#define MOVEBG_POS_Y  ( 210 )						   // MoveBGの表示位置Y
#define MOVEBG_WIDTH  ( 800 )						   // MoveBGの幅
#define MOVEBG_HEIGHT ( 250 )						   // MoveBGの高さ
#define MOVEBG_TEXTURE_WIDTH  ( 1.0f )				   // MoveBGの幅
#define MOVEBG_TEXTURE_HEIGHT ( 1.0f )				   // MoveBGの高さ
#define MOVEBG_MAX (1)								   // MoveBGの数

/*------------------------------------------------------------------------------
  　グローバル変数
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9      g_pTextureMoveBG   = NULL;	// テクスチャインターフェースのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferMoveBG = NULL;  // 頂点バッファ管理インターフェースポインタ

MOVEBG MoveBG[ 2 ];										// MoveBGの構造体

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
HRESULT MakeVertexMoveBG( LPDIRECT3DDEVICE9 pDevice );

/*------------------------------------------------------------------------------
  　初期化処理
------------------------------------------------------------------------------*/
void InitMoveBG( void )
{
	// MoveBGの初期化
	for( int nCntMoveBG = 0; nCntMoveBG < MOVEBG_MAX; nCntMoveBG++ )
	{
		MoveBG[ nCntMoveBG ].fPosTexX        = 0.0f;
		MoveBG[ nCntMoveBG ].fPosTexY		 = 0.0f;
		MoveBG[ nCntMoveBG ].fTexScrollSpeed = 0.0f;
	}

	// 頂点の設定
	MakeVertexPolygon( &g_pVtxBufferMoveBG, MOVEBG_MAX );
	
	VERTEX_2D* pVtx = NULL; // VRAMの仮想アドレスを格納
	
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferMoveBG->Lock( 0,
							  0,				// すべてをロック
							  ( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
							  0 );			
		
	for( int nCntMoveBG = 0; nCntMoveBG < MOVEBG_MAX; nCntMoveBG++ )
	{
		// 頂点座標の設定
		pVtx[ 0 ].pos = D3DXVECTOR3( MOVEBG_POS_X,				  MOVEBG_POS_Y,					0.0f );				// 左上の頂点
		pVtx[ 1 ].pos = D3DXVECTOR3( MOVEBG_POS_X + MOVEBG_WIDTH, MOVEBG_POS_Y,					0.0f );				// 右上の頂点
		pVtx[ 2 ].pos = D3DXVECTOR3( MOVEBG_POS_X,				  MOVEBG_POS_Y + MOVEBG_HEIGHT, 0.0f );				// 左下の頂点	
		pVtx[ 3 ].pos = D3DXVECTOR3( MOVEBG_POS_X + MOVEBG_WIDTH, MOVEBG_POS_Y + MOVEBG_HEIGHT, 0.0f );				// 右下の頂点
	
		// UV座標の設定
		pVtx[ 0 ].tex = D3DXVECTOR2( MoveBG[ nCntMoveBG ].fPosTexX,						   MoveBG[ nCntMoveBG ].fPosTexY );							// 左上の頂点
		pVtx[ 1 ].tex = D3DXVECTOR2( MoveBG[ nCntMoveBG ].fPosTexX + MOVEBG_TEXTURE_WIDTH, MoveBG[ nCntMoveBG ].fPosTexY );							// 右上の頂点
		pVtx[ 2 ].tex = D3DXVECTOR2( MoveBG[ nCntMoveBG ].fPosTexX,						   MoveBG[ nCntMoveBG ].fPosTexY + MOVEBG_TEXTURE_HEIGHT );	// 左下の頂点	
		pVtx[ 3 ].tex = D3DXVECTOR2( MoveBG[ nCntMoveBG ].fPosTexX + MOVEBG_TEXTURE_WIDTH, MoveBG[ nCntMoveBG ].fPosTexY + MOVEBG_TEXTURE_HEIGHT );	// 右下の頂点

		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_pVtxBufferMoveBG->Unlock();

	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	if( FAILED( D3DXCreateTextureFromFile( pDevice,				 // デバイスのポインタを渡す
										   MOVEBG_TEXTURENAME,      // テクスチャのファイル名
										   &g_pTextureMoveBG ) ) )  // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK ); // エラーチェック
	}
	

}

/*------------------------------------------------------------------------------
  　終了処理
------------------------------------------------------------------------------*/
void UninitMoveBG( void )
{
	UninitPolygon( &g_pTextureMoveBG, &g_pVtxBufferMoveBG );
}

/*------------------------------------------------------------------------------
  　更新処理
------------------------------------------------------------------------------*/
void UpdateMoveBG( void )
{	
	// テクスチャの位置更新
	MoveBG[ 0 ].fPosTexX += MoveBG[ 0 ].fTexScrollSpeed;
	
	// スコアの更新処理
	SetScore( MoveBG[ 0 ].fTexScrollSpeed * 10 ); // スコアのセット

	VERTEX_2D* pVtx = NULL; // VRAMの仮想アドレスを格納
	
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferMoveBG->Lock( 0,
							  0,				// すべてをロック
							  ( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
							  0 );			
		
	for( int nCntMoveBG = 0; nCntMoveBG < MOVEBG_MAX; nCntMoveBG++ )
	{
		// 頂点座標の設定
		pVtx[ 0 ].pos = D3DXVECTOR3( MOVEBG_POS_X,				  MOVEBG_POS_Y,					0.0f );				// 左上の頂点
		pVtx[ 1 ].pos = D3DXVECTOR3( MOVEBG_POS_X + MOVEBG_WIDTH, MOVEBG_POS_Y,					0.0f );				// 右上の頂点
		pVtx[ 2 ].pos = D3DXVECTOR3( MOVEBG_POS_X,				  MOVEBG_POS_Y + MOVEBG_HEIGHT, 0.0f );				// 左下の頂点	
		pVtx[ 3 ].pos = D3DXVECTOR3( MOVEBG_POS_X + MOVEBG_WIDTH, MOVEBG_POS_Y + MOVEBG_HEIGHT, 0.0f );				// 右下の頂点
	
		// UV座標の設定
		pVtx[ 0 ].tex = D3DXVECTOR2( MoveBG[ nCntMoveBG ].fPosTexX,						   MoveBG[ nCntMoveBG ].fPosTexY );							// 左上の頂点
		pVtx[ 1 ].tex = D3DXVECTOR2( MoveBG[ nCntMoveBG ].fPosTexX + MOVEBG_TEXTURE_WIDTH, MoveBG[ nCntMoveBG ].fPosTexY );							// 右上の頂点
		pVtx[ 2 ].tex = D3DXVECTOR2( MoveBG[ nCntMoveBG ].fPosTexX,						   MoveBG[ nCntMoveBG ].fPosTexY + MOVEBG_TEXTURE_HEIGHT );	// 左下の頂点	
		pVtx[ 3 ].tex = D3DXVECTOR2( MoveBG[ nCntMoveBG ].fPosTexX + MOVEBG_TEXTURE_WIDTH, MoveBG[ nCntMoveBG ].fPosTexY + MOVEBG_TEXTURE_HEIGHT );	// 右下の頂点

		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_pVtxBufferMoveBG->Unlock();
}

/*------------------------------------------------------------------------------
  　描画処理
------------------------------------------------------------------------------*/
void DrawMoveBG( void )
{
	DrawPolygon( g_pTextureMoveBG, g_pVtxBufferMoveBG, 1 );
}

/*------------------------------------------------------------------------------
  　ゲッター
------------------------------------------------------------------------------*/
MOVEBG* GetMoveBG( void )
{
	return &MoveBG[ 0 ];
}