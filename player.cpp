//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "bg.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "score.h"
#include "file.h"
#include "fade.h"
#include "npc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(300)	// キャラサイズ
#define TEXTURE_HEIGHT				(300)	// 
#define TEXTURE_MAX					(25)	// テクスチャの数

#define SPRITE_DIVIDE_X				(7)
#define SPRITE_DIVIDE_Y				(11)

// idle
#define TEXTURE_PATTERN_DIVIDE_X	(4)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)		// アニメーションパターン数
#define ANIM_WAIT					(6)		// アニメーションの切り替わるWait値

// fall
#define FTEXTURE_PATTERN_DIVIDE_X	(2)		// アニメパターンのテクスチャ内分割数（X)
#define FTEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define FANIM_PATTERN_NUM			(FTEXTURE_PATTERN_DIVIDE_X*FTEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数

// run 
#define RTEXTURE_PATTERN_DIVIDE_X	(6)		// アニメパターンのテクスチャ内分割数（X)
#define RTEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define RANIM_PATTERN_NUM			(RTEXTURE_PATTERN_DIVIDE_X*RTEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数

// jump
#define JTEXTURE_PATTERN_DIVIDE_X	(8)		// アニメパターンのテクスチャ内分割数（X)
#define JTEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define JANIM_PATTERN_NUM			(JTEXTURE_PATTERN_DIVIDE_X*JTEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数

// attack
#define ATEXTURE_PATTERN_DIVIDE_X	(6)		// アニメパターンのテクスチャ内分割数（X)
#define ATEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define AANIM_PATTERN_NUM			(6)		// アニメーションパターン数

// プレイヤーの画面内配置座標
#define PLAYER_DISP_X				(SCREEN_WIDTH / 2)
#define PLAYER_DISP_Y				(SCREEN_HEIGHT / 2)

// ジャンプ処理
#define	PLAYER_JUMP_CNT_MAX			(24)	// 24フレームJUMP_MAX
#define	PLAYER_JUMP_Y_MAX			(300)	// ジャンプの高さ

// attack処理
#define PLAYER_ATTACK_CNT_MAX		(24)	// フレームATTACK_MAX

// dialogue box size
#define MESSAGE_BOX_WIDTH			(100)
#define	MESSAGE_BOX_HEIGHT			(400)

// blood gauge
#define GAUGE_WIDTH					(650)	// gauge size
#define GAUGE_HEIGHT				(165)			

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void DrawPlayerOffset(int no);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	// character(0~1)
	"data/PLAYER/char01.png",			//0
	"data/PLAYER/idleleft.png",			
	"data/PLAYER/runright.png",
	"data/PLAYER/runleft.png",
	"data/PLAYER/attackright.png",
	"data/PLAYER/attackleft.png",

	"data/PLAYER/shadow000.jpg",		//6
	"data/PLAYER/bar_white.png",

	// dialogue box(2~15)
	"data/DIALOGUE/applenone.png",		//8
	"data/DIALOGUE/potionnone.png",
	"data/ITEM/bloodgauge.png",			//10

	"data/TEXTURE/bar_white.png",

};


static BOOL		g_Load = FALSE;			// 初期化を行ったかのフラグ
static BOOL		g_teflag = FALSE;		// TEフラグ
static PLAYER	g_Player[PLAYER_MAX];	// プレイヤー構造体

static int      g_jumpCnt = 0;
//static int		g_jump[PLAYER_JUMP_CNT_MAX] =
//{
//	-15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5,-4,-3,-2,-1,
//	  1,   2,   3,   4,   5,   6,  7,  8,  9, 10, 11,12,13,14,15
//};

