//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : GP11B132 05 王ウ華
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "bg.h"
#include <math.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	 
#define TEXTURE_MAX					(10)			// テクスチャの数

#define TEXTURE_WIDTH_LOGO			(700)			// title logoサイズ
#define TEXTURE_HEIGHT_LOGO			(300)			

#define TEXTURE_WIDTH_SELECTION		(300)			// title selectionサイズ
#define TEXTURE_HEIGHT_SELECTION	(150)			

#define PI							(3.14159263)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TITLE/BG.png",
	"data/TITLE/title.png",
	"data/TITLE/start.png",
	"data/TITLE/load.png",
	"data/TITLE/quit.png",

};


static BOOL				g_Use;				// TRUE:使っている  FALSE:未使用
static float			g_w, g_h;			// 幅と高さ
static XMFLOAT3			g_Pos;				// ポリゴンの座標
static int				g_TexNo;			// テクスチャ番号

static XMFLOAT3			g_lPos;				// title logo position
static float			g_lw, g_lh;			// title logo width & height

static XMFLOAT3			g_oPos;				// title option position
static float			g_ow, g_oh;			// title option width & height

float	alpha;
BOOL	flag_alpha;

static BOOL						g_Load = FALSE;

static float	effect_dx;
static float	effect_dy;

int wavemove;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// 変数の初期化

	// BG
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = XMFLOAT3(g_w / 2, g_h / 2, 0.0f);	

	// title logo
	wavemove = 1;
	g_lw = TEXTURE_WIDTH_LOGO;
	g_lh = TEXTURE_HEIGHT_LOGO;
	g_lPos = XMFLOAT3(g_lw, g_lh, 0.0f);

	// title option
	g_ow = TEXTURE_WIDTH_SELECTION;
	g_oh = TEXTURE_HEIGHT_SELECTION;
	g_oPos = XMFLOAT3(g_ow, g_oh, 0.0f);

	g_TexNo = 0;

	alpha = 1.0f;
	flag_alpha = TRUE;

	effect_dx = 100.0f;
	effect_dy = 100.0f;

	g_Load = TRUE;
	return S_OK;

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
		// title內各個顯示個別定義(e.g. title logo/ title option)
		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter押したら、ステージを切り替える

			//紀錄玩家狀態

			SetFade(FADE_OUT, MODE_GAME);
		}
		// ゲームパッドで入力処理
		else if (IsButtonTriggered(0, BUTTON_START))
		{
			SetFade(FADE_OUT, MODE_GAME);
		}
		else if (IsButtonTriggered(0, BUTTON_B))
		{
			SetFade(FADE_OUT, MODE_GAME);
		}

		// セーブデータをロードする？
		if (GetKeyboardTrigger(DIK_L))
		{
			SetLoadGame(TRUE);
			SetFade(FADE_OUT, MODE_GAME);
		}


	// テストでエフェクトの発生場所を移動させる
	float speed = 4.0f;

	if (GetKeyboardPress(DIK_DOWN))
	{
		effect_dy += speed;
	}
	else if (GetKeyboardPress(DIK_UP))
	{
		effect_dy -= speed;
	}

	if (GetKeyboardPress(DIK_RIGHT))
	{
		effect_dx += speed;
	}
	else if (GetKeyboardPress(DIK_LEFT))
	{
		effect_dx -= speed;
	}

#ifdef _DEBUG	// デバッグ情報を表示する
	PrintDebugProc("Player:↑ → ↓ ←　Space\n");

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	BG* bg = GetBG();

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// タイトルの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	// title
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		float px = (g_Pos.x - 610.0f) - bg->pos.x;
		float py = (g_Pos.y + 50.0f) - bg->pos.y;
		float pw = TEXTURE_WIDTH_LOGO;
		float ph = TEXTURE_HEIGHT_LOGO;

		float tx = 0.0f;
		float ty = 0.0f;
		float tw = 1.0f;
		float th = 1.0f;

		// texture wave effect
		for (int i = py; i < py + ph; i++)
		{
			float wave = 0.01 * sinf(5 * (i + wavemove) * PI / 90);
			float x = 250;

			SetSpriteLeftTop(g_VertexBuffer, x * wave, i, pw, 1.0f, 0.0f, (i - py) / ph, 1.0f, 0.0125f);

			// draw
			GetDeviceContext()->Draw(4, 0);
		}

		wavemove++;
	}

	// start
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

		float px = (g_Pos.x + 280) - bg->pos.x;
		float py = (g_Pos.y + 30) - bg->pos.y;
		float pw = TEXTURE_WIDTH_SELECTION;
		float ph = TEXTURE_HEIGHT_SELECTION;

		float tx = 0.0f;
		float ty = 0.0f;
		float tw = 1.0f;
		float th = 1.0f;

		// texture coordinate sitting
		SetSpriteLeftTop(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

		// draw
		GetDeviceContext()->Draw(4, 0);
	}

	// load
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

		float px = (g_Pos.x + 280) - bg->pos.x;
		float py = (g_Pos.y + 130) - bg->pos.y;
		float pw = TEXTURE_WIDTH_SELECTION;
		float ph = TEXTURE_HEIGHT_SELECTION;

		float tx = 0.0f;
		float ty = 0.0f;
		float tw = 1.0f;
		float th = 1.0f;

		// texture coordinate sitting
		SetSpriteLeftTop(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

		// draw
		GetDeviceContext()->Draw(4, 0);
	}

	// quit
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[4]);

		float px = (g_Pos.x + 280) - bg->pos.x;
		float py = (g_Pos.y + 220) - bg->pos.y;
		float pw = TEXTURE_WIDTH_SELECTION;
		float ph = TEXTURE_HEIGHT_SELECTION;

		float tx = 0.0f;
		float ty = 0.0f;
		float tw = 1.0f;
		float th = 1.0f;

		// texture coordinate sitting
		SetSpriteLeftTop(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

		// draw
		GetDeviceContext()->Draw(4, 0);
	}

