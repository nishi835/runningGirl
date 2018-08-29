/*==============================================================================

    BG処理 [BG.cpp]
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
#include "BG.h"
#include "movebg.h"

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define BG_TEXTURENAME "data/TEXTURE/sky04.png"					// テクスチャファイル名
#define BG_TEXTURENAME_AUDIENCE "data/TEXTURE/audience03.png" 
#define BG_TEXTURENAME_AUDIENCE02 "data/TEXTURE/audience01.png" 
#define BG_TEXTURENAME_NUMBERS "data/TEXTURE/numbers.png" 

#define BG_POS_X ( 0.0f )							// BGの表示位置X
#define BG_POS_Y ( 0.0f )							// BGの表示位置Y
#define BG_WIDTH ( 800.0f )							// BGの幅
#define BG_HEIGHT ( 210.0f )						// BGの高さ

#define BG_AUDIENCE_POS_X ( 0.0f )							// BGの表示位置X
#define BG_AUDIENCE_POS_Y ( 450.0f )						// BGの表示位置Y
#define BG_AUDIENCE_WIDTH ( 800.0f )						// BGの幅
#define BG_AUDIENCE_HEIGHT ( 150.0f )						// BGの高さ

#define BG_AUDIENCE02_POS_X ( 0.0f )						// BGの表示位置X
#define BG_AUDIENCE02_POS_Y ( 70.0f )						// BGの表示位置Y
#define BG_AUDIENCE02_WIDTH ( 800.0f )						// BGの幅
#define BG_AUDIENCE02_HEIGHT ( 80.0f )						// BGの高さ

#define BG_TEX_MAX (4)                          // テクスチャの最大数
#define BG_TEXTURE_WIDTH ( 1.0f )				// BGの幅
#define BG_TEXTURE_HEIGHT ( 1.0f )				// BGの高さ
#define BG_TEXTURE_SCROLL_SPEED ( 0.002f )		// BGのテクスチャスクロール速度
#define BG_SCROLL_COEFFICIENT (800.0f)			// BGのスクロールの係数

#define BG_NUMBERS_POS_X  ( 0.0f )							   // numbersの表示位置X
#define BG_NUMBERS_POS_Y  ( 210.0f )						   // numbersの表示位置Y
#define BG_NUMBERS_WIDTH  ( 800.0f )							   // numbersの幅
#define BG_NUMBERS_HEIGHT ( 240.0f )						   // numbersの高さ

/*------------------------------------------------------------------------------
  　構造体の宣言
------------------------------------------------------------------------------*/
// BGの構造体
typedef struct
{
	D3DXVECTOR2 fPos;   // 座標
	float fPosTexX;		// BGのテクスチャX座標
	float fPosTexY;		// BGのテクスチャY座標
}BG;

