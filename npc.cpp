//=============================================================================
//
// NPC処理 [npc.cpp]
// Author : GP11B132 05 王ウ華
//
//=============================================================================
#include "npc.h"
#include "bg.h"
#include "player.h"
#include "fade.h"
#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

// npc
#define TEXTURE_WIDTH				(300)	// エネミーサイズ
#define TEXTURE_HEIGHT				(300)	// 
#define TEXTURE_MAX					(25)	// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(4)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ANIM_WAIT					(4)		// アニメーションの切り替わるWait値

// dialogue box
#define DIALOGUE_BOX_WIDTH			(700)
#define	DIALOGUE_BOX_HEIGHT			(700)

// item
#define ITEM_TEXTURE_WIDTH			(100)
#define ITEM_TEXTURE_HEIGHT			(100)
#define ITEM_TEXTURE_MAX			(5)

#define PI							(3.14159263)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/NPC/NPC1W.png",			//0
	"data/NPC/NPC2OM.png",
	"data/NPC/NPC3B.png",
	"data/NPC/NPC4OW.png",
	"data/NPC/NPC5M.png",		
	"data/NPC/NPCSP.png",			//5

	"data/DIALOGUE/omoa.png",		//6
	"data/DIALOGUE/omob.png",
	"data/DIALOGUE/omoc.png",	
	"data/DIALOGUE/omod.png",
	"data/DIALOGUE/omoe.png",		//10
	"data/DIALOGUE/true.png",		//11
	"data/DIALOGUE/uraa.png",		//12
	"data/DIALOGUE/urab.png",
	"data/DIALOGUE/urac.png",
	"data/DIALOGUE/urad.png",
	"data/DIALOGUE/urae.png",		//16

	"data/NPC/mainitem1.png",		//17	// flower
	"data/NPC/mainitem2.png",				// flower+book
	"data/NPC/mainitem3.png",		//19	// bracelet

	"data/NPC/TEP.png",				//20
	"data/DIALOGUE/te.png",

	"data/TEXTURE/bar_white.png",	//22
};


static BOOL		g_Load = FALSE;					// 初期化を行ったかのフラグ
static NPC		g_NPC[NPC_MAX];					// NPC構造体
static NC		g_NC[NC_TEXTURE_MAX];			// NPC collision
static ITEM		g_ITEM[ITEM_MAX];				// item 構造体

