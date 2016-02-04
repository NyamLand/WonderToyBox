
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
			TOY,
			BLOCK,
		};
	}
}

//	実体
Stage*	stage = nullptr;

//----------------------------------------------------------------------------
//	初期化・解放
//----------------------------------------------------------------------------

	//	コンストラクタ
	Stage::Stage( void ) : objectID( 0 ), dirLightVec( 0.0f, 0.0f, 0.0f ), adjustV( 0.0f ), stageType( 0 )
	{
		//	ポインタ初期化
		FOR( 0, OBJECT_TYPE::END )
		{
			org[value] = nullptr;
			collisionObj[value] = nullptr;
			forestRiver = nullptr;
		}

		//	全オブジェクト初期化
		FOR( 0, OBJ_MAX )
		{
			object[value] = nullptr;
			object[value] = new Object();
		}

		//	見た目モデル読み込み
		org[OBJECT_TYPE::BASE] = new iexMesh("DATA/Object/Box/back.imo");
		org[OBJECT_TYPE::BLUE_BLOCK] = new iexMesh("DATA/Object/Box/blueBox.imo");
		org[OBJECT_TYPE::GREEN_BLOCK] = new iexMesh("DATA/Object/Box/greenBox.imo");
		org[OBJECT_TYPE::RED_BLOCK] = new iexMesh("DATA/Object/Box/redBox.imo");
		org[OBJECT_TYPE::YELLOW_BLOCK] = new iexMesh("DATA/Object/Box/yellowBox.imo");

		org[OBJECT_TYPE::TREE_TOY] = new iexMesh("DATA/BG/stageobj/tree/tree.IMO");
		org[OBJECT_TYPE::REX_TOY] = new iexMesh("DATA/BG/stageobj/rex/Rex.IMO");
		org[OBJECT_TYPE::RABBIT_TOY] = new iexMesh("DATA/BG/stageobj/usg/usg.IMO");
		org[OBJECT_TYPE::ROBOT_TOY] = new iexMesh("DATA/BG/stageobj/Robot/Robot.IMO");

		org[OBJECT_TYPE::DESK_BASE] = new iexMesh("DATA/BG/stage-desk/stage.IMO");
		org[OBJECT_TYPE::FOREST_BASE] = new iexMesh("DATA/BG/Forest/model/forest_base.IMO");
		org[OBJECT_TYPE::TOY_BASE] = new iexMesh("DATA/BG/stage_toy/stageToy.IMO" );
		forestRiver = new iexMesh("DATA/BG/Forest/model/forest_river.IMO");

		//	当たり判定用モデル読み込み
		collisionObj[OBJECT_TYPE::BASE] = new iexMesh("DATA/Object/Box/back.imo");
		collisionObj[OBJECT_TYPE::BLUE_BLOCK] = new iexMesh("DATA/Object/Box/blueBox.imo");
		collisionObj[OBJECT_TYPE::GREEN_BLOCK] = new iexMesh("DATA/Object/Box/greenBox.imo");
		collisionObj[OBJECT_TYPE::RED_BLOCK] = new iexMesh("DATA/Object/Box/redBox.imo");
		collisionObj[OBJECT_TYPE::YELLOW_BLOCK] = new iexMesh("DATA/Object/Box/yellowBox.imo");
		collisionObj[OBJECT_TYPE::TREE_TOY] = new iexMesh("DATA/BG/stageobj/tree/treeatr.IMO");
		collisionObj[OBJECT_TYPE::REX_TOY] = new iexMesh("DATA/BG/stageobj/rex/Rexatr.IMO");
		collisionObj[OBJECT_TYPE::RABBIT_TOY] = new iexMesh("DATA/BG/stageobj/usg/usg.IMO");
		collisionObj[OBJECT_TYPE::ROBOT_TOY] = new iexMesh("DATA/BG/stageobj/Robot/Robot.IMO");
		collisionObj[OBJECT_TYPE::DESK_BASE] = new iexMesh("DATA/BG/stage-desk/Collision.IMO");
		collisionObj[OBJECT_TYPE::FOREST_BASE] = new iexMesh("DATA/BG/Forest/Collision/collision_forest.IMO");
		collisionObj[OBJECT_TYPE::TOY_BASE] = new iexMesh( "DATA/BG/stage_toy/stageToy.IMO" );

		//	変数初期化
		objectID = 0;
		stageType = gameManager->GetStageType();

		//	各ステージオブジェクトの生成
		switch ( stageType )
		{
		case STAGE_TYPE::DESK:
			Append(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), MOVE_TYPE::FIX_OBJECT , OBJECT_TYPE::DESK_BASE);
			//Append( Vector3( 10.0f, 20.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), MOVE_TYPE::BREAK_OBJECT, OBJECT_TYPE::RED_BLOCK );
			//Append( Vector3( -10.0f, 10.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), MOVE_TYPE::MOVE_BOX_HIEGHT, OBJECT_TYPE::RED_BLOCK );
			break;

		case STAGE_TYPE::TOY:
			Append(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::TOY_BASE);
			//Append(Vector3(-22.0f, 0.0f, 20.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.5f, 0.2f, 0.5f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::YELLOW_BLOCK);
			//Append(Vector3(-14.0f, 3.0f, 20.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.5f, 0.2f, 0.5f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::YELLOW_BLOCK);



			Append(Vector3(-4.0f, 10.0f, 19.5f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 0.2f, 0.5f), MOVE_TYPE::MOVE_SIDE_OBJECT, OBJECT_TYPE::BLUE_BLOCK);
			//Append(Vector3(-22.0f, 0.0f, 18.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(2.0f, 0.5f, 2.0f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::YELLOW_BLOCK);
			//Append(Vector3(-16.0f, 0.0f, 18.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.5f, 0.5f, 1.0f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::GREEN_BLOCK);
			//Append(Vector3(-22.0f, 5.0f, 20.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.5f, 0.5f, 0.5f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::BLUE_BLOCK);
			//Append(Vector3(15.0f, 13.0f, 18.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.5f, 0.5f, 0.5f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::YELLOW_BLOCK);
			//Append(Vector3(-20.0f, 0.0f, 8.0f), Vector3(0.0f, 1.5f, 0.0f), Vector3(0.09f, 0.09f, 0.09f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::REX_TOY);
			/*Append(Vector3(-10.0f, 10.0f, -10.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.5f, 0.5f, 0.5f), MOVE_TYPE::MOVE_SIDE_OBJECT, OBJECT_TYPE::RED_BLOCK);
			Append(Vector3(-5.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.01f, 0.01f, 0.01f), MOVE_TYPE::BREAK_OBJECT, OBJECT_TYPE::TREE_TOY);
			Append(Vector3(-3.0f, 0.0f, 0.0f), Vector3(0.0f, 3.0f, 0.0f), Vector3(0.01f, 0.01f, 0.01f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::RABBIT_TOY);
			Append(Vector3(5.0f, 0.0f, 5.0f), Vector3(0.0f, 3.0f, 0.0f), Vector3(0.01f, 0.01f, 0.01f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::ROBOT_TOY);
			Append(Vector3(-10.0f, 0.0f, -10.0f), Vector3(0.0f, 1.5f, 0.0f), Vector3(0.09f, 0.09f, 0.09f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::REX_TOY);*/
			break;
		}
	}

	//	デストラクタ
	Stage::~Stage( void )
	{
		Release();
	}

	//	平行光初期化
	bool	Stage::LightInitialize( Vector3	dir )
	{
		//	平行光設定
		dirLightVec = dir;

		//	ライト色
		Vector3	lightColor;
		
		switch ( stageType )
		{
		case	STAGE_TYPE::DESK:
			lightColor = Vector3( 1.5f, 1.5f, 1.5f );
			break;
			
		case	STAGE_TYPE::TOY:
			lightColor = Vector3(1.5f, 1.5f, 1.5f);
			break;

		case	STAGE_TYPE::BLOCK:
			lightColor = Vector3( 1.5f, 1.5f, 1.5f );
			break;
		}
		//	シェーダーにセット
		iexLight::DirLight( shader3D, 0, &dirLightVec, lightColor.x, lightColor.y, lightColor.z );
		shader3D->SetValue( "DirLightVec", dirLightVec );

		return	true;
	}

	//	解放
	void	Stage::Release( void )
	{
		//	見た目解放
		SafeDelete( forestRiver );

		//	オリジナル解放
		FOR( 0, OBJECT_TYPE::END )
		{
			SafeDelete( org[value] );
			SafeDelete( collisionObj[value] );
		}
		
		//	全オブジェクト解放
		FOR( 0, OBJ_MAX )
		{
			SafeDelete( object[value] );
		}
	}

