/*==============================================================================

    パーティクルの処理 [particle.cpp]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  　ヘッダファイル
------------------------------------------------------------------------------*/
#include "main.h"
#include "particle.h"
#include "polygon.h"
#include "player.h"

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define PARTICLE_TEXTURENAME	"data/TEXTURE/explosion001.png"	// テクスチャ名(.png ◎ / .jpg / .tga ◎ / .bmp / .dds ◎) ◎は半透明が使用可
#define PARTICLE_MAX			(2048)							// エフェクトの最大数

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTextureParticle   = NULL;		// テクスチャインターフェースのポインタ変数
LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferParticle = NULL;	// 頂点バッファ管理インターフェースポインタ

PARTICLE				Particle[ PARTICLE_MAX ];		// エフェクトの構造体
int						g_nCntParticlePolygon = 0;	// エフェクトのポリゴン数（描画するエフェクトの数×2）

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
HRESULT MakeVertexParticle( LPDIRECT3DDEVICE9 pDevice );

/*------------------------------------------------------------------------------
  　パーティクルの初期化
------------------------------------------------------------------------------*/
void InitParticle( void )
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 初期値の設定
	for( int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++ )
	{
		Particle[ nCntParticle ].bUse			= false;
		Particle[ nCntParticle ].fPos.x			= 0.0f;
		Particle[ nCntParticle ].fPos.y			= 0.0f;
		Particle[ nCntParticle ].fSize.x		= PARTICLE_WIDTH;
		Particle[ nCntParticle ].fSize.y		= PARTICLE_HEIGHT;
		Particle[ nCntParticle ].fRot			= 0.0f;
		Particle[ nCntParticle ].fLength		= 0.0f;
		Particle[ nCntParticle ].fAngle			= 0.0f;
		Particle[ nCntParticle ].fScale			= 1.0f;
		Particle[ nCntParticle ].fSpeed			= 0.0f;
		Particle[ nCntParticle ].fVelocity.x	= 0.0f;
		Particle[ nCntParticle ].fVelocity.y	= 0.0f;
		Particle[ nCntParticle ].nLife			= 0;
	}

	// 頂点の作成
	MakeVertexPolygon( &g_pVtxBufferParticle, PARTICLE_MAX );

	// テクスチャの読み込み
	if( FAILED( D3DXCreateTextureFromFile( pDevice, PARTICLE_TEXTURENAME, &g_pTextureParticle ) ) )
	{
		MessageBox( NULL, "Particleのテクスチャの読み込みに失敗しました。", "エラーメッセージ", MB_OK | MB_ICONEXCLAMATION );
	}
}

/*------------------------------------------------------------------------------
  　パーティクルの終了処理
------------------------------------------------------------------------------*/
void UninitParticle( void )
{
	// テクスチャインターフェースの終了処理
	if( g_pTextureParticle )
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}

	// 頂点バッファ管理インターフェースポインタの終了処理
	if( g_pVtxBufferParticle )
	{
		g_pVtxBufferParticle->Release();
		g_pVtxBufferParticle = NULL;
	}
}

