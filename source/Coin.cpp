
#include	"iextreme.h"
#include	"Random.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"Sound.h"
#include	"Particle.h"

#include	"Coin.h"

//******************************************************************************
//
//	Coinクラス
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	グローバル
//-------------------------------------------------------------------------------

#define	GETAWAY_LENGTH		3.0f	//	逃げる判定距離
#define	MAX_HEIGHT					50.0f	//	想定している高さ最大値

//-------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------

	//	コンストラクタ
	Coin::Coin( void ) : obj( nullptr )
	{
		
	}

	//	デストラクタ
	Coin::~Coin( void )
	{
		SafeDelete( obj );
		SafeDelete( shadow.obj );
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
		getAwayflag = false;

		//	影構造体初期化
		{
			shadow.obj = new iex2DObj( "DATA/Effect/shadow.png" );
			shadow.pos = pos;
			shadow.scale = 1.0f;
			SetVertex( shadow.v[0], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 0.0f, 0.0f, 0xFFFFFFFF );
			SetVertex( shadow.v[1], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 1.0f, 0.0f, 0xFFFFFFFF );
			SetVertex( shadow.v[2], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 0.0f, 1.0f, 0xFFFFFFFF );
			SetVertex( shadow.v[3], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 1.0f, 1.0f, 0xFFFFFFFF );
		}

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

		//	逃げる
		if ( getAwayflag )	GetAway();

		//	マグネット
		Magnet();

		//	判定無効時間減算
		if ( judgeTimer > 0 )	judgeTimer--;
		else							activate = true;

		//	移動値加算
		pos += move;

		//	影座標設定
		shadow.pos = pos;

		//	擬似慣性
		move.x *= 0.9f;
		move.z *= 0.9f;

		//	当たり判定
		StageCollisionCheck();
		PlayerCollisionCheck();
		
		//	影情報更新
		ShadowUpdate();

		//	情報更新
		obj->SetAngle( angle );
		obj->SetPos( pos );
		obj->SetScale( scale );
		obj->Update();
	}

	//	影情報更新
	void	Coin::ShadowUpdate( void )
	{
		//	スケール計算( 高さに応じて影のスケールを調整、影の大きさの最大値はモデルの大きさの2.5倍に設定 )
		float	t = pos.y / MAX_HEIGHT;
		static	float	maxScale = scale * 2.5f;
		shadow.scale = maxScale - ( maxScale * t );

		//	頂点セット
		static	DWORD	vertexColor = 0xFFFFFFFF;
		SetVertex( shadow.v[0], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 0.0f, 0.0f, vertexColor );
		SetVertex( shadow.v[1], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 1.0f, 0.0f, vertexColor );
		SetVertex( shadow.v[2], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 0.0f, 1.0f, vertexColor );
		SetVertex( shadow.v[3], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 1.0f, 1.0f, vertexColor );
	}

	//	描画（ 引数のどちらか一方でも設定しなかったら通常描画 ）
	void	Coin::Render( iexShader* shader, LPSTR technique )
	{
		//	影描画
		iexPolygon::Render3D( shadow.v, 2, shadow.obj, shader3D, "alpha" );

		//	モデル描画
		if ( shader == nullptr || technique == nullptr )
			obj->Render();
		else
			obj->Render( shader, technique );
	}

//-------------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------------

	//	ステージ当たり判定チェック
	void	Coin::StageCollisionCheck( void )
	{
		//	想定しているよりも高くとんでいたらスキップ
		if ( pos.y >= MAX_HEIGHT )	return;
		float work = Collision::GetHeight( pos );
		shadow.pos.y = work + 0.1f;
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
			if ( characterManager->GetUnrivaled( i ) )	continue;
			p_pos[i] = characterManager->GetPos( i );
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
			SetPos( Vector3( Random::GetFloat( -10.0f, 10.0f ), Random::GetFloat( 0.0f, MAX_HEIGHT ), Random::GetFloat( -10.0f, 10.0f ) ) );
		}
	}

	//	ヒット時動作
	void	Coin::Hitduringtheoperation( const Vector3& pos, const int& Num )
	{
		state = false;
		float	effectScale = 0.2f;
		particle->Spark( pos, effectScale );
		gameManager->AddCoin( Num );
		sound->PlaySE( SE::COIN_SE );
	}

	//	逃走
	void	Coin::GetAway( void )
	{
		Vector3	p_pos[4];
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			if ( !activate )	continue;
			p_pos[i] = characterManager->GetPos( i );
			p_pos[i].y = pos.y;

			//	プレイヤーへのベクトル取得
			Vector3	vec = p_pos[i] - pos;
			float	length = vec.Length();

			//	近ければ逃げる
			if ( length <= GETAWAY_LENGTH )
			{
				vec.Normalize();
				move = -vec * 0.1f;
			}
		}
	}

	//	マグネット
	void	Coin::Magnet( void )
	{
		Vector3	p_pos[4];
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			if ( !activate )	continue;
			if ( !characterManager->GetParameterState( i, PARAMETER_STATE::MAGNET ) )	continue;
			p_pos[i] = characterManager->GetPos( i );
			p_pos[i].y = pos.y;

			//	プレイヤーへのベクトル取得
			Vector3	vec = p_pos[i] - pos;
			float	length = vec.Length();

			//	近ければ吸い寄せられる
			if ( length <= GETAWAY_LENGTH )
			{
				vec.Normalize();
				move = vec * 0.01f;
			}
		}
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

	//	逃走フラグ設定
	void	Coin::SetGetAwayFlag( bool flag )
	{
		this->getAwayflag = flag;
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
	bool		Coin::GetState( void )
	{
		return	state;
	}