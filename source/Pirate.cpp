
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"CharacterManager.h"
#include	"GameManager.h"
#include	"BulletManager.h"
#include	"Sound.h"
#include	"Random.h"
#include	"Pirate.h"

//*********************************************************************************
//
//	Pirate�N���X
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	�O���[�o��
//-----------------------------------------------------------------------------------

namespace OFFENSIVE_POWER
{
	enum
	{
		//�N�C�b�N�ƃn�C�p�[�̓o���b�g���ŏ���
		QUICK = 1,
		POWER = 1,
		HYPER = 1,
	};
}

namespace DROP_POWER
{
	enum 
	{
		QUICK = 3,
		POWER = 5,
		HYPER = 15,
	};
}

//-----------------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------------

//	�R���X�g���N�^
Pirate::Pirate(void) : BaseChara()
{
	//	�p�����[�^������
	speed = 0.25f;
	scale = 0.045f;
	diffence = -1;
	isGround = true;
}

//	�f�X�g���N�^
Pirate::~Pirate( void )
{
	SafeDelete( cannon );
}

//	������
bool	Pirate::Initialize(int playerNum, Vector3 pos)
{
	//	�e�̏�����
	BaseChara::Initialize(playerNum, pos);

	//	���f���ǂݍ���
	if (obj == nullptr)
		obj = new iex3DObj("DATA/CHR/Pirate/Pirate.IEM");
	cannon = new iexMesh( "DATA/CHR/Pirate/pirate_cannon.IMO" );

	//	�X�P�[���ݒ�
	obj->SetScale(0.02f);
	obj->Update();
	//cannon->SetScale( 0.02f );



	if (obj == nullptr)	return	false;
	return	true;
}

//-----------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------

//	�`��
void	Pirate::Render(iexShader* shader, LPSTR technique)
{
	SetCannonPos();
	BaseChara::Render( shader, technique );
	if ( renderflag )		cannon->Render( shader, technique );
}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

//	�N�C�b�N�A�[�c
bool	Pirate::QuickArts(void)
{
	attackInfo.power = OFFENSIVE_POWER::QUICK;
	attackInfo.dropPower = DROP_POWER::QUICK;

	SetMotion(PIRATE::MOTION_DATA::QUICK);
	//	�s�񂩂���擾
	Vector3	front = GetFront();
	front.Normalize();
	Vector3	c_pos = Vector3(cannon->TransMatrix._41, cannon->TransMatrix._42, cannon->TransMatrix._43); //�e���΂��ʒu���C�̈ʒu�ɐݒ�

	Vector3 up	  = Vector3(cannon->TransMatrix._21, cannon->TransMatrix._22, cannon->TransMatrix._23);
	Vector3 right = Vector3(cannon->TransMatrix._11, cannon->TransMatrix._12, cannon->TransMatrix._13);
	SetMove(Vector3(0.0f, move.y, 0.0f));
	Vector3 vec = front;

	//�e�𔭎˂���Ƃ��̏�������
	c_pos.y -= 1.0f;
	float	 bulletSpeed = 0.5f;
	int leanpower = 30;
	int playerNum = GetPlayerNum();

	//���˂���e�̎�ނ�����
	int pattern;
	int rnd = Random::GetInt(0, 99);
	if (0 <= rnd && rnd < 70)	pattern = QuickArts_DATA::NORMAL_SHOT;
	if (70 <= rnd && rnd < 100)	pattern = QuickArts_DATA::TIMER_SHOT;

	if (obj->GetFrame() == PIRATE::MOTION_FRAME::QUICKARTS_SHOT)
	{
		switch (pattern)
		{
		case QuickArts_DATA::NORMAL_SHOT:
			m_BulletManager->Set(BULLET_TYPE::PIRATE_01, new PirateBullet01, c_pos, vec, bulletSpeed, playerNum);
			particle->CannonSmoke(c_pos + Vector3(0.0f, 1.0f, 0.0f) + front * 1.0f, front, right, up, 2.0f);
			sound->PlaySE( SE::PIRATE_QUICK_SUCCESS );
			break;
		case QuickArts_DATA::TIMER_SHOT:
			m_BulletManager->Set(BULLET_TYPE::PIRATE_02, new PirateBullet02, c_pos, vec, bulletSpeed, playerNum);
			particle->CannonSmoke(c_pos + Vector3(0.0f, 1.0f, 0.0f) + front * 1.0f, front, right, up, 2.0f);
			sound->PlaySE( SE::PIRATE_QUICK_MISS );
			break;
		}
	}

	//���[�V�����I���܂ōd��
	if (obj->GetFrame() == PIRATE::MOTION_FRAME::QUICKARTS_END)
	{
		return true;
	}
	return	false;
}

//	�p���[�A�[�c
bool	Pirate::PowerArts(void)
{
	attackInfo.power = OFFENSIVE_POWER::POWER;
	attackInfo.dropPower = DROP_POWER::POWER;
	if ( !initflag )
	{
		sound->PlaySE( SE::PIRATE_POWER );
		initflag = true;
	}

	if(attackInfo.t == 0) SetMotion(PIRATE::MOTION_DATA::POWER_START);

	float run_speed = 0.5f;
	SetUnrivaled(false);

	//	�s�񂩂�O���擾
	SetMove(Vector3(0.0f, 0.0f, 0.0f));
	Vector3	front = GetFront();
	Vector3	right = GetRight();
	Vector3	p_pos = GetPos() + front * 2.0f;

	//	�����蔻��̃p�����[�^��^����
	attackInfo.r = 2.0f;
	attackInfo.bottom = p_pos;
	attackInfo.top = attackInfo.bottom + Vector3(0.0f,3.0f,0.0f);

	attackInfo.bottom = p_pos - right * 2.0f;
	attackInfo.top = attackInfo.bottom + right * 4.0f;


	static	float adjustSpeed = 0.02f;
	AngleAdjust(adjustSpeed);

	//	�p�����[�^���Z
	attackInfo.t += 0.015f;

	if (attackInfo.t < 1.0f) move = front * run_speed;
	else SetMotion(PIRATE::MOTION_DATA::POWER_END);

	if (obj->GetFrame() == PIRATE::MOTION_FRAME::POWERARTS_END)
	{
		initflag = false;
		return	true;
	}
	return	false;
}

//	�n�C�p�[�A�[�c
bool	Pirate::HyperArts(void)
{
	attackInfo.power = OFFENSIVE_POWER::HYPER;
	attackInfo.dropPower = DROP_POWER::HYPER;

	if ( !initflag )
	{
		sound->PlaySE( SE::HYPER_ATTACK );
		sound->PlaySE( SE::PIRATE_HYPER );
		initflag = true;
	}

	//��C�̖C�e���ˈʒu����i��ʏ㑤�j
	Vector3 b_pos;
	b_pos = Vector3(0.0f,40.0f,0.0f);

	SetMotion(PIRATE::MOTION_DATA::HYPER);

	float bulletSpeed = 0.3f;
	Vector3 vec(0.0f,-0.5f,0.0f);


	//���[�V�����I�����ɒe����
	if (obj->GetFrame() == PIRATE::MOTION_FRAME::HYPERARTS_END)
	{
		m_BulletManager->Set(BULLET_TYPE::PIRATE_01, new PirateBullet03, b_pos, vec, bulletSpeed, playerNum);
		initflag = false;
		return true;
	}
		
	return false;
}

//	���[�V�����Ǘ�
/*void	Pirate::MotionManagement(int motion)
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

//	��C�ʒu�ݒ�
void	Pirate::SetCannonPos( void )
{
	Matrix	mat;
	mat = *obj->GetBone( 7 ) * obj->TransMatrix;
	cannon->TransMatrix = mat;
	Matrix	cannonMat = cannon->TransMatrix;
	Vector3	up = Vector3( mat._21, mat._22, mat._23 );
	Vector3	cannonPos = Vector3( 0.0f, 0.0f, 0.0f );
	up.Normalize();
	cannonPos = Vector3( cannon->TransMatrix._41, cannon->TransMatrix._42, cannon->TransMatrix._43 );
	cannonPos += up * 0.5f;
	cannon->TransMatrix._41 = cannonPos.x;
	cannon->TransMatrix._42 = cannonPos.y;
	cannon->TransMatrix._43 = cannonPos.z;
}

//-----------------------------------------------------------------------------------
//	���ݒ�
//-----------------------------------------------------------------------------------

//	�U���p�p�����[�^�ݒ�
void	Pirate::SetAttackParam(int attackKind)
{
	switch (attackKind)
	{
	case MODE_STATE::QUICKARTS:
		attackInfo.type = Collision::SPHEREVSCAPSULE;
		break;

	case MODE_STATE::POWERARTS:
		attackInfo.type = Collision::CAPSULEVSCAPSULE;
		knockBackInfo.type = KNOCKBACK_TYPE::MIDDLE;
		break;

	case MODE_STATE::HYPERARTS:
		attackInfo.type = Collision::SPHEREVSCYRINDER;
		knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;
		break;
	}
}