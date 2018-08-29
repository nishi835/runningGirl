/*==============================================================================

    ポリゴン処理 [player.cpp]
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
#include "movebg.h"
#include "input.h"
#include "polygon.h"
#include "player.h"
#include "can.h"
#include "hurdle.h"
#include "goal.h"
#include "message.h"
#include "jumper.h"
#include "item.h"
#include "particle.h"
#include "hiteffect.h"
#include "dasheffect.h"
#include "fade.h"
#include <math.h>

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define PLAYER_TEXTURENAME_RUN    "data/TEXTURE/osanpo_8cut.png"			 // テクスチャファイル名（走行時）
#define PLAYER_TEXTURENAME_TUMBLE "data/TEXTURE/tumble_4cut.png"			 // テクスチャファイル名（転倒時）
#define PLAYER_TEXTURENAME_CRUSH  "data/TEXTURE/crush_2cut.png"				 // テクスチャファイル名（つぶされ時）
#define PLAYER_TEXTURENAME_WHEEZE  "data/TEXTURE/wheeze.png"				 // テクスチャファイル名（息切れ時）

#define VALUE_GRAVITY			(0.6f)								     // 重力

#define PLAYER_POS_X			(200)							 		 // 表示位置X
#define PLAYER_POS_Y			(150)									 // 表示位置Y
#define PLAYER_MOVE_SPEED		(50.0f)									 // 移動速度
#define PLAYER_TEX_MAX			(4)										 // テクスチャ画像数
#define PLAYER_RUN_TEX_SCROLL_SPEED (0.2f)								 // テクスチャスクロールの速度
#define PLAYER_CRUSH_TEX_SCROLL_SPEED (0.06f)							 // テクスチャスクロールの速度
#define PLAYER_WHEEZE_TEX_SCROLL_SPEED (0.08f)							 // テクスチャスクロールの速度
#define PLAYER_RUN_TEX_WIDTH		(0.25f)								 // 走行時のテクスチャ幅
#define PLAYER_RUN_TEX_HEIGHT		(0.5f)								 // 走行時のテクスチャ高さ
#define PLAYER_RUN_MAX_NUMTEX_X     (4)									 // 走行時のテクスチャ列番号の最大数
#define PLAYER_RUN_MAX_NUMTEX_Y     (2)									 // 走行時のテクスチャ行番号の最大数
#define PLAYER_CRUSH_NUMTEX     (2)										 // テクスチャ列番号の最大数（つぶされ時）
#define PLAYER_WHEEZE_NUMTEX     (2)									 // テクスチャ列番号の最大数（息切れ時）
#define PLAYER_WHEEZE_TIME      (120)									 // 息切れの表示時間
#define PLAYER_JUMPER_VALUE		(24.0f)									 // ジャンプ台使用時のジャンプ量
#define PLAYER_CRUSH_TIME       (60)									 // つぶされ時間		
#define PLAYER_TUMBLE_TIME      (80)									 // 転倒時間
#define PLAYER_DASH_TIME        (180)									 // プレイヤーのダッシュ時間
#define PLAYER_NUM_SHADOW       (3)                                      // ダッシュ時の残像の数
#define PLAYER_SHADOW_INTERVAL  (20.0f)									 // ダッシュ時の残像の間隔

/*------------------------------------------------------------------------------
  　グローバル変数
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTexturePlayer[ PLAYER_TEX_MAX ] = {};   // テクスチャインターフェースのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPlayer				   = NULL; // 頂点バッファ管理インターフェースポインタ

PLAYER Player; // プレイヤーの構造体

// 回転関係
float g_fLengthPlayer;		// ポリゴンの対角線の長さ
float g_fAnglePlayer;		// ポリゴンの対角線の角度

/*------------------------------------------------------------------------------
  　初期化処理
------------------------------------------------------------------------------*/
void InitPlayer( void )
{
	// --- プレイヤーの初期化 --- //////////////////////////////////////////////

	Player.fAxis	    = D3DXVECTOR2( 160.0f, 240.0f + PLAYER_HEIGHT * 0.5f );
	Player.fMove	    = D3DXVECTOR2( 0.0f, 0.0f );
	Player.fRotA        = 0.0f;
	Player.fPosTex.x    = 0.0f;
	Player.fPosTex.y    = 0.0f;
	Player.fLandingPosY = 0.0f;
	Player.nNumTexX	    = 0;
	Player.nNumTexY	    = 0;
	Player.nCount       = 0;
	Player.nDashCount   = 0;
	Player.State        = STATE_STOP;
	
	// 表示座標の設定
	Player.fPos.x = Player.fAxis.x + cosf( Player.fRotA - D3DX_PI * 0.5f ) * PLAYER_HEIGHT * 0.5f;
	Player.fPos.y = Player.fAxis.y + sinf( Player.fRotA - D3DX_PI * 0.5f ) * PLAYER_HEIGHT * 0.5f;

	// 四隅の座標の設定
	Player.fLeftUp.x    = Player.fPos.x + cos( Player.fRotC + D3DX_PI + g_fAnglePlayer ) * g_fLengthPlayer;		// 左上の頂点X
	Player.fLeftUp.y    = Player.fPos.y + sin( Player.fRotC + D3DX_PI + g_fAnglePlayer ) * g_fLengthPlayer;		// 左上の頂点Y		
	Player.fRightUp.x   = Player.fPos.x + cos( Player.fRotC			  - g_fAnglePlayer ) * g_fLengthPlayer;		// 右上の頂点X
	Player.fRightUp.y   = Player.fPos.y + sin( Player.fRotC			  - g_fAnglePlayer ) * g_fLengthPlayer;		// 右上の頂点Y
	Player.fLeftDown.x  = Player.fPos.x + cos( Player.fRotC + D3DX_PI - g_fAnglePlayer ) * g_fLengthPlayer;		// 左下の頂点X
	Player.fLeftDown.y  = Player.fPos.y + sin( Player.fRotC + D3DX_PI - g_fAnglePlayer ) * g_fLengthPlayer;		// 左下の頂点Y
	Player.fRightDown.x = Player.fPos.x + cos( Player.fRotC			  + g_fAnglePlayer ) * g_fLengthPlayer;		// 右下の頂点X
	Player.fRightDown.y = Player.fPos.y + sin( Player.fRotC			  + g_fAnglePlayer ) * g_fLengthPlayer;		// 右下の頂点Y

	// --- 回転準備 ---	////////////////////////////////////////////////////////

	Player.fPos		= D3DXVECTOR2( PLAYER_POS_X, PLAYER_POS_Y );									// 中心座標の設定
	Player.fRotC	= 0.0f;																			// ポリゴンの回転量の設定
	g_fLengthPlayer = sqrt( PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HEIGHT * PLAYER_HEIGHT ) / 2.0f;   // 対角線の長さの算出
	g_fAnglePlayer  = atan2( PLAYER_HEIGHT, PLAYER_WIDTH );											// 対角線の角度の算出

	// --- 頂点の設定 --- //////////////////////////////////////////////////////

	MakeVertexPolygon( &g_pVtxBufferPlayer, PLAYER_NUM_SHADOW + 1 );
	
	VERTEX_2D* pVtx = NULL; // VRAMの仮想アドレスを格納

	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferPlayer->Lock( 0,
							  0,				// すべてをロック
							  ( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
							 
							  0 );	
		
	// 頂点座標の設定
	pVtx[ 0 ].pos.x = Player.fLeftUp.x;		// 左上の頂点X
	pVtx[ 0 ].pos.y = Player.fLeftUp.y;		// 左上の頂点Y		
	pVtx[ 1 ].pos.x = Player.fRightUp.x;	// 右上の頂点X
	pVtx[ 1 ].pos.y = Player.fRightUp.y;	// 右上の頂点Y
	pVtx[ 2 ].pos.x = Player.fLeftDown.x;	// 左下の頂点X
	pVtx[ 2 ].pos.y = Player.fLeftDown.y;	// 左下の頂点Y
	pVtx[ 3 ].pos.x = Player.fRightDown.x;	// 右下の頂点X
	pVtx[ 3 ].pos.y = Player.fRightDown.y;	// 右下の頂点Y
	
	// UV座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  Player.fPosTex.y );						// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, Player.fPosTex.y );						// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  Player.fPosTex.y + PLAYER_RUN_TEX_HEIGHT );	// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, Player.fPosTex.y + PLAYER_RUN_TEX_HEIGHT );	// 右下の頂点	

	pVtx += 4;
	
	pVtx[ 0 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  Player.fPosTex.y );						// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, Player.fPosTex.y );						// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  Player.fPosTex.y + PLAYER_RUN_TEX_HEIGHT );	// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, Player.fPosTex.y + PLAYER_RUN_TEX_HEIGHT );	// 右下の頂点	

	// 頂点バッファのアンロック
	g_pVtxBufferPlayer->Unlock();	

	// --- テクスチャの読み込み ////////////////////////////////////////////////

	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					  // デバイスのポインタを渡す
											PLAYER_TEXTURENAME_RUN,		  // テクスチャのファイル名
											&g_pTexturePlayer[ 0 ] ) ) )  // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );    // エラーチェック
	}

	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					  // デバイスのポインタを渡す
											PLAYER_TEXTURENAME_TUMBLE,	  // テクスチャのファイル名
											&g_pTexturePlayer[ 1 ] ) ) )  // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );    // エラーチェック
	}

	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					  // デバイスのポインタを渡す
											PLAYER_TEXTURENAME_CRUSH,	  // テクスチャのファイル名
											&g_pTexturePlayer[ 2 ] ) ) )  // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );    // エラーチェック
	}

	if( FAILED ( D3DXCreateTextureFromFile( pDevice,					  // デバイスのポインタを渡す
											PLAYER_TEXTURENAME_WHEEZE,	  // テクスチャのファイル名
											&g_pTexturePlayer[ 3 ] ) ) )  // テクスチャインターフェースの格納
	{
		MessageBox( NULL, "テクスチャ読み込み失敗", "エラー", MB_OK );    // エラーチェック
	}
}

