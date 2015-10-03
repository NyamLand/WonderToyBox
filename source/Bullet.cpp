
#include	"iextreme.h"
#include	"Random.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Particle.h"
#include	"Player.h"
#include	"PlayerManager.h"
#include	"Coin.h"
#include	"CoinManager.h"

#include	"Bullet.h"

//******************************************************************************
//
//	Bulletクラス
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	グローバル
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------

	//	コンストラクタ
	Bullet::Bullet( void ) : obj( NULL )
	{

	}

	//	デストラクタ
	Bullet::~Bullet( void )
	{
		SafeDelete( obj );
	}

	//	初期化
	bool	Bullet::Initialize( void )
	{
		obj = NULL;
		angle = 0.0f;
		pos = Vector3( 0.0f, 0.0f, 0.0f );
		move = Vector3( 0.0f, 0.0f, 0.0f );
		scale = 0.05f;
		judgeTimer = 0;
		limitTimer = 0;
		activate = false;
		state = false;
		number = 0;
		leanpower = 0;

		return	true;
	}

//-------------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------------

	//	更新
	void	Bullet::Update( void )
	{
		//	動作
		Move();

		if ( judgeTimer > 0 )	judgeTimer--;
		else							activate = true;

		limitTimer++;

		pos += move;
		StageCollisionCheck();
		PlayerCollisionCheck();

		obj->SetAngle( angle );
		obj->SetPos( pos );
		obj->SetScale( scale );
		obj->Update();
	}

	//	描画
	void	Bullet::Render( void )
	{
		obj->Render();
		DrawSphere( Vector3( pos.x, pos.y + 0.5f, pos.z ), scale * 5, 0xFFFF0000 );
	}

	//	シェーダー付き描画
	void	Bullet::Render( iexShader* shader, LPSTR technique )
	{
		obj->Render( shader, technique );
	}

//-------------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------------

	//	ステージ当たり判定チェック
	void	Bullet::StageCollisionCheck( void )
	{
		float work = Collision::GetHeight( pos );

		if ( pos.y <= work )
		{
			pos.y = work;
			move.y = 0;
		}
	}

	//	プレイヤーとのあたりチェック
	void	Bullet::PlayerCollisionCheck( void )
	{
		for ( int i = 0; i < 4; i++ )
		{
			if ( !activate )	continue;
			if ( m_Player->GetUnrivaled( i ) )	continue;
			
			//	プレイヤー情報設定
			Vector3	p_pos_bottom = m_Player->GetPos(i);
			Vector3	p_pos_top = Vector3( p_pos_bottom.x, p_pos_bottom.y + 3.0f, p_pos_bottom.z );
			float		p_r = 1.0f;
			
			//	バレット情報設定
			Vector3	bulletPos = GetPos();
			bulletPos.y += 0.5f;
			float		bullet_r = scale * 5.0f;

			bool isHit = Collision::CapsuleVSSphere( p_pos_bottom, p_pos_top, p_r, bulletPos, bullet_r );

			if ( isHit )
			{
				//	エフェクトだす
				state = false;
				float	effectScale = 0.2f;
				particle->Spark( p_pos_top, effectScale );

				//	ノックバック
				Vector3	knockBackVec = bulletPos - p_pos_bottom;
				knockBackVec.y = p_pos_bottom.y;
				knockBackVec.Normalize();
				Vector3	color = m_Player->GetDamageColor(i);
				m_Player->SetReceiveColor(i,color);
				m_Player->SetKnockBackVec(i, -knockBackVec);
				m_Player->SetLeanFrame(i, leanpower);
				m_Player->SetMode( i, MODE_STATE::DAMAGE_LEANBACKWARD );

				//	コインばらまき方向設定
				std::uniform_real_distribution<float>	vecrand( -1.0f, 1.0f );
				Vector3	vec = Vector3( Random::GetFloat( -1.0f, 1.0f ), 1.0f, Random::GetFloat( -1.0f, 1.0f ) );
				vec.Normalize();

				//	プレイヤー番号取得とばらまきパワー設定
				float	power = 0.2f;
				int		p2_Num = m_Player->GetP_Num( i );
				int		p2_coinNum = gameManager->GetCoinNum( p2_Num );

				//	コインがあればばらまき
				if ( p2_coinNum > 0 )
				{
					m_CoinManager->Set( p_pos_top, vec, power );
					gameManager->SubCoin( p2_Num );
				}
			}
		}
	}

	//	動作
	void	Bullet::Move( void )
	{
		// 反射( ステージ )	
		static float rate = 0.4f;
		Collision::GetReflect( pos, move, rate );
	}

//-------------------------------------------------------------------------------
//	情報設定・取得
//-------------------------------------------------------------------------------

	//	設定
	void	Bullet::SetPos( Vector3 pos ){ this->pos = pos; }
	void	Bullet::SetAngle( float angle ){ this->angle = angle; }
	void	Bullet::SetScale( float scale ){ this->scale = scale; }

	//	取得
	Vector3	Bullet::GetPos( void ){ return	this->pos; }
	float		Bullet::GetAngle( void ){ return	this->angle; }
