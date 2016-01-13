
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



//--------------------------------------------------------------------------------
//	初期化・解放
//--------------------------------------------------------------------------------

	//	初期化
	bool	EventManager::Initialize( void )
	{
		//	変数初期化
		eventflag = EVENT_MODE::NONE;
		
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
		if (EVENT_MODE::JAM_SLOPE_CAMERA <= eventflag && eventflag <= EVENT_MODE::JAM_UFO)	event_jamming->SetEvent(eventflag);
		else if (EVENT_MODE::COIN_SACK <= eventflag && eventflag <= EVENT_MODE::COIN_DUBBLE)	event_coin->SetEvent(eventflag);
		else event_mission->SetEvent(eventflag);
	}