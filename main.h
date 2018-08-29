/*==============================================================================

    mainのヘッダー [main.h]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/4/17
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/
#ifndef _MAIN_H_
#define _MAIN_H_

/*------------------------------------------------------------------------------
  　ヘッダファイル
------------------------------------------------------------------------------*/
// ヘッダのインクルード
#include <d3dx9.h>
#define DIRECTINPUT_VERSION ( 0x0800 )
#include <dinput.h>							// ※順番を必ず守る

// ライブラリのリンク
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "winmm.lib" )		// マルチメディア命令のライブラリ（システム時刻取得に必要）
#pragma comment( lib, "dinput8.lib" )   // DirectInput8のリンク

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define SCREEN_WIDTH ( 800 ) //ウィンドウの幅
#define SCREEN_HEIGHT ( 600 ) //ウィンドウの高さ

#define NUM_VERTEX ( 4 ) // 頂点の数
#define NUM_POLYGON ( 2 ) // ポリゴンの枚数

// 頂点フォーマット
#define FVF_VERTEX_2D ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )  // ( 座標変換済み頂点座標 | 頂点カラー )

// セーフリリースマクロ
#define SAFE_RELEASE( x ) { if( x ) { ( x )->Release(); ( x ) = NULL; } }

/*------------------------------------------------------------------------------
  　列挙体
------------------------------------------------------------------------------*/
typedef enum
{
	MODE_TITLE = 0,
	MODE_DEMO,
	MODE_GAME, 
	MODE_RESULT, 
	MODE_TUTORIAL,
	MODE_RANKING,
	MODE_MAX
}MODE;

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
LPDIRECT3DDEVICE9 GetDevice( void ); // デバイスのポインタ取得
void DrawControl( void );
void SetMode( MODE mode );
MODE GetMode( void );
void ChangeMode( void );

#endif

