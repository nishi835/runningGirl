/*==============================================================================

    パーティクルの処理 [dasheffect.cpp]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  　ヘッダファイル
------------------------------------------------------------------------------*/
#include "main.h"
#include "dasheffect.h"
#include "polygon.h"
#include "player.h"

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define DASHEFFECT_TEXTURENAME	 "data/TEXTURE/effect000.jpg"				// テクスチャ名(.png ◎ / .jpg / .tga ◎ / .bmp / .dds ◎) ◎は半透明が使用可
#define DASHEFFECT_SIZE_VALUE    (0.1f)                                 // エフェクトのサイズの変化量

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTextureDashEffect   = NULL;		// テクスチャインターフェースのポインタ変数
LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferDashEffect = NULL;		// 頂点バッファ管理インターフェースポインタ

DASHEFFECT				DashEffect[ DASHEFFECT_MAX ];					// エフェクトの構造体
DASHEFFECTEMITTER		DashEffectEmitter[ DASHEFFECT_EMITTER_MAX ];	// エフェクトの発生源の構造体
int						g_nCntDashEffectPolygon = 0;					// エフェクトのポリゴン数（描画するエフェクトの数×2）

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
HRESULT MakeVertexDashEffect( LPDIRECT3DDEVICE9 pDevice );

/*------------------------------------------------------------------------------
  　パーティクルの初期化
------------------------------------------------------------------------------*/
void InitDashEffect( void )
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 初期値の設定
	for( int nCntDashEffect = 0; nCntDashEffect < DASHEFFECT_MAX; nCntDashEffect++ )
	{
		DashEffect[ nCntDashEffect ].bUse		= false;
		DashEffect[ nCntDashEffect ].fPos.x		= 0.0f;
		DashEffect[ nCntDashEffect ].fPos.y		= 0.0f;
		DashEffect[ nCntDashEffect ].fVelocity	= D3DXVECTOR2( 0.0f, 0.0f );
		DashEffect[ nCntDashEffect ].fSize.x	= DASHEFFECT_WIDTH;
		DashEffect[ nCntDashEffect ].fSize.y	= DASHEFFECT_HEIGHT;
		DashEffect[ nCntDashEffect ].fColor		= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		DashEffect[ nCntDashEffect ].fLength	= 0.0f;
		DashEffect[ nCntDashEffect ].fAngle		= 0.0f;
		DashEffect[ nCntDashEffect ].fRot		= 0.0f;
		DashEffect[ nCntDashEffect ].fScale		= 1.0f;
		DashEffect[ nCntDashEffect ].nLife		= 0;
	}

	// 頂点の作成
	MakeVertexPolygon( &g_pVtxBufferDashEffect, DASHEFFECT_MAX );

	// テクスチャの読み込み
	if( FAILED( D3DXCreateTextureFromFile( pDevice, DASHEFFECT_TEXTURENAME, &g_pTextureDashEffect ) ) )
	{
		MessageBox( NULL, "DashEffectのテクスチャの読み込みに失敗しました。", "エラーメッセージ", MB_OK | MB_ICONEXCLAMATION );
	}
}

/*------------------------------------------------------------------------------
  　パーティクルの終了処理
------------------------------------------------------------------------------*/
void UninitDashEffect( void )
{
	// テクスチャインターフェースの終了処理
	if( g_pTextureDashEffect )
	{
		g_pTextureDashEffect->Release();
		g_pTextureDashEffect = NULL;
	}

	// 頂点バッファ管理インターフェースポインタの終了処理
	if( g_pVtxBufferDashEffect )
	{
		g_pVtxBufferDashEffect->Release();
		g_pVtxBufferDashEffect = NULL;
	}
}

