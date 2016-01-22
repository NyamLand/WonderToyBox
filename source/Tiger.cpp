
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Tiger.h"

//*********************************************************************************
//
//	Tiger�N���X
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
		attackInfo.r = 0.5f;
		attackInfo.t = 0.0f;
		speed = 0.2f;
		scale = 0.02f;
		diffence = -1;
		isGround = true;
		attackCount = 0;
		attackFlag = false;
	}

	//	�f�X�g���N�^
	Tiger::~Tiger( void )
	{

	}

	//	������
	bool	Tiger::Initialize( int playerNum, Vector3 pos )
	{
		//	�e�̏�����
		BaseChara::Initialize( playerNum, pos );

		//	���f���ǂݍ���
		if ( obj == nullptr )
			obj = new iex3DObj( "DATA/CHR/ECCMAN/ECCMAN.IEM" );

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
	void	Tiger::Render( iexShader* shader, LPSTR technique )
	{
		BaseChara::Render( shader, technique );

		//	�f�o�b�O�p
		if ( !debug )	return;
		DrawSphere( attackInfo.pos, attackInfo.r, 0xFFFFFFFF );
		particle->BlueFlame( Vector3( attackInfo.pos.x, attackInfo.pos.y, attackInfo.pos.z ), 0.3f ); 

		char	str[256];
		Vector3	stringPos;
		WorldToClient( pos, stringPos, matView* matProjection );
		stringPos.y -= 150.0f;
		sprintf_s( str, "��\n��\n��" );
		DrawString( str, ( int )stringPos.x, ( int )stringPos.y );
		sprintf_s( str, "attackPos.x = %f\nattackPos.y = %f\nattackPos.z = %f", attackInfo.pos.x, attackInfo.pos.y, attackInfo.pos.z );
		DrawString( str, 100, 500, 0xFF00FFFF );
	}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

	//	�N�C�b�N�A�[�c
	bool	Tiger::QuickArts( void )
	{		
		power = QUICK;

		//����������点�鎞��(�K��)
		leanFrame = 20;
		//	���擾
		Vector3	front = GetFront();
		Vector3	up = GetUp();

		//	���ݒ�
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );
		Vector3	p_pos = GetPos();
		Vector3	startPos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );
		Vector3	finLinePos = startPos + front * 5.0f;
		Vector3	finCurvePos = startPos + front * 3.0f + up * 5.0f;
		Vector3	controlPoint = startPos + front * 5.0f + up * 3.5f;
		float	t = GetBezier( ePrm_t::eRapid_Lv5, ePrm_t::eSlow_Lv5, attackInfo.t );
		attackInfo.r = 0.5f;
		attackInfo.t += 0.03f;
		if ( attackInfo.t >= 1.0f )	attackInfo.t = 1.0f;

		switch ( attackCount )
		{
		case 0:
		case 1:
		case 2:
		case 3:
			//	���`���
			Lerp( attackInfo.pos, startPos, finLinePos, t );

			//	���쒆�Ƀ{�^�������Ǝ��̍U����
			if ( input->Get( KEY_D ) == 3 )
			{
				if ( !attackFlag )
				{
					if ( attackInfo.t >= 0.3f )
					{
						attackFlag = true;
					}
				}
			}
			break;

		default:
			BezierCurve( attackInfo.pos, startPos, controlPoint, finCurvePos, t );
			break;
		}
		
		if ( attackInfo.t >= 1.0f )
		{
			if ( attackFlag )
			{
				attackFlag = false;
				attackCount++;
				attackInfo.t = 0.0f;
			}
			else
			{
				attackInfo.t = 0.0f;
				attackCount = 0;
				attackInfo.pos = GetPos();
				attackFlag = false;
				return	true;
			}
		}
		return	false;
	}

	//	�p���[�A�[�c
	bool	Tiger::PowerArts( void )
	{
		power = POWER;

		//	���G
		SetParameterState(PARAMETER_STATE::UNRIVALED);

		//	���擾
		Vector3	p_pos = GetPos();
		Vector3	front = GetFront();
		Vector3	moveParam = GetMove();
		
		//	����ݒ�
		switch ( attackCount )
		{
		case 0:			
			moveParam = -front * 0.3f;
			attackCount++;
			break;

		case 1:
			p_pos += moveParam;
			SetDrag( 0.9f );

			if ( moveParam.Length() <= 0.01f )
			{
				attackCount = 0;
				return	true;
			}
			break;
		}

		//	���X�V
		SetMove( moveParam );
		SetPos( p_pos );

		return	false;
	}

	//	�n�C�p�[�A�[�c
	bool	Tiger::HyperArts( void )
	{
		power = HYPER;

		leanFrame = 60;
		//	���擾
		static	int		timer = 0;
		Vector3	front = GetFront();
		Vector3	up = GetUp();
		Vector3	right = GetRight();

		//	���ݒ�
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );
		Vector3	p_pos = GetPos();
		Vector3	startPos_right = p_pos + up * 1.5f + right * 0.5f;
		Vector3	startPos_left = p_pos + up * 1.5f + right * -0.5f;
		Vector3	finPos_right = startPos_right + front * 5.0f;
		Vector3	finPos_left = startPos_left + front * 5.0f;
		float	t = GetBezier( ePrm_t::eRapid_Lv5, ePrm_t::eSlow_Lv5, attackInfo.t );
		attackInfo.r = 0.5f;
		attackInfo.t += 0.07f;	//	�X�s�[�h�ݒ�
		if ( attackInfo.t >= 1.0f )	attackInfo.t = 1.0f;

		switch ( attackCount )
		{
		case 20:
			timer++;
			particle->BlueFlame( p_pos, 1.0f );
			
			if ( timer >= 60 )
			{
				attackCount++;
				attackInfo.t = 0.0f;
			}
			break;

		case 21:
			attackInfo.r = 1.0f;
			Lerp( attackInfo.pos, startPos_right, finPos_right, t );

			if ( attackInfo.t >= 1.0f )
			{
				attackInfo.t = 0.0f;
				timer = 0;
				attackCount = 0;
				attackInfo.pos = p_pos;
				attackFlag = false;
				return	true;
			}
			break;

		default:
			//	���`���
			if ( attackCount % 2 == 0 )		Lerp( attackInfo.pos, startPos_right, finPos_right, t );
			else											Lerp( attackInfo.pos, startPos_left, finPos_left, t );

			//	��������]
			if ( input->Get( KEY_AXISX ) < 0 )		angle.y -= 0.02f;
			if ( input->Get( KEY_AXISX ) > 0 )		angle.y += 0.02f;

			if ( attackInfo.t >= 1.0f )
			{
				attackFlag = false;
				attackCount++;
				attackInfo.t = 0.0f;
			}
			break;
		}

		return	false;
	}

	//	���[�V�����Ǘ�
