
#pragma	once

//*******************************************************************************
//
//	EventManager�N���X
//
//*******************************************************************************

//	include

//	data
namespace
{
	namespace EVENT_MODE
	{
		enum
		{
			JAM_SLOPE_CAMERA,
			JAM_COIN_GETAWAY,
			JAM_SLIP,
			JAM_FALL_BOMB,
			JAM_UFO,

			COIN_SACK,
			COIN_FALL,
			COIN_SPLASH,
			COIN_JUELBOX,
			COIN_WAVE,
			COIN_DUBBLE,

			MISSION_COLLECT,
			MISSION_STAY,

			MAX,
			NONE
		};
	}
}

//	class
class EventManager
{
	
private:
	int		eventMode;
	static bool	eventFlag;

public:
	//	�������E���
	bool	Initialize( void );
	void	Release( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	���擾
	static	EventManager*	GetInstance( void );
	int		GetEvent( void )const;
	bool	GetEventFlag( void )const;

	//	���ݒ�
	void	SetEvent( int eventmode );
	void	SetEventFlag( bool eventFlag );
};

#define	eventManager	( EventManager::GetInstance() )