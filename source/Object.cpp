
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

//------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------

	//	コンストラクタ
	Object::Object( void ) : obj( nullptr ),
		pos(0.0f, 0.0f, 0.0f), angle(0.0f, 0.0f, 0.0f), scale(0.0f,0.0f,0.0f),
		state(true), 
		type(0), durableValue(0)
	{
		
	}

	//	デストラクタ
	Object::~Object( void )
	{
		SafeDelete( obj );
	}

//------------------------------------------------------------------------------
//	更新・描画
//------------------------------------------------------------------------------

	//	更新
	void	Object::Update( void )
	{
		obj->SetScale( scale );
		obj->SetAngle( angle );
		obj->SetPos( pos );
		obj->Update();
	}

	//	描画
	void	Object::Render( iexShader* shader, LPSTR technique )
	{
		if ( shader != nullptr && technique != nullptr )	
			obj->Render( shader, technique );
		else
			obj->Render();
	}

//------------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------------

	//	動作
	void	Object::Move( void )
	{

	}

	//	当たり判定
	void	Object::HitCheck( void )
	{

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
	
	//	タイプ設定
	void	Object::SetType( int objType )
	{
		type = objType;
	}

//------------------------------------------------------------------------------
//	情報取得
//------------------------------------------------------------------------------

	//	座標取得
	Vector3	Object::GetPos( void )const
	{
		return	pos;
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

	//	オブジェクトタイプ取得
	int			Object::GetType( void )const
	{
		return	type;
	}

	//	耐久値取得
	int			Object::GetDurable( void )const
	{
		return	durableValue;
	}
	