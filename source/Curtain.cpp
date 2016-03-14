
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Curtain.h"

//**********************************************************************
//
//	Curtain�N���X
//
//**********************************************************************

#define	VERTEX_MAX	4

//-----------------------------------------------------------------------------
//	�������E�J��
//-----------------------------------------------------------------------------

	//	�R���X�g���N�^
	Curtain::Curtain( void ) : mode( CURTAIN_MODE::CLOSE ), speed( 0.001f ), param( 1.0f )
	{
		//	�摜�ǂݍ���
		Load();

		//	�p�����[�^������
		curtainL.t = param;
		curtainR.t = param;

		//	���_���W�ݒ�
		StartPosInitialize();
		EndPosInitialize();

		//	���_�ݒ�
		FOR( 0, VERTEX_MAX )
		{
			SetVertex( curtainL.tlv[value], 
				startPosL[CLOSE][value],
				( float )( iexSystem::ScreenHeight * ( value / 2 ) ), 
				0.0f, 
				float( value % 2 ), 
				float( value / 2 ), 
				0xFFFFFFFF );

			SetVertex( curtainR.tlv[value], 
				startPosR[CLOSE][value], 
				( float )( iexSystem::ScreenHeight * ( value / 2 ) ), 
				0.0f, 
				float( value % 2 ), 
				float( value / 2 ), 
				0xFFFFFFFF );
		}

		//	�X�V
		Update();
	}

	//	�f�X�g���N�^
	Curtain::~Curtain( void )
	{
		SafeDelete( curtainR.obj );
		SafeDelete( curtainL.obj );
	}

	//	�摜�ǂݍ���
	void	Curtain::Load( void )
	{
		//	�摜�ǂݍ���
		curtainL.obj = new iex2DObj( "DATA/UI/title/curtain1.png" );
		curtainR.obj = new iex2DObj( "DATA/UI/title/curtain2.png" );
	}

	//	�n�_�ݒ�
	void	Curtain::StartPosInitialize( void ) 
	{		
		//	Open�������ʒu�ݒ�
		startPosL[OPEN][0] = 0;																					startPosL[OPEN][1] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );
		startPosL[OPEN][2] = 0;																					startPosL[OPEN][3] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );
		startPosR[OPEN][0] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );	startPosR[OPEN][1] = static_cast<float>( iexSystem::ScreenWidth );
		startPosR[OPEN][2] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );	startPosR[OPEN][3] = static_cast<float>( iexSystem::ScreenWidth );

		//	Close�������ʒu�ݒ�
		startPosL[CLOSE][0] = -( static_cast<float>( iexSystem::ScreenWidth * 0.5f ) );		startPosL[CLOSE][1] = 0;
		startPosL[CLOSE][2] = -( static_cast<float>( iexSystem::ScreenWidth * 0.5f ) );		startPosL[CLOSE][3] = 0;
		startPosR[CLOSE][0] = static_cast<float>( iexSystem::ScreenWidth );						startPosR[CLOSE][1] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );
		startPosR[CLOSE][2] = static_cast<float>( iexSystem::ScreenWidth );						startPosR[CLOSE][3] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );
	}

	//	�I�_�ݒ�
	void	Curtain::EndPosInitialize( void )
	{
		//	Open�������ʒu�ݒ�
		endPosL[OPEN][0] = -( static_cast<float>( iexSystem::ScreenWidth * 0.5f ) );	endPosL[OPEN][1] = 0;
		endPosL[OPEN][2] = -( static_cast<float>( iexSystem::ScreenWidth * 0.5f ) );	endPosL[OPEN][3] = 0;
		endPosR[OPEN][0] = static_cast<float>( iexSystem::ScreenWidth );					endPosR[OPEN][1] = static_cast<float>( iexSystem::ScreenWidth * 1.5f );
		endPosR[OPEN][2] = static_cast<float>( iexSystem::ScreenWidth );					endPosR[OPEN][3] = static_cast<float>( iexSystem::ScreenWidth * 1.5f );

		//	Close�������ʒu�ݒ�
		endPosL[CLOSE][0] = 0;																					endPosL[CLOSE][1] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );
		endPosL[CLOSE][2] = 0;																					endPosL[CLOSE][3] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );
		endPosR[CLOSE][0] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );		endPosR[CLOSE][1] = static_cast<float>( iexSystem::ScreenWidth );
		endPosR[CLOSE][2] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );		endPosR[CLOSE][3] = static_cast<float>( iexSystem::ScreenWidth );
	}

//-----------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------

	//	�X�V
	void	Curtain::Update( void )
	{
		//	�p�����[�^���Z
		isEnd = PercentageUpdate( param, speed );
		
		//	�p�����[�^�K�p
		curtainL.t = param;
		curtainR.t = param;

		switch ( mode )
		{
		case CURTAIN_MODE::OPEN:
			Open();
			break;
			
		case CURTAIN_MODE::CLOSE:
			Close();
			break;
		}
	}

	//	�`��
	void	Curtain::Render( void )
	{
		iexPolygon::Render2D( curtainL.tlv, 2, curtainL.obj, RS_COPY );
		iexPolygon::Render2D( curtainR.tlv, 2, curtainR.obj, RS_COPY );
	}

//-----------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------

	//	�J���@�e���_�ړ�
	void	Curtain::Open( void )
	{
		int speedMode = 0;
		FOR( 0, VERTEX_MAX )
		{
			if ( value / 2 == 0 )	speedMode = ePrm_t::eSlow_Lv1;
			else							speedMode = ePrm_t::eSlow_Lv5;
			Lerp( curtainL.tlv[value].sx, startPosL[OPEN][value], endPosL[OPEN][value], GetBezier( speedMode, speedMode, curtainL.t ) );
			Lerp( curtainR.tlv[value].sx, startPosR[OPEN][value], endPosR[OPEN][value], GetBezier(speedMode, speedMode, curtainR.t ) );
		}
	}

	//	����	�e���_�ړ�
	void	Curtain::Close( void )
	{
		FOR( 0, VERTEX_MAX )
		{
			Lerp( curtainL.tlv[value].sx, startPosL[CLOSE][value], endPosL[CLOSE][value], GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtainL.t ) );
			Lerp( curtainR.tlv[value].sx, startPosR[CLOSE][value], endPosR[CLOSE][value], GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtainR.t ) );
		}
	}

//-----------------------------------------------------------------------------
//	���ݒ�
//-----------------------------------------------------------------------------

	//	���[�h�ݒ�
	void	Curtain::SetMode( CURTAIN_MODE mode, float speed )
	{
		this->mode = mode;
		this->speed = speed;
		this->param = 0.0f;
		isEnd = false;
	}

//-----------------------------------------------------------------------------
//	���擾
//-----------------------------------------------------------------------------	

	//	����I���t���O�擾
	bool	Curtain::GetIsEnd( void )const
	{
		return	isEnd;
	}