/*	void	Tiger::MotionManagement(int motion)
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
			obj->SetMotion(MOTION_DATA::JUMP);
			break;

		case MOTION_NUM::GUARD:
			obj->SetMotion(MOTION_DATA::GUARD);
			break;

		case MOTION_NUM::LANDING:
			obj->SetMotion(MOTION_DATA::POSTURE);
			break;

		case MOTION_NUM::RUN:
			obj->SetMotion(MOTION_DATA::RUN);
			break;

		case MOTION_NUM::ATTACK1:
			obj->SetMotion(MOTION_DATA::ATTACK1);
			break;

		case MOTION_NUM::ATTACK2:
			obj->SetMotion(MOTION_DATA::ATTACK2);
			break;

		case MOTION_NUM::ATTACK3:
			obj->SetMotion(MOTION_DATA::ATTACK3);
			break;
		}
	}*/

//-----------------------------------------------------------------------------------
//	���ݒ�
//-----------------------------------------------------------------------------------

	//	�U���p�p�����[�^�ݒ�
	void	Tiger::SetAttackParam( int attackKind )
	{
		switch ( attackKind )
		{
		case MODE_STATE::QUICKARTS:
			attackInfo.type = Collision::SPHEREVSCAPSULE;
			if (attackCount < 4)
			{
				knockBackInfo.type = KNOCKBACK_TYPE::LEANBACKWARD;
			}
			if (attackCount >= 4)
			{
				knockBackInfo.type = KNOCKBACK_TYPE::MIDDLE;
			}
			break;

		case MODE_STATE::POWERARTS:
			attackInfo.type = 0;
			knockBackInfo.type = KNOCKBACK_TYPE::MIDDLE;
			break;

		case MODE_STATE::HYPERARTS:
			attackInfo.type = Collision::SPHEREVSCYRINDER;
			if (attackCount < 20) knockBackInfo.type = KNOCKBACK_TYPE::LEANBACKWARD;
			if (attackCount >= 20) knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;
			
			break;
		}
	}