/*==============================================================================

    demo処理 [demo.cpp]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  　ヘッダファイル
------------------------------------------------------------------------------*/
#include "demo.h"
#include "main.h"
#include "polygon.h"
#include "input.h"
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
#include "sound.h"

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define DEMO_TEXTURENAME	  "data/TEXTURE/demoplay.png"					 // テクスチャファイル名
#define DEMO_WIDTH			  ( 200 )										 // Demoの幅
#define DEMO_HEIGHT			  ( 50 )										 // Demoの高さ
#define DEMO_POS_X			  ( SCREEN_WIDTH * 0.5f - DEMO_WIDTH * 0.5f )	 // Demoの表示位置X
#define DEMO_POS_Y			  ( SCREEN_HEIGHT * 0.5f - DEMO_HEIGHT * 0.5f )	 // Demoの表示位置Y
#define DEMO_TEXTUER_U		  ( 0.0 )										 // DemoのテクスチャU値
#define DEMO_TEXTUER_V		  ( 0.0 )										 // DemoのテクスチャV値
#define DEMO_TEXTURE_WIDTH    ( 1.0f )										 // Demoのテクスチャ幅
#define DEMO_TEXTURE_HEIGHT   ( 1.0f )										 // Demoのテクスチャ高さ
#define DEMO_TIME			  (900)											 // Demoの時間
#define DEMO_MESSAGE_INTERVAL (60)											 // Demoメッセージの点滅間隔

/*------------------------------------------------------------------------------
  　グローバル変数
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureDemo = NULL;					// テクスチャインターフェースのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferDemo = NULL;			// 頂点バッファ管理インターフェースポインタ

/*------------------------------------------------------------------------------
  　初期化処理
------------------------------------------------------------------------------*/
void InitDemo( void )
{
	// 頂点の設定
	MakeVertexPolygon( &g_pVtxBufferDemo, 1 );
	
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,						 // デバイスのポインタを渡す
											DEMO_TEXTURENAME,			     // テクスチャのファイル名
											&g_pTextureDemo ) ) )			 // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );		 // エラーチェック
	}

	// 背景の初期化
	InitBG();

	// スクロール背景の初期化
	InitMoveBG();

	// スコアの初期化
	InitScore();
	
	// メッセージの初期化
	InitMessage();
	
	// プレイヤーの初期化
	InitPlayer();

	// 丸影の初期化
	InitShadow();

	// 缶の初期化
	InitCan();

	// ジャンプ台の初期化
	InitJumper();

	// ハードルの初期化
	InitHurdle();
	
	// ゴールの初期化
	InitGoal();
	
	// アイテムの初期化
	InitItem();
	
	// パーティクルの初期化
	InitParticle();
	
	// ヒットエフェクトの初期化
	InitHitEffect();
	
	// ダッシュエフェクトの初期化
	InitDashEffect();
}

/*------------------------------------------------------------------------------
  　終了処理
------------------------------------------------------------------------------*/
void UninitDemo( void )
{
	UninitPolygon( &g_pTextureDemo, &g_pVtxBufferDemo );

	// BGMをoff
    StopSound( SOUND_LABEL_BGM000 );

	// 背景の終了処理
	UninitBG();
	
	// スクロール背景の終了処理
	UninitMoveBG();

	// スコアの終了処理
	UninitScore();
	
	// メッセージの終了処理
	UninitMessage();

	// プレイヤーの終了処理
	UninitPlayer();

	// 丸影の終了処理
	UninitShadow();

	// 缶の終了処理
	UninitCan();

	// ジャンプ台の終了処理
	UninitJumper();

	// ハードルの終了処理
	UninitHurdle();	
	
	// ゴールの終了処理
	UninitGoal();	

	// アイテムの終了処理
	UninitItem();
	
	// パーティクルの終了処理
	UninitParticle();

	// ヒットエフェクトの終了処理
	UninitHitEffect();
	
	// ダッシュエフェクトの終了処理
	UninitDashEffect();
}

