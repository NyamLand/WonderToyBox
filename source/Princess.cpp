          
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Princess.h"
#include	"CoinManager.h"
#include	"Sound.h"
#include	"GameManager.h"

//*********************************************************************************
//
//	Princess�N���X
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	�O���[�o��
//-----------------------------------------------------------------------------------

#define	POWER_ARTS_RADIUS	7.5f

namespace OFFENSIVE_POWER
{
	enum 
	{
		QUICK = 1,
		POWER = 0,
		HYPER = 1,
	};
}
namespace DROP_POWER
{
		enum 
		{
			QUICK = 1,
			POWER = 0,
			HYPER = 1,
		};
	}

//-----------------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Princess::Princess( void ) : BaseChara()
	{
		//	�p�����[�^������
		speed = 0.25f;
		scale = 0.06f;
		diffence = -1;
		hyperNum = 0;
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
			obj = new iex3DObj( "DATA/CHR/�v�����Z�X/new motion/prinsess.mkm.IEM" );

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
	}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

	//	�N�C�b�N�A�[�c
	bool	Princess::QuickArts( void )
	{
		//���[�V�����A�g�f�i�I�X
		SetMotion(2);
		if (obj->GetFrame() >= 75) obj->SetFrame(75);

		attackInfo.power = OFFENSIVE_POWER::QUICK;
		attackInfo.dropPower = DROP_POWER::QUICK;

		if ( !initflag )
		{
			sound->PlaySE( SE::PRINCESS_QUICK );
			initflag = true;
		}

		//SetMotion(PRINCESS::MOTIO_DATA::QUICK);

		//	�s�񂩂�O���擾
		Vector3	front = GetFront();
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );

		Vector3	p_pos = GetPos();
		Vector3	startPos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );
		Vector3	finPos = startPos + front * 5.0f;

		//	�����蔻��ʒu�ړ�&�͈͊g��
		float t = GetBezier( ePrm_t::eRapid_Lv3, ePrm_t::eSlow_Lv5, attackInfo.t );
		Lerp( attackInfo.pos, startPos, finPos, t );
		attackInfo.r = 6.0f * t;

		//	�p�����[�^���Z
		attackInfo.t += 0.015f;

		//	�G�t�F�N�g
		particle->Flower( attackInfo.pos, attackInfo.r, Vector3( 1.0f, 0.4f, 0.4f ) );

		//�����������Ԃ�
		attackInfo.addParam = PARAMETER_STATE::CONFUSION;
		//	���G���
		if (attackInfo.t <= 0.5f) SetParameterState(PARAMETER_STATE::UNRIVALED);
		else					SetUnrivaled(false);

		if ( attackInfo.t >= 1.0f )
		{
			initflag = false;
			return	true;
		}
		return	false;
	}

	//	�p���[�A�[�c
	bool	Princess::PowerArts( void )
	{
		//���[�V�����A�g�f�i�I�X
		SetMotion(3);
		if (obj->GetFrame() >= 114) obj->SetFrame(114);
		if ( !initflag )
		{
			sound->PlaySE( SE::PRINCESS_POWER );
			initflag = true;
		}
		attackInfo.power = OFFENSIVE_POWER::POWER;
		attackInfo.dropPower = DROP_POWER::POWER;
		Vector3	p_pos = GetPos();
		attackInfo.pos = Vector3( p_pos.x, p_pos.y + 3.0f, p_pos.z );
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );

		//	�͈͊g��
		Lerp( attackInfo.r, 0.0f, POWER_ARTS_RADIUS, attackInfo.t );

		//	�p�����[�^���Z
		attackInfo.t += 0.02f;

		//	�G�t�F�N�g
		particle->FlowerDisseminate( attackInfo.pos, attackInfo.r, 0.7f, Vector3( 1.0f, 0.4f, 0.4f ) );

		//�͈͓��ɂ���R�C�����擾
		list<Coin*>	coinList = coinManager->GetList();
		FOR_LIST(coinList.begin(), coinList.end())
		{
			bool	state = (*it)->GetState();
			if ( state )
			{
				if ( Collision::DistCheck( attackInfo.pos, (*it)->GetPos(), attackInfo.r ) )
				{
					(*it)->SetState( false );
					gameManager->AddCoin( playerNum );
				}
			}
		}

		//	���G���
		if (attackInfo.t <= 0.5f)		SetParameterState(PARAMETER_STATE::UNRIVALED);
		else							SetUnrivaled(false);

		if ( attackInfo.t >= 1.0f )
		{
			initflag = false;
			return	true;
		}
		return	false;
	}

	//	�n�C�p�[�A�[�c
	bool	Princess::HyperArts( void )
	{
		//2.4���}���u
		//������������~

		SetMotion(4);
		if (obj->GetFrame() >= 184) obj->SetFrame(184);
		move = Vector3(0, 0, 0);

		attackInfo.power = OFFENSIVE_POWER::HYPER;
		attackInfo.dropPower = DROP_POWER::HYPER;

		if ( !initflag )
		{
			sound->PlaySE( SE::HYPER_ATTACK );
			sound->PlaySE( SE::PRINCESS_HYPER );
			hyperNum = 0;
			initflag = true;
		}

		SetMove( Vector3( 0.0f, 0.0f ,0.0f ) );
		Vector3	p_pos = GetPos();
		attackInfo.top = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );
		attackInfo.bottom = Vector3( p_pos.x, p_pos.y - 1.5f, p_pos.z );
		attackInfo.pos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );

		//	�͈͊g��
		float t = GetBezier( ePrm_t::eSlow_Lv4, ePrm_t::eRapid_Lv1, attackInfo.t );
		Lerp( attackInfo.r, 0.0f, 50.0f, t );

		//	�G�t�F�N�g
		particle->FlowerDisseminate( attackInfo.pos, attackInfo.r, 2.0f, Vector3( 1.0f, 0.4f, 0.4f ) );
		//�����������Ԃ�
		attackInfo.addParam = PARAMETER_STATE::CONFUSION;
		//	�p�����[�^���Z
		attackInfo.t += 0.02f;

		if ( attackInfo.t >= 1.0f )
		{
			switch ( hyperNum )
			{
			case 0:
				hyperNum++;
				attackInfo.t  = 0.0f;
				break;

			case 1:
				hyperNum = 0;
				initflag = false;
				return	true;
				break;
			}
		}
		return	false;
	}

	//	���[�V�����Ǘ�
/*	void	Princess::MotionManagement( int motion )
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
	}*/

//-----------------------------------------------------------------------------------
//	���ݒ�
//-----------------------------------------------------------------------------------

	//	�U���p�p�����[�^�ݒ�
	void	Princess::SetAttackParam( int attackKind )
	{
		switch ( attackKind )
		{
		case MODE_STATE::QUICKARTS:
			attackInfo.type = Collision::SPHEREVSCAPSULE;
			knockBackInfo.type = KNOCKBACK_TYPE::WEAK;
			break;

		case MODE_STATE::POWERARTS:
			//�v���C���[�Ƃ̔���Ȃ�
			break;

		case MODE_STATE::HYPERARTS:
			attackInfo.type = Collision::CAPSULEVSCYRINDER;
			knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;
			break;
		}
	}