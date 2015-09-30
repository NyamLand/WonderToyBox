
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
			NEWS_SE,	
			HYPER_SE,
			POWER_SE,
			QUICK_SE,
			COIN_SE,
			DECIDE_SE,
			SELECT_SE,
			CANCEL_SE,
			END,
		};
	}

	namespace BGM
	{
		enum BGM_TYPE
		{
			TITLE_BGM,
			MAIN_BGM,
			RESULT_BGM,
		};
	}
}

//	クラス
class Sound
{
private:
	LPDSSTREAM		stream;
	bool					isPlay;

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
