
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Pirate.h"
#include	"CharacterManager.h"
#include	"GameManager.h"
#include	"BulletManager.h"
#include	"Random.h"

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
Pirate::Pirate(void) : BaseChara()
{
	//	�p�����[�^������
	power = 2;/*��*/
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

	//	�X�P�[���ݒ�
	obj->SetScale(0.02f);
	obj->Update();
	cannon = new iexMesh( "DATA/CHR/Pirate/PirateCannon.IMO" );
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
	BaseChara::Render(shader, technique);
	cannon->Render();
}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

//	�N�C�b�N�A�[�c
bool	Pirate::QuickArts(void)
{
	static int time = 0;

	//	�s�񂩂���擾
	Vector3	front = GetFront();
	front.Normalize();
	Vector3	p_pos = GetPos();
	SetMove(Vector3(0.0f, move.y, 0.0f));
	Vector3 vec = front;

	//�e�𔭎˂���Ƃ��̏�������
	p_pos.y += 1.0f;
	float	 bulletSpeed = 0.5f;
	int leanpower = 30;
	int playerNum = GetPlayerNum();

	//���˂���e�̎�ނ�����
	int pattern;
	int rnd = Random::GetInt(0, 99);
	if (0 <= rnd && rnd < 70)	pattern = QuickArts_DATA::NORMAL_SHOT;
	if (70 <= rnd && rnd < 100)	pattern = QuickArts_DATA::TIMER_SHOT;

	if (time == 0)
	{
		switch (pattern)
		{
		case QuickArts_DATA::NORMAL_SHOT:
			m_BulletManager->Set(BULLET_TYPE::PIRATE_01, new Pirate_Bullet01, p_pos, vec, bulletSpeed, playerNum);
			break;
		case QuickArts_DATA::TIMER_SHOT:
			m_BulletManager->Set(BULLET_TYPE::PIRATE_02, new Pirate_Bullet02, p_pos, vec, bulletSpeed, playerNum);
			break;
		}
	}
	time++;

	//��b�ԍd��
	if (time >= 1 * SECOND)
	{
		time = 0;
		return true;
	}
	return	false;
}

//	�p���[�A�[�c
bool	Pirate::PowerArts(void)
{
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

	if (attackInfo.t < 1.0f)move = front * run_speed;

	if (attackInfo.t >= 1.5f)	return	true;
	return	false;
}

//	�n�C�p�[�A�[�c
bool	Pirate::HyperArts(void)
{
	//��C�̖C�e���ˈʒu����i��ʏ㑤�j
	Vector3 b_pos;	
	b_pos = Vector3(
		Random::GetFloat(-20.0f, -20.0f),
		30.0f,
		Random::GetFloat(-20.0f, 20.0f)
		);

	//�����ȊO�̃v���C���[�O�l�𒅒e�n�_�ɂ���
	Vector3 target[3];
	for (int i = 0,n = 0; i < 4; i++)
	{
		if (i == playerNum) continue;	//�����͏��O
		target[n] = characterManager->GetPos(i);
		n++;
	}

	float bulletSpeed = 0.5f;
	Vector3 vec[3];
	for (int i = 0; i < 3; i++)
	{
		Parabola(vec[i], b_pos, target[i], bulletSpeed, GRAVITY);
		m_BulletManager->Set(BULLET_TYPE::PIRATE_01, new Pirate_Bullet03, b_pos, vec[i], bulletSpeed, playerNum);
	}

	return true;
}

//	���[�V�����Ǘ�
void	Pirate::MotionManagement(int motion)
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
}

//	��C�ʒu�ݒ�
void	Pirate::SetCannonPos( void )
{
	Matrix	mat;
	mat = *obj->GetBone( 8 ) * obj->TransMatrix;
	cannon->TransMatrix = mat;
	particle->BlueFlame( Vector3( mat._41, mat._42, mat._43 ), 0.1f );
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