/*==============================================================================

    入力処理のヘッダ [input.h]
                                                       作成者 : 中西　祐介
                                                       作成日 : 2016/4/17
--------------------------------------------------------------------------------
    ■Update : 
==============================================================================*/
#ifndef _INPUT_H_
#define _INPUT_H_

/*------------------------------------------------------------------------------
  　プロトタイプ宣言
------------------------------------------------------------------------------*/
HRESULT InitKeyboard( HINSTANCE hInstance, HWND hWnd );
void UninitKeyboard( void );
void UpdateKeyboard( void );

bool GetKeyboardPress( int nKey );
bool GetKeyboardTrigger( int nKey );
bool GetKeyboardRepeat( int nKey );
bool GetKeyboardRelease( int nKey );

BYTE* GetKeyboard( void );

#endif