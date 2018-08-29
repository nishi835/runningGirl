/*==============================================================================

    ���͏����̃w�b�_ [input.h]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/4/17
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/
#ifndef _INPUT_H_
#define _INPUT_H_

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
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