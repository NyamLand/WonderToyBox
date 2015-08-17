#include	"iextreme.h"
#include	"system/system.h"
#include	"system/Framework.h"
#include	"GlobalFunction.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"PlayerManager.h"
#include	"GameManager.h"
#include	"UI.h"

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

	//	�R���X�g���N�^
	HeadUpDisplay::HeadUpDisplay( void )
	{
	}

	//	�f�X�g���N�^
	HeadUpDisplay::~HeadUpDisplay( void )
	{
		SafeDelete( c_Coinbar );
		SafeDelete( c_Timer );
	}

	//	������
	bool HeadUpDisplay::Initialize( void )
	{
		c_Coinbar = new CoinBar();
		c_Timer = new Timer();
		return true;
	}

//------------------------------------------------------------------------------
//	�X�V�E�`��
//------------------------------------------------------------------------------

	//	�X�V
	void HeadUpDisplay::Update( void )
	{
		c_Coinbar->Update();
		c_Timer->Update();
	}

	//	�`��
	void HeadUpDisplay::Render( void )
	{
		c_Coinbar->Render();
		c_Timer->Render();
	}

//------------------------------------------------------------------------------
//	���ݒ�
//------------------------------------------------------------------------------

	//	���ԓn��
	void	HeadUpDisplay::SetTimer( int time )
	{
		this->c_Timer->SetTimer( time );
	}

//****************************************************************************************
//
//	CoinBar�N���X
//
//****************************************************************************************

//------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------

	//	�R���X�g���N�^
	CoinBar::CoinBar( void )
	{
		img_bar = new iex2DObj( "DATA\\BG\\coin_gage.png" );
		img_state = new iex2DObj( "DATA\\BG\\face.png" );
		frame_x = ( 1280 / 2 ) - ( 512 / 2 );
		frame_y = 600;
		frame_sx = 512;
		frame_sy = 64;
		for ( int i = 0; i < NUM_BAR; i++ )
		{
			bar_x[i] = frame_x + 16;
			bar_y[i] = frame_y + 16;
			bar_sx[i] = 480;
			bar_sy[i] = 32;
			state_x[i] = 0;
		}
	}

	//	�f�X�g���N�^
	CoinBar::~CoinBar( void )
	{
		SafeDelete( img_bar );
		SafeDelete( img_state );
	}

//------------------------------------------------------------------------------
//	�X�V�E�`��
//------------------------------------------------------------------------------

	//	�X�V
	void CoinBar::Update( void )
	{
		BarControl();
		StateImageControl();
	}

	//	�`��
	void CoinBar::Render( void )
	{
		//�t���[��
		img_bar->Render( frame_x, frame_y, 512, 64, 0, 32 * 5, frame_sx, frame_sy );

		//�D�F�̃o�[
		img_bar->Render( bar_x[0], bar_y[0], 480, 32, 0, 32 * 4, 480, 32 );

		//�F�̃o�[
		img_bar->Render( bar_x[0], bar_y[0], bar_sx[0], 32, 0, 32 * 0, bar_sx[0], bar_sy[0] );
		img_bar->Render( bar_x[1], bar_y[1], bar_sx[1], 32, 0, 32 * 1, bar_sx[1], bar_sy[1] );
		img_bar->Render( bar_x[2], bar_y[2], bar_sx[2], 32, 0, 32 * 2, bar_sx[2], bar_sy[2] );
		img_bar->Render( bar_x[3], bar_y[3], bar_sx[3], 32, 0, 32 * 3, bar_sx[3], bar_sy[3] );

		//��
		img_state->Render( state_x[0], 550, 32, 32, state_type[0] * 32, 0, 32, 32 );
		img_state->Render( state_x[1], 550, 32, 32, state_type[1] * 32, 0, 32, 32 );
		img_state->Render( state_x[2], 550, 32, 32, state_type[2] * 32, 0, 32, 32 );
		img_state->Render( state_x[3], 550, 32, 32, state_type[3] * 32, 0, 32, 32 );
	}

