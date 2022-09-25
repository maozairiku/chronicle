//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author : GP11B132 05 王ウ華
//
//=============================================================================
#include "enemy.h"
#include "bg.h"
#include "player.h"
#include "fade.h"
#include "collision.h"
#include "bullet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

// enemy
#define TEXTURE_WIDTH				(300)	// エネミーサイズ
#define TEXTURE_HEIGHT				(300)	// 
#define TEXTURE_MAX					(10)	// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(4)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(4) 	// アニメーションパターン数
#define ANIM_WAIT					(8)		// アニメーションの切り替わるWait値



// enemy (fly)
#define FTEXTURE_WIDTH				(350)	// エネミーサイズ
#define FTEXTURE_HEIGHT				(350)	// 

#define FTEXTURE_PATTERN_DIVIDE_X	(4)		// アニメパターンのテクスチャ内分割数（X)
#define FTEXTURE_PATTERN_DIVIDE_Y	(2)		// アニメパターンのテクスチャ内分割数（Y)
#define FANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define FANIM_WAIT					(4)		// アニメーションの切り替わるWait値




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/ENEMY/enemyatk.png",
	"data/ENEMY/enemyflyatk.png",


	"data/TEXTURE/bar_white.png",
};


static BOOL		g_Load = FALSE;					// 初期化を行ったかのフラグ
static ENEMY	g_Enemy[ENEMY_MAX];				// エネミー構造体
static ENEMY	g_fEnemy[ENEMYFLY_MAX];			// エネミー構造体


static int		g_EnemyCnt  = ENEMY_MAX;
static int		g_fEnemyCnt = ENEMYFLY_MAX;



