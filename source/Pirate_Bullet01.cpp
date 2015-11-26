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

Pirate_Bullet01::Pirate_Bullet01() :explosion(false)
{

}

bool Pirate_Bullet01::Initialize()
{
	activate	=	true;
	state		=	true;
	scale		=	Vector3(0.05f, 0.05f, 0.05f);
	leanpower	=	0;
//	judgeTimer	=	BULLET_JUDGETIMER	[	BULLET_TYPE::PIRATE_01	];
	radius		=	BULLET_RADIUS		[	BULLET_TYPE::PIRATE_01	];
	limitTimer	=	BULLET_LIMITTIMER	[	BULLET_TYPE::PIRATE_01	];
	return true;
}

void	Pirate_Bullet01::Update(void)
{
	//	動作
	Move();

//	if (judgeTimer > 0)	judgeTimer--;
//	else							activate = true;

	limitTimer--;

	StageCollisionCheck();
	if (PlayerCollisionCheck()) explosion = true;

	if (explosion)
	{
		radius += 5.0f;
		move = Vector3(0, 0, 0);
	}

	//爆発範囲が一定以上になったら消去
	if (radius > 150.0f) state = false;


	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}


void	Pirate_Bullet01::Move(void)
{
	move.y += GRAVITY;
	if (Collision::CheckWall(pos, move)) explosion = true;/*state = false*/;
	pos += move;
}


//	プレイヤーとのあたりチェック
bool	Pirate_Bullet01::PlayerCollisionCheck(void)
{
	for (int i = 0; i < 4; i++)
	{
		if (i == playerNum) continue;	//撃ったプレイヤーなら除外
		if (!activate)	continue;
		if (characterManager->GetUnrivaled(i))	continue;

		//	プレイヤー情報設定
		Vector3	p_pos_bottom = characterManager->GetPos(i);
		Vector3	p_pos_top = Vector3(p_pos_bottom.x, p_pos_bottom.y + 3.0f, p_pos_bottom.z);
		float		p_r = 1.0f;

		//	バレット情報設定
		Vector3	bulletPos = GetPos();
		//bulletPos.y += 0.5f;
		float		bullet_r = scale.y * radius;

		bool isHit = Collision::CapsuleVSSphere(p_pos_bottom, p_pos_top, p_r, bulletPos, bullet_r);

		if (isHit)
		{
			//	エフェクトだす
			//state = false;
			float	effectScale = 0.2f;
			particle->Spark(p_pos_top, effectScale);

			//	ノックバック
			Vector3	knockBackVec = bulletPos - p_pos_bottom;
			knockBackVec.y = p_pos_bottom.y;
			knockBackVec.Normalize();
			Vector3	color = characterManager->GetDamageColor(i);
			characterManager->SetPassColor(i, color);
			characterManager->SetKnockBackVec(i, -knockBackVec);
			characterManager->SetLeanFrame(i, leanpower);
			characterManager->SetForce(i, 2.0f);
			characterManager->SetMode(i, MODE_STATE::DAMAGE_FLYUP);

			//	コインばらまき方向設定
			std::uniform_real_distribution<float>	vecrand(-1.0f, 1.0f);
			Vector3	vec = Vector3(Random::GetFloat(-1.0f, 1.0f), 1.0f, Random::GetFloat(-1.0f, 1.0f));
			vec.Normalize();

			//	プレイヤー番号取得とばらまきパワー設定
			float	power = 0.2f;
			int		p2_Num = characterManager->GetPlayerNum(i);
			int		p2_coinNum = gameManager->GetCoinNum(p2_Num);

			//	コインがあればばらまき
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