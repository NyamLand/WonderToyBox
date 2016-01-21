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
#include	"Thief_Bullet04.h"
#include	"Stage.h"
#include	"Effect.h"

Thief_Bullet04::Thief_Bullet04() :holdCoinNum(0), growSpeed(0.0f), checkMax(false), checkMin(false)
{
	scale		=	Vector3(0.03f, 0.03f, 0.03f);
	leanpower	=	0;
//	judgeTimer	=	BULLET_JUDGETIMER	[	BULLET_TYPE::THIEF_04	];
	radius		=	BULLET_RADIUS		[	BULLET_TYPE::THIEF_04	];
	limitTimer	=	BULLET_LIMITTIMER	[	BULLET_TYPE::THIEF_04	];
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		isPlayerCheck[i] = false;
	}
}

bool Thief_Bullet04::Initialize()
{
	return true;
}

void	Thief_Bullet04::Update(void)
{
	//	動作
	Move();

//	if (judgeTimer > 0)	judgeTimer--;
//	else							activate = true;

	limitTimer--;

	liveTime++;

	PlayerCollisionCheck();

	if (checkMin == true)
	{
		for (int i = 0; i < holdCoinNum; i++)
		{
			gameManager->AddCoin(playerNum);	//消滅時に奪い取ったコイン加算
		}
		state = false;
	}


	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}


void	Thief_Bullet04::Move(void)
{
	Vector3 front = GetFront();
	front.Normalize();
	growSpeed += 0.05f;
	float addLength = sinf(growSpeed);
	move = front * addLength;
	//打ち出したプレイヤーとの距離
	Vector3 ToPlayerVec = characterManager->GetPos(playerNum) - pos;
	float ToPlayerLength = ToPlayerVec.Length();


	if(!stage->CheckWall(pos, move)) pos += move;

	//縮み始めたら最大まで伸びたものとする(縮み始めている)
	if (addLength <= 0)
	{
		checkMax = true;
	}

	//一度縮んでから再度伸びると最小まで縮んだものとする
	if (addLength >= 0 && checkMax == true || ToPlayerLength <= 0.5f && checkMax == true)
	{
		checkMin = true;
	}
}

//	プレイヤーとのあたりチェック
bool	Thief_Bullet04::PlayerCollisionCheck(void)
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
		float		bullet_r = scale.y * radius;

		bool isHit = Collision::CapsuleVSSphere(p_pos_bottom, p_pos_top, p_r, bulletPos, bullet_r);

		if (isHit)
		{
			isPlayerCheck[i] = true;
			//	エフェクトだす
			float	effectScale = 2.2f;
			particle->Spark(pos, effectScale);
			Vector3	color = characterManager->GetDamageColor(i);
			characterManager->SetPassColor(i, color);
			//画面停止
			gameManager->SetTimeStop(SCREEN_STOPTIME);
			//画面振動
			gameManager->SetShakeCamera(SHAKE_POWER, SHAKE_TIME);


			//	ノックバック
			Vector3	knockBackVec = bulletPos - p_pos_bottom;
			knockBackVec.y = p_pos_bottom.y;
			knockBackVec.Normalize();
			characterManager->SetKnockBackVec(i, -knockBackVec);
			characterManager->SetLeanFrame(i, leanpower);
			characterManager->SetForce(i, 2.0f);
			characterManager->SetMode(i, MODE_STATE::DAMAGE);


			//	プレイヤー番号取得
			int		p2_Num = characterManager->GetPlayerNum(i);

			//	コインがあれば奪い取る
			for (int i = 0; i < 30; i++)
			{
				int		p2_coinNum = gameManager->GetCoinNum(p2_Num);
				if (p2_coinNum > 0)
				{
					gameManager->SubCoin(p2_Num);
					holdCoinNum++;
				}
			}
			return true;
		}
	}
	return false;
}