//----------------------------------------------------------------------------
//	更新・描画
//----------------------------------------------------------------------------

	//	更新
	void	Stage::Update( void )
	{
		bool	state = true;


		FOR( 0, OBJ_MAX )
		{
			//	生存チェック( 存在していたら更新 )
			state = object[value]->GetState();
			if ( state )		object[value]->Update();
		}
	}

	//	情報のみ更新
	void	Stage::ObjUpdate(void)
	{
		bool	state = true;

		FOR(0, OBJ_MAX)
		{
			//	生存チェック( 存在していたら更新 )
			state = object[value]->GetState();
			if (state)		object[value]->ObjUpdate();
		}
	}
	//	描画
	void	Stage::Render( iexShader* shader, LPSTR technique )
	{

		FOR( 0, OBJ_MAX )
		{
			if ( !object[value]->GetState() )	continue;
			if ( shader != nullptr && technique != nullptr )		object[value]->Render( shader, technique );
			else																			object[value]->Render();
		}
	}

//----------------------------------------------------------------------------
//	動作関数
//----------------------------------------------------------------------------

	//	リスト追加
	void	Stage::Append( Vector3 pos, Vector3 angle, Vector3 scale, int moveType, int objType )
	{
		FOR( 0, OBJ_MAX )
		{
			//	空きチェック
			if ( object[value]->GetState() )	continue;

			//	初期化
			object[value]->Initialize();
			
			//	パラメータ設定
			object[value]->SetState( true );
			object[value]->SetMoveType( moveType );
			object[value]->SetObjectType( objType );
			object[value]->SetMesh( org[objType]->Clone() );
			object[value]->SetCollisionModel( collisionObj[objType]->Clone() );
			object[value]->SetPos( pos );
			object[value]->SetAngle( angle );
			object[value]->SetScale( scale );
			object[value]->SetOriginHeight( pos.y );
			object[value]->SetId( objectID );
			objectID++;
			break;
		}
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
		FOR( 0, OBJ_MAX )
		{
			//	生存チェック
			if ( !object[value]->GetState() )	continue;
			
			//	オブジェクトのタイプ取得
			outType = object[value]->GetObjectType();

			//	ベース以外との当たり判定
			if (outType != OBJECT_TYPE::BASE 
				&& outType != OBJECT_TYPE::TOY_BASE 
				&& outType != OBJECT_TYPE::FOREST_BASE 
				&& outType != OBJECT_TYPE::DESK_BASE)
			{
				//	距離計算
				v = object[value]->GetPos() - pos;
				length = v.Length();

				//	距離が近かったら当たり判定
				if ( length >= COLLISION_LENGTH )	continue;

				out = Collision::GetReflect( object[value]->GetMesh(), pos, vec, rate );
			}
			else
			{
				out = Collision::GetRefrectFix( object[value]->GetMesh(), pos, vec, rate );
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
		FOR( 0, OBJ_MAX )
		{
			//	生存チェック
			if ( !object[value]->GetState() )	continue;

			//	オブジェクトのタイプを取得
			objType= object[value]->GetObjectType();
			
			//	ベースとの当たり判定
			if (objType == OBJECT_TYPE::BASE 
				|| objType == OBJECT_TYPE::DESK_BASE 
				|| objType == OBJECT_TYPE::FOREST_BASE 
				|| objType == OBJECT_TYPE::TOY_BASE)
			{
				out = Collision::CheckWall( object[value]->GetMesh(), pos, vec );
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
		FOR( 0, OBJ_MAX )
		{
			//	生存チェック
			if ( !object[value]->GetState() )	continue;

			//	ベース以外
			if ( object[value]->GetObjectType() != OBJECT_TYPE::BASE )
			{
				//	距離計算
				v = object[value]->GetPos() - pos;
				length = v.Length();

				//	距離が遠かったらスキップ
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					if ( length <= minLength )
					{
						minLength = length;
						out = Collision::CheckDown( object[value]->GetMesh(), pos, height );
					}
				}
			}
			out = Collision::CheckDown( object[value]->GetMesh(), pos, height );
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
		FOR( 0, OBJ_MAX )
		{
			//	生存チェック
			if ( !object[value]->GetState() )	continue;
			
			//	自分対自分はスルー
			if ( id != 0 && object[value]->GetID() == id )	continue;
			objType = object[value]->GetObjectType();

			//	ベース意外
			if ( objType != OBJECT_TYPE::BASE 
				&& objType != OBJECT_TYPE::DESK_BASE 
				&& objType != OBJECT_TYPE::FOREST_BASE
				&& objType != OBJECT_TYPE::TOY_BASE)
			{
				//	距離計算
				v = object[value]->GetPos() - pos;
				length = v.Length();

				//	距離が近かったら当たり判定
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	１番近い距離を保存
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetHeight( object[value]->GetMesh(), pos );
						outTempPos = object[value]->GetTempPos();
						outId = object[value]->GetID();
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
		FOR( 0, OBJ_MAX )
		{
			//	生存チェック
			if ( !object[value]->GetState() )	continue;

			//	自分対自分はスルー
			if ( id != 0 && object[value]->GetID() == id )	continue;
			objType = object[value]->GetObjectType();

			//	ベース意外
			if ( objType != OBJECT_TYPE::BASE 
				&& objType != OBJECT_TYPE::DESK_BASE 
				&& objType != OBJECT_TYPE::FOREST_BASE
				&& objType != OBJECT_TYPE::TOY_BASE)
			{
				//	距離計算
				v = object[value]->GetPos() - pos;
				length = v.Length();

				//	距離が近かったら当たり判定
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	１番近い距離を保存
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetFront( object[value]->GetMesh(), pos, outHitPos );
						outId = object[value]->GetID();
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
		FOR( 0, OBJ_MAX )
		{
			//	生存チェック
			if ( !object[value]->GetState() )	continue;

			//	自分対自分はスルー
			if ( id != 0 && object[value]->GetID() == id )	continue;
			objType = object[value]->GetObjectType();

			//	ベース意外
			if ( objType != OBJECT_TYPE::BASE 
				&& objType != OBJECT_TYPE::DESK_BASE 
				&& objType != OBJECT_TYPE::FOREST_BASE
				&& objType != OBJECT_TYPE::TOY_BASE)
			{
				//	距離計算
				v = object[value]->GetPos() - pos;
				length = v.Length();

				//	距離が近かったら当たり判定
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	１番近い距離を保存
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetBack( object[value]->GetMesh(), pos, outHitPos );
						outId = object[value]->GetID();
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
		FOR( 0, OBJ_MAX )
		{
			//	生存チェック
			if ( !object[value]->GetState() )	continue;

			//	自分対自分はスルー
			if ( id != 0 && object[value]->GetID() == id )	continue;
			objType = object[value]->GetObjectType();

			//	ベース意外
			if ( objType != OBJECT_TYPE::BASE 
				&& objType != OBJECT_TYPE::DESK_BASE 
				&& objType != OBJECT_TYPE::FOREST_BASE
				&& objType != OBJECT_TYPE::TOY_BASE)
			{
				//	距離計算
				v = object[value]->GetPos() - pos;
				length = v.Length();

				//	距離が近かったら当たり判定
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	１番近い距離を保存
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetRight( object[value]->GetMesh(), pos, outHitPos );
						outId = object[value]->GetID();
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
		FOR( 0, OBJ_MAX )
		{
			//	生存チェック
			if ( !object[value]->GetState() )	continue;

			//	自分対自分はスルー
			if ( id != 0 && object[value]->GetID() == id )	continue;
			objType = object[value]->GetObjectType();

			//	ベース意外
			if ( objType != OBJECT_TYPE::BASE 
				&& objType != OBJECT_TYPE::DESK_BASE 
				&& objType != OBJECT_TYPE::FOREST_BASE
				&& objType != OBJECT_TYPE::TOY_BASE)
			{
				//	距離計算
				v = object[value]->GetPos() - pos;
				length = v.Length();

				//	距離が近かったら当たり判定
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	１番近い距離を保存
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetLeft( object[value]->GetMesh(), pos, outHitPos );
						outId = object[value]->GetID();
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
		FOR( 0, OBJ_MAX )
		{
			//	生存チェック
			if ( !object[value]->GetState() )	continue;

			//	オブジェクトのタイプを取得
			objType = object[value]->GetObjectType();

			//	ベースのみ当たり判定
			if ( objType == OBJECT_TYPE::BASE 
				|| objType == OBJECT_TYPE::DESK_BASE 
				|| objType == OBJECT_TYPE::FOREST_BASE
				|| objType == OBJECT_TYPE::TOY_BASE)
			{
				out = Collision::GetHeight( object[value]->GetMesh(), pos );
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
		FOR( 0, OBJ_MAX )
		{
			//	生存チェック
			if ( !object[value]->GetState() )	continue;

			//	オブジェクトのタイプを取得
			objType = object[value]->GetObjectType();

			//	ベースのみ当たり判定
			if ( objType == OBJECT_TYPE::BASE 
				|| objType == OBJECT_TYPE::DESK_BASE 
				|| objType == OBJECT_TYPE::FOREST_BASE
				|| objType == OBJECT_TYPE::TOY_BASE)
			{
				out = Collision::GetFront( object[value]->GetMesh(), pos );
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
		FOR( 0, OBJ_MAX )
		{
			//	生存チェック
			if ( !object[value]->GetState() )	continue;

			//	オブジェクトのタイプを取得
			objType = object[value]->GetObjectType();

			//	ベースのみ当たり判定
			if ( objType == OBJECT_TYPE::BASE 
				|| objType == OBJECT_TYPE::DESK_BASE 
				|| objType == OBJECT_TYPE::FOREST_BASE
				|| objType == OBJECT_TYPE::TOY_BASE)
			{
				out = Collision::GetBack( object[value]->GetMesh(), pos );
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
		FOR( 0, OBJ_MAX )
		{
			//	生存チェック
			if ( !object[value]->GetState() )	continue;

			//	オブジェクトのタイプを取得
			objType = object[value]->GetObjectType();

			//	ベースのみ当たり判定
			if ( objType == OBJECT_TYPE::BASE 
				|| objType == OBJECT_TYPE::DESK_BASE 
				|| objType == OBJECT_TYPE::FOREST_BASE
				|| objType == OBJECT_TYPE::TOY_BASE)
			{
				out = Collision::GetRight( object[value]->GetMesh(), pos );
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
		FOR( 0, OBJ_MAX )
		{
			//	生存チェック
			if ( !object[value]->GetState() )	continue;

			//	オブジェクトのタイプを取得
			objType = object[value]->GetObjectType();

			//	ベースのみ当たり判定
			if ( objType == OBJECT_TYPE::BASE 
				|| objType == OBJECT_TYPE::DESK_BASE 
				|| objType == OBJECT_TYPE::FOREST_BASE
				|| objType == OBJECT_TYPE::TOY_BASE)
			{
				out = Collision::GetLeft( object[value]->GetMesh(), pos );
			}
		}

		return	out;
	}

	//	耐久値減少
	void	Stage::SubDurableValue( int id )
	{
		FOR( 0, OBJ_MAX )
		{
			//	生存チェック
			if ( !object[value]->GetState() )	continue;
			
			//	同じIDのオブジェクトの耐久値減少
			if ( object[value]->GetID() == id ) 
			{
				object[value]->SubDurableValue();
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
		FOR( 0, OBJ_MAX )
		{
			if ( !object[value]->GetState() )	continue;
			if ( object[value]->GetID() == id )
			{
				object[value]->SetUnrivaled( state );
				break;
			}
		}
	}

//----------------------------------------------------------------------------
//	情報取得
//----------------------------------------------------------------------------

	//	無敵状態取得
	bool	Stage::GetUnrivaled( int id )
	{
		bool	out = false;
		FOR( 0, OBJ_MAX )
		{
			if ( object[value]->GetID() == id )
			{
				out = object[value]->GetUnrivaled();
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
		FOR( 0, OBJ_MAX )
		{
			objId = object[value]->GetID();
			if ( objId == id )
			{
				tempPos = object[value]->GetTempPos();
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
		FOR( 0, OBJ_MAX )
		{
			objId = object[value]->GetID();
			if ( objId == id )
			{
				out = object[value]->GetMove();
				break;
			}
		}

		return	out;
	}

	bool Stage::GetLocal( Vector3 &localPos, Vector3 &localAngle)
	{
		//	出力
		bool			out =false;
		int			moveType;
		Vector3		outLocalPos = localPos;
		Vector3		outLocalAngle = localAngle;

		//	全当たり判定
		FOR(0, OBJ_MAX)
		{
			//	生存チェック
			if (!object[value]->GetState())	continue;

			//	オブジェクトのタイプを取得
			moveType = object[value]->GetMoveType();

			//	ベースのみ当たり判定
			if (moveType == MOVE_TYPE::MOVE_SIDE_OBJECT
				|| moveType == MOVE_TYPE::MOVE_HIEGHT_OBJECT)
			{
				out = object[value]->GetLocal(outLocalPos, outLocalAngle);
				localPos = outLocalPos;
				localAngle = outLocalAngle;
			}
		}

		return	out;
	}

	void Stage::GetWorld( Vector3 &worldPos, Vector3 &worldAngle)
	{
		//	出力
		int			moveType;
		Vector3		outLocalPos =worldPos;
		Vector3		outLocalAngle = worldAngle;

		//	全当たり判定
		FOR(0, OBJ_MAX)
		{
			//	生存チェック
			if (!object[value]->GetState())	continue;

			//	オブジェクトのタイプを取得
			moveType = object[value]->GetMoveType();

			//	ベースのみ当たり判定
			if (moveType == MOVE_TYPE::MOVE_SIDE_OBJECT
				|| moveType == MOVE_TYPE::MOVE_HIEGHT_OBJECT)
			{
				object[value]->GetWorld(outLocalPos, outLocalAngle);
				worldPos = outLocalPos;
				worldAngle = outLocalAngle;
			}
		}
	}