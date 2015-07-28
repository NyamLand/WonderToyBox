
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
		isGround = true;

		//	���[�V�����ԍ��U�蕪��
		SetMotionNum();
	}

	//	�f�X�g���N�^
	ECCMAN::~ECCMAN( void )
	{

	}

	//	���[�V�����ԍ��U�蕪��
	void	ECCMAN::SetMotionNum( void )
	{
		SetMotionData( motionData, MotionType::POSTURE, POSTURE );
		SetMotionData( motionData, MotionType::RUN, RUN );
		SetMotionData( motionData, MotionType::ATTACK1, ATTACK1 );
		SetMotionData( motionData, MotionType::JUMP, JUMP );
		SetMotionData( motionData, MotionType::LANDING, LANDING );
		SetMotionData( motionData, MotionType::ATTACK2, ATTACK2 );
		SetMotionData( motionData, MotionType::ATTACK3, ATTACK3 );
		SetMotionData( motionData, MotionType::GUARD, GUARD );
	}


//--------------------------------------------------------------------------
//	�X�V
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
//	�`��
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	����֐�
//--------------------------------------------------------------------------