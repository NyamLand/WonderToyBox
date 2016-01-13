
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"system/System.h"
#include	"GameManager.h"
#include	"AirPlane.h"

//**********************************************************************
//
//	AirPlane�N���X
//
//**********************************************************************

//--------------------------------------------------------------------------------
//	�O���[�o��
//--------------------------------------------------------------------------------
#define	ROLL_POINT_ADJUST_X	200

//--------------------------------------------------------------------------------
//	�������E���
//--------------------------------------------------------------------------------

	//	�R���X�g���N�^
	AirPlane::AirPlane( void ) : pos( 0.0f, 0.0f, 0.0f ), startPos( 0.0f, 0.0f, 0.0f ), endPos( 0.0f, 0.0f, 0.0f ),
		angle( 0.0f ), scale( 0.0f, 0.0f, 0.0f ), center( GetPoint( 0, 0 ) ), mode( STAY ), heightParam( 0.0f ), sideParam( 0.0f ),
		renderflag(false), type(0)
	{
		int x = static_cast<int>( iexSystem::ScreenWidth * 1.01f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 1.04f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.6f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.14f );
		scale = Vector3( w, h, 0.0f );
		ImageInitialize( obj, ( int )pos.x, ( int )pos.y, ( int )scale.x, ( int )scale.y, 0, 0, 1024, 128 );
		obj.obj = new iex2DObj( "DATA/UI/menu/menu-int.png" );
	}

	//	�f�X�g���N�^
	AirPlane::~AirPlane( void )
	{
		SafeDelete( obj.obj );
	}

//--------------------------------------------------------------------------------
//	�X�V�E�`��
//--------------------------------------------------------------------------------

	//	�X�V
	void	AirPlane::Update( void )
	{
		//	�I������p
		bool	isEnd = false;

		//	�e���[�h����
		switch ( mode )
		{
		case	FLYING_IN:
			isEnd = FlyingIn();
			if ( isEnd )	mode = STAY;
			break;

		case	FLYING_OUT:
			isEnd = FlyingOut();
			if ( isEnd )
			{
				mode = STAY;
				renderflag = false;
			}
			break;

		case	STAY:
			Stay();
			break;
		}


	}

	//	�`��
	void	AirPlane::Render( void )
	{
		//	���X�V
		obj.x = (int)pos.x;
		obj.y = (int)pos.y;
		obj.angle = angle;
		obj.p = center;
		obj.w = (int)scale.x;
		obj.h = (int)scale.y;
		obj.renderflag = renderflag;

		RenderImage( obj, obj.sx, obj.sy, obj.sw, obj.sh, IMAGE_MODE::ADOPTPARAM );
	}

//--------------------------------------------------------------------------------
//	����֐�
//--------------------------------------------------------------------------------

	//	��ї���
	bool	AirPlane::FlyingOut( void )
	{
		//	�p�����[�^���Z
		param += 0.01f;
		if ( param >= 1.0f )	param = 1.0f;

		//	�p�����[�^����
		heightParam = GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv5, param );

		//	��ړ�
		Lerp( pos.y, startPos.y, endPos.y, heightParam );

		//	�p�����[�^����
		sideParam = GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv3, param );

		//	���ړ�
		Lerp( pos.x, startPos.x, endPos.x, sideParam );

		//	��]
		FlyRoll();

		//	�ړ���������
		if ( param >= 1.0f )	return	true;
		return	false;
	}

	//	��ѓ���
	bool	AirPlane::FlyingIn( void )
	{
		//	�p�����[�^���Z
		param += 0.01f;
		if ( param >= 1.0f )	param = 1.0f;

		//	�p�����[�^����
		heightParam = GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv5, param );

		//	��ړ�
		Lerp( pos.y, startPos.y, endPos.y, heightParam );

		//	�p�����[�^����
		sideParam = GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, param );

		//	���ړ�
		Lerp( pos.x, startPos.x, endPos.x, sideParam );

		//	��]
		FlyRoll();

		//	�ړ���������
		if ( param >= 1.0f )	return	true;
		return	false;
	}

	//	�؋�
	void	AirPlane::Stay( void )
	{
		//	�`�悳��ĂȂ��Ԃ̓X�L�b�v
		if ( !renderflag )	return;

		//	�ӂ�ӂ킳����
		pos.y = endPos.y + static_cast<int>( 10.0f * sinf( param ) );
		center = GetPoint( pos.x - scale.x * 0.2f, pos.y );
		angle = ( D3DX_PI / 180.0f * -1.0f ) * cosf( param );
		param += D3DX_PI / 180.0f;
	}

	//	�؋󎞉�]
	void	AirPlane::FlyRoll( void )
	{
		//	��]
		center = GetPoint( pos.x - scale.x * 0.2f, pos.y );
		angle = ( D3DX_PI / 180.0f * -1.0f ) * sinf( D3DX_PI * 0.5f * param );
	}

//--------------------------------------------------------------------------------
//	���ݒ�
//--------------------------------------------------------------------------------

	//	�^�C�v�ݒ�
	void	AirPlane::SetType( int type )
	{
		this->type = type;
		obj.sy = 128 * type;
	}

	//	���̓���A�ʒu�ݒ�
	void	AirPlane::SetNext( Vector3	startPos, Vector3 nextPoint, int mode )
	{
		param = 0.0f;
		this->startPos = startPos;
		endPos = nextPoint;
		this->mode = mode;
		renderflag = true;
	}

	//	�`��t���O�ݒ�
	void	AirPlane::SetRenderFlag( bool state )
	{
		renderflag = state;
	}

	//	�X�P�[���ݒ�
	void	AirPlane::SetScale( float w, float h )
	{
		scale.x = w;
		scale.y = h;
	}

//--------------------------------------------------------------------------------
//	���擾
//--------------------------------------------------------------------------------

	//	���W�擾
	Vector3	AirPlane::GetPos( void )const
	{
		return	pos;
	}

