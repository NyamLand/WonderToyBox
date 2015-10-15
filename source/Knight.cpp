
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"

#include	"Knight.h"

//*********************************************************************************
//
//	Knight�N���X
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	�O���[�o��
//-----------------------------------------------------------------------------------
	namespace KnightData
	{
		enum OFFENSIVE_POWER
		{
			QUICK0 = 1,
			QUICK1 = 2,
			QUICK2 = 4,
			POWER = 8,
			HYPER = 5,
		};
	}

//-----------------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Knight::Knight( void ) : BaseChara()
	{
		//	�p�����[�^������
		lance_r = 0.0f;
		power = 5;
		speed = 0.2f;
		scale = 0.1f;
		isGround = true;
	}

	//	�f�X�g���N�^
	Knight::~Knight( void )
	{

	}

//-----------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------

	//	�`��
	void	Knight::Render( iexShader* shader, LPSTR technique )
	{
		BaseChara::Render( shader, technique );

		DrawCapsule( attackInfo.bottom, attackInfo.top, attackInfo.r, 0xFFFFFFFF );
		particle->BlueFlame( attackInfo.top, 0.3f );
		particle->BlueFlame( attackInfo.bottom, 0.3f );




		char	str[256];
		Vector3	stringPos;
		WorldToClient( pos, stringPos, matView* matProjection );
		stringPos.y -= 170.0f;
		sprintf_s( str, "��\n��\n��\n��" );
		DrawString( str, ( int )stringPos.x, ( int )stringPos.y );
		sprintf_s( str, "x = %f\ny = %f\nz = %f\n", pos.x, pos.y, pos.z );
		DrawString( str, 20, 300 );
		sprintf_s( str, "isGround = %d", isGround );
		DrawString( str, 20, 500 );
	}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

	//	�N�C�b�N�A�[�c
	bool	Knight::QuickArts( void )
	{
		//����������点�鎞��(�K��)
		leanFrame = 20;
		//	�s�񂩂�O���擾
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );
		Vector3	front = GetFront();
		Vector3	p_pos = GetPos();
		Vector3	startPos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );
		Vector3	finPos = startPos + front * 5.0f;

		//	�����蔻��ʒu�ړ�&�͈͊g��
		float t = GetBezier( ePrm_t::eRapid_Lv1, ePrm_t::eSlow_Lv3, attackInfo.t );
		if ( t < 0.5f ){
			Lerp( attackInfo.bottom, startPos, finPos, t );
		}
		else{
			Lerp( attackInfo.bottom, finPos, startPos, t );
		}
		//	�����蔻��̃p�����[�^��^����
		attackInfo.r = 0.5f;
		attackInfo.top = attackInfo.bottom + front * 2.0f;
	
		//	�p�����[�^���Z
		attackInfo.t += 0.015f;

		if ( attackInfo.t >= 1.0f )	return	true;
		return	false;
	}

	//	�p���[�A�[�c
	bool	Knight::PowerArts( void )
	{
		//�s�񂩂�O���擾
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );
		Vector3	front = GetFront();
		Vector3	right = GetRight();
		Vector3	p_pos = GetPos();
		static int step = 0;
		//	�����蔻��ʒu�ړ�&�͈͊g��
		float t = GetBezier( ePrm_t::eRapid_Lv5, ePrm_t::eSlow_Lv1, attackInfo.t );
		Vector3 f = front * ( 2.0f * sinf( D3DX_PI * t ) );
		Vector3 r = -right * ( 2.0f * cosf( D3DX_PI * t ) );
		attackInfo.bottom = p_pos + f + r;
		attackInfo.top = attackInfo.bottom + f + r;

		switch ( step )
		{
		case 0:
			//	�����蔻��̃p�����[�^��^����
			attackInfo.r = 0.5f;
			step++;
			break;
		case 1:
			//	�p�����[�^���Z
			attackInfo.t += 0.02f;
			break;
		}

		if ( attackInfo.t >= 1.0f ){
			step = 0;
			return	true;
		}
		return	false;
	}

	//	�n�C�p�[�A�[�c
	bool	Knight::HyperArts( void )
	{
		//����������点�鎞��(�K��)
		leanFrame = 35;
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );
		static	int		num = 0;	//	��
		//�s�񂩂�O���擾
		Vector3	front = GetFront();
		Vector3	right = GetRight();
		Vector3	p_pos = GetPos();
		static int step = 0;
		Vector3 f, r;

		//	�����蔻��ʒu�ړ�&�͈͊g��
		float t = GetBezier( ePrm_t::eRapid_Lv5, ePrm_t::eSlow_Lv1, attackInfo.t );


		switch ( step )
		{
		case 0:
			//	�����蔻��̃p�����[�^��^����
			attackInfo.r = 0.5f;
			step++;
			break;

		case 1:
			//	�p�����[�^���Z
			attackInfo.t += 0.02f;
			
			//	�E���獶�֓ガ����
			f = front * ( 2.0f * sinf( D3DX_PI * t ) );
			r = right * ( 2.0f * cosf( D3DX_PI * t ) );
			p_pos += front * 0.1f + -right * 0.1f;
			attackInfo.bottom = p_pos + f + r;
			attackInfo.top = attackInfo.bottom + f + r;
			
			//	�ガ�����I�����玟��
			if ( attackInfo.t >= 1.0f ){
				attackInfo.t = 0.0f;
				step++;
			}
			break;

		case 2:
			//	������E�֓ガ����
			f = front * ( 2.0f * sinf( D3DX_PI * t ) );
			r = -right * ( 2.0f * cosf( D3DX_PI * t ) );
			p_pos += front * 0.1f + right * 0.1f;
			attackInfo.bottom = p_pos + f + r;
			attackInfo.top = attackInfo.bottom + f + r;
			//	�p�����[�^���Z
			attackInfo.t += 0.02f;
			//	�ガ�����I�����玟��
			if ( attackInfo.t >= 1.0f ){
				attackInfo.t = 0.0f;
				step++;
			}
			break;

		case 3:
			//	��]�؂�
			attackInfo.bottom.x = p_pos.x + 2.0f * cosf( D3DX_PI / 180 * lance_r );
			attackInfo.bottom.z = p_pos.z + 2.0f * sinf( D3DX_PI / 180 * lance_r );
			attackInfo.top.x = p_pos.x + 4.0f * cosf( D3DX_PI / 180 * lance_r );
			attackInfo.top.z = p_pos.z + 4.0f * sinf( D3DX_PI / 180 * lance_r );
			lance_r += 10.0f;
			if ( lance_r >= 360 * 5 )
			{
				step = 0;
				lance_r = 0.0f;
				return true;
			}
			break;
		}

		SetPos( p_pos );

		return false;
	}

	//	���[�V�����Ǘ�
	void	Knight::MotionManagement( int motion )
	{
		switch ( motion )
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
	void	Knight::SetAttackParam( int attackKind )
	{
		switch ( attackKind )
		{
		case MODE_STATE::QUICKARTS:
			attackInfo.type = COLLISION_TYPE::CAPSULEVSCAPSULE;
			if (attackInfo.t < 0.6f) knockBackInfo.type = KNOCKBACK_TYPE::LEANBACKWARD;	//2Hit�܂ł͋�����̂�
			if (attackInfo.t >= 0.6f && attackInfo.t < 0.8f ) knockBackInfo.type = KNOCKBACK_TYPE::WEAK;		//3hit�ڂ���͐�����΂�����
			break;

		case MODE_STATE::POWERARTS:
			attackInfo.type = COLLISION_TYPE::CAPSULEVSCAPSULE;
			knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;
			break;

		case MODE_STATE::HYPERARTS:
			attackInfo.type = COLLISION_TYPE::CAPSULEVSCAPSULE;
			if (attackInfo.t != 0) knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;	//2Hit�܂ł͐�����΂�����
			if (attackInfo.t == 0 && lance_r < 360 * 4) knockBackInfo.type = KNOCKBACK_TYPE::LEANBACKWARD;		//3~6hit�ڂ���͐�����΂�����
			if (attackInfo.t == 0 && lance_r >= 360 * 4) knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;		//6hit�ڂ���͐�����΂�����
			break;
		}
	}




