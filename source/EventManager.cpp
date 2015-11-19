
#include	"iextreme.h"
#include	"Random.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Sound.h"
#include	"Screen.h"
#include	"Camera.h"
#include	"Particle.h"
#include	"CoinManager.h"
#include	"CharacterManager.h"

#include	"EventManager.h"

//*******************************************************************************
//
//	EventManagerクラス
//
//*******************************************************************************

//--------------------------------------------------------------------------------
//	グローバル
//--------------------------------------------------------------------------------

#define	SLOPE_COUNT_MAX	300
#define	COIN_GETAWAY_COUNT_MAX	600

//--------------------------------------------------------------------------------
//	初期化・解放
//--------------------------------------------------------------------------------

	//	初期化
	bool	EventManager::Initialize( void )
	{
		//	変数初期化
		eventflag = EVENT_MODE::JAM_NONE;

		//	構造体初期化
		{
			//-------------------------------------------
			//　妨害イベント関係
			//-------------------------------------------

			//	画面傾きイベント初期化
			{
				event_Jam_Slope.step = 0;
				event_Jam_Slope.count = 0;
				event_Jam_Slope.param = 0.0f;
				event_Jam_Slope.slope = 0.0f;
				event_Jam_Slope.eventFlag = false;
			}

			//	コイン逃走イベント初期化
			{
				event_Jam_CoinGetAway.step = 0;
				event_Jam_CoinGetAway.count = 0;
				event_Jam_CoinGetAway.eventflag = false;
		}

			//	スリップイベント初期化
			{
				event_Jam_Slip.step = 0;
				event_Jam_Slip.count = 0;
				event_Jam_Slip.eventflag = false;
			}

			//-------------------------------------------
			//　コインイベント関係
			//-------------------------------------------

			//　コイン袋
			{
				event_Coin_Sack.eventflag = false;
				event_Coin_Sack.count = 0;
				event_Coin_Sack.step = 0;
			}

			//　コインの滝
			{
				event_Coin_Fall.eventflag = false;
				event_Coin_Fall.count = 0;
				event_Coin_Fall.step = 0;
			}

			//　コインワッシャァア
			{
				event_Coin_Splash.eventflag = false;
				event_Coin_Splash.count = 0;
				event_Coin_Splash.step = 0;
			}

			//　宝石箱
			{
				event_Coin_Juelbox.eventflag = false;
				event_Coin_Juelbox.count = 0;
				event_Coin_Juelbox.step = 0;
			}

			//　コインのウェーブ
			{
				event_Coin_Wave.eventflag = false;
				event_Coin_Wave.count = 0;
				event_Coin_Wave.step = 0;
			}

			//　コイン２倍
			{
				event_Coin_Dubble.eventflag = false;
				event_Coin_Dubble.count = 0;
				event_Coin_Dubble.step = 0;
			}
		}

		return	true;
	}

	//	解放
	void	EventManager::Release( void )
	{

	}

//--------------------------------------------------------------------------------
//	更新・描画
//--------------------------------------------------------------------------------

	//	更新
	void	EventManager::Update( void )
	{
		//	カメラ傾き
		if (event_Jam_Slope.eventFlag)		Event_Jam_Slope();

		//	コイン逃走
		if (event_Jam_CoinGetAway.eventflag)	Event_Jam_CoinGetAway();

		//	スリップ
		if (event_Jam_Slip.eventflag)		Event_Jam_Slip();
	}

	//	描画
	void	EventManager::Render( void )
	{

	}

