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
#include	"Thief_Bullet04.h"

Thief_Bullet04::Thief_Bullet04() :growSpeed(0.0f), checkMax(false), checkMin(false)
{
}

bool Thief_Bullet04::Initialize()
{
	judgeTimer = 10000;
	radius = 100.0f;
	limitTimer = 10 * SECOND;
	activate = true;
	state = true;
	scale = Vector3(0.03f, 0.03f, 0.03f);
	leanpower = 0;
	return true;
}

void	Thief_Bullet04::Update(void)
{
	//	動作
	Move();

	if (judgeTimer > 0)	judgeTimer--;
	else							activate = true;

	limitTimer--;

	PlayerCollisionCheck();

	if (checkMin == true) state = false;


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
	pos += move;

	//縮み始めたら最大まで伸びたものとする(縮み始めている)
	if (addLength <= 0)
	{
		checkMax = true;
	}

	//一度縮んでから再度伸びると最小まで縮んだものとする
	if (addLength >= 0 && checkMax == true)
	{
		checkMin = true;
	}
}

//	プレイヤーとのあたりチェック
bool	Thief_Bullet04::PlayerCollisionCheck(void)
{
	for (int i = 0; i < 4; i++)
	{
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
			characterManager->SetMode(i, MODE_STATE::DAMAGE_LEANBACKWARD);

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
				m_CoinManager->Set(p_pos_top, vec, power);
				gameManager->SubCoin(p2_Num);
			}
			return true;
		}
	}
	return false;
}