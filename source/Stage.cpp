
#include	"iextreme.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Particle.h"
#include	"Collision.h"
#include	"Stage.h"

//**********************************************************************
//
//	Stageクラス
//
//**********************************************************************

//----------------------------------------------------------------------------
//	グローバル
//----------------------------------------------------------------------------

#define	COLLISION_LENGTH	15.0f

namespace 
{
	namespace STAGE_TYPE
	{
		enum
		{
			DESK,
			FOREST,
			BLOCK,
		};
	}
}

//----------------------------------------------------------------------------
//	初期化・解放
//----------------------------------------------------------------------------

	//	コンストラクタ
	Stage::Stage( void ) : objectID( 0 ), dirLightVec( 0.0f, 0.0f, 0.0f ), adjustV(0.0f), stageType(0)
	{
		FOR( 0, OBJECT_TYPE::END )
		{
			org[value] = nullptr;
			collisionObj[value] = nullptr;
			forestRiver = nullptr;
		}

		//	見た目モデル読み込み
		org[OBJECT_TYPE::BASE] = new iexMesh( "DATA/Object/Box/back.imo" );
		org[OBJECT_TYPE::BLUE_BLOCK] = new iexMesh( "DATA/Object/Box/blueBox.imo" );
		org[OBJECT_TYPE::GREEN_BLOCK] = new iexMesh( "DATA/Object/Box/greenBox.imo" );
		org[OBJECT_TYPE::RED_BLOCK] = new iexMesh( "DATA/Object/Box/redBox.imo" );
		org[OBJECT_TYPE::YELLOW_BLOCK] = new iexMesh( "DATA/Object/Box/yellowBox.imo" );
		org[OBJECT_TYPE::DESK_BASE] = new iexMesh( "DATA/BG/stage-desk/stage.IMO" );
		org[OBJECT_TYPE::FOREST_BASE] = new iexMesh( "DATA/BG/Forest/model/forest_base.IMO" );
		forestRiver = new iexMesh( "DATA/BG/Forest/model/forest_river.IMO" );

		//	当たり判定用モデル読み込み
		collisionObj[OBJECT_TYPE::BASE] = new iexMesh( "DATA/Object/Box/back.imo" );
		collisionObj[OBJECT_TYPE::BLUE_BLOCK] = new iexMesh( "DATA/Object/Box/blueBox.imo" );
		collisionObj[OBJECT_TYPE::GREEN_BLOCK] = new iexMesh( "DATA/Object/Box/greenBox.imo" );
		collisionObj[OBJECT_TYPE::RED_BLOCK] = new iexMesh( "DATA/Object/Box/redBox.imo" );
		collisionObj[OBJECT_TYPE::YELLOW_BLOCK] = new iexMesh( "DATA/Object/Box/yellowBox.imo" );
		collisionObj[OBJECT_TYPE::DESK_BASE] = new iexMesh( "DATA/BG/stage-desk/Collision.IMO" );
		collisionObj[OBJECT_TYPE::FOREST_BASE] = new iexMesh( "DATA/BG/Forest/Collision/collision_forest.IMO" );

		//	リスト初期化
		objList.clear();
	}

	//	デストラクタ
	Stage::~Stage( void )
	{
		Release();
	}

	//	初期化
	bool	Stage::Initialize( void )
	{
		objectID = 0;
		stageType = gameManager->GetStageType();
		switch ( stageType )
		{
		case STAGE_TYPE::DESK:
			Append( Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 1.0f, 1.0f, 1.0f ), MOVE_TYPE::FIX_BOX, OBJECT_TYPE::DESK_BASE );
			Append( Vector3( 10.0f, 20.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), MOVE_TYPE::BREAK_OBJECT, OBJECT_TYPE::RED_BLOCK );
			Append( Vector3( -10.0f, 10.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), MOVE_TYPE::MOVE_BOX_HIEGHT, OBJECT_TYPE::RED_BLOCK );
			Collision::Initiallize( collisionObj[OBJECT_TYPE::DESK_BASE] );
			iexLight::DirLight( shader3D, 0, &dirLightVec, 1.5f, 1.5f, 1.5f );
			break;

		case STAGE_TYPE::FOREST:
			Append( Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 1.0f, 1.0f, 1.0f ), MOVE_TYPE::FIX_BOX, OBJECT_TYPE::FOREST_BASE );
			Collision::Initiallize( collisionObj[OBJECT_TYPE::FOREST_BASE] );
			iexLight::DirLight( shader3D, 0, &dirLightVec, 0.5f, 0.5f, 0.5f );
			break;

		case STAGE_TYPE::BLOCK:
			Append( Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 1.0f, 1.0f, 1.0f ), MOVE_TYPE::FIX_BOX, OBJECT_TYPE::BASE );
			Append( Vector3( 20.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), MOVE_TYPE::FIX_BOX, OBJECT_TYPE::BLUE_BLOCK );
			Append( Vector3( -10.0f, 2.5f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), MOVE_TYPE::MOVE_BOX_HIEGHT, OBJECT_TYPE::RED_BLOCK );
			Append( Vector3( -10.0f, 20.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), MOVE_TYPE::BREAK_OBJECT, OBJECT_TYPE::RED_BLOCK );
			iexLight::DirLight( shader3D, 0, &dirLightVec, 1.5f, 1.5f, 1.5f );
			break;
		}
		return	true;
	}

	//	解放
	void	Stage::Release( void )
	{
		//	見た目解放
		SafeDelete( forestRiver );

		//	オリジナル解放
		for ( int i = 0; i < OBJECT_TYPE::END; i++ )
		{
			SafeDelete( org[i] );
			SafeDelete( collisionObj[i] );
		}

		//	リスト解放
		for ( auto it = objList.begin(); it != objList.end(); )
		{
			it = objList.erase( it );
		}
	}

