#define	_USE_MATH_DEFINES
#include	<math.h>

#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"Bullet.h"
#include	"BulletManager.h"
#include	"Coin.h"
#include	"CoinManager.h"
#include	"Squirrel.h"


//*********************************************************************************
//
//	Squirrel�N���X
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	�O���[�o��
//-----------------------------------------------------------------------------------

namespace SquirrelData
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
Squirrel::Squirrel(void)
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
Squirrel::~Squirrel(void)
{

}

//	���[�V�����f�[�^�o�^
void	Squirrel::SetMotionData(void)
{
	motionData.STAND = SquirrelData::STAND;
	motionData.POSTURE = SquirrelData::POSTURE;
	motionData.RUN = SquirrelData::RUN;
	motionData.ATTACK1 = SquirrelData::ATTACK1;
	motionData.JUMP = SquirrelData::JUMP;
	motionData.ATTACK2 = SquirrelData::ATTACK2;
	motionData.ATTACK3 = SquirrelData::ATTACK3;
	motionData.GUARD = SquirrelData::GUARD;
	motionData.POSTURE = SquirrelData::POSTURE;
}

//-----------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------

//	�X�V
void	Squirrel::Update(void)
{
	ModeManagement();
	BaseObj::Update();
}

//	�`��
void	Squirrel::Render(iexShader* shader, LPSTR technique)
{
	BaseObj::Render(shader, technique);

	//	�f�o�b�O�p
	if ( !debug ) 	return;
	char	str[256];
	DrawSphere(attackPos, attack_r, 0xFFFFFFFF);
	sprintf_s(str, "pos.x = %f", pos.x);
	DrawString(str, 20, 540);
	sprintf_s(str, "pos.y = %f", pos.y);
	DrawString(str, 20, 560);
	sprintf_s(str, "pos.z = %f", pos.z);
	DrawString(str, 20, 580);
	sprintf_s(str, "angle = %f", angle);
	DrawString(str, 20, 600);
	
	Vector3	stringPos;
	WorldToClient( pos, stringPos, matView* matProjection );
	stringPos.y -= 100.0f;
	sprintf_s( str, "��\n��\n��" );
	DrawString( str, ( int )stringPos.x, ( int )stringPos.y );
}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

//	���[�h�Ǘ�
void	Squirrel::ModeManagement(void)
{
	switch (mode)
	{
	case PlayerData::MOVE:
		Move();
		break;

	case PlayerData::ATTACK:
	case	PlayerData::POWERARTS:
	case PlayerData::HYPERARTS:
	case PlayerData::QUICKARTS:
		move = Vector3(0.0f, 0.0f, 0.0f);
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
		Damage(mode);
		break;
	}
}

//	���[�hMove
void	Squirrel::Move(void)
{
	CommonMove();

	if ( input->Get( KEY_A ) == 3 )		mode = PlayerData::QUICKARTS;
	if ( input->Get( KEY_B ) == 3 )		mode = PlayerData::POWERARTS;
	if ( input->Get( KEY_C ) == 3 )		mode = PlayerData::HYPERARTS;
	if ( input->Get( KEY_D ) == 3 )		mode = PlayerData::JUMP;
	if ( input->Get( KEY_B7 ) == 3 )	mode = PlayerData::GUARD;
	if ( input->Get( KEY_B10 ) == 3 )	mode = PlayerData::DAMAGE_STRENGTH;
}

//	�N�C�b�N�A�[�c
bool	Squirrel::QuickArts(void)
{
	static int time = 0;
	//	�s�񂩂�O���擾
	Matrix	mat = obj->TransMatrix;
	Vector3	front = Vector3( mat._31, mat._32, mat._33 );
	front.Normalize();

	if ( time == 0 )m_BulletManager->Set( pos, front * 5.0f, 0.5f );
	time++;

	if ( time >= 60 )
	{
		time = 0;
		return true;
	}
	return	false;
}

//	�p���[�A�[�c
bool	Squirrel::PowerArts( void )
{
	static int time = 0;

	//	�s�񂩂�O���擾
	Matrix	mat = obj->TransMatrix;
	Vector3	right = Vector3( mat._11, mat._12, mat._13 );
	Vector3	front = Vector3( mat._31, mat._32, mat._33 );
	front.Normalize();
	right.Normalize();

	if ( time == 0 )
	{
		m_BulletManager->Set( pos, front * 5.0f + right * 5.0f, 0.5f );
		m_BulletManager->Set( pos, front * 5.0f, 0.5f );
		m_BulletManager->Set( pos, front * 5.0f + right * -5.0f, 0.5f );
	}
	time++;

	if ( time >= 60 )
	{
		time = 0;
		return true;
	}
	return	false;
}

//	�n�C�p�[�A�[�c
bool	Squirrel::HyperArts( void )
{
	static	int		step = 0;	//	��
	static int time = 0;
	
	Matrix	mat = obj->TransMatrix;
	Vector3	up = Vector3(mat._21, mat._22, mat._23);
	Vector3	front = Vector3(mat._31, mat._32, mat._33);
	front.Normalize();

	switch (step)
	{
	case 0:
		move.y += 0.5f;
		time++;
		if (time >= 20)
		{
			time = 0;
			step++;
		}
		break;
	case 1:
		angle += 0.1f;
		if (time % 16 == 0) m_BulletManager->Set(pos, front * 5.0f + up * -150.0f, 1.0f, 0.5f);
		time++;
		if (time > 16 * 4 - 1) step++;
		break;
	case 2:
		step = 0;
		time = 0;
		return true;
		//step++;
		break;
	}

	return	false;
}

//	���[�hAttack
void	Squirrel::Attack(int attackKind)
{
	SetMotion(motionData.ATTACK1);
	int		frame = obj->GetFrame();

	bool	isEnd = false;

	switch (attackKind)
	{
	case PlayerData::QUICKARTS:
		isEnd = QuickArts();
		if (!isEnd){
			attackParam = PlayerData::SPHEREVSCAPSULE;
			knockBackType = PlayerData::KNOCKBACK_WEAK;
		}
		break;

	case PlayerData::POWERARTS:
		isEnd = PowerArts();
		if (!isEnd){
			attackParam = PlayerData::SPHEREVSCAPSULE;
			knockBackType = PlayerData::KNOCKBACK_MIDDLE;
		}
		break;

	case PlayerData::HYPERARTS:
		isEnd = HyperArts();
		if (!isEnd){
			unrivaled = true;
			attackParam = PlayerData::SPHEREVSCYRINDER;
			knockBackType = PlayerData::KNOCKBACK_STRENGTH;
		}
		break;
	}

	//	���[�V�����I������
	if (isEnd)
	{
		attack_t = 0.0f;
		attack_r = 0.0f;
		attackParam = 0;
		knockBackType = 0;
		unrivaled = false;
		mode = PlayerData::MOVE;
	}
}

//	���[�hJump
void	Squirrel::Jump(void)
{
	CommonJump();
}

//	���[�hGuard
void	Squirrel::Guard(void)
{
	move.x = move.z = 0.0f;
	SetMotion(SquirrelData::STAND);
	CommonGuard();
}

//	���[�hDamage
void	Squirrel::Damage(int type)
{
	CommonKnockBack();
}