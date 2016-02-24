
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"NumberUI.h"

//**********************************************************************
//
//	NumberUIクラス
//
//**********************************************************************

//--------------------------------------------------------------------------------------
//	グローバル
//--------------------------------------------------------------------------------------

#define	SECRET_SRCPOS_X		12 * 64

//--------------------------------------------------------------------------------------
//	初期化・解放
//--------------------------------------------------------------------------------------

	//	コンストラクタ
	NumberUI::NumberUI( void ) : orgNumber( nullptr ),
		secretflag( false )
	{
		numImageInfo.one.obj = nullptr;
		numImageInfo.ten.obj = nullptr;
		numImageInfo.hundred.obj = nullptr;
	}

	//	デストラクタ
	NumberUI::~NumberUI( void )
	{
		Release();
	}

	//	初期化
	bool	NumberUI::Initialize( int x, int y, int scale, int num )
	{
		//	数値保存
		number = num;

		//	画像読み込み
		Load();

		//	画像設定
		SetImage();

		//	座標初期化
		SetPos( x, y );

		//	スケール初期化
		SetScale( scale );
		
		//	色初期化（白）
		SetColor( Vector3( 0.0f, 0.0f, 0.0f ) );
		
		//	数値設定
		SetNumber( num );

		//	隠ぺい設定
		SetSecretFlag( false );

		//	構造体に情報設定
		SetNumberImageInfo( numImageInfo, numInfo, number );

		return	true;
	}

	//	解放
	void	NumberUI::Release( void )
	{
		SafeDelete( orgNumber );
	}

	//	読み込み
	void	NumberUI::Load( void )
	{
		orgNumber = new iex2DObj( "DATA/UI/number.png" );
	}

//--------------------------------------------------------------------------------------
//	更新・描画
//--------------------------------------------------------------------------------------

	//	更新
	void	NumberUI::Update( void )
	{
		//	数値構造体更新
		SetNumberImageInfo( numImageInfo, numInfo, number );

		//	数値隠蔽
		if ( secretflag )
		{
			SetNumberImageInfo( numImageInfo, numInfo, 10 );
			numImageInfo.hundred.sx = SECRET_SRCPOS_X;
			numImageInfo.ten.sx = SECRET_SRCPOS_X;
			numImageInfo.one.sx = SECRET_SRCPOS_X;
		}
	}
	
	//	描画
	void	NumberUI::Render( void )
	{
		//	百の位描画
		if ( numImageInfo.hundredRenderFlag )
		{
			RenderImage( numImageInfo.hundred, 
			numImageInfo.hundred.sx,	numImageInfo.hundred.sy, 
			numImageInfo.hundred.sw, numImageInfo.hundred.sh, 
			IMAGE_MODE::ADOPTPARAM );
		}
		
		//	十の位描画
		RenderImage( numImageInfo.ten,
			numImageInfo.ten.sx, numImageInfo.ten.sy,
			numImageInfo.ten.sw, numImageInfo.ten.sh,
			IMAGE_MODE::ADOPTPARAM );

		//	一の位描画
		RenderImage( numImageInfo.one,
			numImageInfo.one.sx, numImageInfo.one.sy,
			numImageInfo.one.sw, numImageInfo.one.sh,
			IMAGE_MODE::ADOPTPARAM );
	}

