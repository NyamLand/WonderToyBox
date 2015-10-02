
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
	bool		screenState;
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

	//	���ݒ�
	void	SetScreenMode( int mode );

	//	���擾
	bool	GetScreenState( void )const;
	static	Screen*	GetInstance( void );
};

#define	screen ( Screen::GetInstance() )