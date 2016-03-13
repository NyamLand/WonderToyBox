
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Screen.h"
#include	"system\System.h"

//*******************************************************************************
//
//	Screen�N���X
//
//*******************************************************************************

//-----------------------------------------------------------------------------------
//	�O���[�o��
//-----------------------------------------------------------------------------------

#define	WIPE_MAX_SIZE	1000.0f
#define	ADVISORY_SPEED	0.01f		//	�������x

	namespace
	{
		namespace Color
		{
			const Vector3 WHITE( 1.0f, 1.0f, 1.0f );
			const Vector3 BLACK( 0.0f, 0.0f, 0.0f );
		}
		namespace Size
		{
			const int MAX = 255;
			const int MIN = 0;
		}
	}

//-----------------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Screen::Screen( void ) : color( Color::BLACK ),
		alpha( 1.0f ), speed( 1.0f ), wipeSize( 1.0f ), param( 0.0f ),
		screenState( false ),
		mode( SCREEN_MODE::FADE_IN ), size( Size::MIN )
	{
		//	�X�N���[���T�C�Y��ݒ�
		width = static_cast<float>( iexSystem::ScreenWidth );
		height = static_cast<float>( iexSystem::ScreenHeight );
		widthHalf = width * 0.5f;
		heightHalf = height * 0.5f;

		//	�e��ԏ����l�A�ŏI�l�ݒ�
		startParam[SCREEN_MODE::FADE_IN]		=		startParam[SCREEN_MODE::WHITE_IN]		=		startParam[SCREEN_MODE::WIPE_OUT] = 1.0f;
		startParam[SCREEN_MODE::FADE_OUT]	=		startParam[SCREEN_MODE::WHITE_OUT]	=		startParam[SCREEN_MODE::WIPE_IN] = 0.0f;
		endParam[SCREEN_MODE::FADE_OUT]		=		endParam[SCREEN_MODE::WHITE_OUT]	=		endParam[SCREEN_MODE::WIPE_IN] = 1.0f;
		endParam[SCREEN_MODE::FADE_IN]			=		endParam[SCREEN_MODE::WHITE_IN]		=		endParam[SCREEN_MODE::WIPE_OUT] = 0.0f;

		shader3D->SetValue( "screen_width", width );
		shader3D->SetValue( "screen_height", height );
	}

	//	�f�X�g���N�^
	Screen::~Screen( void )
	{

	}

//-----------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------

	//	�X�V
	bool	Screen::Update( void )
	{
		//	��Ԋ������Z
		PercentageUpdate( param, ADVISORY_SPEED * speed );

		//	�X�N���[����ԏ�����
		screenState = false;

		switch ( mode )
		{
		case SCREEN_MODE::FADE_IN:
		case SCREEN_MODE::WHITE_IN:
		case SCREEN_MODE::FADE_OUT:
		case SCREEN_MODE::WHITE_OUT:
			screenState = Fade();
			break;
		case SCREEN_MODE::WIPE_IN:
		case SCREEN_MODE::WIPE_OUT:
			screenState = Wipe();
		}

		return	screenState;
	}

	//	�`��
	void	Screen::Render( void )
	{
		switch ( mode )
		{
		case SCREEN_MODE::WIPE_IN:
		case SCREEN_MODE::WIPE_OUT:
			iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, shader3D, "WipeEffect", 0xFFFFFFFF );
			break;

		default:
			iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, RS_COPY, GetColor( color, alpha ) );
			break;
		}
	}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

	//	�t�F�[�h����
	bool	Screen::Fade( void )
	{
		//	���
		bool	isEnd = CubicFunctionInterpolation( alpha, startParam[mode], endParam[mode], param );

		return	isEnd;
	}

	//	���C�v����
	bool	Screen::Wipe( void )
	{
		//	���
		bool isEnd = CubicFunctionInterpolation( wipeSize, startParam[mode], endParam[mode], param );

		//	�V�F�[�_�[�ɒl�Z�b�g
		shader3D->SetValue( "effect_size", WIPE_MAX_SIZE * wipeSize );
		return	isEnd;
	}

//-----------------------------------------------------------------------------------
//	���ݒ�
//-----------------------------------------------------------------------------------

	//	�X�N���[�����[�h
	void	Screen::SetScreenMode( int mode, float speed )
	{
		this->mode = mode;
		this->speed = speed;
		this->screenState = false;
		param = 0.0f;
		switch ( this->mode )
		{
		case SCREEN_MODE::FADE_IN:
		case SCREEN_MODE::FADE_OUT:
			color = Color::BLACK;
			break;

		case SCREEN_MODE::WHITE_IN:
		case SCREEN_MODE::WHITE_OUT:
			color = Color::WHITE;
			break;

		default:
			break;
		}
	}

//-----------------------------------------------------------------------------------
//	���擾
//-----------------------------------------------------------------------------------

	//	�X�N���[������float�Ŏ擾
	float	Screen::GetWidthFloat( void )const
	{
		return	width;
	}

	//	�X�N���[�����̔�����float�Ŏ擾
	float	Screen::GetWidthHalfFloat( void )const
	{
		return	widthHalf;
	}

	//	�X�N���[��������float�Ŏ擾
	float	Screen::GetHeightFloat( void )const
	{
		return	height;
	}

	//	�X�N���[�������̔�����float�Ŏ擾
	float	Screen::GetHeightHalfFloat( void )const
	{
		return	heightHalf;
	}

	//	�X�N���[������int�Ŏ擾
	int	Screen::GetWidthInt( void )const
	{
		return	static_cast<int>( width );
	}

	//	�X�N���[�����̔�����int�Ŏ擾
	int	Screen::GetWidthHalfInt( void )const
	{
		return	static_cast<int>( widthHalf );
	}

	//	�X�N���[��������int�Ŏ擾
	int	Screen::GetHeightInt( void )const
	{
		return	static_cast<int>( height );
	}

	//	�X�N���[�������̔�����int�Ŏ擾
	int	Screen::GetHeightHalfInt( void )const
	{
		return	static_cast<int>( heightHalf );
	}

	//	�X�N���[����Ԏ擾
	bool	Screen::GetScreenState( void )const
	{
		return	screenState;
	}

	//	���̎擾
	Screen*	Screen::GetInstance( void )
	{
		static	Screen	out;
		return	&out;
	}