
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

	//	������
	bool	Screen::Initialize( void )
	{
		mode = SCREEN_MODE::FADE_IN;
		color = Color::BLACK;
		screenState = false;
		alpha = 1.0f;
		speed = 1.0f;
		size = Size::MIN;

		shader3D->SetValue("screen_width", static_cast<float>(iexSystem::ScreenWidth));
		shader3D->SetValue("screen_height", static_cast<float>(iexSystem::ScreenHeight));

		return	false;
	}

	//	���
	void	Screen::Release( void )
	{

	}

//-----------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------

	//	�X�V
	bool	Screen::Update( void )
	{
		screenState = false;

		switch ( mode )
		{
		case SCREEN_MODE::FADE_IN:
		case SCREEN_MODE::WHITE_IN:
			screenState = FadeIn();
			break;

		case SCREEN_MODE::FADE_OUT:
		case SCREEN_MODE::WHITE_OUT:
			screenState = FadeOut();
			break;

		case SCREEN_MODE::WIPE_IN:
			screenState = WipeIn();
			break;

		case SCREEN_MODE::WIPE_OUT:
			screenState = WipeOut();
			break;
		}

		if (mode == SCREEN_MODE::WIPE_IN ||
			mode == SCREEN_MODE::WIPE_OUT){
			shader->SetValue("effect_size", size);
		}

		return	screenState;
	}

	//	�`��
	void	Screen::Render( void )
	{
		if ( mode == SCREEN_MODE::WIPE_IN ||
			 mode == SCREEN_MODE::WIPE_OUT )
		{
			iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, shader3D, "WipeEffect",  GetColor( color, alpha ) );

		}
		else{
			iexPolygon::Rect( 0, 0, 1280, 720, RS_COPY, GetColor( color, alpha ) );
		}
	}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

	//	�t�F�[�h�C��
	bool	Screen::FadeIn( void )
	{
		alpha -= D3DX_PI / 180.0f * speed;
		
		if ( alpha <= 0.0f ){
			alpha = 0.0f;
			screenState = true;
			return true;
		}
		
		return	false;
	}

	//	�t�F�[�h�A�E�g
	bool	Screen::FadeOut( void )
	{
		if ( alpha >= 1.0f ){
			alpha = 1.0f;
			screenState = true;
			return true;
		}

		alpha += D3DX_PI / 180.0f * speed;

		return	false;
	}

	//	���C�v�A�E�g
	bool	Screen::WipeOut( void )
	{
		size -= (int)speed;
		if (size < Size::MIN)
		{
			size = Size::MIN;
			return true;
		}
		return false;
	}

	//	���C�v�C��
	bool	Screen::WipeIn(void)
	{
		size += (int)speed;
		if (size > Size::MAX)
		{
			size = Size::MAX;
			return true;
		}
		return false;
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

		switch ( this->mode )
		{
		case SCREEN_MODE::FADE_IN:
			color = Color::BLACK;
			alpha = 1.0f;
			break;

		case SCREEN_MODE::FADE_OUT:
			color = Color::BLACK;
			alpha = 0.0f;
			break;

		case SCREEN_MODE::WHITE_IN:
			color = Color::WHITE;
			alpha = 1.0f;
			break;

		case SCREEN_MODE::WHITE_OUT:
			color = Color::WHITE;
			alpha = 0.0f;
			break;

		case SCREEN_MODE::WIPE_OUT:
			color = Color::BLACK;
			alpha = 1.0f;
			size = Size::MAX;
			break;

		case SCREEN_MODE::WIPE_IN:
			color = Color::BLACK;
			alpha = 1.0f;
			size = Size::MIN;
			break;

		}
	}

//-----------------------------------------------------------------------------------
//	���擾
//-----------------------------------------------------------------------------------

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