static INTERPOLATION_DATA g_MoveTbl0[] = {
	//座標									回転率							拡大率				  時間
	{ XMFLOAT3(250.0f, 250.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(10.0f, 10.0f, 10.0f),60 },
	//{ XMFLOAT3(250.0f, 250.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(10.0f, 10.0f, 10.0f),60 },
	{ XMFLOAT3(250.0f, 250.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(20.0f, 20.0f, 20.0f),60 },

	//{ XMFLOAT3(50.0f,  50.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	//{ XMFLOAT3(250.0f,  50.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 3.14f),	XMFLOAT3(0.0f, 0.0f, 1.0f),	60 },
	//{ XMFLOAT3(250.0f, 250.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 6.28f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	60 },
};


static INTERPOLATION_DATA g_MoveTbl1[] = {
	//座標									回転率							拡大率				時間
	{ XMFLOAT3(1700.0f,  0.0f, 0.0f),XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(1700.0f,  0.0f, 0.0f),XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	60 },

	//{ XMFLOAT3(1700.0f,   0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),			XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	//{ XMFLOAT3(1700.0f,  SCREEN_HEIGHT, 0.0f),XMFLOAT3(0.0f, 0.0f, 6.28f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	60 },
};


static INTERPOLATION_DATA g_MoveTbl2[] = {
	//座標									回転率							拡大率							時間
	{ XMFLOAT3(3000.0f, 100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(3000 + SCREEN_WIDTH, 100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },

	//{ XMFLOAT3(3000.0f, 100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	//{ XMFLOAT3(3000 + SCREEN_WIDTH, 100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 6.28f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
};


static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	g_MoveTbl0,
	g_MoveTbl1,
	g_MoveTbl2,

};


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
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


	// エネミー構造体の初期化
	g_EnemyCnt = 0;
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		// ground enemy
		g_Enemy[i].use = FALSE;
		g_Enemy[i].pos = XMFLOAT3(100.0f, 100.0f, 0.0f);	// 中心点から表示
		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_Enemy[i].w = TEXTURE_WIDTH;
		g_Enemy[i].h = TEXTURE_HEIGHT;
		g_Enemy[i].texNo = 0;

		g_Enemy[i].countAnim = 0;
		g_Enemy[i].patternAnim = 0;

		g_Enemy[i].move = XMFLOAT3(4.0f, 0.0f, 0.0f);		// 移動量

		g_Enemy[i].time = 0.0f;			// 線形補間用のタイマーをクリア
		g_Enemy[i].tblNo = 0;			// 再生する行動データテーブルNoをセット
		g_Enemy[i].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット

		g_EnemyCnt++;

		// fly enemy
		g_fEnemy[i].use = FALSE;
		g_fEnemy[i].pos = XMFLOAT3(100.0f, 100.0f, 0.0f);	// 中心点から表示
		g_fEnemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_fEnemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_fEnemy[i].w = TEXTURE_WIDTH;
		g_fEnemy[i].h = TEXTURE_HEIGHT;
		g_fEnemy[i].texNo = 0;

		g_fEnemy[i].countAnim = 0;
		g_fEnemy[i].patternAnim = 0;

		g_fEnemy[i].move = XMFLOAT3(4.0f, 0.0f, 0.0f);		// 移動量

		g_fEnemy[i].time = 0.0f;			// 線形補間用のタイマーをクリア
		g_fEnemy[i].tblNo = 0;				// 再生する行動データテーブルNoをセット
		g_fEnemy[i].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット

		g_fEnemyCnt++;

	}

	// various enemy 
	switch (GetMode())
	{
	case MODE_GAME:
		g_Enemy[0].use = TRUE;
		g_Enemy[0].pos = XMFLOAT3(1950.0f, 1222.0f, 0.0f);	// 中心点から表示

		g_Enemy[1].use = TRUE;
		g_Enemy[1].pos = XMFLOAT3(550.0f, 1120.0f, 0.0f);	// 中心点から表示

		g_Enemy[2].use = TRUE;
		g_Enemy[2].pos = XMFLOAT3(1700.0f, 1920.0f, 0.0f);	// 中心点から表示

		g_Enemy[3].use = TRUE;
		g_Enemy[3].pos = XMFLOAT3(1800.0f, 475.0f, 0.0f);	// 中心点から表示

		//// 0番だけ線形補間で動かしてみる
		//g_Enemy[0].time = 0.0f;		// 線形補間用のタイマーをクリア
		//g_Enemy[0].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
		//g_Enemy[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
		//// 1番だけ線形補間で動かしてみる
		//g_Enemy[1].time = 0.0f;		// 線形補間用のタイマーをクリア
		//g_Enemy[1].tblNo = 1;		// 再生するアニメデータの先頭アドレスをセット
		//g_Enemy[1].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		//// 2番だけ線形補間で動かしてみる
		//g_Enemy[2].time = 0.0f;		// 線形補間用のタイマーをクリア
		//g_Enemy[2].tblNo = 2;		// 再生するアニメデータの先頭アドレスをセット
		//g_Enemy[2].tblMax = sizeof(g_MoveTbl2) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		// sky enemy
		g_fEnemy[0].use = TRUE;
		g_fEnemy[0].pos = XMFLOAT3(2900.0f, 500.0f, 0.0f);	// 中心点から表示

		//g_fEnemy[1].time = 0.0f;		// 線形補間用のタイマーをクリア
		//g_fEnemy[1].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
		//g_fEnemy[1].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
		break;

	case MODE_GAME_MAP2:
		// ground enemy
		g_Enemy[0].use = TRUE;
		g_Enemy[0].pos = XMFLOAT3(320.0f, 745.0f, 0.0f);	// 中心点から表示

		g_Enemy[1].use = TRUE;
		g_Enemy[1].pos = XMFLOAT3(1525.0f, 1130.0f, 0.0f);	// 中心点から表示

		g_Enemy[2].use = TRUE;
		g_Enemy[2].pos = XMFLOAT3(1060.0f, 2150.0f, 0.0f);	// 中心点から表示

		g_Enemy[3].use = TRUE;
		g_Enemy[3].pos = XMFLOAT3(2700.0f, 1670.0f, 0.0f);	// 中心点から表示

		g_Enemy[4].use = TRUE;
		g_Enemy[4].pos = XMFLOAT3(5300.0f, 1980.0f, 0.0f);	// 中心点から表示

		g_Enemy[5].use = TRUE;
		g_Enemy[5].pos = XMFLOAT3(6200.0f, 1435.0f, 0.0f);	// 中心点から表示
		
		g_Enemy[6].use = TRUE;
		g_Enemy[6].pos = XMFLOAT3(5880.0f, 870.0f, 0.0f);	// 中心点から表示

		//// 0番だけ線形補間で動かしてみる
		//g_Enemy[4].time = 0.0f;		// 線形補間用のタイマーをクリア
		//g_Enemy[4].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
		//g_Enemy[4].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		//// 1番だけ線形補間で動かしてみる
		//g_Enemy[5].time = 0.0f;		// 線形補間用のタイマーをクリア
		//g_Enemy[5].tblNo = 1;		// 再生するアニメデータの先頭アドレスをセット
		//g_Enemy[5].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		//// 2番だけ線形補間で動かしてみる
		//g_Enemy[6].time = 0.0f;		// 線形補間用のタイマーをクリア
		//g_Enemy[6].tblNo = 2;		// 再生するアニメデータの先頭アドレスをセット
		//g_Enemy[6].tblMax = sizeof(g_MoveTbl2) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		// sky enemy
		//g_fEnemy[0].use = TRUE;
		//g_fEnemy[0].pos = XMFLOAT3(1500.0f, 1000.0f, 0.0f);	// 中心点から表示

		//g_fEnemy[1].use = TRUE;
		//g_fEnemy[1].pos = XMFLOAT3(1500.0f, 1000.0f, 0.0f);	// 中心点から表示

		//g_fEnemy[0].time = 0.0f;		// 線形補間用のタイマーをクリア
		//g_fEnemy[0].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
		//g_fEnemy[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		//g_fEnemy[1].time = 0.0f;		// 線形補間用のタイマーをクリア
		//g_fEnemy[1].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
		//g_fEnemy[1].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
		break;

	case MODE_GAME_MAP3:
		// ground enemy
		g_Enemy[0].use = TRUE;
		g_Enemy[0].pos = XMFLOAT3(330.0f, 1385.0f, 0.0f);	// 中心点から表示

		g_Enemy[1].use = TRUE;
		g_Enemy[1].pos = XMFLOAT3(1680.0f, 1630.0f, 0.0f);	// 中心点から表示

		g_Enemy[2].use = TRUE;
		g_Enemy[2].pos = XMFLOAT3(2950.0f, 930.0f, 0.0f);	// 中心点から表示

		g_Enemy[3].use = TRUE;
		g_Enemy[3].pos = XMFLOAT3(3870.0f, 2145.0f, 0.0f);	// 中心点から表示

		g_Enemy[4].use = TRUE;
		g_Enemy[4].pos = XMFLOAT3(4890.0f, 1400.0f, 0.0f);	// 中心点から表示

		g_Enemy[5].use = TRUE;
		g_Enemy[5].pos = XMFLOAT3(7100.0f, 2145.0f, 0.0f);	// 中心点から表示

		g_Enemy[6].use = TRUE;
		g_Enemy[6].pos = XMFLOAT3(7370.0f, 1680.0f, 0.0f);	// 中心点から表示

		// fly enemy
		//g_fEnemy[4].use = TRUE;
		//g_fEnemy[4].pos = XMFLOAT3(1500.0f, 1000.0f, 0.0f);	// 中心点から表示

		//g_fEnemy[5].use = TRUE;
		//g_fEnemy[5].pos = XMFLOAT3(1500.0f, 1000.0f, 0.0f);	// 中心点から表示
		break;
	}

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
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
void UpdateEnemy(void)
{
	g_EnemyCnt = 0;		// 生きてるエネミーの数
	g_fEnemyCnt = 0;	// 生きてるエネミーの数

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		// 生きてるエネミーだけ処理をする
		if (g_Enemy[i].use == TRUE)
		{
			// 地形との当たり判定用に座標のバックアップを取っておく
			XMFLOAT3 pos_old = g_Enemy[i].pos;

			// アニメーション  
			g_Enemy[i].countAnim += 1.0f;
			if (g_Enemy[i].countAnim > ANIM_WAIT)
			{
				g_Enemy[i].countAnim = 0.0f;
				// パターンの切り替え
				g_Enemy[i].patternAnim = (g_Enemy[i].patternAnim + 1) % ANIM_PATTERN_NUM;
			}

			g_fEnemy[i].countAnim += 1.0f;
			if (g_fEnemy[i].countAnim > ANIM_WAIT)
			{
				g_fEnemy[i].countAnim = 0.0f;
				// パターンの切り替え
				g_fEnemy[i].patternAnim = (g_fEnemy[i].patternAnim + 1) % ANIM_PATTERN_NUM;
			}

			// 移動処理
			if (g_Enemy[i].tblMax > 0)	// 線形補間を実行する？
			{	// 線形補間の処理
				int nowNo = (int)g_Enemy[i].time;			// 整数分であるテーブル番号を取り出している
				int maxNo = g_Enemy[i].tblMax;				// 登録テーブル数を数えている
				int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
				INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Enemy[i].tblNo];	// 行動テーブルのアドレスを取得
				
				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換
				
				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している
				
				float nowTime = g_Enemy[i].time - nowNo;	// 時間部分である少数を取り出している
				
				Pos *= nowTime;								// 現在の移動量を計算している
				Rot *= nowTime;								// 現在の回転量を計算している
				Scl *= nowTime;								// 現在の拡大率を計算している

				// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
				XMStoreFloat3(&g_Enemy[i].pos, nowPos + Pos);

				// 計算して求めた回転量を現在の移動テーブルに足している
				XMStoreFloat3(&g_Enemy[i].rot, nowRot + Rot);

				// 計算して求めた拡大率を現在の移動テーブルに足している
				XMStoreFloat3(&g_Enemy[i].scl, nowScl + Scl);
				g_Enemy[i].w = TEXTURE_WIDTH * g_Enemy[i].scl.x;
				g_Enemy[i].h = TEXTURE_HEIGHT * g_Enemy[i].scl.y;

				// frameを使て時間経過処理をする
				g_Enemy[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
				if ((int)g_Enemy[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
				{
					g_Enemy[i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
				}
			}
			g_EnemyCnt++;		// 生きてた敵の数
		}
		if (g_Enemy[i].use == TRUE)
		{
			// アニメーション  
			g_Enemy[i].countAnim += 1.0f;
			if (g_Enemy[i].countAnim > ANIM_WAIT)
			{
				g_Enemy[i].countAnim = 0.0f;
				// パターンの切り替え
				g_Enemy[i].patternAnim = (g_Enemy[i].patternAnim + 1) % ANIM_PATTERN_NUM;
			}
		}
		if (g_fEnemy[i].use == TRUE) 
		{
			// アニメーション  
			g_fEnemy[i].countAnim += 1.0f;
			if (g_fEnemy[i].countAnim > ANIM_WAIT)
			{
				g_fEnemy[i].countAnim = 0.0f;
				// パターンの切り替え
				g_fEnemy[i].patternAnim = (g_fEnemy[i].patternAnim + 1) % ANIM_PATTERN_NUM;
			}
		}
	}


	// エネミー全滅チェック
	//if (g_EnemyCnt <= 0)
	//{
	//	SetFade(FADE_OUT, MODE_RESULT);
	//}

#ifdef _DEBUG	// デバッグ情報を表示する


#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
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

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].use == TRUE)			// このエネミーが使われている？
		{									
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			//エネミーの位置やテクスチャー座標を反映
			float px = g_Enemy[i].pos.x - bg->pos.x;	// エネミーの表示位置X
			float py = g_Enemy[i].pos.y - bg->pos.y;	// エネミーの表示位置Y
			float pw = g_Enemy[i].w;					// エネミーの表示幅
			float ph = g_Enemy[i].h;					// エネミーの表示高さ

			// アニメーション用
			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
			float tx = (float)(g_Enemy[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
			float ty = (float)(g_Enemy[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				g_Enemy[i].rot.z);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

		// enemy (fly)
		if (g_fEnemy[i].use == TRUE)			// このエネミーが使われている？
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

			//エネミーの位置やテクスチャー座標を反映
			float px = g_fEnemy[i].pos.x - bg->fpos.x;	// エネミーの表示位置X
			float py = g_fEnemy[i].pos.y - bg->fpos.y;	// エネミーの表示位置Y
			float pw = g_fEnemy[i].w;					// エネミーの表示幅
			float ph = g_fEnemy[i].h;					// エネミーの表示高さ

			// アニメーション用
			float tw = 1.0f / FTEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
			float th = 1.0f / FTEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
			float tx = (float)(g_fEnemy[i].patternAnim % FTEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
			float ty = (float)(g_fEnemy[i].patternAnim / FTEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				g_fEnemy[i].rot.z);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
}


//=============================================================================
// Enemy構造体の先頭アドレスを取得
//=============================================================================
ENEMY* GetEnemy(void)
{
	return &g_Enemy[0];
}





