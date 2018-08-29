/*==============================================================================

    ダッシュエフェクトのヘッダ [dasheffect.h]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/
#ifndef _DASHEFFECT_H_
#define _DASHEFFECT_H_

/*------------------------------------------------------------------------------
  　ヘッダファイル
------------------------------------------------------------------------------*/
#include "main.h"

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define DASHEFFECT_WIDTH		 (0.8f)											// ポリゴンの幅
#define DASHEFFECT_HEIGHT		 (0.8f)											// ポリゴンの高さ
#define DASHEFFECT_VELOCITY_X	 (-5.0f)										// エフェクトの横移動速度
#define DASHEFFECT_EMITTER_MAX	 (43)											// エフェクトの発生源の最大数
#define DASHEFFECT_MAX			 (2048)											// エフェクトの最大数
#define DASHEFFECT_LIFE			 (20)											// エフェクトの体力

/*------------------------------------------------------------------------------
  　構造体の宣言
------------------------------------------------------------------------------*/
// エフェクトの構造体
typedef struct
{
	D3DXVECTOR2	fPos;			// 座標
	D3DXVECTOR2	fSize;			// 基準サイズ
	D3DXVECTOR2	fVelocity;		// 速度
	D3DXVECTOR2 fLeftUp;		// 左上の頂点
	D3DXVECTOR2 fRightUp;		// 右上の頂点
	D3DXVECTOR2 fLeftDown;		// 左下の頂点
	D3DXVECTOR2 fRightDown;		// 右下の頂点
	D3DXCOLOR	fColor;			// フェードする色
	D3DXCOLOR	fColorValue;	// 色の変化量
	float		fRot;			// 回転量（ラジアン）
	float		fLength;		// 半径
	float		fAngle;			// 対角線の角度
	float		fScale;			// 大きさの倍率
	int			nCounter;		// カウンター
	int			nLife;			// 体力
	int         nMaxLife;       // 最大体力
	bool		bUse;			// 使用中か否か
}DASHEFFECT;

// エフェクトの発生源の構造体
typedef struct
{
	D3DXVECTOR2	fPos;			// 座標
	D3DXVECTOR2	fSize;			// サイズ
	D3DXVECTOR2	fVelocity;		// 速度
	float		fLength;		// 半径
	float		fAngle;			// 対角線の角度
	int         nLife;			// 体力
	bool		bUse;           // 使用フラグ
}DASHEFFECTEMITTER;

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
void InitDashEffect( void );
void UninitDashEffect( void );
void UpdateDashEffect( void );
void DrawDashEffect( void );
void CreateDashEffectEmitter( D3DXVECTOR2 pos, D3DXVECTOR2 size, D3DXVECTOR2 velocity, int life );
void MoveDashEffectEmitter( float movey );
void UnuseDashEffect( void );

#endif