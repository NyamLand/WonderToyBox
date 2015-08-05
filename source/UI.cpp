#include	"iextreme.h"
#include	"system/system.h"
#include	"system/Framework.h"
#include	"GlobalFunction.h"
#include	"UI.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"PlayerManager.h"

//****************************************************************************************
//
//	HeadUpDisplay�N���X
//
//****************************************************************************************

//------------------------------------------------------------------------------
//	�O���[�o��
//------------------------------------------------------------------------------

HeadUpDisplay*	m_UI = NULL;

//------------------------------------------------------------------------------
//	�������E���
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
	timer = new Timer();
	return true;
}

//------------------------------------------------------------------------------
//	�X�V�E�`��
//------------------------------------------------------------------------------
void HeadUpDisplay::Update()
{
	coinbar->Update();
	timer->Update();
}

void HeadUpDisplay::Render()
{
	coinbar->Render();
	timer->Render();
}


//****************************************************************************************
//
//	CoinBar�N���X
//
//****************************************************************************************
//------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------
CoinBar::CoinBar()
{
	img_bar = new iex2DObj("DATA\\BG\\coin_gage.png");
	img_state = new iex2DObj("DATA\\BG\\face.png");
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
		state_x[i] = 0;
	}
}

CoinBar::~CoinBar()
{
	SafeDelete(img_bar);
	SafeDelete(img_state);
}

//------------------------------------------------------------------------------
//	�X�V�E�`��
//------------------------------------------------------------------------------

void CoinBar::Update()
{
	BarControl();
	StateImageControl();
}


void CoinBar::Render()
{
	//�t���[��
	img_bar->Render(frame_x, frame_y, 512, 64, 0, 32 * 5, frame_sx, frame_sy);

	//�D�F�̃o�[
	img_bar->Render(bar_x[0], bar_y[0], 480, 32, 0, 32 * 4, 480, 32);

	//�F�̃o�[
	img_bar->Render(bar_x[0], bar_y[0], bar_sx[0], 32, 0, 32 * 0, bar_sx[0], bar_sy[0]);
	img_bar->Render(bar_x[1], bar_y[1], bar_sx[1], 32, 0, 32 * 1, bar_sx[1], bar_sy[1]);
	img_bar->Render(bar_x[2], bar_y[2], bar_sx[2], 32, 0, 32 * 2, bar_sx[2], bar_sy[2]);
	img_bar->Render(bar_x[3], bar_y[3], bar_sx[3], 32, 0, 32 * 3, bar_sx[3], bar_sy[3]);

	//��
	img_state->Render(state_x[0], 550, 32, 32, state_type[0] * 32, 0, 32, 32);
	img_state->Render(state_x[1], 550, 32, 32, state_type[1] * 32, 0, 32, 32);
	img_state->Render(state_x[2], 550, 32, 32, state_type[2] * 32, 0, 32, 32);
	img_state->Render(state_x[3], 550, 32, 32, state_type[3] * 32, 0, 32, 32);


}

//------------------------------------------------------------------------------
//	�֐�
//------------------------------------------------------------------------------
void CoinBar::BarControl()
{
	//-------------------------------------------------
	//	����ŕς���
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

	//	����ŕς���
	//--------------------------------------------------
}

void CoinBar::StateImageControl()
{
	//�摜�̕`��ꏊ = �e�F�̐擪�@+�@�e�F�̒��S�@-�@�摜�T�C�Y�̔���
	state_x[0] = ( bar_x[0] + ( bar_x[1] - bar_x[0] ) / 2 ) - 16;
	state_x[1] = ( bar_x[1] + ( bar_x[2] - bar_x[1] ) / 2 ) - 16;
	state_x[2] = ( bar_x[2] + ( bar_x[3] - bar_x[2] ) / 2 ) - 16;
	state_x[3] = ( bar_x[3] + ( bar_x[3] + bar_sx[3] - bar_x[3] ) / 2 ) - 16;


	int num_coin[4],temp_coin[4];
	num_coin[0] = temp_coin[0] = m_Player->GetCoinNum(0);
	num_coin[1] = temp_coin[1] = m_Player->GetCoinNum(1);
	num_coin[2] = temp_coin[2] = m_Player->GetCoinNum(2);
	num_coin[3] = temp_coin[3] = m_Player->GetCoinNum(3);

	for (int i = 0,temp; i < 4 - 1; i++)
	{
		for (int j = 4 - 1; j > i; j--)
		{
			if (temp_coin[j - 1] > temp_coin[j])
			{
				temp = temp_coin[j];
				temp_coin[j] = temp_coin[j - 1];
				temp_coin[j - 1] = temp;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (num_coin[i] == temp_coin[j])
			{
				if (j == 0) state_type[i] = CoinBar::PlayerState::Bad;
				if (j == 1 || j == 2) state_type[i] = CoinBar::PlayerState::Normal;
				if (j == 3) state_type[i] = CoinBar::PlayerState::Good;
			}
		}
	}

}

//****************************************************************************************
//
//	Timer�N���X
//
//****************************************************************************************
//------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------
Timer::Timer() : sec(0),timer(TIMELIMIT)
{
	img = new iex2DObj("DATA\\BG\\number.png");
	x = 200;
	y = 0;
	sx = 64;
	sy = 64;
	for (int i = 0; i < 2; i++)
	{
		minute[i] = 0;
	}
}

Timer::~Timer()
{
	SafeDelete(img);
}

//------------------------------------------------------------------------------
//	�X�V�E�`��
//------------------------------------------------------------------------------

void Timer::Update()
{
	Calculate_Time();
}


void Timer::Render()
{
	//img->Render(x, y, 32, 32, timer / 60 / 60 * 64, 0, sx, sy);
	img->Render(x + 64 * 0, y, 64, 64, sec * 64, 0, sx, sy);
	img->Render(x + 64 * 1, y, 64, 64, 10 * 64, 0, sx, sy);
	img->Render(x + 64 * 2, y, 64, 64, minute[0] * 64, 0, sx, sy);
	img->Render(x + 64 * 3, y, 64, 64, minute[1] * 64, 0, sx, sy);
	//char	str[256];
	//sprintf_s(str, "timelimit = %d��%d�b", timer / 60 / 60, timer / 60);
	//DrawString(str, 550, 100);
}

//------------------------------------------------------------------------------
//	�֐�
//------------------------------------------------------------------------------
void Timer::Calculate_Time()
{
	timer--;
	//��
	sec = timer / MINUTE % 10;
	//�b�񌅖�
	minute[0] = (timer / SECOND) % 60 / 10 % 10;
	//�b�ꌅ��
	minute[1] = timer / SECOND % 10;

}