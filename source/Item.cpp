
#include	"iextreme.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"Particle.h"
#include	"Player.h"
#include	"PlayerManager.h"
#include	"GameManager.h"

#include	"Item.h"

//******************************************************************************
//
//	Itemクラス
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------

//	コンストラクタ
Item::Item(void) : obj(NULL)
{

}

//	デストラクタ
Item::~Item(void)
{
	SafeDelete(obj);
}

//	初期化
bool	Item::Initialize(void)
{
	obj = NULL;
	angle = 0.0f;
	pos = Vector3(0.0f, 0.0f, 0.0f);
	move = Vector3(0.0f, 0.0f, 0.0f);
	scale = 0.5f;
	judgeTimer = 0;
	activate = false;
	state = false;

	return	true;
}

//-------------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------------

//	更新
void	Item::Update(void)
{
	//	動作
	Move();

	if (judgeTimer > 0)	judgeTimer--;
	else							activate = true;

	pos += move;
	StageCollisionCheck();
	PlayerCollisionCheck();

	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}

//	描画
void	Item::Render(void)
{
	obj->Render();

	//	デバッグ用
	if (!debug)	return;
	DrawSphere(Vector3(pos.x, pos.y + 0.5f, pos.z), 0.5f, 0xFFFF0000);
}

//	シェーダー付き描画
void	Item::Render(iexShader* shader, LPSTR technique)
{
	obj->Render(shader, technique);
}

//-------------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------------

//	ステージ当たり判定チェック
void	Item::StageCollisionCheck(void)
{
	float work = Collision::GetHeight(pos);

	if (pos.y <= work)
	{
		pos.y = work;
		move.y = 0;
	}
}

//	プレイヤーとのあたりチェック
void	Item::PlayerCollisionCheck(void)
{
	Vector3	p_pos[4];
	for (int i = 0; i < 4; i++)
	{
		if (!activate)	continue;
		if (m_Player->GetUnrivaled(i))	continue;
		p_pos[i] = m_Player->GetPos(i);
		bool isHit = Collision::CapsuleVSSphere(p_pos[i], Vector3(p_pos[i].x, p_pos[i].y + 3.0f, p_pos[i].z), 1.0f, Vector3(pos.x, pos.y + 0.5f, pos.z), 0.5f);

		if (isHit)
		{
			state = false;
			float	effectScale = 0.2f;
			Particle::Spark(p_pos[i], effectScale);
			//m_Player->AddCoin( i );
			GameManager::AddCoin(i);
			IEX_PlaySound(SoundInfo::COIN_SE, false);
		}
	}
}

//	動作
void	Item::Move(void)
{
	//	重力加算
	move.y += GRAVITY;

	//	回転
	angle += 0.05f;

	// 反射( ステージ )	
	static float rate = 0.4f;
	Collision::GetReflect(pos, move, rate);
}

//-------------------------------------------------------------------------------
//	情報設定・取得
//-------------------------------------------------------------------------------

//	設定
void	Item::SetPos(Vector3 pos){ this->pos = pos; }
void	Item::SetAngle(float angle){ this->angle = angle; }
void	Item::SetScale(float scale){ this->scale = scale; }

//	取得
Vector3	Item::GetPos(void){ return	this->pos; }
float		Item::GetAngle(void){ return	this->angle; }
