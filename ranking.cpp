/*==============================================================================

    Ranking処理 [Ranking.cpp]
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
#include "ranking.h"
#include "player.h"
#include "input.h"
#include "fade.h"
#include "score.h"
#include "game.h"
#include <stdio.h>

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define RANKING_TEXTURENAME "data/TEXTURE/number05.png"		 // テクスチャファイル名
#define RANKING_TEXTURENAME02 "data/TEXTURE/ranking.jpg"	 // テクスチャファイル名
#define RANKING_TEXTURENAME03 "data/TEXTURE/all.png"	 // テクスチャファイル名
#define RANKING_MAX (5)									     // Rankingの総数
#define RANKING_POS_X ( 450 )							     // Rankingの表示位置X
#define RANKING_POS_Y ( 230 )							     // Rankingの表示位置Y
#define RANKING_STAGE_POS_X ( RANKING_POS_X + 180 )			 // Rankingのステージ数の表示位置X
#define RANKING_STAGE_POS_Y ( 180 )							 // Rankingのステージ数の表示位置Y
#define RANKING_HEIGHT (68)									 // Rankingの高さ
#define RANKING_NUM_WIDTH ( 30 )							 // Rankingの数字の幅
#define RANKING_NUM_HEIGHT ( 35 )							 // Rankingの数字の高さ
#define RANKING_ALL_WIDTH ( 70 )							 // RankingのALLの幅
#define RANKING_ALL_HEIGHT ( 30 )							 // RankingのALLの高さ
#define RANKING_INTERVAL ( 10 )								 // Rankingの間隔
#define RANKING_MAX_DIGIT ( 4 )								 // Rankingの最大桁数
#define RANKING_TEXTURE_WIDTH ( 0.1f )						 // Rankingのテクスチャ幅
#define RANKING_TEXTURE_HEIGHT ( 1.0f )						 // Rankingのテクスチャ高さ

/*------------------------------------------------------------------------------
  　グローバル変数
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureRanking = NULL;		  	 // テクスチャインターフェースのポインタ
LPDIRECT3DTEXTURE9 g_pTextureRankingBG = NULL;			 // テクスチャインターフェースのポインタ
LPDIRECT3DTEXTURE9 g_pTextureRankingAll = NULL;			 // テクスチャインターフェースのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferRanking = NULL;		 // 頂点バッファ管理インターフェースポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferRankingBG = NULL;    // 頂点バッファ管理インターフェースポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferRankingAll = NULL;		 // 頂点バッファ管理インターフェースポインタ

RANKING Ranking[5];										 // Rankingの構造体
bool g_bRankIn = true;									 // ランクインしたかどうか
int g_Timer = 0;										 // タイマー

/*------------------------------------------------------------------------------
  　初期化処理
------------------------------------------------------------------------------*/
void InitRanking( void )
{
	for( int nCnt = 0; nCnt < 5; nCnt++ )
	{
		Ranking[ nCnt ].nStage = 0;
	}
	g_Timer = 0;

	// 頂点の設定
	MakeVertexPolygon( &g_pVtxBufferRanking, ( RANKING_MAX_DIGIT + 1 ) * RANKING_MAX );
	MakeVertexPolygon( &g_pVtxBufferRankingBG, 1 );
	MakeVertexPolygon( &g_pVtxBufferRankingAll, RANKING_MAX );
	
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					// デバイスのポインタを渡す
											RANKING_TEXTURENAME,			// テクスチャのファイル名
											&g_pTextureRanking ) ) )		// テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );  // エラーチェック
	}
	
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,						// デバイスのポインタを渡す
											RANKING_TEXTURENAME02,			// テクスチャのファイル名
											&g_pTextureRankingBG ) ) )		// テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );  // エラーチェック
	}
	
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,						// デバイスのポインタを渡す
											RANKING_TEXTURENAME03,			// テクスチャのファイル名
											&g_pTextureRankingAll ) ) )		// テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );  // エラーチェック
	}
	
	// ランキングの作成
	g_bRankIn = false;
	LoadRanking();
}

