/*==============================================================================

    GAME処理 [game.cpp]
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
#include <stdio.h>

/*------------------------------------------------------------------------------
  　グローバル変数
------------------------------------------------------------------------------*/
GAMEMODE g_GameMode = GAMEMODE_READY;
int      g_nStage   = 1;

/*------------------------------------------------------------------------------
  　初期化処理
------------------------------------------------------------------------------*/
void InitGame( void )
{
	g_nStage = 1;

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

	// BGMの再生
	/*if( FAILED( PlaySound( SOUND_LABEL_BGM000 ) ) )
	{
		MessageBox( 0, "再生に失敗しました", "警告", MB_ICONWARNING );
	}*/

	// ステージ情報のロード
	LoadGame();
}

/*------------------------------------------------------------------------------
  　終了処理
------------------------------------------------------------------------------*/
void UninitGame( void )
{	
	g_nStage = 1;

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
void UpdateGame( void )
{
	// メッセージの情報取得
	MESSAGE* pMessage = GetMessage();

	switch( pMessage->nType )
	{ 
	case MESSAGE_TYPE_STAGE:
	case MESSAGE_TYPE_READY:
	case MESSAGE_TYPE_GAMEOVER:
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
	case MESSAGE_TYPE_TIMEUP:
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
}

/*------------------------------------------------------------------------------
  　描画処理
------------------------------------------------------------------------------*/
void DrawGame( void )
{
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
}

/*------------------------------------------------------------------------------
  　ロード処理
------------------------------------------------------------------------------*/
void LoadGame( void )
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
		sprintf( aTemp, "data\\stage%d.bin", g_nStage );
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

/*------------------------------------------------------------------------------
  　ゲッター
------------------------------------------------------------------------------*/
int GetStage( void )
{
	return g_nStage;
}

/*------------------------------------------------------------------------------
  　セッター
------------------------------------------------------------------------------*/
void FowardStage( void )
{
	g_nStage++;
}