static int		g_NPCCnt  = NPC_MAX;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitNPC(void)
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


	// NPC構造体の初期化
	g_NPCCnt = 0;

	for (int i = 0; i < NPC_MAX; i++)
	{
		switch (GetMode())
		{
		case MODE_TITLE:
			g_NPC[i].teflag = FALSE;
			break;

		case MODE_GAME:
			g_NC[i].use = FALSE;
			g_NC[i].nr = g_NPC[i].pos.x + (g_NC[i].cwx / 2);
			g_NC[i].nl = g_NPC[i].pos.x - (g_NC[i].cwx / 2);
			g_NC[i].nu = g_NPC[i].pos.y - (g_NC[i].chy / 2);
			g_NC[i].nd = g_NPC[i].pos.y + (g_NC[i].chy / 2);

			g_NPC[i].use = FALSE;
			g_NPC[i].pos = XMFLOAT3(200.0f, 100.0f, 0.0f);		// 中心点から表示
			g_NPC[i].w = TEXTURE_WIDTH;
			g_NPC[i].h = TEXTURE_HEIGHT;
			g_NPC[i].texNo = 0;
			g_NPC[i].ctexNo = 0;
			g_NPC[i].countAnim = 0;
			g_NPC[i].patternAnim = 0;

			g_ITEM[i].iuse = FALSE;
			g_ITEM[i].ipos = XMFLOAT3(200.0f, 100.0f, 0.0f);		// 中心点から表示
			g_ITEM[i].iw = ITEM_TEXTURE_WIDTH;
			g_ITEM[i].ih = ITEM_TEXTURE_HEIGHT;
			g_ITEM[i].itexNo = 0;

			g_NPCCnt++;
			break;

		case MODE_GAME_MAP2:
			g_NC[i].use = FALSE;
			g_NC[i].nr = g_NPC[i].pos.x + (g_NC[i].cwx / 2);
			g_NC[i].nl = g_NPC[i].pos.x - (g_NC[i].cwx / 2);
			g_NC[i].nu = g_NPC[i].pos.y - (g_NC[i].chy / 2);
			g_NC[i].nd = g_NPC[i].pos.y + (g_NC[i].chy / 2);

			g_NPC[i].use = FALSE;
			g_NPC[i].pos = XMFLOAT3(200.0f, 100.0f, 0.0f);		// 中心点から表示
			g_NPC[i].w = TEXTURE_WIDTH;
			g_NPC[i].h = TEXTURE_HEIGHT;
			g_NPC[i].texNo = 0;
			g_NPC[i].ctexNo = 0;
			g_NPC[i].countAnim = 0;
			g_NPC[i].patternAnim = 0;

			g_ITEM[i].iuse = FALSE;
			g_ITEM[i].ipos = XMFLOAT3(200.0f, 100.0f, 0.0f);	// 中心点から表示
			g_ITEM[i].iw = ITEM_TEXTURE_WIDTH;
			g_ITEM[i].ih = ITEM_TEXTURE_HEIGHT;
			g_ITEM[i].itexNo = 0;

			g_NPCCnt++;
			break;

		case MODE_GAME_MAP3:
			g_NC[i].use = FALSE;
			g_NC[i].nr = g_NPC[i].pos.x + (g_NC[i].cwx / 2);
			g_NC[i].nl = g_NPC[i].pos.x - (g_NC[i].cwx / 2);
			g_NC[i].nu = g_NPC[i].pos.y - (g_NC[i].chy / 2);
			g_NC[i].nd = g_NPC[i].pos.y + (g_NC[i].chy / 2);

			g_NPC[i].use = FALSE;
			g_NPC[i].pos = XMFLOAT3(200.0f, 100.0f, 0.0f);		// 中心点から表示
			g_NPC[i].w = TEXTURE_WIDTH;
			g_NPC[i].h = TEXTURE_HEIGHT;
			g_NPC[i].texNo = 0;
			g_NPC[i].ctexNo = 0;
			g_NPC[i].countAnim = 0;
			g_NPC[i].patternAnim = 0;

			g_ITEM[i].iuse = FALSE;
			g_ITEM[i].ipos = XMFLOAT3(200.0f, 100.0f, 0.0f);	// 中心点から表示
			g_ITEM[i].iw = ITEM_TEXTURE_WIDTH;
			g_ITEM[i].ih = ITEM_TEXTURE_HEIGHT;
			g_ITEM[i].itexNo = 0;

			g_NPCCnt++;
			break;
		
		case MODE_GAME_MAP4:
			g_NC[i].use = FALSE;
			g_NC[i].nr = g_NPC[i].pos.x + (g_NC[i].cwx / 2);
			g_NC[i].nl = g_NPC[i].pos.x - (g_NC[i].cwx / 2);
			g_NC[i].nu = g_NPC[i].pos.y - (g_NC[i].chy / 2);
			g_NC[i].nd = g_NPC[i].pos.y + (g_NC[i].chy / 2);

			g_NPC[i].use = FALSE;
			g_NPC[i].pos = XMFLOAT3(200.0f, 100.0f, 0.0f);		// 中心点から表示
			g_NPC[i].w = TEXTURE_WIDTH;
			g_NPC[i].h = TEXTURE_HEIGHT;
			g_NPC[i].texNo = 0;
			g_NPC[i].ctexNo = 0;
			g_NPC[i].countAnim = 0;
			g_NPC[i].patternAnim = 0;

			g_ITEM[i].iuse = FALSE;
			g_ITEM[i].ipos = XMFLOAT3(200.0f, 100.0f, 0.0f);	// 中心点から表示
			g_ITEM[i].iw = ITEM_TEXTURE_WIDTH;
			g_ITEM[i].ih = ITEM_TEXTURE_HEIGHT;
			g_ITEM[i].itexNo = 0;

			g_NPCCnt++;
			break;
		}
	}

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitNPC(void)
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
void UpdateNPC(void)
{
	g_NPCCnt = 0;	

	// NPC処理をする
	for (int i = 0; i < NPC_MAX; i++)
	{
		// アニメーション  
		g_NPC[i].countAnim += 1.0f;
		if (g_NPC[i].countAnim > ANIM_WAIT)
		{
			g_NPC[i].countAnim = 0.0f;
			// パターンの切り替え
			g_NPC[i].patternAnim = (g_NPC[i].patternAnim + 1) % ANIM_PATTERN_NUM;
		}
	}

	// npc position & collision
	for (int t = 0; t < NPC_MAX; t++)
	{
		switch (GetMode())
		{
		case MODE_GAME:
			g_NPC[0].use = TRUE;
			g_NPC[0].pos = XMFLOAT3(4500.0f, 2150.0f, 0.0f);	// 中心点から表示
			g_NPC[0].texNo = 0;
			g_NPC[0].ctexNo = 6;
			g_NC[0].use = TRUE;		
			g_NC[0].cwx = 300;
			g_NC[0].chy = 300;
			g_NC[0].pos = XMFLOAT3(4500.0f, 2150.0f, 0.0f);

			g_NPC[1].use = TRUE;
			g_NPC[1].pos = XMFLOAT3(5700.0f, 2150.0f, 0.0f);	// 中心点から表示
			g_NPC[1].texNo = 1;
			g_NPC[1].ctexNo = 7;
			g_NC[1].use = TRUE;
			g_NC[1].cwx = 300;
			g_NC[1].chy = 300;
			g_NC[1].pos = XMFLOAT3(5700.0f, 2150.0f, 0.0f);

			g_NPC[2].use = TRUE;
			g_NPC[2].pos = XMFLOAT3(5300.0f, 1450.0f, 0.0f);	// 中心点から表示
			g_NPC[2].texNo = 2;
			g_NPC[2].ctexNo = 8;
			g_NC[2].use = TRUE;
			g_NC[2].cwx = 300;
			g_NC[2].chy = 300;
			g_NC[2].pos = XMFLOAT3(5300.0f, 1450.0f, 0.0f);

			g_NPC[3].use = TRUE;
			g_NPC[3].pos = XMFLOAT3(7100.0f, 2150.0f, 0.0f);	// 中心点から表示
			g_NPC[3].texNo = 3;
			g_NPC[3].ctexNo = 9;
			g_NC[3].use = TRUE;
			g_NC[3].cwx = 300;
			g_NC[3].chy = 300;
			g_NC[3].pos = XMFLOAT3(7100.0f, 2150.0f, 0.0f);

			g_NPC[4].use = TRUE;
			g_NPC[4].pos = XMFLOAT3(7500.0f, 650.0f, 0.0f);		// 中心点から表示
			g_NPC[4].texNo = 4;
			g_NPC[4].ctexNo = 10;
			g_NC[4].use = TRUE;
			g_NC[4].cwx = 300;
			g_NC[4].chy = 300;
			g_NC[4].pos = XMFLOAT3(7500.0f, 650.0f, 0.0f);

			g_ITEM[0].iuse = TRUE;	// flower
			g_ITEM[0].ipos = XMFLOAT3(2000.0f, 800.0f, 0.0f);	// 中心点から表示
			g_NC[5].use = TRUE;	
			g_NC[5].cwx = 600;
			g_NC[5].chy = 230;
			g_NC[5].pos = XMFLOAT3(4000.0f, 2150.0f, 0.0f);
			g_ITEM[0].itexNo = 17;
			break;

		case MODE_GAME_MAP2:
			g_NPC[0].use = TRUE;	// SPNPC
			g_NPC[0].pos = XMFLOAT3(7100.0f, 500.0f, 0.0f);		// 中心点から表示
			g_NPC[0].texNo = 5;
			g_NPC[0].ctexNo = 11;
			g_NC[0].use = TRUE;	
			g_NC[0].cwx = 200;
			g_NC[0].chy = 1000;
			g_NC[0].pos = XMFLOAT3(7100.0f, 500.0f, 0.0f);

			if (g_NPC[t].teflag == TRUE)
			{
				// SPNPC
				g_NPC[0].use = FALSE;
				g_NC[0].use = FALSE;

				// flower+book
				g_NPC[1].use = TRUE;
				g_NPC[1].pos = XMFLOAT3(7500.0f, 200.0f, 0.0f);	// 中心点から表示
				g_NC[1].use = TRUE;	
				g_NC[1].cwx = 200;
				g_NC[1].chy = 1000;
				g_NC[1].pos = XMFLOAT3(7100.0f, 500.0f, 0.0f);
				g_ITEM[0].itexNo = 18;
			}
			break;

		case MODE_GAME_MAP3:
			g_NPC[t].teflag = TRUE;		// te conversation change

			g_NPC[0].use = TRUE;		// bracelet
			g_NPC[0].pos = XMFLOAT3(7500.0f, 2150.0f, 0.0f);	// 中心点から表示
			g_NC[0].use = TRUE;
			g_NC[0].cwx = 300;
			g_NC[0].chy = 2500;
			g_NC[0].pos = XMFLOAT3(7500.0f, 2150.0f, 0.0f);
			g_ITEM[0].itexNo = 19;
			break;

		case MODE_GAME_MAP4:
			g_NPC[0].use = TRUE;		// ending hero
			g_NPC[0].pos = XMFLOAT3(7500.0f, 2150.0f, 0.0f);	// 中心点から表示
			g_NPC[0].texNo = 20;
			g_NPC[0].ctexNo = 21;
			g_NC[0].use = TRUE;		
			g_NC[0].cwx = 300;
			g_NC[0].chy = 2500;
			g_NC[0].pos = XMFLOAT3(7500.0f, 2150.0f, 0.0f);
			break;
		}
	}

	// NPC collision jadge
	// 目前敵人不會移動所以碰撞判定寫在哪邊影響不大,但若日後會移動,先行碰撞判定再決定座標的話會影響顯示結果,須注意
	PLAYER* g_Player = GetPlayer();

	for (int n = 0; n < NC_TEXTURE_MAX; n++)
	{
		if (g_NC[n].use == TRUE)
		{
			BOOL ncollision_ans = CollisionBB(g_Player[0].pos, g_Player[0].w, g_Player[0].h, g_NC[n].pos, g_NC[n].cwx, g_NC[n].chy);

			// 判断
			if (ncollision_ans == TRUE)
			{
				g_NPC[n].conbox = TRUE;
			}
			else
			{
				g_NPC[n].conbox = FALSE;
			}
		}
	}

	// item collision jadge
	for (int e = 0; e < ITEM_MAX; e++)
	{
		if (g_ITEM[e].iuse == TRUE)
		{
			BOOL icollision_ans = CollisionBB(g_Player[0].pos, g_Player[0].w, g_Player[0].h, g_NC[e].pos, g_NC[e].cwx, g_NC[e].chy);

			// 判断
			if (icollision_ans == TRUE)
			{
				g_ITEM[e].iconbox = TRUE;
			}
			else
			{
				g_ITEM[e].iconbox = TRUE;
			}

		}
	}



#ifdef _DEBUG	// デバッグ情報を表示する


#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawNPC(void)
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

	for (int i = 0; i < NPC_MAX; i++)
	{
		if (g_NPC[i].use == TRUE)
		{
			BG* bg = GetBG();

			// NPC
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_NPC[i].texNo]);

			// NPCの位置やテクスチャー座標を反映
			float px = g_NPC[i].pos.x - bg->pos.x;		// NPCの表示位置X
			float py = g_NPC[i].pos.y - bg->pos.y;		// NPCの表示位置Y
			float pw = g_NPC[i].w;		// NPCの表示幅
			float ph = g_NPC[i].h;		// NPCの表示高さ

			// アニメーション用
			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅 
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
			float tx = (float)(g_NPC[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
			float ty = (float)(g_NPC[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				g_NPC[i].rot.z);
		}

		// dialogue box
		BG* bg = GetBG();

		if (g_NPC[i].conbox == TRUE)
		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_NPC[i].ctexNo]);

			// NPCの位置やテクスチャー座標を反映
			float px = (g_NPC[i].pos.x - bg->pos.x) - (DIALOGUE_BOX_WIDTH / 2);	// dialogueの表示位置X
			float py = (g_NPC[i].pos.y - bg->pos.y) - (DIALOGUE_BOX_HEIGHT / 4);	// dialogueの表示位置Y
			float pw = DIALOGUE_BOX_WIDTH;			// dialogueの表示幅
			float ph = DIALOGUE_BOX_HEIGHT;			// dialogueの表示高さ

			// texture
			float tw = 1.0f;		// テクスチャの幅
			float th = 1.0f;		// テクスチャの高さ
			float tx = 0.0f;		// テクスチャの左上X座標
			float ty = 0.0f;		// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				g_NPC[i].rot.z);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

		// item
		for (int i = 0; i < ITEM_MAX; i++)
		{
			if (g_ITEM[i].iuse == TRUE)
			{
				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);

				// item
				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_ITEM[i].itexNo]);

				float px1 = (g_NPC[i].pos.x - bg->pos.x) - (ITEM_TEXTURE_WIDTH);	// itemの表示位置X
				float py1 = (g_NPC[i].pos.y - bg->pos.y) - (ITEM_TEXTURE_HEIGHT);	// itemの表示位置Y
				float pw1 = ITEM_TEXTURE_WIDTH;			// itemの表示幅
				float ph1 = ITEM_TEXTURE_HEIGHT;			// itemの表示高さ

				// texture
				float tw1 = 1.0f;		// テクスチャの幅
				float th1 = 1.0f;		// テクスチャの高さ
				float tx1 = 0.0f;		// テクスチャの左上X座標
				float ty1 = 0.0f;		// テクスチャの左上Y座標

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColorRotation(g_VertexBuffer, px1, py1, pw1, ph1, tx1, ty1, tw1, th1,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
					g_NPC[i].rot.z);

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
			}
		}



#ifdef _DEBUG	// デバッグ情報を表示する
		//テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[22]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
			if (g_NC[i].use == TRUE)
			{
				SetSpriteColor(g_VertexBuffer,
					g_NPC[i].pos.x - bg->pos.x, g_NPC[i].pos.y - bg->pos.y, g_NPC[i].w, g_NPC[i].h,
					0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
			}
#endif
		
	}
}


//=============================================================================
// NPC構造体の先頭アドレスを取得
//=============================================================================
NPC* GetNPC(void)
{
	return &g_NPC[0];
}

NC* GetNC(void)
{
	return &g_NC[0];
}

ITEM* GetITEM(void)
{
	return &g_ITEM[0];
}





