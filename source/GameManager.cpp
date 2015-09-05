
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
	
	//	static変数
	int		GameManager::charatype[4] = { 0, 0, 0, 0 };
	int		GameManager::coinNum[4] = { 0, 0, 0, 0 };
	int		GameManager::playerNum = 0;
	int		GameManager::stageType = 0;
	int		GameManager::waitTimer = 0;
	bool	GameManager::changeSceneflag = false; 
	bool	GameManager::donketsuBoostState = false;
	int		GameManager::timer = 0;
	int		GameManager::mode = 0;
	int		GameManager::lastBonus = 0;
	bool	GameManager::newsflag = false;
	NewsBar	GameManager::newsbar;


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

		//	ラストボーナス設定
		lastBonus = rand() % 4;

		//	ニュースバー初期化
		{
			newsflag = false;
			newsbar.left = 1280;
			newsbar.top = 0;
			newsbar.right = 1280;
			newsbar.bottom = 50;
			newsbar.text = GameInfo::NewsText[lastBonus];
			newsbar.alpha = 0.5f;
			newsbar.color = Vector3( 0.3f, 0.3f, 0.3f );
			newsbar.step = 0;
			newsbar.textleft = 1500;
		}
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

		case TIMEUP:
			TimeUpRender();
			break;
		}
	}

	//	メインゲーム情報描画
	void	GameManager::MainGameInfoRender( void )
	{
		m_UI->Render();
		NewsRender();

		if ( !debug )	return;
		//	デバッグ用
		char	str[256];
		for ( int i = 0; i < 4; i++ )
		{
			sprintf_s( str, "p%d_coin = %d", i + 1, coinNum[i] );
			DrawString( str, 20, 150 + i * 30 );
		}

		if ( donketsuBoostState )
			DrawString( "どんけつブーストなう", 600, 250 );
	}

	//	タイムアップ描画
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

	//	ニュース描画
	void	GameManager::NewsRender( void )
	{
		if ( newsflag )
		{
			iexPolygon::Rect( newsbar.left, newsbar.top, newsbar.right - newsbar.left, newsbar.bottom - newsbar.top, RS_COPY, GetColor( newsbar.color, newsbar.alpha ) );
			IEX_DrawText( newsbar.text, newsbar.textleft, newsbar.top + 10, 500, 200, 0xFFFFFFFF );
		}
	}

//-------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------

	//	メインゲーム更新
	void	GameManager::MainGameUpdate( void )
	{
		//	タイマー更新
		timer--;
		if ( timer <= 0 )	mode = TIMEUP;
		m_UI->SetTimer( timer );
		m_UI->Update();

		//	どんけつブースト設定
		if ( m_UI->GetTimer() <= 30 * SECOND )		donketsuBoostState = true;
		else	donketsuBoostState = false;
		m_UI->SetDonketsuBoostState( donketsuBoostState );

		//	ニュース設定
		{
			if ( timer == 1 * MINUTE )	newsflag = true;
			if ( newsflag )	SetLastBonusNews();
		}
	}

	//	タイムアップ更新
	void	GameManager::TimeUpUpdate( void )
	{
		waitTimer--;

		if ( waitTimer <= 0 )
		{
			MainFrame->ChangeScene( new sceneResult() );
			return;
		}
	}

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

	//	ラストボーナスニュース
	void	GameManager::SetLastBonusNews( void )
	{
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
			newsflag = false;
			newsbar.left = 1280;
			newsbar.right = 1280;
			newsbar.textleft = 1500;
			newsbar.step = 0;
			break;
		}
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

	//	ラストボーナス取得
	int		GameManager::GetLastBonus( void )
	{
		return	lastBonus;
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
