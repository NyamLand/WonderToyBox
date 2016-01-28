
#ifndef __SOUND_H__
#define	__SOUND_H__

//*********************************************************************************
//
//	Soundクラス
//
//*********************************************************************************

//	SoundInfo
namespace
{
	namespace SE
	{
		enum SE_TYPE
		{
			NEWS_SE,	//	ニュースSE
			HYPER_SE,	//	ハイパー発動時SE
			POWER_SE,	//	パワー発動時SE
			QUICK_SE,	//	クイック発動時SE
			COIN_SE,		//	コインSE
			ITEM_SE,		//	アイテムSE
			DECIDE_SE,	//	決定時SE
			SELECT_SE,	//	カーソル移動SE
			CANCEL_SE,//	キャンセルSE
			EVENT_SE,	//	イベントSE
			HIT_SE,		//	ヒットSE
			DRAMROLL_SE,	//	ｄｒｒｒｒｒｒSE
			RESULT_JAN,		//	発表時SE
			GAMESTART_SE,
			GAMESET_SE,	//	ゲームセットSE
			DEATH_SE,
			CHOICE_SE,		//	選択SE
			GUARD_SE,		//	ガード時SE

			END,		//	終端
		};
	}

	namespace BGM
	{
		enum BGM_TYPE
		{
			TITLE_BGM,
			MENU_BGM,
			MAIN_BGM,
			FAINLROUND_BGM,
			RESULT_BGM,
		};
	}
}

//	クラス
class Sound
{
private:
	LPDSSTREAM		stream;
	bool						isPlay;

public:
	//	初期化・解放
	Sound( void );
	~Sound( void );

	//	登録
	void	Initialize( void );

	//	SE再生
	void	PlaySE( int type );
	void	StopSE( int type );

	//	BGM再生
	void	PlayBGM( int type );
	void	StopBGM( void );
	void	AllStop( void );

	//	情報取得
	bool	GetSEState( int type );
	bool	GetBGMState( void );
	static	Sound*	GetInstance( void );
};

#define	sound ( Sound::GetInstance() )

//*********************************************************************************
#endif // !__SOUND_H__
