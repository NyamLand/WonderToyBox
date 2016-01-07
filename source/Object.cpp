
#include	"iextreme.h"
#include	"system/System.h"
#include	"Random.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Sound.h"
#include	"CharacterManager.h"
#include	"Stage.h"
#include	"Object.h"

//*******************************************************************************
//
//	Objectクラス
//
//*******************************************************************************

#define	MOVE_HEIGHT			5.0f
#define	DURABLE_VALUE		3		//	耐久値
#define	UNRIVALEDTIME		30		//	無敵時間

//------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------

	//	コンストラクタ
	Object::Object( void ) : obj( nullptr ), collisionObj( nullptr ),
		pos(0.0f, 0.0f, 0.0f), angle(0.0f, 0.0f, 0.0f), scale(0.0f, 0.0f, 0.0f), tempPos(0.0f, 0.0f, 0.0f), move(0.0f, 0.0f, 0.0f),
		moveHeight(0.0f), flashParam(0.0f),
		state(false), unrivaled(false),
		moveType(MOVE_TYPE::FIX_BOX), durableValue(DURABLE_VALUE), objectType(OBJECT_TYPE::BASE), id(1), unrivaledTime(0)
	{
		
	}

	//	デストラクタ
	Object::~Object( void )
	{
		SafeDelete( obj );
		SafeDelete( collisionObj );
	}

	//	初期化
	bool	Object::Initialize( void )
	{
		move = Vector3( 0.0f, 0.0f, 0.0f );
		moveHeight = 0.0f;
		flashParam = 0.0f;
		unrivaled = false;
		durableValue = DURABLE_VALUE;
		unrivaledTime = 0;
		return	true;
	}

//------------------------------------------------------------------------------
//	更新・描画
//------------------------------------------------------------------------------

	//	更新
	void	Object::Update( void )
	{
		//	動作
		Move();

		//	移動量加算
		pos += move;

		//	情報更新
		obj->SetScale( scale );
		if ( objectType == OBJECT_TYPE::DESK_BASE ) 	obj->SetAngle( angle.y + D3DX_PI );
		else																			obj->SetAngle( angle );
		obj->SetPos( pos );
		obj->Update();
		
		//	情報更新
		collisionObj->SetScale( scale );
		collisionObj->SetAngle( angle );
		collisionObj->SetPos( pos );
		collisionObj->Update();
	}

	//	描画
	void	Object::Render( iexShader* shader, LPSTR technique )
	{
		if ( shader != nullptr && technique != nullptr )
		{
			if ( moveType == MOVE_TYPE::BREAK_OBJECT )
			{
				shader3D->SetValue( "flashParam", 0.5f + 0.5f * sinf( flashParam) );
			}
			obj->Render( shader, technique );
			shader3D->SetValue( "flashParam", 0.0f );
		}
		else
		{
			obj->Render();
		}
	}

//------------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------------

	//	動作
	void	Object::Move( void )
	{
		switch ( moveType )
		{
		case MOVE_TYPE::FIX_BOX:
			//	固定なのでなにもしない
			break;

		case MOVE_TYPE::BREAK_OBJECT:
			//	耐久値０で破壊
			if ( durableValue <= 0 )
			{
				particle->BlueFlame( pos, 3.0f );
				state = false;
			}

			//	無敵時間解除
			if ( unrivaled )
			{
				unrivaledTime++;
				
				if ( unrivaledTime >= UNRIVALEDTIME )
				{
					unrivaledTime = 0;
					unrivaled = false;
				}
			}
			
			//	重力適用
			move.y += GRAVITY;

			//	接地判定
			StageCollisionCheck();

			//	点滅
			flashParam += D3DX_PI / 180 * 1.0f;
			if ( flashParam >= 2 * D3DX_PI )	flashParam = 0.0f;

			//	攻撃当たり判定
			//	エフェクト( ヒット時エフェクト、破壊エフェクト )
			break;

		case	MOVE_TYPE::MOVE_BOX_HIEGHT:
			//	上下に移動
			MoveHeight();
			break;

		case	MOVE_TYPE::MOVE_BOX_SIDE:
			//	左右に移動
			MoveSide();
			break;
		}
	}

	//	上下移動
	void	Object::MoveHeight( void )
	{
		static	float	param = 0.0f;
		param += D3DX_PI * 0.01f;
		if ( param >= D3DX_PI * 2.0f )	param = 0.0f;

		//	一秒で一往復
		move.y = MOVE_HEIGHT / 60.0f * sinf( param );
	}

	//	左右移動
	void	Object::MoveSide( void )
	{
		static	float	param = 0.0f;
		static	float	speed = 0.6f;
		param += D3DX_PI * 0.01f;
		if ( param >= D3DX_PI * 2.0f )	param = 0.0f;

		//	一秒で一往復
		move.x = MOVE_HEIGHT / 60.0f * sinf( param );
	}

	//	ステージあたり判定
	void	Object::StageCollisionCheck( void )
	{
		float	work = 0.0f;
		float	objectWork = 0.0f;
		Vector3	tempPos;
		int	outId;

		work = stage->GetHeight( pos );
		objectWork = stage->GetHeightToObject( pos, tempPos, outId, id );
		if ( pos.y < work || pos.y < objectWork )
		{
			if ( pos.y < objectWork )
			{
				pos.y = objectWork;
				pos += tempPos;
			}
			if ( pos.y < work )			pos.y = work;
			move.y = 0.0f;
		}
	}

	//	当たり判定
	void	Object::HitCheck( void )
	{

	}

	//	耐久値減少
	void	Object::SubDurableValue( void )
	{
		durableValue--;
	}

