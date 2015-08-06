
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"Y2009.h"

//*******************************************************************************
//
//	Player_Y2009�N���X
//
//*******************************************************************************

//--------------------------------------------------------------------------
//	�������E���
//--------------------------------------------------------------------------

	//	�R���X�g���N�^
	Y2009::Y2009( void )
	{
		//	�p�����[�^������
		speed = 0.2f;
		scale = 0.02f;
		SetMotionData();
		isGround = true;
	}
	
	//	�f�X�g���N�^
	Y2009::~Y2009( void )
	{
	
	}

	//	���[�V�����f�[�^�o�^
	void	Y2009::SetMotionData( void )
	{
		motionData.STAND		=	MotionNum::STAND;
		motionData.POSTURE	=	MotionNum::POSTURE;
		motionData.RUN			=	MotionNum::RUN;
		motionData.ATTACK1		=	MotionNum::ATTACK1;
		motionData.JUMP			=	MotionNum::JUMP;
		motionData.ATTACK2		=	MotionNum::ATTACK2;
		motionData.ATTACK3		=	MotionNum::ATTACK3;
		motionData.GUARD		=	MotionNum::GUARD;
		motionData.POSTURE	=	MotionNum::POSTURE;
	}

//--------------------------------------------------------------------------
//	�X�V�E�`��
//--------------------------------------------------------------------------

	//	�X�V
	void	Y2009::Update( void )
	{
		ModeManagement();
		BaseObj::Update();
	}

	//	�`��
	void	Y2009::Render( iexShader* shader, LPSTR technique )
	{
		BaseObj::Render( shader, technique );
	}

//--------------------------------------------------------------------------
//	����֐�
//--------------------------------------------------------------------------

	//	���[�h�Ǘ�
	void	Y2009::ModeManagement( void )
	{
		switch ( mode )
		{
		case MOVE:
			Move();
			break;

		case ATTACK:
			Attack();
			break;

		case JUMP:
			Jump();
			break;

		case GUARD:
			break;
		}
	}

	//	���[�hMove
	void	Y2009::Move( void )
	{
		CommonMove();

		if ( input->Get( KEY_A ) == 3 )	mode = ATTACK;
		if ( input->Get( KEY_B ) == 3 )	mode = JUMP;
	}

	//	���[�hAttack
	void	Y2009::Attack( void )
	{
		SetMotion( motionData.ATTACK1 );
		int		frame = obj->GetFrame();

		//	�U������
		if ( frame >= 377 && frame <= 385 )		attackParam = 1;
		else attackParam = 0;

		//	���[�V�����I������
		if ( frame >= 410 )		mode = MOVE;
	}

	//	���[�hJump
	void	Y2009::Jump( void )
	{
		CommonJump();
	}

	