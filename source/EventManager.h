
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
			SLIP,
			FALL_BOMB,
			COIN_GETAWAY,
			UFO,
			NONE,
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
private:
	int		mode;
	EVENT_SLOPE	eventSlope;

public:
	//	�������E���
	bool	Initialize( void );
	void	Release( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	����֐�
	void	EventSlope( void );

	//	���擾
	static	EventManager*	GetInstance( void );
	int		GetEvent( void )const;

	//	���ݒ�
	void	SetEvent( int eventflag );
};

#define	eventManager	( EventManager::GetInstance() )