//------------------------------------------------------------------------------
//	情報設定
//------------------------------------------------------------------------------

	//	座標設定
	void	Object::SetPos( Vector3 Pos )
	{
		pos = Pos;
	}

	//	向き設定
	void	Object::SetAngle( Vector3 Angle )
	{
		angle = Angle;
	}

	//	向き設定
	void	Object::SetAngle( float Angle )
	{
		angle = Vector3( 0.0f, Angle, 0.0f );
	}

	//	スケール設定
	void	Object::SetScale( Vector3 Scale )
	{
		scale = Scale;
	}
	
	//	スケール設定
	void	Object::SetScale( float Scale )
	{
		scale = Vector3( Scale, Scale, Scale );
	}
	
	//	モデル設定
	void	Object::SetMesh( iexMesh* org )
	{
		obj = org;
	}

	//	当たり判定用モデル設定
	void	Object::SetCollisionModel( iexMesh* org )
	{
		collisionObj = org;
	}
	
	//	動作タイプ設定
	void	Object::SetMoveType( int MoveType )
	{
		moveType = MoveType;
	}

	//	モデルタイプ設定
	void	Object::SetObjectType( int ObjectType )
	{
		objectType = ObjectType;
	}

	//	元の高さ設定
	void	Object::SetOriginHeight( float height )
	{
		originHeight = height;
	}

	//	ID設定
	void	Object::SetId( int ID )
	{
		id = ID;
	}

	//	無敵設定
	void	Object::SetUnrivaled( bool state )
	{
		unrivaled = state;
	}

	//	初期値移動量初期設定
	void	Object::InitTempPos( void )
	{
		tempPos = pos;
	}

	//	状態設定
	void	Object::SetState( bool state )
	{
		this->state = state;
	}

//------------------------------------------------------------------------------
//	情報取得
//------------------------------------------------------------------------------

	//	動作タイプ取得
	int	Object::GetMoveType( void )const
	{
		return	moveType;
	}

	//	オブジェクトタイプ取得
	int	Object::GetObjectType( void )const
	{
		return	objectType;
	}

	//	モデル取得
	iexMesh*	Object::GetMesh( void )const
	{
		return	collisionObj;
	}
	
	//	座標取得
	Vector3	Object::GetPos( void )const
	{
		return	pos;
	}

	//	前回からの移動量取得
	Vector3	Object::GetTempPos( void )const
	{
		return	tempPos * 2.0f;
	}

	//	向き取得
	float			Object::GetAngle( void )const
	{
		return	angle.y;
	}

	//	向き取得
	void			Object::GetAngle( Vector3& out )const
	{
		out = angle;
	}

	//	スケール取得
	Vector3	Object::GetScale( void )const
	{
		return	scale;
	}

	//	状態取得
	bool			Object::GetState( void )const
	{
		return	state;
	}

	//	無敵状態取得
	bool			Object::GetUnrivaled( void )const
	{
		return	unrivaled;
	}

	//	耐久値取得
	int			Object::GetDurable( void )const
	{
		return	durableValue;
	}

	//	ID取得
	int			Object::GetID( void )const
	{
		return	id;
	}

	//	移動量取得
	Vector3	Object::GetMove( void )const
	{
		static DWORD last = timeGetTime();
		DWORD elapse = timeGetTime() - last;
		float dt = elapse / 1000.0f;
		last += elapse;

		return	move;
	}
	