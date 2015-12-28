
#include	"iextreme.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
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
	namespace OBJECT_TYPE
	{
		enum
		{
			BASE,
			BLUE_BLOCK,
			GREEN_BLOCK,
			RED_BLOCK,
			YELLOW_BLOCK,
		};
	}
}

//----------------------------------------------------------------------------
//	初期化・解放
//----------------------------------------------------------------------------

	//	コンストラクタ
	Stage::Stage( void )
	{
		//	モデル読み込み
		org[0] = new iexMesh( "DATA/Object/Box/back.imo" );
		org[1] = new iexMesh( "DATA/Object/Box/blueBox.imo" );
		org[2] = new iexMesh( "DATA/Object/Box/greenBox.imo" );
		org[3] = new iexMesh( "DATA/Object/Box/redBox.imo" );

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
		Append( Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 1.0f, 1.0f, 1.0f ), 0 );

		Append( Vector3( 20.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 1 );
		Append( Vector3( 0.0f, 20.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 2 );
		Append( Vector3( 0.0f, 0.0f, 20.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 3 );
		Append( Vector3( 10.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 1 );
		Append( Vector3( 0.0f, 10.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 2 );
		Append( Vector3( 0.0f, 0.0f, 10.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 3 );
		Append( Vector3( 5.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 1 );
		Append( Vector3( 0.0f, 5.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 2 );
		Append( Vector3( 0.0f, 0.0f, 5.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 3 );
		Append( Vector3( 0.0f, 10.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 2 );
		Append( Vector3( 0.0f, 0.0f, 10.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 3 );
		Append( Vector3( 5.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 1 );
		Append( Vector3( 0.0f, 5.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 2 );
		Append( Vector3( 0.0f, 0.0f, 5.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 3 );
		return	true;
	}

	//	解放
	void	Stage::Release( void )
	{
		//	オリジナル解放
		for ( int i = 0; i < 4; i++ )
		{
			SafeDelete( org[i] );
		}

		//	リスト解放
		for ( auto it = objList.begin(); it != objList.end(); )
		{
			it = objList.erase(it);
		}
	}

//----------------------------------------------------------------------------
//	更新・描画
//----------------------------------------------------------------------------

	//	更新
	void	Stage::Update( void )
	{
		FOR_LIST( objList.begin(), objList.end() )
		{
			( *it )->obj->Update();
		}
	}

	//	描画
	void	Stage::Render( void )
	{
		FOR_LIST( objList.begin(), objList.end() )
		{
			( *it )->obj->Render( shader3D, "full" );
		}
	}

//----------------------------------------------------------------------------
//	動作関数
//----------------------------------------------------------------------------

	//	リスト追加
	void	Stage::Append( Vector3 pos, Vector3 angle, Vector3 scale, int type )
	{
		meshInfo*	mesh;
		mesh = new meshInfo();
		mesh->obj = nullptr;

		//	パラメータ設定
		mesh->type = type;
		mesh->obj = org[type]->Clone();
		mesh->obj->SetPos( pos );
		mesh->obj->SetAngle( angle );
		mesh->obj->SetScale( scale );
		mesh->obj->Update();

		//	リストに追加
		objList.push_back( mesh );
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

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type != OBJECT_TYPE::BASE )
			{
				//	距離計算
				v = ( *it )->obj->GetPos() - pos;
				length = v.Length();

				//	距離が近かったら当たり判定
				if ( length >= COLLISION_LENGTH )	continue;
			}
			
			out = Collision::GetReflect( ( *it )->obj, pos, vec, rate );
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

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			//	ベース以外だったら
			if ( ( *it )->type != OBJECT_TYPE::BASE )
			{
				//	距離計算
				v = ( *it )->obj->GetPos() - pos;
				length = v.Length();

				//	距離が遠かったらスキップ
				if ( length >= COLLISION_LENGTH )	continue;
			}
			out = Collision::CheckWall( ( *it )->obj, pos, vec );
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
			if ( ( *it )->type != OBJECT_TYPE::BASE )
			{
				//	距離計算
				v = ( *it )->obj->GetPos() - pos;
				length = v.Length();

				//	距離が遠かったらスキップ
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					if ( length <= minLength )
					{
						minLength = length;
						out = Collision::CheckDown( ( *it )->obj, pos, height );
					}
				}
			}
			out = Collision::CheckDown( ( *it )->obj, pos, height );
		}
		outHeight = height;
		return	out;
	}

	//	ステージ下方取得(オブジェクト)
	float	Stage::GetHeightToObject( const Vector3& pos )
	{
		//	距離用
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	出力
		float			out = -1000.0f;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type != OBJECT_TYPE::BASE )
			{
				//	距離計算
				v = ( *it )->obj->GetPos() - pos;
				length = v.Length();

				//	距離が近かったら当たり判定
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	１番近い距離を保存
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetHeight( ( *it )->obj, pos );
					}
				}
			}
		}

		return	out;
	}

	//	ステージ前方取得(オブジェクト)
	float	Stage::GetFrontToObject( const Vector3& pos )
	{
		//	距離用
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	出力
		float			out = 1000.0f;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type != OBJECT_TYPE::BASE )
			{
				//	距離計算
				v = (*it)->obj->GetPos() - pos;
				length = v.Length();

				//	距離が近かったら当たり判定
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	１番近い距離を保存
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetFront( ( *it )->obj, pos );
					}
				}
			}
		}

		return	out;
	}

	//	ステージ後方取得(オブジェクト)
	float	Stage::GetBackToObject( const Vector3& pos )
	{
		//	距離用
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	出力
		float			out = -1000.0f;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type != OBJECT_TYPE::BASE )
			{
				//	距離計算
				v = (*it)->obj->GetPos() - pos;
				length = v.Length();

				//	距離が近かったら当たり判定
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	１番近い距離を保存
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetBack( ( *it )->obj, pos );
					}
				}
			}
		}

		return	out;
	}

	//	ステージ右方取得(オブジェクト)
	float	Stage::GetRightToObject( const Vector3& pos )
	{
		//	距離用
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	出力
		float			out = 1000.0f;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type != OBJECT_TYPE::BASE )
			{
				//	距離計算
				v = ( *it )->obj->GetPos() - pos;
				length = v.Length();

				//	距離が近かったら当たり判定
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	１番近い距離を保存
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetRight( ( *it )->obj, pos );
					}
				}
			}
		}

		return	out;
	}
	
	//	ステージ左方取得(オブジェクト)
	float	Stage::GetLeftToObject( const Vector3& pos )
	{
		//	距離用
		Vector3	v = Vector3(0.0f, 0.0f, 0.0f);
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	出力
		float			out = -1000.0f;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type != OBJECT_TYPE::BASE )
			{
				//	距離計算
				v = ( *it )->obj->GetPos() - pos;
				length = v.Length();

				//	距離が近かったら当たり判定
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	１番近い距離を保存
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetLeft( ( *it )->obj, pos );
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

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type == OBJECT_TYPE::BASE )
			{
				out = Collision::GetHeight( ( *it )->obj, pos );
			}
		}

		return	out;
	}

	//	ステージ前方取得
	float	Stage::GetFront( const Vector3& pos )
	{
		//	出力
		float			out = 1000.0f;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type == OBJECT_TYPE::BASE )
			{
				out = Collision::GetFront( ( *it )->obj, pos );
			}
		}

		return	out;
	}
	
	//	ステージ後方取得
	float	Stage::GetBack( const Vector3& pos )
	{
		//	出力
		float			out = -1000.0f;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type == OBJECT_TYPE::BASE )
			{
				out = Collision::GetBack( ( *it )->obj, pos );
			}
		}
		return	out;
	}

	//	ステージ右方取得
	float	Stage::GetRight( const Vector3& pos )
	{
		//	出力
		float			out = 1000.0f;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type == OBJECT_TYPE::BASE )
			{
				out = Collision::GetRight( ( *it )->obj, pos );
			}
		}

		return	out;
	}

	//	ステージ左方取得
	float	Stage::GetLeft( const Vector3& pos )
	{
		//	出力
		float			out = -1000.0f;

		//	全当たり判定
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type == OBJECT_TYPE::BASE )
			{
				out = Collision::GetLeft( ( *it )->obj, pos );
			}
		}

		return	out;
	}

//----------------------------------------------------------------------------
//	情報設定
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//	情報取得
//----------------------------------------------------------------------------

	//	実体取得
	Stage*	Stage::GetInstance( void )
	{
		static	Stage	out;
		return	&out;
	}