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
#include	"Thief_Bullet01.h"
#include	"Stage.h"
#include	"Sound.h"




Thief_Bullet01::Thief_Bullet01()
{
	scale		=	Vector3( 0.05f,0.05f,0.05f );
	leanpower	=	0;
	angle.y		=	characterManager->GetAngle(playerNum);
//	judgeTimer	=	BULLET_JUDGETIMER	[	BULLET_TYPE::THIEF_01	];
	radius		=	BULLET_RADIUS		[	BULLET_TYPE::THIEF_01	];
	limitTimer	=	BULLET_LIMITTIMER	[	BULLET_TYPE::THIEF_01	];
}

bool Thief_Bullet01::Initialize()
{

	return true;
}

void	Thief_Bullet01::Update(void)
{
	//	動作
	Move();

//	if (judgeTimer > 0)	judgeTimer--;
//	else							activate = true;

	limitTimer--;


	liveTime++;

	if (move.Length() > 0.0f )	PlayerCollisionCheck();	//壁にも地面にも触れていないときプレイヤーとあたり判定を取る


	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}


void	Thief_Bullet01::Move(void)
{
	ToPlayerLength = 10000.0f;	//とりあえず超でかい数字
	ToPlayerVec;
	frontDot, backDot, upDot, downDot;		//プレイヤー方向とそれぞれの方向の内積
	cross;
	front = GetFront();
	up = GetUp();



	switch (step)
	{
	case STEP::CREATED://トランプ出現
		if (liveTime >= 20) step = STEP::WAIT;
		break;
	case STEP::WAIT:
		Wait();
		break;
	case STEP::TARGETING:
		Targeting();
		break;
	case STEP::FIRING:	//発射
		Firing();
		break;
	case STEP::STALK:	//追跡
		Stalk();
		break;
	}

	if (StageCollisionCheck()/* || stage->CheckWall(pos, move)*/)	move = Vector3(0, 0, 0);	//壁か地面に触れると停止
	pos += move;
}


//	プレイヤーとのあたりチェック
bool	Thief_Bullet01::PlayerCollisionCheck(void)
{
	for (int i = 0; i < 4; i++)
	{
		if (!activate)	continue;
		if (characterManager->GetUnrivaled(i))	continue;
		if (i == playerNum) continue;	//撃ったプレイヤーなら除外

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
			int bcMode = characterManager->GetMode(i);
			if (bcMode == MODE_STATE::GUARD)
			{
				sound->PlaySE(SE::GUARD_SE);
				continue;
			}
			//サウンド再生
			sound->PlaySE(SE::HIT_SE);
			//	エフェクトだす
			state = false;
			float	effectScale = 0.2f;
			particle->Spark(p_pos_top, effectScale);

			//	ノックバック
			Vector3	knockBackVec = bulletPos - p_pos_bottom;
			knockBackVec.y = p_pos_bottom.y;
			knockBackVec.Normalize();
			Vector3	color = Vector3(0.5f, 0.5f, 0.0f);
			characterManager->SetPassColor(i, color);
			characterManager->SetKnockBackVec(i, -knockBackVec);
			characterManager->SetLeanFrame(i, leanpower);
			characterManager->SetMode(i, MODE_STATE::DAMAGE_LEANBACKWARD);

			//	コインばらまき方向設定
			std::uniform_real_distribution<float>	vecrand(-1.0f, 1.0f);
			Vector3	vec = Vector3(Random::GetFloat(-1.0f, 1.0f), 1.0f, Random::GetFloat(-1.0f, 1.0f));
			vec.Normalize();

			//	プレイヤー番号取得とばらまきパワー設定
			float		power = 0.5f;
			int		p2_Num = characterManager->GetPlayerNum( i );
			int		p2_coinNum = gameManager->GetCoinNum( p2_Num );

			//	コインがあればばらまき
			static	int coinNum = 3;
			FOR( 0, coinNum )
			{
				if ( p2_coinNum > 0 )
				{
					coinManager->Append( p_pos_top, vec, power, Coin::COIN );
					gameManager->SubCoin( p2_Num );
				}
			}
			return true;
		}
	}
	return false;
}

void Thief_Bullet01::Wait()
{
	move = Vector3(0, 0, 0);

	//Targetingで狙う場所を設定しておく
	for (int i = 0; i < 4; i++)
	{
		if (i == playerNum) continue;	//自分の打った弾なら除外
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

		//高さを考慮しない左右のみの角度
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

		//高さを考慮しない左右のみの角度
		Vector3 tempBack = Vector3(-front.x, 0.0f, -front.z);
		Vector3 tempToPlayerVec = Vector3(ToPlayerVec.x, 0.0f, ToPlayerVec.z);
		backDot = GetDegreeAngle(tempBack, tempToPlayerVec);
		if (backDot <= 10)
		{
			step = STEP::TARGETING;
		}
	}
}

void Thief_Bullet01::Targeting()
{
	upDot = GetDegreeAngle(up, ToPlayerVec);
	backDot = GetDegreeAngle(-up, ToPlayerVec);

	angle.x += 0.5f;

	if (liveTime >= 40)
	{
		step = STEP::FIRING;
	}
}

void Thief_Bullet01::Firing()
{
	move = ToPlayerVec * 1.0f;
	step = STEP::STALK;
}

void Thief_Bullet01::Stalk()
{
	for (int i = 0; i < 4; i++)
	{
		if (i == playerNum) continue;	//自分の打った弾なら除外
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