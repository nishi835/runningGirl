/*==============================================================================

    Score処理のヘッダ [Score.h]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/
#ifndef _SCORE_H_
#define _SCORE_H_

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
void InitScore( void );
void UninitScore( void );
void UpdateScore( void );
void DrawScore( void );
float GetScore( void );
void SetScore( float fScore );
void SetScoreNumber( VERTEX_2D* pVtx, int num, int index );
void SetTimeNumber( VERTEX_2D* pVtx, int num, int index );

#endif