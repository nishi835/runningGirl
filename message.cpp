/*==============================================================================

    メッセージの処理 [message.cpp]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/

/*------------------------------------------------------------------------------
  　ヘッダファイル
------------------------------------------------------------------------------*/
#include "main.h"
#include "message.h"
#include "polygon.h"
#include "player.h"
#include "input.h"
#include "game.h"
#include "fade.h"

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define MESSAGE_TEXTURENAME_STAGE	 "data/TEXTURE/stage.png"	// テクスチャ名(.png ◎ / .jpg / .tga ◎ / .bmp / .dds ◎) ◎は半透明が使用可
#define MESSAGE_TEXTURENAME_READY	 "data/TEXTURE/ready.png"	// テクスチャ名(.png ◎ / .jpg / .tga ◎ / .bmp / .dds ◎) ◎は半透明が使用可
#define MESSAGE_TEXTURENAME_GO       "data/TEXTURE/go.png"	    // テクスチャ名
#define MESSAGE_TEXTURENAME_GOAL     "data/TEXTURE/goal.png"	    // テクスチャ名
#define MESSAGE_TEXTURENAME_GAMEOVER "data/TEXTURE/gameover.png"
#define MESSAGE_TEXTURENAME_TIMEUP   "data/TEXTURE/timeup.png"
#define MESSAGE_MAX					(1)					        // メッセージの最大数
#define MESSAGE_TEX_MAX				(6)					        // テクスチャの最大数

/*------------------------------------------------------------------------------
  　グローバル変数
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9		g_pTextureMessage[ MESSAGE_TEX_MAX ] = {};		// テクスチャインターフェースのポインタ変数
LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferMessage					 = NULL;	// 頂点バッファ管理インターフェースポインタ

MESSAGE					Message[ MESSAGE_MAX ];		// メッセージの構造体
int						g_nCntMessagePolygon = 0;	// メッセージのポリゴン数（描画するメッセージの数×2）
int						g_nFrameCount = 0;			// タイマー

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
HRESULT MakeVertexMessage( LPDIRECT3DDEVICE9 pDevice );

/*------------------------------------------------------------------------------
  　メッセージの初期化
------------------------------------------------------------------------------*/
void InitMessage( void )
{
	// カウンターの初期化
	g_nFrameCount = 0;

	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 初期値の設定
	for( int nCntMessage = 0; nCntMessage < MESSAGE_MAX; nCntMessage++ )
	{
		Message[ nCntMessage ].bUse			= false;
		Message[ nCntMessage ].fPos.x		= 0.0f;
		Message[ nCntMessage ].fPos.y		= 0.0f;
		Message[ nCntMessage ].fSize.x		= MESSAGE_WIDTH;
		Message[ nCntMessage ].fSize.y		= MESSAGE_HEIGHT;
		Message[ nCntMessage ].fRot			= 0.0f;
		Message[ nCntMessage ].fLength		= 0.0f;
		Message[ nCntMessage ].fAngle		= 0.0f;
		Message[ nCntMessage ].fScale		= 1.0f;
		Message[ nCntMessage ].fSpeed		= 0.0f;
		Message[ nCntMessage ].fVelocity.x	= 0.0f;
		Message[ nCntMessage ].fVelocity.y	= 0.0f;
		Message[ nCntMessage ].nCounter		= 0;
		Message[ nCntMessage ].nLife		= 0;
		Message[ nCntMessage ].nType		= MESSAGE_TYPE_STAGE;
	}

	// 頂点の作成
	MakeVertexPolygon( &g_pVtxBufferMessage, MESSAGE_MAX );

	// テクスチャの読み込み
	if( FAILED( D3DXCreateTextureFromFile( pDevice, MESSAGE_TEXTURENAME_STAGE, &g_pTextureMessage[ 0 ] ) ) )
	{
		MessageBox( NULL, "Messageのテクスチャの読み込みに失敗しました。", "エラーメッセージ", MB_OK | MB_ICONEXCLAMATION );
	}

	if( FAILED( D3DXCreateTextureFromFile( pDevice, MESSAGE_TEXTURENAME_READY, &g_pTextureMessage[ 1 ] ) ) )
	{
		MessageBox( NULL, "Messageのテクスチャの読み込みに失敗しました。", "エラーメッセージ", MB_OK | MB_ICONEXCLAMATION );
	}

	if( FAILED( D3DXCreateTextureFromFile( pDevice, MESSAGE_TEXTURENAME_GO, &g_pTextureMessage[ 2 ] ) ) )
	{
		MessageBox( NULL, "Messageのテクスチャの読み込みに失敗しました。", "エラーメッセージ", MB_OK | MB_ICONEXCLAMATION );
	}

	if( FAILED( D3DXCreateTextureFromFile( pDevice, MESSAGE_TEXTURENAME_GOAL, &g_pTextureMessage[ 3 ] ) ) )
	{
		MessageBox( NULL, "Messageのテクスチャの読み込みに失敗しました。", "エラーメッセージ", MB_OK | MB_ICONEXCLAMATION );
	}
	
	if( FAILED( D3DXCreateTextureFromFile( pDevice, MESSAGE_TEXTURENAME_TIMEUP, &g_pTextureMessage[ 4 ] ) ) )
	{
		MessageBox( NULL, "Messageのテクスチャの読み込みに失敗しました。", "エラーメッセージ", MB_OK | MB_ICONEXCLAMATION );
	}

	if( FAILED( D3DXCreateTextureFromFile( pDevice, MESSAGE_TEXTURENAME_GAMEOVER, &g_pTextureMessage[ 5 ] ) ) )
	{
		MessageBox( NULL, "Messageのテクスチャの読み込みに失敗しました。", "エラーメッセージ", MB_OK | MB_ICONEXCLAMATION );
	}
}

