
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Sound.h"
#include	"Screen.h"
#include	"Camera.h"
#include	"Particle.h"

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

//--------------------------------------------------------------------------------
//	初期化・解放
//--------------------------------------------------------------------------------

	//	初期化
	bool	EventManager::Initialize( void )
	{
		//	変数初期化
		mode = EVENT_MODE::NONE;

		//	構造体初期化
		{
			//	画面傾きイベント初期化
			{
				eventSlope.step = 0;
				eventSlope.count = 0;
				eventSlope.param = 0.0f;
				eventSlope.slope = 0.0f;
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

		switch ( eventSlope.step )
		{
		case 0:
			//	パラメータ加算
			eventSlope.param += 0.01f;

			//	パラメータ限度設定
			if ( eventSlope.param >= 1.0f )
			{
				eventSlope.param = 1.0f;
			}

			//	画面を４５度傾ける
			isEnd = CubicFunctionInterpolation( eventSlope.slope, 0.0f, D3DX_PI / 180.0f * 45.0f, eventSlope.param );

			//	カメラへ設定
			m_Camera->SetSlope( eventSlope.slope );

			//	傾き終わると次のステップへ
			if ( isEnd )	eventSlope.step++;
			break;

		case 1:
			//	数秒待つ
			eventSlope.count++;

			if ( eventSlope.count >= SLOPE_COUNT_MAX )
			{
				eventSlope.count = 0;
				eventSlope.param = 0.0f;
				eventSlope.step++;
			}
			break;

		case 2:
			//	パラメータ加算
			eventSlope.param += 0.01f;

			//	パラメータ限度設定
			if ( eventSlope.param >= 1.0f )
			{
				eventSlope.param = 1.0f;
			}

			//	画面を４５度傾ける
			isEnd = CubicFunctionInterpolation( eventSlope.slope, D3DX_PI / 180.0f * 45.0f, 0.0f, eventSlope.param );
			
			//	カメラへ設定
			m_Camera->SetSlope( eventSlope.slope );
			
			//	傾き終わると次のステップへ
			if ( isEnd )
			{
				eventSlope.step = 0;
				eventSlope.eventFlag = false;
				eventSlope.slope = 0.0f;
				mode = EVENT_MODE::NONE;
			}
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
		return	mode;
	}

//--------------------------------------------------------------------------------
//	情報設定
//--------------------------------------------------------------------------------

	//	イベントフラグ設定
	void	EventManager::SetEvent( int eventflag )
	{
		switch ( eventflag )
		{
		case EVENT_MODE::SLOPE_CAMERA:
			eventSlope.eventFlag = true;
			break;

		case	EVENT_MODE::SLIP:
			break;

		case EVENT_MODE::FALL_BOMB:
			break;

		case EVENT_MODE::UFO:
			break;

		case EVENT_MODE::COIN_GETAWAY:
			break;
		}
	}