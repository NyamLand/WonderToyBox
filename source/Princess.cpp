          
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Player.h"
#include	"Princess.h"

//*********************************************************************************
//
//	Princess�N���X
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	�O���[�o��
//-----------------------------------------------------------------------------------

	namespace PrincessData
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
	Princess::Princess( void )
	{
		//	�p�����[�^������
		attack_r = 0.0f;
		attack_t = 0.0f;
		speed = 0.25f;
		scale = 0.02f;
		diffence = -1;
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
		motionData.STAND		=		PrincessData::STAND;
		motionData.POSTURE	=		PrincessData::POSTURE;
		motionData.RUN			=		PrincessData::RUN;
		motionData.ATTACK1		=		PrincessData::ATTACK1;
		motionData.JUMP			=		PrincessData::JUMP;
		motionData.ATTACK2		=		PrincessData::ATTACK2;
		motionData.ATTACK3		=		PrincessData::ATTACK3;
		motionData.GUARD		=		PrincessData::GUARD;
		motionData.POSTURE	=		PrincessData::POSTURE;
	}

//-----------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------

	//	�`��
	void	Princess::Render( iexShader* shader, LPSTR technique )
	{
		CommonRender( shader, technique );

		//	�f�o�b�O�p
		if ( !debug )	return;
		DrawSphere( attackPos, attack_r, 0xFFFFFFFF );
		
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
		//	�s�񂩂�O���擾
		Matrix	mat = GetMatrix();
		Vector3	front = Vector3( mat._31, mat._32, mat._33 );
		front.Normalize();
		move = Vector3( 0.0f, 0.0f, 0.0f );

		Vector3	p_pos = GetPos();
		Vector3	startPos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );
		Vector3	finPos = startPos + front * 5.0f;

		//	�����蔻��ʒu�ړ�&�͈͊g��
		float t = GetBezier( ePrm_t::eRapid_Lv3, ePrm_t::eSlow_Lv5, attack_t );
		Lerp( attackPos, startPos, finPos, t );
		attack_r = 3.0f * t;

		//	�p�����[�^���Z
		attack_t += 0.015f;

		//	���G���
		if ( attack_t <= 0.5f )	unrivaled = true;
		else								unrivaled = false;

		if ( attack_t >= 1.0f )	return	true;
		return	false;
	}

	//	�p���[�A�[�c
	bool	Princess::PowerArts( void )
	{
		Vector3	p_pos = GetPos();
		attackPos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );
		move = Vector3( 0.0f, 0.0f, 0.0f );

		//	�͈͊g��
		Lerp( attack_r, 0.0f, 3.0f, attack_t );

		//	�p�����[�^���Z
		attack_t += 0.02f;

		//	���G���
		if (attack_t <= 0.5f)		unrivaled = true;
		else								unrivaled = false;

		if ( attack_t >= 1.0f )	return	true;
		return	false;
	}

	//	�n�C�p�[�A�[�c
	bool	Princess::HyperArts( void )
	{
		static	int		num = 0;	//	��
		move = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	p_pos = GetPos();
		attackPos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );

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

//-----------------------------------------------------------------------------------
//	���ݒ�
//-----------------------------------------------------------------------------------

	//	�U���p�p�����[�^�ݒ�
	void	Princess::SetAttackParam( int attackKind )
	{
		switch ( attackKind )
		{
		case PlayerData::QUICKARTS:
			attackParam = PlayerData::SPHEREVSCAPSULE;
			knockBackType = PlayerData::KNOCKBACK_WEAK;
			break;

		case PlayerData::POWERARTS:
			attackParam = PlayerData::SPHEREVSCAPSULE;
			knockBackType = PlayerData::KNOCKBACK_MIDDLE;
			break;

		case PlayerData::HYPERARTS:
			attackParam = PlayerData::SPHEREVSCYRINDER;
			knockBackType = PlayerData::KNOCKBACK_STRENGTH;
			break;
		}
	}