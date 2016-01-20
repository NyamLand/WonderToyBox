#include	"iextreme.h"
#include	"Random.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"Particle.h"
#include	"CoinManager.h"
#include	"BaseBullet.h"
#include	"BulletManager.h"
#include	"Pirate_Bullet01.h"
#include	"Stage.h"

Pirate_Bullet01::Pirate_Bullet01() :explosion(false)
{
	radius		=	BULLET_RADIUS		[	BULLET_TYPE::PIRATE_01	];
	limitTimer	=	BULLET_LIMITTIMER	[	BULLET_TYPE::PIRATE_01	];
	scale		=	Vector3(0.01f, 0.01f, 0.01f);
	leanpower	=	0;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		isPlayerCheck[i] = false;
	}
}

bool Pirate_Bullet01::Initialize()
{
	return true;
}

void	Pirate_Bullet01::Update(void)
{

	//	����
	Move();
//	if (judgeTimer > 0)	judgeTimer--;
//	else							activate = true;

	//limitTimer--;

	liveTime++;

	StageCollisionCheck();

	if (PlayerCollisionCheck() || stage->CheckWall(pos, move))
	{
		enable = false;
		explosion = true;
	}

	if (explosion) Explode();



	pos += move;
	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}


void	Pirate_Bullet01::Move(void)
{
	move.y += GRAVITY;
}

void	Pirate_Bullet01::Explode(void)
{
	radius += 1.0f;
	move = Vector3(0, 0, 0);
	particle->Bom(this->pos, radius, 1.0f);
	//�����͈͂����ȏ�ɂȂ��������
	if (radius > 40.0f) state = false;
}

//	�v���C���[�Ƃ̂�����`�F�b�N
bool	Pirate_Bullet01::PlayerCollisionCheck(void)
{
	for (int i = 0; i < 4; i++)
	{
		if (i == playerNum) continue;	//�������v���C���[�Ȃ珜�O
		if (isPlayerCheck[i]) continue;	//����̃v���C���[�Ɠ�x�ȏ�G���̋֎~
		if (!activate)	continue;
		if (characterManager->GetUnrivaled(i))	continue;

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
			isPlayerCheck[i] = true;
			//	�G�t�F�N�g����
			//state = false;
			float	effectScale = 0.2f;
			particle->Spark(p_pos_top, effectScale);

			//	�m�b�N�o�b�N
			Vector3	knockBackVec = bulletPos - p_pos_bottom;
			knockBackVec.y = p_pos_bottom.y;
			knockBackVec.Normalize();
			Vector3	color = characterManager->GetDamageColor(i);
			characterManager->SetPassColor(i, color);
			characterManager->SetKnockBackVec(i, -knockBackVec);
			characterManager->SetLeanFrame(i, leanpower);
			characterManager->SetForce(i, 2.0f);
			characterManager->SetMode(i, MODE_STATE::DAMAGE);

			//	�R�C���΂�܂������ݒ�
			std::uniform_real_distribution<float>	vecrand(-1.0f, 1.0f);
			Vector3	vec = Vector3(Random::GetFloat(-1.0f, 1.0f), 1.0f, Random::GetFloat(-1.0f, 1.0f));
			vec.Normalize();

			//	�v���C���[�ԍ��擾�Ƃ΂�܂��p���[�ݒ�
			float	power = 0.2f;
			int		p2_Num = characterManager->GetPlayerNum(i);
			int		p2_coinNum = gameManager->GetCoinNum(p2_Num);

			//	�R�C��������΂΂�܂�
			if (p2_coinNum > 0)
			{
				coinManager->Append(p_pos_top, vec, power);
				gameManager->SubCoin(p2_Num);
			}
			return true;
		}
	}
	return false;
}