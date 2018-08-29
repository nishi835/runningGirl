/*==============================================================================

    Score処理 [Score.cpp]
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
#include "score.h"
#include "message.h"
#include "player.h"

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define SCORE_TEXTURENAME "data/TEXTURE/number05.png"			// テクスチャファイル名
#define SCORE_RED_TEXTURENAME "data/TEXTURE/number06.png"			// テクスチャファイル名
#define SCORE_TITLE_TEXTURENAME "data/TEXTURE/scoretitle.png"   // テクスチャファイル名
#define TIME_TITLE_TEXTURENAME "data/TEXTURE/timetitle.png"   // テクスチャファイル名
#define SCORE_POS_X ( 280.0f )									// Scoreの表示位置X
#define SCORE_POS_Y ( 0.0f )									// Scoreの表示位置Y
#define SCORE_WIDTH ( 25 )										// Scoreの幅
#define SCORE_HEIGHT ( 35 )										// Scoreの高さ
#define SCORE_TEXTURE_WIDTH ( 0.1f )							// Scoreのテクスチャ幅
#define SCORE_TEXTURE_HEIGHT ( 1.0f )							// Scoreのテクスチャ高さ
#define SCORE_INTERVAL ( 10 )									// Scoreの間隔
#define SCORE_MAX_DIGIT ( 4 )									// Scoreの最大桁数
#define SCORE_TITLE_POS_X ( 0.0f )								// Score見出しの表示位置X
#define SCORE_TITLE_POS_Y ( 0.0f )								// Score見出しの表示位置Y
#define SCORE_TITLE_WIDTH ( 150 )								// Score見出しの幅
#define SCORE_TITLE_HEIGHT ( 40 )								// Score見出しの高さ
#define TIME_POS_X ( 240.0f )									// Timeの表示位置X
#define TIME_POS_Y ( 450.0f )									// Timeの表示位置Y
#define TIME_TITLE_POS_X ( 0.0f )								// Time見出しの表示位置X
#define TIME_TITLE_POS_Y ( 450.0f )								// Time見出しの表示位置Y
#define TIME_MAX_DIGIT (3)										// Timeの最大桁数
#define LIMIT_TIME ( 130 )										// 制限タイム

/*------------------------------------------------------------------------------
  　構造体の宣言
------------------------------------------------------------------------------*/
// Scoreの構造体
typedef struct
{
	float fScore;		// スコア格納
	int   nTime;        // 残り時間格納
	float fSizeVar;     // 数字のサイズの変化量
}SCORE;

/*------------------------------------------------------------------------------
  　グローバル変数
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;			// テクスチャインターフェースのポインタ
LPDIRECT3DTEXTURE9 g_pTextureScoreRed = NULL;			// テクスチャインターフェースのポインタ
LPDIRECT3DTEXTURE9 g_pTextureScoreTitle = NULL;		// テクスチャインターフェースのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTimeTitle  = NULL;		// テクスチャインターフェースのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferScore = NULL;   // 頂点バッファ管理インターフェースポインタ

SCORE Score;										// Scoreの構造体
int   g_nTimer = 0; // 汎用タイマー

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
HRESULT MakeVertexScore( LPDIRECT3DDEVICE9 pDevice );

/*------------------------------------------------------------------------------
  　初期化処理
------------------------------------------------------------------------------*/
void InitScore( void )
{
	// Scoreの初期化
	Score.fScore = 0.0f;
	Score.nTime  = LIMIT_TIME;
	Score.fSizeVar = 0.0f;

	// 頂点の設定
	MakeVertexPolygon( &g_pVtxBufferScore, SCORE_MAX_DIGIT + TIME_MAX_DIGIT + 2 );
	
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					// デバイスのポインタを渡す
											SCORE_TEXTURENAME,			// テクスチャのファイル名
											&g_pTextureScore ) ) )		// テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );  // エラーチェック
	}
	
	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					// デバイスのポインタを渡す
											SCORE_RED_TEXTURENAME,			// テクスチャのファイル名
											&g_pTextureScoreRed ) ) )		// テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );  // エラーチェック
	}

	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					// デバイスのポインタを渡す
											SCORE_TITLE_TEXTURENAME,			// テクスチャのファイル名
											&g_pTextureScoreTitle ) ) )		// テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );  // エラーチェック
	}

	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					// デバイスのポインタを渡す
											TIME_TITLE_TEXTURENAME,			// テクスチャのファイル名
											&g_pTextureTimeTitle ) ) )		// テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );  // エラーチェック
	}

	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					// デバイスのポインタを渡す
											TIME_TITLE_TEXTURENAME,			// テクスチャのファイル名
											&g_pTextureTimeTitle ) ) )		// テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );  // エラーチェック
	}
}
/*------------------------------------------------------------------------------
  　終了処理
------------------------------------------------------------------------------*/
void UninitScore( void )
{
	// テクスチャインターフェースの解放
	if( g_pTextureScore != NULL )
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	// テクスチャインターフェースの解放
	if( g_pTextureScore != NULL )
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
		
	// テクスチャインターフェースの解放
	if( g_pTextureScoreTitle != NULL )
	{
		g_pTextureScoreTitle->Release();
		g_pTextureScoreTitle = NULL;
	}
	
	// テクスチャインターフェースの解放
	if( g_pTextureTimeTitle != NULL )
	{
		g_pTextureTimeTitle->Release();
		g_pTextureTimeTitle = NULL;
	}

	// 頂点バッファインターフェースの解放
	if( g_pVtxBufferScore != NULL )
	{
		g_pVtxBufferScore->Release();
		g_pVtxBufferScore = NULL;
	}
}