/*------------------------------------------------------------------------------
  　終了処理
------------------------------------------------------------------------------*/
void UninitRanking( void )
{
	UninitPolygon( &g_pTextureRanking, &g_pVtxBufferRanking );
	UninitPolygon( &g_pTextureRankingBG, &g_pVtxBufferRankingBG );
	UninitPolygon( &g_pTextureRankingAll, &g_pVtxBufferRankingAll );
}

/*------------------------------------------------------------------------------
  　更新処理
------------------------------------------------------------------------------*/
void UpdateRanking( void )
{
	// キーボード更新
	UpdateKeyboard();

	// ランキング数字のセット
	SetRankingNumber();

	// モード切替
   	if( GetKeyboardTrigger( DIK_SPACE ) && g_Timer > 120 )
	{
		SetFade( FADE_OUT, MODE_TITLE );
	}

	g_Timer++;
}

/*------------------------------------------------------------------------------
  　ランキング描画処理
------------------------------------------------------------------------------*/
void DrawRanking( void )
{
	DrawPolygon( g_pTextureRankingBG, g_pVtxBufferRankingBG, 1 );
	DrawPolygon( g_pTextureRanking, g_pVtxBufferRanking, ( RANKING_MAX_DIGIT + 1 ) * RANKING_MAX );
	DrawPolygon( g_pTextureRankingAll, g_pVtxBufferRankingAll, RANKING_MAX );
} 

