
#include	"iextreme.h"
#include	"system/Framework.h"
#include	<iostream>
#include	<fstream>
#include	<string>
#include	"GlobalFunction.h"
#include	"textLoader.h"
#include	"Random.h"
#include	"Collision.h"
#include	"UI.h"
#include	"EventManager.h"
#include	"ItemManager.h"
#include	"CoinManager.h"
#include	"sceneResult.h"
#include	"CharacterManager.h"
#include	"Sound.h"
#include	"Camera.h"
#include	"GameManager.h"

//*******************************************************************************
//
//	GameManagerクラス
//
//*******************************************************************************

//-------------------------------------------------------------------------
//	グローバル
//-------------------------------------------------------------------------

#define	START_LIFE	5
#define	ITEM_GENERATION_INTERVAL	5
#define	COIN_GENERATION_INTERVAL	1
#define	LIMIT_COIN_NUM						300
	
//-------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------

	//	コンストラクタ
	GameManager::GameManager( void ) : 
		itemflg(false), newsflag(false), coinCollision(false), absCoinEventFlag(false), 
		maxlife(0), playerNum(0), stageType(0), timer(0), timelimit(0), mode(0), worst(0), timeStop(0), round(0), eventmode(0)
	{
		FOR( 0, PLAYER_MAX )
		{
			startLife[value] = 0;
			charatype[value] = 0;
			coinNum[value] = 0;

			//	ラウンドごとのコイン枚数初期化
			for ( int i = 0; i < Round::END; i++ )
			{
				totalCoin[i][value] = 0;
			}
		}

		//	プレイヤー初期位置設定
		InitPos[0] = Vector3( -10.0f, 10.0f, 15.0f );
		InitPos[1] = Vector3( 10.0f, 10.0f, 15.0f );
		InitPos[2] = Vector3( -10.0f, 10.0f, -15.0f );
		InitPos[3] = Vector3( 10.0f, 10.0f, -15.0f );

		//	プレイヤー色設定
		playerColor[0] = Vector3( 1.0f, 0.0f, 0.0f );
		playerColor[1] = Vector3( 0.0f, 0.0f, 1.0f );
		playerColor[2] = Vector3( 1.0f, 1.0f, 0.0f );
		playerColor[3] = Vector3( 0.0f, 1.0f, 0.0f );
	}

	//	デストラクタ
	GameManager::~GameManager( void )
	{

	}

	//	初期化
	bool	GameManager::Initialize( void )
	{
		Random::Initialize();

		FOR( 0, PLAYER_MAX )
		{
			charatype[value] = 0;		
			coinNum[value] = 0;
			startLife[value] = START_LIFE;
		}

		playerNum = 0;
		stageType = 1;
		mode = 0;
		timeStop = 0;
		round = 0;
		timer = 0;
		maxlife = LIFE_MAX_NUM::LIFE_5;
		coinCollision = false;
		eventmode = 0;
		absCoinEventFlag = false;

		FOR( 0, Round::END )
		{
			for ( int i = 0; i < PLAYER_MAX; i++ )
			{
				totalCoin[value][i] = 0;
			}
		}

		return	true;
	}

	//	解放
	void	GameManager::Release( void )
	{

	}

	//	リトライ用初期化
	void	GameManager::RetryInitialize( void )
	{
		FOR( 0, PLAYER_MAX )	coinNum[value] = 0;
		mode = 0;
		donketsuBoostState = false;
		lastBonus = Random::GetInt( 0, 3 );
		timeStop = 0;
		eventmode = 0;
		timer = timelimit;
	}

//-------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------

	//	更新
	void	GameManager::Update( void )
	{
		//	タイマー更新
		TimerUpdate();

		//	時間経過で各要素発生
		switch ( timer )
		{
		case TIMER_EVENT::TIME_UP:	
			TimeUp();
			break;

		case TIMER_EVENT::MOVE_CLIMAX:
			SetMode( GAME_MODE::CLIMAX );
			break;
		}

		//	イベント管理
		EventManagement();
		
		//	アイテム、コイン生成管理
		ItemAndCoinManagement();
	}

	//	描画
	void	GameManager::Render( void )
	{
		//	情報描画
	}