/*------------------------------------------------------------------------------
  　更新処理
------------------------------------------------------------------------------*/
void UpdateScore( void )
{
	VERTEX_2D* pVtx = NULL;   // VRAMの仮想アドレスを格納
	
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferScore->Lock( 0,
							 0,				   // すべてをロック
							 ( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
							 0 );			
	// 頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( SCORE_TITLE_POS_X ,					SCORE_TITLE_POS_Y,				        0.0f );	// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( SCORE_TITLE_POS_X + SCORE_TITLE_WIDTH, SCORE_TITLE_POS_Y,						0.0f );	// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( SCORE_TITLE_POS_X ,					SCORE_TITLE_POS_Y + SCORE_TITLE_HEIGHT, 0.0f );	// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( SCORE_TITLE_POS_X + SCORE_TITLE_WIDTH, SCORE_TITLE_POS_Y + SCORE_TITLE_HEIGHT, 0.0f );	// 右下の頂点

	pVtx += 4;
	
	// 頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( TIME_TITLE_POS_X ,				       TIME_TITLE_POS_Y,				      0.0f );	// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( TIME_TITLE_POS_X + SCORE_TITLE_WIDTH, TIME_TITLE_POS_Y,				  	  0.0f );	// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( TIME_TITLE_POS_X ,					   TIME_TITLE_POS_Y + SCORE_TITLE_HEIGHT, 0.0f );	// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( TIME_TITLE_POS_X + SCORE_TITLE_WIDTH, TIME_TITLE_POS_Y + SCORE_TITLE_HEIGHT, 0.0f );	// 右下の頂点
	
	pVtx += 4;
	
	int value = ( int )Score.fScore; // 分解用スコア格納
	int number;				  // スコアの数字

	for( int nCnt = 0; nCnt < SCORE_MAX_DIGIT; nCnt++ )
	{
		// スコア分解
		number = value % 10;			// 最下位桁の取り出し
		value /= 10;					// 最下位桁の更新
			
		// スコア数字のセット
		SetScoreNumber( pVtx, number, nCnt );
			
		pVtx += 4;
	}
	
	value = Score.nTime; // 分解用タイム格納
	for( int nCnt = 0; nCnt < TIME_MAX_DIGIT; nCnt++ )
	{
		// タイム分解
		number = value % 10;			// 最下位桁の取り出し
		value /= 10;					// 最下位桁の更新
			
		// タイム数字のセット
		SetTimeNumber( pVtx, number, nCnt );
			
		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_pVtxBufferScore->Unlock();

	// タイマーのデクリメント
	MESSAGE* pMessage = GetMessage();
	if( g_nTimer % 60 == 0 && pMessage->nType == MESSAGE_TYPE_GO )
	{
		Score.nTime--;
	}

	if( Score.nTime <= 10 && pMessage->nType == MESSAGE_TYPE_GO )
	{
		if( g_nTimer % 60 < 30 )
		{
			Score.fSizeVar += 0.3f;
		}
		else
		{
			Score.fSizeVar -= 0.3f;
		}
	}

	// タイマーが0になったら
	if( Score.nTime == 0 && ( pMessage->nType != MESSAGE_TYPE_TIMEUP && pMessage->nType != MESSAGE_TYPE_GAMEOVER ) )
	{
		SetPlayerState( STATE_TIMEUP );
		SetMessageType( MESSAGE_TYPE_TIMEUP );
	}

	// 汎用タイマーのインクリメント
	g_nTimer++;
}

/*------------------------------------------------------------------------------
  　描画処理
------------------------------------------------------------------------------*/
void DrawScore( void )
{
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// パイプライン（ストリーム）を設定
	pDevice->SetStreamSource( 0,						// パイプライン番号
							  g_pVtxBufferScore,		// パイプラインの元になる領域を指定
							  0,						// オフセット（単位はbyte）※ストリームの開始位置を指定できる
							  sizeof( VERTEX_2D ) );	// 流すデータの単位サイズ（ストライド量）

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_2D );

	// プリミティブ（Polygon）の描画
	for( int nCnt = 0; nCnt < SCORE_MAX_DIGIT + TIME_MAX_DIGIT + 2; nCnt++ )
	{
		// テクスチャの設定
		if( nCnt == 0 )
		{
			pDevice->SetTexture( 0, g_pTextureScoreTitle );
		}
		else if( nCnt == 1 )
		{
			pDevice->SetTexture( 0, g_pTextureTimeTitle );
		}
		else if( nCnt >= SCORE_MAX_DIGIT + 2 && Score.nTime < 10 )
		{
			pDevice->SetTexture( 0, g_pTextureScoreRed );
		}
		else
		{
			pDevice->SetTexture( 0, g_pTextureScore );
		}

		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, // プリミティブの種類
								nCnt * 4,			 // 描画開始位置のオフセット（頂点数）
								NUM_POLYGON );		 // 描画するプリミティブの数
	}
}

