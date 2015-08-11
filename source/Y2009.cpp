
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
//	�O���[�o��
//--------------------------------------------------------------------------

namespace Y2009Data
{
	//	�萔
	enum MotionNum
	{
		STAND = 1,			//	����
		POSTURE,				//	�\��
		RUN = 4,				//	����
		ATTACK1,				//	�U���P�i�K��
		ATTACK2,				//	�U���Q�i�K��
		ATTACK3,				//	�U���R�i�K��
		JUMP,
		GUARD,
	};
}

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
		motionData.STAND		=	Y2009Data::STAND;
		motionData.POSTURE	=	Y2009Data::POSTURE;
		motionData.RUN			=	Y2009Data::RUN;
		motionData.ATTACK1		=	Y2009Data::ATTACK1;
		motionData.JUMP			=	Y2009Data::JUMP;
		motionData.ATTACK2		=	Y2009Data::ATTACK2;
		motionData.ATTACK3		=	Y2009Data::ATTACK3;
		motionData.GUARD		=	Y2009Data::GUARD;
		motionData.POSTURE	=	Y2009Data::POSTURE;
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
		case PlayerData::MOVE:
			Move();
			break;

		case PlayerData::ATTACK:
		case PlayerData::QUICKARTS:
		case PlayerData::POWERARTS:
		case PlayerData::HYPERARTS:
			Attack();
			break;

		case PlayerData::JUMP:
			Jump();
			break;

		case PlayerData::GUARD:
			break;
		}
	}

	//	���[�hMove
	void	Y2009::Move( void )
	{
		CommonMove();

		if ( input->Get( KEY_A ) == 3 )	mode = PlayerData::QUICKARTS;
		if ( input->Get( KEY_B ) == 3 )	mode = PlayerData::POWERARTS;
		if ( input->Get( KEY_C ) == 3 )	mode = PlayerData::HYPERARTS;
		if ( input->Get( KEY_D ) == 3 )	mode = PlayerData::JUMP;
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
		if ( frame >= 410 )		mode = PlayerData::MOVE;
	}

	//	���[�hJump
	void	Y2009::Jump( void )
	{
		CommonJump();
	}

	