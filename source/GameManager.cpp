
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
	
//-------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------

	//	コンストラクタ
	GameManager::GameManager( void )
	{
		InitPos[0] = Vector3( -10.0f, 10.0f, 15.0f );
		InitPos[1] = Vector3( 10.0f, 10.0f, 15.0f );
		InitPos[2] = Vector3( -10.0f, 10.0f, -15.0f );
		InitPos[3] = Vector3( 10.0f, 10.0f, -15.0f );
	}

	//	デストラクタ
	GameManager::~GameManager( void )
	{

	}

	//	初期化
	bool	GameManager::Initialize( void )
	{
		Random::Initialize();
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			charatype[i] = 0;		
			coinNum[i] = 0;
			startLife[i] = 5;
		}
		playerNum = 0;
		stageType = 1;
		mode = 0;
		donketsuBoostState = false;
		lastBonus = Random::GetInt( 0, 3 );
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

	//　（メインから始めるための）デバッグ用
	bool	GameManager::InitializeDebug( void )
	{
		//　プレイヤー
		//charatype[0] = 0;	//　掃除屋
		//charatype[0] = 1;	//　プリンセス
		charatype[0] = 2;	//　怪盗
		//charatype[0] = 3;	//　海賊
		characterManager->Initialize(0, charatype[0], gameManager->InitPos[0], true);

		//　ステージ
		//stageType = 0;	//　机
		stageType = 1;	//　森

		//　時間
		//timelimit = 32 * SECOND;

		//　コイン最大
		maxlife = LIFE_MAX_NUM::LIFE_5;

		//　CPU初期化
		for (int i = 1; i < PLAYER_MAX; i++)
		{
			characterManager->Initialize(i, charatype[i], gameManager->InitPos[i], false);
			charatype[i] = i;
		}
		
		//　他いろいろ
		for (int i = 0; i < PLAYER_MAX; i++)	coinNum[i] = 0;
		playerNum = 1;	//　操作する人数
		mode = 0;
		donketsuBoostState = false;
		lastBonus = rand() % PLAYER_MAX;
		timeStop = 0;
		timer = timelimit;
		
		return	true;
	}

	//	解放
	void	GameManager::Release( void )
	{

	}

	//	リトライ用初期化
	void	GameManager::RetryInitialize( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			coinNum[i] = 0;
		}

		mode = 0;
		donketsuBoostState = false;
		lastBonus = Random::GetInt( 0, 3 );
		timeStop = 0;
		eventmode = 0;
		absCoinEventFlag = false;

		//	ゲームデータテキストを読み込む
		//LoadTextData();
		timer = timelimit;
	}

	//	キャラ選択用初期化

