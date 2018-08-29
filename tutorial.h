/*==============================================================================

    tutorial処理のヘッダ [tutorial.h]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/5/31
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
void InitTutorial( void );
void UninitTutorial( void );
void UpdateTutorial( void );
void DrawTutorial( void );
void LoadTutorial( void );
int GetTutorialTimer( void );

#endif