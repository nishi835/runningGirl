/*==============================================================================

    ポリゴン処理のヘッダ [polygon.h]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/
#ifndef _POLYGON_H_
#define _POLYGON_H_

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define POLYGON_WIDTH ( 300 )								 // ポリゴンの幅
#define POLYGON_HEIGHT ( 600 )								 // ポリゴンの高さ

/*------------------------------------------------------------------------------
  　構造体の宣言
------------------------------------------------------------------------------*/
// 頂点座標構造体( 上の頂点フォーマットと構造を合わせること )
typedef struct 
{
	D3DXVECTOR3 pos;	// 頂点座標の構造体
	float rhw;
	D3DCOLOR color;		// 頂点カラーの構造体
	D3DXVECTOR2 tex;	// ＵＶ座標の構造体　※必ず構造体の最後に宣言する
}VERTEX_2D;

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
void InitPolygon( void );
void UninitPolygon( LPDIRECT3DTEXTURE9* pTexture, LPDIRECT3DVERTEXBUFFER9* pVtxBuffer );
void DrawPolygon( LPDIRECT3DTEXTURE9 pTexture, LPDIRECT3DVERTEXBUFFER9 pVtxBuffer, int nNumPolygon );
HRESULT MakeVertexPolygon( LPDIRECT3DVERTEXBUFFER9* pVtxBuffer, int nNumPolygon );


#endif