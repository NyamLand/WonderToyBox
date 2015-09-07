
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"UI.h"
#include	"sceneResult.h"
#include	"GameManager.h"

//*******************************************************************************
//
//	GameManagerクラス
//
//*******************************************************************************

//-------------------------------------------------------------------------
//	グローバル
//-------------------------------------------------------------------------
	
	//	static
	int		GameManager::charatype[4] = { 0, 0, 0, 0 };
	int		GameManager::coinNum[4] = { 0, 0, 0, 0 };
	int		GameManager::playerNum = 0;
	int		GameManager::stageType = 0;
	int		GameManager::waitTimer = 0;
	bool	GameManager::changeSceneflag = false; 
	bool	GameManager::donketsuBoostState = false;
	int		GameManager::timer = 0;
	int		GameManager::mode = 0;
	int		GameManager::worst = 0;

//-------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------

	//	コンストラクタ
	GameManager::GameManager( void )
	{
	
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
		changeSceneflag = false;
		timer = TIMELIMIT;
		waitTimer = 2 * SECOND;
		mode = 0;
		donketsuBoostState = false;

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
		switch ( mode )
		{
		case MAINGAME:
			MainGameUpdate();
			break;

		//　どんけつ用演出
		case DONKETSU_DIRECTION:
			//　ここでビリを決定（以後変更なし）
			DonketsuDirectionUpdate(); 
			break;
		
		case TIMEUP:
			TimeUpUpdate();
			break;
		}
	}

	//	描画
	void	GameManager::Render( void )
	{
		switch ( mode )
		{
		case MAINGAME:
			MainGameInfoRender();
			break;

		//　どんけつ用演出
		case DONKETSU_DIRECTION:
			DonketsuDirectionRender();
			break;
		
		case TIMEUP:
			TimeUpRender();
			break;

		}
	}
	//------------------------------------------------------------------
	//	ゲーム動作中
	//------------------------------------------------------------------
	//	更新
	void	GameManager::MainGameUpdate(void)
	{
		timer--;
		if (timer == 30 * SECOND)
		{
			DecideWorst();
			mode = DONKETSU_DIRECTION;
		}
		if (timer <= 0)
		{
			mode = TIMEUP;
		}

		m_UI->SetTimer(timer);
		m_UI->Update();

		if (m_UI->GetTimer() <= 30 * SECOND)		donketsuBoostState = true;
		else	donketsuBoostState = false;

		m_UI->SetDonketsuBoostState(donketsuBoostState);
	}

	//	メインゲーム情報描画
	void	GameManager::MainGameInfoRender( void )
	{
		m_UI->Render();

		if ( !debug )	return;
		//	デバッグ用
		char	str[256];
		int		maxCoin = 0;
		for ( int i = 0; i < 4; i++ )
		{
			sprintf_s( str, "p%d_coin = %d", i + 1, coinNum[i] );
			DrawString( str, 20, 150 + i * 30 );
		}

		if ( donketsuBoostState )
		{
			DrawString( "どんけつブーストなう", 600, 250 );
		}
	}

	//------------------------------------------------------------------
	//	どんけつ演出
	//------------------------------------------------------------------
	//　更新
	void	GameManager::DonketsuDirectionUpdate(void)
	{
		static int wait(5 * SECOND);
		wait--;

		if (wait <= 0)
		{
			wait = 30;
			mode = MAINGAME;
		}
	}

	//　描画
	void	GameManager::DonketsuDirectionRender(void)
	{
		char	str[256];
		DrawString("どんけつ演出", 200, 50);
		wsprintf(str, "ビリは p%d", worst+1);
		DrawString(str, 200, 70);
	}
	
	//------------------------------------------------------------------
	//	タイムアップ演出
	//------------------------------------------------------------------
	//　更新
	void	GameManager::TimeUpUpdate(void)
	{
		waitTimer--;

		if (waitTimer <= 0)
		{
			MainFrame->ChangeScene(new sceneResult());
			return;
		}
	}

	//	描画
	void	GameManager::TimeUpRender( void )
	{
		m_UI->Render();

		if ( !debug )	return;
		//	デバッグ用
		char	str[256];
		int		maxCoin = 0;
		for ( int i = 0; i < 4; i++ )
		{
			sprintf_s( str, "p%d_coin = %d", i + 1, coinNum[i] );
			DrawString( str, 20, 150 + i * 30 );
		}

		DrawString( "TimeUp!!", 600, 350, Vector3( 1.0f, 1.0f, 0.0f ) );
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
	void		GameManager::DecideWorst(void)
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

	//	シーン切り替えフラグ取得
	bool	GameManager::GetChangeSceneFlag( void )
	{
		return	changeSceneflag;
	}

	//	プレイヤー人数取得
	int		GameManager::GetPlayerNum( void )
	{
		return	playerNum;
	}

	//	プレイヤータイプ取得
	int		GameManager::GetCharacterType( int num )
	{
		return	charatype[num];
	}

	//	ステージタイプ取得
	int		GameManager::GetStageType( void )
	{
		return	stageType;
	}

	//	コイン情報取得
	int		GameManager::GetCoinNum( int num )
	{
		return	coinNum[num];
	}

	//　どんけつ中かどうかを取得
	bool	GameManager::GetDonketsuBoostState( void )
	{
		return	donketsuBoostState;
	}

	//　ビリを取得
	int		GameManager::GetWorst( void )
	{
		return	worst;
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