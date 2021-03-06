/*==============================================================================

    Ranking処理のヘッダ [Ranking.h]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/
#ifndef _RANKING_H_
#define _RANKING_H_

/*------------------------------------------------------------------------------
  　マクロ定義
------------------------------------------------------------------------------*/
#define RANKING_DOLLAR (200)		// ドルのスコア

/*------------------------------------------------------------------------------
  　構造体の宣言
------------------------------------------------------------------------------*/
// Rankingの構造体
typedef struct
{
	int nScore;		// スコア格納
	int nStage;		// 到達ステージ格納
}RANKING;

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
void InitRanking( void );
void UninitRanking( void );
void UpdateRanking( void );
void DrawRanking( void );
void MakeRanking( void );
void SetRanking( float fRanking );
void SetRankingNumber( void );
void SetFuelNumber( void );
void SaveRanking( void );
void LoadRanking( void );
bool GetRankIn( void );

#endif