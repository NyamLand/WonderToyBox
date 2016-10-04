#include	"iextreme.h"
#include	"Random.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"Particle.h"
#include	"CoinManager.h"
#include	"CharacterManager.h"
#include	"BaseBullet.h"
#include	"BulletManager.h"
#include	"ThiefBullet01.h"
#include	"Stage.h"
#include	"Sound.h"




ThiefBullet01::ThiefBullet01()
{
	scale		=	Vector3( 0.05f,0.05f,0.05f );
	angle.y		=	characterManager->GetAngle(playerNum);
//	judgeTimer	=	BULLET_JUDGETIMER	[	BULLET_TYPE::THIEF_01	];
	radius		=	BULLET_RADIUS		[	BULLET_TYPE::THIEF_01	];
	limitTimer	=	BULLET_LIMITTIMER	[	BULLET_TYPE::THIEF_01	];
}

bool ThiefBullet01::Initialize()
{

	return true;
}

void	ThiefBullet01::Update(void)
{
	//	����
	Move();

//	if (judgeTimer > 0)	judgeTimer--;
//	else							activate = true;

	limitTimer--;


	liveTime++;

	if (move.Length() > 0.0f )	PlayerCollisionCheck();	//�ǂɂ��n�ʂɂ��G��Ă��Ȃ��Ƃ��v���C���[�Ƃ����蔻������


	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}


void	ThiefBullet01::Move(void)
{
	ToPlayerLength = 10000.0f;	//�Ƃ肠�������ł�������
	ToPlayerVec;
	frontDot, backDot, upDot, downDot;		//�v���C���[�����Ƃ��ꂼ��̕����̓���
	cross;
	front = GetFront();
	up = GetUp();



	switch (step)
	{
	case STEP::CREATED://�g�����v�o��
		if (liveTime >= 20) step = STEP::WAIT;
		break;
	case STEP::WAIT:
		Wait();
		break;
	case STEP::TARGETING:
		Targeting();
		break;
	case STEP::FIRING:	//����
		Firing();
		break;
	case STEP::STALK:	//�ǐ�
		Stalk();
		break;
	}

	if (StageCollisionCheck()/* || stage->CheckWall(pos, move)*/)	move = Vector3(0, 0, 0);	//�ǂ��n�ʂɐG���ƒ�~
	pos += move;
}


//	�v���C���[�Ƃ̂�����`�F�b�N
bool	ThiefBullet01::PlayerCollisionCheck(void)
{
	for (int i = 0; i < 4; i++)
	{
		if (!activate)	continue;
		if (characterManager->GetUnrivaled(i))	continue;
		if (i == playerNum) continue;	//�������v���C���[�Ȃ珜�O

		//	�v���C���[���ݒ�
		Vector3	p_pos_bottom = characterManager->GetPos(i);
		Vector3	p_pos_top = Vector3(p_pos_bottom.x, p_pos_bottom.y + 3.0f, p_pos_bottom.z);
		float		p_r = 1.0f;

		//	�o���b�g���ݒ�
		Vector3	bulletPos = GetPos();
		//bulletPos.y += 0.5f;
		float		bullet_r = radius;

		bool isHit = Collision::CapsuleVSSphere(p_pos_bottom, p_pos_top, p_r, bulletPos, bullet_r);

		if (isHit)
		{
			int bcMode = characterManager->GetMode(i);
			if (bcMode == MODE_STATE::GUARD)
			{
				sound->PlaySE(SE::GUARD_SE);
				continue;
			}
			//�T�E���h�Đ�
			sound->PlaySE(SE::HIT_SE);
			//	�G�t�F�N�g����
			state = false;
			float	effectScale = 0.2f;
			particle->Spark(p_pos_top, effectScale);

			//	�m�b�N�o�b�N
			Vector3	knockBackVec = bulletPos - p_pos_bottom;
			knockBackVec.y = p_pos_bottom.y;
			knockBackVec.Normalize();
			Vector3	color = Vector3(0.5f, 0.5f, 0.0f);
			characterManager->SetPassColor(i, color);
			characterManager->SetKnockBackVec(i, -knockBackVec);



			//	�v���C���[�ԍ��擾�Ƃ΂�܂��p���[�ݒ�
			float	power = 0.2f;
			int		p2_Num = characterManager->GetPlayerNum(i);

			//int		dropCoin = characterManager->GetTotalPower(i);
			for (int i = 0; i < 2; i++)
			{
				int		p2_coinNum = gameManager->GetCoinNum(p2_Num);
				//	�R�C��������΂΂�T��
				if (p2_coinNum > 0)
				{	
					//	�R�C���΂�܂������ݒ�
					std::uniform_real_distribution<float>	vecrand(-1.0f, 1.0f);
					Vector3	vec = Vector3(Random::GetFloat(-1.0f, 1.0f), 1.0f, Random::GetFloat(-1.0f, 1.0f));
					vec *= 3.0f;
					//vec.Normalize();
					coinManager->Append(p_pos_top, vec, power, Coin::COIN);
					gameManager->SubCoin(p2_Num);
				}
			}
			return true;
		}
	}
	return false;
}

