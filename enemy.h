//=============================================================================
//
// エネミー処理 [enemy.h]
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
#define ENEMY_MAX		(15)		// エネミーのMax人数
#define ENEMYFLY_MAX	(15)		// エネミーのMax人数


//*****************************************************************************
// 構造体定義
//*****************************************************************************

struct ENEMY
{
	// ground enemy
	int			lucky;

	XMFLOAT3	pos;			// ポリゴンの座標
	XMFLOAT3	rot;			// ポリゴンの回転量
	XMFLOAT3	scl;			// ポリゴンの拡大縮小
	BOOL		use;			// true:使っている  false:未使用
	BOOL		atkuse;			// 攻撃中か
	float		w, h;			// 幅と高さ
	float		countAnim;		// アニメーションカウント
	int			patternAnim;	// アニメーションパターンナンバー
	int			texNo;			// テクスチャ番号
	XMFLOAT3	move;			// 移動速度


	float		time;			// 線形補間用
	int			tblNo;			// 行動データのテーブル番号
	int			tblMax;			// そのテーブルのデータ数

	BOOL		ecollision;
	float		kill;

	INTERPOLATION_DATA*		tbl_adr;			// アニメデータのテーブル先頭アドレス
	int						tbl_size;			// 登録したテーブルのレコード総数
	float					move_time;			// 実行時間

};




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY* GetEnemy(void);