/*------------------------------------------------------------------------------
  　パーティクルの更新
------------------------------------------------------------------------------*/
void UpdateDashEffect( void )
{
	// カウンターの準備
	static int nFrameCount = 0;

	// プレイヤー情報の取得
	PLAYER* pPlayer = GetPlayer();

	// 座標更新
	for( int nCntDashEffect = 0; nCntDashEffect < DASHEFFECT_MAX; nCntDashEffect++ )
	{
		// 未使用中なら以下のプログラムを飛ばす
		if( !DashEffect[ nCntDashEffect ].bUse ) { continue; }
	}

	// 発生源からエフェクトを生成	
	for( int nCntDashEffectEmitter = 0; nCntDashEffectEmitter < DASHEFFECT_EMITTER_MAX; nCntDashEffectEmitter++ )
	{
		// 発生源が使われていたら
		if( DashEffectEmitter[ nCntDashEffectEmitter ].bUse )
		{	
			// エフェクトの生成
			for( int nCntDashEffect = 0; nCntDashEffect < DASHEFFECT_MAX; nCntDashEffect++ )
			{
				// エフェクトが使われていたら
				if( DashEffect[ nCntDashEffect ].bUse ) { continue; }
 
				DashEffect[ nCntDashEffect ].fPos	   = DashEffectEmitter[ nCntDashEffectEmitter ].fPos;
				DashEffect[ nCntDashEffect ].fSize	   = DashEffectEmitter[ nCntDashEffectEmitter ].fSize;
				DashEffect[ nCntDashEffect ].fVelocity = DashEffectEmitter[ nCntDashEffectEmitter ].fVelocity;
				DashEffect[ nCntDashEffect ].fAngle	   = DashEffectEmitter[ nCntDashEffectEmitter ].fAngle;
				DashEffect[ nCntDashEffect ].fLength   = DashEffectEmitter[ nCntDashEffectEmitter ].fLength;
				DashEffect[ nCntDashEffect ].nLife	   = DashEffectEmitter[ nCntDashEffectEmitter ].nLife;
				DashEffect[ nCntDashEffect ].nMaxLife  = DashEffectEmitter[ nCntDashEffectEmitter ].nLife;
				DashEffect[ nCntDashEffect ].bUse	   = true;

				break;
			}
		}
	}

	// 頂点バッファ格納変数
	VERTEX_2D* pVtx = NULL;

	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferDashEffect->Lock( 0, 0, ( void** )&pVtx, 0 );
	
	// エフェクトの更新
	for( int nCntDashEffect = 0; nCntDashEffect < DASHEFFECT_MAX; nCntDashEffect++ )
	{
		if( !DashEffect[ nCntDashEffect ].bUse ) { continue; }
		
		// 中心座標、体力の更新		
		DashEffect[ nCntDashEffect ].fPos += DashEffect[ nCntDashEffect ].fVelocity; 
		DashEffect[ nCntDashEffect ].nLife--;

		// 座標の更新
		//DashEffectEmitter[ nCntDashEffect ].fPos		 = pPlayer->fPos;

		// カウンターのインクリメント
		DashEffect[ nCntDashEffect ].nCounter++;

		// サイズの更新
		if( DashEffect[ nCntDashEffect ].nCounter < DashEffect[ nCntDashEffect ].nMaxLife * 0.5f )
		{
			DashEffect[ nCntDashEffect ].fSize.x += DASHEFFECT_SIZE_VALUE;
			DashEffect[ nCntDashEffect ].fSize.y += DASHEFFECT_SIZE_VALUE;
		}
		else
		{
			DashEffect[ nCntDashEffect ].fSize.x -= DASHEFFECT_SIZE_VALUE;
			DashEffect[ nCntDashEffect ].fSize.y -= DASHEFFECT_SIZE_VALUE;
		}

		// 回転関係の更新
		DashEffect[ nCntDashEffect ].fLength = sqrt( DashEffect[ nCntDashEffect ].fSize.x * DashEffect[ nCntDashEffect ].fSize.x + DashEffect[ nCntDashEffect ].fSize.y * DashEffect[ nCntDashEffect ].fSize.y );
		DashEffect[ nCntDashEffect ].fAngle	 = atan2( DashEffect[ nCntDashEffect ].fSize.y, DashEffect[ nCntDashEffect ].fSize.x );

		// 体力がなくなったら消去
		if( DashEffect[ nCntDashEffect ].nLife <= 0 )
		{
			DashEffect[ nCntDashEffect ].bUse		= false;
			DashEffect[ nCntDashEffect ].fPos.x		= 0.0f;
			DashEffect[ nCntDashEffect ].fPos.y		= 0.0f;
			DashEffect[ nCntDashEffect ].fVelocity	= D3DXVECTOR2( 0.0f, 0.0f );
			DashEffect[ nCntDashEffect ].nCounter	= 0;
		}

		// 四隅の座標の更新
		DashEffect[ nCntDashEffect ].fLeftUp.x    = DashEffect[ nCntDashEffect ].fPos.x + cos( DashEffect[ nCntDashEffect ].fRot + D3DX_PI + DashEffect[ nCntDashEffect ].fAngle ) * DashEffect[ nCntDashEffect ].fLength;		// 左上の頂点X
		DashEffect[ nCntDashEffect ].fLeftUp.y    = DashEffect[ nCntDashEffect ].fPos.y + sin( DashEffect[ nCntDashEffect ].fRot + D3DX_PI + DashEffect[ nCntDashEffect ].fAngle ) * DashEffect[ nCntDashEffect ].fLength;		// 左上の頂点Y		
		DashEffect[ nCntDashEffect ].fRightUp.x   = DashEffect[ nCntDashEffect ].fPos.x + cos( DashEffect[ nCntDashEffect ].fRot		   - DashEffect[ nCntDashEffect ].fAngle ) * DashEffect[ nCntDashEffect ].fLength;		// 右上の頂点X
		DashEffect[ nCntDashEffect ].fRightUp.y   = DashEffect[ nCntDashEffect ].fPos.y + sin( DashEffect[ nCntDashEffect ].fRot		   - DashEffect[ nCntDashEffect ].fAngle ) * DashEffect[ nCntDashEffect ].fLength;		// 右上の頂点Y
		DashEffect[ nCntDashEffect ].fLeftDown.x  = DashEffect[ nCntDashEffect ].fPos.x + cos( DashEffect[ nCntDashEffect ].fRot + D3DX_PI - DashEffect[ nCntDashEffect ].fAngle ) * DashEffect[ nCntDashEffect ].fLength;		// 左下の頂点X
		DashEffect[ nCntDashEffect ].fLeftDown.y  = DashEffect[ nCntDashEffect ].fPos.y + sin( DashEffect[ nCntDashEffect ].fRot + D3DX_PI - DashEffect[ nCntDashEffect ].fAngle ) * DashEffect[ nCntDashEffect ].fLength;		// 左下の頂点Y
		DashEffect[ nCntDashEffect ].fRightDown.x = DashEffect[ nCntDashEffect ].fPos.x + cos( DashEffect[ nCntDashEffect ].fRot		   + DashEffect[ nCntDashEffect ].fAngle ) * DashEffect[ nCntDashEffect ].fLength;		// 右下の頂点X
		DashEffect[ nCntDashEffect ].fRightDown.y = DashEffect[ nCntDashEffect ].fPos.y + sin( DashEffect[ nCntDashEffect ].fRot		   + DashEffect[ nCntDashEffect ].fAngle ) * DashEffect[ nCntDashEffect ].fLength;		// 右下の頂点Y
		
		// 頂点座標の設定
		pVtx[ 0 ].pos.x = DashEffect[ nCntDashEffect ].fLeftUp.x;		// 左上の頂点X
		pVtx[ 0 ].pos.y = DashEffect[ nCntDashEffect ].fLeftUp.y;		// 左上の頂点Y		
		pVtx[ 1 ].pos.x = DashEffect[ nCntDashEffect ].fRightUp.x;		// 右上の頂点X
		pVtx[ 1 ].pos.y = DashEffect[ nCntDashEffect ].fRightUp.y;		// 右上の頂点Y
		pVtx[ 2 ].pos.x = DashEffect[ nCntDashEffect ].fLeftDown.x;		// 左下の頂点X
		pVtx[ 2 ].pos.y = DashEffect[ nCntDashEffect ].fLeftDown.y;		// 左下の頂点Y
		pVtx[ 3 ].pos.x = DashEffect[ nCntDashEffect ].fRightDown.x;	// 右下の頂点X
		pVtx[ 3 ].pos.y = DashEffect[ nCntDashEffect ].fRightDown.y;	// 右下の頂点Y
		
		// テクスチャ座標の設定( 0.0f～1.0f )
		pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );	// 左上の座標
		pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );	// 右上の座標
		pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );	// 左下の座標
		pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );	// 左下の座標
		
		// 頂点色の設定
		pVtx[ 0 ].color = DashEffect[ nCntDashEffect ].fColor;
		pVtx[ 1 ].color = DashEffect[ nCntDashEffect ].fColor;
		pVtx[ 2 ].color = DashEffect[ nCntDashEffect ].fColor;
		pVtx[ 3 ].color = DashEffect[ nCntDashEffect ].fColor;

		// 次のポリゴンの頂点へ
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBufferDashEffect->Unlock();

	// カウンターのインクリメント
	nFrameCount++;
}

