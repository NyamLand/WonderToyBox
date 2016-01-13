
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"system/System.h"
#include	"GameManager.h"
#include	"AirPlane.h"

//**********************************************************************
//
//	AirPlaneクラス
//
//**********************************************************************

//--------------------------------------------------------------------------------
//	グローバル
//--------------------------------------------------------------------------------
#define	ROLL_POINT_ADJUST_X	200

//--------------------------------------------------------------------------------
//	初期化・解放
//--------------------------------------------------------------------------------

	//	コンストラクタ
	AirPlane::AirPlane( void ) : pos( 0.0f, 0.0f, 0.0f ), startPos( 0.0f, 0.0f, 0.0f ), endPos( 0.0f, 0.0f, 0.0f ),
		angle( 0.0f ), scale( 0.0f, 0.0f, 0.0f ), center( GetPoint( 0, 0 ) ), mode( STAY ), heightParam( 0.0f ), sideParam( 0.0f ),
		renderflag(false), type(0)
	{
		int x = static_cast<int>( iexSystem::ScreenWidth * 1.01f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 1.04f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.6f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.14f );
		scale = Vector3( w, h, 0.0f );
		ImageInitialize( obj, ( int )pos.x, ( int )pos.y, ( int )scale.x, ( int )scale.y, 0, 0, 1024, 128 );
		obj.obj = new iex2DObj( "DATA/UI/menu/menu-int.png" );
	}

	//	デストラクタ
	AirPlane::~AirPlane( void )
	{
		SafeDelete( obj.obj );
	}

//--------------------------------------------------------------------------------
//	更新・描画
//--------------------------------------------------------------------------------

	//	更新
	void	AirPlane::Update( void )
	{
		//	終了判定用
		bool	isEnd = false;

		//	各モード動作
		switch ( mode )
		{
		case	FLYING_IN:
			isEnd = FlyingIn();
			if ( isEnd )	mode = STAY;
			break;

		case	FLYING_OUT:
			isEnd = FlyingOut();
			if ( isEnd )
			{
				mode = STAY;
				renderflag = false;
			}
			break;

		case	STAY:
			Stay();
			break;
		}


	}

	//	描画
	void	AirPlane::Render( void )
	{
		//	情報更新
		obj.x = (int)pos.x;
		obj.y = (int)pos.y;
		obj.angle = angle;
		obj.p = center;
		obj.w = (int)scale.x;
		obj.h = (int)scale.y;
		obj.renderflag = renderflag;

		RenderImage( obj, obj.sx, obj.sy, obj.sw, obj.sh, IMAGE_MODE::ADOPTPARAM );
	}

//--------------------------------------------------------------------------------
//	動作関数
//--------------------------------------------------------------------------------

	//	飛び立つ
	bool	AirPlane::FlyingOut( void )
	{
		//	パラメータ加算
		param += 0.01f;
		if ( param >= 1.0f )	param = 1.0f;

		//	パラメータ調整
		heightParam = GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv5, param );

		//	上移動
		Lerp( pos.y, startPos.y, endPos.y, heightParam );

		//	パラメータ調整
		sideParam = GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv3, param );

		//	左移動
		Lerp( pos.x, startPos.x, endPos.x, sideParam );

		//	回転
		FlyRoll();

		//	移動完了判定
		if ( param >= 1.0f )	return	true;
		return	false;
	}

	//	飛び入り
	bool	AirPlane::FlyingIn( void )
	{
		//	パラメータ加算
		param += 0.01f;
		if ( param >= 1.0f )	param = 1.0f;

		//	パラメータ調整
		heightParam = GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv5, param );

		//	上移動
		Lerp( pos.y, startPos.y, endPos.y, heightParam );

		//	パラメータ調整
		sideParam = GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, param );

		//	左移動
		Lerp( pos.x, startPos.x, endPos.x, sideParam );

		//	回転
		FlyRoll();

		//	移動完了判定
		if ( param >= 1.0f )	return	true;
		return	false;
	}

	//	滞空
	void	AirPlane::Stay( void )
	{
		//	描画されてない間はスキップ
		if ( !renderflag )	return;

		//	ふわふわさせる
		pos.y = endPos.y + static_cast<int>( 10.0f * sinf( param ) );
		center = GetPoint( pos.x - scale.x * 0.2f, pos.y );
		angle = ( D3DX_PI / 180.0f * -1.0f ) * cosf( param );
		param += D3DX_PI / 180.0f;
	}

	//	滞空時回転
	void	AirPlane::FlyRoll( void )
	{
		//	回転
		center = GetPoint( pos.x - scale.x * 0.2f, pos.y );
		angle = ( D3DX_PI / 180.0f * -1.0f ) * sinf( D3DX_PI * 0.5f * param );
	}

//--------------------------------------------------------------------------------
//	情報設定
//--------------------------------------------------------------------------------

	//	タイプ設定
	void	AirPlane::SetType( int type )
	{
		this->type = type;
		obj.sy = 128 * type;
	}

	//	つぎの動作、位置設定
	void	AirPlane::SetNext( Vector3	startPos, Vector3 nextPoint, int mode )
	{
		param = 0.0f;
		this->startPos = startPos;
		endPos = nextPoint;
		this->mode = mode;
		renderflag = true;
	}

	//	描画フラグ設定
	void	AirPlane::SetRenderFlag( bool state )
	{
		renderflag = state;
	}

	//	スケール設定
	void	AirPlane::SetScale( float w, float h )
	{
		scale.x = w;
		scale.y = h;
	}

//--------------------------------------------------------------------------------
//	情報取得
//--------------------------------------------------------------------------------

	//	座標取得
	Vector3	AirPlane::GetPos( void )const
	{
		return	pos;
	}

