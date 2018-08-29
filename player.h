/*==============================================================================

    ポリゴン処理のヘッダ [polygon.h]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/
#ifndef _PLAYER_H_
#define _PLAYER_H_

/*------------------------------------------------------------------------------
  　ヘッダファイル
------------------------------------------------------------------------------*/
#include "polygon.h"

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define PLAYER_WIDTH ( 130.0f )								 // プレイヤーの幅
#define PLAYER_HEIGHT ( 180.0f )							 // プレイヤーの高さ
#define PLAYER_JUMP_VALUE ( 12.0f )							 // プレイヤーのジャンプ量
#define LIMIT_BOTTOM ( 465.0f )								 // 下移動制限
#define LIMIT_TOP ( 250.0f )								 // 上移動制限

/*------------------------------------------------------------------------------
  　列挙体の宣言
------------------------------------------------------------------------------*/
// プレイヤーの状態の列挙体
typedef enum
{
	STATE_STOP = 0, // 停止状態
	STATE_RUN,      // 走行状態
	STATE_JUMP,     // ジャンプ状態
	STATE_JUMPER,   // ジャンプ台によるジャンプ状態
	STATE_CRUSH,    // つぶされ状態
	STATE_TUMBLE,   // 転倒状態
	STATE_GOAL,     // ゴール状態
	STATE_WHEEZE,   // ゴール後の息切れ状態
	STATE_TIMEUP,   // タイムアップ状態
	STATE_GAMEOVER, // ゲームオーバー状態
	STATE_MAX
}STATE;

/*------------------------------------------------------------------------------
  　構造体の宣言
------------------------------------------------------------------------------*/
// プレイヤーの構造体
typedef struct
{
	D3DXVECTOR2 fPos;			// 中心
	D3DXVECTOR2 fAxis;			// 回転軸（足元）
	D3DXVECTOR2 fMove;			// 速度
	D3DXVECTOR2 fLeftUp;		// 左上の頂点
	D3DXVECTOR2 fRightUp;		// 右上の頂点
	D3DXVECTOR2 fLeftDown;		// 左下の頂点
	D3DXVECTOR2 fRightDown;		// 右下の頂点
	D3DXVECTOR2 fPosTex;		// テクスチャの座標
	float		fRotC;			// 中心を軸とした回転量
	float		fRotA;			// Axisを軸とした回転量
	float		fScrollTex;		// テクスチャのスクロール位置
	float		fLandingPosY;   // ジャンプの着地点
	int			nStunCount;		// スタン中のフレーム数をカウント
	int			nDashCount;     // ダッシュ中のフレーム数をカウント
	int         nCount;         // 汎用カウンター
	int			nNumTexX;		// テクスチャの列番号
	int			nNumTexY;		// テクスチャの行番号
	STATE		State;          // プレイヤーの状態
	bool		bDash;          // ダッシュしているか否か
}PLAYER;

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
void InitPlayer( void );
void UninitPlayer( void );
void UpdatePlayer( void );
void DrawPlayer( void );
void RotationPlayer( VERTEX_2D* pVtx );
bool HitObject( float pos1x, float pos1y, float width1, float height1, float pos2x, float pos2y, float width2, float height2 );
PLAYER* GetPlayer( void );
void SetPlayerState( STATE state );
void MovePlayerPos( D3DXVECTOR2 pos );

#endif