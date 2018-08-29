/*==============================================================================

    パーティクルの処理 [hiteffect.cpp]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  　ヘッダファイル
------------------------------------------------------------------------------*/
#include "main.h"
#include "hiteffect.h"
#include "polygon.h"
#include "player.h"

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define HITEFFECT_TEXTURENAME	 "data/TEXTURE/line.png"				// テクスチャ名(.png ◎ / .jpg / .tga ◎ / .bmp / .dds ◎) ◎は半透明が使用可

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTextureHitEffect   = NULL;		// テクスチャインターフェースのポインタ変数
LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferHitEffect = NULL;		// 頂点バッファ管理インターフェースポインタ

HITEFFECT				HitEffect[ HITEFFECT_MAX ];		// エフェクトの構造体
int						g_nCntHitEffectPolygon = 0;		// エフェクトのポリゴン数（描画するエフェクトの数×2）

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
HRESULT MakeVertexHitEffect( LPDIRECT3DDEVICE9 pDevice );

/*------------------------------------------------------------------------------
  　パーティクルの初期化
------------------------------------------------------------------------------*/
void InitHitEffect( void )
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 初期値の設定
	for( int nCntHitEffect = 0; nCntHitEffect < HITEFFECT_MAX; nCntHitEffect++ )
	{
		HitEffect[ nCntHitEffect ].bUse			= false;
		HitEffect[ nCntHitEffect ].fPos.x		= 0.0f;
		HitEffect[ nCntHitEffect ].fPos.y		= 0.0f;
		HitEffect[ nCntHitEffect ].fSize.x		= HITEFFECT_WIDTH;
		HitEffect[ nCntHitEffect ].fSize.y		= HITEFFECT_HEIGHT;
		HitEffect[ nCntHitEffect ].fRot			= 0.0f;
		HitEffect[ nCntHitEffect ].fLength		= 0.0f;
		HitEffect[ nCntHitEffect ].fRadius		= 0.0f;
		HitEffect[ nCntHitEffect ].fAngle		= 0.0f;
		HitEffect[ nCntHitEffect ].fScale		= 1.0f;
		HitEffect[ nCntHitEffect ].fSpeed		= 0.0f;
		HitEffect[ nCntHitEffect ].fVelocity.x	= 0.0f;
		HitEffect[ nCntHitEffect ].fVelocity.y	= 0.0f;
		HitEffect[ nCntHitEffect ].nLife		= 0;
	}

	// 頂点の作成
	MakeVertexPolygon( &g_pVtxBufferHitEffect, HITEFFECT_MAX );

	// テクスチャの読み込み
	if( FAILED( D3DXCreateTextureFromFile( pDevice, HITEFFECT_TEXTURENAME, &g_pTextureHitEffect ) ) )
	{
		MessageBox( NULL, "HitEffectのテクスチャの読み込みに失敗しました。", "エラーメッセージ", MB_OK | MB_ICONEXCLAMATION );
	}
}

/*------------------------------------------------------------------------------
  　パーティクルの終了処理
------------------------------------------------------------------------------*/
void UninitHitEffect( void )
{
	// テクスチャインターフェースの終了処理
	if( g_pTextureHitEffect )
	{
		g_pTextureHitEffect->Release();
		g_pTextureHitEffect = NULL;
	}

	// 頂点バッファ管理インターフェースポインタの終了処理
	if( g_pVtxBufferHitEffect )
	{
		g_pVtxBufferHitEffect->Release();
		g_pVtxBufferHitEffect = NULL;
	}
}