//--------------------------------------------------------------------------------
//	動作関数
//--------------------------------------------------------------------------------

	//	画面が傾く
	void	EventManager::Event_Jam_Slope(void)
	{
		//	パラメータ準備
		bool	isEnd;
		static	float	slopeAngle = 0.0f;

		switch ( event_Jam_Slope.step )
		{
		case 0:
			//	初期化
			slopeAngle = Random::GetFloat( D3DX_PI / 180.0f * -90.0f, D3DX_PI / 180.0f * 90.0f );
			event_Jam_Slope.step++;
			break;

		case 1:
			//	パラメータ加算
			event_Jam_Slope.param += 0.01f;

			//	パラメータ限度設定
			if ( event_Jam_Slope.param >= 1.0f )
			{
				event_Jam_Slope.param = 1.0f;
			}

			//	画面を４５度傾ける
			isEnd = CubicFunctionInterpolation( event_Jam_Slope.slope, 0.0f, slopeAngle, event_Jam_Slope.param );

			//	カメラへ設定
			mainView->SetSlope( event_Jam_Slope.slope );

			//	傾き終わると次のステップへ
			if ( isEnd )
			{
				event_Jam_Slope.step++;
			}
			break;

		case 2:
			//	数秒待つ
			event_Jam_Slope.count++;

			if ( event_Jam_Slope.count >= SLOPE_COUNT_MAX )
			{
				event_Jam_Slope.count = 0;
				event_Jam_Slope.param = 0.0f;
				event_Jam_Slope.step++;
			}
			break;

		case 3:
			//	パラメータ加算
			event_Jam_Slope.param += 0.01f;

			//	パラメータ限度設定
			if ( event_Jam_Slope.param >= 1.0f )
			{
				event_Jam_Slope.param = 1.0f;
			}

			//	画面を傾ける
			isEnd = CubicFunctionInterpolation( event_Jam_Slope.slope, slopeAngle, 0.0f, event_Jam_Slope.param );
			
			//	カメラへ設定
			mainView->SetSlope( event_Jam_Slope.slope );
			
			//	傾き終わると次のステップへ
			if ( isEnd )
			{
				event_Jam_Slope.step = 0;
				event_Jam_Slope.eventFlag = false;
				event_Jam_Slope.slope = 0.0f;
				eventflag = EVENT_MODE::JAM_NONE;
			}
			break;
		}
	} 

	//	コイン逃走
	void	EventManager::Event_Jam_CoinGetAway(void)
	{
		switch ( event_Jam_CoinGetAway.step )
		{
		case 0:
			//	コインの動作切り替え
			m_CoinManager->SetCoinGetAwayFlag( true );
			event_Jam_CoinGetAway.step++;
			break;

		case 1:
			event_Jam_CoinGetAway.count++;
			if ( event_Jam_CoinGetAway.count >= COIN_GETAWAY_COUNT_MAX )
			{
				event_Jam_CoinGetAway.step++;
				event_Jam_CoinGetAway.count = 0;
			}
			break;

		case 2:
			m_CoinManager->SetCoinGetAwayFlag( false );
			event_Jam_CoinGetAway.step = 0;
			event_Jam_CoinGetAway.eventflag = false;
			eventflag = EVENT_MODE::JAM_NONE;
			break;
		}
	}

	//	スリップ
	void	EventManager::Event_Jam_Slip(void)
	{
		switch ( event_Jam_Slip.step )
		{
		case 0:
			for ( int i = 0; i < PLAYER_MAX; i++ )
			{
				characterManager->SetParameterInfo( i, PARAMETER_STATE::SLIP );
			}
			event_Jam_Slip.step++;
			break;

		case 1:
			event_Jam_Slip.eventflag = false;
			event_Jam_Slip.count = 0;
			event_Jam_Slip.step = 0;
			break;
		}
	}
	
//--------------------------------------------------------------------------------
//	情報取得
//--------------------------------------------------------------------------------

	//	実体取得
	EventManager*	EventManager::GetInstance( void )
	{
		static	EventManager	out;
		return	&out;
	}

	//	発動中のイベントを取得
	int		EventManager::GetEvent( void )const
	{
		return	eventflag;
	}

//--------------------------------------------------------------------------------
//	情報設定
//--------------------------------------------------------------------------------

	//	イベントフラグ設定
	void	EventManager::SetEvent( int eventflag )
	{
		this->eventflag = eventflag;
		switch ( eventflag )
		{
		case EVENT_MODE::JAM_SLOPE_CAMERA:
			event_Jam_Slope.eventFlag = true;
			break;

		case EVENT_MODE::JAM_SLIP:
			event_Jam_Slip.eventflag = true;
			break;

		case EVENT_MODE::JAM_FALL_BOMB:
			break;

		case EVENT_MODE::JAM_UFO:
			break;

		case EVENT_MODE::JAM_COIN_GETAWAY:
			event_Jam_CoinGetAway.eventflag = true;
			break;

		case EVENT_MODE::COIN_SACK:

			break;

		case EVENT_MODE::COIN_FALL: 
			break;

		case EVENT_MODE::COIN_SPLASH: 
			break;

		case EVENT_MODE::COIN_JUELBOX: 
			break;

		case EVENT_MODE::COIN_WAVE: 
			break;

		case EVENT_MODE::COIN_DUBBLE: 
			break;

		}
	}