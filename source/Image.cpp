
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
	bool	Image::Initialize( float x, float y, float w, float h, int sx, int sy, int sw, int sh, float angle, float alpha, int timer )
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		this->sx = sx;
		this->sy = sy;
		this->sw = sw;
		this->sh = sh;
		this->angle = angle;
		this->alpha = alpha;
		this->timer = timer;
		this->plusScale_x = 0.0f;
		this->plusScale_y = 0.0f;
		renderflag = false;

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

		plusScale_x += 140.0f / maxFrame;
		plusScale_y += 140.0f / maxFrame;

		if ( timer >= maxFrame )
		{
			alpha = 0.0f;
			renderflag = false;
		}
	}

//-----------------------------------------------------------------------------
//	更新
//-----------------------------------------------------------------------------

	//	個々のパラメータで描画
	void	Image::Render( void )
	{
		float		width = w + plusScale_x;
		float		height = h + plusScale_y;
		float		posx = x - width / 2.0f;
		float		posy = y - height / 2.0f;

		if ( renderflag )
		obj->Render( ( int )posx, ( int )posy, ( int )width, ( int )height, sx, sy, sw, sh, p, angle, RS_COPY, GetColor( 1.0f, 1.0f, 1.0f, alpha ) );
	}

	//	個々のパラメータで描画（ 拡大・透明度非採用 ）
	void	Image::NormalRender( void )
	{
		float		width = w;
		float		height = h;
		float		posx = x - w / 2;
		float		posy = y - h / 2;

		obj->Render( ( int )posx, ( int )posy, ( int )width, ( int )height, sx, sy, sw, sh, p, angle, RS_COPY );
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
	void	Image::Render( int x, int y, int w, int h, int sx, int sy, int sw, int sh, POINT p, float angle, u32 dwFlags, DWORD color )
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
	void	Image::SetPos( float x, float y )
	{
		this->x = x;
		this->y = y;
	}

	//	サイズ設定
	void	Image::SetSize( float w, float h )
	{
		this->w = w;
		this->h = h;
	}

	//	読み込み位置設定
	void	Image::SetSearchPos( int sx, int sy )
	{
		this->sx = sx;
		this->sy = sy;
	}

	//	読み込み範囲設定
	void	Image::SetSearchSize( int sw, int sh )
	{
		this->sw = sw;
		this->sh = sh;
	}