/*------------------------------------------------------------------------------
  　グローバル変数
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureBG[ BG_TEX_MAX ] = {};			// テクスチャインターフェースのポインタ
LPDIRECT3DTEXTURE9      g_pTextureNumbers   = NULL;	// テクスチャインターフェースのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBG = NULL;  // 頂点バッファ管理インターフェースポインタ

BG Bg[ BG_TEX_MAX ];											// BGの構造体

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
HRESULT MakeVertexBG( LPDIRECT3DDEVICE9 pDevice );

/*------------------------------------------------------------------------------
  　初期化処理
------------------------------------------------------------------------------*/
void InitBG( void )
{
	// BGの初期化
	Bg[ 0 ].fPosTexX = 0.0f;
	Bg[ 0 ].fPosTexY = 0.0f;
	Bg[ 1 ].fPosTexX = 0.0f;
	Bg[ 1 ].fPosTexY = 0.0f;
	Bg[ 2 ].fPosTexX = 0.0f;
	Bg[ 2 ].fPosTexY = 0.0f;
	Bg[ 3 ].fPosTexX = 0.0f;
	Bg[ 3 ].fPosTexY = 0.0f;

	// 頂点の設定
	MakeVertexPolygon( &g_pVtxBufferBG, BG_TEX_MAX );
	
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,				 // デバイスのポインタを渡す
											BG_TEXTURENAME,     // テクスチャのファイル名
											&g_pTextureBG[ 0 ] ) ) ) // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK ); // エラーチェック
	}

	if( FAILED ( D3DXCreateTextureFromFile( pDevice,				 // デバイスのポインタを渡す
											BG_TEXTURENAME_AUDIENCE,     // テクスチャのファイル名
											&g_pTextureBG[ 1 ] ) ) ) // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK ); // エラーチェック
	}

	if( FAILED ( D3DXCreateTextureFromFile( pDevice,				 // デバイスのポインタを渡す
											BG_TEXTURENAME_AUDIENCE02,     // テクスチャのファイル名
											&g_pTextureBG[ 2 ] ) ) ) // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK ); // エラーチェック
	}
	
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,				 // デバイスのポインタを渡す
											BG_TEXTURENAME_NUMBERS,     // テクスチャのファイル名
											&g_pTextureBG[ 3 ] ) ) ) // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK ); // エラーチェック
	}
	
	VERTEX_2D* pVtx = NULL; // VRAMの仮想アドレスを格納

	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferBG->Lock( 0,
						  0,				// すべてをロック
						  ( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
						  0 );			

	// 頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( BG_POS_X,			  BG_POS_Y,				0.0f );				// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( BG_POS_X + BG_WIDTH, BG_POS_Y,				0.0f );				// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( BG_POS_X,			  BG_POS_Y + BG_HEIGHT, 0.0f );				// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( BG_POS_X + BG_WIDTH, BG_POS_Y + BG_HEIGHT, 0.0f );				// 右下の頂点
	
	// 頂点座標の設定
	pVtx[ 4 ].pos = D3DXVECTOR3( BG_AUDIENCE_POS_X,					    BG_AUDIENCE_POS_Y,						0.0f );				// 左上の頂点
	pVtx[ 5 ].pos = D3DXVECTOR3( BG_AUDIENCE_POS_X + BG_AUDIENCE_WIDTH, BG_AUDIENCE_POS_Y,						0.0f );				// 右上の頂点
	pVtx[ 6 ].pos = D3DXVECTOR3( BG_AUDIENCE_POS_X,					    BG_AUDIENCE_POS_Y + BG_AUDIENCE_HEIGHT, 0.0f );				// 左下の頂点	
	pVtx[ 7 ].pos = D3DXVECTOR3( BG_AUDIENCE_POS_X + BG_AUDIENCE_WIDTH, BG_AUDIENCE_POS_Y + BG_AUDIENCE_HEIGHT, 0.0f );				// 右下の頂点
	
	// 頂点座標の設定
	pVtx[ 8 ].pos  = D3DXVECTOR3( BG_AUDIENCE02_POS_X,					   BG_AUDIENCE02_POS_Y,						   0.0f );				// 左上の頂点
	pVtx[ 9 ].pos  = D3DXVECTOR3( BG_AUDIENCE02_POS_X + BG_AUDIENCE_WIDTH, BG_AUDIENCE02_POS_Y,						   0.0f );				// 右上の頂点
	pVtx[ 10 ].pos = D3DXVECTOR3( BG_AUDIENCE02_POS_X,					   BG_AUDIENCE02_POS_Y + BG_AUDIENCE02_HEIGHT, 0.0f );				// 左下の頂点	
	pVtx[ 11 ].pos = D3DXVECTOR3( BG_AUDIENCE02_POS_X + BG_AUDIENCE_WIDTH, BG_AUDIENCE02_POS_Y + BG_AUDIENCE02_HEIGHT, 0.0f );				// 右下の頂点
	
	// 頂点座標の設定
	pVtx[ 12 ].pos = D3DXVECTOR3( Bg[ 3 ].fPos.x,				     BG_NUMBERS_POS_Y,					   0.0f );				// 左上の頂点
	pVtx[ 13 ].pos = D3DXVECTOR3( Bg[ 3 ].fPos.x + BG_NUMBERS_WIDTH, BG_NUMBERS_POS_Y,					   0.0f );				// 右上の頂点
	pVtx[ 14 ].pos = D3DXVECTOR3( Bg[ 3 ].fPos.x,					 BG_NUMBERS_POS_Y + BG_NUMBERS_HEIGHT, 0.0f );				// 左下の頂点	
	pVtx[ 15 ].pos = D3DXVECTOR3( Bg[ 3 ].fPos.x + BG_NUMBERS_WIDTH, BG_NUMBERS_POS_Y + BG_NUMBERS_HEIGHT, 0.0f );				// 右下の頂点

	// UV座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( Bg[ 0 ].fPosTexX,					  Bg[ 0 ].fPosTexY );							// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( Bg[ 0 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 0 ].fPosTexY );							// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( Bg[ 0 ].fPosTexX,					  Bg[ 0 ].fPosTexY + BG_TEXTURE_HEIGHT );		// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( Bg[ 0 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 0 ].fPosTexY + BG_TEXTURE_HEIGHT );		// 右下の頂点
	
	// UV座標の設定
	pVtx[ 4 ].tex = D3DXVECTOR2( Bg[ 1 ].fPosTexX,					  Bg[ 1 ].fPosTexY );							// 左上の頂点
	pVtx[ 5 ].tex = D3DXVECTOR2( Bg[ 1 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 1 ].fPosTexY );							// 右上の頂点
	pVtx[ 6 ].tex = D3DXVECTOR2( Bg[ 1 ].fPosTexX,					  Bg[ 1 ].fPosTexY + BG_TEXTURE_HEIGHT );		// 左下の頂点	
	pVtx[ 7 ].tex = D3DXVECTOR2( Bg[ 1 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 1 ].fPosTexY + BG_TEXTURE_HEIGHT );		// 右下の頂点
	
	pVtx[ 8 ].tex  = D3DXVECTOR2( Bg[ 2 ].fPosTexX,					   Bg[ 2 ].fPosTexY );							// 左上の頂点
	pVtx[ 9 ].tex  = D3DXVECTOR2( Bg[ 2 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 2 ].fPosTexY );							// 右上の頂点
	pVtx[ 10 ].tex = D3DXVECTOR2( Bg[ 2 ].fPosTexX,					   Bg[ 2 ].fPosTexY + BG_TEXTURE_HEIGHT );		// 左下の頂点	
	pVtx[ 11 ].tex = D3DXVECTOR2( Bg[ 2 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 2 ].fPosTexY + BG_TEXTURE_HEIGHT );		// 右下の頂点
	
	pVtx[ 12 ].tex = D3DXVECTOR2( Bg[ 3 ].fPosTexX,					   Bg[ 3 ].fPosTexY );							// 左上の頂点
	pVtx[ 13 ].tex = D3DXVECTOR2( Bg[ 3 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 3 ].fPosTexY );							// 右上の頂点
	pVtx[ 14 ].tex = D3DXVECTOR2( Bg[ 3 ].fPosTexX,					   Bg[ 3 ].fPosTexY + BG_TEXTURE_HEIGHT );		// 左下の頂点	
	pVtx[ 15 ].tex = D3DXVECTOR2( Bg[ 3 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 3 ].fPosTexY + BG_TEXTURE_HEIGHT );		// 右下の頂点
	
	// 頂点バッファのアンロック
	g_pVtxBufferBG->Unlock();	
}

