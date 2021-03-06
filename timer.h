/*==============================================================================

    Timer処理のヘッダ [Timer.h]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/
#ifndef _TIMER_H_
#define _TIMER_H_

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
void InitTimer( void );
void UninitTimer( void );
void UpdateTimer( void );
void DrawTimer( void );
int GetTimer( void );
void SetTimer( float fTimer );
void SetTimerNumber( VERTEX_2D* pVtx, int num, int index );

#endif