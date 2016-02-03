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
	scale		=	Vector3(0.1f, 0.1f, 0.1f);
	leanpower	=	0;
	power = 2;
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

	//プレイヤーに当たるか地面（障害物除く）に当たると爆破
	if (PlayerCollisionCheck() ||/* Collision::CheckWall(pos, move)*/pos.y <= 0.0f)
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
	//move.y += GRAVITY;
	pos += move;
}

void	Pirate_Bullet03::Explode(void)
{
	radius += 2.0f;
	move = Vector3(0, 0, 0);
	particle->Bomb(Vector3(pos.x,pos.y + 4.0f,pos.z), radius * 0.1f, Vector3(0.7f, 0.2f, 0.1f));
	//particle->Bom(this->pos, radius, 1.0f);
	//爆発範囲が一定以上になったら消去
	if (radius > 20.0f) state = false;
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
		bulletPos.y += 4.0f;
		float		bullet_r = radius;

		bool isHit = Collision::CapsuleVSSphere(p_pos_bottom, p_pos_top, p_r, bulletPos, bullet_r);

		if (isHit)
		{
			isPlayerCheck[i] = true;
			int bcMode = characterManager->GetMode(i);
			if (bcMode == MODE_STATE::GUARD)
			{
				sound->PlaySE(SE::GUARD_SE);
				continue;
			}
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
			//	ライフ減らす
			FOR(0, power) characterManager->SubLife(i);
			//	ノックバック
			Vector3	knockBackVec = bulletPos - p_pos_bottom;
			knockBackVec.y = p_pos_bottom.y;
			knockBackVec.Normalize();
			Vector3	color = characterManager->GetDamageColor(i);
			characterManager->SetPassColor(i, color);
			characterManager->SetKnockBackVec(i, -knockBackVec);
			characterManager->SetLeanFrame(i, leanpower);
			characterManager->SetForce(i, 2.0f);
			(characterManager->GetIsPlayer(i)) ?
				characterManager->SetMode(i, MODE_STATE::DAMAGE) :
				characterManager->SetAIMode(i, AI_MODE_STATE::DAMAGE);
			//characterManager->SetMode(i, MODE_STATE::DAMAGE);

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