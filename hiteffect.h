/*==============================================================================

    パーティクルのヘッダ [particle.h]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/
#ifndef _HITEFFECT_H_
#define _HITEFFECT_H_

/*------------------------------------------------------------------------------
  　ヘッダファイル
------------------------------------------------------------------------------*/
#include "main.h"

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define HITEFFECT_WIDTH			 (4.0f)											// ポリゴンの幅
#define HITEFFECT_HEIGHT		 (2.0f)										// ポリゴンの高さ
#define HITEFFECT_RADIUS		 (70.0f)										// エフェクトの放射半径
#define HITEFFECT_SPEED          (2.0f)											// パーティクルの基準速さ
#define HITEFFECT_LIFE           (60)											// パーティクルの基準寿命
#define HITEFFECT_RADIUS_VALUE   (HITEFFECT_RADIUS / ( HITEFFECT_LIFE * 0.2f ))	// エフェクトの放射半径の変化量
#define HITEFFECT_MAX			 (72)											// エフェクトの最大数

/*------------------------------------------------------------------------------
  　構造体の宣言
------------------------------------------------------------------------------*/
// エフェクトの構造体
typedef struct
{
	D3DXVECTOR2	fPos;			// 座標
	D3DXVECTOR2	fAxis;			// 放射の中心座標
	D3DXVECTOR2	fSize;			// 基準サイズ
	D3DXVECTOR2	fVelocity;		// 速度
	D3DXVECTOR2 fLeftUp;		// 左上の頂点
	D3DXVECTOR2 fRightUp;		// 右上の頂点
	D3DXVECTOR2 fLeftDown;		// 左下の頂点
	D3DXVECTOR2 fRightDown;		// 右下の頂点
	D3DXCOLOR	fColor;			// フェードする色
	D3DXCOLOR	fColorValue;	// 色の変化量
	float		fRot;			// 回転量(ラジアン角)
	float       fRadius;        // 放射の中心座標からの半径
	float		fLength;		// 半径
	float		fAngle;			// 対角線の角度
	float		fScale;			// 大きさの倍率
	float		fSpeed;			// 速さ（スカラー）
	int			nLife;			// 体力
	bool		bUse;			// 使用中か否か
}HITEFFECT;

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
void InitHitEffect( void );
void UninitHitEffect( void );
void UpdateHitEffect( void );
void DrawHitEffect( void );
void CreateHitEffect( D3DXVECTOR2 pos, D3DXVECTOR2 size, float speed, float angleOfFire, D3DXCOLOR color, D3DXCOLOR colorValue,  int life );

#endif