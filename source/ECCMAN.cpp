
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"ECCMAN.h"

//*******************************************************************************
//
//	Player_ECCMAN�N���X
//
//*******************************************************************************

//--------------------------------------------------------------------------
//	�������E���
//--------------------------------------------------------------------------

	//	�R���X�g���N�^
	ECCMAN::ECCMAN( void )
	{
		//	�p�����[�^������
		speed = 0.3f;
		scale = 0.02f;
		SetMotionData();
		isGround = true;
	}

	//	�f�X�g���N�^
	ECCMAN::~ECCMAN( void )
	{

	}

	//	���[�V�����f�[�^�o�^
	void	ECCMAN::SetMotionData( void )
	{
		motionData.POSTURE = MotionNum::POSTURE;
		motionData.RUN = MotionNum::RUN;
		motionData.ATTACK1 = MotionNum::ATTACK1;
		motionData.JUMP = MotionNum::JUMP;
		motionData.LANDING = MotionNum::LANDING;
		motionData.ATTACK2 = MotionNum::ATTACK2;
		motionData.ATTACK3 = MotionNum::ATTACK3;
		motionData.GUARD = MotionNum::GUARD;
		motionData.POSTURE = MotionNum::POSTURE;
	}

//--------------------------------------------------------------------------
//	�X�V�E�`��
//--------------------------------------------------------------------------

	//	�X�V
	void	ECCMAN::Update( void )
	{
		ModeManagement();
		BaseObj::Update();
	}

	//	�`��
	void	ECCMAN::Render( iexShader* shader, LPSTR technique )
	{
		BaseObj::Render( shader, technique );
	}

//--------------------------------------------------------------------------
//	����֐�
//--------------------------------------------------------------------------

	//	���[�h�Ǘ�
	void	ECCMAN::ModeManagement( void )
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
	void	ECCMAN::Move( void )
	{
		CommonMove();

		if ( input->Get( KEY_A ) == 3 )	mode = ATTACK;
		if ( input->Get( KEY_B ) == 3 )	mode = JUMP;
	}
	
	//	���[�hAttack
	void	ECCMAN::Attack( void )
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
	void	ECCMAN::Jump( void )
	{
		CommonJump();
	}
	