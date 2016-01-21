
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
#include	"Event_Coin.h"
#include	"Event_Jamming.h"
#include	"Event_Mission.h"

#include	"EventManager.h"

//*******************************************************************************
//
//	EventManagerクラス
//
//*******************************************************************************

//--------------------------------------------------------------------------------
//	グローバル
//--------------------------------------------------------------------------------
bool EventManager::eventFlag = false;
int  EventManager::count = 0;
int  EventManager::state = EVENT_STATE::INTRO;

//--------------------------------------------------------------------------------
//	初期化・解放
//--------------------------------------------------------------------------------

	//	初期化
	bool	EventManager::Initialize( void )
	{
		//	変数初期化
		count = 0;
		eventMode = EVENT_MODE::NONE;
		eventFlag = false;
		
		//　各種イベントパラメータ初期化
		event_coin->Initialize();
		event_jamming->Initialize();
		event_mission->Initialize();

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
		event_coin->Update();
		event_jamming->Update();
		event_mission->Update();
	}

	//	描画
	void	EventManager::Render( void )
	{
		
		//RenderImage()

	}

//--------------------------------------------------------------------------------
//	動作関数
//--------------------------------------------------------------------------------
	//　フラグ・状態などを設定
	void	EventManager::CommonSetting(bool& eventflag, int& step, int countMax)
	{
		if (count >= FLYING_COUNT)	state = EVENT_STATE::ACTIVE;
		if (count >= countMax - FLYING_COUNT)	state = EVENT_STATE::END;

		//　リセット
		if (count >= countMax)
		{
			eventflag = false;
			step = 0;

			count = 0;
			state = EVENT_STATE::INTRO;
			return;
		}

		count++;
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
		return	eventMode;
	}

	//　イベント発動中かどうかのフラグを取得
	bool	EventManager::GetEventFlag(void)const
	{
		return	eventFlag;
	}

	//　状態取得
	int		EventManager::GetState(void)const
	{
		return state;
	}

//--------------------------------------------------------------------------------
//	情報設定
//--------------------------------------------------------------------------------

	//	イベントモード設定
	void	EventManager::SetEvent( int eventMode )
	{
		this->eventMode = eventMode;
		if (EVENT_MODE::JAM_SLOPE_CAMERA <= eventMode && eventMode <= EVENT_MODE::JAM_SLIP)	event_jamming->SetEvent(eventMode);
		else if (EVENT_MODE::COIN_FALL <= eventMode && eventMode <= EVENT_MODE::COIN_DUBBLE)	event_coin->SetEvent(eventMode);
		else event_mission->SetEvent(eventMode);
	}

	//	イベントフラグ設定
	void	EventManager::SetEventFlag(bool eventFlag)
	{
		this->eventFlag = eventFlag;
	}