//=============================================================================
//
// NPC���� [npc.h]
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
#define NPC_MAX		(15)	// NPC Max
#define ITEM_MAX	(15)	// ITEM Max


//*****************************************************************************
// �\���̒�`
//*****************************************************************************

struct NPC
{	
	BOOL		use;			// npc flag
	XMFLOAT3	pos;			// �|���S���̍��W
	XMFLOAT3	rot;			// �|���S���̉�]��
	XMFLOAT3	scl;			// �|���S���̊g��k��
	float		w, h;			// ���ƍ���
	float		countAnim;		// �A�j���[�V�����J�E���g
	int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[
	int			texNo;			// �e�N�X�`���ԍ�
	BOOL		conbox;			// dialogue box
	int			ctexNo;			// dialogue box number
	BOOL		teflag;			// for teflag conversation
};

// NPC collision
struct NC
{	
	BOOL		use;			// collision using flag
	float		cwx;			// collision width
	float		chy;			// collision height
	XMFLOAT3	pos;			// collision coordinate
	float		nr;				// NPC collision right
	float		nl;				// NPC collision left
	float		nu;				// NPC collision up
	float		nd;				// NPC collision down
};

struct ITEM
{
	BOOL		iuse;			// npc flag
	XMFLOAT3	ipos;			// �|���S���̍��W
	XMFLOAT3	irot;			// �|���S���̉�]��
	XMFLOAT3	iscl;			// �|���S���̊g��k��
	float		iw, ih;			// ���ƍ���
	BOOL		iconbox;		// dialogue box
	int			itexNo;			// dialogue box number
};

#define NC_TEXTURE_MAX	(15)		// collision texture

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitNPC(void);
void UninitNPC(void);
void UpdateNPC(void);
void DrawNPC(void);

NPC* GetNPC(void);
NC* GetNC(void);
ITEM* GetITEM(void);