/*------------------------------------------------------------------------------
  　描画処理
------------------------------------------------------------------------------*/
void DrawDashEffect( void )
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 加算合成に切り替え
	pDevice->SetRenderState( D3DRS_DESTBLEND,		// 設定名(インデックス)	D3DRS_DESTBLEND：背景のアルファ設定
							   D3DBLEND_ONE );		// 設定値

	// ストリーム(パイプライン)設定
	pDevice->SetStreamSource( 0,						// 繋げるパイプの番号
							  g_pVtxBufferDashEffect,		// ストリームの元となる頂点バッファの指定
							  0,						// オフセット(バイト)
							  sizeof( VERTEX_2D ) );		// ストライド値(ストライド量)

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_2D );

	// テクスチャの設定
	pDevice->SetTexture( 0, g_pTextureDashEffect );
	
	for( int nCntDashEffect = 0; nCntDashEffect < DASHEFFECT_MAX; nCntDashEffect++ )
	{
		if( !DashEffect[ nCntDashEffect ].bUse ) { continue; }
				
		// プリミティブの描画
		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,		// プリミティブの種類
								nCntDashEffect * 4,			// オフセット(頂点数)
								2 );						// プリミティブ数
	}

	// 乗算合成に切り替え
	pDevice->SetRenderState( D3DRS_DESTBLEND,			// 設定名(インデックス)	D3DRS_DESTBLEND：背景のアルファ設定
						     D3DBLEND_INVSRCALPHA );	// 設定値	D3DBLEND_INVSRCALPHA：１－α

}