void ThiefBullet01::Wait()
{
	move = Vector3(0, 0, 0);

	//Targeting�ő_���ꏊ��ݒ肵�Ă���
	for (int i = 0; i < 4; i++)
	{
		if (i == playerNum) continue;	//�����̑ł����e�Ȃ珜�O
		Vector3 playerPos = characterManager->GetPos(i);
		Vector3 vec = playerPos - pos;
		float len = vec.Length();
		if (len < ToPlayerLength)
		{
			ToPlayerLength = len;
			ToPlayerVec = characterManager->GetPos(i) - pos;
			frontDot = Vector3Dot(front, ToPlayerVec);
			backDot = Vector3Dot(-front, ToPlayerVec);

			Vector3Cross(cross, front, ToPlayerVec);

		}
	}
	ToPlayerVec.Normalize();

	if (frontDot > backDot)
	{
		if (cross.y < 0) angle.y -= 0.1f;
		else angle.y += 0.1f;

		//�������l�����Ȃ����E�݂̂̊p�x
		Vector3 tempFront = Vector3(front.x, 0.0f, front.z);		
		Vector3 tempToPlayerVec = Vector3(ToPlayerVec.x, 0.0f, ToPlayerVec.z);
		frontDot = GetDegreeAngle(tempFront, tempToPlayerVec);
		if (frontDot <= 10)
		{
			step = STEP::TARGETING;
		}
	}
	else
	{
		if (cross.y < 0) angle.y += 0.1f;
		else angle.y -= 0.1f;

		//�������l�����Ȃ����E�݂̂̊p�x
		Vector3 tempBack = Vector3(-front.x, 0.0f, -front.z);
		Vector3 tempToPlayerVec = Vector3(ToPlayerVec.x, 0.0f, ToPlayerVec.z);
		backDot = GetDegreeAngle(tempBack, tempToPlayerVec);
		if (backDot <= 10)
		{
			step = STEP::TARGETING;
		}
	}
}

void ThiefBullet01::Targeting()
{
	upDot = GetDegreeAngle(up, ToPlayerVec);
	backDot = GetDegreeAngle(-up, ToPlayerVec);

	angle.x += 0.5f;

	if (liveTime >= 40)
	{
		step = STEP::FIRING;
	}
}

void ThiefBullet01::Firing()
{
	move = ToPlayerVec * 1.0f;
	step = STEP::STALK;
}

void ThiefBullet01::Stalk()
{
	for (int i = 0; i < 4; i++)
	{
		if (i == playerNum) continue;	//�����̑ł����e�Ȃ珜�O
		Vector3 playerPos = characterManager->GetPos(i);
		Vector3 vec = playerPos - pos;
		float len = vec.Length();
		if (len < ToPlayerLength)
		{
			ToPlayerLength = len;
			ToPlayerVec = characterManager->GetPos(i) - pos;
		}
	}
	ToPlayerVec.Normalize();

	move.x += ToPlayerVec.x * 0.02f;
	move.z += ToPlayerVec.z * 0.02f;
}