//-------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------

	//	コイン加算
	void	GameManager::AddCoin( int playerNum )
	{
		coinNum[playerNum]++;
	}

	//	コイン減算
	void	GameManager::SubCoin( int playerNum )
	{
		if ( coinNum[playerNum] > 0 )
			coinNum[playerNum]--;
	}

	//　ビリが誰かを決定
	void	GameManager::DecideWorst( void )
	{
		//　プレイヤー同士のコイン数を比較して
		//　コイン数が最小のプレイヤーの番号を返す（はず）

		int work(coinNum[0]);
		int Min(0);
		for (int i = 1; i < PLAYER_MAX; i++)
		{
			if (coinNum[i] < work)
			{
				work = coinNum[i];
				Min = i;
			}
		}

		worst = Min;
	}

	//	タイマー更新
	void	GameManager::TimerUpdate( void )
	{
		//	タイマー更新
		if ( timeStop > 0 )
		{
			timeStop--;
			return;
		}
		else timer--;

		//	タイマー下限設定
		if ( timer <= 0 )	timer = 0;
	}

	//	時間切れ
	void	GameManager::TimeUp( void )
	{
		//	モードをタイムアップに変更
		if ( mode != GAME_MODE::TIMEUP )
		{
			mode = GAME_MODE::TIMEUP;
			sound->PlaySE( SE::GAMESET_SE );
		}

		FOR( 0, PLAYER_MAX )
		{
			//	全員無敵にする
			characterManager->SetParameterInfo( value, PARAMETER_STATE::UNRIVALED );

			//	全員待機状態にする
			if ( characterManager->GetIsPlayer( value ) )
			{
				if ( characterManager->GetMode( value ) == MODE_STATE::MOVE )
				{
					characterManager->SetMode( value, MODE_STATE::WAIT );
				}
			}
			else
			{
				characterManager->SetAIMode( value, AI_MODE_STATE::STOP );
			}

		}
	}

	//	イベント発生管理
	void	GameManager::EventManagement( void )
	{
		if ( timer <= 0 )	return;

		int alert_type = 0;
		int alert_sound = 0;

		/*
		ランダムでイベント決定、
		イベントの各種パラメータ設定後、
		eventManagerでイベントを発生させる
		*/

		//　演出開始
		if ( timer % EVENT_INTERVAL == EVENT_TIMING )
		{
			if ( timer % ( 3 * EVENT_INTERVAL ) == EVENT_TIMING )	absCoinEventFlag = true;

			eventmode = ( absCoinEventFlag ) ?
				Random::GetInt( EVENT_MODE::COIN_FALL, EVENT_MODE::COIN_DUBBLE ) :
				Random::GetInt( 0, EVENT_MODE::MAX - 1 );

			if ( eventmode <= EVENT_MODE::JAM_SLIP )
			{
				alert_type = ALERT_TYPE_INFO::JAM;
				alert_sound = SE::EVENT_SE;
			}
			else if ( EVENT_MODE::COIN_FALL <= eventmode  && eventmode <= EVENT_MODE::COIN_DUBBLE )
			{
				alert_type = ALERT_TYPE_INFO::COIN;
				alert_sound = SE::EVENT_SE;		//　仮　コインイベント用に変更
			}
			else
			{
				alert_type = ALERT_TYPE_INFO::MISSION;
				alert_sound = SE::EVENT_SE;		//　仮　ミッションイベント用に変更
			}

			//　画像用に設定
			switch ( eventmode )
			{
			case EVENT_MODE::COIN_FALL:						ui->SetEventInfoMode( EVENT_TEX_INFO::C_FALL );		break;
			case EVENT_MODE::COIN_WAVE:					ui->SetEventInfoMode( EVENT_TEX_INFO::C_WAVE );		break;
			case EVENT_MODE::COIN_DUBBLE:				ui->SetEventInfoMode( EVENT_TEX_INFO::C_DUBBLE );	break;
			case EVENT_MODE::JAM_SLOPE_CAMERA:		ui->SetEventInfoMode( EVENT_TEX_INFO::J_CAMERA );	break;
			case EVENT_MODE::JAM_SLIP:						ui->SetEventInfoMode( EVENT_TEX_INFO::J_SLIP );			break;
			case EVENT_MODE::JAM_COIN_GETAWAY:	ui->SetEventInfoMode( EVENT_TEX_INFO::J_GETAWAY ); break;
			default:	break;
			}

			ui->SetAlertInfo( true, alert_type );
			sound->PlaySE( alert_sound );
		}

		//　イベント発生
		if ( timer % EVENT_INTERVAL == EVENT_TIMING - 2 * SECOND )
		{
			eventManager->SetEvent( eventmode );
			absCoinEventFlag = false;
		}
	}

	//	アイテムとコインの生成管理
	void	GameManager::ItemAndCoinManagement( void )
	{
		if ( timer == 0 )	return;

		//---------------------------------------------------------------------------
		//	アイテムばらまき
		//---------------------------------------------------------------------------
			//	5秒ごとにアイテムを３割の確率ででランダムに配置
			if ( timer % ( ITEM_GENERATION_INTERVAL * SECOND ) == 0 )
			{
				if ( itemflg )
				{
					if ( Random::PercentageRandom( 0.7f ) )
					{
						itemManager->Append( 
							Vector3( Random::GetFloat( -20.0f, 20.0f ), 
							50.0f, 
							Random::GetFloat( -20.0f, 15.0f ) ),
							Random::GetInt( 0, ITEM_TYPE::MAX - 1 ) );
					}
				}
			}
		//---------------------------------------------------------------------------
		//	コインばらまき
		//---------------------------------------------------------------------------
			//	コインを４割の確率でランダムに配置
			if ( timer % ( COIN_GENERATION_INTERVAL * SECOND ) == 0 )
			{
				if ( Random::PercentageRandom( 0.6f ) )
				{
					int	totalCoinNum = coinNum[0] + coinNum[1] + coinNum[2] + coinNum[3] + coinManager->GetFreeCoinNum();
					if ( totalCoinNum < LIMIT_COIN_NUM )
					{
						if ( Random::PercentageRandom( 0.5f ) )
						{
							coinManager->Append( 
								Vector3( Random::GetFloat( -20.0f, 20.0f ), 
								50.0f, 
								Random::GetFloat( -20.0f, 12.0f ) ),
								Vector3( 0.0f, -1.0f, 0.0f ),
								1.0f, 
								Coin::COIN_BAG_5 );
						}
					}
				}
			}
		}

