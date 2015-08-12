
#include	"iextreme.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"Particle.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"PlayerManager.h"

#include	"Bullet.h"

//******************************************************************************
//
//	Bulletクラス
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------

//	コンストラクタ
Bullet::Bullet(void) : obj(NULL)
{

}

//	デストラクタ
Bullet::~Bullet(void)
{
	SafeDelete(obj);
}

//	初期化
bool	Bullet::Initialize(void)
{
	obj = NULL;
	angle = 0.0f;
	pos = Vector3(0.0f, 0.0f, 0.0f);
	move = Vector3(0.0f, 0.0f, 0.0f);
	scale = 0.05f;
	judgeTimer = 0;
	limitTimer = 0;
	activate = false;
	state = false;

	return	true;
}

//-------------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------------

//	更新
void	Bullet::Update(void)
{
	//	動作
	Move();

	if (judgeTimer > 0)	judgeTimer--;
	else							activate = true;

	limitTimer++;

	pos += move;
	StageCollisionCheck();
	PlayerCollisionCheck();

	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}

//	描画
void	Bullet::Render(void)
{
	obj->Render();
	DrawSphere(Vector3(pos.x, pos.y + 0.5f, pos.z), scale * 5, 0xFFFF0000);
}

//	シェーダー付き描画
void	Bullet::Render(iexShader* shader, LPSTR technique)
{
	obj->Render(shader, technique);
}

//-------------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------------

//	ステージ当たり判定チェック
void	Bullet::StageCollisionCheck(void)
{
	float work = Collision::GetHeight(pos);

	if (pos.y <= work)
	{
		pos.y = work;
		move.y = 0;
	}
}

//	プレイヤーとのあたりチェック
void	Bullet::PlayerCollisionCheck(void)
{
	Vector3	p_pos[4];
	for (int i = 0; i < 4; i++)
	{
		if (!activate)	continue;
		if (m_Player->GetUnrivaled(i))	continue;
		p_pos[i] = m_Player->GetPos(i);
		bool isHit = Collision::CapsuleVSSphere(p_pos[i], Vector3(p_pos[i].x, p_pos[i].y + 3.0f, p_pos[i].z), 1.0f, Vector3(pos.x, pos.y + 0.5f, pos.z), scale * 5);

		if (isHit)
		{
			state = false;
			float	effectScale = 0.2f;
			Particle::Spark(p_pos[i], effectScale);
			printf("a");
		}
	}
}

//	動作
void	Bullet::Move(void)
{

	// 反射( ステージ )	
	static float rate = 0.4f;
	Collision::GetReflect(pos, move, rate);
}

//-------------------------------------------------------------------------------
//	情報設定・取得
//-------------------------------------------------------------------------------

//	設定
void	Bullet::SetPos(Vector3 pos){ this->pos = pos; }
void	Bullet::SetAngle(float angle){ this->angle = angle; }
void	Bullet::SetScale(float scale){ this->scale = scale; }

//	取得
Vector3	Bullet::GetPos(void){ return	this->pos; }
float		Bullet::GetAngle(void){ return	this->angle; }
