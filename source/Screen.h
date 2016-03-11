
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

//	include
#include	"iextreme.h"

//	class
class Screen
{
private:
	//	�p�����[�^
	Vector3	color;
	float			wipeSize;
	float			param;
	float			alpha;
	float			speed;
	float			widthHalf;
	float			heightHalf;
	float			width;
	float			height;		
	bool			screenState;
	int			size;
	int			mode;

private:
	//	�������E���
	Screen( void );
	~Screen( void );

public:
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
	float GetWidthFloat( void )const;
	float	GetWidthHalfFloat( void )const;
	float	GetHeightFloat( void )const;
	float	GetHeightHalfFloat( void )const;
	bool	GetScreenState( void )const;
	int	GetWidthInt( void )const;
	int	GetWidthHalfInt( void )const;
	int	GetHeightInt( void )const;
	int	GetHeightHalfInt( void )const;
	static	Screen*	GetInstance( void );
};

#define	screen ( Screen::GetInstance() )