
#include	"iextreme.h"
#include	"Random.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"Stage.h"
#include	"Sound.h"
#include	"Particle.h"
#include	"Event_Coin.h"
#include	"Scavenger.h"

#include	"Coin.h"

//******************************************************************************
//
//	Coinクラス
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	グローバル
//-------------------------------------------------------------------------------

#define	GETAWAY_LENGTH		5.0f	//	逃げる判定距離
#define	MAX_HEIGHT					50.0f	//	想定している高さ最大値
#define	MASS							10.0f;	//	質量

//-------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------

	//	コンストラクタ
	Coin::Coin( void ) : obj( nullptr ), moveCheck( true )
	{
		shadow.obj = new iex2DObj( "DATA/Effect/shadow.png" );	
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
		scale = 3.0f;
		judgeTimer = 0;
		type = 0;
		activate = false;
		state = false;
		getAwayflag = false;
		absorbedflg = false;

		//	影構造体初期化
		{
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
		//	判定無効時間減算
		if ( judgeTimer > 0 )		judgeTimer--;
		else							activate = true;

		//	動作
		Move();

		//	逃げる
		if ( getAwayflag )	GetAway();

		//	マグネット
		Magnet();

		//	移動値加算
		pos += move;

		//	影座標設定
		shadow.pos = pos;

		//	擬似慣性
		if ( !absorbedflg )	//掃除屋の吸い込みを受けているときは無効
		{
			move.x *= 0.97f;
			move.z *= 0.97f;
		}

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
		static	float	maxScale = scale * 1.2f;
		shadow.scale = maxScale - ( maxScale * t );

		//	頂点セット
		static	DWORD	vertexColor = 0x55FFFFFF;
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
		int	outId;
		float	height = 0.0f;
		Vector3	tempPos;
		//	下方レイ判定
		float work = stage->GetHeight( pos );
		float objectWork = stage->GetHeightToObject( pos, tempPos, outId );

		//	影設定
		if ( work < objectWork )	height = objectWork;
		else									height = work;

		//	接地判定
		if ( pos.y < work || pos.y < objectWork )
		{
			if ( pos.y < objectWork )
			{
				pos.y = height = objectWork;
				pos += tempPos;
			}
			if ( pos.y < work )					pos.y = height = work;
			move.y = 0.0f;
		}

		//	影高さ設定
		shadow.pos.y = height + 0.1f;
	}

	//	プレイヤーとのあたりチェック
	void	Coin::PlayerCollisionCheck( void )
	{
		//	コインのコリジョンの有効無効を取得
		if ( !gameManager->GetCoinCollision() )	return;

		Vector3	p_pos;
		float r = 0.0f;
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			if ( !activate )	continue;
			if ( characterManager->GetCoinUnrivaled( i ) )	continue;
			//if ( characterManager->GetUnrivaled( i ) )	continue;
			p_pos = characterManager->GetPos( i );
			switch ( type )
			{
			case COIN:
				r = 1.5f;
				break;
			case COIN_BAG_5:
			case COIN_BAG_10:
				r = 2.0f;
				break;
			}
			bool isHit = Collision::CapsuleVSSphere( p_pos, Vector3( p_pos.x, p_pos.y + 3.0f, p_pos.z ), 2.0f, Vector3( pos.x, pos.y + ( scale * 0.5f ), pos.z ), r );

			if ( isHit )
			{
				Hitduringtheoperation(p_pos,i);
			}
		}
	}

	//	動作
	void	Coin::Move( void )
	{
		//	重力加算
		if ( moveCheck )
		if (!absorbedflg) move.y += GRAVITY;
		//吸い込まれる力を質量の代わりとして使用
		if (absorbedflg) move.y += GRAVITY * SCAVENGER::SUCK_POWER;
		
		//	回転
		angle += 0.05f;

		// 反射( ステージ )	
		static float rate = 0.4f;
		stage->GetReflect( pos, move, rate );

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
		float	effectScale = 0.8f;
		particle->CoinUp(pos, effectScale);
		switch ( type )
		{
		case COIN:
			gameManager->AddCoin( Num );
			if (event_coin->GetDubbleInst().eventflag) gameManager->AddCoin( Num );
			break;

		case COIN_BAG_5:
			FOR( 0, 5 )
			{
				gameManager->AddCoin(Num);
				if (event_coin->GetDubbleInst().eventflag) gameManager->AddCoin(Num);
			}
			break;

		case COIN_BAG_10:
			FOR( 0, 10 )
			{
				gameManager->AddCoin( Num );
				if ( event_coin->GetDubbleInst().eventflag )	gameManager->AddCoin( Num );
			}
			break;
		}
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
			//p_pos[i].y = pos.y;

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
		Vector3	p_pos;
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			if ( !activate )	continue;
			if ( !characterManager->GetParameterState( i, PARAMETER_STATE::MAGNET ) )	continue;
			p_pos = characterManager->GetPos( i );
			//p_pos.y = pos.y;

			//	プレイヤーへのベクトル取得
			Vector3	vec = p_pos - pos;
			float	length = vec.Length();

			//	近ければ吸い寄せられる
			if ( length <= 6.0f )
			{
				vec.Normalize();
				move = vec * 0.1f;
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

	//移動量設定
	void	Coin::SetMove(const Vector3& move)
	{
		this->move = move;
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

	//	掃除屋に吸収されている状態フラグ設定
	void	Coin::SetAbsorbedFlag(bool flag)
	{
		this->absorbedflg = flag;
	}
	
	//状態設定
	void	Coin::SetState(bool state)
	{
		this->state = state;
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
