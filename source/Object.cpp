
#include	"iextreme.h"
#include	"Random.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Sound.h"
#include	"CharacterManager.h"
#include	"Object.h"

//*******************************************************************************
//
//	Objectクラス
//
//*******************************************************************************

//-------------------------------------------------------------------------------
//	グローバル
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------

	//	コンストラクタ
	Object::Object( void ) : obj( nullptr )
	{
	
	}

	//	デストラクタ
	Object::~Object( void )
	{
		Release();
	}

	//	初期化
	bool	Object::Initialize( void )
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
	
	//	解放
	void	Object::Release( void )
	{
		SafeDelete( obj );
	}

//-------------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------------

	//	更新
	void	Object::Update()
	{
		//	動作
		Move();

		//	判定無効時間
		if ( judgeTimer > 0 )	judgeTimer--;
		else							activate = true;

		//	移動値加算
		pos += move;
		
		//	各当たり判定
		StageCollisionCheck();
		PlayerCollisionCheck();

		//	情報更新
		obj->SetAngle( angle );
		obj->SetPos( pos );
		obj->SetScale( scale );
		obj->Update();
	}

	//	描画
	void	Object::Render( iexShader* shader, LPSTR technique )
	{
		if ( shader == nullptr || technique == nullptr )
		{
			obj->Render();
		}
		else
		{
			obj->Render( shader, technique );
		}
	}

//-------------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------------

	//	動作
	void	Object::Move( void )
	{
		//	重力加算
		move.y += GRAVITY;

		//	回転
		angle += 0.05f;

		//	落下したら再配置
		if ( GetPos().y <= -3.0f )
		{
			float x = Random::GetFloat( -10.0f, 10.0f );
			float y = Random::GetFloat( 0.0f, 50.0f );
			float z = Random::GetFloat( -10.0f, 10.0f );
			SetPos( Vector3( x, y, z ) );
		}
	}

	//	ステージ当たり判定
	void	Object::StageCollisionCheck( void )
	{
		float	work = Collision::GetHeight( pos );

		if ( pos.y < work )
		{
			pos.y = work;
			move.y = 0;
		}
	}

	//	プレイヤー当たり判定
	void	Object::PlayerCollisionCheck( void )
	{
		Vector3	p_pos[PLAYER_MAX];
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			if ( !activate )	continue;
			if ( characterManager->GetUnrivaled( i ) )	continue;
			p_pos[i] = characterManager->GetPos( i );
			bool	isHit = Collision::CapsuleVSSphere( p_pos[i], Vector3( p_pos[i].x, p_pos[i].y + 3.0f, p_pos[i].z ), 1.0f, Vector3( pos.x, pos.y + 0.5f, pos.z ), 0.5f );

			if ( isHit )
			{
				HitDuaringTheOperation( p_pos[i], i );
			}
		}
	}

	//	ヒット時動作
	void	Object::HitDuaringTheOperation( const Vector3& pos, const int& num )
	{
		state = false;
		float	effectScale = 0.2f;
		particle->Spark( pos, effectScale );
		gameManager->AddCoin( num );
		sound->PlaySE( SE::COIN_SE );
	}

//-------------------------------------------------------------------------------
//	情報取得
//-------------------------------------------------------------------------------

	//	座標取得
	Vector3	Object::GetPos( void )const
	{
		return	this->pos;
	}

	//	向き取得
	float		Object::GetAngle( void )const
	{
		return	this->angle;
	}

	//	スケール取得
	bool		Object::GetState( void )const
	{
		return	this->state;
	}

//-------------------------------------------------------------------------------
//	情報設定
//-------------------------------------------------------------------------------

	//	メッシュ設定
	void	Object::SetObj( iexMesh* obj )
	{
		this->obj = obj;
	}

	//	座標設定
	void	Object::SetPos( Vector3 pos )
	{
		this->pos = pos;
	}

	//	向き設定
	void	Object::SetAngle( float angle )
	{
		this->angle = angle;
	}

	//	スケール設定
	void	Object::SetScale( float scale )
	{
		this->scale = scale;
	}

	//	

