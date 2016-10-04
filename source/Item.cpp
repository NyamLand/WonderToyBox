
#include	"iextreme.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"Random.h"
#include	"Sound.h"
#include	"Particle.h"
#include	"CharacterManager.h"
#include	"GameManager.h"
#include	"Stage.h"
#include	"Effect.h"

#include	"Item.h"

//******************************************************************************
//
//	Itemクラス
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	グローバル
//-------------------------------------------------------------------------------
#define	ITEM_SCALE				2.0f
#define	DELETE_TIME				7
#define	ROTATIONAL_SPEED	0.05f
#define	COLLISION_SCALE		2.0f
#define	MAX_HEIGHT				50.0f
#define	RELOCATION_HEIGHT	-3.0f

//-------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------

	//	コンストラクタ
	Item::Item( void ) : obj( nullptr ),
		pos( ZERO_VECTOR3 ), move( ZERO_VECTOR3 ),
		scale( ITEM_SCALE ), angle( 0.0f ), stageHeight( 0.0f ),
		state( true ),
		type( 0 ),	time( 0 )
	{

	}

	//	デストラクタ
	Item::~Item(void)
	{
		SafeDelete( shadow.obj );
	}

	//	初期化5
	bool	Item::Initialize( void )
	{
		Load();
		return	true;
	}

	//	読み込み
	void	Item::Load( void )
	{
		shadow.obj = new iex2DObj( "DATA/Effect/shadow.png" );
	}

//-------------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------------

	//	更新
	void	Item::Update( void )
	{
		//	動作
		Move();

		//	移動値加算
		pos += move;

		//	影座標設定
		shadow.pos = pos;

		//	当たり判定
		StageCollisionCheck();
		PlayerCollisionCheck();

		//	影情報更新
		ShadowUpdate();

		//	オブジェクト更新
		ObjectUpdate();
	}

	//	描画
	void	Item::Render( iexShader* shader, LPSTR technique )
	{
		//	影描画
		iexPolygon::Render3D( shadow.v, 2, shadow.obj, shader3D, "alpha" );

		//	モデル描画
		if ( shader == nullptr || technique == nullptr )
		{
			obj->Render();
		}
		else
		{
			obj->Render( shader, technique );
		}
	}

	//	オブジェクト更新
	void	Item::ObjectUpdate( void )
	{
		//	情報更新
		obj->SetAngle( angle );
		obj->SetScale( scale );
		obj->SetPos( pos );
		obj->Update();
	}

	//	影更新
	void	Item::ShadowUpdate( void )
	{
		//	スケール計算（ 高さに応じて影のスケールを調整、影の大きさの最大値はモデルの大きさの2.5倍に設定 ）
		float	t = pos.y / MAX_HEIGHT;
		static	float		MAX_SCALE = scale * 1.2f;
		shadow.scale = MAX_SCALE - ( MAX_SCALE * t );

		//	頂点セット
		static	DWORD	vertexColor = 0x55FFFFFF;
		SetVertex( shadow.v[0], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 0.0f, 0.0f, vertexColor );
		SetVertex( shadow.v[1], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 1.0f, 0.0f, vertexColor );
		SetVertex( shadow.v[2], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 0.0f, 1.0f, vertexColor );
		SetVertex( shadow.v[3], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 1.0f, 1.0f, vertexColor );
	}

//-------------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------------

	//	動作
	void	Item::Move( void )
	{
		//	重力加算
		move.y += GRAVITY;

		//	回転
		angle += ROTATIONAL_SPEED;

		//	タイマー加算
		time++;

		//	DELETE_TIME秒後に消滅
		if ( time >= DELETE_TIME * SECOND )	state = false;
	}

	//	ヒット時動作
	void	Item::Hitduringtheoperation( const Vector3& pos, const int& playerNum )
	{
		state = false;
		static	float	effectScale = 0.2f;
		particle->Spark( pos, effectScale );
		sound->PlaySE( SE::ITEM_SE );
		m_Effect->StateEffectSet( playerNum, type );
	}

	//	ステージとの当たり判定
	void	Item::StageCollisionCheck( void )
	{		
		//	地面高さ取得
		stageHeight = stage->GetHeight( pos );

		//	影高さ設定
		shadow.pos.y = stageHeight + 0.1f;

		//	接地判定
		if ( pos.y < stageHeight )
		{
			pos.y = stageHeight;
			move.y = 0.0f;
		}

		//	落下したら再配置
		if ( GetPos().y <= RELOCATION_HEIGHT )
		{
			SetPos( Vector3( Random::GetFloat( -10.0f, 10.0f ), 
				Random::GetFloat( 0.0f, MAX_HEIGHT ),
				Random::GetFloat( -10.0f, 10.0f ) ) );
		}
	}

	//	プレイヤーとの当たり判定
	void	Item::PlayerCollisionCheck( void )
	{
		//	コインのコリジョンの有効無効を取得
		if ( !gameManager->GetCoinCollision() )	return;

		Vector3	p_pos = ZERO_VECTOR3;
		bool			isHit = false;
		
		FOR( 0, PLAYER_MAX )
		{
			if( characterManager->GetCoinUnrivaled( value ) )	continue;
			p_pos = characterManager->GetPos( value );

			isHit = Collision::CapsuleVSSphere( p_pos,
				Vector3( p_pos.x, p_pos.y + 3.0f, p_pos.z ),
				2.0f,
				Vector3( pos.x, pos.y + ( scale * 0.5f ), pos.z ), 
				COLLISION_SCALE );

			if ( isHit )	Hitduringtheoperation( p_pos, value );
		}
	}

//-------------------------------------------------------------------------------
//	情報設定
//-------------------------------------------------------------------------------

	//	座標設定
	void	Item::SetPos( const Vector3& pos )
	{
		this->pos = pos;
	}

	//	移動量設定
	void	Item::SetMove( const Vector3& move )
	{
		this->move = move;
	}

	//	スケール設定
	void	Item::SetScale( const float& scale )
	{
		this->scale = scale;
	}

	//	向き設定
	void	Item::SetAngle( const float& angle )
	{
		this->angle = angle;
	}

	//	モデル渡し
	void	Item::SetMesh( iexMesh* obj )
	{
		this->obj = obj;
	}

	//	状態設定
	void	Item::SetState( bool state )
	{
		this->state = state;
	}

	//	タイプ設定
	void	Item::SetType( const int& type )
	{
		this->type = type;
	}

//-------------------------------------------------------------------------------
//	情報取得
//-------------------------------------------------------------------------------

	//	座標取得
	Vector3	Item::GetPos( void )const
	{
		return	pos;
	}
	
	//	向き設定
	float		Item::GetAngle( void )const
	{
		return	angle;
	}

	//	タイプ取得
	int		Item::GetType( void )const
	{
		int		out = type;
		return	out;
	}

	//	生存状態習得
	bool	Item::GetState( void )const
	{
		bool		out = state;
		return	out;
	}