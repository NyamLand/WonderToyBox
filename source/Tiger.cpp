
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Player.h"
#include	"Tiger.h"

//*********************************************************************************
//
//	Tiger�N���X
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	�O���[�o��
//-----------------------------------------------------------------------------------

	namespace TigerData
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
			QUICK2 = 3,
			POWER = 0,
			HYPER = 10,
		};
	}

//-----------------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Tiger::Tiger( void )
	{
		//	�p�����[�^������
		attack_r = 0.5f;
		attack_t = 0.0f;
		speed = 0.2f;
		scale = 0.02f;
		diffence = -1;
		SetMotionData();
		isGround = true;
		attackCount = 0;
		attackFlag = false;
	}

	//	�f�X�g���N�^
	Tiger::~Tiger( void )
	{

	}

	//	���[�V�����f�[�^�o�^
	void	Tiger::SetMotionData( void )
	{
		motionData.STAND = TigerData::STAND;
		motionData.POSTURE = TigerData::POSTURE;
		motionData.RUN = TigerData::RUN;
		motionData.ATTACK1 = TigerData::ATTACK1;
		motionData.JUMP = TigerData::JUMP;
		motionData.ATTACK2 = TigerData::ATTACK2;
		motionData.ATTACK3 = TigerData::ATTACK3;
		motionData.GUARD = TigerData::GUARD;
		motionData.POSTURE = TigerData::POSTURE;
	}

//-----------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------

	//	�`��
	void	Tiger::Render( iexShader* shader, LPSTR technique )
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
		sprintf_s( str, "attackPos.x = %f\nattackPos.y = %f\nattackPos.z = %f", attackPos.x, attackPos.y, attackPos.z );
		DrawString( str, 100, 500, 0xFF00FFFF );
	}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

	//	�N�C�b�N�A�[�c
	bool	Tiger::QuickArts( void )
	{
		//	���擾
		Matrix	mat = GetMatrix();
		Vector3	front = Vector3( mat._31, mat._32, mat._33 );
		Vector3	up = Vector3( mat._21, mat._22, mat._23 );
		front.Normalize();
		up.Normalize();

		//	���ݒ�
		move = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	p_pos = GetPos();
		Vector3	startPos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );
		Vector3	finLinePos = startPos + front * 5.0f;
		Vector3	finCurvePos = startPos + front * 3.0f + up * 5.0f;
		Vector3	controlPoint = startPos + front * 5.0f + up * 3.5f;
		float	t = GetBezier( ePrm_t::eRapid_Lv5, ePrm_t::eSlow_Lv5, attack_t );
		attack_r = 0.5f;
		attack_t += 0.03f;
		if ( attack_t >= 1.0f )	attack_t = 1.0f;

		switch ( attackCount )
		{
		case 0:
		case 1:
		case 2:
		case 3:
			//	���`���
			Lerp( attackPos, startPos, finLinePos, t );

			//	���쒆�Ƀ{�^�������Ǝ��̍U����
			if ( input->Get( KEY_A ) == 3 )
			{
				if ( !attackFlag )
				{
					if ( attack_t >= 0.3f )
					{
						attackFlag = true;
					}
				}
			}
			break;

		default:
			BezierCurve( attackPos, startPos, controlPoint, finCurvePos, t );
			break;
		}
		
		if ( attack_t >= 1.0f )
		{
			if ( attackFlag )
			{
				attackFlag = false;
				attackCount++;
				attack_t = 0.0f;
			}
			else
			{
				attack_t = 0.0f;
				attackCount = 0;
				attackPos = GetPos();
				attackFlag = false;
				return	true;
			}
		}
		return	false;
	}

	//	�p���[�A�[�c
	bool	Tiger::PowerArts( void )
	{
		//	���G
		unrivaled = true;

		//	���擾
		Matrix	mat = GetMatrix();
		Vector3	front = Vector3( mat._31, mat._32, mat._33 );
		front.Normalize();
		move = Vector3( 0.0f, 0.0f, 0.0f );
		
		//	����ݒ�
		switch ( attackCount )
		{
		case 0:			
			move = -front * 0.3f;
			attackCount++;
			break;

		case 1:
			pos += move;
			move *= 0.9f;
			if ( move.Length() <= 0.01f )
			{
				attackCount = 0;
				return	true;
			}
			break;
		}

		return	false;
	}

	//	�n�C�p�[�A�[�c
	bool	Tiger::HyperArts( void )
	{
		//	���擾
		static	int		timer = 0;
		Matrix	mat = GetMatrix();
		Vector3	front = Vector3( mat._31, mat._32, mat._33 );
		Vector3	up = Vector3( mat._21, mat._22, mat._23 );
		Vector3	right = Vector3( mat._11, mat._12, mat._13 );
		front.Normalize();
		up.Normalize();
		right.Normalize();

		//	���ݒ�
		move = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	p_pos = GetPos();
		Vector3	startPos_right = p_pos + up * 1.5f + right * 0.5f;
		Vector3	startPos_left = p_pos + up * 1.5f + right * -0.5f;
		Vector3	finPos_right = startPos_right + front * 5.0f;
		Vector3	finPos_left = startPos_left + front * 5.0f;
		float	t = GetBezier( ePrm_t::eRapid_Lv5, ePrm_t::eSlow_Lv5, attack_t );
		attack_r = 0.5f;
		attack_t += 0.07f;	//	�X�s�[�h�ݒ�
		if ( attack_t >= 1.0f )	attack_t = 1.0f;

		switch ( attackCount )
		{
		case 20:
			timer++;
			Particle::BlueFlame( p_pos, 1.0f );
			
			if ( timer >= 60 )
			{
				attackCount++;
				attack_t = 0.0f;
			}
			break;

		case 21:
			attack_r = 1.0f;
			Lerp( attackPos, startPos_right, finPos_right, t );

			if ( attack_t >= 1.0f )
			{
				attack_t = 0.0f;
				timer = 0;
				attackCount = 0;
				attackPos = p_pos;
				attackFlag = false;
				return	true;
			}
			break;

		default:
			//	���`���
			if ( attackCount % 2 == 0 )		Lerp( attackPos, startPos_right, finPos_right, t );
			else											Lerp( attackPos, startPos_left, finPos_left, t );

			//	��������]
			if ( input->Get( KEY_AXISX ) < 0 )		angle -= 0.02f;
			if ( input->Get( KEY_AXISX ) > 0 )		angle += 0.02f;

			if ( attack_t >= 1.0f )
			{
				attackFlag = false;
				attackCount++;
				attack_t = 0.0f;
			}
			break;
		}

		return	false;
	}

//-----------------------------------------------------------------------------------
//	���ݒ�
//-----------------------------------------------------------------------------------

	//	�U���p�p�����[�^�ݒ�
	void	Tiger::SetAttackParam( int attackKind )
	{
		switch ( attackKind )
		{
		case PlayerData::QUICKARTS:
			attackParam = PlayerData::SPHEREVSCAPSULE;
			knockBackType = PlayerData::KNOCKBACK_WEAK;
			break;

		case PlayerData::POWERARTS:
			attackParam = 0;
			knockBackType = PlayerData::KNOCKBACK_MIDDLE;
			break;

		case PlayerData::HYPERARTS:
			attackParam = PlayerData::SPHEREVSCYRINDER;
			knockBackType = PlayerData::KNOCKBACK_STRENGTH;
			break;
		}
	}