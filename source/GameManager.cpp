
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"GlobalFunction.h"
#include	"Random.h"
#include	"Collision.h"
#include	"UI.h"
#include	"EventManager.h"
#include	"ItemManager.h"
#include	"CoinManager.h"
#include	"sceneResult.h"
#include	"CharacterManager.h"
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
		InitPos[0] = Vector3( -10.0f, 10.0f, 0.0f );
		InitPos[1] = Vector3( 10.0f, 10.0f, 0.0f );
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
		for ( int i = 0; i < 4; i++ )
		{
			charatype[i] = 0;
			coinNum[i] = 0;
		}
		playerNum = 0;
		stageType = 0;
		timer = TIMELIMIT;
		mode = 0;
		donketsuBoostState = false;
		lastBonus = rand() % 4;
		return	true;
	}

	//	解放
	void	GameManager::Release( void )
	{

	}

//-------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------

	//	更新
	void	GameManager::Update( void )
	{
		//	タイマー更新
		timer--;

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
		if ( timer == 42 * SECOND )	ui->SetAlertFlag( true );
		if ( timer == 40 * SECOND )	eventManager->SetEvent( Random::GetInt( 0, EVENT_MODE::NONE - 1 ) );

		//	３秒ごとにアイテムを３割の確率ででランダムに配置
		if ( timer % ( 3* SECOND ) == 0 )
		{
			if ( Random::PercentageRandom( 0.7f ) )
			{
				itemManager->Append( Vector3( Random::GetFloat( -20.0f, 20.0f ), 50.0f, Random::GetFloat( -20.0f, 15.0f ) ), ITEM_TYPE::ATTACK_UP );
			}
		}

		//	２秒ごとにコインを４割の確率でランダムに配置
		if ( timer % ( 1 * SECOND ) == 0 )
		{
			if ( Random::PercentageRandom( 0.6f ) )
			{
				if ( coinNum[0] + coinNum[1] + coinNum[2] + coinNum[3] + m_CoinManager->GetFreeCoinNum() < 201)
				m_CoinManager->Set( Vector3( Random::GetFloat( -20.0f, 20.0f ), 50.0f, Random::GetFloat( -20.0f, 15.0f ) ), Vector3( 0.0f, -1.0f, 0.0f ), 1.0f );
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
		for (int i = 1; i < 4; i++)
		{
			if (coinNum[i] < work)
			{
				work = coinNum[i];
				Min = i;
			}
		}

		worst = Min;
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

