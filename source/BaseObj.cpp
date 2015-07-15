
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"

#include	"BaseObj.h"

//****************************************************************************************
//
//	BaseObjクラス
//
//****************************************************************************************

//-------------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------------
	
	//	コンストラクタ
	BaseObj::BaseObj( void ) : obj( NULL ),
		pos( 0.0f, 0.0f, 0.0f ), move( 0.0f, 0.0f, 0.0f ),
		angle( 0.0f ), scale( 0.0f ), speed( 0.0f ), mode( 0 ),
		attackParam( 0 ), attackPos( 0.0f, 0.0f, 0.0f ), isGround( true)
	{
		
	}

	//	デストラクタ
	BaseObj::~BaseObj( void )
	{
		SafeDelete( obj );
	}

	//	初期化
	bool	BaseObj::Initialize( int input, int type, Vector3 pos )
	{
		this->input = ::input[input];
		obj = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );

		this->pos = pos;

		if ( obj == NULL )	return	false;
		return	true;
	}

//-------------------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------------------

	//	更新
	void	BaseObj::Update( void )
	{
		//	重力加算
		move.y += GRAVITY;

		//	ステージ当たり判定
		StageCollisionCheck();

		//	移動値加算
		pos += move;

		obj->SetPos( pos );
		obj->SetAngle( angle );
		obj->SetScale( scale );
		obj->Animation();
		obj->Update();
	}

	//	描画
	void	BaseObj::Render( void )
	{
		obj->Render();
	}

	//	シェーダー付き描画
	void	BaseObj::Render( iexShader* shader, LPSTR technique )
	{
		obj->Render( shader, technique );
	}

//-------------------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------------------

	//	ステージ当たり判定チェック
	void	BaseObj::StageCollisionCheck( void )
	{
		float work = Collision::GetHeight( pos );

		if ( pos.y <= work )
		{
			pos.y = work;
			move.y = 0;
			isGround = true;
		}
		else
		{
			isGround = false;
		}

		//	壁判定
		Collision::CheckWall( pos, move );
	}

//-------------------------------------------------------------------------------------
//	情報設定・取得
//-------------------------------------------------------------------------------------

	//	設定
	void	BaseObj::SetMotion( int motion )
	{
		if ( obj->GetMotion() != motion )
		{
			obj->SetMotion( motion );
		}
	}
	void	BaseObj::SetMode( int mode )
	{
		if ( this->mode != mode )		this->mode = mode;
	}
	void	BaseObj::SetPos( Vector3 pos ){ this->pos = pos; }
	void	BaseObj::SetPos( float x, float y, float z ){ this->pos = Vector3( x, y, z ); }
	void	BaseObj::SetAngle( float angle ){ this->angle = angle; }
	void	BaseObj::SetScale( float scale ){ this->scale = scale; }

	//	取得
	Vector3		BaseObj::GetPos( void ){ return	pos; }
	Vector3		BaseObj::GetAttackPos( void ){ return attackPos; }
	Matrix		BaseObj::GetMatrix( void ){ return obj->TransMatrix; }
	float			BaseObj::GetAngle( void ){ return angle; }
	int				BaseObj::GetAttackParam( void ){ return attackParam; }