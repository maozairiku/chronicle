//=============================================================================
//
// サウンド処理 [sound.h]
// Author : GP11B132 05 王ウ華
//
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
enum 
{
	SOUND_LABEL_BGM_title,			// title
	SOUND_LABEL_BGM_map1,			// map1
	SOUND_LABEL_BGM_village,		// village
	SOUND_LABEL_BGM_map2,			// map2
	SOUND_LABEL_BGM_mountain_hut,	// MH
	SOUND_LABEL_BGM_map3,			// map3
	SOUND_LABEL_BGM_lake,			// lake
	SOUND_LABEL_BGM_lake_inside,	// lake_inside

	SOUND_LABEL_SE_bomb000,		// 爆発音
	SOUND_LABEL_SE_defend000,	// 
	SOUND_LABEL_SE_defend001,	// 
	SOUND_LABEL_SE_hit000,		// 
	SOUND_LABEL_SE_laser000,	// 
	SOUND_LABEL_SE_lockon000,	// 
	SOUND_LABEL_SE_shot000,		// 
	SOUND_LABEL_SE_shot001,		// 

	SOUND_LABEL_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

