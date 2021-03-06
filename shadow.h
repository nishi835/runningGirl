/*==============================================================================

    shadowのヘッダー [shadow.h]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/4/17
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/
#ifndef _SHADOW_H_
#define _SHADOW_H_

/*------------------------------------------------------------------------------
  　ヘッダファイル
------------------------------------------------------------------------------*/
#include "main.h"

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
  　構造体の宣言
------------------------------------------------------------------------------*/
typedef struct
{
	D3DXVECTOR3 fPos;   // 座標
	D3DXVECTOR2 fSize;	// 幅,高さ
	D3DXCOLOR   fColor; // 色
}SHADOW;

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
void UninitShadow( void ); // 終了処理
void UpdateShadow( void );
HRESULT InitShadow( void ); // ポリゴンの初期化
void DrawShadow( void ); // ポリゴンの描画
void Set3DShadow( void );

#endif

