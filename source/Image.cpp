
#include	"iextreme.h"
#include	"system/System.h"
#include	"GlobalFunction.h"

#include	"Image.h"

//********************************************************************************
//
//	Imageクラス
//
//********************************************************************************

//-----------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------

	//	コンストラクタ
	Image::Image( LPSTR filename ) : obj( NULL ), x( 0 ), y( 0 ), w( 0 ), h( 0 ), sx( 0 ), sy( 0 ), sw( 0 ), sh( 0 ),angle( 0.0f ), alpha( 0.0f ), renderflag( false )
	{
		obj = new iex2DObj( filename );
		alpha = 1.0f;
	}

	//	デストラクタ
	Image::~Image( void )
	{
		SafeDelete( obj );
	}

	//	初期化
	bool	Image::Initialize( int x, int y, int w, int h, int sx, int sy, int sw, int sh, float angle, float alpha, int timer )
	{
		this->x = x;
		this->y = y;
		this->sx = sx;
		this->sy = sy;
		this->angle = angle;
		this->alpha = alpha;
		this->timer = timer;
		this->plusScale_x = 0;
		this->plusScale_y = 0;
		renderflag = true;

		return	true;
	}

//-----------------------------------------------------------------------------
//	更新
//-----------------------------------------------------------------------------

	//	更新
	void	Image::Update( void )
	{
		//	波紋更新
		WaveUpdate();
	}

	//	波紋更新
	void	Image::WaveUpdate( void )
	{
		static float maxFrame = 30.0f;

		//	更新
		timer++;
		alpha -= 1.0f / maxFrame;

		plusScale_x += 140 / ( int )maxFrame;
		plusScale_y += 140 / ( int )maxFrame;

		if ( timer >= ( int )maxFrame )
		{
			alpha = 0.0f;
		}

		if ( timer >= maxFrame * 2 )
		{
			renderflag = false;
		}
	}

//-----------------------------------------------------------------------------
//	更新
//-----------------------------------------------------------------------------

	//	個々のパラメータで描画
	void	Image::Render( void )
	{
		int		width = w + plusScale_x;
		int		height = h + plusScale_y;
		int		posx = x - w / 2;
		int		posy = y - h / 2;

		if ( renderflag )
		obj->Render( posx, posy, width, height, sx, sy, sw, sh, p, angle, RS_COPY, GetColor( 1.0f, 1.0f, 1.0f, alpha ) );
	}

	//	個々のパラメータで描画（ 拡大・透明度非採用 ）
	void	Image::NormalRender( void )
	{
		int		width = w;
		int		height = h;
		int		posx = x - w / 2;
		int		posy = y - h / 2;

		obj->Render( posx, posy, width, height, sx, sy, sw, sh, p, angle, RS_COPY );
	}

	//	通常描画
	void	Image::Render( int x, int y, int w, int h, int sx, int sy, int sw, int sh, u32 dwFlags, DWORD color )
	{
		obj->Render( x, y, w, h, sx, sy, sw, sh, dwFlags, color );
	}

	//	回転、シェーダー対応
	void	Image::Render( int x, int y, int w, int h, int sx, int sy, int sw, int sh, POINT p, float angle, iexShader* shader, LPSTR technique )
	{
		obj->Render( x, y, w, h, sx, sy, sw, sh, p, angle, shader, technique );
	}

	//	回転対応
	void	Image::Render( int x, int y, int w, int h, int sx, int sy, int sw, int sh, POINT p, float ange, u32 dwFlags, DWORD color )
	{
		obj->Render( x, y, w, h, sx, sy, sw, sh, p, angle, dwFlags, color );
	}

	//	シェーダー対応
	void	Image::Render( int x, int y, int w, int h, int sx, int sy, int sw, int sh, iexShader* shader, LPSTR technique, DWORD color )
	{
		obj->Render( x, y, w, h, sx, sy, sw, sh, shader, technique, color );
	}

//-----------------------------------------------------------------------------
//	情報設定
//-----------------------------------------------------------------------------

	//	波紋発生
	void	Image::SetWave( void )
	{
		plusScale_x = plusScale_y = 0;
		alpha = 1.0f;
		timer = 0;
		renderflag = true;
	}

	//	座標設定(画像の中心を設定)
	void	Image::SetPos( int x, int y )
	{
		this->x = x;
		this->y = y;
	}