/*------------------------------------------------------------------------------
  　ランキング数字の設定処理
------------------------------------------------------------------------------*/
void SetRankingNumber( void )
{
	// 背景の準備
	VERTEX_2D* pVtx = NULL; // VRAMの仮想アドレスを格納
	
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferRankingBG->Lock( 0,
							     0,				// すべてをロック
							     ( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
							     0 );			
	
	// 頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );		// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( 800.0f, 0.0f, 0.0f );		// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( 0.0f, 600.0f, 0.0f );		// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( 800.0f, 600.0f, 0.0f );	// 右下の頂点
	
	// UV座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );				// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );				// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );				// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );				// 右下の頂点

	// 頂点バッファのアンロック
	g_pVtxBufferRankingBG->Unlock();	
	
	// ランキング数字の準備

	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferRanking->Lock( 0,
							   0,				 // すべてをロック
							   ( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
							   0 );			
		
	for( int nCntRank = 0; nCntRank < RANKING_MAX; nCntRank++ )
	{
		int value = ( int )Ranking[ nCntRank ].nScore; // 分解用ランキング格納
		int number;									   // ランキングの数字

		for( int nCnt = 0; nCnt < RANKING_MAX_DIGIT; nCnt++ )
		{
			// ランキング分解
			number = value % 10;			// 最下位桁の取り出し
			value *= 0.1;					// 最下位桁の更新

			// 頂点座標の設定			  
			pVtx[ 0 ].pos = D3DXVECTOR3( RANKING_POS_X - ( RANKING_NUM_WIDTH + RANKING_INTERVAL ) * nCnt,					  RANKING_POS_Y 					 + RANKING_HEIGHT * nCntRank, 0.0f );	// 左上の頂点
			pVtx[ 1 ].pos = D3DXVECTOR3( RANKING_POS_X - ( RANKING_NUM_WIDTH + RANKING_INTERVAL ) * nCnt + RANKING_NUM_WIDTH, RANKING_POS_Y 					 + RANKING_HEIGHT * nCntRank, 0.0f );	// 右上の頂点
			pVtx[ 2 ].pos = D3DXVECTOR3( RANKING_POS_X - ( RANKING_NUM_WIDTH + RANKING_INTERVAL ) * nCnt,					  RANKING_POS_Y + RANKING_NUM_HEIGHT + RANKING_HEIGHT * nCntRank, 0.0f );	// 左下の頂点	
			pVtx[ 3 ].pos = D3DXVECTOR3( RANKING_POS_X - ( RANKING_NUM_WIDTH + RANKING_INTERVAL ) * nCnt + RANKING_NUM_WIDTH, RANKING_POS_Y + RANKING_NUM_HEIGHT + RANKING_HEIGHT * nCntRank, 0.0f );	// 右下の頂点

			// UV座標の設定
			pVtx[ 0 ].tex = D3DXVECTOR2( number * RANKING_TEXTURE_WIDTH,						 0.0f );					// 左上の頂点
			pVtx[ 1 ].tex = D3DXVECTOR2( number * RANKING_TEXTURE_WIDTH + RANKING_TEXTURE_WIDTH, 0.0f );					// 右上の頂点
			pVtx[ 2 ].tex = D3DXVECTOR2( number * RANKING_TEXTURE_WIDTH,						 RANKING_TEXTURE_HEIGHT );	// 左下の頂点	
			pVtx[ 3 ].tex = D3DXVECTOR2( number * RANKING_TEXTURE_WIDTH + RANKING_TEXTURE_WIDTH, RANKING_TEXTURE_HEIGHT );	// 右下の頂点

			// 頂点カラーの設定（ 0 ～ 255の整数値 )
			if( /*g_bRankIn*/ 1)
			{
				static int nAlpha = 0;
				nAlpha++;

				pVtx[ 0 ].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );	// 左上の頂点
				pVtx[ 1 ].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );	// 右上の頂点
				pVtx[ 2 ].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );	// 左下の頂点
				pVtx[ 3 ].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );	// 右下の頂点
			}

			pVtx += 4;
		}

		// 頂点座標の設定			  
		pVtx[ 0 ].pos = D3DXVECTOR3( RANKING_STAGE_POS_X,					  RANKING_POS_Y 		  			 + RANKING_HEIGHT * nCntRank, 0.0f );	// 左上の頂点
		pVtx[ 1 ].pos = D3DXVECTOR3( RANKING_STAGE_POS_X + RANKING_NUM_WIDTH, RANKING_POS_Y 					 + RANKING_HEIGHT * nCntRank, 0.0f );	// 右上の頂点
		pVtx[ 2 ].pos = D3DXVECTOR3( RANKING_STAGE_POS_X,					  RANKING_POS_Y + RANKING_NUM_HEIGHT + RANKING_HEIGHT * nCntRank, 0.0f );	// 左下の頂点	
		pVtx[ 3 ].pos = D3DXVECTOR3( RANKING_STAGE_POS_X + RANKING_NUM_WIDTH, RANKING_POS_Y + RANKING_NUM_HEIGHT + RANKING_HEIGHT * nCntRank, 0.0f );	// 右下の頂点

		// UV座標の設定
		pVtx[ 0 ].tex = D3DXVECTOR2( Ranking[ nCntRank ].nStage * RANKING_TEXTURE_WIDTH,						 0.0f );					// 左上の頂点
		pVtx[ 1 ].tex = D3DXVECTOR2( Ranking[ nCntRank ].nStage * RANKING_TEXTURE_WIDTH + RANKING_TEXTURE_WIDTH, 0.0f );					// 右上の頂点
		pVtx[ 2 ].tex = D3DXVECTOR2( Ranking[ nCntRank ].nStage * RANKING_TEXTURE_WIDTH,						 RANKING_TEXTURE_HEIGHT );	// 左下の頂点	
		pVtx[ 3 ].tex = D3DXVECTOR2( Ranking[ nCntRank ].nStage * RANKING_TEXTURE_WIDTH + RANKING_TEXTURE_WIDTH, RANKING_TEXTURE_HEIGHT );	// 右下の頂点

		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_pVtxBufferRanking->Unlock();
		
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferRankingAll->Lock( 0,
							   0,				 // すべてをロック
							   ( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
							   0 );			

	for( int nCntRank = 0; nCntRank < RANKING_MAX; nCntRank++ )
	{	
		if( Ranking[ nCntRank ].nStage >= STAGE_MAX )
		{
			// 頂点座標の設定			  
			pVtx[ 0 ].pos = D3DXVECTOR3( RANKING_STAGE_POS_X,					  RANKING_POS_Y 		  			 + RANKING_HEIGHT * nCntRank, 0.0f );	// 左上の頂点
			pVtx[ 1 ].pos = D3DXVECTOR3( RANKING_STAGE_POS_X + RANKING_ALL_WIDTH, RANKING_POS_Y 					 + RANKING_HEIGHT * nCntRank, 0.0f );	// 右上の頂点
			pVtx[ 2 ].pos = D3DXVECTOR3( RANKING_STAGE_POS_X,					  RANKING_POS_Y + RANKING_NUM_HEIGHT + RANKING_HEIGHT * nCntRank, 0.0f );	// 左下の頂点	
			pVtx[ 3 ].pos = D3DXVECTOR3( RANKING_STAGE_POS_X + RANKING_ALL_WIDTH, RANKING_POS_Y + RANKING_NUM_HEIGHT + RANKING_HEIGHT * nCntRank, 0.0f );	// 右下の頂点

			// UV座標の設定
			pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );	// 左上の頂点
			pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );	// 右上の頂点
			pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );	// 左下の頂点	
			pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );	// 右下の頂点

			pVtx += 4;
		}
	}

	// 頂点バッファのアンロック
	g_pVtxBufferRankingAll->Unlock();
}

