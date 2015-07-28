
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

//------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Y2009::Y2009( void )
	{
		//	�p�����[�^������
		speed = 0.2f;
		scale = 0.02f;
		isGround = true;

		//	���[�V�����ԍ��o�^
		SetMotionNum();
	}

	//	�f�X�g���N�^
	Y2009::~Y2009( void )
	{

	}
	
	//	���[�V�����ԍ��o�^
	void	Y2009::SetMotionNum( void )
	{
		SetMotionData( motionData, MotionType::POSTURE, POSTURE );
		SetMotionData( motionData, MotionType::RUN, RUN );
		SetMotionData( motionData, MotionType::ATTACK1, ATTACK1 );
		SetMotionData( motionData, MotionType::JUMP, JUMP );
		SetMotionData( motionData, MotionType::ATTACK2, ATTACK2 );
		SetMotionData( motionData, MotionType::ATTACK3, ATTACK3 );
		SetMotionData( motionData, MotionType::GUARD, GUARD );
	}


//------------------------------------------------------------------------------
//	�X�V
//------------------------------------------------------------------------------
	
	//	�X�V

//------------------------------------------------------------------------------
//	�`��
//------------------------------------------------------------------------------

	//	�`��

//------------------------------------------------------------------------------
//	����֐�
//------------------------------------------------------------------------------

	//	�ړ�

	//	�U��

	