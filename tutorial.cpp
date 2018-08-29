/*==============================================================================

    tutorialの処理 [tutorial.cpp]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  　ヘッダファイル
------------------------------------------------------------------------------*/
#include "tutorial.h"
#include "main.h"
#include "bg.h"
#include "input.h"
#include "movebg.h"
#include "player.h"
#include "can.h"
#include "hurdle.h"
#include "goal.h"
#include "jumper.h"
#include "shadow.h"
#include "hiteffect.h"
#include "dasheffect.h"
#include "particle.h"
#include "score.h"
#include "message.h"
#include "item.h"
#include "game.h"
#include "polygon.h"
#include "input.h"
#include "fade.h"
#include <stdio.h>

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define TUTORIAL_WINDOW_TEXTURENAME "data/TEXTURE/tutorialwindows.png" // テクスチャファイル名
#define TUTORIAL_WINDOW_POS_X ( 100 )								// TutorialMessageの表示位置X
#define TUTORIAL_WINDOW_POS_Y ( 300 )								// TutorialMessageの表示位置Y
#define TUTORIAL_WINDOW_WIDTH ( 600 )								// TutorialMessageの幅
#define TUTORIAL_WINDOW_HEIGHT ( 250 )								// TutorialMessageの高さ
#define TUTORIAL_WINDOW_TEXTUER_U ( 0.0 )							// TutorialMessageのテクスチャU値
#define TUTORIAL_WINDOW_TEXTUER_V ( 0.0 )							// TutorialMessageのテクスチャV値
#define TUTORIAL_WINDOW_TEXTURE_WIDTH ( 1.0f )						// TutorialMessageのテクスチャ幅
#define TUTORIAL_WINDOW_TEXTURE_HEIGHT ( 0.33f )					// TutorialMessageのテクスチャ高さ

#define TUTORIAL_WINDOW_TIME (300)						    // タイトルの表示時間

/*------------------------------------------------------------------------------
  　構造体
------------------------------------------------------------------------------*/
typedef struct
{
	int Phase;
	int Timer;
	int StopTimer;
	bool Move;
}TUTORIAL;

/*------------------------------------------------------------------------------
  　グローバル変数
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureTutorial = NULL;					// テクスチャインターフェースのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTutorial = NULL;			// 頂点バッファ管理インターフェースポインタ

TUTORIAL Tutorial;

/*------------------------------------------------------------------------------
  　初期化処理
------------------------------------------------------------------------------*/
void InitTutorial( void )
{
	// 初期化
	Tutorial.Phase = 0;
	Tutorial.Timer = 0;
	Tutorial.StopTimer = 0;
	Tutorial.Move = false;

	InitBG();
	InitMoveBG();
	InitScore();	
	InitShadow();
	InitPlayer();

	InitCan();
	InitJumper();
	InitHurdle();
	InitGoal();
	InitItem();

	InitParticle();
	InitHitEffect();
	InitDashEffect();

	// ロード
	LoadTutorial();

	// 頂点の設定
	MakeVertexPolygon( &g_pVtxBufferTutorial, 1 );
	
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,						 // デバイスのポインタを渡す
											TUTORIAL_WINDOW_TEXTURENAME,			     // テクスチャのファイル名
											&g_pTextureTutorial ) ) )			 // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );		 // エラーチェック
	}

	VERTEX_2D* pVtx = NULL; // VRAMの仮想アドレスを格納
	
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferTutorial->Lock( 0,
							 0,					// すべてをロック
						     ( void** )&pVtx,   // 仮想アドレス用ポインタのアドレスを渡す
							 0 );			

	// 頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( TUTORIAL_WINDOW_POS_X,  TUTORIAL_WINDOW_POS_Y, 0.0f );									// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( TUTORIAL_WINDOW_POS_X + TUTORIAL_WINDOW_WIDTH,  TUTORIAL_WINDOW_POS_Y, 0.0f );					// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( TUTORIAL_WINDOW_POS_X,  TUTORIAL_WINDOW_POS_Y + TUTORIAL_WINDOW_HEIGHT, 0.0f );					// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( TUTORIAL_WINDOW_POS_X + TUTORIAL_WINDOW_WIDTH,  TUTORIAL_WINDOW_POS_Y + TUTORIAL_WINDOW_HEIGHT, 0.0f );	// 右下の頂点
	
	// UV座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( TUTORIAL_WINDOW_TEXTUER_U,									TUTORIAL_WINDOW_TEXTUER_V );												// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( TUTORIAL_WINDOW_TEXTUER_U + TUTORIAL_WINDOW_TEXTURE_WIDTH, TUTORIAL_WINDOW_TEXTUER_V );							// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( TUTORIAL_WINDOW_TEXTUER_U,									TUTORIAL_WINDOW_TEXTUER_V + TUTORIAL_WINDOW_TEXTURE_HEIGHT );						// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( TUTORIAL_WINDOW_TEXTUER_U + TUTORIAL_WINDOW_TEXTURE_WIDTH, TUTORIAL_WINDOW_TEXTUER_V + TUTORIAL_WINDOW_TEXTURE_HEIGHT );	// 右下の頂点

	// 頂点バッファのアンロック
	g_pVtxBufferTutorial->Unlock();	
}

