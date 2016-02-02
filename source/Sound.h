
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
			NEWS_SE,	//	�j���[�XSE
			HYPER_SE,	//	�n�C�p�[������SE
			POWER_SE,	//	�p���[������SE
			QUICK_SE,	//	�N�C�b�N������SE
			COIN_SE,		//	�R�C��SE
			ITEM_SE,		//	�A�C�e��SE
			DECIDE_SE,	//	���莞SE
			SELECT_SE,	//	�J�[�\���ړ�SE
			CANCEL_SE,//	�L�����Z��SE
			EVENT_SE,	//	�C�x���gSE
			HIT_SE,		//	�q�b�gSE
			DRAMROLL_SE,	//	��������������SE
			RESULT_JAN,		//	���\��SE
			GAMESTART_SE,
			GAMESET_SE,	//	�Q�[���Z�b�gSE
			DEATH_SE,
			CHOICE_SE,		//	�I��SE
			GUARD_SE,		//	�K�[�h��SE
			QUICK_HIT_SE,
			POWER_HIT_SE,
			HYPER_HIT_SE,
			HYPER_ATTACK,

			//	�U��SE
			MAJO_QUICK_START,
			MAJO_QUICK_BOMB,
			MAJO_POWER,
			MAJO_HYPER,
			PRINCESS_QUICK,
			PRINCESS_POWER,
			PRINCESS_HYPER,
			KAITO_QUICK,
			KAITO_POWER,
			KAITO_HYPER,
			PIRATE_QUICK_MISS,
			PIRATE_QUICK_SUCCESS,
			PIRATE_QUICK_BOMB,
			PIRATE_POWER,
			PIRATE_HYPER,

			END,		//	�I�[
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

//	�N���X
class Sound
{
private:
	LPDSSTREAM		stream;
	bool						isPlay;

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
