//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author : GP11B132 05 ���E��
//
//=============================================================================
#include "enemy.h"
#include "bg.h"
#include "player.h"
#include "fade.h"
#include "collision.h"
#include "bullet.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

// enemy
#define TEXTURE_WIDTH				(300)	// �G�l�~�[�T�C�Y
#define TEXTURE_HEIGHT				(300)	// 
#define TEXTURE_MAX					(10)	// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(4)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(4) 	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(8)		// �A�j���[�V�����̐؂�ւ��Wait�l



// enemy (fly)
#define FTEXTURE_WIDTH				(350)	// �G�l�~�[�T�C�Y
#define FTEXTURE_HEIGHT				(350)	// 

#define FTEXTURE_PATTERN_DIVIDE_X	(4)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define FTEXTURE_PATTERN_DIVIDE_Y	(2)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define FANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define FANIM_WAIT					(4)		// �A�j���[�V�����̐؂�ւ��Wait�l




//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/ENEMY/enemyatk.png",
	"data/ENEMY/enemyflyatk.png",


	"data/TEXTURE/bar_white.png",
};


static BOOL		g_Load = FALSE;					// ���������s�������̃t���O
static ENEMY	g_Enemy[ENEMY_MAX];				// �G�l�~�[�\����
static ENEMY	g_fEnemy[ENEMYFLY_MAX];			// �G�l�~�[�\����


static int		g_EnemyCnt  = ENEMY_MAX;
static int		g_fEnemyCnt = ENEMYFLY_MAX;



