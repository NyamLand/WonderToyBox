
#include	"iextreme.h"
#include	"Random.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"Sound.h"
#include	"Particle.h"
#include	"Player.h"
#include	"PlayerManager.h"
#include	"GameManager.h"

#include	"Coin.h"

//******************************************************************************
//
//	Coinクラス
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	グローバル
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------

	//	コンストラクタ
	Coin::Coin( void ) : obj( NULL )
	{
		
	}

	//	デストラクタ
	Coin::~Coin( void )
	{
		SafeDelete( obj );
	}

	//	初期化
	bool	Coin::Initialize( void )
	{
		obj = nullptr;
		angle = 0.0f;
		pos = Vector3( 0.0f, 0.0f, 0.0f );
		move = Vector3( 0.0f, 0.0f, 0.0f );
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
	void	Coin::Update( void )
	{
		//	動作
		Move();

		if ( judgeTimer > 0 )	judgeTimer--;
		else							activate = true;

		pos += move;
		StageCollisionCheck();
		PlayerCollisionCheck();

		obj->SetAngle( angle );
		obj->SetPos( pos );
		obj->SetScale( scale );
		obj->Update();
	}

	//	描画
	void	Coin::Render( void )
	{
		obj->Render();

		//	デバッグ用
		if ( !debug )	return;
		DrawSphere( Vector3( pos.x, pos.y + 0.5f, pos.z ), 0.5f, 0xFFFF0000 );
	}

	//	シェーダー付き描画
	void	Coin::Render( iexShader* shader, LPSTR technique )
	{
		obj->Render( shader, technique );
	}

//-------------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------------

	//	ステージ当たり判定チェック
	void	Coin::StageCollisionCheck( void )
	{
		if ( pos.y >= 15.0f )		return;
		float work = Collision::GetHeight( pos );

		if ( pos.y <= work )
		{
			pos.y = work;
			move.y = 0;
		}
	}

	//	プレイヤーとのあたりチェック
	void	Coin::PlayerCollisionCheck( void )
	{
		Vector3	p_pos[4];
		for ( int i = 0; i < 4; i++ )
		{
			if ( !activate )	continue;
			if ( m_Player->GetUnrivaled( i ) )	continue;
			p_pos[i] = m_Player->GetPos( i );
			bool isHit = Collision::CapsuleVSSphere( p_pos[i],Vector3( p_pos[i].x, p_pos[i].y + 3.0f, p_pos[i].z ), 1.0f, Vector3( pos.x, pos.y + 0.5f, pos.z ), 0.5f );

			if ( isHit )
			{
				Hitduringtheoperation(p_pos[i],i);
			}
		}
	}

	//	動作
	void	Coin::Move( void )
	{
		//	重力加算
		move.y += GRAVITY;
		
		//	回転
		angle += 0.05f;

		// 反射( ステージ )	
		static float rate = 0.4f;
		Collision::GetReflect( pos, move, rate );

		//	落下したら再配置
		if ( GetPos().y <= -3.0f )
		{
			std::uniform_real_distribution<float> posrand( -10.0f, 10.0f );
			std::uniform_real_distribution<float> heightrand( 0.0f, 50.0f );
			SetPos( Vector3( Random::GetFloat( -10.0f, 10.0f ), Random::GetFloat( 0.0f, 50.0f ), Random::GetFloat( -10.0f, 10.0f ) ) );
		}
	}

	//	ヒット時動作
	void	Coin::Hitduringtheoperation(const Vector3& pos, const int& Num)
	{
		state = false;
		float	effectScale = 0.2f;
		particle->Spark(pos, effectScale);
		GameManager::AddCoin(Num);
		sound->PlaySE( SE::COIN_SE );
	}

//-------------------------------------------------------------------------------
//	情報設定
//-------------------------------------------------------------------------------

	//	座標設定
	void	Coin::SetPos( const Vector3& pos )
	{
		this->pos = pos; 
	}

	//	向き設定
	void	Coin::SetAngle( const float& angle )
	{
		this->angle = angle; 
	}

	//	スケール設定
	void	Coin::SetScale( const float& scale )
	{
		this ->scale = scale; 
	}

//-------------------------------------------------------------------------------
//	情報取得
//-------------------------------------------------------------------------------

	//	座標取得
	Vector3	Coin::GetPos( void )
	{
		Vector3	out = this->pos;
		return	out; 
	}

	//	向き取得
	float		Coin::GetAngle( void )
	{
		float	out = this->angle;
		return	out; 
	}
	
	//	状態取得
	bool		Coin::GetState()
	{
		return	state;
	}