/*------------------------------------------------------------------------------
  　終了処理
------------------------------------------------------------------------------*/	
void UninitPlayer( void )
{
	// テクスチャインターフェースの開放
	for( int nCnt = 0; nCnt < PLAYER_TEX_MAX; nCnt++ )
	{
		if( g_pTexturePlayer[ nCnt ] != NULL )
		{
			g_pTexturePlayer[ nCnt ]->Release();
			g_pTexturePlayer[ nCnt ] = NULL;
		}
	}

	// 頂点バッファインターフェースの開放
	if( g_pVtxBufferPlayer != NULL )
	{
		g_pVtxBufferPlayer->Release();
		g_pVtxBufferPlayer = NULL;
	}
}

/*------------------------------------------------------------------------------
  　更新処理
------------------------------------------------------------------------------*/
void UpdatePlayer( void )
{	
	// カウンターの処理 ////////////////////////////////////////////////////////

	// カウンターの初期化
	static int nFrameCount = 0;	// タイマー

	// つぶされ、転倒、ジャンプ処理 ////////////////////////////////////////////

	// 移動背景の取得
	MOVEBG* pMoveBg = NULL;
	pMoveBg			= GetMoveBG();

	// つぶされ処理
	if( Player.State == STATE_CRUSH )
	{
		pMoveBg->fTexScrollSpeed  *= 0.7f;					// スクロールを減速
		Player.fRotA			   = D3DX_PI + D3DX_PI / 2; // プレイヤーを倒す
		Player.fRotC			   = D3DX_PI + D3DX_PI / 2; 
		Player.nStunCount++;								// スタンカウンタをインクリメント

		// スタンカウンタがつぶされ時間を超えたら
		if( Player.nStunCount > PLAYER_CRUSH_TIME )
		{
			Player.State	  = STATE_STOP;		 // 停止状態に移行
			Player.fRotA	  = 0.0f;			 // プレイヤーを起こす
			Player.fRotC	  = 0.0f;			 // プレイヤーを起こす
			Player.nStunCount = 0;				 // スタンカウンタをリセット
		}
	}
	
	// 転倒処理
	if( Player.State == STATE_TUMBLE )
	{
		pMoveBg->fTexScrollSpeed *= 0.98f;					// スクロールを減速
		Player.nStunCount++;								// スタンカウンタをインクリメント
		
		// スタンカウンタが転倒時間の半分を超えたら
		if( Player.nStunCount > PLAYER_TUMBLE_TIME * 0.5f && Player.fScrollTex >= 3.0f )
		{
			pMoveBg->fTexScrollSpeed  *= 0.7f;				// スクロールを減速
			Player.fScrollTex		   = 3.0f;				// アニメーションを仰向け状態のカットに
		}

		// スタンカウンタが転倒時間を超えたら
		if( Player.nStunCount > PLAYER_TUMBLE_TIME )
		{
			Player.State	  = STATE_STOP;  // 停止状態に移行
			Player.nStunCount = 0;			 // スタンカウンタをリセット
		}
	}

	// ジャンプの滞空時間制御
	if( Player.State == STATE_JUMP || Player.State == STATE_JUMPER )
	{
		Player.fMove.y += VALUE_GRAVITY;		 // 重力加速

		// プレイヤーのY座標が着地座標以下になったら
		if( Player.fAxis.y >= Player.fLandingPosY )
		{
			Player.fAxis.y		= Player.fLandingPosY;  // プレイヤーのY座標を補正
			Player.State	    = STATE_RUN;			// 走行状態に戻す
			Player.fMove.y	    = 0.0f;					// Y移動量を0に
			Player.fLandingPosY = 0.0f;					// 着地座標を初期化
		}
	}
	
	// 砂ぼこりパーティクルの生成　////////////////////////////////////////////////
	
	if( Player.State == STATE_RUN && nFrameCount % 15 == 0 )
	{
		int temp = rand() % 21 - 10; // サイズ調整用
		CreateParticle( D3DXVECTOR2( Player.fAxis.x + rand() % 11 - 5, Player.fAxis.y + rand() % 11 - 15 ), // 座標
						D3DXVECTOR2( PARTICLE_WIDTH + temp, PARTICLE_HEIGHT + temp ),					    // サイズ
						PARTICLE_SPEED/* + rand() % 3 - 1*/,											    // 速さ
						D3DX_PI/*( 376 + rand() % 128 - 64 ) * 0.01f*/,									    // 角度（ラジアン）
						D3DXCOLOR( 1.0, 1.0, 1.0, 1.0f ),   // 色
						D3DXCOLOR( 0.0f, 0.0f, 0.0f, -(1.0f / PARTICLE_LIFE ) ),						    // 色の変化量
						PARTICLE_LIFE - rand() % 20 );													    // 寿命
	}

	// 缶との当たり判定 ///////////////////////////////////////////////////////////

	// 缶の取得
	CAN* pCan = NULL;
	pCan	  = GetCan();

	// 当たり判定
	for( int nCntCan = 0; nCntCan < CAN_MAX; nCntCan++ )
	{
		if( !pCan[ nCntCan ].bUse ) { continue; }

		if( ( Player.State == STATE_RUN || Player.State == STATE_STOP ) &&
			HitObject( Player.fLeftUp.x, Player.fLeftUp.y, PLAYER_WIDTH, PLAYER_HEIGHT, pCan[ nCntCan ].fLeftUp.x, pCan[ nCntCan ].fLeftUp.y, CAN_WIDTH, CAN_HEIGHT ) )
		{
			if( Player.bDash && pCan[ nCntCan ].bFly == false )
			{
				pCan[ nCntCan ].bFly    = true;
				pCan[ nCntCan ].fMove.x = 3.0f;
				pCan[ nCntCan ].fMove.y = - 8.0f;
			}
			else if( !Player.bDash ) 
			{
				Player.State = STATE_CRUSH;
				Player.fScrollTex = 0.0f;
			}
		}
	}

	// ハードルとの当たり判定 /////////////////////////////////////////////////////

	// ハードルの取得
	HURDLE* pHurdle = NULL;
	pHurdle	        = GetHurdle();

	// 当たり判定
	for( int nCntHurdle = 0; nCntHurdle < HURDLE_MAX; nCntHurdle++ )
	{
		if( !pHurdle[ nCntHurdle ].bUse ) { continue; }

		if( Player.State == STATE_RUN && !pHurdle[ nCntHurdle ].bFall &&
			HitObject( Player.fLeftUp.x, Player.fLeftUp.y, PLAYER_WIDTH, PLAYER_HEIGHT, pHurdle[ nCntHurdle ].fLeftUp.x, pHurdle[ nCntHurdle ].fLeftUp.y, HURDLE_WIDTH * 0.5f, HURDLE_HEIGHT ) )
		{
			Player.State   = STATE_TUMBLE;
			pHurdle[ nCntHurdle ].bFall = true;
		
			// ヒットエフェクトの作成
			for( int nCntHitEffect = 0; nCntHitEffect < HITEFFECT_MAX; nCntHitEffect++ )
			{
				if( nCntHitEffect % 6 == 0 )
				{
					CreateHitEffect( Player.fPos, D3DXVECTOR2( HITEFFECT_WIDTH * 3.0f, HITEFFECT_HEIGHT ), 0.0f, ( D3DX_PI * 2.0f / HITEFFECT_MAX ) * nCntHitEffect, D3DXCOLOR( 1.0f, 0.0f, 0.0f, 0.4f ), D3DXCOLOR( 0.0f, 0.0f, 0.0f, - ( 1.0f / HITEFFECT_LIFE * 1.0f) ), HITEFFECT_LIFE );
				}
				else
				{
					CreateHitEffect( Player.fPos, D3DXVECTOR2( HITEFFECT_WIDTH, HITEFFECT_HEIGHT ), 0.0f, ( D3DX_PI * 2.0f / HITEFFECT_MAX ) * nCntHitEffect, D3DXCOLOR( 1.0f, 0.0f, 0.0f, 0.4f ), D3DXCOLOR( 0.0f, 0.0f, 0.0f, - ( 1.0f / HITEFFECT_LIFE * 1.0f ) ), HITEFFECT_LIFE );
				}
			}
		}
	}
	
	// ジャンプ台との当たり判定 ///////////////////////////////////////////////////
	
	// ジャンプ台の取得
	JUMPER* pJumper = NULL;
	pJumper	        = GetJumper();
	
	// 当たり判定	
	for( int nCntJumper = 0; nCntJumper < HURDLE_MAX; nCntJumper++ )
	{
		if( !pJumper[ nCntJumper ].bUse ) { continue; }

		if( Player.State == STATE_RUN && 
			HitObject( Player.fLeftUp.x, Player.fLeftUp.y, PLAYER_WIDTH, PLAYER_HEIGHT, pJumper[ nCntJumper ].fLeftUp.x + JUMPER_WIDTH * 0.5f, pJumper[ nCntJumper ].fLeftUp.y, JUMPER_WIDTH * 0.1f, JUMPER_HEIGHT ) )
		{
			Player.State		= STATE_JUMPER;				// ジャンプ状態にする
			Player.fMove.y	    = - PLAYER_JUMPER_VALUE;	// 縦移動量を設定
			Player.fLandingPosY = Player.fAxis.y;           // ジャンプ時の座標を着地座標にする
		}
	}

	// アイテムとの当たり判定 ////////////////////////////////////////////////////

	// アイテムの情報の取得
	ITEM* pItem = GetItem();

	// 当たり判定	
	for( int nCntItem = 0; nCntItem < HURDLE_MAX; nCntItem++ )
	{
		if( !pItem[ nCntItem ].bUse ) { continue; }
	
		if( HitObject( Player.fLeftUp.x, Player.fLeftUp.y, PLAYER_WIDTH, PLAYER_HEIGHT, pItem[ nCntItem ].fLeftUp.x, pItem[ nCntItem ].fLeftUp.y, JUMPER_WIDTH, JUMPER_HEIGHT ) )
		{
			Player.bDash = true;    // ダッシュ状態にする
		}
	}

	if( Player.bDash )
	{
		// ダッシュエフェクトの発生源の作成
		for( int nCntDashEffect = 0; nCntDashEffect < DASHEFFECT_EMITTER_MAX; nCntDashEffect++ )
		{
			CreateDashEffectEmitter( D3DXVECTOR2( Player.fPos.x - DASHEFFECT_WIDTH - rand() % 100 * 0.1f, Player.fPos.y + rand() % ( int )PLAYER_HEIGHT - PLAYER_HEIGHT * 0.5f ),
									 D3DXVECTOR2( DASHEFFECT_WIDTH, DASHEFFECT_HEIGHT ),
									 D3DXVECTOR2( DASHEFFECT_VELOCITY_X, 0.0f ),
									 DASHEFFECT_LIFE + rand() % 20 );
		}
	}	
	
	// ゴールの処理 //////////////////////////////////////////////////////////////

	// ゴールの情報の取得
	GOAL* pGoal = GetGoal();
	
	// 当たり判定
	if( pGoal->bUse )
	{
		if( HitObject( Player.fLeftUp.x, Player.fLeftUp.y, PLAYER_WIDTH, PLAYER_HEIGHT, pGoal->fPos.x + GOAL_WIDTH * 0.5f, 0.0f, GOAL_WIDTH, SCREEN_HEIGHT ) && ( Player.State == STATE_RUN || Player.State == STATE_JUMPER ) )
		{
			CreateMessage( D3DXVECTOR2( 800.0f, 250.0f ), D3DXVECTOR2( MESSAGE_READY_WIDTH, 80.0f ), 2.0f, D3DX_PI, 0.0f, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0 ), D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0 ), MESSAGE_TYPE_GOAL );
			Player.State = STATE_GOAL;
			Player.bDash = false;
		}
	}

	// 次のステージへ進む
	if( Player.State == STATE_WHEEZE )
	{
		Player.nCount++;

		if( Player.nCount > PLAYER_WHEEZE_TIME )
		{
			FowardStage();
			if( GetStage() <= STAGE_MAX )
			{
				InitGame();
			}
			else
			{
				SetFade( FADE_OUT, MODE_RESULT );
			}
		}
	}

	// ゲームオーバーの処理 ////////////////////////////////////////////////////////////
	if( Player.State == STATE_GAMEOVER )
	{
		//pMoveBg->fTexScrollSpeed = 0.0f;
	}

	// 操作の処理 ////////////////////////////////////////////////////////////////
		
	// ジャンプ
	if( GetKeyboardTrigger( DIK_X ) && ( Player.State == STATE_STOP || Player.State == STATE_RUN ) ) 
	{
		Player.fMove.y	    = - PLAYER_JUMP_VALUE;	// 縦移動量を設定
		Player.fLandingPosY = Player.fAxis.y;       // ジャンプ時の座標を着地座標にする
		Player.State	    = STATE_JUMP;			// プレイヤーをジャンプ状態にする
	}
	
	// 自動前進の処理 ////////////////////////////////////////////////////////////

	// ダッシュ状態の場合
	if( Player.bDash )
	{
		pMoveBg->fTexScrollSpeed += MOVEBG_TEXTURE_SCROLL_ACCELERATION_DASH;	
		Player.nDashCount++;
		if( Player.nDashCount > PLAYER_DASH_TIME )
		{
			Player.bDash = false;
			Player.nDashCount = 0;
			UnuseDashEffect();
		}
	}
	// ゴール状態の場合
	else if( Player.State == STATE_GOAL || Player.State == STATE_TIMEUP )
	{
		pMoveBg->fTexScrollSpeed -= MOVEBG_TEXTURE_SCROLL_ACCELERATION * 0.5f;
		
		if( pMoveBg->fTexScrollSpeed < 0.0f )
		{
			pMoveBg->fTexScrollSpeed = 0.0f;

			if( Player.State == STATE_GOAL )
			{
				Player.State = STATE_WHEEZE;
			}
			else 
			{
				Player.State = STATE_GAMEOVER;
			}
		}
	}
	// 停止または走行状態の場合
	else
	{
		if( Player.State == STATE_STOP || Player.State == STATE_RUN )
		{
			pMoveBg->fTexScrollSpeed += MOVEBG_TEXTURE_SCROLL_ACCELERATION;
			Player.State = STATE_RUN;
		}
	}

	// 前進速度制限
	if( Player.bDash )
	{
		if( pMoveBg->fTexScrollSpeed > MOVEBG_TEXTURE_LIMIT_SCROOL_SPEED_DASH )
		{
			pMoveBg->fTexScrollSpeed = MOVEBG_TEXTURE_LIMIT_SCROOL_SPEED_DASH;
		}
	}
	else
	{
		if( pMoveBg->fTexScrollSpeed > MOVEBG_TEXTURE_LIMIT_SCROOL_SPEED )
		{
			pMoveBg->fTexScrollSpeed = MOVEBG_TEXTURE_LIMIT_SCROOL_SPEED;
		}
	}

	// 下移動
	if( GetKeyboardTrigger( DIK_S ) && ( Player.State == STATE_STOP || Player.State == STATE_RUN ) )
	{
		Player.fAxis.y += PLAYER_MOVE_SPEED;
		MoveDashEffectEmitter( PLAYER_MOVE_SPEED );
	}
	
	// 上移動
	if( GetKeyboardTrigger( DIK_W ) && ( Player.State == STATE_STOP || Player.State == STATE_RUN ) )
	{
		Player.fAxis.y -= PLAYER_MOVE_SPEED;
		MoveDashEffectEmitter( - PLAYER_MOVE_SPEED );
	} 

	// 上移動制限
	if( ( Player.fPos.y + PLAYER_HEIGHT / 2 < LIMIT_TOP ) && Player.State != STATE_JUMP )
	{
		Player.fPos.y = LIMIT_TOP - PLAYER_HEIGHT / 2;
	}
	
	// 下移動制限
	if( ( Player.fPos.y + PLAYER_HEIGHT / 2  > LIMIT_BOTTOM ) && Player.State != STATE_JUMP )
	{
		Player.fPos.y = LIMIT_BOTTOM - PLAYER_HEIGHT / 2;
	}

	// 座標の更新 ///////////////////////////////////////////////////////////////

	// 回転軸の更新
	Player.fAxis.y += Player.fMove.y;
	
	// 表示座標の更新
	Player.fPos.x = Player.fAxis.x + cosf( Player.fRotA - D3DX_PI * 0.5f ) * PLAYER_HEIGHT * 0.5f;
	Player.fPos.y = Player.fAxis.y + sinf( Player.fRotA - D3DX_PI * 0.5f ) * PLAYER_HEIGHT * 0.5f;
	
	// 四隅の座標の更新
	Player.fLeftUp.x    = Player.fPos.x + cos( Player.fRotC + D3DX_PI + g_fAnglePlayer ) * g_fLengthPlayer;		// 左上の頂点X
	Player.fLeftUp.y    = Player.fPos.y + sin( Player.fRotC + D3DX_PI + g_fAnglePlayer ) * g_fLengthPlayer;		// 左上の頂点Y		
	Player.fRightUp.x   = Player.fPos.x + cos( Player.fRotC			  - g_fAnglePlayer ) * g_fLengthPlayer;		// 右上の頂点X
	Player.fRightUp.y   = Player.fPos.y + sin( Player.fRotC			  - g_fAnglePlayer ) * g_fLengthPlayer;		// 右上の頂点Y
	Player.fLeftDown.x  = Player.fPos.x + cos( Player.fRotC + D3DX_PI - g_fAnglePlayer ) * g_fLengthPlayer;		// 左下の頂点X
	Player.fLeftDown.y  = Player.fPos.y + sin( Player.fRotC + D3DX_PI - g_fAnglePlayer ) * g_fLengthPlayer;		// 左下の頂点Y
	Player.fRightDown.x = Player.fPos.x + cos( Player.fRotC			  + g_fAnglePlayer ) * g_fLengthPlayer;		// 右下の頂点X
	Player.fRightDown.y = Player.fPos.y + sin( Player.fRotC			  + g_fAnglePlayer ) * g_fLengthPlayer;		// 右下の頂点Y
	
	// テクスチャの更新 //////////////////////////////////////////////////////////

	switch( Player.State )
	{
	case STATE_RUN:
	case STATE_TUMBLE:
	case STATE_GOAL:
	case STATE_TIMEUP:
		// テクスチャ番号の更新
		Player.nNumTexX = ( int )Player.fScrollTex % PLAYER_RUN_MAX_NUMTEX_X;
		Player.nNumTexY = ( int )Player.fScrollTex / PLAYER_RUN_MAX_NUMTEX_X % PLAYER_RUN_MAX_NUMTEX_Y;

		// テクスチャ座標の更新
		Player.fPosTex.x = Player.nNumTexX * ( 1.0f / PLAYER_RUN_MAX_NUMTEX_X );
		Player.fPosTex.y = Player.nNumTexY * ( 1.0f / PLAYER_RUN_MAX_NUMTEX_Y );
		
		// テクスチャスクロール位置の更新
		Player.fScrollTex += PLAYER_RUN_TEX_SCROLL_SPEED;
		break;
	case STATE_CRUSH:		
		// テクスチャ座標の更新
		if( Player.fScrollTex < 1.0f )
		{
			Player.fPosTex.x = 0.0f;
		}
		else
		{
			Player.fPosTex.x = 0.5f;
		}

		// テクスチャスクロール位置の更新
		Player.fScrollTex += PLAYER_CRUSH_TEX_SCROLL_SPEED;
		break;
	case STATE_WHEEZE:
		// テクスチャ番号の更新
		Player.nNumTexX = ( int )Player.fScrollTex % PLAYER_WHEEZE_NUMTEX;

		// テクスチャ座標の更新
		Player.fPosTex.x = Player.nNumTexX * ( 1.0f / PLAYER_WHEEZE_NUMTEX );
		
		// テクスチャスクロール位置の更新
		Player.fScrollTex += PLAYER_WHEEZE_TEX_SCROLL_SPEED;
		break;
	case STATE_GAMEOVER:
		// テクスチャ座標の設定
		Player.fPosTex.x = 0.5f;
		break;
	case STATE_STOP:
		break;
	}	

	// 頂点バッファの更新 ////////////////////////////////////////////////////////

	VERTEX_2D* pVtx = NULL; // VRAMの仮想アドレスを格納

	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferPlayer->Lock( 0,
							  0,				// すべてをロック
							  ( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
							 
							  0 );	

	// 回転、頂点バッファ設定
	RotationPlayer( pVtx );
	
	// UV座標の設定
	if( Player.State == STATE_TUMBLE || Player.State == STATE_JUMPER )
	{
		pVtx[ 0 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  0.0f );						// 左上の頂点
		pVtx[ 1 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, 0.0f );						// 右上の頂点
		pVtx[ 2 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  1.0f );	// 左下の頂点	
		pVtx[ 3 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, 1.0f );	// 右下の頂点

		pVtx += 4;

		pVtx[ 0 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  0.0f );						// 左上の頂点
		pVtx[ 1 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, 0.0f );						// 右上の頂点
		pVtx[ 2 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  1.0f );	// 左下の頂点	
		pVtx[ 3 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, 1.0f );	// 右下の頂点
	}
	else if( Player.State == STATE_CRUSH || Player.State == STATE_WHEEZE || Player.State == STATE_GAMEOVER )
	{
		pVtx[ 0 ].tex = D3DXVECTOR2( Player.fPosTex.x,					  0.0f );						// 左上の頂点
		pVtx[ 1 ].tex = D3DXVECTOR2( Player.fPosTex.x + 0.5f			, 0.0f );						// 右上の頂点
		pVtx[ 2 ].tex = D3DXVECTOR2( Player.fPosTex.x,					  1.0f );	// 左下の頂点	
		pVtx[ 3 ].tex = D3DXVECTOR2( Player.fPosTex.x + 0.5f			, 1.0f );	// 右下の頂点
	}
	else
	{
		if( Player.bDash )
		{
			for( int nCnt = 0; nCnt < PLAYER_NUM_SHADOW + 1; nCnt++ )
			{
				if( nCnt > 0 )
				{
					// 頂点カラーの設定（ 0 ～ 255の整数値 )
					pVtx[ 0 ].color = D3DCOLOR_RGBA( 255, 255, 255, 64 );	// 左上の頂点
					pVtx[ 1 ].color = D3DCOLOR_RGBA( 255, 255, 255, 64 );	// 右上の頂点
					pVtx[ 2 ].color = D3DCOLOR_RGBA( 255, 255, 255, 64 );	// 左下の頂点
					pVtx[ 3 ].color = D3DCOLOR_RGBA( 255, 255, 255, 64 );	// 右下の頂点
				}

				pVtx[ 0 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  Player.fPosTex.y );						// 左上の頂点
				pVtx[ 1 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, Player.fPosTex.y );						// 右上の頂点
				pVtx[ 2 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  Player.fPosTex.y + PLAYER_RUN_TEX_HEIGHT );	// 左下の頂点	
				pVtx[ 3 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, Player.fPosTex.y + PLAYER_RUN_TEX_HEIGHT );	// 右下の頂点
																																														
				pVtx += 4;
			}
		}
		else
		{
			pVtx[ 0 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  Player.fPosTex.y );						// 左上の頂点
			pVtx[ 1 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, Player.fPosTex.y );						// 右上の頂点
			pVtx[ 2 ].tex = D3DXVECTOR2( Player.fPosTex.x,						  Player.fPosTex.y + PLAYER_RUN_TEX_HEIGHT );	// 左下の頂点	
			pVtx[ 3 ].tex = D3DXVECTOR2( Player.fPosTex.x + PLAYER_RUN_TEX_WIDTH, Player.fPosTex.y + PLAYER_RUN_TEX_HEIGHT );	// 右下の頂点
		}
	}																		

	// 頂点バッファのアンロック
	g_pVtxBufferPlayer->Unlock();	

	// カウンターの処理 ////////////////////////////////////////////////////////

	// カウンターのインクリメント
	nFrameCount++;
}

