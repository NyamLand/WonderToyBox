
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"system/System.h"
#include	"Particle.h"
#include	"Camera.h"
#include	"StartFinUI.h"
#include	"Sound.h"


//**********************************************************************
//
//	CoinUIEffect�N���X
//	
//**********************************************************************

//---------------------------------------------------------------------------------------
//	�O���[�o��
//---------------------------------------------------------------------------------------
#define	FIN_DELAY	2

//---------------------------------------------------------------------------------------
//	�������E���
//---------------------------------------------------------------------------------------
	
	//	�R���X�g���N�^
	StartFinUI::StartFinUI( void ) : count( 0 ), waitTimer( 0 ), s_t( 0 ), s_step( 0 ), changeflag( false )
	{
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.5f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.27f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.49f );
		ImageInitialize( s_obj, x, y, w, h, 0, 0, 512, 512 );
		w = static_cast<int>( iexSystem::ScreenWidth * 0.49f );
		h = static_cast<int>( iexSystem::ScreenHeight * 0.27f );
		ImageInitialize( f_obj, x, y, w, h, 0, 512, 1024, 512 );

		s_obj.obj = new iex2DObj( "DATA/UI/bfUI_02.png" );
		f_obj.obj = new iex2DObj( "DATA/UI/bfUI.png" );

		s_pos = Vector3( ( float )s_obj.x, -( ( float )s_obj.h / 2.0f ), 0.0f );
		f_pos = Vector3( ( float )s_obj.x, ( float )s_obj.y, 0.0f );
	}

	//	�f�X�g���N�^
	StartFinUI::~StartFinUI( void )
	{
		SafeDelete( s_obj.obj );
		SafeDelete( f_obj.obj );
	}


//---------------------------------------------------------------------------------------
//	�X�V�E�`��
//---------------------------------------------------------------------------------------
	void	StartFinUI::StartUpdate( void )
	{
		int round = gameManager->GetRound();
		static int	delay;
		switch ( s_step )
		{
			//	��ʏ㕔����ړ�
		case 0:
			if ( StartMove() )
			{
				SetScaling( s_obj, 4.0f );
				s_step++;
				break;
			}
			break;
			//	���n���Ȃ���t�F�[�h�A�E�g
		case 1:
			if ( !s_obj.scalingFlag )
			{
				s_obj.alpha -= 1.0f / 60.0f;
				if ( s_obj.alpha <= 0.0f )
				{
					s_obj.sx = 512;
					s_obj.sy = 512;
					SetScaling( s_obj, 3.0f, false );
					s_step++;
					break;
				}
			}
			ScalingLandingUpdate( s_obj, 100 );
			break;
			//	GO���t�F�[�h�C��
		case 2:
			sound->PlaySE(SE::GAMESTART_SE );
			if ( !s_obj.scalingFlag )
			{
				delay = 90;		//	������~�t���[����
				s_step++;
				break;
			}
			ScalingAlphaUpdate( s_obj, 300 );
			break;
			//	������~�t���[������AGo���t�F�[�h�A�E�g
		case 3:
			//	�^�C�}�[�̊Ԃ�break;
			delay--;
			if ( delay >= 0 )	break;

			changeflag = true;
			waitTimer = FIN_DELAY * SECOND;
			break;
		}

		//	FIGHT�`��̂��߃X�L�b�v
		if (s_step > 1) return;
		//	�ǂݍ��݈ʒu�E�T�C�Y�ݒ�
		switch (round)
		{
		case Round::ROUND1:
			s_obj.sx = 0;
			s_obj.sy = 0;

			break;

		case Round::ROUND2:
			s_obj.sx = 512;
			s_obj.sy = 0;
			break;

		case Round::ROUND_FINAL:
			s_obj.sx = 0;
			s_obj.sy = 512;
			break;
		}

	}

	//	���E���h�����ړ�
	bool	StartFinUI::StartMove( void )
	{
		Vector3 pos;

		//	�p�����[�^���Z
		s_t += 1.0f / 30.0f;

		if ( s_t >= 1.0f )
		{
			return true;
		}

		Lerp( pos, s_pos, f_pos, s_t );
		s_obj.x = ( int )pos.x;
		s_obj.y = ( int )pos.y;

		if ( s_t >= 1.0f ) s_t = 1.0f;

		return false;
	}

	//	�^�C���A�b�v���o
	void	StartFinUI::FinishUpdate( void )
	{
		waitTimer--;

		if ( waitTimer <= 0 )	changeflag = true;
	}


//---------------------------------------------------------------------------------------
//	���C���`��
//---------------------------------------------------------------------------------------

	//	�J�n�`��
	void	StartFinUI::StartRender( void )
	{
		RenderImage( s_obj, s_obj.sx, s_obj.sy, s_obj.sw, s_obj.sh, IMAGE_MODE::SCALING );
	}

	//	�I���`��
	void	StartFinUI::FinishRender( void )
	{
		RenderImage( f_obj, f_obj.sx, f_obj.sy, f_obj.sw, f_obj.sh, IMAGE_MODE::NORMAL );
	}


//------------------------------------------------------------------------------
//	���ݒ�E�擾
//------------------------------------------------------------------------------
	
	//	���[�h�ύX�t���O�ݒ�
	void	StartFinUI::SetChangeFlag( const bool& flag )
	{
		changeflag = flag;
	}

	//	���[�h�ύX�t���O�擾
	bool	StartFinUI::GetChangeFlag( void )
	{
		bool	out = this->changeflag;
		return	out;
	}
