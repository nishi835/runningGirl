/*==============================================================================

    Game�����̃w�b�_ [game.h]
                                                       �쐬�� : �����@�S��
                                                       �쐬�� : 2016/5/31
--------------------------------------------------------------------------------
    ��Update : 
==============================================================================*/
#ifndef _GAME_H_
#define _GAME_H_

/*------------------------------------------------------------------------------
  �@�}�N��
------------------------------------------------------------------------------*/
#define STAGE_MAX (1)

/*------------------------------------------------------------------------------
  �@�񋓑̂̐錾
------------------------------------------------------------------------------*/
// �Q�[���̏�Ԃ̗񋓑�
typedef enum
{
	GAMEMODE_READY = 0, // ��~���
	GAMEMODE_GO,		// �v���C���
	GAMEMODE_MAX
}GAMEMODE;

/*------------------------------------------------------------------------------
  �@�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
void InitGame( void );
void UninitGame( void );
void UpdateGame( void );
void DrawGame( void );
void LoadGame( void );
int GetStage( void );
void FowardStage( void );

#endif