static INTERPOLATION_DATA g_MoveTbl0[] = {
	//���W									��]��							�g�嗦				  ����
	{ XMFLOAT3(250.0f, 250.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(10.0f, 10.0f, 10.0f),60 },
	//{ XMFLOAT3(250.0f, 250.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(10.0f, 10.0f, 10.0f),60 },
	{ XMFLOAT3(250.0f, 250.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(20.0f, 20.0f, 20.0f),60 },

	//{ XMFLOAT3(50.0f,  50.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	//{ XMFLOAT3(250.0f,  50.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 3.14f),	XMFLOAT3(0.0f, 0.0f, 1.0f),	60 },
	//{ XMFLOAT3(250.0f, 250.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 6.28f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	60 },
};


static INTERPOLATION_DATA g_MoveTbl1[] = {
	//���W									��]��							�g�嗦				����
	{ XMFLOAT3(1700.0f,  0.0f, 0.0f),XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(1700.0f,  0.0f, 0.0f),XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	60 },

	//{ XMFLOAT3(1700.0f,   0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),			XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	//{ XMFLOAT3(1700.0f,  SCREEN_HEIGHT, 0.0f),XMFLOAT3(0.0f, 0.0f, 6.28f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	60 },
};


static INTERPOLATION_DATA g_MoveTbl2[] = {
	//���W									��]��							�g�嗦							����
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
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
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


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �G�l�~�[�\���̂̏�����
	g_EnemyCnt = 0;
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		// ground enemy
		g_Enemy[i].use = FALSE;
		g_Enemy[i].pos = XMFLOAT3(100.0f, 100.0f, 0.0f);	// ���S�_����\��
		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_Enemy[i].w = TEXTURE_WIDTH;
		g_Enemy[i].h = TEXTURE_HEIGHT;
		g_Enemy[i].texNo = 0;

		g_Enemy[i].countAnim = 0;
		g_Enemy[i].patternAnim = 0;

		g_Enemy[i].move = XMFLOAT3(4.0f, 0.0f, 0.0f);		// �ړ���

		g_Enemy[i].time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
		g_Enemy[i].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Enemy[i].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

		g_EnemyCnt++;

		// fly enemy
		g_fEnemy[i].use = FALSE;
		g_fEnemy[i].pos = XMFLOAT3(100.0f, 100.0f, 0.0f);	// ���S�_����\��
		g_fEnemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_fEnemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_fEnemy[i].w = TEXTURE_WIDTH;
		g_fEnemy[i].h = TEXTURE_HEIGHT;
		g_fEnemy[i].texNo = 0;

		g_fEnemy[i].countAnim = 0;
		g_fEnemy[i].patternAnim = 0;

		g_fEnemy[i].move = XMFLOAT3(4.0f, 0.0f, 0.0f);		// �ړ���

		g_fEnemy[i].time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
		g_fEnemy[i].tblNo = 0;				// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_fEnemy[i].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

		g_fEnemyCnt++;

	}

	// various enemy 
	switch (GetMode())
	{
	case MODE_GAME:
		g_Enemy[0].use = TRUE;
		g_Enemy[0].pos = XMFLOAT3(1950.0f, 1222.0f, 0.0f);	// ���S�_����\��

		g_Enemy[1].use = TRUE;
		g_Enemy[1].pos = XMFLOAT3(550.0f, 1120.0f, 0.0f);	// ���S�_����\��

		g_Enemy[2].use = TRUE;
		g_Enemy[2].pos = XMFLOAT3(1700.0f, 1920.0f, 0.0f);	// ���S�_����\��

		g_Enemy[3].use = TRUE;
		g_Enemy[3].pos = XMFLOAT3(1800.0f, 475.0f, 0.0f);	// ���S�_����\��

		//// 0�Ԃ������`��Ԃœ������Ă݂�
		//g_Enemy[0].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
		//g_Enemy[0].tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		//g_Enemy[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
		//// 1�Ԃ������`��Ԃœ������Ă݂�
		//g_Enemy[1].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
		//g_Enemy[1].tblNo = 1;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		//g_Enemy[1].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		//// 2�Ԃ������`��Ԃœ������Ă݂�
		//g_Enemy[2].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
		//g_Enemy[2].tblNo = 2;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		//g_Enemy[2].tblMax = sizeof(g_MoveTbl2) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		// sky enemy
		g_fEnemy[0].use = TRUE;
		g_fEnemy[0].pos = XMFLOAT3(2900.0f, 500.0f, 0.0f);	// ���S�_����\��

		//g_fEnemy[1].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
		//g_fEnemy[1].tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		//g_fEnemy[1].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
		break;

	case MODE_GAME_MAP2:
		// ground enemy
		g_Enemy[0].use = TRUE;
		g_Enemy[0].pos = XMFLOAT3(320.0f, 745.0f, 0.0f);	// ���S�_����\��

		g_Enemy[1].use = TRUE;
		g_Enemy[1].pos = XMFLOAT3(1525.0f, 1130.0f, 0.0f);	// ���S�_����\��

		g_Enemy[2].use = TRUE;
		g_Enemy[2].pos = XMFLOAT3(1060.0f, 2150.0f, 0.0f);	// ���S�_����\��

		g_Enemy[3].use = TRUE;
		g_Enemy[3].pos = XMFLOAT3(2700.0f, 1670.0f, 0.0f);	// ���S�_����\��

		g_Enemy[4].use = TRUE;
		g_Enemy[4].pos = XMFLOAT3(5300.0f, 1980.0f, 0.0f);	// ���S�_����\��

		g_Enemy[5].use = TRUE;
		g_Enemy[5].pos = XMFLOAT3(6200.0f, 1435.0f, 0.0f);	// ���S�_����\��
		
		g_Enemy[6].use = TRUE;
		g_Enemy[6].pos = XMFLOAT3(5880.0f, 870.0f, 0.0f);	// ���S�_����\��

		//// 0�Ԃ������`��Ԃœ������Ă݂�
		//g_Enemy[4].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
		//g_Enemy[4].tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		//g_Enemy[4].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		//// 1�Ԃ������`��Ԃœ������Ă݂�
		//g_Enemy[5].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
		//g_Enemy[5].tblNo = 1;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		//g_Enemy[5].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		//// 2�Ԃ������`��Ԃœ������Ă݂�
		//g_Enemy[6].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
		//g_Enemy[6].tblNo = 2;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		//g_Enemy[6].tblMax = sizeof(g_MoveTbl2) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		// sky enemy
		//g_fEnemy[0].use = TRUE;
		//g_fEnemy[0].pos = XMFLOAT3(1500.0f, 1000.0f, 0.0f);	// ���S�_����\��

		//g_fEnemy[1].use = TRUE;
		//g_fEnemy[1].pos = XMFLOAT3(1500.0f, 1000.0f, 0.0f);	// ���S�_����\��

		//g_fEnemy[0].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
		//g_fEnemy[0].tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		//g_fEnemy[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		//g_fEnemy[1].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
		//g_fEnemy[1].tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		//g_fEnemy[1].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
		break;

	case MODE_GAME_MAP3:
		// ground enemy
		g_Enemy[0].use = TRUE;
		g_Enemy[0].pos = XMFLOAT3(330.0f, 1385.0f, 0.0f);	// ���S�_����\��

		g_Enemy[1].use = TRUE;
		g_Enemy[1].pos = XMFLOAT3(1680.0f, 1630.0f, 0.0f);	// ���S�_����\��

		g_Enemy[2].use = TRUE;
		g_Enemy[2].pos = XMFLOAT3(2950.0f, 930.0f, 0.0f);	// ���S�_����\��

		g_Enemy[3].use = TRUE;
		g_Enemy[3].pos = XMFLOAT3(3870.0f, 2145.0f, 0.0f);	// ���S�_����\��

		g_Enemy[4].use = TRUE;
		g_Enemy[4].pos = XMFLOAT3(4890.0f, 1400.0f, 0.0f);	// ���S�_����\��

		g_Enemy[5].use = TRUE;
		g_Enemy[5].pos = XMFLOAT3(7100.0f, 2145.0f, 0.0f);	// ���S�_����\��

		g_Enemy[6].use = TRUE;
		g_Enemy[6].pos = XMFLOAT3(7370.0f, 1680.0f, 0.0f);	// ���S�_����\��

		// fly enemy
		//g_fEnemy[4].use = TRUE;
		//g_fEnemy[4].pos = XMFLOAT3(1500.0f, 1000.0f, 0.0f);	// ���S�_����\��

		//g_fEnemy[5].use = TRUE;
		//g_fEnemy[5].pos = XMFLOAT3(1500.0f, 1000.0f, 0.0f);	// ���S�_����\��
		break;
	}

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	g_EnemyCnt = 0;		// �����Ă�G�l�~�[�̐�
	g_fEnemyCnt = 0;	// �����Ă�G�l�~�[�̐�

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		// �����Ă�G�l�~�[��������������
		if (g_Enemy[i].use == TRUE)
		{
			// �n�`�Ƃ̓����蔻��p�ɍ��W�̃o�b�N�A�b�v������Ă���
			XMFLOAT3 pos_old = g_Enemy[i].pos;

			// �A�j���[�V����  
			g_Enemy[i].countAnim += 1.0f;
			if (g_Enemy[i].countAnim > ANIM_WAIT)
			{
				g_Enemy[i].countAnim = 0.0f;
				// �p�^�[���̐؂�ւ�
				g_Enemy[i].patternAnim = (g_Enemy[i].patternAnim + 1) % ANIM_PATTERN_NUM;
			}

			g_fEnemy[i].countAnim += 1.0f;
			if (g_fEnemy[i].countAnim > ANIM_WAIT)
			{
				g_fEnemy[i].countAnim = 0.0f;
				// �p�^�[���̐؂�ւ�
				g_fEnemy[i].patternAnim = (g_fEnemy[i].patternAnim + 1) % ANIM_PATTERN_NUM;
			}

			// �ړ�����
			if (g_Enemy[i].tblMax > 0)	// ���`��Ԃ����s����H
			{	// ���`��Ԃ̏���
				int nowNo = (int)g_Enemy[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
				int maxNo = g_Enemy[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
				int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
				INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Enemy[i].tblNo];	// �s���e�[�u���̃A�h���X���擾
				
				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�
				
				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���
				
				float nowTime = g_Enemy[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���
				
				Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
				Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
				Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

				// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
				XMStoreFloat3(&g_Enemy[i].pos, nowPos + Pos);

				// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&g_Enemy[i].rot, nowRot + Rot);

				// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&g_Enemy[i].scl, nowScl + Scl);
				g_Enemy[i].w = TEXTURE_WIDTH * g_Enemy[i].scl.x;
				g_Enemy[i].h = TEXTURE_HEIGHT * g_Enemy[i].scl.y;

				// frame���g�Ď��Ԍo�ߏ���������
				g_Enemy[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
				if ((int)g_Enemy[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
				{
					g_Enemy[i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
				}
			}
			g_EnemyCnt++;		// �����Ă��G�̐�
		}
		if (g_Enemy[i].use == TRUE)
		{
			// �A�j���[�V����  
			g_Enemy[i].countAnim += 1.0f;
			if (g_Enemy[i].countAnim > ANIM_WAIT)
			{
				g_Enemy[i].countAnim = 0.0f;
				// �p�^�[���̐؂�ւ�
				g_Enemy[i].patternAnim = (g_Enemy[i].patternAnim + 1) % ANIM_PATTERN_NUM;
			}
		}
		if (g_fEnemy[i].use == TRUE) 
		{
			// �A�j���[�V����  
			g_fEnemy[i].countAnim += 1.0f;
			if (g_fEnemy[i].countAnim > ANIM_WAIT)
			{
				g_fEnemy[i].countAnim = 0.0f;
				// �p�^�[���̐؂�ւ�
				g_fEnemy[i].patternAnim = (g_fEnemy[i].patternAnim + 1) % ANIM_PATTERN_NUM;
			}
		}
	}


	// �G�l�~�[�S�Ń`�F�b�N
	//if (g_EnemyCnt <= 0)
	//{
	//	SetFade(FADE_OUT, MODE_RESULT);
	//}

#ifdef _DEBUG	// �f�o�b�O����\������


#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	BG* bg = GetBG();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].use == TRUE)			// ���̃G�l�~�[���g���Ă���H
		{									
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Enemy[i].pos.x - bg->pos.x;	// �G�l�~�[�̕\���ʒuX
			float py = g_Enemy[i].pos.y - bg->pos.y;	// �G�l�~�[�̕\���ʒuY
			float pw = g_Enemy[i].w;					// �G�l�~�[�̕\����
			float ph = g_Enemy[i].h;					// �G�l�~�[�̕\������

			// �A�j���[�V�����p
			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
			float tx = (float)(g_Enemy[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
			float ty = (float)(g_Enemy[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				g_Enemy[i].rot.z);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

		// enemy (fly)
		if (g_fEnemy[i].use == TRUE)			// ���̃G�l�~�[���g���Ă���H
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

			//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_fEnemy[i].pos.x - bg->fpos.x;	// �G�l�~�[�̕\���ʒuX
			float py = g_fEnemy[i].pos.y - bg->fpos.y;	// �G�l�~�[�̕\���ʒuY
			float pw = g_fEnemy[i].w;					// �G�l�~�[�̕\����
			float ph = g_fEnemy[i].h;					// �G�l�~�[�̕\������

			// �A�j���[�V�����p
			float tw = 1.0f / FTEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
			float th = 1.0f / FTEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
			float tx = (float)(g_fEnemy[i].patternAnim % FTEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
			float ty = (float)(g_fEnemy[i].patternAnim / FTEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				g_fEnemy[i].rot.z);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}


//=============================================================================
// Enemy�\���̂̐擪�A�h���X���擾
//=============================================================================
ENEMY* GetEnemy(void)
{
	return &g_Enemy[0];
}