static float g_tamp;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
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


	// プレイヤー構造体の初期化
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_Player[i].pos = XMFLOAT3(200.0f, 200.0f, 0.0f);	// 中心点から表示
		g_Player[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Player[i].w = TEXTURE_WIDTH;
		g_Player[i].h = TEXTURE_HEIGHT;
		g_Player[i].texNo = 0;
		
		g_Player[i].cPos = XMFLOAT3(g_Player[i].pos.x, g_Player[i].pos.y, g_Player[i].pos.z);
		g_Player[i].cw = 100.0f;
		g_Player[i].ch = 250.0f;

		g_Player[i].countAnim = 0;
		g_Player[i].patternAnim = 0;

		// moving
		g_Player[i].move = XMFLOAT3(10.0f, 0.0f, 0.0f);		// 移動量
		g_Player[i].dir = CHAR_DIR_DOWN;					// 下向きにしとくか
		g_Player[i].moving = FALSE;							// 移動中フラグ
		g_Player[i].patternAnim = g_Player[i].dir * TEXTURE_PATTERN_DIVIDE_X;

		// jump
		g_Player[i].jump = FALSE;
		g_Player[i].jumpCnt = 0;
		g_Player[i].jumpY = 30.0f;
		g_Player[i].jumpYMax = PLAYER_JUMP_Y_MAX;

		// attack
		g_Player[i].atkuse = FALSE;
		g_Player[i].atkCnt = 0;


		// 分身用
		g_Player[i].dash = FALSE;
		for (int j = 0; j < PLAYER_OFFSET_CNT; j++)
		{
			g_Player[i].offset[j] = g_Player[i].pos;
		}
		
		switch (GetMode())
		{
		case MODE_GAME:
			// player status
			g_Player[i].hp = 300;
			g_Player[i].apple = 5;
			g_Player[i].potion = 3;

			// collision part
			g_Player[i].use = TRUE;
			g_Player[i].cpflag = TRUE;
			g_Player[i].pr = g_Player[i].pos.x + (TEXTURE_WIDTH / 2);
			g_Player[i].pl = g_Player[i].pos.x - (TEXTURE_WIDTH / 2);
			g_Player[i].pu = g_Player[i].pos.y - (TEXTURE_HEIGHT / 2);
			g_Player[i].pd = g_Player[i].pos.y + (TEXTURE_HEIGHT / 2);
			
			g_Player[i].pos = XMFLOAT3(200.0f, 200.0f, 0.0f);	// 中心点から表示
			g_Player[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Player[i].w = TEXTURE_WIDTH;
			g_Player[i].h = TEXTURE_HEIGHT;
			g_Player[i].texNo = 0;
			g_Player[i].gaugepos = XMFLOAT3(200.0f, 200.0f, 0.0f);

			break;

		case MODE_GAME_MAP2:
			// collision part
			g_Player[i].use = TRUE;
			g_Player[i].cpflag = TRUE;
			g_Player[i].pr = g_Player[i].pos.x + (TEXTURE_WIDTH / 2);
			g_Player[i].pl = g_Player[i].pos.x - (TEXTURE_WIDTH / 2);
			g_Player[i].pu = g_Player[i].pos.y - (TEXTURE_HEIGHT / 2);
			g_Player[i].pd = g_Player[i].pos.y + (TEXTURE_HEIGHT / 2);

			g_Player[i].pos = XMFLOAT3(200.0f, 1350.0f, 0.0f);	// 中心点から表示
			g_Player[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Player[i].w = TEXTURE_WIDTH;
			g_Player[i].h = TEXTURE_HEIGHT;
			g_Player[i].texNo = 0;
			g_Player[i].gaugepos = XMFLOAT3(200.0f, 200.0f, 0.0f);

			break;

		case MODE_GAME_MAP3:
			// collision part
			g_Player[i].use = TRUE;
			g_Player[i].cpflag = TRUE;
			g_Player[i].pr = g_Player[i].pos.x + (TEXTURE_WIDTH / 2);
			g_Player[i].pl = g_Player[i].pos.x - (TEXTURE_WIDTH / 2);
			g_Player[i].pu = g_Player[i].pos.y - (TEXTURE_HEIGHT / 2);
			g_Player[i].pd = g_Player[i].pos.y + (TEXTURE_HEIGHT / 2);

			g_Player[i].pos = XMFLOAT3(300.0f, 300.0f, 0.0f);	// 中心点から表示
			g_Player[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Player[i].w = TEXTURE_WIDTH;
			g_Player[i].h = TEXTURE_HEIGHT;
			g_Player[i].texNo = 0;
			g_Player[i].gaugepos = XMFLOAT3(200.0f, 200.0f, 0.0f);

			break;

		case MODE_GAME_MAP4:
			// collision part
			g_Player[i].use = TRUE;
			g_Player[i].cpflag = TRUE;
			g_Player[i].pr = g_Player[i].pos.x + (TEXTURE_WIDTH / 2);
			g_Player[i].pl = g_Player[i].pos.x - (TEXTURE_WIDTH / 2);
			g_Player[i].pu = g_Player[i].pos.y - (TEXTURE_HEIGHT / 2);
			g_Player[i].pd = g_Player[i].pos.y + (TEXTURE_HEIGHT / 2);

			g_Player[i].pos = XMFLOAT3(400.0f, 400.0f, 0.0f);	// 中心点から表示
			g_Player[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Player[i].w = TEXTURE_WIDTH;
			g_Player[i].h = TEXTURE_HEIGHT;
			g_Player[i].texNo = 0;
			g_Player[i].gaugepos = XMFLOAT3(200.0f, 200.0f, 0.0f);

			break;
		}

		g_Load = TRUE;
		g_tamp = 0.0f;
		return S_OK;
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
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
void UpdatePlayer(void)
{

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		// 生きてるプレイヤーだけ処理をする
		if (g_Player[i].use == TRUE)
		{
			// 地形との当たり判定用に座標のバックアップを取っておく
			XMFLOAT3 pos_old = g_Player[i].pos;
			g_Player[i].cPos = XMFLOAT3(g_Player[i].pos.x, g_Player[i].pos.y + 20.0f, g_Player[i].pos.z);


			// collision地形との当たり判定
			COLLISION* collision = GetCOLLISION();
			g_Player[i].moving = FALSE;
			g_Player[i].onGround = FALSE;

			float speed = g_Player[i].move.x;
			g_Player[i].pos.y += speed;

			for (int c = 0; c < CO_TEXTURE_MAX; c++)
			{
				if (collision[c].use == TRUE) 
				{
					BOOL collision_ans = CollisionBB(g_Player[i].cPos, g_Player[i].cw, g_Player[i].ch, collision[c].pos, collision[c].cwx, collision[c].chy);

					// 判断
					if (collision_ans == TRUE) 
					{	
						int top = fabsf((g_Player[i].cPos.y + (g_Player[i].ch / 2)) - (collision[c].pos.y - (collision[c].chy / 2)));		// pd - gu
						int down = fabsf((collision[c].pos.y + (collision[c].chy / 2)) - (g_Player[i].cPos.y - (g_Player[i].ch / 2)));		// gu - pd
						int right = fabsf((collision[c].pos.x - (collision[c].cwx / 2)) - (g_Player[i].cPos.x + (g_Player[i].cw / 2)));		// gl - pr
						int left = fabsf((g_Player[i].cPos.x - (g_Player[i].cw / 2)) - (collision[c].pos.x + (collision[c].cwx / 2)));		// pl - gr
						int min = min(top, min(down, min(right, left)));

						// compare
						if (min == top)
						{
							g_Player[i].pos.y = pos_old.y;
							g_Player[i].onGround = TRUE;
						}
					}
				}
			}

			// 分身用
			for (int j = PLAYER_OFFSET_CNT - 1; j > 0; j--)
			{
				g_Player[i].offset[j] = g_Player[i].offset[j - 1];
			}
			g_Player[i].offset[0] = pos_old;

			// アニメーション  
			g_Player[i].countAnim += 1.0f;					// 這個部分是在說明單一動作的偵數刷新
			if (g_Player[i].jump == TRUE)
			{
				if (g_Player[i].countAnim >= (ANIM_WAIT / 2))
				{
					g_Player[i].countAnim = 0.0f;
					// パターンの切り替え(執行下一個動作)
					g_Player[i].patternAnim++;
				}
			}
			else
			{
				if (g_Player[i].countAnim >= ANIM_WAIT)
				{
					g_Player[i].countAnim = 0.0f;
					// パターンの切り替え(執行下一個動作)
					g_Player[i].patternAnim++;
				}
			}

			// キー入力で移動 + 動作
			{	
				g_Player[i].moving = FALSE;
				g_Player[i].dash = FALSE;

				if (GetKeyboardPress(DIK_C) || IsButtonPressed(0, BUTTON_A))
				{
					speed *= 4;
					g_Player[i].dash = TRUE;
				}

				if (GetKeyboardPress(DIK_RIGHT))
				{
					g_Player[i].pos.x += speed;
					g_Player[i].dir = CHAR_DIR_RIGHT;
					g_Player[i].moving = TRUE;

					// collision
					for (int c = 0; c < CO_TEXTURE_MAX; c++)
					{
						if (collision[c].use == TRUE)
						{
							BOOL collision_ans = CollisionBB(g_Player[i].cPos, g_Player[i].cw, g_Player[i].ch, collision[c].pos, collision[c].cwx, collision[c].chy);

							// 判断
							if (collision_ans == TRUE)
							{
								int top = fabsf((g_Player[i].cPos.y + (g_Player[i].ch / 2)) - (collision[c].pos.y - (collision[c].chy / 2)));	// pd - gu
								int down = fabsf((collision[c].pos.y - (collision[c].chy / 2)) - (g_Player[i].cPos.y + (g_Player[i].ch / 2)));	// gu - pd
								int right = fabsf((collision[c].pos.x - (collision[c].cwx / 2)) - (g_Player[i].cPos.x + (g_Player[i].cw / 2)));	// gl - pr
								int left = fabsf((g_Player[i].cPos.x - (g_Player[i].cw / 2)) - (collision[c].pos.x + (collision[c].cwx / 2)));	// pl - gr
								int min = min(top, min(down, min(right, left)));

								// compare
								if (min == right)
								{
									g_Player[i].pos.x = pos_old.x;
								}
							}
						}
					}
				}
				else if (GetKeyboardPress(DIK_LEFT))
				{
					g_Player[i].pos.x -= speed;
					g_Player[i].dir = CHAR_DIR_LEFT;
					g_Player[i].moving = TRUE;

					// collision
					for (int c = 0; c < CO_TEXTURE_MAX; c++)
					{
						if (collision[c].use == TRUE)
						{
							BOOL collision_ans = CollisionBB(g_Player[i].cPos, g_Player[i].cw, g_Player[i].ch, collision[c].pos, collision[c].cwx, collision[c].chy);

							// determin
							if (collision_ans == TRUE)
							{
								int top = fabsf((g_Player[i].cPos.y + (g_Player[i].ch / 2)) - (collision[c].pos.y - (collision[c].chy / 2)));	// pd - gu
								int down = fabsf((collision[c].pos.y - (collision[c].chy / 2)) - (g_Player[i].cPos.y + (g_Player[i].ch / 2)));	// gu - pd
								int right = fabsf((collision[c].pos.x - (collision[c].cwx / 2)) - (g_Player[i].cPos.x + (g_Player[i].cw / 2)));	// gl - pr
								int left = fabsf((g_Player[i].cPos.x - (g_Player[i].cw / 2)) - (collision[c].pos.x + (collision[c].cwx / 2)));	// pl - gr
								int min = min(top, min(down, min(right, left)));

								// compare
								if (min == left)
								{
									g_Player[i].pos.x = pos_old.x;
								}
							}
						}
					}
				}

				// using item 
				//if (GetKeyboardPress(DIK_1))
				//{
				//	if (g_Player[i].apple > 0) 
				//	{
				//		g_Player[i].apple--;
				//		g_Player[i].hp += 5;
				//		if (g_Player[i].hp > 250) 
				//		{
				//			g_Player[i].hp == 250;
				//		}
				//	}
				//	else if (g_Player[i].apple == 0)
				//	{
				//		// when enough
				//		{
				//			g_Player[i].aenough = TRUE;
				//		}
				//	}
				//}
				//if (GetKeyboardPress(DIK_2))
				//{
				//	if (g_Player[i].potion > 0)
				//	{
				//		g_Player[i].potion--;
				//		g_Player[i].hp += 15;
				//		if (g_Player[i].hp > 250) 
				//		{
				//			g_Player[i].hp = 250;
				//		}
				//	}
				//	else if (g_Player[i].potion == 0)
				//	{
				//		// when enough
				//		{
				//			g_Player[i].potion = TRUE;
				//		}
				//	}
				//}

				// ゲームパッドでで移動処理
				if (IsButtonPressed(0, BUTTON_DOWN))
				{
					g_Player[i].pos.y += speed;
					g_Player[i].dir = CHAR_DIR_DOWN;
					g_Player[i].moving = TRUE;
				}
				else if (IsButtonPressed(0, BUTTON_UP))
				{
					g_Player[i].pos.y -= speed;
					g_Player[i].dir = CHAR_DIR_UP;
					g_Player[i].moving = TRUE;
				}

				if (IsButtonPressed(0, BUTTON_RIGHT))
				{
					g_Player[i].pos.x += speed;
					g_Player[i].dir = CHAR_DIR_RIGHT;
					g_Player[i].moving = TRUE;
				}
				else if (IsButtonPressed(0, BUTTON_LEFT))
				{
					g_Player[i].pos.x -= speed;
					g_Player[i].dir = CHAR_DIR_LEFT;
					g_Player[i].moving = TRUE;
				}

				// ジャンプ処理中
				if (g_Player[i].jump == TRUE)
				{
					float angle = (XM_PI / (PLAYER_JUMP_CNT_MAX *2)) * g_Player[i].jumpCnt;
					float y = g_Player[i].jumpYMax * cosf(XM_PI / 2 + angle);
					g_Player[i].jumpY = y;
					g_Player[i].pos.y = g_tamp + g_Player[i].jumpY;


					// sky collision
					for (int c = 0; c < CO_TEXTURE_MAX; c++)
					{
						if (collision[c].use == TRUE)
						{
							BOOL collision_ans = CollisionBB(g_Player[i].cPos, g_Player[i].cw, g_Player[i].ch, collision[c].pos, collision[c].cwx, collision[c].chy);

							// 判断
							if (collision_ans == TRUE)
							{
								int top = fabsf((g_Player[i].cPos.y + (g_Player[i].ch / 2)) - (collision[c].pos.y - (collision[c].chy / 2)));	// pd - gu
								int down = fabsf((collision[c].pos.y + (collision[c].chy / 2)) - (g_Player[i].cPos.y - (g_Player[i].ch / 2)));	// gu - pd
								int right = fabsf((collision[c].pos.x - (collision[c].cwx / 2)) - (g_Player[i].cPos.x + (g_Player[i].cw / 2)));	// gl - pr
								int left = fabsf((g_Player[i].cPos.x - (g_Player[i].cw / 2)) - (collision[c].pos.x + (collision[c].cwx / 2)));	// pl - gr
								int min = min(top, min(down, min(right, left)));

								// compare
								if (min == down)
								{
									g_Player[i].pos.y = pos_old.y;
								}
							}
						}
					}

					g_Player[i].jumpCnt++;
					if (g_Player[i].jumpCnt > PLAYER_JUMP_CNT_MAX)
					{
						g_Player[i].jump = FALSE;
						g_Player[i].jumpCnt = 0;
						//g_Player[i].jumpY = 0.0f;
					}
				}

				// ジャンプボタン押した(因為y的下墜有用speed++做處理因此下墜的部分不需要再用波型計算補足)
				else if ((g_Player[i].jump == FALSE) && (GetKeyboardTrigger(DIK_J)))
				{
					g_Player[i].onGround = FALSE;
					g_Player[i].jump = TRUE;
					g_Player[i].jumpCnt = 0;
					g_Player[i].jumpY = 0.0f;
					g_Player[i].patternAnim = 0;
					g_tamp = g_Player[i].pos.y;
				}

				// attack (做完整個攻擊動作後才可以在攻擊和上方jump的概念相同)
				if ((g_Player[i].attack == FALSE) && GetKeyboardTrigger(DIK_SPACE))
				{
					g_Player[i].attack = TRUE;
					g_Player[i].atkCnt = 0;
					g_Player[i].patternAnim = 0;

				}

				// MAP外チェック
				BG* bg = GetBG();

				if (g_Player[i].pos.x < 0.0f)
				{
					g_Player[i].pos.x = 0.0f;
				}

				if (g_Player[i].pos.x > bg->w)
				{
					g_Player[i].pos.x = bg->w;
				}

				if (g_Player[i].pos.y < 0.0f)
				{
					g_Player[i].pos.y = 0.0f;
				}

				if (g_Player[i].pos.y > bg->h)
				{
					g_Player[i].pos.y = bg->h;
				}

				// change map
				if (g_Player[i].pos.x == 8000) 
				{
					switch (GetMode()) 
					{
					case MODE_GAME:
						SetFade(FADE_OUT,MODE_GAME_MAP2);
						break;

					case MODE_GAME_MAP2:
						SetFade(FADE_OUT, MODE_GAME_MAP3);
						break;

					case MODE_GAME_MAP3:
						SetFade(FADE_OUT, MODE_GAME_MAP4);
						break;

					case MODE_GAME_MAP4:
						SetFade(FADE_OUT, MODE_END);
						break;
					}
				}
				if (g_Player[i].pos.x <= 1) 
				{
					switch (GetMode()) 
					{
					case MODE_GAME_MAP2:
						SetFade(FADE_OUT, MODE_GAME);
						break;
					
					case MODE_GAME_MAP3:
						SetFade(FADE_OUT, MODE_GAME_MAP2);
						break;
					}
				}

				// プレイヤーの立ち位置からMAPのスクロール座標を計算する
				bg->pos.x = g_Player[i].pos.x - PLAYER_DISP_X;
				if (bg->pos.x < 0) bg->pos.x = 0;
				if (bg->pos.x > bg->w - SCREEN_WIDTH) bg->pos.x = bg->w - SCREEN_WIDTH;

				bg->pos.y = g_Player[i].pos.y - PLAYER_DISP_Y;
				if (bg->pos.y < 0) bg->pos.y = 0;
				if (bg->pos.y > bg->h - SCREEN_HEIGHT) bg->pos.y = bg->h - SCREEN_HEIGHT;

				// 移動が終わったらエネミーとの当たり判定
				{
					ENEMY* enemy = GetEnemy();

					// エネミーの数分当たり判定を行う
					for (int j = 0; j < ENEMY_MAX; j++)
					{
						// 生きてるground enemyと当たり判定をする
						if (enemy[j].use == TRUE)
						{
							BOOL ans = CollisionBB(g_Player[i].cPos, g_Player[i].cw, g_Player[i].ch,
								enemy[j].pos, enemy[j].w, enemy[j].h);
							// 当たっている？
							if (ans == TRUE)
							{
								// 当たった時の処理
								if ((g_Player[i].hp > 0))
								{
									// enemy status
									int enemydamage = rand() % 35;
									int enemycritical = 15 + (rand() % 40);
									g_Player[i].lucky = rand() % 65;

									if (g_Player[i].lucky < 30)
									{
										g_Player[i].hp -= enemycritical;
									}
									else if (g_Player[i].lucky >= 30)
									{
										g_Player[i].hp -= enemydamage;
									}
								}
								enemy[j].use = FALSE;		
								AddScore(10);
							}
						}
					}

				}
				// バレット処理
				if (GetKeyboardTrigger(DIK_SPACE))
				{
					XMFLOAT3 pos = g_Player[i].pos;
					pos.x += 100;
					SetBullet(pos);
				}
				if (IsButtonTriggered(0, BUTTON_B))
				{
					XMFLOAT3 pos = g_Player[i].pos;
					pos.x += g_Player[i].jumpY;
					SetBullet(pos);
				}
			}
		}

		// game over
		if (g_Player[i].hp <= 0)
		{
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}

	// 現状をセーブする
	if (GetKeyboardTrigger(DIK_S))
	{
		SaveData();
	}



#ifdef _DEBUG	// デバッグ情報を表示する
	PrintDebugProc("g_playey.pos.x: %f\n", g_Player[0].pos.x);
	PrintDebugProc("g_playey.pos.y: %f\n", g_Player[0].pos.y);
	PrintDebugProc("g_playey.jumpY: %f\n", g_Player[0].jumpY);
	PrintDebugProc("g_playey.apple: %d\n", g_Player[0].apple);
	PrintDebugProc("g_playey.potion: %d\n", g_Player[0].potion);

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
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

	BG* bg = GetBG();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[i].use == TRUE)		// このプレイヤーが使われている？
		{
			{	// 影表示
				SetBlendState(BLEND_MODE_SUBTRACT);	// 減算合成

				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[6]);

				float px = g_Player[i].pos.x - bg->pos.x;	// プレイヤーの表示位置X
				float py = g_Player[i].pos.y - bg->pos.y;	// プレイヤーの表示位置Y
				float pw = g_Player[i].w / 3;				// プレイヤーの表示幅
				float ph = g_Player[i].h / 10;				// プレイヤーの表示高さ
				py += 100.0f;								// 足元に表示

				float tw = 1.0f;	// テクスチャの幅
				float th = 1.0f;	// テクスチャの高さ
				float tx = 0.0f;	// テクスチャの左上X座標
				float ty = 0.0f;	// テクスチャの左上Y座標

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);

				SetBlendState(BLEND_MODE_ALPHABLEND);	// 半透明処理を元に戻す
			}

			// プレイヤーの分身を描画
			if (g_Player[i].dash)
			{	// ダッシュ中だけ分身処理
				DrawPlayerOffset(i);
			}

			// player action
			if (g_Player[i].onGround == FALSE && g_Player[i].jump == FALSE)
			{
				if (g_Player[i].patternAnim >= FANIM_PATTERN_NUM)
				{
					g_Player[i].patternAnim = 0;
				}

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

				//プレイヤーの位置やテクスチャー座標を反映
				float px = g_Player[i].pos.x - bg->pos.x;	// プレイヤーの表示位置X
				float py = g_Player[i].pos.y - bg->pos.y;	// プレイヤーの表示位置Y
				float pw = g_Player[i].w;					// プレイヤーの表示幅
				float ph = g_Player[i].h;					// プレイヤーの表示高さ

				if (g_Player[i].dir == CHAR_DIR_RIGHT)
				{
					// アニメーション用
					float tw = 1.0f / SPRITE_DIVIDE_X;	// テクスチャの幅
					float th = 1.0f / SPRITE_DIVIDE_Y;	// テクスチャの高さ
					float tx = (float)(g_Player[i].patternAnim + 1) * tw;	// テクスチャの左上X座標
					float ty = (float)3 * th;	// テクスチャの左上Y座標

					// １枚のポリゴンの頂点とテクスチャ座標を設定
					SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						g_Player[i].rot.z);

					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);
				}
				else
				{
					// アニメーション用
					float tw = -1.0f / SPRITE_DIVIDE_X;	// テクスチャの幅
					float th = 1.0f / SPRITE_DIVIDE_Y;	// テクスチャの高さ
					float tx = (float)(g_Player[i].patternAnim + 4) * tw;	// テクスチャの左上X座標
					float ty = 3 * th;	// テクスチャの左上Y座標

					// １枚のポリゴンの頂点とテクスチャ座標を設定
					SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						g_Player[i].rot.z);

					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);
				}
			}
			else if (g_Player[i].onGround == TRUE && g_Player[i].moving == FALSE)
			{
				if (g_Player[i].patternAnim >= ANIM_PATTERN_NUM)
				{
					g_Player[i].patternAnim = 0;
				}

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

				//プレイヤーの位置やテクスチャー座標を反映
				float px = g_Player[i].pos.x - bg->pos.x;	// プレイヤーの表示位置X
				float py = g_Player[i].pos.y - bg->pos.y;	// プレイヤーの表示位置Y
				float pw = g_Player[i].w;		// プレイヤーの表示幅
				float ph = g_Player[i].h;		// プレイヤーの表示高さ

				if (g_Player[i].dir == CHAR_DIR_RIGHT)
				{
					// アニメーション用
					float tw = 1.0f / SPRITE_DIVIDE_X;	// テクスチャの幅
					float th = 1.0f / SPRITE_DIVIDE_Y;	// テクスチャの高さ
					float tx = (float)(g_Player[i].patternAnim) * tw;	// テクスチャの左上X座標
					float ty = 0.0f;	// テクスチャの左上Y座標

					// １枚のポリゴンの頂点とテクスチャ座標を設定
					SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						g_Player[i].rot.z);

					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);
				}
				else
				{
					// アニメーション用
					float tw = -1.0f / SPRITE_DIVIDE_X;	// テクスチャの幅
					float th = 1.0f / SPRITE_DIVIDE_Y;	// テクスチャの高さ
					float tx = (float)(g_Player[i].patternAnim + 3) * tw;	// テクスチャの左上X座標(從反向右邊數回來,所以原本的0123變成3456因而+3)
					float ty = 0.0f;	// テクスチャの左上Y座標

					// １枚のポリゴンの頂点とテクスチャ座標を設定
					SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						g_Player[i].rot.z);

					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);
				}
			}
			else if((g_Player[i].dir == CHAR_DIR_RIGHT) && (g_Player[i].onGround == TRUE))
			{
				if (g_Player[i].patternAnim >= RANIM_PATTERN_NUM)
				{
					g_Player[i].patternAnim = 0;
				}

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

				//プレイヤーの位置やテクスチャー座標を反映
				float px = g_Player[i].pos.x - bg->pos.x;	// プレイヤーの表示位置X
				float py = g_Player[i].pos.y - bg->pos.y;	// プレイヤーの表示位置Y
				float pw = g_Player[i].w;		// プレイヤーの表示幅
				float ph = g_Player[i].h;		// プレイヤーの表示高さ

				//py += g_Player[i].jumpY;		// ジャンプ中の高さを足す

				// アニメーション用
				float tw = 1.0f / SPRITE_DIVIDE_X;	// テクスチャの幅
				float th = 1.0f / SPRITE_DIVIDE_Y;	// テクスチャの高さ
				float tx = (float)(g_Player[i].patternAnim + 1) * tw;	// テクスチャの左上X座標
				float ty = (float)(1) * th;	// テクスチャの左上Y座標

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
					g_Player[i].rot.z);

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
			}
			else if ((g_Player[i].dir == CHAR_DIR_LEFT) && (g_Player[i].onGround == TRUE))
			{
				if (g_Player[i].patternAnim >= RANIM_PATTERN_NUM)
				{
					g_Player[i].patternAnim = 0;
				}
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

				//プレイヤーの位置やテクスチャー座標を反映
				float px = g_Player[i].pos.x - bg->pos.x;	// プレイヤーの表示位置X
				float py = g_Player[i].pos.y - bg->pos.y;	// プレイヤーの表示位置Y
				float pw = g_Player[i].w;		// プレイヤーの表示幅
				float ph = g_Player[i].h;		// プレイヤーの表示高さ

				//py += g_Player[i].jumpY;		// ジャンプ中の高さを足す

				// アニメーション用
				float tw = -1.0f / SPRITE_DIVIDE_X;	// テクスチャの幅
				float th = 1.0f / SPRITE_DIVIDE_Y;	// テクスチャの高さ
				float tx = (float)(g_Player[i].patternAnim) * tw;	// テクスチャの左上X座標
				float ty = (float)(1) * th;	// テクスチャの左上Y座標

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
					g_Player[i].rot.z);

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
			}
			else if (g_Player[i].jump == TRUE)
			{
				if (g_Player[i].patternAnim >= JANIM_PATTERN_NUM)
				{
					g_Player[i].patternAnim = 0;
				}

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

				//プレイヤーの位置やテクスチャー座標を反映
				float px = g_Player[i].pos.x - bg->pos.x;	// プレイヤーの表示位置X
				float py = g_Player[i].pos.y - bg->pos.y;	// プレイヤーの表示位置Y
				float pw = g_Player[i].w;		// プレイヤーの表示幅
				float ph = g_Player[i].h;		// プレイヤーの表示高さ

				//py += g_Player[i].jumpY;		// ジャンプ中の高さを足す

				if (g_Player[i].dir == CHAR_DIR_RIGHT)
				{
					// アニメーション用
					float tw = 1.0f / SPRITE_DIVIDE_X;	// テクスチャの幅
					float th = 1.0f / SPRITE_DIVIDE_Y;	// テクスチャの高さ
					float tx = (float)((g_Player[i].patternAnim) % 7) * tw;	// テクスチャの左上X座標(原始圖片因為有分行,以X座標來看是0~7和最後為0,為了循環用%來讓首張和末張餘數為0)
					float ty = (float)((g_Player[i].patternAnim + 14) / 7) * th;	// テクスチャの左上Y座標

					// draw without using math
					//switch (g_Player[i].patternAnim)
					//{
					//case 0:
					//	tx = 0.0f * tw;
					//	ty = 2.0f * th;
					//	break;

					//case 1:
					//	tx = 1.0f * tw;
					//	ty = 2.0f * th;
					//	break;

					//case 2:
					//	tx = 2.0f * tw;
					//	ty = 2.0f * th;
					//	break;

					//case 3:
					//	tx = 3.0f * tw;
					//	ty = 2.0f * th;
					//	break;

					//case 4:
					//	tx = 4.0f * tw;
					//	ty = 2.0f * th;
					//	break;

					//case 5:
					//	tx = 5.0f * tw;
					//	ty = 2.0f * th;
					//	break;

					//case 6:
					//	tx = 6.0f * tw;
					//	ty = 2.0f * th;
					//	break;

					//case 7:
					//	tx = 0.0f * tw;
					//	ty = 3.0f * th;
					//	break;
					//}

					// １枚のポリゴンの頂点とテクスチャ座標を設定
					SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						g_Player[i].rot.z);

					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);
				}
				else
				{
					// アニメーション用
					float tw = -1.0f / SPRITE_DIVIDE_X;	// テクスチャの幅
					float th = 1.0f / SPRITE_DIVIDE_Y;	// テクスチャの高さ
					float tx = (float)(6 - (g_Player[i].patternAnim % 7)) * tw;		// テクスチャの左上X座標
					float ty = (float)((g_Player[i].patternAnim + 14) / 7) * th;	// テクスチャの左上Y座標

					// １枚のポリゴンの頂点とテクスチャ座標を設定
					SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						g_Player[i].rot.z);

					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);
				}
			}

			// attack
			if (g_Player[i].attack == TRUE)
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

				//プレイヤーの位置やテクスチャー座標を反映
				float px = g_Player[i].pos.x - bg->pos.x;	// プレイヤーの表示位置X
				float py = g_Player[i].pos.y - bg->pos.y;	// プレイヤーの表示位置Y
				float pw = g_Player[i].w;		// プレイヤーの表示幅
				float ph = g_Player[i].h;		// プレイヤーの表示高さ


				if (g_Player[i].dir == CHAR_DIR_RIGHT)
				{
					// アニメーション用
					float tw = 1.0f / ATEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
					float th = 1.0f / ATEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
					float tx = (float)(5 + g_Player[i].patternAnim % 7) * tw;		// テクスチャの左上X座標
					float ty = (float)((g_Player[i].patternAnim + 14) / 7) * th;	// テクスチャの左上Y座標
				}

			}

			// draw item message box message
			{
				if (g_Player[i].aenough == TRUE)
				{
					// テクスチャ設定
					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[8]);

					//プレイヤーの位置やテクスチャー座標を反映
					float px = SCREEN_CENTER_X;		// message box messageの表示位置X
					float py = SCREEN_CENTER_Y;		// message box messageの表示位置Y
					float pw = MESSAGE_BOX_WIDTH;	// message box messageの表示幅
					float ph = MESSAGE_BOX_WIDTH;	// message box messageの表示高さ

					float tx = 0.0f;
					float ty = 0.0f;
					float tw = 1.0f;
					float th = 1.0f;

					// texture coordinate sitting
					SetSpriteLeftTop(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);

				}
				else if (g_Player[i].penough == TRUE)
				{
					// テクスチャ設定
					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[9]);

					//プレイヤーの位置やテクスチャー座標を反映
					float px = SCREEN_CENTER_X;		// message box messageの表示位置X
					float py = SCREEN_CENTER_Y;		// message box messageの表示位置Y
					float pw = MESSAGE_BOX_WIDTH;	// message box messageの表示幅
					float ph = MESSAGE_BOX_WIDTH;	// message box messageの表示高さ

					float tx = 0.0f;
					float ty = 0.0f;
					float tw = 1.0f;
					float th = 1.0f;

					// texture coordinate sitting
					SetSpriteLeftTop(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);
				}
			}
			// gauge
			{
				// gauge texture
				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[10]);

				float px = 630.0f;			// geuge textureの表示位置X
				float py = 0.0f;			// gauge textureの表示位置Y
				float pw = GAUGE_WIDTH;		// ゲージの表示幅
				float ph = GAUGE_HEIGHT;	// ゲージの表示高さ

				float tw = 1.0f;			// テクスチャの幅
				float th = 1.0f;			// テクスチャの高さ
				float tx = 0.0f;			// テクスチャの左上X座標
				float ty = 0.0f;			// テクスチャの左上Y座標

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteLeftTop(g_VertexBuffer, px, py, GAUGE_WIDTH, GAUGE_HEIGHT, tx, ty, tw, th);

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);


				// HP gauge
				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);

				float px1 = 647.0f;			// HP gauseの表示位置X
				float py1 = 114.0f;			// HP gaugeの表示位置Y
				float pw1 = g_Player[0].hp;	// ゲージの表示幅
				float ph1 = 22.0f;			// ゲージの表示高さ

				float tw1 = 1.0f;			// テクスチャの幅
				float th1 = 1.0f;			// テクスチャの高さ
				float tx1 = 0.0f;			// テクスチャの左上X座標
				float ty1 = 0.0f;			// テクスチャの左上Y座標

				pw1 = (float)(g_Player[i].hp / 300.0f) * 500.0f ;

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteLTColor(g_VertexBuffer,
					px1, py1, pw1, ph1,
					tx1, ty1, tw1, th1,
					XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
			}
		}


