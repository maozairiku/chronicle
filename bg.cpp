//=============================================================================
//
// BG処理 [bg.cpp]
// Author : GP11B132 05 王ウ華
//
//=============================================================================
#include "main.h"
#include "bg.h"
#include "player.h"
#include "enemy.h"
#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(8000)		// (SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(2500)		// (SCREEN_HEIGHT)	
#define TEXTURE_MAX					(20)		// BGテクスチャの数

#define UI_PANEL_WIDTH				(310)		//item box size
#define UI_PANEL_HEIGHT				(150)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };		// BGテクスチャ情報


static char *g_TexturName[TEXTURE_MAX] = {
	"data/MAP/mapC1.png",
	"data/MAP/mapC2.png",
	"data/MAP/mapC3.png",
	"data/MAP/mapC4.png",

	"data/ITEM/itembox.png",

	"data/TEXTURE/bar_white.png",

};



static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ
static BG	g_BG;
static COLLISION g_CO[CO_TEXTURE_MAX];



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBG(void)
{
	ID3D11Device *pDevice = GetDevice();



	// BGテクスチャ生成
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


	// BG変数の初期化
	for (int i = 0; i < CO_TEXTURE_MAX; i++)
	{
		g_CO[i].use = FALSE;

		g_CO[i].gr = g_CO[i].pos.x + (g_CO[i].cwx / 2);
		g_CO[i].gl = g_CO[i].pos.x - (g_CO[i].cwx / 2);
		g_CO[i].gu = g_CO[i].pos.y - (g_CO[i].chy / 2);
		g_CO[i].gd = g_CO[i].pos.y + (g_CO[i].chy / 2);
	}
	g_BG.w     = TEXTURE_WIDTH;
	g_BG.h     = TEXTURE_HEIGHT;
	g_BG.pos   = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_BG.texNo = 0;

	g_BG.scrl  = 0.0f;		// TEXスクロール
	g_BG.scrl2 = 0.0f;		// TEXスクロール

	g_Load = TRUE;
	return S_OK;

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBG(void)
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
void UpdateBG(void)
{
	g_BG.old_pos = g_BG.pos;	// １フレ前の情報を保存


	//g_BG.scrl -= 0.0f;		// 0.005f;		// スクロール

	switch (GetMode())
	{
	case MODE_GAME:
		g_CO[0].use = TRUE;		// map1 ground
		g_CO[0].cwx = 8000;
		g_CO[0].chy = 215;
		g_CO[0].pos = XMFLOAT3(4000.0f, 2400.0f, 0.0f);

		g_CO[1].use = TRUE;
		g_CO[1].cwx = 1380;
		g_CO[1].chy = 290;
		g_CO[1].pos = XMFLOAT3(685.0f, 785.0f, 0.0f);

		g_CO[2].use = TRUE;
		g_CO[2].cwx = 305;
		g_CO[2].chy = 650;
		g_CO[2].pos = XMFLOAT3(0.0f, 1000.0f, 0.0f);

		g_CO[3].use = TRUE;
		g_CO[3].cwx = 800;
		g_CO[3].chy = 290;
		g_CO[3].pos = XMFLOAT3(380.0f, 1420.0f, 0.0f);

		g_CO[4].use = TRUE;
		g_CO[4].cwx = 600;
		g_CO[4].chy = 230;
		g_CO[4].pos = XMFLOAT3(165.0f, 1540.0f, 0.0f);

		g_CO[5].use = TRUE;
		g_CO[5].cwx = 155;
		g_CO[5].chy = 900;
		g_CO[5].pos = XMFLOAT3(80.0f, 1900.0f, 0.0f);

		g_CO[6].use = TRUE;
		g_CO[6].cwx = 1000;
		g_CO[6].chy = 100;
		g_CO[6].pos = XMFLOAT3(1800.0f, 880.0f, 0.0f);
	
		g_CO[7].use = TRUE;
		g_CO[7].cwx = 630;
		g_CO[7].chy = 290;
		g_CO[7].pos = XMFLOAT3(2480.0f, 785.0f, 0.0f);

		g_CO[8].use = TRUE;
		g_CO[8].cwx = 820;
		g_CO[8].chy = 95;
		g_CO[8].pos = XMFLOAT3(1510.0f, 1615.0f, 0.0f);

		g_CO[9].use = TRUE;
		g_CO[9].cwx = 1250;
		g_CO[9].chy = 290;
		g_CO[9].pos = XMFLOAT3(2495.0f, 1520.0f, 0.0f);

		g_CO[10].use = TRUE;
		g_CO[10].cwx = 1240;
		g_CO[10].chy = 20;
		g_CO[10].pos = XMFLOAT3(2065.0f, 2080.0f, 0.0f);

		g_CO[11].use = TRUE;
		g_CO[11].cwx = 320;
		g_CO[11].chy = 1550;
		g_CO[11].pos = XMFLOAT3(3270.0f, 1200.0f, 0.0f);

		g_CO[12].use = TRUE;
		g_CO[12].cwx = 650;
		g_CO[12].chy = 20;
		g_CO[12].pos = XMFLOAT3(5220.0f, 1610.0f, 0.0f);

		g_CO[13].use = TRUE;
		g_CO[13].cwx = 620;
		g_CO[13].chy = 20;
		g_CO[13].pos = XMFLOAT3(5745.0f, 1250.0f, 0.0f);

		g_CO[14].use = TRUE;
		g_CO[14].cwx = 620;
		g_CO[14].chy = 20;
		g_CO[14].pos = XMFLOAT3(5830.0f, 1930.0f, 0.0f);

		g_CO[15].use = TRUE;
		g_CO[15].cwx = 315;
		g_CO[15].chy = 20;
		g_CO[15].pos = XMFLOAT3(1800.0f, 630.0f, 0.0f);

		g_CO[16].use = TRUE;
		g_CO[16].cwx = 320;
		g_CO[16].chy = 20;
		g_CO[16].pos = XMFLOAT3(6560.0f, 995.0f, 0.0f);

		g_CO[17].use = TRUE;
		g_CO[17].cwx = 960;
		g_CO[17].chy = 20;
		g_CO[17].pos = XMFLOAT3(7550.0f, 800.0f, 0.0f);

		g_CO[18].use = TRUE;
		g_CO[18].cwx = 870;
		g_CO[18].chy = 20;
		g_CO[18].pos = XMFLOAT3(7140.0f, 1760.0f, 0.0f);
		break;

	case MODE_GAME_MAP2:
		g_CO[0].use = TRUE;	// map2 ground
		g_CO[0].cwx = 8000;
		g_CO[0].chy = 210;
		g_CO[0].pos = XMFLOAT3(4000.0f, 2400.0f, 0.0f);

		g_CO[1].use = TRUE;
		g_CO[1].cwx = 1620;
		g_CO[1].chy = 290;
		g_CO[1].pos = XMFLOAT3(15.0f, 1035.0f, 0.0f);

		g_CO[2].use = TRUE;
		g_CO[2].cwx = 320;
		g_CO[2].chy = 800;
		g_CO[2].pos = XMFLOAT3(155.0f, 1970.0f, 0.0f);

		g_CO[3].use = TRUE;
		g_CO[3].cwx = 300;
		g_CO[3].chy = 20;
		g_CO[3].pos = XMFLOAT3(460.0f, 1785.0f, 0.0f);

		g_CO[4].use = TRUE;
		g_CO[4].cwx = 300;
		g_CO[4].chy = 20;
		g_CO[4].pos = XMFLOAT3(910.0f, 2040.0f, 0.0f);

		g_CO[5].use = TRUE;
		g_CO[5].cwx = 300;
		g_CO[5].chy = 20;
		g_CO[5].pos = XMFLOAT3(1210.0f, 1575.0f, 0.0f);

		g_CO[6].use = TRUE;
		g_CO[6].cwx = 325;
		g_CO[6].chy = 1050;
		g_CO[6].pos = XMFLOAT3(1525.0f, 1800.0f, 0.0f);

		g_CO[7].use = TRUE;
		g_CO[7].cwx = 475;
		g_CO[7].chy = 1550;
		g_CO[7].pos = XMFLOAT3(1925.0f, 1585.0f, 0.0f);

		g_CO[8].use = TRUE;
		g_CO[8].cwx = 1150;
		g_CO[8].chy = 500;
		g_CO[8].pos = XMFLOAT3(2680.0f, 2070.0f, 0.0f);

		g_CO[9].use = TRUE;
		g_CO[9].cwx = 300;
		g_CO[9].chy = 20;
		g_CO[9].pos = XMFLOAT3(2575.0f, 635.0f, 0.0f);

		g_CO[10].use = TRUE;
		g_CO[10].cwx = 300;
		g_CO[10].chy = 20;
		g_CO[10].pos = XMFLOAT3(3015.0f, 690.0f, 0.0f);

		g_CO[11].use = TRUE;
		g_CO[11].cwx = 300;
		g_CO[11].chy = 20;
		g_CO[11].pos = XMFLOAT3(3320.0f, 785.0f, 0.0f);

		g_CO[12].use = TRUE;
		g_CO[12].cwx = 300;
		g_CO[12].chy = 20;
		g_CO[12].pos = XMFLOAT3(3225.0f, 1460.0f, 0.0f);

		g_CO[13].use = TRUE;
		g_CO[13].cwx = 300;
		g_CO[13].chy = 20;
		g_CO[13].pos = XMFLOAT3(3550.0f, 2050.0f, 0.0f);

		g_CO[14].use = TRUE;
		g_CO[14].cwx = 300;
		g_CO[14].chy = 20;
		g_CO[14].pos = XMFLOAT3(3730.0f, 1200.0f, 0.0f);

		g_CO[15].use = TRUE;
		g_CO[15].cwx = 300;
		g_CO[15].chy = 20;
		g_CO[15].pos = XMFLOAT3(3945.0f, 1305.0f, 0.0f);

		g_CO[16].use = TRUE;
		g_CO[16].cwx = 300;
		g_CO[16].chy = 20;
		g_CO[16].pos = XMFLOAT3(4390.0f, 1305.0f, 0.0f);

		g_CO[17].use = TRUE;
		g_CO[17].cwx = 1250;
		g_CO[17].chy = 20;
		g_CO[17].pos = XMFLOAT3(5135.0f, 2135.0f, 0.0f);

		g_CO[18].use = TRUE;
		g_CO[18].cwx = 350;
		g_CO[18].chy = 15;
		g_CO[18].pos = XMFLOAT3(6220.0f, 1600.0f, 0.0f);

		g_CO[19].use = TRUE;
		g_CO[19].cwx = 310;
		g_CO[19].chy = 20;
		g_CO[19].pos = XMFLOAT3(6545.0f, 1965.0f, 0.0f);

		g_CO[20].use = TRUE;
		g_CO[20].cwx = 1220;
		g_CO[20].chy = 380;
		g_CO[20].pos = XMFLOAT3(5440.0f, 1495.0f, 0.0f);

		g_CO[21].use = TRUE;
		g_CO[21].cwx = 2030;
		g_CO[21].chy = 290;
		g_CO[21].pos = XMFLOAT3(6010.0f, 1160.0f, 0.0f);

		g_CO[22].use = TRUE;
		g_CO[22].cwx = 310;
		g_CO[22].chy = 1100;
		g_CO[22].pos = XMFLOAT3(6860.0f, 1800.0f, 0.0f);

		g_CO[23].use = TRUE;
		g_CO[23].cwx = 1000;
		g_CO[23].chy = 1700;
		g_CO[23].pos = XMFLOAT3(7510.0f, 1495.0f, 0.0f);
		break;

	case MODE_GAME_MAP3:
		g_CO[0].use = TRUE;
		g_CO[0].cwx = 8000;
		g_CO[0].chy = 210;
		g_CO[0].pos = XMFLOAT3(4000.0f, 2400.0f, 0.0f);

		g_CO[1].use = TRUE;
		g_CO[1].cwx = 720;
		g_CO[1].chy = 460;
		g_CO[1].pos = XMFLOAT3(190.0f, 880.0f, 0.0f);

		g_CO[2].use = TRUE;
		g_CO[2].cwx = 215;
		g_CO[2].chy = 650;
		g_CO[2].pos = XMFLOAT3(75.0f, 1420.0f, 0.0f);

		g_CO[3].use = TRUE;
		g_CO[3].cwx = 2000;
		g_CO[3].chy = 1000;
		g_CO[3].pos = XMFLOAT3(115.0f, 2035.0f, 0.0f);

		g_CO[4].use = TRUE;
		g_CO[4].cwx = 315;
		g_CO[4].chy = 20;
		g_CO[4].pos = XMFLOAT3(875.0f, 805.0f, 0.0f);

		g_CO[5].use = TRUE;
		g_CO[5].cwx = 315;
		g_CO[5].chy = 20;
		g_CO[5].pos = XMFLOAT3(1475.0f, 1070.0f, 0.0f);

		g_CO[6].use = TRUE;
		g_CO[6].cwx = 470;
		g_CO[6].chy = 600;
		g_CO[6].pos = XMFLOAT3(1685.0f, 2080.0f, 0.0f);

		g_CO[7].use = TRUE;
		g_CO[7].cwx = 1230;
		g_CO[7].chy = 270;
		g_CO[7].pos = XMFLOAT3(2540.0f, 1220.0f, 0.0f);

		g_CO[8].use = TRUE;
		g_CO[8].cwx = 615;
		g_CO[8].chy = 400;
		g_CO[8].pos = XMFLOAT3(2545.0f, 2260.0f, 0.0f);

		g_CO[9].use = TRUE;
		g_CO[9].cwx = 600;
		g_CO[9].chy = 20;
		g_CO[9].pos = XMFLOAT3(3800.0f, 1045.0f, 0.0f);

		g_CO[10].use = TRUE;
		g_CO[10].cwx = 1715;
		g_CO[10].chy = 20;
		g_CO[10].pos = XMFLOAT3(5050.0f, 1565.0f, 0.0f);

		g_CO[11].use = TRUE;
		g_CO[11].cwx = 310;
		g_CO[11].chy = 20;
		g_CO[11].pos = XMFLOAT3(6825.0f, 1845.0f, 0.0f);

		g_CO[12].use = TRUE;
		g_CO[12].cwx = 800;
		g_CO[12].chy = 700;
		g_CO[12].pos = XMFLOAT3(7665.0f, 2185.0f, 0.0f);
		break;

	case MODE_GAME_MAP4:
		g_CO[0].use = TRUE;
		g_CO[0].cwx = 8000;
		g_CO[0].chy = 210;
		g_CO[0].pos = XMFLOAT3(4000.0f, 2400.0f, 0.0f);
		break;
	
	}



#ifdef _DEBUG	// デバッグ情報を表示する


#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBG(void)
{
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
		switch (GetMode())
		{
		case MODE_GAME:
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLTColor(g_VertexBuffer,
				0 - g_BG.pos.x, 0 - g_BG.pos.y, g_BG.w, g_BG.h,
				0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
			break;

		case MODE_GAME_MAP2:
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLTColor(g_VertexBuffer,
				0 - g_BG.pos.x, 0 - g_BG.pos.y, g_BG.w, g_BG.h,
				0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
			break;

		case MODE_GAME_MAP3:
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLTColor(g_VertexBuffer,
				0 - g_BG.pos.x, 0 - g_BG.pos.y, g_BG.w, g_BG.h,
				0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
			break;

		case MODE_GAME_MAP4:
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLTColor(g_VertexBuffer,
				0 - g_BG.pos.x, 0 - g_BG.pos.y, g_BG.w, g_BG.h,
				0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
			break;
		}
	}

	//// UI Panelを描画
	//{
	//	// テクスチャ設定
	//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[4]);

	//	// １枚のポリゴンの頂点とテクスチャ座標を設定
	//	SetSpriteLeftTop(g_VertexBuffer, 10.0f, 920.0f, UI_PANEL_WIDTH, UI_PANEL_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

	//	// ポリゴン描画
	//	GetDeviceContext()->Draw(4, 0);
	//}





#ifdef _DEBUG	// デバッグ情報を表示する
	//テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[5]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	for (int i = 0; i < CO_TEXTURE_MAX; i++)
	{
		if (g_CO[i].use == TRUE) 
		{
			SetSpriteColor(g_VertexBuffer,
			g_CO[i].pos.x-g_BG.pos.x,g_CO[i].pos.y-g_BG.pos.y, g_CO[i].cwx, g_CO[i].chy,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

#endif
}





//=============================================================================
// BG構造体の先頭アドレスを取得
//=============================================================================
BG* GetBG(void)
{
	return &g_BG;
}

COLLISION* GetCOLLISION(void)
{
	return &g_CO[0];
}






