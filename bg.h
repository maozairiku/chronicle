//=============================================================================
//
// BG���� [bg.h]
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
struct BG
{
	XMFLOAT3	pos;		// �|���S���̍��W
	XMFLOAT3	fpos;		// �|���S��(fly)�̍��W
	float		w, h;		// ���ƍ���
	int			texNo;		// �g�p���Ă���e�N�X�`���ԍ�

	float		scrl;		// �X�N���[��
	XMFLOAT3	old_pos;	// �P�t���O�̃|���S���̍��W
	float		scrl2;		// �X�N���[��

	// flag for change map
	BOOL		map1flag;
	BOOL		map2flag;
	BOOL		map3flag;
	BOOL		map4flag;

};

struct COLLISION
{
	float		cwx;			// collision width
	float		chy;			// collision height
	XMFLOAT3	pos;			// collision coordinate
	BOOL		use;			// collision using flag
	float		gr;				// ground collision right
	float		gl;				// ground collision left
	float		gu;				// ground collision up
	float		gd;				// ground collision down
};

#define CO_TEXTURE_MAX	(100)		// collision texture
#define	SKY_H			(150.0f)	// ��̃|���S���̍���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

BG* GetBG(void);
COLLISION* GetCOLLISION(void);



