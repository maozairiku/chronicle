//=============================================================================
//
// BG処理 [bg.h]
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
struct BG
{
	XMFLOAT3	pos;		// ポリゴンの座標
	XMFLOAT3	fpos;		// ポリゴン(fly)の座標
	float		w, h;		// 幅と高さ
	int			texNo;		// 使用しているテクスチャ番号

	float		scrl;		// スクロール
	XMFLOAT3	old_pos;	// １フレ前のポリゴンの座標
	float		scrl2;		// スクロール

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
#define	SKY_H			(150.0f)	// 空のポリゴンの高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

BG* GetBG(void);
COLLISION* GetCOLLISION(void);



