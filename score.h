/*==============================================================================

    Score�����̃w�b�_ [Score.h]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/5/31
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/
#ifndef _SCORE_H_
#define _SCORE_H_

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
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