/*------------------------------------------------------------------------------
  　メッセージの終了処理
------------------------------------------------------------------------------*/
void UninitMessage( void )
{
	// テクスチャインターフェースの終了処理
	for( int nCnt = 0; nCnt < MESSAGE_TEX_MAX; nCnt++ )
	{
		if( g_pTextureMessage[ nCnt ] != NULL )
		{
			g_pTextureMessage[ nCnt ]->Release();
			g_pTextureMessage[ nCnt ] = NULL;
		}
	}

	// 頂点バッファ管理インターフェースポインタの終了処理
	if( g_pVtxBufferMessage )
	{
		g_pVtxBufferMessage->Release();
		g_pVtxBufferMessage = NULL;
	}
}

/*------------------------------------------------------------------------------
  　メッセージの更新
------------------------------------------------------------------------------*/
void UpdateMessage( void )
{
	// ステージメッセージ
	if( g_nFrameCount == 0 )
	{
		CreateMessage( D3DXVECTOR2( 800.0f, 250.0f ), D3DXVECTOR2( MESSAGE_READY_WIDTH, 80.0f ), 2.0f, D3DX_PI, 0.0f, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0 ), D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0 ), MESSAGE_TYPE_STAGE );
	}

	// 座標更新
	for( int nCntMessage = 0; nCntMessage < MESSAGE_MAX; nCntMessage++ )
	{
		// 未使用中なら以下のプログラムを飛ばす
		if( !Message[ nCntMessage ].bUse ) { continue; }

		if( Message[ 0 ].nType == MESSAGE_TYPE_TIMEUP || Message[ 0 ].nType == MESSAGE_TYPE_GAMEOVER ) { continue; }

		if( Message[ nCntMessage ].fPos.x > SCREEN_WIDTH * 0.5f )
		{
			if( GetKeyboardTrigger( DIK_SPACE ) )
			{
				Message[ nCntMessage ].fPos.x = SCREEN_WIDTH * 0.5f;
			}

			Message[ nCntMessage ].fVelocity.x += 1.0f * cosf( Message[ nCntMessage ].fDirect );
			Message[ nCntMessage ].fPos.x += Message[ nCntMessage ].fVelocity.x;
			Message[ nCntMessage ].fPos.y += Message[ nCntMessage ].fVelocity.y;
		}
		else
		{						
			if( GetKeyboardTrigger( DIK_SPACE ) && Message[ nCntMessage ].nCounter && Message[ nCntMessage ].nType != MESSAGE_TYPE_GO )
			{
				Message[ nCntMessage ].nType++;
				Message[ nCntMessage ].nCounter = 0;
			}

			Message[ nCntMessage ].nCounter++;

			// 画面中央でメッセージを止める
			if( Message[ nCntMessage ].nCounter < 60 || Message[ nCntMessage ].nType == MESSAGE_TYPE_GOAL )
			{
				Message[ nCntMessage ].fVelocity.x = 0;
			}
			else
			{
				// メッセージを飛ばす
				if( GetKeyboardTrigger( DIK_SPACE ) && Message[ nCntMessage ].nType != MESSAGE_TYPE_GO )
				{
					Message[ nCntMessage ].fPos.x = SCREEN_WIDTH * 0.5;
					Message[ nCntMessage ].nType++;
					Message[ nCntMessage ].nCounter = 0;
				}
				// メッセージをスクロールさせる
				else
				{
					Message[ nCntMessage ].fVelocity.x += 1.0f * cosf( Message[ nCntMessage ].fDirect );
					Message[ nCntMessage ].fPos.x += Message[ nCntMessage ].fVelocity.x;
					Message[ nCntMessage ].fPos.y += Message[ nCntMessage ].fVelocity.y;
				}
			}
		}

		Message[ nCntMessage ].fColor += Message[ nCntMessage ].fColorValue;

		// 画面外(左右)に出た処理
		if( Message[ nCntMessage ].fPos.x + MESSAGE_READY_WIDTH < 0.0f )
		{
			Message[ nCntMessage ].fPos.x = SCREEN_WIDTH;
			Message[ nCntMessage ].fVelocity.x = 0.0f;
			Message[ nCntMessage ].nCounter = 0;	
			
			if( Message[ nCntMessage ].nType == MESSAGE_TYPE_STAGE )
			{
				Message[ nCntMessage ].nType = MESSAGE_TYPE_READY;
			}
			else if( Message[ nCntMessage ].nType == MESSAGE_TYPE_READY )
			{
				Message[ nCntMessage ].nType = MESSAGE_TYPE_GO;
			}
			else if( Message[ nCntMessage ].nType == MESSAGE_TYPE_GO )
			{
				Message[ nCntMessage ].bUse = false;
			}
		}
	}

	// 時間切れメッセージ
	if( Message[ 0 ].nType == MESSAGE_TYPE_TIMEUP )
	{
		if( Message[ 0 ].nCounter == 0 )
		{
			Message[ 0 ].bUse = true;
			Message[ 0 ].fPos.x = SCREEN_WIDTH * 0.5f;
			Message[ 0 ].fPos.y = SCREEN_HEIGHT * 0.5f;
		}

		Message[ 0 ].nCounter++;

		if( Message[ 0 ].nCounter > 180 )
		{
			Message[ 0 ].bUse = false;
			Message[ 0 ].nCounter = 0;
			Message[ 0 ].nType = MESSAGE_TYPE_GAMEOVER;
		}
	}
	
	// ゲームオーバーメッセージ
	if( Message[ 0 ].nType == MESSAGE_TYPE_GAMEOVER )
	{
		if( Message[ 0 ].nCounter == 0 )
		{
			Message[ 0 ].bUse = true;
			Message[ 0 ].fPos.x = SCREEN_WIDTH * 0.5f;
			Message[ 0 ].fPos.y = 0.0f;
			Message[ 0 ].fVelocity.y = 3.0f;
		}

		Message[ 0 ].nCounter++;
		Message[ 0 ].fVelocity.y += 1.0f;
		Message[ 0 ].fPos.y += Message[ 0 ].fVelocity.y;

		if( Message[ 0 ].fPos.y >= SCREEN_HEIGHT * 0.5f )
		{
			Message[ 0 ].fPos.y = SCREEN_HEIGHT * 0.5f;
			Message[ 0 ].fVelocity.y *= - 0.8f;
		}

		if( Message[ 0 ].nCounter > 360 )
		{
			Message[ 0 ].bUse = false;
			SetFade( FADE_OUT, MODE_RESULT );
		}
	}
	
	// 頂点バッファ格納変数
	VERTEX_2D* pVtx = NULL;

	// 頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferMessage->Lock( 0, 0, ( void** )&pVtx, 0 );
	
	// 更新
	for( int nCntMessage = 0; nCntMessage < MESSAGE_MAX; nCntMessage++ )
	{
		if( !Message[ nCntMessage ].bUse ) { continue; }
				
		// 四隅の座標の更新
		Message[ nCntMessage ].fLeftUp.x    = Message[ nCntMessage ].fPos.x + cos( Message[ nCntMessage ].fRot + D3DX_PI + Message[ nCntMessage ].fAngle ) * Message[ nCntMessage ].fLength;		// 左上の頂点X
		Message[ nCntMessage ].fLeftUp.y    = Message[ nCntMessage ].fPos.y + sin( Message[ nCntMessage ].fRot + D3DX_PI + Message[ nCntMessage ].fAngle ) * Message[ nCntMessage ].fLength;		// 左上の頂点Y		
		Message[ nCntMessage ].fRightUp.x   = Message[ nCntMessage ].fPos.x + cos( Message[ nCntMessage ].fRot		     - Message[ nCntMessage ].fAngle ) * Message[ nCntMessage ].fLength;		// 右上の頂点X
		Message[ nCntMessage ].fRightUp.y   = Message[ nCntMessage ].fPos.y + sin( Message[ nCntMessage ].fRot		     - Message[ nCntMessage ].fAngle ) * Message[ nCntMessage ].fLength;		// 右上の頂点Y
		Message[ nCntMessage ].fLeftDown.x  = Message[ nCntMessage ].fPos.x + cos( Message[ nCntMessage ].fRot + D3DX_PI - Message[ nCntMessage ].fAngle ) * Message[ nCntMessage ].fLength;		// 左下の頂点X
		Message[ nCntMessage ].fLeftDown.y  = Message[ nCntMessage ].fPos.y + sin( Message[ nCntMessage ].fRot + D3DX_PI - Message[ nCntMessage ].fAngle ) * Message[ nCntMessage ].fLength;		// 左下の頂点Y
		Message[ nCntMessage ].fRightDown.x = Message[ nCntMessage ].fPos.x + cos( Message[ nCntMessage ].fRot		     + Message[ nCntMessage ].fAngle ) * Message[ nCntMessage ].fLength;		// 右下の頂点X
		Message[ nCntMessage ].fRightDown.y = Message[ nCntMessage ].fPos.y + sin( Message[ nCntMessage ].fRot		     + Message[ nCntMessage ].fAngle ) * Message[ nCntMessage ].fLength;		// 右下の頂点Y
		
		// 頂点座標の設定
		pVtx[ 0 ].pos.x = Message[ nCntMessage ].fLeftUp.x;		// 左上の頂点X
		pVtx[ 0 ].pos.y = Message[ nCntMessage ].fLeftUp.y;		// 左上の頂点Y		
		pVtx[ 1 ].pos.x = Message[ nCntMessage ].fRightUp.x;	// 右上の頂点X
		pVtx[ 1 ].pos.y = Message[ nCntMessage ].fRightUp.y;	// 右上の頂点Y
		pVtx[ 2 ].pos.x = Message[ nCntMessage ].fLeftDown.x;	// 左下の頂点X
		pVtx[ 2 ].pos.y = Message[ nCntMessage ].fLeftDown.y;	// 左下の頂点Y
		pVtx[ 3 ].pos.x = Message[ nCntMessage ].fRightDown.x;	// 右下の頂点X
		pVtx[ 3 ].pos.y = Message[ nCntMessage ].fRightDown.y;	// 右下の頂点Y
		
		if( Message[ nCntMessage ].nType == MESSAGE_TYPE_STAGE )
		{
			int temp = GetStage();
			pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.33f * ( temp - 1 ) );	// 左上の座標
			pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.33f * ( temp - 1 ) );	// 右上の座標
			pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 0.33f * temp );			// 左下の座標
			pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 0.33f * temp );			// 右下の座標
		}
		else
		{
			// テクスチャ座標の設定( 0.0f～1.0f )
			pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );	// 左上の座標
			pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );	// 右上の座標
			pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );	// 左下の座標
			pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );	// 右下の座標
		}
		
		// 頂点色の設定
		pVtx[ 0 ].color = Message[ nCntMessage ].fColor;
		pVtx[ 1 ].color = Message[ nCntMessage ].fColor;
		pVtx[ 2 ].color = Message[ nCntMessage ].fColor;
		pVtx[ 3 ].color = Message[ nCntMessage ].fColor;

		// 次のポリゴンの頂点へ
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBufferMessage->Unlock();

	// カウンターのインクリメント
	g_nFrameCount++;
}

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
void DrawMessage( void )
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// ストリーム(パイプライン)設定
	pDevice->SetStreamSource( 0,						// 繋げるパイプの番号
							  g_pVtxBufferMessage,		// ストリームの元となる頂点バッファの指定
							  0,						// オフセット(バイト)
							  sizeof( VERTEX_2D ) );		// ストライド値(ストライド量)

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_2D );

	// テクスチャの設定
	switch( Message[ 0 ].nType )
	{
	case MESSAGE_TYPE_STAGE:
		pDevice->SetTexture( 0, g_pTextureMessage[ 0 ] );
		break;
	case MESSAGE_TYPE_READY:
		pDevice->SetTexture( 0, g_pTextureMessage[ 1 ] );
		break;
	case MESSAGE_TYPE_GO:
		pDevice->SetTexture( 0, g_pTextureMessage[ 2 ] );
		break;
	case MESSAGE_TYPE_GOAL:
		pDevice->SetTexture( 0, g_pTextureMessage[ 3 ] );
		break;
	case MESSAGE_TYPE_TIMEUP:
		pDevice->SetTexture( 0, g_pTextureMessage[ 4 ] );
		break;
	case MESSAGE_TYPE_GAMEOVER:
		pDevice->SetTexture( 0, g_pTextureMessage[ 5 ] );
		break;
	}
	
	for( int nCntMessage = 0; nCntMessage < MESSAGE_MAX; nCntMessage++ )
	{
		if( !Message[ nCntMessage ].bUse ) { continue; }
				
		// プリミティブの描画
		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,		// プリミティブの種類
								nCntMessage * 4,			// オフセット(頂点数)
								2 );						// プリミティブ数
	}
}