#ifdef _DEBUG	// デバッグ情報を表示する
			//テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[11]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			if (g_Player[i].use == TRUE)
			{
				SetSpriteColor(g_VertexBuffer,
					g_Player[i].cPos.x - bg->pos.x, g_Player[i].cPos.y - bg->pos.y, g_Player[i].cw, g_Player[i].ch,
					0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f));

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
			}

#endif
		
	}
}


//=============================================================================
// Player構造体の先頭アドレスを取得
//=============================================================================
PLAYER* GetPlayer(void)
{
	return &g_Player[0];
}


//=============================================================================
// プレイヤーの分身を描画
//=============================================================================
void DrawPlayerOffset(int no)
{
	BG* bg = GetBG();
	float alpha = 0.0f;

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[no].texNo]);

	for (int j = PLAYER_OFFSET_CNT - 1; j >= 0; j--)
	{
		//プレイヤーの位置やテクスチャー座標を反映
		float px = g_Player[no].offset[j].x - bg->pos.x;	// プレイヤーの表示位置X
		float py = g_Player[no].offset[j].y - bg->pos.y;	// プレイヤーの表示位置Y
		float pw = g_Player[no].w;		// プレイヤーの表示幅
		float ph = g_Player[no].h;		// プレイヤーの表示高さ

		// アニメーション用
		float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
		float tx = (float)(g_Player[no].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
		float ty = (float)(g_Player[no].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標


		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, alpha),
			g_Player[no].rot.z);

		alpha += (1.0f / PLAYER_OFFSET_CNT);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
}



