#define	_USE_MATH_DEFINES
#include	<math.h>

#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Player.h"
#include	"Bullet.h"
#include	"BulletManager.h"
#include	"Coin.h"
#include	"CoinManager.h"
#include	"Squirrel.h"


//*********************************************************************************
//
//	Squirrel�N���X
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	�O���[�o��
//-----------------------------------------------------------------------------------

	namespace SquirrelData
	{
		//	�萔
		enum MotionNum
		{
			STAND		= 1,		//	����
			POSTURE,				//	�\��
			RUN			= 4,		//	����
			ATTACK1,				//	�U���P�i�K��
			ATTACK2,				//	�U���Q�i�K��
			ATTACK3,				//	�U���R�i�K��
			JUMP,
			GUARD,
		};

		enum OFFENSIVE_POWER
		{
			QUICK = 2,
			POWER = 4,
			HYPER = 25,
		};
	}

//-----------------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Squirrel::Squirrel( void )
	{
		//	�p�����[�^������
		attack_r	= 0.0f;
		attack_t	= 0.0f;
		speed		= 0.3f;
		scale			= 0.02f;
		diffence		= -5;
		isGround	= true;
		SetMotionData();
	}

	//	�f�X�g���N�^
	Squirrel::~Squirrel( void )
	{

	}

	//	���[�V�����f�[�^�o�^
	void	Squirrel::SetMotionData( void )
	{
		motionData.STAND		=	SquirrelData::STAND;
		motionData.POSTURE	=	SquirrelData::POSTURE;
		motionData.RUN			=	SquirrelData::RUN;
		motionData.ATTACK1		=	SquirrelData::ATTACK1;
		motionData.JUMP			=	SquirrelData::JUMP;
		motionData.ATTACK2		=	SquirrelData::ATTACK2;
		motionData.ATTACK3		=	SquirrelData::ATTACK3;
		motionData.GUARD		=	SquirrelData::GUARD;
		motionData.POSTURE	=	SquirrelData::POSTURE;
	}

//-----------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------

	//	�`��
	void	Squirrel::Render( iexShader* shader, LPSTR technique )
	{
		CommonRender( shader, technique );

		//	�f�o�b�O�p
		if ( !debug ) 	return;
		char	str[256];
		DrawSphere( attackPos, attack_r, 0xFFFFFFFF );
	
		Vector3	stringPos;
		Vector3	p_pos = GetPos();
		WorldToClient( p_pos, stringPos, matView* matProjection );
		stringPos.y -= 100.0f;
		sprintf_s( str, "��\n��\n��" );
		DrawString( str, ( int )stringPos.x, ( int )stringPos.y );
		sprintf_s(str, "x = %f\ny = %f\nz = %f\n", p_pos.x, p_pos.y, p_pos.z);
		DrawString( str, 20, 400 );
	}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

	//	�N�C�b�N�A�[�c
	bool	Squirrel::QuickArts( void )
	{
		static int time = 0;
		
		//	���擾
		Vector3	front = GetFront();
		Vector3	p_pos = GetPos();

		//	���ݒ�
		Vector3	vec = front * 5.0f;
		static float bulletSpeed = 0.5f;

		if ( time == 0 )m_BulletManager->Set( p_pos, vec, bulletSpeed );
		time++;

		if ( time >= 60 )
		{
			time = 0;
			return true;
		}
		return	false;
	}

	//	�p���[�A�[�c
	bool	Squirrel::PowerArts( void )
	{
		static int time = 0;

		//	�s�񂩂���擾
		Vector3	front = GetFront();
		Vector3	right = GetRight();
		Vector3	p_pos = GetPos();

		//	���ݒ�
		Vector3	vec[3] =
		{
			front * 5.0f + right * 5.0f,
			front * 5.0f,
			front * 5.0f + right * -5.0f
		};
		static float	 bulletSpeed = 0.5f;


		if ( time == 0 )
		{
			for ( int i = 0; i < 3; i++ )
			{
				m_BulletManager->Set( p_pos, vec[i], bulletSpeed );
			}
		}
		time++;

		if ( time >= 60 )
		{
			time = 0;
			return true;
		}
		return	false;
	}

	//	�n�C�p�[�A�[�c
	bool	Squirrel::HyperArts( void )
	{
		static int step = 0;		//	��
		static int time = 0;
	
		//	���擾
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );
		Matrix	mat = GetMatrix();
		Vector3	front = GetFront();
		Vector3	up = GetUp();
		Vector3	p_pos = GetPos();
		Vector3	moveParam = GetMove();
		float	angleParam = GetAngle();

		//	���ݒ�
		Vector3	vec = front * 5.0f + up * -15.0f;
		static	float	bulletScale = 1.0f;
		static	float	bulletSpeed = 0.5f;

		switch ( step )
		{
		case 0:
			moveParam.y += 0.5f;
			p_pos += moveParam;
			time++;
			if ( time >= 20 )
			{
				time = 0;
				step++;
			}
			break;

		case 1:
			angleParam += 0.1f;
			if ( time % 16 == 0 ) m_BulletManager->Set( p_pos, vec, bulletScale, bulletSpeed );
			time++;
			if ( time > 16 * 4 - 1 ) step++;
			break;

		case 2:
			step = 0;
			time = 0;
			return true;
			break;
		}

		//	���X�V
		SetPos( p_pos );
		SetAngle( angleParam );

		return	false;
	}

//-----------------------------------------------------------------------------------
//	���ݒ�
//-----------------------------------------------------------------------------------

	//	�U���p�p�����[�^�ݒ�
	void	Squirrel::SetAttackParam( int attackKind )
	{
		switch ( attackKind )
		{
		case MODE_STATE::QUICKARTS:
			attackParam = COLLISION_TYPE::SPHEREVSCAPSULE;
			knockBackType = KNOCKBACK_TYPE::WEAK;
			break;

		case MODE_STATE::POWERARTS:
			attackParam = COLLISION_TYPE::SPHEREVSCAPSULE;
			knockBackType = KNOCKBACK_TYPE::MIDDLE;
			break;

		case MODE_STATE::HYPERARTS:
			attackParam =	COLLISION_TYPE::SPHEREVSCYRINDER;
			knockBackType = KNOCKBACK_TYPE::STRENGTH;
			break;
		}
	}