//--------------------------------------------------------------------------------------
//	動作関数
//--------------------------------------------------------------------------------------

	//	数値構造体設定( 数値構造体、数値 )
	void	NumberUI::SetNumberInfo( NumberInfo& numberInfo, int num )
	{
		numberInfo.hundred = num / 100 % 10;
		numberInfo.ten = num / 10 % 10;
		numberInfo.one = num % 10;
	}

	//	設定した数値にあわせて構造体情報を設定、１００以上かで配置も変更( 数字画像情報、コイン画像情報、コイン枚数 )
	void	NumberUI::SetNumberImageInfo( NumberImageInfo& numImageInfo, NumberInfo& numInfo, const int& num )
	{
		//	桁数確認
		if ( num >= 100 )		numImageInfo.hundredRenderFlag = true;
		else							numImageInfo.hundredRenderFlag = false;

		//	数字構造体設定
		SetNumberInfo( numInfo, num );

		//	各位画像構造体初期化
		int		x, y, w, h, sx, sy, sw, sh;
		//------------------------------------------------------------------------------------------------
		//	総数用構造体設定
		//------------------------------------------------------------------------------------------------
		if ( numImageInfo.hundredRenderFlag )
		{
			//	１０の位設定
			x = numImageInfo.pos.x;
			y = numImageInfo.pos.y;
			w = h = numImageInfo.scale;
			sx = numInfo.ten * 64;
			sy = 0;
			sw = sh = 64;
			ImageInitialize( numImageInfo.ten, x, y, w, h, sx, sy, sw, sh );

			//	１００の位設定
			x = numImageInfo.pos.x - static_cast<int>( numImageInfo.ten.w / 1.5f );
			sx = numInfo.hundred * 64;
			ImageInitialize( numImageInfo.hundred, x, y, w, h, sx, sy, sw, sh );

			//	１の位設定
			x = numImageInfo.pos.x + static_cast<int>( numImageInfo.ten.w / 1.5f );
			sx = numInfo.one * 64;
			ImageInitialize( numImageInfo.one, x, y, w, h, sx, sy, sw, sh );
		}
		else
		{
			//	１０の位設定
			w = h = numImageInfo.scale;
			x = numImageInfo.pos.x - w / 3;
			y = numImageInfo.pos.y;
			sx = numInfo.ten * 64;
			sy = 0;
			sw = sh = 64;
			ImageInitialize( numImageInfo.ten, x, y, w, h, sx, sy, sw, sh );

			//	１の位設定
			x = numImageInfo.pos.x + w / 3;
			sx = numInfo.one * 64;
			ImageInitialize( numImageInfo.one, x, y, w, h, sx, sy, sw, sh );

			//	１００の位設定( 初期化の時のみセットするため ）
			x = numImageInfo.pos.x - static_cast<int>( numImageInfo.ten.w / 1.5f );
			sx = numInfo.hundred * 64;
			ImageInitialize( numImageInfo.hundred, x, y, w, h, sx, sy, sw, sh );
		}
	}

//--------------------------------------------------------------------------------------
//	情報設定
//--------------------------------------------------------------------------------------

	//	座標設定
	void	NumberUI::SetPos( int x, int y )
	{
		numImageInfo.pos.x = x;
		numImageInfo.pos.y = y;
	}

	//	スケール設定
	void	NumberUI::SetScale( int scale )
	{
		numImageInfo.scale = scale;
	}

	//	数値色設定
	void	NumberUI::SetColor( const Vector3& color )
	{
		numImageInfo.one.color = color;
		numImageInfo.ten.color = color;
		numImageInfo.hundred.color = color;
	}

	//	隠ぺいフラグ設定( trueで隠ぺい )
	void	NumberUI::SetSecretFlag( bool state )
	{
		secretflag = state;
	}

	//	画像設定
	void	NumberUI::SetImage( void )
	{
		numImageInfo.hundred.obj = orgNumber;
		numImageInfo.ten.obj = orgNumber;
		numImageInfo.one.obj = orgNumber;
	}

	//	数値設定
	void	NumberUI::SetNumber( int num )
	{
		number = num;
	}

//--------------------------------------------------------------------------------------
//	情報取得
//--------------------------------------------------------------------------------------

	//	X座標取得
	int	NumberUI::GetPosX( void )const
	{
		return			numImageInfo.pos.x;
	}

	//	Y座標取得
	int	NumberUI::GetPosY( void )const
	{
		return	numImageInfo.pos.y;
	}