//----------------------------------------------------------------------------
//	更新・描画
//----------------------------------------------------------------------------

	//	更新
	void	Stage::Update( void )
	{
		bool	state = true;

		//	UVアニメーション
		if ( stageType == STAGE_TYPE::FOREST )
		{
			//　川の流れ（UVアニメ）
			adjustV -= 0.001f;
		}

		FOR_LIST( objList.begin(), objList.end() )
		{
			( *it )->Update();

			//	生存チェック
			state = ( *it )->GetState();
			if ( !state )
			{
				it = objList.erase( it );
				break;
			}
		}
	}

	//	描画
	void	Stage::Render( iexShader* shader, LPSTR technique )
	{
		//	森ステージ川
		if ( stageType == STAGE_TYPE::FOREST )
		{
			shader3D->SetValue( "adjustV", adjustV );
			forestRiver->Render( shader, "effect_add" );
		}

		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( shader != nullptr && technique != nullptr )		( *it )->Render( shader, technique );
			else																			( *it )->Render();
		}
	}

//----------------------------------------------------------------------------
//	動作関数
//----------------------------------------------------------------------------

	//	リスト追加
	void	Stage::Append( Vector3 pos, Vector3 angle, Vector3 scale, int moveType, int objType )
	{
		Object*	object = nullptr;
		object = new Object();

		//	パラメータ設定
		object->SetMoveType( moveType );
		object->SetObjectType( objType );
		object->SetMesh( org[objType]->Clone() );
		object->SetCollisionModel( collisionObj[objType]->Clone() );
		object->SetPos( pos );
		object->SetAngle( angle );
		object->SetScale( scale );
		object->SetOriginHeight( pos.y );
		//object->InitTempPos();
		object->Update();
		object->SetId( objectID );
		objectID++;

		//	リストに追加
		objList.push_back( object );
	}

	//	ステージ当たり判定（ 反射 ）（ 距離が近いものだけ当たり判定 ）
	bool	Stage::GetReflect( Vector3& outPos, Vector3& outVec, float rate )
	{
		//	変数退避
		Vector3	pos = outPos;
		Vector3	vec = outVec;

		//	距離用
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;

		//	出力
		bool			out = false;
		int	outType;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			outType = ( *it )->GetObjectType();
			if ( outType != OBJECT_TYPE::BASE && outType != OBJECT_TYPE::FOREST_BASE && outType != OBJECT_TYPE::DESK_BASE )
			{
				//	距離計算
				v = ( *it )->GetPos() - pos;
				length = v.Length();

				//	距離が近かったら当たり判定
				if ( length >= COLLISION_LENGTH )	continue;

				out = Collision::GetReflect( ( *it )->GetMesh(), pos, vec, rate );
			}
			else
			{
				out = Collision::GetRefrectFix( ( *it )->GetMesh(), pos, vec, rate );
			}
		}

		//	結果を反映
		outPos = pos;
		outVec = vec;

		return	out;
	}

	//	ステージ当たり判定（ 壁 ）
	bool	Stage::CheckWall( const Vector3& p_pos, Vector3& outVec )
	{
		//	変数退避
		Vector3	pos = p_pos;
		Vector3	vec = outVec;

		//	距離用
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;

		//	出力
		bool			out = false;
		int			objType;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			objType= ( *it )->GetObjectType();
			
			if ( objType == OBJECT_TYPE::BASE || objType == OBJECT_TYPE::DESK_BASE || objType == OBJECT_TYPE::FOREST_BASE ) 
			{
				out = Collision::CheckWall( ( *it )->GetMesh(), pos, vec );
			}
		}

		//	結果を反映
		outVec = vec;

		return	out;
	}
	
	//	ステージ当たり判定（ 地面 ）
	bool	Stage::CheckDown( const Vector3& p_pos, float& outHeight )
	{
		//	変数退避
		Vector3	pos = p_pos;
		Vector3	vec = Vector3( 0.0f, -1.0f, 0.0f );
		float			height = p_pos.y;

		//	距離用
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	出力
		bool			out = false;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->GetObjectType() != OBJECT_TYPE::BASE )
			{
				//	距離計算
				v = ( *it )->GetPos() - pos;
				length = v.Length();

				//	距離が遠かったらスキップ
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					if ( length <= minLength )
					{
						minLength = length;
						out = Collision::CheckDown( ( *it )->GetMesh(), pos, height );
					}
				}
			}
			out = Collision::CheckDown( ( *it )->GetMesh(), pos, height );
		}
		outHeight = height;
		return	out;
	}

	//	ステージ下方取得(オブジェクト)
	float	Stage::GetHeightToObject( const Vector3& pos, Vector3& outTempPos, int& outId, int id  )
	{
		//	距離用
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	出力
		Vector3	resultPos = Vector3( 0.0f, 0.0f, 0.0f );
		float			out = -1000.0f;
		int			objType;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			//	自分対自分はスルー
			if ( id != 0 && ( *it )->GetID() == id )	continue;
			objType = ( *it )->GetObjectType();

			//	ベース意外
			if ( objType != OBJECT_TYPE::BASE && objType != OBJECT_TYPE::DESK_BASE && objType != OBJECT_TYPE::FOREST_BASE )
			{
				//	距離計算
				v = ( *it )->GetPos() - pos;
				length = v.Length();

				//	距離が近かったら当たり判定
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	１番近い距離を保存
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetHeight( ( *it )->GetMesh(), pos );
						outTempPos = ( *it )->GetTempPos();
						outId = ( *it )->GetID();
					}
				}
			}
		}
		return	out;
	}

	//	ステージ前方取得(オブジェクト)
	float	Stage::GetFrontToObject( const Vector3& pos, Vector3& outHitPos, int& outId, int id )
	{
		//	距離用
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	出力
		float			out = 1000.0f;
		int			objType;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			//	自分対自分はスルー
			if ( id != 0 && (*it)->GetID() == id )	continue;
			objType = (*it)->GetObjectType();

			//	ベース意外
			if (objType != OBJECT_TYPE::BASE && objType != OBJECT_TYPE::DESK_BASE && objType != OBJECT_TYPE::FOREST_BASE)
			{
				//	距離計算
				v = ( *it )->GetPos() - pos;
				length = v.Length();

				//	距離が近かったら当たり判定
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	１番近い距離を保存
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetFront( ( *it )->GetMesh(), pos, outHitPos );
						outId = ( *it )->GetID();
					}
				}
			}
		}

		return	out;
	}

	//	ステージ後方取得(オブジェクト)
	float	Stage::GetBackToObject( const Vector3& pos, Vector3& outHitPos, int& outId, int id )
	{
		//	距離用
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	出力
		float			out = -1000.0f;
		int			objType;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			//	自分対自分はスルー
			if ( id != 0 && ( *it )->GetID() == id )	continue;
			objType = ( *it )->GetObjectType();

			//	ベース意外
			if ( objType != OBJECT_TYPE::BASE && objType != OBJECT_TYPE::DESK_BASE && objType != OBJECT_TYPE::FOREST_BASE )
			{
				//	距離計算
				v = ( *it )->GetPos() - pos;
				length = v.Length();

				//	距離が近かったら当たり判定
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	１番近い距離を保存
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetBack( ( *it )->GetMesh(), pos, outHitPos );
						outId = ( *it )->GetID();
					}
				}
			}
		}

		return	out;
	}

	//	ステージ右方取得(オブジェクト)
	float	Stage::GetRightToObject( const Vector3& pos, Vector3& outHitPos, int& outId, int id )
	{
		//	距離用
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	出力
		float			out = 1000.0f;
		int			objType;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			//	自分対自分はスルー
			if ( id != 0 && ( *it )->GetID() == id )	continue;
			objType = ( *it )->GetObjectType();

			//	ベース意外
			if ( objType != OBJECT_TYPE::BASE && objType != OBJECT_TYPE::DESK_BASE && objType != OBJECT_TYPE::FOREST_BASE )
			{
				//	距離計算
				v = ( *it )->GetPos() - pos;
				length = v.Length();

				//	距離が近かったら当たり判定
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	１番近い距離を保存
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetRight( ( *it )->GetMesh(), pos, outHitPos );
						outId = ( *it )->GetID();
					}
				}
			}
		}

		return	out;
	}
	
	//	ステージ左方取得(オブジェクト)
	float	Stage::GetLeftToObject( const Vector3& pos, Vector3& outHitPos, int& outId, int id )
	{
		//	距離用
		Vector3	v = Vector3(0.0f, 0.0f, 0.0f);
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	出力
		float			out = -1000.0f;
		int			objType;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			//	自分対自分はスルー
			if ( id != 0 && ( *it )->GetID() == id )	continue;
			objType = ( *it )->GetObjectType();

			//	ベース意外
			if ( objType != OBJECT_TYPE::BASE && objType != OBJECT_TYPE::DESK_BASE && objType != OBJECT_TYPE::FOREST_BASE )
			{
				//	距離計算
				v = ( *it )->GetPos() - pos;
				length = v.Length();

				//	距離が近かったら当たり判定
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	１番近い距離を保存
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetLeft( ( *it )->GetMesh(), pos, outHitPos );
						outId = ( *it )->GetID();
					}
				}
			}
		}

		return	out;
	}

	//	ステージ下方取得
	float	Stage::GetHeight( const Vector3& pos )
	{
		//	距離用
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	出力
		float			out = -1000.0f;
		int			objType;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			objType = ( *it )->GetObjectType();

			//	ベース
			if ( objType == OBJECT_TYPE::BASE || objType == OBJECT_TYPE::DESK_BASE || objType == OBJECT_TYPE::FOREST_BASE )
			{
				out = Collision::GetHeight( ( *it )->GetMesh(), pos );
			}
		}

		return	out;
	}

	//	ステージ前方取得
	float	Stage::GetFront( const Vector3& pos )
	{
		//	出力
		float			out = 1000.0f;
		int			objType;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			objType = ( *it )->GetObjectType();

			//	ベース意外
			if ( objType == OBJECT_TYPE::BASE || objType == OBJECT_TYPE::DESK_BASE || objType == OBJECT_TYPE::FOREST_BASE )
			{
				out = Collision::GetFront( ( *it )->GetMesh(), pos );
			}
		}

		return	out;
	}
	
	//	ステージ後方取得
	float	Stage::GetBack( const Vector3& pos )
	{
		//	出力
		float			out = -1000.0f;
		int			objType;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			objType = ( *it )->GetObjectType();

			//	ベース意外
			if ( objType == OBJECT_TYPE::BASE || objType == OBJECT_TYPE::DESK_BASE || objType == OBJECT_TYPE::FOREST_BASE )
			{
				out = Collision::GetBack( ( *it )->GetMesh(), pos );
			}
		}
		return	out;
	}

	//	ステージ右方取得
	float	Stage::GetRight( const Vector3& pos )
	{
		//	出力
		float			out = 1000.0f;
		int			objType;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			objType = ( *it )->GetObjectType();

			//	ベース意外
			if ( objType == OBJECT_TYPE::BASE || objType == OBJECT_TYPE::DESK_BASE || objType == OBJECT_TYPE::FOREST_BASE )
			{
				out = Collision::GetRight( ( *it )->GetMesh(), pos );
			}
		}

		return	out;
	}

	//	ステージ左方取得
	float	Stage::GetLeft( const Vector3& pos )
	{
		//	出力
		float			out = -1000.0f;
		int			objType;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			objType = ( *it )->GetObjectType();

			//	ベース意外
			if ( objType == OBJECT_TYPE::BASE || objType == OBJECT_TYPE::DESK_BASE || objType == OBJECT_TYPE::FOREST_BASE )
			{
				out = Collision::GetLeft( ( *it )->GetMesh(), pos );
			}
		}

		return	out;
	}

	//	耐久値減少
	void	Stage::SubDurableValue( int id )
	{
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->GetID() == id ) 
			{
				(*it)->SubDurableValue();
				break;
			}
		}
	}