/*------------------------------------------------------------------------------
  　スコア数字の設定処理
------------------------------------------------------------------------------*/
void SetScoreNumber( VERTEX_2D* pVtx, int number, int index )
{
	// 頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( SCORE_POS_X - ( SCORE_WIDTH + SCORE_INTERVAL ) * index,			   SCORE_POS_Y,				   0.0f );	// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( SCORE_POS_X - ( SCORE_WIDTH + SCORE_INTERVAL ) * index + SCORE_WIDTH, SCORE_POS_Y,				   0.0f );	// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( SCORE_POS_X - ( SCORE_WIDTH + SCORE_INTERVAL ) * index,			   SCORE_POS_Y + SCORE_HEIGHT, 0.0f );	// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( SCORE_POS_X - ( SCORE_WIDTH + SCORE_INTERVAL ) * index + SCORE_WIDTH, SCORE_POS_Y + SCORE_HEIGHT, 0.0f );	// 右下の頂点

	// UV座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( number * SCORE_TEXTURE_WIDTH,						 0.0f );					// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( number * SCORE_TEXTURE_WIDTH + SCORE_TEXTURE_WIDTH, 0.0f );					// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( number * SCORE_TEXTURE_WIDTH,						 SCORE_TEXTURE_HEIGHT );	// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( number * SCORE_TEXTURE_WIDTH + SCORE_TEXTURE_WIDTH, SCORE_TEXTURE_HEIGHT );	// 右下の頂点
}

/*------------------------------------------------------------------------------
  　タイム数字の設定処理
------------------------------------------------------------------------------*/
void SetTimeNumber( VERTEX_2D* pVtx, int number, int index )
{
	// 頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( TIME_POS_X - ( SCORE_WIDTH + SCORE_INTERVAL + Score.fSizeVar ) * index,							    TIME_POS_Y + Score.fSizeVar,			    0.0f );	// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( TIME_POS_X - ( SCORE_WIDTH + SCORE_INTERVAL + Score.fSizeVar ) * index + SCORE_WIDTH + Score.fSizeVar, TIME_POS_Y + Score.fSizeVar,				0.0f );	// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( TIME_POS_X - ( SCORE_WIDTH + SCORE_INTERVAL + Score.fSizeVar ) * index,								TIME_POS_Y + Score.fSizeVar + SCORE_HEIGHT, 0.0f );	// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( TIME_POS_X - ( SCORE_WIDTH + SCORE_INTERVAL + Score.fSizeVar ) * index + SCORE_WIDTH + Score.fSizeVar, TIME_POS_Y + Score.fSizeVar + SCORE_HEIGHT, 0.0f );	// 右下の頂点

	// UV座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( number * SCORE_TEXTURE_WIDTH,						 0.0f );					// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( number * SCORE_TEXTURE_WIDTH + SCORE_TEXTURE_WIDTH, 0.0f );					// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( number * SCORE_TEXTURE_WIDTH,						 SCORE_TEXTURE_HEIGHT );	// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( number * SCORE_TEXTURE_WIDTH + SCORE_TEXTURE_WIDTH, SCORE_TEXTURE_HEIGHT );	// 右下の頂点
}

/*------------------------------------------------------------------------------
  　スコアのセッター
------------------------------------------------------------------------------*/
void SetScore( float fScore )
{
	Score.fScore += fScore;
}

float GetScore( void )
{
	return Score.fScore;
}