/*------------------------------------------------------------------------------
  　描画処理
------------------------------------------------------------------------------*/
void DrawPlayer( void )
{
	// プレイヤーの描画
	switch( Player.State )
	{
	case STATE_STOP:
	case STATE_RUN:
	case STATE_JUMP:
	case STATE_GOAL:
	case STATE_TIMEUP:
		DrawPolygon( g_pTexturePlayer[ 0 ], g_pVtxBufferPlayer, PLAYER_NUM_SHADOW + 1 );
		break;
	case STATE_TUMBLE:
	case STATE_JUMPER:
		DrawPolygon( g_pTexturePlayer[ 1 ], g_pVtxBufferPlayer, 1 );
		break;
	case STATE_CRUSH:
		DrawPolygon( g_pTexturePlayer[ 2 ], g_pVtxBufferPlayer, 1 );
		break;
	case STATE_WHEEZE:
	case STATE_GAMEOVER:
		DrawPolygon( g_pTexturePlayer[ 3 ], g_pVtxBufferPlayer, 1 );
		break;
	}
}

/*------------------------------------------------------------------------------
  　缶の回転、頂点バッファ設定
------------------------------------------------------------------------------*/
void RotationPlayer( VERTEX_2D* pVtx )
{
	if( Player.bDash )
	{
		for( int nCnt = 0; nCnt < PLAYER_NUM_SHADOW + 1; nCnt++ )
		{
			// 頂点座標の設定
			pVtx[ 0 ].pos.x = Player.fLeftUp.x - PLAYER_SHADOW_INTERVAL * nCnt;		// 左上の頂点X
			pVtx[ 0 ].pos.y = Player.fLeftUp.y;										// 左上の頂点Y		
			pVtx[ 1 ].pos.x = Player.fRightUp.x - PLAYER_SHADOW_INTERVAL * nCnt;	// 右上の頂点X
			pVtx[ 1 ].pos.y = Player.fRightUp.y;									// 右上の頂点Y
			pVtx[ 2 ].pos.x = Player.fLeftDown.x - PLAYER_SHADOW_INTERVAL * nCnt;	// 左下の頂点X
			pVtx[ 2 ].pos.y = Player.fLeftDown.y;									// 左下の頂点Y
			pVtx[ 3 ].pos.x = Player.fRightDown.x - PLAYER_SHADOW_INTERVAL * nCnt;	// 右下の頂点X
			pVtx[ 3 ].pos.y = Player.fRightDown.y;									// 右下の頂点Y

			pVtx += 4;
		}
	}
	else
	{
		// 頂点座標の設定
		pVtx[ 0 ].pos.x = Player.fLeftUp.x;		// 左上の頂点X
		pVtx[ 0 ].pos.y = Player.fLeftUp.y;						// 左上の頂点Y		
		pVtx[ 1 ].pos.x = Player.fRightUp.x;		// 右上の頂点X
		pVtx[ 1 ].pos.y = Player.fRightUp.y;					// 右上の頂点Y
		pVtx[ 2 ].pos.x = Player.fLeftDown.x;	// 左下の頂点X
		pVtx[ 2 ].pos.y = Player.fLeftDown.y;					// 左下の頂点Y
		pVtx[ 3 ].pos.x = Player.fRightDown.x;	// 右下の頂点X
		pVtx[ 3 ].pos.y = Player.fRightDown.y;					// 右下の頂点Y
	}
}

/*------------------------------------------------------------------------------
  　オブジェクトとの当たり判定
------------------------------------------------------------------------------*/
bool HitObject( float pos1x, float pos1y, float width1, float height1, float pos2x, float pos2y, float width2, float height2 )
{
	bool bHit = false;
	if( pos1x + width1 > pos2x && pos1x < pos2x + width2 )
	{
		if( pos1y + height1 > pos2y && pos1y + height1 < pos2y + height2 )
		{
			bHit = true;
		}
	}

	return bHit;
}

/*------------------------------------------------------------------------------
  　ゲッター
------------------------------------------------------------------------------*/
PLAYER* GetPlayer( void )
{
	return &Player;
}

/*------------------------------------------------------------------------------
  　セッター
------------------------------------------------------------------------------*/
void SetPlayerState( STATE state )
{
	Player.State = state;
}

void MovePlayerPos( D3DXVECTOR2 pos )
{
	Player.fAxis += pos;
}
