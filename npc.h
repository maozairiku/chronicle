//=============================================================================
//
// NPC処理 [npc.h]
// Author : GP11B132 05 王ウ華
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NPC_MAX		(15)	// NPC Max
#define ITEM_MAX	(15)	// ITEM Max


//*****************************************************************************
// 構造体定義
//*****************************************************************************

struct NPC
{	
	BOOL		use;			// npc flag
	XMFLOAT3	pos;			// ポリゴンの座標
	XMFLOAT3	rot;			// ポリゴンの回転量
	XMFLOAT3	scl;			// ポリゴンの拡大縮小
	float		w, h;			// 幅と高さ
	float		countAnim;		// アニメーションカウント
	int			patternAnim;	// アニメーションパターンナンバー
	int			texNo;			// テクスチャ番号
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
	XMFLOAT3	ipos;			// ポリゴンの座標
	XMFLOAT3	irot;			// ポリゴンの回転量
	XMFLOAT3	iscl;			// ポリゴンの拡大縮小
	float		iw, ih;			// 幅と高さ
	BOOL		iconbox;		// dialogue box
	int			itexNo;			// dialogue box number
};

#define NC_TEXTURE_MAX	(15)		// collision texture

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitNPC(void);
void UninitNPC(void);
void UpdateNPC(void);
void DrawNPC(void);

NPC* GetNPC(void);
NC* GetNC(void);
ITEM* GetITEM(void);



