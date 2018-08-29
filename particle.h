/*==============================================================================

    パーティクルのヘッダ [particle.h]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

/*------------------------------------------------------------------------------
  　ヘッダファイル
------------------------------------------------------------------------------*/
#include "main.h"

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define PARTICLE_WIDTH			(40.0f)							// ポリゴンの幅の半分
#define PARTICLE_HEIGHT			(40.0f)							// ポリゴンの高さの半分
#define PARTICLE_SPEED          (2.0f)							// パーティクルの基準速さ
#define PARTICLE_LIFE           (60)							// パーティクルの基準寿命

/*------------------------------------------------------------------------------
  　構造体の宣言
------------------------------------------------------------------------------*/
// 機雷の構造体
typedef struct
{
	D3DXVECTOR2	fPos;			// 左上座標
	D3DXVECTOR2	fSize;			// 基準サイズ
	D3DXVECTOR2	fVelocity;		// 速度
	D3DXVECTOR2 fLeftUp;		// 左上の頂点
	D3DXVECTOR2 fRightUp;		// 右上の頂点
	D3DXVECTOR2 fLeftDown;		// 左下の頂点
	D3DXVECTOR2 fRightDown;		// 右下の頂点
	D3DXCOLOR	fColor;			// フェードする色
	D3DXCOLOR	fColorValue;	// 色の変化量
	float		fRot;			// 回転量(ラジアン角)
	float		fLength;		// 半径
	float		fAngle;			// 対角線の角度
	float		fScale;			// 大きさの倍率
	float		fSpeed;			// 速さ（スカラー）
	int			nLife;			// 体力
	bool		bUse;			// 使用中か否か
}PARTICLE;

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
void InitParticle( void );
void UninitParticle( void );
void UpdateParticle( void );
void DrawParticle( void );
void CreateParticle( D3DXVECTOR2 pos, D3DXVECTOR2 size, float speed, float angleOfFire, D3DXCOLOR color, D3DXCOLOR colorValue,  int life );

#endif