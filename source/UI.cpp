
#include	"iextreme.h"
#include	"system/system.h"
#include	"system/Framework.h"
#include	"GlobalFunction.h"
#include	"Player.h"
#include	"PlayerManager.h"
#include	"GameManager.h"
#include	"Image.h"
#include	"UI.h"

//****************************************************************************************
//
//	UIクラス
//
//****************************************************************************************

//------------------------------------------------------------------------------
//	グローバル
//------------------------------------------------------------------------------

	namespace
	{
		//	顔情報
		namespace FACE_INFO
		{
			enum
			{
				Normal,
				Good,
				Bad,
			};
		}	
	}

	//	実体
	UI*	ui = nullptr;

//------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------

	//	コンストラクタ
	UI::UI( void )
	{

	}

	//	デストラクタ
	UI::~UI( void )
	{
		Release();
	}

	//	初期化
	bool	UI::Initialize( void )
	{
		timer = new iex2DObj( "DATA/BG/number.png" );
		coinbar = new iex2DObj( "DATA/BG/coin_gage.png" );
		face = new iex2DObj( "DATA/BG/face.png" );
		countDown = new Image( "DATA/bfUI.png" );

		//	共通変数初期化
		changeflag = false;

		//	各UI情報初期化
		CoinBarInitialize();
		TimerInitialize();
		StartAndTimeUpInitialize();
		NewsBarInitialize();

		return	true;
	}

	//	解放
	void	UI::Release( void )
	{
		SafeDelete( timer );
		SafeDelete( coinbar );
		SafeDelete( face );
		SafeDelete( countDown );
	}

	//	コインバー初期化
	void	UI::CoinBarInitialize( void )
	{
		frame_x = ( 1280 / 2 ) - ( 512 / 2 );
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

	//	タイマー初期化
	void	UI::TimerInitialize( void )
	{
		x = 200;
		y = 0;
		sx = 64;
		sy = 64;
		for ( int i = 0; i < 2; i++ )
		{
			minute[i] = 0;
		}
	}

	//	ニュースバー初期化
	void	UI::NewsBarInitialize( void )
	{
		//	ニュースバー初期化
		newsbar.renderflag = false;
		newsbar.left = 1280;
		newsbar.top = 0;
		newsbar.right = 1280;
		newsbar.bottom = 50;
		newsbar.text = GameInfo::NewsText[ GameManager::GetLastBonus() ];
		newsbar.alpha = 0.5f;
		newsbar.color = Vector3( 0.3f, 0.3f, 0.3f );
		newsbar.step = 0;
		newsbar.textleft = 1500;
	}

	//	カウントダウン・スタート・終了演出
	void	UI::StartAndTimeUpInitialize( void )
	{
		countDown->Initialize( 640, 360, 350, 350, 0, 0, 256, 256 );
		count = 0;
		waitTimer = 0;
	}
	
//------------------------------------------------------------------------------
//	更新
//------------------------------------------------------------------------------

	//	更新
	void	UI::Update( const int& mode )
	{
		switch ( mode )
		{
		case GAME_MODE::GAMESTART:
			StartUpdate();
			break;

		case GAME_MODE::MAINGAME:
			TimerUpdate();
			NewsBarUpdate();
			CoinBarUpdate();
			break;

		case GAME_MODE::DONKETSU_DIRECTION:
			DonketsuDirectionUpdate();
			break;

		case GAME_MODE::CLIMAX:
			TimerUpdate();
			NewsBarUpdate();
			break;

		case GAME_MODE::TIMEUP:
			FinishUpdate();
			break;
		}
	}

	//	タイマー関連動作
	void	UI::TimerUpdate( void )
	{
		this->time = GameManager::GetTimer();
		//分
		second = this->time / MINUTE % 10;
		//秒二桁目
		minute[0] = ( this->time / SECOND ) % 60 / 10 % 10;
		//秒一桁目
		minute[1] = this->time / SECOND % 10;
	}

	//	ニュースバー関連動作
	void	UI::NewsBarUpdate( void )
	{
		if ( !GameManager::GetNewsFlag() )	return;

		switch ( newsbar.step )
		{
		case 0:
			//	バー出現
			newsbar.left -= 30;
			if ( newsbar.left <= 0 )
			{
				newsbar.left = 0;
				newsbar.step++;
			}
			break;

		case 1:
			//	テキスト出現
			newsbar.textleft--;
			if ( newsbar.textleft <= -320 )
			{
				newsbar.textleft = 1500;
				newsbar.step++;
			}
			break;

		case 2:
			//	バー退避
			newsbar.right -= 30;
			if ( newsbar.right <= 0 )
			{
				newsbar.right = 0;
				newsbar.step++;
			}
			break;

		case 3:
			//	初期化
			GameManager::SetNewsFlag( false );
			newsbar.left = 1280;
			newsbar.right = 1280;
			newsbar.textleft = 1500;
			newsbar.step = 0;
			break;
		}
	}

	//	コインバー関連動作
	void	UI::CoinBarUpdate( void )
	{
		//	バー動作
		BarControl();

		//	顔動作
		StateImageControl();
	}

	//	カウントダウン・スタート演出
	void	UI::StartUpdate( void )
	{
		//	タイマー更新
		waitTimer++;

		//	一秒ごとに画像進める
		if ( waitTimer % SECOND == 0 )
		{
			//	カウントダウン
			count++;

			//	読み込み位置・サイズ設定
			switch ( count )
			{
			case 1:
				countDown->SetSearchPos( 256, 0 );
				break;

			case 2:
				countDown->SetSearchPos( 0, 256 );
				break;

			case 3:
				countDown->SetSearchPos( 512, 0 );
				countDown->SetSearchSize( 512, 512 );
				countDown->SetSize( 750, 750 );
				countDown->SetWave();
				break;

			case 4:
				waitTimer = 2 * SECOND;

				//	メインゲームへ
				changeflag = true;
				break;
			}

		}

		countDown->Update();
	}

	//	タイムアップ演出
	void	UI::FinishUpdate( void )
	{
		waitTimer--;

		//	画像読み込み位置・サイズ設定
		countDown->SetSize( 600, 370 );
		countDown->SetSearchPos( 0, 512 );
		countDown->SetSearchSize( 1024, 512 );

		if ( waitTimer <= 0 )	changeflag = true;
	}

	//	どんけつ演出
	void	UI::DonketsuDirectionUpdate( void )
	{
		static int wait( 5 * SECOND );
		wait--;

		if ( wait <= 0 )
		{
			wait = 30;
			changeflag = true;
		}
	}

//------------------------------------------------------------------------------
//	描画
//------------------------------------------------------------------------------

	//	描画
	void	UI::Render( const int& mode )
	{
		switch ( mode )
		{
		case GAME_MODE::GAMESTART:
			StartRender();
			break;

		case GAME_MODE::MAINGAME:
			TimerRender();
			NewsBarRender();
			CoinBarRender();
			break;

		case GAME_MODE::DONKETSU_DIRECTION:
			TimerRender();
			NewsBarRender();
			CoinBarRender();
			DonketsuDirectionRender();
			break;

		case GAME_MODE::CLIMAX:
			TimerRender();
			NewsBarRender();
			break;

		case GAME_MODE::TIMEUP:
			FinishUpdate();
			break;
		}
	}

	//	コインバー描画
	void	UI::CoinBarRender( void )
	{
		//フレーム
		coinbar->Render(frame_x, frame_y, 512, 64, 0, 32 * 5, frame_sx, frame_sy);

		//灰色のバー
		coinbar->Render(bar_x[0], bar_y[0], 480, 32, 0, 32 * 4, 480, 32);

		//色のバー
		coinbar->Render(bar_x[0], bar_y[0], bar_sx[0], 32, 0, 32 * 0, bar_sx[0], bar_sy[0]);
		coinbar->Render(bar_x[1], bar_y[1], bar_sx[1], 32, 0, 32 * 1, bar_sx[1], bar_sy[1]);
		coinbar->Render(bar_x[2], bar_y[2], bar_sx[2], 32, 0, 32 * 2, bar_sx[2], bar_sy[2]);
		coinbar->Render(bar_x[3], bar_y[3], bar_sx[3], 32, 0, 32 * 3, bar_sx[3], bar_sy[3]);

		//顔
		face->Render(state_x[0], 550, 32, 32, state_type[0] * 32, 0, 32, 32);
		face->Render(state_x[1], 550, 32, 32, state_type[1] * 32, 0, 32, 32);
		face->Render(state_x[2], 550, 32, 32, state_type[2] * 32, 0, 32, 32);
		face->Render(state_x[3], 550, 32, 32, state_type[3] * 32, 0, 32, 32);
	}

	//	ニュース描画
	void	UI::NewsBarRender( void )
	{
		if( !GameManager::GetNewsFlag() )	return;
		
		iexPolygon::Rect( newsbar.left, newsbar.top, newsbar.right - newsbar.left, newsbar.bottom - newsbar.top, RS_COPY, GetColor( newsbar.color, newsbar.alpha ) );
		IEX_DrawText( newsbar.text, newsbar.textleft, newsbar.top + 10, 500, 200, 0xFFFFFFFF );
	}

	//	タイマー描画
	void	UI::TimerRender( void )
	{
		timer->Render( x + 64 * 0, y, 64, 64, second * 64, 0, sx, sy );
		timer->Render( x + 64 * 1, y, 64, 64, 10 * 64, 0, sx, sy );
		timer->Render( x + 64 * 2, y, 64, 64, minute[0] * 64, 0, sx, sy );
		timer->Render( x + 64 * 3, y, 64, 64, minute[1] * 64, 0, sx, sy );
	}

	//	カウントダウン・スタート演出
	void	UI::StartRender( void )
	{
		countDown->NormalRender();
		countDown->Render();
	}

	//	タイムアップ演出
	void	UI::FinishRender( void )
	{
		countDown->NormalRender();
	}

	//	どんけつ演出
	void	UI::DonketsuDirectionRender( void )
	{
		char	str[256];
		int		worst = GameManager::GetWorst();
		DrawString( "どんけつ演出", 200, 50 );
		wsprintf( str, "ビリは p%d", worst + 1 );
		DrawString( str, 200, 70 );
	}

//------------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------------

	//	バー動作
	void	UI::BarControl( void )
	{
		static const int MAX_COIN = 201;

		bar_x[1] = bar_x[0] + bar_sx[0];
		bar_x[2] = bar_x[1] + bar_sx[1];
		bar_x[3] = bar_x[2] + bar_sx[2];

		int num_coin[4];
		for ( int i = 0; i < 4; i++ )
		{
			num_coin[i] = GameManager::GetCoinNum( i );
			bar_sx[i] = 480 * num_coin[i] / MAX_COIN;
		}
	}

	//	顔動作
	void	UI::StateImageControl( void )
	{
		//画像の描画場所 = 各色の先頭　+　各色の中心　-　画像サイズの半分
		state_x[0] = (bar_x[0] + (bar_x[1] - bar_x[0]) / 2) - 16;
		state_x[1] = (bar_x[1] + (bar_x[2] - bar_x[1]) / 2) - 16;
		state_x[2] = (bar_x[2] + (bar_x[3] - bar_x[2]) / 2) - 16;
		state_x[3] = (bar_x[3] + (bar_x[3] + bar_sx[3] - bar_x[3]) / 2) - 16;


		int num_coin[4], temp_coin[4];
		for (int i = 0; i < 4; i++)
		{
			num_coin[i] = temp_coin[i] = GameManager::GetCoinNum(i);
		}

		for (int i = 0, temp; i < 4 - 1; i++)
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

		for ( int i = 0; i < 4; i++ )
		{
			for ( int j = 0; j < 4; j++ )
			{
				if ( num_coin[i] == temp_coin[j] )
				{
					switch ( j )
					{
					case 0:
						state_type[i] = FACE_INFO::Bad;
						break;

					case 1:
					case 2:
						state_type[i] = FACE_INFO::Normal;
						break;

					case 3:
						state_type[i] = FACE_INFO::Good;
						break;
					}
				}
			}
		}
	}

//------------------------------------------------------------------------------
//	情報設定・取得
//------------------------------------------------------------------------------
	
	//	モード変更フラグ設定
	void	UI::SetChangeFlag( const bool& flag )
	{
		this->changeflag = flag;
	}

	//	モード変更フラグ取得
	bool	UI::GetChangeFlag( void )
	{
		bool	out = this->changeflag;
		return	out;
	}