//-------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------

	//	更新
	void	GameManager::Update( void )
	{
		//	タイマー更新
		if (timeStop > 0)
		{
			timeStop--;
			return;
		}
		else timer--;

		//-------------------------------------------------------
		//	時間切れ
		//-------------------------------------------------------
		if ( timer <= 0 )
		{
			timer = 0;
			mode = GAME_MODE::TIMEUP;
			for ( int i = 0; i < PLAYER_MAX; i++ )	characterManager->SetMode( i, MODE_STATE::WAIT );
		}

		//-------------------------------------------------------
		//	クライマックスへ
		//-------------------------------------------------------
		if ( timer == 30 * SECOND ) SetMode( GAME_MODE::CLIMAX );

		//-------------------------------------------------------
		//	ニュース設定
		//-------------------------------------------------------
		if ( timer == 1 * MINUTE )		newsflag = true;

		//-------------------------------------------------------
		//　イベント
		//-------------------------------------------------------
		int alert_type = 0;
		int alert_sound = 0;

		/*
			ランダムでイベント決定、
			イベントの各種パラメータ設定後、
			eventManagerでイベントを発生させる
		*/

		//　演出開始
		if (timer % EVENT_INTERVAL == EVENT_TIMING)
		{
			if (timer % (3 * EVENT_INTERVAL) == EVENT_TIMING)	absCoinEventFlag = true;

			eventmode = (absCoinEventFlag) ?
				Random::GetInt(EVENT_MODE::COIN_FALL, EVENT_MODE::COIN_DUBBLE) :
				Random::GetInt(0, EVENT_MODE::MAX - 1);

			if (eventmode <= EVENT_MODE::JAM_SLIP)
			{
				alert_type = ALERT_TYPE_INFO::JAM;
				alert_sound = SE::EVENT_SE;
			}
			else if (EVENT_MODE::COIN_FALL <= eventmode  && eventmode <= EVENT_MODE::COIN_DUBBLE)
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
			switch (eventmode)
			{
			case EVENT_MODE::COIN_FALL:			ui->SetEventInfoMode(EVENT_TEX_INFO::C_FALL);	break;
			case EVENT_MODE::COIN_WAVE:			ui->SetEventInfoMode(EVENT_TEX_INFO::C_WAVE);	break;
			case EVENT_MODE::COIN_DUBBLE:		ui->SetEventInfoMode(EVENT_TEX_INFO::C_DUBBLE);	break;
			case EVENT_MODE::JAM_SLOPE_CAMERA:	ui->SetEventInfoMode(EVENT_TEX_INFO::J_CAMERA);	break;
			case EVENT_MODE::JAM_SLIP:			ui->SetEventInfoMode(EVENT_TEX_INFO::J_SLIP);	break;
			case EVENT_MODE::JAM_COIN_GETAWAY:	ui->SetEventInfoMode(EVENT_TEX_INFO::J_GETAWAY); break;
			default:	break;
			}

			ui->SetAlertInfo(true, alert_type);
			sound->PlaySE(alert_sound);
		}
		
		//　イベント発生
		if (timer % EVENT_INTERVAL == EVENT_TIMING - 2*SECOND)
		{
			//ui->SetAlertInfo(false, alert_type);
			eventManager->SetEvent(eventmode);

			absCoinEventFlag = false;
		}
		

		//-------------------------------------------------------
		//		アイテム・コインばらまき
		//-------------------------------------------------------
		if ( timer != 0 )
		{
			//	5秒ごとにアイテムを３割の確率ででランダムに配置
			if ( timer % ( 5* SECOND ) == 0 )
			{
				if (itemflg)
				{
					if ( Random::PercentageRandom( 0.7f ) )
					{
						itemManager->Append(Vector3(Random::GetFloat(-20.0f, 20.0f), 50.0f, Random::GetFloat(-20.0f, 15.0f)), Random::GetInt( 0, ITEM_TYPE::MAX - 1 ) );
					}
				}
			}

			//	２秒ごとにコインを４割の確率でランダムに配置
			if ( timer % ( 1 * SECOND ) == 0 )
			{
				if ( Random::PercentageRandom( 0.6f ) )
				{
					if (coinNum[0] + coinNum[1] + coinNum[2] + coinNum[3] + coinManager->GetFreeCoinNum() < 300)
					{
						if ( Random::PercentageRandom( 0.5f ) )
						{
							coinManager->Append( Vector3( Random::GetFloat( -20.0f, 20.0f ), 50.0f, Random::GetFloat( -20.0f, 12.0f ) ), Vector3( 0.0f, -1.0f, 0.0f ), 1.0f, Coin::COIN_BAG_5 );
						}
					}
				}
			}
		}
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

	//	テキスト読み込み
	void	GameManager::LoadTextData( void )
	{
		//	文字列を保存するバッファ,変数を用意
		char	buffer[256];
		int		minute = 0;		//	分
		int		second = 0;		//	秒

		//	ファイルを開く
		std::ifstream	ifs( "GameData.txt" );

		//	最初はコメントなので読み飛ばす, 値は適当
		ifs.getline( buffer, 50 );

		//	読み込んだ値を変数へ代入
		ifs >> minute;

		//	コメントを読み飛ばす
		ifs >> buffer;
		ifs.getline( buffer, 50 );

		//	読み込んだ値を変数へ代入
		ifs >> second;

		//	合計値をタイムリミット変数へ代入
		timelimit = minute * MINUTE + second * SECOND;
	}

//-------------------------------------------------------------------------
//	情報取得
//-------------------------------------------------------------------------

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