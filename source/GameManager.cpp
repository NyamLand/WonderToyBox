
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
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			charatype[i] = 0;		
			coinNum[i] = 0;
		}
		playerNum = 0;
		stageType = 1;
		mode = 0;
		donketsuBoostState = false;
		lastBonus = rand() % PLAYER_MAX;
		timeStop = 0;

		//	ゲームデータテキストを読み込む
		//LoadTextData();
		timer = 0;

		return	true;
	}

	//　（メインから始めるための）デバッグ用
	bool	GameManager::InitializeDebug(void)
	{
		charatype[0] = 1;

		for (int i = 1; i < PLAYER_MAX; i++)
		{
			charatype[i] = i;
			coinNum[i] = 0;
		}
		playerNum = 1;	//　操作する人数
		stageType = 0;
		mode = 0;
		donketsuBoostState = false;
		lastBonus = rand() % PLAYER_MAX;
		timeStop = 0;
		//	ゲームデータテキストを読み込む
		LoadTextData();
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
		lastBonus = rand() % PLAYER_MAX;
		timeStop = 0;

		//	ゲームデータテキストを読み込む
		LoadTextData();
		timer = timelimit;
	}

//-------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------

	//	更新
	void	GameManager::Update( void )
	{
		//	タイマー更新
		if (timeStop > 0) timeStop--;
		else timer--;


		//	残り時間３０秒でどんけつ演出へ
		if ( timer == 30 * SECOND )
		{
			DecideWorst();
			mode = GAME_MODE::DONKETSU_DIRECTION;
		}

		//	時間切れ
		if ( timer <= 0 )
		{
			timer = 0;
			mode = GAME_MODE::TIMEUP;
			for ( int i = 0; i < PLAYER_MAX; i++ )	characterManager->SetMode( i, MODE_STATE::WAIT );
		}

		//	どんけつブースト設定
		if ( timer <= 30 * SECOND )		donketsuBoostState = true;
		else	donketsuBoostState = false;

		//	ニュース設定
		if ( timer == 1 * MINUTE )		newsflag = true;

		//	イベント設定
		if (timer == 42 * SECOND)	{
			ui->SetAlertFlag(true);
			sound->PlaySE(SE::EVENT_SE);
		}
		if ( timer == 40 * SECOND ) eventManager->SetEvent( EVENT_MODE::SLOPE_CAMERA/*Random::GetInt( 0, EVENT_MODE::NONE - 1 )*/ );

		if ( timer != 0 )
		{
			//	5秒ごとにアイテムを３割の確率ででランダムに配置
			if ( timer % ( 5* SECOND ) == 0 )
			{
				if (itemflg){
					if (Random::PercentageRandom(0.7f))
					{
						itemManager->Append(Vector3(Random::GetFloat(-20.0f, 20.0f), 50.0f, Random::GetFloat(-20.0f, 15.0f)), ITEM_TYPE::ATTACK_UP);
					}
				}
			}

			//	２秒ごとにコインを４割の確率でランダムに配置
			if ( timer % ( 1 * SECOND ) == 0 )
			{
				if ( Random::PercentageRandom( 0.6f ) )
				{
					if ( coinNum[0] + coinNum[1] + coinNum[2] + coinNum[3] + m_CoinManager->GetFreeCoinNum() < 201)
					m_CoinManager->Set( Vector3( Random::GetFloat( -20.0f, 20.0f ), 50.0f, Random::GetFloat( -20.0f, 12.0f ) ), Vector3( 0.0f, -1.0f, 0.0f ), 1.0f );
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
	int		GameManager::GetPlayerNum( void )
	{
		return	playerNum;
	}

	//	プレイヤータイプ取得
	int		GameManager::GetCharacterType( int num )
	{
		int		out = charatype[num];
		return	out;
	}

	//	ステージタイプ取得
	int		GameManager::GetStageType( void )
	{
		int		out = stageType;
		return	out;
	}

	//	コイン情報取得
	int		GameManager::GetCoinNum( int num )
	{
		int		out = coinNum[num];
		return out;
	}

	//　どんけつ中かどうかを取得
	bool	GameManager::GetDonketsuBoostState( void )
	{
		bool	out = donketsuBoostState;
		return	out;
	}

	//	ラストボーナス取得
	int		GameManager::GetLastBonus( void )
	{
		int		out = lastBonus;
		return	out;
	}

	//	タイマー取得
	int		GameManager::GetTimer( void )
	{
		int		out = timer;
		return	out;
	}

	//	ニュースフラグ取得
	bool	GameManager::GetNewsFlag( void )
	{
		bool	out = newsflag;
		return	out;
	}

	//	最下位取得
	int		GameManager::GetWorst( void )
	{
		int		out = worst;
		return	worst;
	}

	//	モード取得
	int		GameManager::GetMode( void )
	{
		int		out = mode;
		return	out;
	}

	//画面一時停止残り秒数取得
	int		GameManager::GetTimeStop( void )
	{
		int out = this->timeStop;
		return out;
	}

	//コイン枚数取得
	int	GameManager::GetCoinMax(void)const
	{
		return coinmax;
	}

	//アイテム有無取得
	bool	GameManager::GetItemFlg(void)const
	{
		return itemflg;
	}
	//　順位更新
	int		GameManager::GetRank( int player )
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

	//タイムリミット分設定
	void	GameManager::SetTime(int minute,int second)
	{
		timer=minute*MINUTE + second*SECOND;
	}

	//コイン枚数設定
	void	GameManager::SetCoinMax(int coinmax)
	{
		this->coinmax = coinmax;
	}

	//アイテム有無設定
	void	GameManager::SetItemFlg(bool itemflg)
	{
		this->itemflg = itemflg;
	}