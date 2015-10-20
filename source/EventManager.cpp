
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
		eventflag = EVENT_MODE::NONE;

		//	構造体初期化
		{
			//	画面傾きイベント初期化
			{
				eventSlope.step = 0;
				eventSlope.count = 0;
				eventSlope.param = 0.0f;
				eventSlope.slope = 0.0f;
				eventSlope.eventFlag = false;
			}

			//	コイン逃走イベント初期化
			{
				eventCoinGetAway.step = 0;
				eventCoinGetAway.count = 0;
				eventCoinGetAway.eventflag = false;
			}

			//	スリップイベント初期化
			{
				eventSlip.step = 0;
				eventSlip.count = 0;
				eventSlip.eventflag = false;
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
		if ( eventSlope.eventFlag )		EventSlope();

		//	コイン逃走
		if ( eventCoinGetAway.eventflag )	EventCoinGetAway();

		//	スリップ
		if ( eventSlip.eventflag )		EventSlip();
	}

	//	描画
	void	EventManager::Render( void )
	{

	}

//--------------------------------------------------------------------------------
//	動作関数
//--------------------------------------------------------------------------------

	//	画面が傾く
	void	EventManager::EventSlope( void )
	{
		//	パラメータ準備
		bool	isEnd;
		static	float	slopeAngle = 0.0f;

		switch ( eventSlope.step )
		{
		case 0:
			//	初期化
			slopeAngle = Random::GetFloat( D3DX_PI / 180.0f * -90.0f, D3DX_PI / 180.0f * 90.0f );
			eventSlope.step++;
			break;

		case 1:
			//	パラメータ加算
			eventSlope.param += 0.01f;

			//	パラメータ限度設定
			if ( eventSlope.param >= 1.0f )
			{
				eventSlope.param = 1.0f;
			}

			//	画面を４５度傾ける
			isEnd = CubicFunctionInterpolation( eventSlope.slope, 0.0f, slopeAngle, eventSlope.param );

			//	カメラへ設定
			mainView->SetSlope( eventSlope.slope );

			//	傾き終わると次のステップへ
			if ( isEnd )
			{
				eventSlope.step++;
			}
			break;

		case 2:
			//	数秒待つ
			eventSlope.count++;

			if ( eventSlope.count >= SLOPE_COUNT_MAX )
			{
				eventSlope.count = 0;
				eventSlope.param = 0.0f;
				eventSlope.step++;
			}
			break;

		case 3:
			//	パラメータ加算
			eventSlope.param += 0.01f;

			//	パラメータ限度設定
			if ( eventSlope.param >= 1.0f )
			{
				eventSlope.param = 1.0f;
			}

			//	画面を傾ける
			isEnd = CubicFunctionInterpolation( eventSlope.slope, slopeAngle, 0.0f, eventSlope.param );
			
			//	カメラへ設定
			mainView->SetSlope( eventSlope.slope );
			
			//	傾き終わると次のステップへ
			if ( isEnd )
			{
				eventSlope.step = 0;
				eventSlope.eventFlag = false;
				eventSlope.slope = 0.0f;
				eventflag = EVENT_MODE::NONE;
			}
			break;
		}
	} 

	//	コイン逃走
	void	EventManager::EventCoinGetAway( void )
	{
		switch ( eventCoinGetAway.step )
		{
		case 0:
			//	コインの動作切り替え
			m_CoinManager->SetCoinGetAwayFlag( true );
			eventCoinGetAway.step++;
			break;

		case 1:
			eventCoinGetAway.count++;
			if ( eventCoinGetAway.count >= COIN_GETAWAY_COUNT_MAX )
			{
				eventCoinGetAway.step++;
				eventCoinGetAway.count = 0;
			}
			break;

		case 2:
			m_CoinManager->SetCoinGetAwayFlag( false );
			eventCoinGetAway.step = 0;
			eventCoinGetAway.eventflag = false;
			eventflag = EVENT_MODE::NONE;
			break;
		}
	}

	//	スリップ
	void	EventManager::EventSlip( void )
	{
		switch ( eventSlip.step )
		{
		case 0:
			for ( int i = 0; i < PLAYER_MAX; i++ )
			{
				characterManager->SetParameterInfo( i, PARAMETER_STATE::SLIP );
			}
			eventSlip.step++;
			break;

		case 1:
			eventSlip.eventflag = false;
			eventSlip.count = 0;
			eventSlip.step = 0;
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
		case EVENT_MODE::SLOPE_CAMERA:
			eventSlope.eventFlag = true;
			break;

		case	EVENT_MODE::SLIP:
			eventSlip.eventflag = true;
			break;

		case EVENT_MODE::FALL_BOMB:
			break;

		case EVENT_MODE::UFO:
			break;

		case EVENT_MODE::COIN_GETAWAY:
			eventCoinGetAway.eventflag = true;
			break;
		}
	}