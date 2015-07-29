#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"UI.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"PlayerManager.h"

//****************************************************************************************
//
//	HeadUpDisplayクラス
//
//****************************************************************************************

//------------------------------------------------------------------------------
//	グローバル
//------------------------------------------------------------------------------

HeadUpDisplay*	m_UI = NULL;

//------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------
HeadUpDisplay::HeadUpDisplay()
{

}

HeadUpDisplay::~HeadUpDisplay()
{
	SafeDelete(coinbar);
}

bool HeadUpDisplay::Initialize()
{
	coinbar = new CoinBar();
	return true;
}

//------------------------------------------------------------------------------
//	更新・描画
//------------------------------------------------------------------------------
void HeadUpDisplay::Update()
{
	coinbar->Update();
}

void HeadUpDisplay::Render()
{
	coinbar->Render();
}


//****************************************************************************************
//
//	CoinBarクラス
//
//****************************************************************************************
//------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------
CoinBar::CoinBar()
{
	img = new iex2DObj("DATA\\BG\\coin_gage.png");
	frame_x = (1280 / 2) - (512 / 2);
	frame_y = 600;
	frame_sx = 512;
	frame_sy = 64;
	for (int i = 0; i < NUM_BAR; i++)
	{
		bar_x[i] = frame_x + 16;
		bar_y[i] = frame_y + 16;
		bar_sx[i] = 480;
		bar_sy[i] = 32;
	}
}

CoinBar::~CoinBar()
{
	SafeDelete(img);
}

//------------------------------------------------------------------------------
//	更新・描画
//------------------------------------------------------------------------------

void CoinBar::Update()
{
	BarControl();
}


void CoinBar::Render()
{
	//フレーム
	img->Render(frame_x, frame_y, 512, 64, 0, 32 * 5, frame_sx, frame_sy);

	//灰色のバー
	img->Render(bar_x[0], bar_y[0], 480, 32, 0, 32 * 4, 480, 32);

	//色のバー
	img->Render(bar_x[0], bar_y[0], bar_sx[0], 32, 0, 32 * 0, bar_sx[0], bar_sy[0]);
	img->Render(bar_x[1], bar_y[1], bar_sx[1], 32, 0, 32 * 1, bar_sx[1], bar_sy[1]);
	img->Render(bar_x[2], bar_y[2], bar_sx[2], 32, 0, 32 * 2, bar_sx[2], bar_sy[2]);
	img->Render(bar_x[3], bar_y[3], bar_sx[3], 32, 0, 32 * 3, bar_sx[3], bar_sy[3]);


}

//------------------------------------------------------------------------------
//	関数
//------------------------------------------------------------------------------
void CoinBar::BarControl()
{
	//-------------------------------------------------
	//	↓後で変える
	const int MAX_COIN = 201;

	int num_coin[4];
	num_coin[0] = m_Player->GetCoinNum(0);
	num_coin[1] = m_Player->GetCoinNum(1);
	num_coin[2] = m_Player->GetCoinNum(2);
	num_coin[3] = m_Player->GetCoinNum(3);
	//if (KEY_Get(KEY_A) == 1) num_coin[2]--;
	//if (KEY_Get(KEY_B) == 1) num_coin[2]++;


	bar_x[1] = bar_x[0] + bar_sx[0];
	bar_x[2] = bar_x[1] + bar_sx[1];
	bar_x[3] = bar_x[2] + bar_sx[2];

	bar_sx[0] = 480 * num_coin[0] / MAX_COIN;
	bar_sx[1] = 480 * num_coin[1] / MAX_COIN;
	bar_sx[2] = 480 * num_coin[2] / MAX_COIN;
	bar_sx[3] = 480 * num_coin[3] / MAX_COIN;

	//	↑後で変える
	//--------------------------------------------------
}