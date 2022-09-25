//=============================================================================
//
// �G�l�~�[���� [enemy.h]
// Author : GP11B132 05 ���E��
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_MAX		(15)		// �G�l�~�[��Max�l��
#define ENEMYFLY_MAX	(15)		// �G�l�~�[��Max�l��


//*****************************************************************************
// �\���̒�`
//*****************************************************************************

struct ENEMY
{
	// ground enemy
	int			lucky;

	XMFLOAT3	pos;			// �|���S���̍��W
	XMFLOAT3	rot;			// �|���S���̉�]��
	XMFLOAT3	scl;			// �|���S���̊g��k��
	BOOL		use;			// true:�g���Ă���  false:���g�p
	BOOL		atkuse;			// �U������
	float		w, h;			// ���ƍ���
	float		countAnim;		// �A�j���[�V�����J�E���g
	int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[
	int			texNo;			// �e�N�X�`���ԍ�
	XMFLOAT3	move;			// �ړ����x


	float		time;			// ���`��ԗp
	int			tblNo;			// �s���f�[�^�̃e�[�u���ԍ�
	int			tblMax;			// ���̃e�[�u���̃f�[�^��

	BOOL		ecollision;
	float		kill;

	INTERPOLATION_DATA*		tbl_adr;			// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	int						tbl_size;			// �o�^�����e�[�u���̃��R�[�h����
	float					move_time;			// ���s����

};




//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY* GetEnemy(void);



