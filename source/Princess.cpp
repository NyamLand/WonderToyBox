          
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Princess.h"

//*********************************************************************************
//
//	Princess�N���X
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	�O���[�o��
//-----------------------------------------------------------------------------------

	namespace
	{
		enum OFFENSIVE_POWER
		{
			QUICK = 1,
			POWER = 5,
			HYPER = 15,
		};
	}

//-----------------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Princess::Princess( void ) : BaseChara()
	{
		//	�p�����[�^������
		power = 2;/*��*/
		speed = 0.25f;
		scale = 0.04f;
		diffence = -1;
		isGround = true;
	}

	//	�f�X�g���N�^
	Princess::~Princess( void )
	{

	}

	//	������
	bool	Princess::Initialize( int playerNum, Vector3 pos )
	{
		//	�e�̏�����
		BaseChara::Initialize( playerNum, pos );

		//	���f���ǂݍ���
		if( obj == nullptr )
			obj = new iex3DObj( "DATA/CHR/�v�����Z�X/prinsess1.IEM" );

		//	�X�P�[���ݒ�
		obj->SetScale( scale );
		obj->Update();

		if ( obj == nullptr )	return	false;
		return	true;
	}

//-----------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------

	//	�`��
	void	Princess::Render( iexShader* shader, LPSTR technique )
	{
		BaseChara::Render( shader, technique );

		//	�f�o�b�O�p
		if ( !debug )	return;
		DrawSphere( attackInfo.pos, attackInfo.r, 0xFFFFFFFF );
		particle->BlueFlame( Vector3( attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z - attackInfo.r ), 0.3f );
		particle->BlueFlame( Vector3( attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z + attackInfo.r ), 0.3f );
		particle->BlueFlame( Vector3( attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z ), 0.3f );
		particle->BlueFlame( Vector3( attackInfo.pos.x - attackInfo.r, attackInfo.pos.y, attackInfo.pos.z ), 0.3f );
		
		char	str[256];
		Vector3	stringPos;
		WorldToClient( pos, stringPos, matView* matProjection );
		stringPos.y -= 150.0f;
		sprintf_s( str, "��\n��\n��" );
		DrawString( str, ( int )stringPos.x, ( int )stringPos.y );
	}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

	//	�N�C�b�N�A�[�c
	bool	Princess::QuickArts( void )
	{
		power = QUICK;

		//����������点�鎞��(�K��)
		leanFrame = 20;
		//	�s�񂩂�O���擾
		Vector3	front = GetFront();
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );

		Vector3	p_pos = GetPos();
		Vector3	startPos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );
		Vector3	finPos = startPos + front * 5.0f;

		//	�����蔻��ʒu�ړ�&�͈͊g��
		float t = GetBezier( ePrm_t::eRapid_Lv3, ePrm_t::eSlow_Lv5, attackInfo.t );
		Lerp( attackInfo.pos, startPos, finPos, t );
		attackInfo.r = 3.0f * t;

		//	�p�����[�^���Z
		attackInfo.t += 0.015f;

		//	���G���
		if ( attackInfo.t <= 0.5f )	unrivaled = true;
		else								unrivaled = false;

		if ( attackInfo.t >= 1.0f )	return	true;
		return	false;
	}

	//	�p���[�A�[�c
	bool	Princess::PowerArts( void )
	{
		power = POWER;
		Vector3	p_pos = GetPos();
		attackInfo.pos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );

		//	�͈͊g��
		Lerp( attackInfo.r, 0.0f, 3.0f, attackInfo.t );

		//	�p�����[�^���Z
		attackInfo.t += 0.02f;

		//	���G���
		if ( attackInfo.t <= 0.5f )		unrivaled = true;
		else										unrivaled = false;

		if ( attackInfo.t >= 1.0f )	return	true;
		return	false;
	}

	//	�n�C�p�[�A�[�c
	bool	Princess::HyperArts( void )
	{
		power = HYPER;

		static	int		num = 0;	//	��
		SetMove( Vector3( 0.0f, 0.0f ,0.0f ) );
		Vector3	p_pos = GetPos();
		attackInfo.pos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );

		//	�͈͊g��
		float t = GetBezier( ePrm_t::eSlow_Lv4, ePrm_t::eRapid_Lv1, attackInfo.t );
		Lerp( attackInfo.r, 0.0f, 50.0f, t );

		//	�p�����[�^���Z
		attackInfo.t += 0.02f;

		if ( attackInfo.t >= 1.0f )
		{
			switch ( num )
			{
			case 0:
				num++;
				attackInfo.t  = 0.0f;
				break;

			case 1:
				num = 0;
				return	true;
				break;
			}
		}
		return	false;
	}

	//	���[�V�����Ǘ�
	void	Princess::MotionManagement( int motion )
	{
		switch ( motion )
		{
		case MOTION_NUM::STAND:
			obj->SetMotion( MOTION_DATA::STAND );
			break;

		case MOTION_NUM::POSTURE:
			obj->SetMotion( MOTION_DATA::POSTURE );
			break;

		case MOTION_NUM::RUN:
			obj->SetMotion( MOTION_DATA::RUN );
			break;

		case MOTION_NUM::JUMP:
			obj->SetMotion( MOTION_DATA::JUMP );
			break;

		case MOTION_NUM::GUARD:
			obj->SetMotion( MOTION_DATA::GUARD );
			break;

		case MOTION_NUM::FALLTOGROUND:
			obj->SetMotion( MOTION_DATA::FALLTOGROUND );
			break;

		case MOTION_NUM::ATTACK1:
			obj->SetMotion( MOTION_DATA::ATTACK1 );
			break;

		case MOTION_NUM::ATTACK2:
			obj->SetMotion( MOTION_DATA::ATTACK2 );
			break;

		case MOTION_NUM::GETUP:
			SetMotion( MOTION_DATA::GETUP );
			break;

		case MOTION_NUM::KNOCKBACK:
			SetMotion( MOTION_DATA::KNOCKBACK );
			break;
		}
	}

//-----------------------------------------------------------------------------------
//	���ݒ�
//-----------------------------------------------------------------------------------

	//	�U���p�p�����[�^�ݒ�
	void	Princess::SetAttackParam( int attackKind )
	{
		switch ( attackKind )
		{
		case MODE_STATE::QUICKARTS:
			knockBackInfo.isUp = false;
			attackInfo.type = Collision::SPHEREVSCAPSULE;
			if(attackInfo.t < 0.6) knockBackInfo.type = KNOCKBACK_TYPE::LEANBACKWARD;	//2Hit�܂ł͋�����̂�
			if (attackInfo.t >= 0.6) knockBackInfo.type = KNOCKBACK_TYPE::WEAK;		//3hit�ڂ���͐�����΂�����
			break;

		case MODE_STATE::POWERARTS:
			knockBackInfo.isUp = true;
			attackInfo.type = Collision::SPHEREVSCAPSULE;
			knockBackInfo.type = KNOCKBACK_TYPE::MIDDLE;
			break;

		case MODE_STATE::HYPERARTS:
			knockBackInfo.isUp = false;
			attackInfo.type = Collision::SPHEREVSCYRINDER;
			knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;
			break;
		}
	}