/*------------------------------------------------------------------------------
  　終了処理
------------------------------------------------------------------------------*/
void UninitBG( void )
{
	// テクスチャインターフェースの終了処理
	for( int nCnt = 0; nCnt < BG_TEX_MAX; nCnt++ )
	{
		if( g_pTextureBG[ nCnt ] != NULL )
		{
			g_pTextureBG[ nCnt ]->Release();
			g_pTextureBG[ nCnt ] = NULL;
		}
	}

	// 頂点バッファ管理インターフェースポインタの終了処理
	if( g_pVtxBufferBG )
	{
		g_pVtxBufferBG->Release();
		g_pVtxBufferBG = NULL;
	}
}

/*------------------------------------------------------------------------------
  　更新処理
------------------------------------------------------------------------------*/
void UpdateBG( void )
{
	// スクロール背景の取得
	MOVEBG* pMoveBg = NULL;
	pMoveBg			= GetMoveBG();

	VERTEX_2D* pVtx = NULL; // VRAMの仮想アドレスを格納

	// テクスチャ座標の更新
	Bg[ 0 ].fPosTexX += pMoveBg->fTexScrollSpeed * 0.1f;
	Bg[ 1 ].fPosTexX += pMoveBg->fTexScrollSpeed;
	Bg[ 2 ].fPosTexX += pMoveBg->fTexScrollSpeed * 0.3f;
	Bg[ 3 ].fPos.x   -= pMoveBg->fTexScrollSpeed * BG_SCROLL_COEFFICIENT;

	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferBG->Lock( 0,
						  0,				// すべてをロック
						  ( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
						  0 );			

	// 頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( BG_POS_X,			  BG_POS_Y,				0.0f );				// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( BG_POS_X + BG_WIDTH, BG_POS_Y,				0.0f );				// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( BG_POS_X,			  BG_POS_Y + BG_HEIGHT, 0.0f );				// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( BG_POS_X + BG_WIDTH, BG_POS_Y + BG_HEIGHT, 0.0f );				// 右下の頂点
	
	// 頂点座標の設定
	pVtx[ 4 ].pos = D3DXVECTOR3( BG_AUDIENCE_POS_X,					    BG_AUDIENCE_POS_Y,						0.0f );				// 左上の頂点
	pVtx[ 5 ].pos = D3DXVECTOR3( BG_AUDIENCE_POS_X + BG_AUDIENCE_WIDTH, BG_AUDIENCE_POS_Y,						0.0f );				// 右上の頂点
	pVtx[ 6 ].pos = D3DXVECTOR3( BG_AUDIENCE_POS_X,					    BG_AUDIENCE_POS_Y + BG_AUDIENCE_HEIGHT, 0.0f );				// 左下の頂点	
	pVtx[ 7 ].pos = D3DXVECTOR3( BG_AUDIENCE_POS_X + BG_AUDIENCE_WIDTH, BG_AUDIENCE_POS_Y + BG_AUDIENCE_HEIGHT, 0.0f );				// 右下の頂点
	
	// 頂点座標の設定
	pVtx[ 8 ].pos  = D3DXVECTOR3( BG_AUDIENCE02_POS_X,					   BG_AUDIENCE02_POS_Y,						   0.0f );				// 左上の頂点
	pVtx[ 9 ].pos  = D3DXVECTOR3( BG_AUDIENCE02_POS_X + BG_AUDIENCE_WIDTH, BG_AUDIENCE02_POS_Y,						   0.0f );				// 右上の頂点
	pVtx[ 10 ].pos = D3DXVECTOR3( BG_AUDIENCE02_POS_X,					   BG_AUDIENCE02_POS_Y + BG_AUDIENCE02_HEIGHT, 0.0f );				// 左下の頂点	
	pVtx[ 11 ].pos = D3DXVECTOR3( BG_AUDIENCE02_POS_X + BG_AUDIENCE_WIDTH, BG_AUDIENCE02_POS_Y + BG_AUDIENCE02_HEIGHT, 0.0f );				// 右下の頂点
	
	// 頂点座標の設定
	pVtx[ 12 ].pos = D3DXVECTOR3( Bg[ 3 ].fPos.x,				     BG_NUMBERS_POS_Y,					   0.0f );				// 左上の頂点
	pVtx[ 13 ].pos = D3DXVECTOR3( Bg[ 3 ].fPos.x + BG_NUMBERS_WIDTH, BG_NUMBERS_POS_Y,					   0.0f );				// 右上の頂点
	pVtx[ 14 ].pos = D3DXVECTOR3( Bg[ 3 ].fPos.x,					 BG_NUMBERS_POS_Y + BG_NUMBERS_HEIGHT, 0.0f );				// 左下の頂点	
	pVtx[ 15 ].pos = D3DXVECTOR3( Bg[ 3 ].fPos.x + BG_NUMBERS_WIDTH, BG_NUMBERS_POS_Y + BG_NUMBERS_HEIGHT, 0.0f );				// 右下の頂点

	// UV座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( Bg[ 0 ].fPosTexX,					  Bg[ 0 ].fPosTexY );							// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( Bg[ 0 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 0 ].fPosTexY );							// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( Bg[ 0 ].fPosTexX,					  Bg[ 0 ].fPosTexY + BG_TEXTURE_HEIGHT );		// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( Bg[ 0 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 0 ].fPosTexY + BG_TEXTURE_HEIGHT );		// 右下の頂点
	
	// UV座標の設定
	pVtx[ 4 ].tex = D3DXVECTOR2( Bg[ 1 ].fPosTexX,					  Bg[ 1 ].fPosTexY );							// 左上の頂点
	pVtx[ 5 ].tex = D3DXVECTOR2( Bg[ 1 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 1 ].fPosTexY );							// 右上の頂点
	pVtx[ 6 ].tex = D3DXVECTOR2( Bg[ 1 ].fPosTexX,					  Bg[ 1 ].fPosTexY + BG_TEXTURE_HEIGHT );		// 左下の頂点	
	pVtx[ 7 ].tex = D3DXVECTOR2( Bg[ 1 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 1 ].fPosTexY + BG_TEXTURE_HEIGHT );		// 右下の頂点
	
	pVtx[ 8 ].tex  = D3DXVECTOR2( Bg[ 2 ].fPosTexX,					   Bg[ 2 ].fPosTexY );							// 左上の頂点
	pVtx[ 9 ].tex  = D3DXVECTOR2( Bg[ 2 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 2 ].fPosTexY );							// 右上の頂点
	pVtx[ 10 ].tex = D3DXVECTOR2( Bg[ 2 ].fPosTexX,					   Bg[ 2 ].fPosTexY + BG_TEXTURE_HEIGHT );		// 左下の頂点	
	pVtx[ 11 ].tex = D3DXVECTOR2( Bg[ 2 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 2 ].fPosTexY + BG_TEXTURE_HEIGHT );		// 右下の頂点
	
	pVtx[ 12 ].tex = D3DXVECTOR2( Bg[ 3 ].fPosTexX,					   Bg[ 3 ].fPosTexY );							// 左上の頂点
	pVtx[ 13 ].tex = D3DXVECTOR2( Bg[ 3 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 3 ].fPosTexY );							// 右上の頂点
	pVtx[ 14 ].tex = D3DXVECTOR2( Bg[ 3 ].fPosTexX,					   Bg[ 3 ].fPosTexY + BG_TEXTURE_HEIGHT );		// 左下の頂点	
	pVtx[ 15 ].tex = D3DXVECTOR2( Bg[ 3 ].fPosTexX + BG_TEXTURE_WIDTH, Bg[ 3 ].fPosTexY + BG_TEXTURE_HEIGHT );		// 右下の頂点
	
	// 頂点バッファのアンロック
	g_pVtxBufferBG->Unlock();	
}

/*------------------------------------------------------------------------------
  　描画処理
------------------------------------------------------------------------------*/
void DrawBG( void )
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// ストリーム(パイプライン)設定
	pDevice->SetStreamSource( 0,						// 繋げるパイプの番号
							  g_pVtxBufferBG,			// ストリームの元となる頂点バッファの指定
							  0,						// オフセット(バイト)
							  sizeof( VERTEX_2D ) );	// ストライド値(ストライド量)

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_2D );
		
	for( int nCntBG = 0; nCntBG < BG_TEX_MAX; nCntBG++ )
	{
		// テクスチャの設定
		pDevice->SetTexture( 0, g_pTextureBG[ nCntBG ] );	

		// プリミティブの描画
		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,		// プリミティブの種類
								nCntBG * 4,					// オフセット(頂点数)
								2 );						// プリミティブ数
	}
}