/*------------------------------------------------------------------------------
  　パーティクルの更新
------------------------------------------------------------------------------*/
void UpdateParticle( void )
{
	// プレイヤー情報の取得
	PLAYER* pPlayer = GetPlayer();

	// 座標更新
	for( int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++ )
	{
		// 未使用中なら以下のプログラムを飛ばす
		if( !Particle[ nCntParticle ].bUse ) { continue; }

		Particle[ nCntParticle ].fPos.x += Particle[ nCntParticle ].fVelocity.x;
		Particle[ nCntParticle ].fPos.y += Particle[ nCntParticle ].fVelocity.y;
		Particle[ nCntParticle ].fColor += Particle[ nCntParticle ].fColorValue;
		Particle[ nCntParticle ].nLife--;

		// 体力がなくなったら消す
		if( Particle[ nCntParticle ].nLife <= 0 )
		{
			Particle[ nCntParticle ].bUse		= false;
			Particle[ nCntParticle ].fPos.x		= 0.0f;
			Particle[ nCntParticle ].fPos.y		= 0.0f;
			Particle[ nCntParticle ].nLife		= 0;

			continue;
		}

		// 画面外(左右)に出たら消す
		if( Particle[ nCntParticle ].fPos.x - Particle[ nCntParticle ].fLength < 0.0f ||
			Particle[ nCntParticle ].fPos.x + Particle[ nCntParticle ].fLength > SCREEN_WIDTH )
		{
			Particle[ nCntParticle ].bUse		= false;
			Particle[ nCntParticle ].fPos.x		= 0.0f;
			Particle[ nCntParticle ].fPos.y		= 0.0f;
			Particle[ nCntParticle ].nLife		= 0;

			continue;
		}

		// 画面外(上下)に出たら消す
		if( Particle[ nCntParticle ].fPos.y - Particle[ nCntParticle ].fLength < 0.0f ||
			Particle[ nCntParticle ].fPos.y + Particle[ nCntParticle ].fLength > SCREEN_HEIGHT )
		{
			Particle[ nCntParticle ].bUse		= false;
			Particle[ nCntParticle ].fPos.x		= 0.0f;
			Particle[ nCntParticle ].fPos.y		= 0.0f;
			Particle[ nCntParticle ].nLife		= 0;

			continue;
		}
	}
	
	// 頂点バッファ格納変数
	VERTEX_2D* pVtx = NULL;

	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferParticle->Lock( 0, 0, (void**)&pVtx, 0 );
	
	// 更新
	for( int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++ )
	{
		if( !Particle[ nCntParticle ].bUse ) { continue; }
				
		// 四隅の座標の更新
		Particle[ nCntParticle ].fLeftUp.x    = Particle[ nCntParticle ].fPos.x + cos( Particle[ nCntParticle ].fRot + D3DX_PI + Particle[ nCntParticle ].fAngle ) * Particle[ nCntParticle ].fLength;		// 左上の頂点X
		Particle[ nCntParticle ].fLeftUp.y    = Particle[ nCntParticle ].fPos.y + sin( Particle[ nCntParticle ].fRot + D3DX_PI + Particle[ nCntParticle ].fAngle ) * Particle[ nCntParticle ].fLength;		// 左上の頂点Y		
		Particle[ nCntParticle ].fRightUp.x   = Particle[ nCntParticle ].fPos.x + cos( Particle[ nCntParticle ].fRot		   - Particle[ nCntParticle ].fAngle ) * Particle[ nCntParticle ].fLength;		// 右上の頂点X
		Particle[ nCntParticle ].fRightUp.y   = Particle[ nCntParticle ].fPos.y + sin( Particle[ nCntParticle ].fRot		   - Particle[ nCntParticle ].fAngle ) * Particle[ nCntParticle ].fLength;		// 右上の頂点Y
		Particle[ nCntParticle ].fLeftDown.x  = Particle[ nCntParticle ].fPos.x + cos( Particle[ nCntParticle ].fRot + D3DX_PI - Particle[ nCntParticle ].fAngle ) * Particle[ nCntParticle ].fLength;		// 左下の頂点X
		Particle[ nCntParticle ].fLeftDown.y  = Particle[ nCntParticle ].fPos.y + sin( Particle[ nCntParticle ].fRot + D3DX_PI - Particle[ nCntParticle ].fAngle ) * Particle[ nCntParticle ].fLength;		// 左下の頂点Y
		Particle[ nCntParticle ].fRightDown.x = Particle[ nCntParticle ].fPos.x + cos( Particle[ nCntParticle ].fRot		   + Particle[ nCntParticle ].fAngle ) * Particle[ nCntParticle ].fLength;		// 右下の頂点X
		Particle[ nCntParticle ].fRightDown.y = Particle[ nCntParticle ].fPos.y + sin( Particle[ nCntParticle ].fRot		   + Particle[ nCntParticle ].fAngle ) * Particle[ nCntParticle ].fLength;		// 右下の頂点Y
		
		// 頂点座標の設定
		pVtx[ 0 ].pos.x = Particle[ nCntParticle ].fLeftUp.x;		// 左上の頂点X
		pVtx[ 0 ].pos.y = Particle[ nCntParticle ].fLeftUp.y;		// 左上の頂点Y		
		pVtx[ 1 ].pos.x = Particle[ nCntParticle ].fRightUp.x;		// 右上の頂点X
		pVtx[ 1 ].pos.y = Particle[ nCntParticle ].fRightUp.y;		// 右上の頂点Y
		pVtx[ 2 ].pos.x = Particle[ nCntParticle ].fLeftDown.x;		// 左下の頂点X
		pVtx[ 2 ].pos.y = Particle[ nCntParticle ].fLeftDown.y;		// 左下の頂点Y
		pVtx[ 3 ].pos.x = Particle[ nCntParticle ].fRightDown.x;	// 右下の頂点X
		pVtx[ 3 ].pos.y = Particle[ nCntParticle ].fRightDown.y;	// 右下の頂点Y
		
		// テクスチャ座標の設定( 0.0f～1.0f )
		pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );		// 左上の座標
		pVtx[ 1 ].tex = D3DXVECTOR2( 0.125f, 0.0f );	// 右上の座標
		pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );		// 左下の座標
		pVtx[ 3 ].tex = D3DXVECTOR2( 0.125f, 1.0f );	// 左下の座標
		
		// 頂点色の設定
		pVtx[ 0 ].color = Particle[ nCntParticle ].fColor;
		pVtx[ 1 ].color = Particle[ nCntParticle ].fColor;
		pVtx[ 2 ].color = Particle[ nCntParticle ].fColor;
		pVtx[ 3 ].color = Particle[ nCntParticle ].fColor;

		// 次のポリゴンの頂点へ
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBufferParticle->Unlock();
}

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
void DrawParticle( void )
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 加算合成に切り替え
	//pDevice->SetRenderState( D3DRS_DESTBLEND,		// 設定名(インデックス)	D3DRS_DESTBLEND：背景のアルファ設定
	//						   D3DBLEND_ONE );		// 設定値

	// ストリーム(パイプライン)設定
	pDevice->SetStreamSource( 0,						// 繋げるパイプの番号
							  g_pVtxBufferParticle,		// ストリームの元となる頂点バッファの指定
							  0,						// オフセット(バイト)
							  sizeof( VERTEX_2D ) );		// ストライド値(ストライド量)

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_2D );

	// テクスチャの設定
	pDevice->SetTexture( 0, g_pTextureParticle );
	
	for( int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++ )
	{
		if( !Particle[ nCntParticle ].bUse ) { continue; }
				
		// プリミティブの描画
		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,		// プリミティブの種類
								nCntParticle * 4,			// オフセット(頂点数)
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
void CreateParticle( D3DXVECTOR2 pos, D3DXVECTOR2 size, float speed, float angleOfFire, D3DXCOLOR color, D3DXCOLOR colorValue,  int life )
{
	for( int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++ )
	{
		// エフェクトが使われていなかったら
		if( !Particle[ nCntParticle ].bUse )
		{
			Particle[ nCntParticle ].bUse			= true;
			Particle[ nCntParticle ].fPos			= pos;
			Particle[ nCntParticle ].fLength		= sqrt( ( size.y * size.y ) + ( size.x * size.x  ) );
			Particle[ nCntParticle ].fAngle			= atan2( size.y, size.x );
			Particle[ nCntParticle ].fVelocity.x	= speed * cos( angleOfFire );
			Particle[ nCntParticle ].fVelocity.y	= speed * sin( angleOfFire );
			Particle[ nCntParticle ].fColor			= color;
			Particle[ nCntParticle ].fColorValue	= colorValue;
			Particle[ nCntParticle ].nLife			= life;

			break;	// for文を抜ける
		}
	}
}

//******************************* EOF **************************************