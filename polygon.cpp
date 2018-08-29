/*==============================================================================

    ポリゴン処理 [polygon.cpp]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  　ヘッダファイル
------------------------------------------------------------------------------*/
#include "main.h"
#include "polygon.h"

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
  　終了処理
------------------------------------------------------------------------------*/
void UninitPolygon( LPDIRECT3DTEXTURE9* pTexture, LPDIRECT3DVERTEXBUFFER9* pVtxBuffer )
{
	// テクスチャインターフェースの終了処理
	SAFE_RELEASE( *pTexture );
	*pTexture = NULL;

	// 頂点バッファ管理インターフェースの終了処理
	SAFE_RELEASE( *pVtxBuffer );
	*pVtxBuffer = NULL;
}

/*------------------------------------------------------------------------------
  　更新処理
------------------------------------------------------------------------------*/
void UpdatePolygon( LPDIRECT3DVERTEXBUFFER9 pVtxBuffer, float fPosX, float fPosY, float fWidth, float fHeight, float fTexX, float fTexY, float fTexWidth, float fTexHeight )
{
	VERTEX_2D* pVtx = NULL; // VRAMの仮想アドレスを格納
	
	// 頂点バッファをロックして、仮想アドレスを取得する
	pVtxBuffer->Lock( 0,
					  0,				// すべてをロック
					  ( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
					  0 );			
	
	// 頂点座標の設定( 2D座標は右回りに頂点を用意する )
	pVtx[ 0 ].pos = D3DXVECTOR3( fPosX, fPosY, 0.0f );						// 左上の頂点
	pVtx[ 1 ].pos = D3DXVECTOR3( fPosX + fWidth, fPosY, 0.0f );				// 右上の頂点
	pVtx[ 2 ].pos = D3DXVECTOR3( fPosX, fPosY + fHeight, 0.0f );			// 左下の頂点
	pVtx[ 3 ].pos = D3DXVECTOR3( fPosX + fWidth, fPosY + fHeight, 0.0f );	// 右下の頂点

	// UV座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( fTexX, fTexY );							// 左上の頂点
	pVtx[ 1 ].tex = D3DXVECTOR2( fTexX + fTexWidth, fTexY );				// 右上の頂点
	pVtx[ 2 ].tex = D3DXVECTOR2( fTexX, fTexY + fTexHeight );				// 左下の頂点	
	pVtx[ 3 ].tex = D3DXVECTOR2( fTexX + fTexWidth, fTexY + fTexHeight );	// 右下の頂点

	// 頂点バッファのアンロック
	pVtxBuffer->Unlock();
}

/*------------------------------------------------------------------------------
  　描画処理
------------------------------------------------------------------------------*/
void DrawPolygon( LPDIRECT3DTEXTURE9 pTexture, LPDIRECT3DVERTEXBUFFER9 pVtxBuffer, int nNumPolygon )
{
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// パイプライン（ストリーム）を設定
	pDevice->SetStreamSource( 0,						// パイプライン番号
							  pVtxBuffer,				// パイプラインの元になる領域を指定
							  0,						// オフセット（単位はbyte）※ストリームの開始位置を指定できる
							  sizeof( VERTEX_2D ) );	// 流すデータの単位サイズ（ストライド量）

	// テクスチャの設定
	pDevice->SetTexture( 0, pTexture );

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_2D );

	// プリミティブ（Polygon）の描画
	for( int nCnt = 0; nCnt < nNumPolygon; nCnt++ )
	{
		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, // プリミティブの種類
								nCnt * 4,			 // 描画開始位置のオフセット（頂点数）
								NUM_POLYGON );		 // 描画するプリミティブの数
	}
}

/*------------------------------------------------------------------------------
  　頂点の作成
------------------------------------------------------------------------------*/
HRESULT MakeVertexPolygon( LPDIRECT3DVERTEXBUFFER9* pVtxBuffer, int nNumPolygon )
{
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// *** 頂点バッファの作成 ***
	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_2D ) * NUM_VERTEX * nNumPolygon,  // 作成したい頂点バッファの合計データサイズ
											 D3DUSAGE_WRITEONLY,						 	  // 頂点バッファの使用方法（書き込みのみ）
											 FVF_VERTEX_2D,								      // ？
											 D3DPOOL_MANAGED,								  // 頂点バッファの置き場となるメモリを指定（デバイスに委託）
											 pVtxBuffer,									  // 頂点バッファ管理インターフェースのポインタ（インターフェースの格納）
											 NULL ) ) )										  // 使わない
											 {
												 return E_FAIL;
											 }											
	
	// *** 頂点バッファの初期化 ***										

	VERTEX_2D* pVtx; // VRAMの仮想アドレスを格納

	// 頂点バッファをロックして、仮想アドレスを取得する
	if( FAILED ( (*pVtxBuffer)->Lock( 0,
									  0,				// すべてをロック
								      ( void** )&pVtx,  // 仮想アドレス用ポインタのアドレスを渡す
									  0 ) ) )			// ロックフラグ？
								      {	
										  return E_FAIL;
									  }					

	for( int nCnt = 0; nCnt < nNumPolygon; nCnt++ )
	{
		// 頂点座標の設定( 2D座標・右回り )
		pVtx[ 0 ].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );	// 左上の座標
		pVtx[ 1 ].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );	// 右上の座標
		pVtx[ 2 ].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );	// 左下の座標
		pVtx[ 3 ].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );	// 左下の座標

		// rhwの設定（必ず1.0f）※手前か奥（バッファ）の情報？
		pVtx[ 0 ].rhw = 1.0f;	// 左上の頂点
		pVtx[ 1 ].rhw = 1.0f;	// 右上の頂点
		pVtx[ 2 ].rhw = 1.0f;	// 左下の頂点
		pVtx[ 3 ].rhw = 1.0f;	// 右下の頂点

		// 頂点カラーの設定（ 0 ～ 255の整数値 )
		pVtx[ 0 ].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );	// 左上の頂点
		pVtx[ 1 ].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );	// 右上の頂点
		pVtx[ 2 ].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );	// 左下の頂点
		pVtx[ 3 ].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );	// 右下の頂点
	
		// テクスチャ座標の設定( 0.0f～1.0f )
		pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );	// 左上の座標
		pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );	// 右上の座標
		pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );	// 左下の座標
		pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );	// 左下の座標

		pVtx += 4;
	}

	// 頂点バッファのロックを解除
	( *pVtxBuffer )->Unlock();

	return S_OK;
	}