//-------------------------------------------------------------------------
//	情報取得
//-------------------------------------------------------------------------

	//	プレイヤー色取得
	Vector3	GameManager::GetPlayerColor( int playerNum )const
	{
		return	playerColor[playerNum];
	}

	//	プレイヤー人数取得
	int		GameManager::GetPlayerNum( void )const
	{
		return	playerNum;
	}

	//	プレイヤータイプ取得
	int		GameManager::GetCharacterType( int num )const
	{
		int		out = charatype[num];
		return	out;
	}

	//	ステージタイプ取得
	int		GameManager::GetStageType( void )const
	{
		int		out = stageType;
		return	out;
	}
	
	//	コイン情報取得
	int		GameManager::GetCoinNum( int num )const
	{
		int		out = coinNum[num];
		return out;
	}

	//　どんけつ中かどうかを取得
	bool	GameManager::GetDonketsuBoostState( void )const
	{
		bool	out = donketsuBoostState;
		return	out;
	}

	//	ラストボーナス取得
	int		GameManager::GetLastBonus( void )const
	{
		int		out = lastBonus;
		return	out;
	}

	//	タイマー取得
	int		GameManager::GetTimer( void )const
	{
		int		out = timer;
		return	out;
	}

	//	ニュースフラグ取得
	bool		GameManager::GetNewsFlag( void )const
	{
		bool	out = newsflag;
		return	out;
	}

	//	最下位取得
	int		GameManager::GetWorst( void )const
	{
		int		out = worst;
		return	worst;
	}

	//	モード取得
	int		GameManager::GetMode( void )const
	{
		int		out = mode;
		return	out;
	}

	//画面一時停止残り秒数取得
	int		GameManager::GetTimeStop( void )const
	{
		int out = this->timeStop;
		return out;
	}

	//コイン枚数取得
	int		GameManager::GetMaxLife( void )const
	{
		return maxlife;
	}

	//アイテム有無取得
	bool		GameManager::GetItemFlg(void)const
	{
		return itemflg;
	}
	
	//　順位更新
	int		GameManager::GetRank( int player )const
	{
		int num_coin[PLAYER_MAX], temp_coin[PLAYER_MAX];
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			num_coin[i] = temp_coin[i] = GetCoinNum(i);
		}

		for (int i = 0, temp; i < PLAYER_MAX - 1; i++)
		{
			for (int j = PLAYER_MAX - 1; j > i; j--)
			{
				if (temp_coin[j - 1] > temp_coin[j])
				{
					temp = temp_coin[j];
					temp_coin[j] = temp_coin[j - 1];
					temp_coin[j - 1] = temp;
				}
			}
		}

		for (int i = 0; i < PLAYER_MAX; i++)
		{
			for (int j = 0; j < PLAYER_MAX; j++)
			{
				if (num_coin[player] == temp_coin[j])
				{
					return PLAYER_MAX - j;
				}
			}
		}

		return	0;
	}

	//	プレイヤー初期体力取得
	int		GameManager::GetStartLife( int player )const
	{
		return	startLife[player];
	}

	//	現ラウンド取得
	int		GameManager::GetRound( void )const
	{
		return	round;
	}

	//	トータルコイン取得
	int		GameManager::GetTotalCoin( int round, int player )const
	{
		return	totalCoin[round][player];
	}

	//	コインとの当たり判定のON,OFF
	bool		GameManager::GetCoinCollision( void )const
	{
		return	coinCollision;
	}

	//	実体取得
	GameManager*	GameManager::GetInstance( void )
	{
		static	GameManager	out;
		return	&out;
	}