/*------------------------------------------------------------------------------
  　パーティクルの更新
------------------------------------------------------------------------------*/
void UpdateHitEffect( void )
{
	// プレイヤー情報の取得
	PLAYER* pPlayer = GetPlayer();

	// 座標更新
	for( int nCntHitEffect = 0; nCntHitEffect < HITEFFECT_MAX; nCntHitEffect++ )
	{
		// 未使用中なら以下のプログラムを飛ばす
		if( !HitEffect[ nCntHitEffect ].bUse ) { continue; }

		HitEffect[ nCntHitEffect ].fPos.x += HitEffect[ nCntHitEffect ].fVelocity.x;
		HitEffect[ nCntHitEffect ].fPos.y += HitEffect[ nCntHitEffect ].fVelocity.y;
		HitEffect[ nCntHitEffect ].fColor += HitEffect[ nCntHitEffect ].fColorValue;
		HitEffect[ nCntHitEffect ].nLife--;

		// 体力がなくなったら消す
		if( HitEffect[ nCntHitEffect ].nLife <= 0 )
		{
			HitEffect[ nCntHitEffect ].bUse		= false;
			HitEffect[ nCntHitEffect ].fPos.x		= 0.0f;
			HitEffect[ nCntHitEffect ].fPos.y		= 0.0f;
			HitEffect[ nCntHitEffect ].fRadius		= 0.0f;
			HitEffect[ nCntHitEffect ].nLife		= 0;

			continue;
		}
	}
	
	// 頂点バッファ格納変数
	VERTEX_2D* pVtx = NULL;

	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferHitEffect->Lock( 0, 0, ( void** )&pVtx, 0 );
	
	// 更新
	for( int nCntHitEffect = 0; nCntHitEffect < HITEFFECT_MAX; nCntHitEffect++ )
	{
		if( !HitEffect[ nCntHitEffect ].bUse ) { continue; }
		
		// 表示座標の更新		
		HitEffect[ nCntHitEffect ].fRadius += HITEFFECT_RADIUS_VALUE;
		HitEffect[ nCntHitEffect ].fPos.x   = HitEffect[ nCntHitEffect ].fAxis.x + cosf( HitEffect[ nCntHitEffect ].fRot /*- D3DX_PI * 0.5f*/ ) * HitEffect[ nCntHitEffect ].fRadius;
		HitEffect[ nCntHitEffect ].fPos.y   = HitEffect[ nCntHitEffect ].fAxis.y + sinf( HitEffect[ nCntHitEffect ].fRot /*- D3DX_PI * 0.5f*/ ) * HitEffect[ nCntHitEffect ].fRadius;	

		// 四隅の座標の更新
		HitEffect[ nCntHitEffect ].fLeftUp.x    = HitEffect[ nCntHitEffect ].fPos.x + cos( HitEffect[ nCntHitEffect ].fRot + D3DX_PI + HitEffect[ nCntHitEffect ].fAngle ) * HitEffect[ nCntHitEffect ].fLength;		// 左上の頂点X
		HitEffect[ nCntHitEffect ].fLeftUp.y    = HitEffect[ nCntHitEffect ].fPos.y + sin( HitEffect[ nCntHitEffect ].fRot + D3DX_PI + HitEffect[ nCntHitEffect ].fAngle ) * HitEffect[ nCntHitEffect ].fLength;		// 左上の頂点Y		
		HitEffect[ nCntHitEffect ].fRightUp.x   = HitEffect[ nCntHitEffect ].fPos.x + cos( HitEffect[ nCntHitEffect ].fRot		     - HitEffect[ nCntHitEffect ].fAngle ) * HitEffect[ nCntHitEffect ].fLength;		// 右上の頂点X
		HitEffect[ nCntHitEffect ].fRightUp.y   = HitEffect[ nCntHitEffect ].fPos.y + sin( HitEffect[ nCntHitEffect ].fRot		     - HitEffect[ nCntHitEffect ].fAngle ) * HitEffect[ nCntHitEffect ].fLength;		// 右上の頂点Y
		HitEffect[ nCntHitEffect ].fLeftDown.x  = HitEffect[ nCntHitEffect ].fPos.x + cos( HitEffect[ nCntHitEffect ].fRot + D3DX_PI - HitEffect[ nCntHitEffect ].fAngle ) * HitEffect[ nCntHitEffect ].fLength;		// 左下の頂点X
		HitEffect[ nCntHitEffect ].fLeftDown.y  = HitEffect[ nCntHitEffect ].fPos.y + sin( HitEffect[ nCntHitEffect ].fRot + D3DX_PI - HitEffect[ nCntHitEffect ].fAngle ) * HitEffect[ nCntHitEffect ].fLength;		// 左下の頂点Y
		HitEffect[ nCntHitEffect ].fRightDown.x = HitEffect[ nCntHitEffect ].fPos.x + cos( HitEffect[ nCntHitEffect ].fRot		     + HitEffect[ nCntHitEffect ].fAngle ) * HitEffect[ nCntHitEffect ].fLength;		// 右下の頂点X
		HitEffect[ nCntHitEffect ].fRightDown.y = HitEffect[ nCntHitEffect ].fPos.y + sin( HitEffect[ nCntHitEffect ].fRot		     + HitEffect[ nCntHitEffect ].fAngle ) * HitEffect[ nCntHitEffect ].fLength;		// 右下の頂点Y
		
		// 頂点座標の設定
		pVtx[ 0 ].pos.x = HitEffect[ nCntHitEffect ].fLeftUp.x;		// 左上の頂点X
		pVtx[ 0 ].pos.y = HitEffect[ nCntHitEffect ].fLeftUp.y;		// 左上の頂点Y		
		pVtx[ 1 ].pos.x = HitEffect[ nCntHitEffect ].fRightUp.x;	// 右上の頂点X
		pVtx[ 1 ].pos.y = HitEffect[ nCntHitEffect ].fRightUp.y;	// 右上の頂点Y
		pVtx[ 2 ].pos.x = HitEffect[ nCntHitEffect ].fLeftDown.x;	// 左下の頂点X
		pVtx[ 2 ].pos.y = HitEffect[ nCntHitEffect ].fLeftDown.y;	// 左下の頂点Y
		pVtx[ 3 ].pos.x = HitEffect[ nCntHitEffect ].fRightDown.x;	// 右下の頂点X
		pVtx[ 3 ].pos.y = HitEffect[ nCntHitEffect ].fRightDown.y;	// 右下の頂点Y
		
		// テクスチャ座標の設定( 0.0f～1.0f )
		pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );	// 左上の座標
		pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );	// 右上の座標
		pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );	// 左下の座標
		pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );	// 左下の座標
		
		// 頂点色の設定
		pVtx[ 0 ].color = HitEffect[ nCntHitEffect ].fColor;
		pVtx[ 1 ].color = HitEffect[ nCntHitEffect ].fColor;
		pVtx[ 2 ].color = HitEffect[ nCntHitEffect ].fColor;
		pVtx[ 3 ].color = HitEffect[ nCntHitEffect ].fColor;

		// 次のポリゴンの頂点へ
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBufferHitEffect->Unlock();
}

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
void DrawHitEffect( void )
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 加算合成に切り替え
	//pDevice->SetRenderState( D3DRS_DESTBLEND,		// 設定名(インデックス)	D3DRS_DESTBLEND：背景のアルファ設定
	//						   D3DBLEND_ONE );		// 設定値

	// ストリーム(パイプライン)設定
	pDevice->SetStreamSource( 0,						// 繋げるパイプの番号
							  g_pVtxBufferHitEffect,		// ストリームの元となる頂点バッファの指定
							  0,						// オフセット(バイト)
							  sizeof( VERTEX_2D ) );		// ストライド値(ストライド量)

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_2D );

	// テクスチャの設定
	pDevice->SetTexture( 0, g_pTextureHitEffect );
	
	for( int nCntHitEffect = 0; nCntHitEffect < HITEFFECT_MAX; nCntHitEffect++ )
	{
		if( !HitEffect[ nCntHitEffect ].bUse ) { continue; }
				
		// プリミティブの描画
		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,		// プリミティブの種類
								nCntHitEffect * 4,			// オフセット(頂点数)
								2 );						// プリミティブ数
	}

	// 乗算合成に切り替え
	pDevice->SetRenderState( D3DRS_DESTBLEND,			// 設定名(インデックス)	D3DRS_DESTBLEND：背景のアルファ設定
						     D3DBLEND_INVSRCALPHA );	// 設定値	D3DBLEND_INVSRCALPHA：１－α

}

