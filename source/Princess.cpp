
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"Princess.h"

//*********************************************************************************
//
//	Princess�N���X
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Princess::Princess( void )
	{
		//	�p�����[�^������
		attack_r = 0.0f;
		attack_t = 0.0f;
		speed = 0.2f;
		scale = 0.02f;
		SetMotionData();
		isGround = true;
	}

	//	�f�X�g���N�^
	Princess::~Princess( void )
	{

	}

	//	���[�V�����f�[�^�o�^
	void	Princess::SetMotionData( void )
	{
		motionData.STAND = MotionNum::STAND;
		motionData.POSTURE = MotionNum::POSTURE;
		motionData.RUN = MotionNum::RUN;
		motionData.ATTACK1 = MotionNum::ATTACK1;
		motionData.JUMP = MotionNum::JUMP;
		motionData.ATTACK2 = MotionNum::ATTACK2;
		motionData.ATTACK3 = MotionNum::ATTACK3;
		motionData.GUARD = MotionNum::GUARD;
		motionData.POSTURE = MotionNum::POSTURE;
	}

//-----------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------

	//	�X�V
	void	Princess::Update( void )
	{
		ModeManagement();
		BaseObj::Update();
	}

	//	�`��
	void	Princess::Render( iexShader* shader, LPSTR technique )
	{
		BaseObj::Render( shader, technique );

		//	�f�o�b�O�p
		DrawSphere( attackPos, attack_r, 0xFFFFFFFF );
	}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

	//	���[�h�Ǘ�
	void	Princess::ModeManagement( void )
	{
		switch ( mode )
		{
		case MOVE:
			Move();
			break;

		case ATTACK:
		case	POWERARTS:
		case HYPERARTS:
		case QUICKARTS:
			move = Vector3( 0.0f, 0.0f, 0.0f );
			Attack( mode );
			break;

		case JUMP:
			Jump();
			break;

		case GUARD:
			break;
		}
	}

	//	���[�hMove
	void	Princess::Move( void )
	{
		CommonMove();

		if ( input->Get( KEY_A ) == 3 )	mode = QUICKARTS;
		if ( input->Get( KEY_B ) == 3 )	mode = POWERARTS;
		if ( input->Get( KEY_C ) == 3 )	mode = HYPERARTS;
		if ( input->Get( KEY_D ) == 3 )	mode = JUMP;
	}

	//	�N�C�b�N�A�[�c
	bool	Princess::QuickArts( void )
	{
		//	�s�񂩂�O���擾
		Matrix	mat = obj->TransMatrix;
		Vector3	front = Vector3( mat._31, mat._32, mat._33 );
		front.Normalize();

		Vector3	startPos = Vector3( pos.x, pos.y + 1.5f, pos.z );
		Vector3	finPos = startPos + front * 5.0f;

		//	�����蔻��ʒu�ړ�&�͈͊g��
		float t = GetBezier( ePrm_t::eRapid_Lv3, ePrm_t::eSlow_Lv5, attack_t );
		Lerp( attackPos, startPos, finPos, t );
		attack_r = 3.0f * t;

		//	�p�����[�^���Z
		attack_t += 0.015f;

		if ( attack_t >= 1.0f )	return	true;
		return	false;
	}

	//	�p���[�A�[�c
	bool	Princess::PowerArts( void )
	{
		attackPos = Vector3( pos.x, pos.y + 1.5f, pos.z );

		//	�͈͊g��
		Lerp( attack_r, 0.0f, 3.0f, attack_t );

		//	�p�����[�^���Z
		attack_t += 0.02f;

		if ( attack_t >= 1.0f )	return	true;
		return	false;
	}

	//	�n�C�p�[�A�[�c
	bool	Princess::HyperArts( void )
	{
		static	int		num = 0;	//	��
		attackPos = Vector3( pos.x, pos.y + 1.5f, pos.z );

		//	�͈͊g��
		float t = GetBezier( ePrm_t::eSlow_Lv4, ePrm_t::eRapid_Lv1, attack_t );
		Lerp( attack_r, 0.0f, 50.0f, t );

		//	�p�����[�^���Z
		attack_t += 0.02f;

		if ( attack_t >= 1.0f )
		{
			switch ( num )
			{
			case 0:
				num++;
				attack_t  = 0.0f;
				break;

			case 1:
				num = 0;
				return	true;
				break;
			}
		}
		return	false;
	}

	//	���[�hAttack
	void	Princess::Attack( int attackKind )
	{
		SetMotion( motionData.ATTACK1 );
		int		frame = obj->GetFrame();

		bool	isEnd = false;

		switch ( attackKind )
		{
		case QUICKARTS:
			isEnd = QuickArts();
			if ( !isEnd )	attackParam = 1;
			break;

		case POWERARTS:
			isEnd = PowerArts();
			if ( !isEnd )	attackParam = 2;
			break;

		case HYPERARTS:
			isEnd = HyperArts();
			if ( !isEnd )	attackParam = 3;
			break;
		}

		//	���[�V�����I������
		if ( isEnd )
		{
			mode = MOVE;
			attack_t = 0.0f;
			attack_r = 0.0f;
		}
	}

	//	���[�hJump
	void	Princess::Jump( void )
	{
		CommonJump();
	}