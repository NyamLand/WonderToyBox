
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Random.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"FallLife.h"

//**********************************************************************
//
//	FallLife�N���X
//
//**********************************************************************

//--------------------------------------------------------------------------------
//	�O���[�o��
//--------------------------------------------------------------------------------
#define	MAX_LIFE		5
#define	FALL_SPEED		0.03f
#define	PERCENTAGE_MAX	1.0f
#define	MOVE_X	70
#define	MOVE_Y	-15
#define	SUB_MOVE_Y	1

//--------------------------------------------------------------------------------
//	�������E���
//--------------------------------------------------------------------------------

	//	�R���X�g���N�^
	FallLife::FallLife( void ) : orgLife( nullptr )
	{
		FOR( 0, PLAYER_MAX )
		{
			lifeImage[value].obj = nullptr;
			damageFlag[value] = false;
			param[value] = PERCENTAGE_MAX;
			fallDirection[value] = LEFT_FALL;
			savePos[value].x = 0;
			savePos[value].y = 0;
			moveY[value] = MOVE_Y;
		}
	}

	//	�f�X�g���N�^
	FallLife::~FallLife( void )
	{
		Release();
	}

	//	������
	bool	FallLife::Initialize( void )
	{
		//	�ǂݍ���
		Load();

		//	�摜�ݒ�
		FOR( 0, PLAYER_MAX )
		{
			lifeImage[value].obj = orgLife;
			ImageInitialize( lifeImage[value], 0, 0, 75, 75, 0, 0, 64, 64 );
			lifeImage[value].renderflag = false;
			if ( lifeImage[value].obj == nullptr )	return	false;
		}

		return	true;
	}

	//	���
	void	FallLife::Release( void )
	{
		SafeDelete( orgLife );
	}

	//	�ǂݍ���
	void	FallLife::Load( void )
	{
		orgLife = new iex2DObj( "DATA/UI/NLife.png" );
	}

//--------------------------------------------------------------------------------
//	�X�V�E�`��
//--------------------------------------------------------------------------------

	//	�X�V
	void	FallLife::Update( void )
	{
		bool	active = false;

		FOR( 0, PLAYER_MAX )
		{
			//	�p�����[�^���Z( �t���O�����ĂȂ�������Ƃ΂� )
			active = AddPercentage( value );
			if ( !active )	continue;

			//	����
			Fall( value );

			//	�����x�X�V
			AlphaUpdate( value );

			//	�����X�V
			AngleUpdate( value );
		}
	}

	//	�`��
	void	FallLife::Render( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			//	�_���[�W�󂯂ĂȂ��Ƃ��`�悵�Ȃ�
			if ( !damageFlag[value] )	continue;

			//	�摜�`��
			RenderImage( lifeImage[value], 
				lifeImage[value].sx, lifeImage[value].sy,
				lifeImage[value].sw, lifeImage[value].sh,
				IMAGE_MODE::ADOPTPARAM );
		}
	}

//--------------------------------------------------------------------------------
//	����֐�
//--------------------------------------------------------------------------------

	//	�������Z( �v���C���[�ԍ� ) : param�����Z�A100%�Ŏ~�߂�A�t���O�������ĂȂ�������false
	bool	FallLife::AddPercentage( int playerNum )
	{
		if ( !damageFlag[playerNum] )		return false;

		//	�p�����[�^���Z
		param[playerNum] += FALL_SPEED;
		if ( param[playerNum] >= PERCENTAGE_MAX )
		{
			param[playerNum] = PERCENTAGE_MAX;
			damageFlag[playerNum] = false;
			lifeImage[playerNum].renderflag = false;
			lifeImage[playerNum].angle = 0.0f;
		}

		return	true;
	}

	//	�����@���W�ړ�
	void	FallLife::Fall( int playerNum )
	{
		//	X���W�ړ����
		Lerp( lifeImage[playerNum].x, 
			savePos[playerNum].x, 
			savePos[playerNum].x + MOVE_X * fallDirection[playerNum], 
			GetBezier( ePrm_t::eSlow_Lv3, ePrm_t::eSlow_Lv1, param[playerNum] ) );

		//	Y���W�ړ��l���Z
		moveY[playerNum] += SUB_MOVE_Y;

		//	Y���W���Z
		lifeImage[playerNum].y += moveY[playerNum];
	}

	//	�����x�X�V
	void	FallLife::AlphaUpdate( int playerNum )
	{
		//	���
		Lerp( 
			lifeImage[playerNum].alpha,
			1.0f, 0.0f, 
			GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5,param[playerNum] ) );
	}

	//	�����X�V
	void	FallLife::AngleUpdate( int playerNum )
	{
		//	���
		CubicFunctionInterpolation(
			lifeImage[playerNum].angle,
			0.0f, 
			( D3DX_PI  * 0.25f ) * fallDirection[playerNum],
			param[playerNum] );

		//	��]���S���W�ݒ�
		lifeImage[playerNum].p = GetPoint( lifeImage[playerNum].x, lifeImage[playerNum].y );
	}

//--------------------------------------------------------------------------------
//	���ݒ�
//--------------------------------------------------------------------------------

	//	���W�ݒ�( �v���C���[�ԍ��AX���W�AY���W )
	void	FallLife::SetPos( int playerNum, int posX, int posY )
	{
		savePos[playerNum].x = posX;
		savePos[playerNum].y = posY;
	}

	//	�ǂݍ��݈ʒu�ݒ�
	void	FallLife::SetSrcPos( int playerNum, int sx, int sy )
	{
		lifeImage[playerNum].sx = sx;
		lifeImage[playerNum].sy = sy;
	}

	//	�_���[�W�t���O�ݒ�( �v���C���[�ԍ��Atrue�Ń��C�t�������o���Z�b�g�A����O�̃��C�t )
	void	FallLife::SetDamageFlag( int playerNum, bool flag, int culLife )
	{
		damageFlag[playerNum] = flag;
		lifeImage[playerNum].renderflag = true;
		param[playerNum] = 0.0f;
		lifeImage[playerNum].y = savePos[playerNum].y;
		moveY[playerNum] = MOVE_Y;
		
		//	���������ݒ�(�����_��)
		if ( Random::PercentageRandom( 0.5f ) )	fallDirection[playerNum] = FALL_DIRECTION::LEFT_FALL;
		else																fallDirection[playerNum] = FALL_DIRECTION::RIGHT_FALL;

		//	�ǂݍ��݈ʒu�ݒ�
		SetSrcPos( 
			playerNum,
			lifeImage[playerNum].sw * ( ( MAX_LIFE - culLife ) % 4 ),
			lifeImage[playerNum].sh * ( ( MAX_LIFE- culLife ) / 4 ) );
	}

//--------------------------------------------------------------------------------
//	���擾
//--------------------------------------------------------------------------------
