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
#include	"Sound.h"
#include	"Pirate_Bullet03.h"

Pirate_Bullet03::Pirate_Bullet03()
{
	radius		=	BULLET_RADIUS		[	BULLET_TYPE::PIRATE_03	];
	limitTimer	=	BULLET_LIMITTIMER	[	BULLET_TYPE::PIRATE_03	];
	scale		=	Vector3(0.01f, 0.01f, 0.01f);
	leanpower	=	0;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		isPlayerCheck[i] = false;
	}
}

void	Pirate_Bullet03::Update(void)
{

	//	if (judgeTimer > 0)	judgeTimer--;
	//	else							activate = true;

	//limitTimer--;

	liveTime++;

	//

	if (PlayerCollisionCheck() ||/* Collision::CheckWall(pos, move)*/StageCollisionCheck())
	{
		enable = false;
		explosion = true;
	}

	if (explosion) Explode();


	//	動作
	Move();

	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}


void	Pirate_Bullet03::Move(void)
{
	move.y += GRAVITY;
	pos += move;
}



//	プレイヤーとのあたりチェック
bool	Pirate_Bullet03::PlayerCollisionCheck(void)
{
	for (int i = 0; i < 4; i++)
	{
		if (i == playerNum) continue;	//撃ったプレイヤーなら除外
		if (isPlayerCheck[i]) continue;	//同一のプレイヤーと二度以上触れるの禁止
		if (!activate)	continue;
		if (characterManager->GetUnrivaled(i))	continue;

		//	プレイヤー情報設定
		Vector3	p_pos_bottom = characterManager->GetPos(i);
		Vector3	p_pos_top = Vector3(p_pos_bottom.x, p_pos_bottom.y + 3.0f, p_pos_bottom.z);
		float		p_r = 1.0f;

		//	バレット情報設定
		Vector3	bulletPos = GetPos();
		//bulletPos.y += 0.5f;
		float		bullet_r = radius;

		bool isHit = Collision::CapsuleVSSphere(p_pos_bottom, p_pos_top, p_r, bulletPos, bullet_r);

		if (isHit)
		{
			isPlayerCheck[i] = true;
			//	エフェクトだす
			//state = false;
			float	effectScale = 0.2f;
			particle->Spark(p_pos_top, effectScale);

			//画面停止
			gameManager->SetTimeStop(SCREEN_STOPTIME);
			//画面振動
			gameManager->SetShakeCamera(SHAKE_POWER, SHAKE_TIME);
			//サウンド再生
			sound->PlaySE(SE::HYPER_HIT_SE);
			//	ノックバック
			Vector3	knockBackVec = bulletPos - p_pos_bottom;
			knockBackVec.y = p_pos_bottom.y;
			knockBackVec.Normalize();
			Vector3	color = characterManager->GetDamageColor(i);
			characterManager->SetPassColor(i, color);
			characterManager->SetKnockBackVec(i, -knockBackVec);
			characterManager->SetLeanFrame(i, leanpower);
			characterManager->SetForce(i, 2.0f);
			characterManager->SetMode(i, MODE_STATE::DAMAGE);

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
				coinManager->Append(p_pos_top, vec, power, Coin::COIN );
				gameManager->SubCoin(p2_Num);
			}
			return true;
		}
	}
	return false;
}