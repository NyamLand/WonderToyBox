          
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

	//	���[�h�Ǘ�
	void	Princess::ModeManagement( void )
	{
		switch ( mode )
		{
		case PlayerData::MOVE:
			Move();
			break;

		case PlayerData::ATTACK:
		case	PlayerData::POWERARTS:
		case PlayerData::HYPERARTS:
		case PlayerData::QUICKARTS:
			unrivaled = true;
			move = Vector3( 0.0f, 0.0f, 0.0f );
			Attack( mode );
			break;

		case PlayerData::JUMP:
			Jump();
			break;

		case PlayerData::GUARD:
			Guard();
			break;

		case PlayerData::DAMAGE_STRENGTH:
			CommonKnockBackStrength();
			break;

		case PlayerData::DAMAGE:
			Damage();
			break;
		}
	}

	//	���[�hMove
	void	Princess::Move( void )
	{
		CommonMove();

		if ( input->Get( KEY_A ) == 3 )		mode = PlayerData::QUICKARTS;
		if ( input->Get( KEY_B ) == 3 )		mode = PlayerData::POWERARTS;
		if ( input->Get( KEY_C ) == 3 )		mode = PlayerData::HYPERARTS;
		if ( input->Get( KEY_D ) == 3 )		mode = PlayerData::JUMP;
		if ( input->Get( KEY_B7 ) == 3 )	mode = PlayerData::GUARD;
		if ( input->Get( KEY_B10 ) == 3 )
		{
			mode = PlayerData::DAMAGE_STRENGTH;
		}
	}

	//	�N�C�b�N�A�[�c
	bool	Princess::QuickArts( void )
	{
		//	�s�񂩂�O���擾
		Matrix	mat = obj->TransMatrix;
		Vector3	front = Vector3( mat._31, mat._32, mat._33 );
		front.Normalize();

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

	//	���[�hAttack
	void	Princess::Attack( int attackKind )
	{
		SetMotion( motionData.ATTACK1 );
		int		frame = obj->GetFrame();

		bool	isEnd = false;

		switch ( attackKind )
		{
		case PlayerData::QUICKARTS:
			isEnd = QuickArts();
			if ( !isEnd ){
				attackParam = PlayerData::SPHEREVSCAPSULE;
				knockBackType = PlayerData::KNOCKBACK_WEAK;
			}
			break;

		case PlayerData::POWERARTS:
			isEnd = PowerArts();
			if ( !isEnd ){
				attackParam = PlayerData::SPHEREVSCAPSULE;
				knockBackType = PlayerData::KNOCKBACK_MIDDLE;
			}
			break;

		case PlayerData::HYPERARTS:
			isEnd = HyperArts();
			if ( !isEnd ){
				attackParam = PlayerData::SPHEREVSCYRINDER;
				knockBackType = PlayerData::KNOCKBACK_STRENGTH;
			}
			break;
		}

		//	���[�V�����I������
		if ( isEnd )
		{
			mode = PlayerData::MOVE;
			attack_t = 0.0f;
			attack_r = 0.0f;
			attackParam = 0;
			knockBackType = 0;
			unrivaled = false;
		}
	}

	//	���[�hJump
	void	Princess::Jump( void )
	{
		CommonJump();
	}

	//	���[�hGuard
	void	Princess::Guard( void )
	{
		move.x = move.z = 0.0f;
		SetMotion( PrincessData::STAND );
		CommonGuard();
	}

	//	���[�hDamage
	void	Princess::Damage( void )
	{
		CommonKnockBack();
	}