/*------------------------------------------------------------------------------
  　エフェクトの発生源の作成
------------------------------------------------------------------------------*/
void CreateDashEffectEmitter( D3DXVECTOR2 pos, D3DXVECTOR2 size, D3DXVECTOR2 velocity, int life )
{
	for( int nCntDashEffectEmitter = 0; nCntDashEffectEmitter < DASHEFFECT_EMITTER_MAX; nCntDashEffectEmitter++ )
	{
		// 発生源が使われていなかったら
		if( !DashEffectEmitter[ nCntDashEffectEmitter ].bUse )
		{
			DashEffectEmitter[ nCntDashEffectEmitter ].bUse		 = true;
			DashEffectEmitter[ nCntDashEffectEmitter ].fPos		 = pos;
			DashEffectEmitter[ nCntDashEffectEmitter ].fSize	 = size;
			DashEffectEmitter[ nCntDashEffectEmitter ].fLength	 = sqrt( size.x * size.x + size.y * size.y );
			DashEffectEmitter[ nCntDashEffectEmitter ].fAngle	 = atan2( size.y, size.x );
			DashEffectEmitter[ nCntDashEffectEmitter ].fVelocity = velocity;
			DashEffectEmitter[ nCntDashEffectEmitter ].nLife	 = life;

			break;	// for文を抜ける
		}
	}
}

/*------------------------------------------------------------------------------
  　エフェクトの発生源の消去
------------------------------------------------------------------------------*/
void UnuseDashEffect( void )
{
	for( int nCntDashEffectEmitter = 0; nCntDashEffectEmitter < DASHEFFECT_EMITTER_MAX; nCntDashEffectEmitter++ )
	{
		// 発生源が使われていたら
		if( DashEffectEmitter[ nCntDashEffectEmitter ].bUse )
		{
			DashEffectEmitter[ nCntDashEffectEmitter ].bUse		 = false;
			DashEffectEmitter[ nCntDashEffectEmitter ].fPos		 = D3DXVECTOR2( 0.0f, 0.0f );
			DashEffectEmitter[ nCntDashEffectEmitter ].fSize	 = D3DXVECTOR2( 0.0f, 0.0f );
			DashEffectEmitter[ nCntDashEffectEmitter ].fLength	 = 0.0f;
			DashEffectEmitter[ nCntDashEffectEmitter ].fAngle	 = 0.0f;
			DashEffectEmitter[ nCntDashEffectEmitter ].fVelocity = D3DXVECTOR2( 0.0f, 0.0f );
			DashEffectEmitter[ nCntDashEffectEmitter ].nLife	 = 0;
		}
	}
}

/*------------------------------------------------------------------------------
  　エフェクトの発生源の移動
------------------------------------------------------------------------------*/
void MoveDashEffectEmitter( float movey )
{
	for( int nCntDashEffectEmitter = 0; nCntDashEffectEmitter < DASHEFFECT_EMITTER_MAX; nCntDashEffectEmitter++ ) 
	{
		DashEffectEmitter[ nCntDashEffectEmitter ].fPos.y += movey;
	}
}

//******************************* EOF **************************************