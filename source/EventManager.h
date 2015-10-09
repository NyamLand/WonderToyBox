
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
			SLOPE_CAMERA,
			COIN_GETAWAY,
			NONE,
			SLIP,
			FALL_BOMB,
			UFO,
		};
	}
}

//	class
class EventManager
{
private:
	struct EVENT_SLOPE
	{
		bool	eventFlag;
		int		count;
		float	param;
		float	slope;
		int		step;
	};
	struct EVENT_COIN_GETAWAY
	{
		bool	eventflag;
		int		count;
		int		step;
	};
private:
	int		eventflag;
	EVENT_SLOPE				eventSlope;
	EVENT_COIN_GETAWAY	eventCoinGetAway;

public:
	//	�������E���
	bool	Initialize( void );
	void	Release( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	����֐�
	void	EventSlope( void );
	void	EventCoinGetAway( void );

	//	���擾
	static	EventManager*	GetInstance( void );
	int		GetEvent( void )const;

	//	���ݒ�
	void	SetEvent( int eventflag );
};

#define	eventManager	( EventManager::GetInstance() )