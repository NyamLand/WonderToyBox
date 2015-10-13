
#include	"iextreme.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"Sound.h"
#include	"Particle.h"
#include	"CharacterManager.h"
#include	"GameManager.h"

#include	"Item.h"

//******************************************************************************
//
//	Itemクラス
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	グローバル
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------

	//	コンストラクタ
	Item::Item( void )
	{

	}

	//	デストラクタ
	Item::~Item(void)
	{

	}

	//	初期化
	bool	Item::Initialize( void )
	{
		Coin::Initialize();
		type = 0;
		scale = 1.0f;
		return	true;
	}

//-------------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------------

	//	動作
	void	Item::Move( void )
	{
		//	重力加算
		move.y += GRAVITY;

		//	回転
		angle += 0.05f;
	}

	//	ヒット時動作
	void	Item::Hitduringtheoperation( const Vector3& pos, const int& Num )
	{
		state = false;

		switch ( type )
		{
		case ITEM_TYPE::ATTACK_UP:
			characterManager->SetParameterInfo( Num, PARAMETER_STATE::ATTACKUP );
			break;
		}
		float	effectScale = 0.2f;
		particle->Spark(pos, effectScale);
	
		sound->PlaySE( SE::COIN_SE );
	}

//-------------------------------------------------------------------------------
//	情報設定
//-------------------------------------------------------------------------------

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

	//	タイプ取得
	int		Item::GetType( void )
	{
		int		out = type;
		return	out;
	}

	bool	Item::GetState( void )
	{
		bool		out = state;
		return	out;
	}