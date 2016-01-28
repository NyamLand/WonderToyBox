
#include	"iextreme.h"
#include	"Sound.h"

//*******************************************************************************
//
//	Soundクラス
//
//*******************************************************************************

//---------------------------------------------------------------------------
//	グローバル
//---------------------------------------------------------------------------

	namespace
	{
		namespace
		{
			const		LPSTR	bgmfile[] =
			{
				"DATA/SOUND/BGM/TitleBGM.ogg",	//	Title
				"DATA/SOUND/BGM/MenuBGM.ogg",	//	Menu
				"DATA/SOUND/BGM/FightBGM.ogg",	//	Main
				"DATA/SOUND/BGM/FinalRoundBGM.ogg",	//	FAINALROUND
				"DATA/SOUND/BGM/ResultBGM.ogg",	//	Result
			};
		}
	}

//---------------------------------------------------------------------------
//	初期化
//---------------------------------------------------------------------------

	//	コンストラクタ
	Sound::Sound( void )
	{
	
	}

	//	デストラクタ
	Sound::~Sound( void )
	{

	}

	//	初期化
	void	Sound::Initialize( void )
	{
		IEX_SetWAV( SE::HYPER_SE, "DATA/Sound/SE/attack-h.wav" );
		IEX_SetWAV( SE::POWER_SE, "DATA/Sound/SE/attack-p.wav" );
		IEX_SetWAV( SE::QUICK_SE, "DATA/Sound/SE/attack-q.wav" );
		IEX_SetWAV( SE::COIN_SE, "DATA/Sound/SE/coin.wav" );
		IEX_SetWAV( SE::ITEM_SE, "DATA/Sound/SE/item-get.wav" );
		IEX_SetWAV( SE::DECIDE_SE, "DATA/Sound/SE/decide.wav" );
		IEX_SetWAV(SE::EVENT_SE, "DATA/Sound/SE/event.wav");
		IEX_SetWAV( SE::NEWS_SE, "DATA/Sound/SE/decision3.wav" );
		IEX_SetWAV(SE::HIT_SE, "DATA/Sound/SE/attack-p.wav");
		IEX_SetWAV( SE::DRAMROLL_SE, "DATA/Sound/SE/dramroll.wav" );
		IEX_SetWAV( SE::DEATH_SE, "DATA/Sound/SE/death.wav" );
		IEX_SetWAV( SE::GAMESTART_SE, "DATA/Sound/SE/GameStart.wav" );
		IEX_SetWAV( SE::RESULT_JAN, "DATA/Sound/SE/result-jan.wav" );
		IEX_SetWAV(SE::GAMESET_SE, "DATA/Sound/SE/GameSet.wav");
		IEX_SetWAV(SE::GUARD_SE, "DATA/Sound/SE/guard.wav" );
		IEX_SetWAV(SE::CHOICE_SE, "DATA/Sound/SE/choice.wav");

		isPlay = false;
	}

//---------------------------------------------------------------------------
//	動作関数
//---------------------------------------------------------------------------

	//	SE再生
	void	Sound::PlaySE( int type )
	{
		IEX_PlaySound( type, false );
	}

	//	SE停止
	void	Sound::StopSE( int type )
	{
		IEX_StopSound( type );
	}

	//	BGM再生
	void	Sound::PlayBGM( int type )
	{
		stream = IEX_PlayStreamSound( bgmfile[ type ] );
		isPlay = true;
	}

	//	BGM停止
	void	Sound::StopBGM( void )
	{
		if ( isPlay )
		{
			IEX_StopStreamSound( stream );
			isPlay = false;
			
		}
	}

	//	全停止
	void	Sound::AllStop( void )
	{
		//	SE停止
		for ( int i = 0; i < SE::END; i++ )	IEX_StopSound( i );
		
		//	BGM停止
		StopBGM();
	}

//---------------------------------------------------------------------------
//	情報取得
//---------------------------------------------------------------------------

	//	SE再生状態取得
	bool	Sound::GetSEState( int type )
	{
		BOOL state = IEX_GetSoundStatus( type );

		if ( state )	return	true;
		return	false;
	}

	//	BGM再生状態取得
	bool	Sound::GetBGMState( void )
	{
		bool	out = isPlay;
		return	out;
	}

	//	実体取得
	Sound*	Sound::GetInstance( void )
	{
		static	Sound	out;
		return	&out;
	}