//-------------------------------------------------------------------------
//	情報設定
//-------------------------------------------------------------------------

	//	プレイヤー人数
	void	GameManager::SetPlayerNum( int num )
	{
		playerNum = num;
	}

	//	プレイヤータイプ
	void	GameManager::SetCharacterType( int num, int type )
	{
		charatype[num] = type;
	}

	//	ステージタイプ
	void	GameManager::SetStageType( int type )
	{
		stageType = type;
	}

	//	コイン情報
	void	GameManager::SetCoinNum( int num, int param )
	{
		coinNum[num] = param;
	}

	//	モード設定
	void	GameManager::SetMode( const int& modeNum )
	{
		mode = modeNum;
	}

	//	ニュースフラグ設定
	void	GameManager::SetNewsFlag( const bool& flag )
	{
		newsflag = flag;
	}

	// カメラの振動地設定
	void	GameManager::SetShakeCamera( float wide, int timer )
	{
		if ( mainView ) mainView->ShakeSet( wide, timer );
	}

	//画面一時停止時間設定
	void	GameManager::SetTimeStop( int time )
	{
		this->timeStop = time;
	}

	// タイムリミット分設定
	void	GameManager::SetTime( int minute,int second )
	{
		timer = minute * MINUTE + second * SECOND;
		timelimit = timer;
	}

	//コイン枚数設定
	void	GameManager::SetMaxLife( int life )
	{
		this->maxlife = life;
		FOR( 0, PLAYER_MAX )	startLife[value] = maxlife + 3;
	}

	//アイテム有無設定
	void	GameManager::SetItemFlg( bool itemflg )
	{
		this->itemflg = itemflg;
	}

	//	初期体力設定
	void	GameManager::SetStartLife( int player, int life )
	{
		startLife[player] = life;
	}

	//	ラウンド設定
	void	GameManager::SetRound( int round )
	{
		this->round = round;
	}

	//	トータルコイン設定
	void	GameManager::SetTotalCoin( int round, int player, int total )
	{
		totalCoin[round][player] = total;
	}

	//	コイン当たり判定の状態習得
	void	GameManager::SetCoinCollision( bool state )
	{
		coinCollision = state;
	}