//----------------------------------------------------------------------------
//	情報設定
//----------------------------------------------------------------------------

	//	無敵状態設定
	void	Stage::SetUnrivaled( int id, bool state )
	{
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->GetID() == id )
			{
				( *it )->SetUnrivaled( state );
				break;
			}
		}
	}

	//	平行光設定
	void	Stage::SetDirLightVec( Vector3 dir )
	{
		dirLightVec = dir;
	}

//----------------------------------------------------------------------------
//	情報取得
//----------------------------------------------------------------------------

	//	無敵状態取得
	bool	Stage::GetUnrivaled( int id )
	{
		bool	out = false;
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->GetID() == id )
			{
				out = ( *it )->GetUnrivaled();
				break;
			}
		}
		return	out;
	}

	//	移動値取得
	Vector3	Stage::GetTempPos( int id )
	{
		int	objId = 0;
		Vector3	tempPos = Vector3( 0.0f, 0.0f, 0.0f );
		FOR_LIST( objList.begin(), objList.end() )
		{
			objId = ( *it )->GetID();
			if ( objId == id )
			{
				tempPos = ( *it )->GetTempPos();
				break;
			}
		}

		return	tempPos;
	}

	//	移動値取得
	Vector3	Stage::GetMove( int id )
	{
		int	objId = 0;
		Vector3	out = Vector3( 0.0f, 0.0f, 0.0f );
		FOR_LIST( objList.begin(), objList.end() )
		{
			objId = (*it)->GetID();
			if ( objId == id )
			{
				out = ( *it )->GetMove();
				break;
			}
		}

		return	out;
	}

	//	実体取得
	Stage*	Stage::GetInstance( void )
	{
		static	Stage	out;
		return	&out;
	}