/*------------------------------------------------------------------------------
  　終了処理
------------------------------------------------------------------------------*/
void UninitTutorial( void )
{
	UninitPolygon( &g_pTextureTutorial, &g_pVtxBufferTutorial );
	
	UninitBG();
	UninitMoveBG();
	UninitPlayer();
	UninitScore();

	UninitCan();
	UninitJumper();
	UninitHurdle();
	UninitGoal();
	UninitItem();
}

/*------------------------------------------------------------------------------
  　更新処理
------------------------------------------------------------------------------*/
void UpdateTutorial( void )
{
	// キーボードの更新
	UpdateKeyboard();
	
	if( Tutorial.Move )
	{
		UpdateBG();
		UpdateMoveBG();
		UpdatePlayer();
		UpdateScore();

		UpdateCan();
		UpdateJumper();
		UpdateHurdle();
		UpdateGoal();
		UpdateItem();
	}
	
	if( Tutorial.Phase == 0 )
	{
		Tutorial.StopTimer++;
		if( Tutorial.StopTimer > 180 )
		{
			Tutorial.Phase++;
			Tutorial.StopTimer = 0;
		}
	}

	if( Tutorial.Phase == 1 )
	{
		Tutorial.StopTimer++;
		if( Tutorial.StopTimer > 180 )
		{
			Tutorial.Phase++;
			Tutorial.StopTimer = 0;
			Tutorial.Move = true;
		}
	}

	if( Tutorial.Timer > 540 && Tutorial.Phase == 2 )
	{
		Tutorial.Move = false;
	}
	
	if( Tutorial.Phase == 2 && !Tutorial.Move )
	{
		Tutorial.StopTimer++;
		if( Tutorial.StopTimer > 180 )
		{
			Tutorial.Phase++;
			Tutorial.StopTimer = 0;
			Tutorial.Move = true;
		}
	}

	if( Tutorial.Timer == 600 )
	{
		MovePlayerPos( D3DXVECTOR2( 0.0f, 60.0f ) );
	}	
	
	if( Tutorial.Timer == 950 )
	{
		MovePlayerPos( D3DXVECTOR2( 0.0f, -60.0f ) );
	}	

	VERTEX_2D* pVtx = NULL; // VRAMの仮想アドレスを格納
	
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferTutorial->Lock( 0,
								0,					// すべてをロック
								( void** )&pVtx,   // 仮想アドレス用ポインタのアドレスを渡す
								0 );			
	
	// UV座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( TUTORIAL_WINDOW_TEXTUER_U,									TUTORIAL_WINDOW_TEXTUER_V + Tutorial.Phase	       * TUTORIAL_WINDOW_TEXTURE_HEIGHT );	// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( TUTORIAL_WINDOW_TEXTUER_U + TUTORIAL_WINDOW_TEXTURE_WIDTH, TUTORIAL_WINDOW_TEXTUER_V + Tutorial.Phase		   * TUTORIAL_WINDOW_TEXTURE_HEIGHT );	// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( TUTORIAL_WINDOW_TEXTUER_U,									TUTORIAL_WINDOW_TEXTUER_V + ( Tutorial.Phase + 1 ) * TUTORIAL_WINDOW_TEXTURE_HEIGHT );	// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( TUTORIAL_WINDOW_TEXTUER_U + TUTORIAL_WINDOW_TEXTURE_WIDTH, TUTORIAL_WINDOW_TEXTUER_V + ( Tutorial.Phase + 1 ) * TUTORIAL_WINDOW_TEXTURE_HEIGHT );	// 右下の頂点

	// 頂点バッファのアンロック
	g_pVtxBufferTutorial->Unlock();	
	
	// タイマーのインクリメント
	Tutorial.Timer++;
}

/*------------------------------------------------------------------------------
  　描画処理
------------------------------------------------------------------------------*/
void DrawTutorial( void )
{
	DrawBG();
	DrawMoveBG();
	DrawPlayer();	

	DrawCan();
	DrawJumper();
	DrawHurdle();
	DrawGoal();
	DrawItem();

	if( !Tutorial.Move )
	{
		DrawPolygon( g_pTextureTutorial, g_pVtxBufferTutorial, 1 );
	}

	DrawControl();
}

void LoadTutorial( void )
{
	// 情報取得
	CAN* pCan = GetCan();
	HURDLE* pHurdle = GetHurdle();
	JUMPER* pJumper = GetJumper();
	GOAL* pGoal = GetGoal();
	ITEM* pItem = GetItem();

	// NULLチェック
	if( pCan != NULL && pHurdle != NULL && pJumper != NULL && pGoal != NULL && pItem != NULL )
	{
		char aTemp[ 256 ] = {};
		sprintf( aTemp, "data\\tutorial.bin" );
		FILE* fp;
		fp = fopen( aTemp, "rb" );
		if( fp == NULL )
		{
			MessageBox( NULL, "バイナリファイルオープン失敗", "エラー", MB_OK );
			return;
		}

		fread( pCan, sizeof( CAN ), CAN_MAX, fp );
		fread( pHurdle, sizeof( HURDLE ), HURDLE_MAX, fp );
		fread( pJumper, sizeof( JUMPER ), JUMPER_MAX, fp );
		fread( pGoal, sizeof( GOAL ), 1, fp );
		fread( pItem, sizeof( HURDLE ), HURDLE_MAX, fp );

		fclose( fp );
	}
}

// ゲッター
int GetTutorialTimer( void )
{
	return Tutorial.Timer;
}