//------------------------------------------------------------------------------
//	�֐�
//------------------------------------------------------------------------------

	//	�o�[����
	void CoinBar::BarControl( void )
	{
		//-------------------------------------------------
		//	����ŕς���
		const int MAX_COIN = 201;

		int num_coin[4];
		for ( int i = 0; i < 4; i++ )
		{
			num_coin[i] = GameManager::GetCoinNum( i );
		}
		//num_coin[0] = m_Player->GetCoinNum( 0 );
		//num_coin[1] = m_Player->GetCoinNum( 1 );
		//num_coin[2] = m_Player->GetCoinNum( 2 );
		//num_coin[3] = m_Player->GetCoinNum( 3 );

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

	//	�瓮��
	void CoinBar::StateImageControl( void )
	{
		//�摜�̕`��ꏊ = �e�F�̐擪�@+�@�e�F�̒��S�@-�@�摜�T�C�Y�̔���
		state_x[0] = ( bar_x[0] + ( bar_x[1] - bar_x[0] ) / 2 ) - 16;
		state_x[1] = ( bar_x[1] + ( bar_x[2] - bar_x[1] ) / 2 ) - 16;
		state_x[2] = ( bar_x[2] + ( bar_x[3] - bar_x[2] ) / 2 ) - 16;
		state_x[3] = ( bar_x[3] + ( bar_x[3] + bar_sx[3] - bar_x[3] ) / 2 ) - 16;


		int num_coin[4],temp_coin[4];
		for ( int i = 0; i < 4; i++ )
		{
			num_coin[i] = temp_coin[i] = GameManager::GetCoinNum( i );
		}
		//num_coin[0] = temp_coin[0] = m_Player->GetCoinNum(0);
		//num_coin[1] = temp_coin[1] = m_Player->GetCoinNum( 1 );
		//num_coin[2] = temp_coin[2] = m_Player->GetCoinNum( 2 );
		//num_coin[3] = temp_coin[3] = m_Player->GetCoinNum( 3 );

		for ( int i = 0,temp; i < 4 - 1; i++ )
		{
			for ( int j = 4 - 1; j > i; j-- )
			{
				if ( temp_coin[j - 1] > temp_coin[j] )
				{
					temp = temp_coin[j];
					temp_coin[j] = temp_coin[j - 1];
					temp_coin[j - 1] = temp;
				}
			}
		}

		for ( int i = 0; i < 4; i++ )
		{
			for ( int j = 0; j < 4; j++ )
			{
				if ( num_coin[i] == temp_coin[j] )
				{
					if ( j == 0 ) state_type[i] = CoinBar::PlayerState::Bad;
					if ( j == 1 || j == 2 ) state_type[i] = CoinBar::PlayerState::Normal;
					if ( j == 3 ) state_type[i] = CoinBar::PlayerState::Good;
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

	//	�R���X�g���N�^
	Timer::Timer( void ) : sec( 0 ),timer( TIMELIMIT )
	{
		img = new iex2DObj( "DATA\\BG\\number.png" );
		x = 200;
		y = 0;
		sx = 64;
		sy = 64;
		for ( int i = 0; i < 2; i++ )
		{
			minute[i] = 0;
		}
	}

	//	�f�X�g���N�^
	Timer::~Timer( void )
	{
		SafeDelete( img );
	}

//------------------------------------------------------------------------------
//	�X�V�E�`��
//------------------------------------------------------------------------------

	//	�X�V
	void Timer::Update( void )
	{
		Calculate_Time();
	}

	//	�`��
	void Timer::Render( void )
	{
		//img->Render(x, y, 32, 32, timer / 60 / 60 * 64, 0, sx, sy);
		img->Render( x + 64 * 0, y, 64, 64, sec * 64, 0, sx, sy );
		img->Render( x + 64 * 1, y, 64, 64, 10 * 64, 0, sx, sy );
		img->Render( x + 64 * 2, y, 64, 64, minute[0] * 64, 0, sx, sy );
		img->Render( x + 64 * 3, y, 64, 64, minute[1] * 64, 0, sx, sy );
		//char	str[256];
		//sprintf_s(str, "timelimit = %d��%d�b", timer / 60 / 60, timer / 60);
		//DrawString(str, 550, 100);
	}

//------------------------------------------------------------------------------
//	�֐�
//------------------------------------------------------------------------------
		
	//	�e���v�Z
	void Timer::Calculate_Time( void )
	{
		//��
		sec = timer / MINUTE % 10;
		//�b�񌅖�
		minute[0] = ( timer / SECOND ) % 60 / 10 % 10;
		//�b�ꌅ��
		minute[1] = timer / SECOND % 10;
	}