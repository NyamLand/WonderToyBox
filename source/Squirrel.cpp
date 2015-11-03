#define	_USE_MATH_DEFINES
#include	<math.h>

#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
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

	namespace
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
	Squirrel::Squirrel( void ) : BaseChara()
	{
		//	�p�����[�^������
		attackInfo.r	= 0.0f;
		attackInfo.t	= 0.0f;
		speed		= 0.3f;
		scale			= 0.02f;
		diffence		= -5;
		isGround	= true;
	}

	//	�f�X�g���N�^
	Squirrel::~Squirrel( void )
	{

	}

	//	������
	bool	Squirrel::Initialize( int playerNum, Vector3 pos )
	{
		//	�e�̏�����
		BaseChara::Initialize( playerNum, pos );

		//	���f���ǂݍ���
		if ( obj == nullptr )
			obj = new iex3DObj( "DATA/CHR/SQUIRREL/SQUIRREL.IEM" );

		//	�X�P�[���ݒ�
		obj->SetScale( 0.02f );
		obj->Update();

		if ( obj == nullptr )	return	false;
		return	true;
	}

//-----------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------

	//	�`��
	void	Squirrel::Render( iexShader* shader, LPSTR technique )
	{
		BaseChara::Render( shader, technique );

		//	�f�o�b�O�p
		if ( !debug ) 	return;
		char	str[256];
		DrawSphere( attackInfo.pos, attackInfo.r, 0xFFFFFFFF );
	
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
		power = QUICK;

		static int time = 0;
		SetMove( Vector3( 0.0f, move.y, 0.0f ) );
		
		//	���擾
		Vector3	front = GetFront();
		Vector3	p_pos = GetPos();

		//	���ݒ�
		Vector3	vec = front * 5.0f;
		static float bulletSpeed = 0.5f;

		int leanpower = 0;
		if ( time == 0 )m_BulletManager->Set( p_pos, vec, bulletSpeed ,leanpower);
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
		power = POWER;

		static int time = 0;

		//	�s�񂩂���擾
		Vector3	front = GetFront();
		Vector3	right = GetRight();
		Vector3	p_pos = GetPos();
		SetMove( Vector3( 0.0f, move.y, 0.0f ) );

		//	���ݒ�
		Vector3	vec[3] =
		{
			front * 5.0f + right * 5.0f,
			front * 5.0f,
			front * 5.0f + right * -5.0f
		};
		static float	 bulletSpeed = 0.5f;
		int leanpower = 30;


		if ( time == 0 )
		{
			for ( int i = 0; i < 3; i++ )
			{
				m_BulletManager->Set( p_pos, vec[i], bulletSpeed ,leanpower);
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
		power = HYPER;

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

	//	���[�V�����Ǘ�
	void	Squirrel::MotionManagement(int motion)
	{
		switch (motion)
		{
		case MOTION_NUM::STAND:
			obj->SetMotion(MOTION_DATA::STAND);
			break;

		case MOTION_NUM::POSTURE:
			obj->SetMotion(MOTION_DATA::POSTURE);
			break;

		case MOTION_NUM::JUMP:
			obj->SetMotion(MOTION_DATA::POSTURE);
			break;

		case MOTION_NUM::GUARD:
			obj->SetMotion(MOTION_DATA::POSTURE);
			break;

		case MOTION_NUM::LANDING:
			obj->SetMotion(MOTION_DATA::POSTURE);
			break;

		case MOTION_NUM::RUN:
			obj->SetMotion(MOTION_DATA::POSTURE);
			break;

		case MOTION_NUM::ATTACK1:
			obj->SetMotion(MOTION_DATA::POSTURE);
			break;

		case MOTION_NUM::ATTACK2:
			obj->SetMotion(MOTION_DATA::POSTURE);
			break;

		case MOTION_NUM::ATTACK3:
			obj->SetMotion(MOTION_DATA::POSTURE);
			break;
		}
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
			attackInfo.type = COLLISION_TYPE::SPHEREVSCAPSULE;
			knockBackInfo.type = KNOCKBACK_TYPE::WEAK;
			break;

		case MODE_STATE::POWERARTS:
			attackInfo.type = COLLISION_TYPE::SPHEREVSCAPSULE;
			knockBackInfo.type = KNOCKBACK_TYPE::MIDDLE;
			break;

		case MODE_STATE::HYPERARTS:
			attackInfo.type =	COLLISION_TYPE::SPHEREVSCYRINDER;
			knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;
			break;
		}
	}