/*------------------------------------------------------------------------------
  　メッセージの作成
------------------------------------------------------------------------------*/
void CreateMessage( D3DXVECTOR2 pos, D3DXVECTOR2 size, float speed, float direct, float angleOfFire, D3DXCOLOR color, D3DXCOLOR colorValue,  MESSAGETYPE type )
{
	for( int nCntMessage = 0; nCntMessage < MESSAGE_MAX; nCntMessage++ )
	{
		// エフェクトが使われていなかったら
		if( !Message[ nCntMessage ].bUse )
		{
			Message[ nCntMessage ].bUse			= true;
			Message[ nCntMessage ].fPos			= pos;
			Message[ nCntMessage ].fLength		= sqrt( ( size.y * size.y ) + ( size.x * size.x  ) );
			Message[ nCntMessage ].fAngle		= atan2( size.y, size.x );
			Message[ nCntMessage ].fDirect		= direct;
			Message[ nCntMessage ].fRot			= angleOfFire;
			Message[ nCntMessage ].fVelocity.x	= speed * cos( direct );
			Message[ nCntMessage ].fVelocity.y	= speed * sin( direct );
			Message[ nCntMessage ].fColor		= color;
			Message[ nCntMessage ].fColorValue	= colorValue;
			Message[ nCntMessage ].nType		= type;

			break;	// for文を抜ける
		}
	}
}

/*------------------------------------------------------------------------------
  　ゲッター
------------------------------------------------------------------------------*/
MESSAGE* GetMessage( void )
{
	return Message;
}

/*------------------------------------------------------------------------------
  　セッター
------------------------------------------------------------------------------*/
void SetMessageType( MESSAGETYPE type )
{
	Message[ 0 ].nType = type;
}

//******************************* EOF **************************************