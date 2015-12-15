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
#include	"Thief_Bullet02.h"

Thief_Bullet02::Thief_Bullet02() : holdCoinNum(0), range(0)
{
	scale		=	Vector3(0.05f,0.05f,0.05f);
	leanpower	=	0;
	range		=	0.97f;
//	judgeTimer	=	BULLET_JUDGETIMER	[	BULLET_TYPE::THIEF_02	];
	radius		=	BULLET_RADIUS		[	BULLET_TYPE::THIEF_02	];
	limitTimer	=	BULLET_LIMITTIMER	[	BULLET_TYPE::THIEF_02	];
}

bool Thief_Bullet02::Initialize()
{
	return true;
}

void	Thief_Bullet02::Update(void)
{
	//	動作
	CollectCoin();
	Move();

//	if (judgeTimer > 0)	judgeTimer--;
//	else							activate = true;

	limitTimer--;

	liveTime++;
	PlayerCollisionCheck();

	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}


void	Thief_Bullet02::Move(void)
{
	move.y += GRAVITY; 
	StageCollisionCheck();
	Collision::CheckWall(Vector3(pos.x,pos.y + 2.0f, pos.z), move);
	Vector3 vecBvsP = characterManager->GetPos(playerNum) - pos;	//打ち出したプレイヤーとのベクトル
	vecBvsP.Normalize();

	switch (step)
	{
	case 0:
		move *= range;
		if (move.Length() <= 0.05f) step++;
		break;
	case 1:
		range -= 0.01f;
		move = vecBvsP * (1 - range );	//プレイヤーの方向に徐々に早く
		if (HitCheckVsMyPlayer())
		{
			for (int i = 0; i < holdCoinNum; i++)
			{
				gameManager->AddCoin(playerNum);
			}
			state = false;
		}
		break;
	}
	pos += move;
	//if (StageCollisionCheck()) move = Vector3(0,0,0);
}

void Thief_Bullet02::CollectCoin()
{
	float length = 3.0f;

	//	コイン全体をまわす
	list<Coin*>	coinList = coinManager->GetList();
	FOR_LIST( coinList.begin(), coinList.end() )
	{
		if (( *it )->GetState() == true )
		{
			Vector3 vec = ( *it )->GetPos() - this->pos;
			float lengthBvsC = vec.Length();	//バレットとコインの距離
			vec.Normalize();
			if (lengthBvsC < length)
			{
				( *it )->SetMove(-vec * 2.0f);
			}

			//コインとの距離が近くなればコインを回収
			if (lengthBvsC < 1.5f)
			{
				( *it )->SetState(false);
				holdCoinNum++;
			}
		}
		//if (stayTime > 2 * SECOND)
		//{
		//	stayTime = 0;
		//	absorb_length = DEFAULT_ABSORB_LENGTH;
		//	return true;
		//}
	}
}

//打ち出したプレイヤーとの判定
bool Thief_Bullet02::HitCheckVsMyPlayer()
{
	float length = 0.5f;
	Vector3 vec = characterManager->GetPos(playerNum) - pos;
	if (vec.Length() <= length)
	{
		return true;
	}
	return false;
}