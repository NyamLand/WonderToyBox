
#pragma	once

//*******************************************************************************
//
//	Screen�N���X
//
//*******************************************************************************

namespace
{
	namespace SCREEN_MODE
	{
		enum
		{
			FADE_IN,
			FADE_OUT,
			WHITE_IN,
			WHITE_OUT,
			WIPE_IN,
			WIPE_OUT,
			END,
		};
	}
}

//	class
class Screen
{
private:
	int			mode;
	float		alpha;
	float		speed;
	bool		screenState;
	int			size;
	float		wipeSize;
	Vector3	color;

public:
	//	�������E���
	bool	Initialize( void );
	void	Release( void );

	//	�X�V�E�`��
	bool	Update( void );
	void	Render( void );

	//	����֐�
	bool	FadeIn( void );
	bool	FadeOut( void );
	bool	WipeIn( void );
	bool	WipeOut( void );

	//	���ݒ�
	void	SetScreenMode( int mode, float speed );

	//	���擾
	bool	GetScreenState( void )const;
	static	Screen*	GetInstance( void );
};

#define	screen ( Screen::GetInstance() )