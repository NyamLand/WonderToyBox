
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
//	�O���[�o��
//--------------------------------------------------------------------------

namespace ECCMANData
{
	enum MotionNum
	{
		POSTURE,				//	�\��
		RUN,						//	����
		ATTACK1,				//	�U���P�i�K��
		JUMP,
		LANDING,				//	���n
		ATTACK2,				//	�U���Q�i�K��
		ATTACK3,				//	�U���R�i�K��
		GUARD,					//	�K�[�h
	};
}

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
		motionData.POSTURE = ECCMANData::POSTURE;
		motionData.RUN = ECCMANData::RUN;
		motionData.ATTACK1 = ECCMANData::ATTACK1;
		motionData.JUMP = ECCMANData::JUMP;
		motionData.LANDING = ECCMANData::LANDING;
		motionData.ATTACK2 = ECCMANData::ATTACK2;
		motionData.ATTACK3 = ECCMANData::ATTACK3;
		motionData.GUARD = ECCMANData::GUARD;
		motionData.POSTURE = ECCMANData::POSTURE;
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
		case PlayerData::MOVE:
			Move();
			break;

		case PlayerData::ATTACK:
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
	void	ECCMAN::Move( void )
	{
		CommonMove();

		if ( input->Get( KEY_A ) == 3 )	mode = PlayerData::ATTACK;
		if ( input->Get( KEY_B ) == 3 )	mode = PlayerData::JUMP;
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
		if ( frame >= 410 )		mode = PlayerData::MOVE;
	}

	//	���[�hJump
	void	ECCMAN::Jump( void )
	{
		CommonJump();
	}
	