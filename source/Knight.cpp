
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Player.h"
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
	Knight::Knight( void )
	{
		//	�p�����[�^������
		attack_r = 0.0f;
		attack_t = 0.0f;
		lance_r = 0.0f;
		power = 2600;
		speed = 0.2f;
		scale = 0.02f;
		SetMotionData();
		isGround = true;
	}

	//	�f�X�g���N�^
	Knight::~Knight( void )
	{

	}

	//	���[�V�����f�[�^�o�^
	void	Knight::SetMotionData( void )
	{
		motionData.STAND = KnightData::STAND;
		motionData.POSTURE = KnightData::POSTURE;
		motionData.RUN = KnightData::RUN;
		motionData.ATTACK1 = KnightData::ATTACK1;
		motionData.JUMP = KnightData::JUMP;
		motionData.ATTACK2 = KnightData::ATTACK2;
		motionData.ATTACK3 = KnightData::ATTACK3;
		motionData.GUARD = KnightData::GUARD;
		motionData.POSTURE = KnightData::POSTURE;
	}

//-----------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------

	//	�`��
	void	Knight::Render( iexShader* shader, LPSTR technique )
	{
		CommonRender( shader, technique );

		//	�f�o�b�O�p
		if ( !debug )	return;
		DrawCapsule( attackPos_bottom, attackPos_top, attack_r, 0xFFFFFFFF );

		char	str[256];
		Vector3	stringPos;
		WorldToClient( pos, stringPos, matView* matProjection );
		stringPos.y -= 170.0f;
		sprintf_s( str, "��\n��\n��\n��" );
		DrawString( str, ( int )stringPos.x, ( int )stringPos.y );
		sprintf_s( str, "x = %f\ny = %f\nz = %f\n", pos.x, pos.y, pos.z );
		DrawString( str, 20, 300 );
	}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

	//	�N�C�b�N�A�[�c
	bool	Knight::QuickArts( void )
	{
		//	�s�񂩂�O���擾
		move = Vector3(0.0f, 0.0f, 0.0f);
		Matrix	mat = GetMatrix();
		Vector3	front = Vector3( mat._31, mat._32, mat._33 );
		front.Normalize();

		Vector3	 p_pos = GetPos();
		Vector3	startPos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );
		Vector3	finPos = startPos + front * 5.0f;

		//	�����蔻��ʒu�ړ�&�͈͊g��
		float t = GetBezier( ePrm_t::eRapid_Lv1, ePrm_t::eSlow_Lv3, attack_t );
		if ( t < 0.5f ){
			Lerp( attackPos_bottom, startPos, finPos, t );
		}
		else{
			Lerp( attackPos_bottom, finPos, startPos, t );
		}
		//	�����蔻��̃p�����[�^��^����
		attack_r = 0.5f;
		attackPos_top = attackPos_bottom + front * 2.0f;
	
		//	�p�����[�^���Z
		attack_t += 0.015f;

		if ( attack_t >= 1.0f )	return	true;
		return	false;
	}

	//	�p���[�A�[�c
	bool	Knight::PowerArts( void )
	{
			//�s�񂩂�O���擾
		move = Vector3(0.0f, 0.0f, 0.0f);
		Matrix	mat = GetMatrix();
		Vector3	right = Vector3( mat._11, mat._12, mat._13 );
		Vector3	front = Vector3( mat._31, mat._32, mat._33 );
		right.Normalize();
		front.Normalize();
		static int step = 0;
		//	�����蔻��ʒu�ړ�&�͈͊g��
		float t = GetBezier( ePrm_t::eRapid_Lv5, ePrm_t::eSlow_Lv1, attack_t );

		switch ( step )
		{
		case 0:
			//	�����蔻��̃p�����[�^��^����
			attack_r = 0.5f;
			step++;
			break;
		case 1:
			Vector3	p_pos = GetPos();
			Vector3 f = front * ( 2.0f * sinf( PI * t ) );
			Vector3 r = -right * ( 2.0f * cosf( PI * t ) );
			attackPos_bottom = p_pos + f + r;
			attackPos_top = attackPos_bottom + f + r;
			//	�p�����[�^���Z
			attack_t += 0.02f;
			break;
		}

		if ( attack_t >= 1.0f ){
			step = 0;
			return	true;
		}
		return	false;
	}

	//	�n�C�p�[�A�[�c
	bool	Knight::HyperArts( void )
	{
		move = Vector3( 0.0f, 0.0f, 0.0f );
		static	int		num = 0;	//	��
		//�s�񂩂�O���擾
		Matrix	mat = GetMatrix();
		Vector3	right = Vector3( mat._11, mat._12, mat._13 );
		Vector3	front = Vector3( mat._31, mat._32, mat._33 );
		right.Normalize();
		front.Normalize();
		static int step = 0;
		Vector3 f, r;

		//	�����蔻��ʒu�ړ�&�͈͊g��
		float t = GetBezier( ePrm_t::eRapid_Lv5, ePrm_t::eSlow_Lv1, attack_t );

		Vector3	p_pos = GetPos();

		switch ( step )
		{
		case 0:
			//	�����蔻��̃p�����[�^��^����
			attack_r = 0.5f;
			step++;
			break;

		case 1:
			//	�E���獶�֓ガ����
			f = front * ( 2.0f * sinf( PI * t ) );
			r = right * ( 2.0f * cosf( PI * t ) );
			SetPos(p_pos + front * 0.1f + -right * 0.1f);
			attackPos_bottom = p_pos + f + r;
			attackPos_top = attackPos_bottom + f + r;
			//	�p�����[�^���Z
			attack_t += 0.02f;
			//	�ガ�����I�����玟��
			if ( attack_t >= 1.0f ){
				attack_t = 0.0f;
				step++;
			}
			break;

		case 2:
			//	������E�֓ガ����
			f = front * ( 2.0f * sinf( PI * t ) );
			r = -right * ( 2.0f * cosf( PI * t ) );
			SetPos(p_pos + front * 0.1f + right * 0.1f);

			attackPos_bottom = p_pos + f + r;
			attackPos_top = attackPos_bottom + f + r;
			//	�p�����[�^���Z
			attack_t += 0.02f;
			//	�ガ�����I�����玟��
			if ( attack_t >= 1.0f ){
				attack_t = 0.0f;
				step++;
			}
			break;

		case 3:
			//	��]�؂�
			attackPos_bottom.x = p_pos.x + 2.0f * cosf( PI / 180 * lance_r );
			attackPos_bottom.z = p_pos.z + 2.0f * sinf( PI / 180 * lance_r );

			attackPos_top.x = p_pos.x + 4.0f * cosf( PI / 180 * lance_r );
			attackPos_top.z = p_pos.z + 4.0f * sinf( PI / 180 * lance_r );
			lance_r += 10.0f;
			if ( lance_r >= 360 * 5 )
			{
				step = 0;
				lance_r = 0.0f;
				return true;
			}
		}

		return false;
	}

//-----------------------------------------------------------------------------------
//	���ݒ�
//-----------------------------------------------------------------------------------

	//	�U���p�p�����[�^�ݒ�
	void	Knight::SetAttackParam( int attackKind )
	{
		switch ( attackKind )
		{
		case PlayerData::QUICKARTS:
			attackParam = PlayerData::COLLISION_TYPE::CAPSULEVSCAPSULE;
			knockBackType = PlayerData::KNOCKBACK_WEAK;
			break;

		case PlayerData::POWERARTS:
			attackParam = PlayerData::COLLISION_TYPE::CAPSULEVSCAPSULE;
			knockBackType = PlayerData::KNOCKBACK_MIDDLE;
			break;

		case PlayerData::HYPERARTS:
			attackParam = PlayerData::COLLISION_TYPE::CAPSULEVSCAPSULE;
			knockBackType = PlayerData::KNOCKBACK_STRENGTH;
			break;
		}
	}