/*------------------------------------------------------------------------------
  　ランキングの処理
------------------------------------------------------------------------------*/
void MakeRanking( void )
{
	// ランク判定
	int nScore = ( int )GetScore();
	int nRank = 0;
	bool bRankIn = false;
	int nCnt = RANKING_MAX - 1;
	while( 1 )
	{
		if( Ranking[ nCnt ].nScore > nScore )
		{
			nRank = nCnt + 1;
			break;
		}
		nCnt--;
		if( nCnt < 0 )
		{
			nRank = 0;
			break;
		}
	}

	// ランクソート
	if( nRank <= RANKING_MAX - 1 )
	{
		g_bRankIn = true;
		Ranking[ RANKING_MAX - 1 ].nScore = nScore;
		Ranking[ RANKING_MAX - 1 ].nStage = GetStage();

		for( int nCnt = RANKING_MAX - 1; nCnt > nRank; nCnt-- )
		{
			int work = Ranking[ nCnt - 1 ].nScore;
			Ranking[ nCnt - 1 ].nScore = Ranking[ nCnt ].nScore;
			Ranking[ nCnt ].nScore = work;

			work = Ranking[ nCnt - 1 ].nStage;
			Ranking[ nCnt - 1 ].nStage = Ranking[ nCnt ].nStage;
			Ranking[ nCnt ].nStage = work;
		}
	}
}

/*------------------------------------------------------------------------------
  　ランキングのセーブ
------------------------------------------------------------------------------*/
void SaveRanking( void )
{
	FILE* fp = NULL;
	if( ( fp = fopen( "data\\ranking.txt", "w" ) ) == NULL ) 
	{
		MessageBox( NULL, "ファイルオープンできません(セーブ）", "エラー", MB_OK );
	}
	else
	{
		for( int nCnt = 0; nCnt < RANKING_MAX; nCnt++ )
		{
			fprintf( fp, "%d\n", Ranking[ nCnt ].nScore );
			fprintf( fp, "%d\n", Ranking[ nCnt ].nStage );
		}

		fclose( fp );
	}
}

/*------------------------------------------------------------------------------
  　ランキングのロード
------------------------------------------------------------------------------*/
void LoadRanking( void )
{	
	FILE* fp = NULL;
	if( ( fp = fopen( "data\\ranking.txt", "r" ) ) == NULL ) 
	{
		MessageBox( NULL, "ファイルオープンできません(ロード）", "エラー", MB_OK );
	}
	else
	{
		for( int nCnt = 0; nCnt < RANKING_MAX; nCnt++ )
		{
			fscanf( fp, "%d", &Ranking[nCnt].nScore );
			fscanf( fp, "%d", &Ranking[nCnt].nStage );
		}

		fclose( fp );
	}
}

/*------------------------------------------------------------------------------
  　ゲッター
------------------------------------------------------------------------------*/
bool GetRankIn( void )
{	
	return g_bRankIn;
}