//	// title logo wave effect (horizontal)
//	{
//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);
//
//		float px = 0.0f;
//		float py = 300.0f;
//		float pw = TEXTURE_WIDTH_LOGO;
//		float ph = TEXTURE_HEIGHT_LOGO;
//
//		float tx = 0.0f;
//		float ty = 0.0f;
//		float tw = 1.0f / 1;
//		float th = 1.0f / TEXTURE_HEIGHT_LOGO;
//
//		// texture coordinate sitting
//		for (int i = py; i < py + ph; i++) {
//
//			float wave = 0.01 * sinf(5 * (i + wavemove) * PI / 90);	// sin外面*N→控制震幅　/　sin裡面i之前*N→控制波長(頻率)
//			float x = 250;	//X不會固定在單一點上所以設一個新變數以抖動
//
//			SetSpriteLeftTop(g_VertexBuffer, x * wave, i, TEXTURE_WIDTH_LOGO, 1.0f, 0.0f, (i - py) / TEXTURE_HEIGHT_LOGO, 1.0f, 0.0125f);
//
//			// draw
//			GetDeviceContext()->Draw(4, 0);
//
//		}
//
//		wavemove++;
//
//	}
//
//	// title logo wave effect (vertical)
//	{
//
//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);
//
//		float px = 0.0f;
//		float py = 150.0f;
//		float pw = TEXTURE_WIDTH_LOGO;
//		float ph = TEXTURE_HEIGHT_LOGO;
//
//		float tx = 0.0f;
//		float ty = 0.0f;
//		float tw = 1.0f / TEXTURE_WIDTH_LOGO;
//		float th = 1.0f / 1;
//
//		// texture coordinate sitting
//		for (int i = px; i < px + pw; i++) {
//
//			float wave = 0.01 * cosf(6 * (i + wavemove) * PI / 90);
//			float y = 250;
//
//																														//↓1的部分增加可造成模糊效果
//			SetSpriteLeftTop(g_VertexBuffer, i, y * wave, 1.0f, TEXTURE_HEIGHT_LOGO, (i - px) / TEXTURE_WIDTH_LOGO, 0.0f, 1 / pw, 1.0f);
//									//float X , floatY,float Width, float Heigth   ,float U						,float V,floatUW,floatVH
//												//↑因為現在是橫波紋,(切片後)寬(floatY)為1,長(float Width)為圖片原始長度
//																											  // float V為0是因為他是切片後的最上端頂點,若為1.0則為下方底點
//
//			// draw
//			GetDeviceContext()->Draw(4, 0);
//
//		}
//
//		wavemove++;
//
//	}
//
//	// title logo wave effect (trapzeoid)
//	{
//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);
//
//		float px = 0.0f;
//		float py = 150.0f;
//		float pw = TEXTURE_WIDTH_LOGO;
//		float ph = TEXTURE_HEIGHT_LOGO;
//
//		float tx = 0.0f;
//		float ty = 0.0f;
//		float tw = 1.0f / TEXTURE_WIDTH_LOGO;
//		float th = 1.0f / 1;
//
//		// texture coordinate setting
//		for (int i = py; i < py + ph; i++) {
//
//			float x = 450;
//
//			SetSprite(g_VertexBuffer, x, i, (TEXTURE_WIDTH_LOGO+i)-py, 1.0f, 0.0f, (i - py) / TEXTURE_HEIGHT_LOGO, 1.0f, 0.0125f);
//									//float X ,floatY,float Width,float Heigth,float U			 ,float V   ,floatUW, floatVH
//
//			// draw
//			GetDeviceContext()->Draw(4, 0);
//
//		}
//	}
//
//	// title logo wave effect (trapzeoid2)
//	{
//		GetDeviceContext()->PSGetShaderResources(0, 1, &g_Texture[3]);
//
//	}
//
//
//	// 加減算のテスト
//	SetBlendState(BLEND_MODE_ADD);		// 加算合成
////	SetBlendState(BLEND_MODE_SUBTRACT);	// 減算合成
//		
//	// テクスチャ設定
//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);
//	
//	for (int i = 0; i < 30; i++)
//	{
//		// １枚のポリゴンの頂点とテクスチャ座標を設定
//		float dx = effect_dx;
//		float dy = effect_dy;
//		float sx = (float)(rand() % 100);
//		float sy = (float)(rand() % 100);
//
//		SetSpriteColor(g_VertexBuffer, dx + sx, dy + sy, 50, 50, 0.0f, 0.0f, 1.0f, 1.0f,
//			XMFLOAT4(1.0f, 0.3f, 1.0f, 0.5f));
//
//		// ポリゴン描画
//		GetDeviceContext()->Draw(4, 0);
//	}
//
//	SetBlendState(BLEND_MODE_ALPHABLEND);	// 半透明処理を元に戻す
//
}