/*------------------------------------------------------------------------------
  　パーティクルの作成
--------------------------------------------------------------------------------
   引数 pos			 : 座標
        scale		 : 拡大度
        color		 : 色
        alphaValue	 : αの変化量
        life		 : 体力
        type		 : エフェクトの種類
------------------------------------------------------------------------------*/
void CreateHitEffect( D3DXVECTOR2 pos, D3DXVECTOR2 size, float speed, float angleOfFire, D3DXCOLOR color, D3DXCOLOR colorValue,  int life )
{
	for( int nCntHitEffect = 0; nCntHitEffect < HITEFFECT_MAX; nCntHitEffect++ )
	{
		// エフェクトが使われていなかったら
		if( !HitEffect[ nCntHitEffect ].bUse )
		{
			HitEffect[ nCntHitEffect ].bUse			= true;
			HitEffect[ nCntHitEffect ].fAxis		= pos;
			HitEffect[ nCntHitEffect ].fLength		= sqrt( ( size.y * size.y ) + ( size.x * size.x  ) );
			HitEffect[ nCntHitEffect ].fAngle		= atan2( size.y, size.x );
			HitEffect[ nCntHitEffect ].fRot			= angleOfFire;
			HitEffect[ nCntHitEffect ].fVelocity.x	= speed * cos( angleOfFire );
			HitEffect[ nCntHitEffect ].fVelocity.y	= speed * sin( angleOfFire );
			HitEffect[ nCntHitEffect ].fColor		= color;
			HitEffect[ nCntHitEffect ].fColorValue	= colorValue;
			HitEffect[ nCntHitEffect ].nLife		= life;

			break;	// for文を抜ける
		}
	}
}

//******************************* EOF **************************************