
#include	"iextreme.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"Particle.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"PlayerManager.h"

#include	"Coin.h"

//******************************************************************************
//
//	Coinクラス
//
//******************************************************************************

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
		obj = NULL;
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
			p_pos[i] = m_Player->GetPos( i );
			bool isHit = Collision::CapsuleVSSphere( p_pos[i],Vector3( p_pos[i].x, p_pos[i].y + 3.0f, p_pos[i].z ), 1.0f, Vector3( pos.x, pos.y + 0.5f, pos.z ), 0.5f );

			if ( isHit )
			{
				state = false;
				float	effectScale = 0.2f;
				Particle::Spark( p_pos[i], effectScale );
				m_Player->AddCoin( i );
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
	}

//-------------------------------------------------------------------------------
//	情報設定・取得
//-------------------------------------------------------------------------------

	//	設定
	void	Coin::SetPos( Vector3 pos ){ this->pos = pos; }
	void	Coin::SetAngle( float angle ){ this->angle = angle; }
	void	Coin::SetScale( float scale ){ this ->scale = scale; }

	//	取得
	Vector3	Coin::GetPos( void ){ return	this->pos; }
	float		Coin::GetAngle( void ){ return	this->angle; }
	