/*------------------------------------------------------------------------------
  　更新処理
------------------------------------------------------------------------------*/
void UpdateDemo( void )
{
	// タイマーの設定
	static int nTimer = 0;

	// キーボードの更新
	UpdateKeyboard();

	VERTEX_2D* pVtx = NULL; // VRAMの仮想アドレスを格納
	
	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferDemo->Lock( 0,
							0,					// すべてをロック
						    ( void** )&pVtx,    // 仮想アドレス用ポインタのアドレスを渡す
							0 );			

	// 頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( DEMO_POS_X,			   DEMO_POS_Y,				 0.0f );	// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( DEMO_POS_X + DEMO_WIDTH,  DEMO_POS_Y,				 0.0f );	// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( DEMO_POS_X,			   DEMO_POS_Y + DEMO_HEIGHT, 0.0f );	// 左下の頂点	
	pVtx[ 3 ].pos = D3DXVECTOR3( DEMO_POS_X + DEMO_WIDTH,  DEMO_POS_Y + DEMO_HEIGHT, 0.0f );	// 右下の頂点
	
	// UV座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( DEMO_TEXTUER_U,					  DEMO_TEXTUER_V );							// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( DEMO_TEXTUER_U + DEMO_TEXTURE_WIDTH, DEMO_TEXTUER_V );							// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( DEMO_TEXTUER_U,					  DEMO_TEXTUER_V + DEMO_TEXTURE_HEIGHT );	// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( DEMO_TEXTUER_U + DEMO_TEXTURE_WIDTH, DEMO_TEXTUER_V + DEMO_TEXTURE_HEIGHT );	// 右下の頂点

	// 頂点バッファのアンロック
	g_pVtxBufferDemo->Unlock();	

	// メッセージの情報取得
	MESSAGE* pMessage = GetMessage();

	switch( pMessage->nType )
	{ 
	case MESSAGE_TYPE_STAGE:
	case MESSAGE_TYPE_READY:
		// 背景の更新
		//UpdateBG();

		// スコアの更新
		UpdateScore();

		// メッセージの更新
		UpdateMessage();

		// 丸影の更新
		UpdateShadow();
	
		// キーボードの更新
		UpdateKeyboard();

		break;

	case MESSAGE_TYPE_GO:
	case MESSAGE_TYPE_GOAL:
		// 背景の更新
		UpdateBG();

		// スクロール背景の更新
		UpdateMoveBG();

		// スコアの更新
		UpdateScore();
	
		// メッセージの更新
		UpdateMessage();

		// プレイヤーの更新
		UpdatePlayer();

		// 丸影の更新
		UpdateShadow();

		// 缶の更新
		UpdateCan();

		// ハードルの更新
		UpdateHurdle();
	
		// ゴールの更新
		UpdateGoal();
	
		// ジャンプ台の更新
		UpdateJumper();
	
		// アイテムの更新
		UpdateItem();
	
		// パーティクルの更新
		UpdateParticle();

		// ヒットエフェクトの更新
		UpdateHitEffect();
		
		// ダッシュエフェクトの更新
		UpdateDashEffect();

		// キーボードの更新
		UpdateKeyboard();

		break;
	}

	// モード切替
  	if( GetKeyboardPress( DIK_SPACE ) || nTimer > DEMO_TIME )
	{
		SetMode( MODE_TITLE );
		nTimer = 0;
	}

	// タイマーのインクリメント
	nTimer++;
}

/*------------------------------------------------------------------------------
  　描画処理
------------------------------------------------------------------------------*/
void DrawDemo( void )
{
	// タイマーの設定
	static int nTimer = 0;

	// スクロール背景の描画
	DrawMoveBG();

	// 背景の描画
	DrawBG();

	// スコアの描画
	DrawScore();
	
	// メッセージの描画
	DrawMessage();

	// 操作方法の描画
	DrawControl();
	
	// 丸影の描画
	DrawShadow();

	// ハードルの描画
	DrawHurdle();
	
	// ゴールの描画
	DrawGoal();
	
	// ジャンプ台の描画
	DrawJumper();
	
	// アイテムの描画
	DrawItem();
	
	// パーティクルの描画
	DrawParticle();

	// ヒットエフェクトの描画
	DrawHitEffect();
	
	// ダッシュエフェクトの描画
	DrawDashEffect();

	// プレイヤーの描画
	DrawPlayer();

	// 缶の描画
	DrawCan();
	
	// デモメッセージの表示
	if( nTimer < DEMO_MESSAGE_INTERVAL )
	{
		DrawPolygon( g_pTextureDemo, g_pVtxBufferDemo, 1 );
	}

	if( nTimer > DEMO_MESSAGE_INTERVAL * 2 )
	{
		nTimer = 0;
	}
	
	// タイマーのインクリメント
	nTimer++;
}