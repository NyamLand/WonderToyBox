
#include	"iextreme.h"
#include	"Random.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"Particle.h"
#include	"CoinManager.h"
#include	"Stage.h"
#include	"Sound.h"

#include	"BaseBullet.h"

//******************************************************************************
//
//	Bulletクラス
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	グローバル
//-------------------------------------------------------------------------------

//プレイヤーごとの色
namespace
{
	const Vector3	BULLET_COLOR[] =
	{
		Vector3(1.0f, 0.0f, 0.0f),
		Vector3(0.0f, 0.0f, 1.0f),
		Vector3(1.0f, 1.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(0.0f, 0.0f, 1.0f),
	};
}
//-------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------

	//	コンストラクタ
	BaseBullet::BaseBullet(void) :
	obj(NULL),
	pos(0, 0, 0), move(0, 0, 0),
	limitTimer(0),  number(0),playerNum(0), liveTime(0), step(0), 
	radius(0),
	scale(0,0,0), angle(0,0,0),
	state(true), activate(true)
		{
		}

	//	デストラクタ
	BaseBullet::~BaseBullet(void)
	{
		SafeDelete( obj );
	}

	//	初期化
	//bool	BaseBullet::Initialize(void)
	//{
	//	obj = NULL;
	//	angle = 0.0f;
	//	pos = Vector3( 0.0f, 0.0f, 0.0f );
	//	move = Vector3( 0.0f, 0.0f, 0.0f );
	//	scale = 0.05f;
	//	judgeTimer = 0;
	//	limitTimer = 0;
	//	activate = false;
	//	state = false;
	//	number = 0;

	//	return	true;
	//}

//-------------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------------



	//	描画
	void	BaseBullet::Render(void)
	{
		if(enable) obj->Render();
		//DrawSphere( Vector3( pos.x, pos.y, pos.z ), radius / 5, 0xFFFF0000 );
		//DrawSphere(Vector3(pos.x, pos.y + 4.0f, pos.z), radius, 0xFFFF0000);
	}

	//	シェーダー付き描画
	void	BaseBullet::Render(iexShader* shader, LPSTR technique)
	{
		Vector3 color = BULLET_COLOR[playerNum];
		shader3D->SetValue("BulletColor", color);
		obj->Render(shader, technique);
		
	}

//-------------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------------

	//	ステージ当たり判定チェック
	bool	BaseBullet::StageCollisionCheck(void)
	{
		float work = stage->GetHeight( pos );

		if ( pos.y <= work )
		{
			pos.y = work;
			move.y = 0;
			return true;
		}
	return false;
	}

	//	プレイヤーとのあたりチェック
	bool	BaseBullet::PlayerCollisionCheck(void)
	{
		for ( int i = 0; i < 4; i++ )
		{
			if ( !activate )	continue;
			if (characterManager->GetUnrivaled(i))	continue;
			if (i == playerNum) continue;	//撃ったプレイヤーなら除外
			
			//	プレイヤー情報設定
			Vector3	p_pos_bottom = characterManager->GetPos(i);
			Vector3	p_pos_top = Vector3( p_pos_bottom.x, p_pos_bottom.y + 3.0f, p_pos_bottom.z );
			float		p_r = 1.0f;
			
			//	バレット情報設定
			Vector3	bulletPos = GetPos();
			//bulletPos.y += 0.5f;
			float		bullet_r = radius;

			bool isHit = Collision::CapsuleVSSphere( p_pos_bottom, p_pos_top, p_r, bulletPos, bullet_r );

			if ( isHit )
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
				particle->Spark( p_pos_top, effectScale );

				//	ノックバック
				Vector3	knockBackVec = bulletPos - p_pos_bottom;
				knockBackVec.y = p_pos_bottom.y;
				knockBackVec.Normalize();
				Vector3	color = characterManager->GetDamageColor(i);
				characterManager->SetPassColor(i,color);
				characterManager->SetKnockBackVec(i, -knockBackVec);



				//	プレイヤー番号取得とばらまきパワー設定
				float	power = 0.2f;
				int		dropCoin = characterManager->GetTotalPower(playerNum);
				int		p2_Num = characterManager->GetPlayerNum( i );
				int		p2_coinNum = gameManager->GetCoinNum( p2_Num );

				for (int i = 0; i < dropCoin; i++)
				{
					//	コインがあればばらまき
					if (p2_coinNum > 0)
					{
						//	コインばらまき方向設定
						std::uniform_real_distribution<float>	vecrand(-1.0f, 1.0f);
						Vector3	vec = Vector3(Random::GetFloat(-1.0f, 1.0f), 1.0f, Random::GetFloat(-1.0f, 1.0f));
						vec.Normalize();

						coinManager->Append(p_pos_top, vec, power, Coin::COIN);
						gameManager->SubCoin(p2_Num);
					}
				}
				return true;
			}
		}
	return false;
	}

//-------------------------------------------------------------------------------
//	情報設定・取得
//-------------------------------------------------------------------------------

	//	設定
	void	BaseBullet::SetPos(Vector3 pos){ this->pos = pos; }
	void	BaseBullet::SetAngle(Vector3 angle){ this->angle = angle; }
	void	BaseBullet::SetScale(Vector3 scale){ this->scale = scale; }

	//	取得
	Vector3	BaseBullet::GetPos(void){ return	this->pos; }
	Vector3		BaseBullet::GetAngle(void){ return	this->angle; }


	//	前方取得
	Vector3	BaseBullet::GetFront(void)
	{
		Matrix	mat = obj->TransMatrix;
		Vector3	out = Vector3(mat._31, mat._32, mat._33);
		out.Normalize();
		return	out;
	}

	//	右方取得
	Vector3	BaseBullet::GetRight(void)
	{
		Matrix	mat = obj->TransMatrix;
		Vector3	out = Vector3(mat._11, mat._12, mat._13);
		out.Normalize();
		return	out;
	}

	//	上方取得
	Vector3	BaseBullet::GetUp(void)
	{
		Matrix	mat = obj->TransMatrix;
		Vector3	out = Vector3(mat._21, mat._22, mat._23);
		out.Normalize();
		return	out;
	}


	float BaseBullet::GetDegreeAngle(Vector3 vec1, Vector3 vec2)
	{
		float out;
		out = Vector3Dot(vec1, vec2) / (vec1.Length() * vec2.Length());
		out = acos(out);
		out = out * 180.0f / PI;
		return out;
	}