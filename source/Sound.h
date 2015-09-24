
#ifndef __SOUND_H__
#define	__SOUND_H__

//*********************************************************************************
//
//	Sound�N���X
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

//	�N���X
class Sound
{
private:
	LPDSSTREAM		stream;
	bool					isPlay;

public:
	//	�������E���
	Sound( void );
	~Sound( void );

	//	�o�^
	void	Initialize( void );

	//	SE�Đ�
	void	PlaySE( int type );
	void	StopSE( int type );

	//	BGM�Đ�
	void	PlayBGM( int type );
	void	StopBGM( void );
	void	AllStop( void );

	//	���擾
	bool	GetSEState( int type );
	bool	GetBGMState( void );
	static	Sound*	GetInstance( void );
};

#define	sound ( Sound::GetInstance() )

//*********************************************************************************
#endif // !__SOUND_H__
