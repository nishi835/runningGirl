/*==============================================================================

    Timer処理 [Timer.cpp]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  　ヘッダファイル
------------------------------------------------------------------------------*/
#include "main.h"
#include "game.h"
#include "polygon.h"
#include "timer.h"

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define TIMER_TEXTURENAME "data/TEXTURE/number.png" // テクスチャファイル名
#define TIMER_POS_X ( 300.0f )							// Timerの表示位置X
#define TIMER_POS_Y ( 0.0f )							// Timerの表示位置Y
#define TIMER_WIDTH ( 50 )							// Timerの幅
#define TIMER_HEIGHT ( 75 )							// Timerの高さ
#define TIMER_INTERVAL ( 10 )						// Timerの間隔
#define TIMER_MAX_DIGIT ( 4 )						// Timerの最大桁数
#define TIMER_TEXTURE_WIDTH ( 0.1f )				// Timerのテクスチャ幅
#define TIMER_TEXTURE_HEIGHT ( 1.0f )				// Timerのテクスチャ高さ
#define TIMER_STAGE1 (36000)                         // Stage1のクリアタイム
#define TIMER_STAGE2 (3600)                         // Stage2のクリアタイム
#define TIMER_STAGE3 (3600)                         // Stage3のクリアタイム

/*------------------------------------------------------------------------------
  　構造体の宣言
------------------------------------------------------------------------------*/
// Timerの構造体
typedef struct
{
	float fTimer;		// タイマー格納
}TIMER;

/*------------------------------------------------------------------------------
  　グローバル変数
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureTimer = NULL;			// テクスチャインターフェースのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTimer = NULL;  // 頂点バッファ管理インターフェースポインタ

TIMER Timer;											// Timerの構造体

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
HRESULT MakeVertexTimer( LPDIRECT3DDEVICE9 pDevice );

/*------------------------------------------------------------------------------
  　初期化処理
------------------------------------------------------------------------------*/
void InitTimer( void )
{
	// Timerの初期化
	switch( GetStage() )
	{
	case 1:
		Timer.fTimer = TIMER_STAGE1;
		break;
	case 2:
		Timer.fTimer = TIMER_STAGE2;
		break;
	case 3:
		Timer.fTimer = TIMER_STAGE3;
		break;
	}

	// 頂点の設定
	MakeVertexPolygon( &g_pVtxBufferTimer, TIMER_MAX_DIGIT );
	
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					// デバイスのポインタを渡す
											TIMER_TEXTURENAME,			// テクスチャのファイル名
											&g_pTextureTimer ) ) )		// テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );  // エラーチェック
	}
}

/*------------------------------------------------------------------------------
  　終了処理
------------------------------------------------------------------------------*/
void UninitTimer( void )
{
	UninitPolygon( &g_pTextureTimer, &g_pVtxBufferTimer );
}

/*------------------------------------------------------------------------------
  　更新処理
------------------------------------------------------------------------------*/
void UpdateTimer( void )
{
	int value = ( int )Timer.fTimer; // 分解用タイマー格納
	int number;				  // タイマーの数字

	VERTEX_2D* pVtx = NULL;   // VRAMの仮想アドレスを格納
	
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferTimer->Lock( 0,
							 0,				   // すべてをロック
							 ( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
							 0 );			

	for( int nCnt = 0; nCnt < TIMER_MAX_DIGIT; nCnt++ )
	{
		// タイマー分解
		number = value % 10;			// 最下位桁の取り出し
		value /= 10;					// 最下位桁の更新
			
		// タイマー数字のセット
		SetTimerNumber( pVtx, number, nCnt );
			
		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_pVtxBufferTimer->Unlock();
	
	// タイマーのインクリメント
	Timer.fTimer++;
}

/*------------------------------------------------------------------------------
  　描画処理
------------------------------------------------------------------------------*/
void DrawTimer( void )
{
	DrawPolygon( g_pTextureTimer, g_pVtxBufferTimer, TIMER_MAX_DIGIT );
}

/*------------------------------------------------------------------------------
  　タイマー数字の設定処理
------------------------------------------------------------------------------*/
void SetTimerNumber( VERTEX_2D* pVtx, int number, int index )
{
	// 頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( TIMER_POS_X - ( TIMER_WIDTH + TIMER_INTERVAL ) * index,			   TIMER_POS_Y,				   0.0f );	// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( TIMER_POS_X - ( TIMER_WIDTH + TIMER_INTERVAL ) * index + TIMER_WIDTH, TIMER_POS_Y,				   0.0f );	// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( TIMER_POS_X - ( TIMER_WIDTH + TIMER_INTERVAL ) * index,			   TIMER_POS_Y + TIMER_HEIGHT, 0.0f );	// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( TIMER_POS_X - ( TIMER_WIDTH + TIMER_INTERVAL ) * index + TIMER_WIDTH, TIMER_POS_Y + TIMER_HEIGHT, 0.0f );	// 右下の頂点

	// UV座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( number * TIMER_TEXTURE_WIDTH,						 0.0f );					// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( number * TIMER_TEXTURE_WIDTH + TIMER_TEXTURE_WIDTH, 0.0f );					// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( number * TIMER_TEXTURE_WIDTH,						 TIMER_TEXTURE_HEIGHT );	// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( number * TIMER_TEXTURE_WIDTH + TIMER_TEXTURE_WIDTH, TIMER_TEXTURE_HEIGHT );	// 右下の頂点
}

/*------------------------------------------------------------------------------
  　タイマーのセッター
------------------------------------------------------------------------------*/
void SetTimer( float fTimer )
{
	Timer.fTimer = fTimer;
}

/*------------------------------------------------------------------------------
  　タイマーのゲッター
------------------------------------------------------------------------------*/
int GetTimer( void )
{
	return Timer.fTimer;
}