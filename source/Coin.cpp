
#include	"iextreme.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"

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
		obj = new iexMesh( "DATA/coin.imo" );
		angle = 0.0f;
		pos = Vector3( 0.0f, 0.0f, 0.0f );
		move = Vector3( 0.0f, 0.0f, 0.0f );
		scale = 0.5f;
		activate = true;

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

		pos += move;
		StageCollisionCheck();

		obj->SetAngle( angle );
		obj->SetPos( pos );
		obj->SetScale( scale );
		obj->Update();
	}

	//	描画
	void	Coin::Render( void )
	{
		obj->Render();
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
	