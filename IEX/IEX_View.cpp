#include	"iExtreme.h"

//*****************************************************************************
//
//		視界関連
//
//*****************************************************************************

//*****************************************************************************
//		視界クリア
//*****************************************************************************
//------------------------------------------------------
//	カラーバッファ＋Ｚバッファクリア
//------------------------------------------------------
void iexView::Clear( DWORD color, bool bClearZ )
{
	DWORD	flag = D3DCLEAR_TARGET;

	if( bClearZ ) flag |= D3DCLEAR_ZBUFFER;
	iexSystem::GetDevice()->Clear( 0, NULL, flag, color, 1.0f, 0 );
}

//------------------------------------------------------
//	Ｚバッファクリア
//------------------------------------------------------
void iexView::ClearZ()
{
	iexSystem::GetDevice()->Clear( 0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );
}


//*****************************************************************************
//		視点設定
//*****************************************************************************
//------------------------------------------------------
//	視点・注視点設定
//------------------------------------------------------
void iexView::Set( Vector& pos, Vector& target )
{
	Pos = pos;
	Target = target;
}

//------------------------------------------------------
//	位置・角度設定
//------------------------------------------------------
void iexView::Set( float x, float y, float z, float ax, float ay, float az )
{
	float sx = sinf(ax);
	float sy = sinf(ay);
	float cx = cosf(ax);
	float cy = cosf(ay);
	
	Vector	p, t;
	p.x = x;
	p.y = y;
	p.z = z;
	t.x = x + (cx*sy);
	t.y = y + (-sx);
	t.z = z + (cx*cy);

	Set( p, t );
}

//*****************************************************************************
//		投影平面設定
//*****************************************************************************
void iexView::SetViewport()
{
	Viewport.X = 0;
	Viewport.Y = 0;
	Viewport.Width  = iexSystem::ScreenWidth;
	Viewport.Height = iexSystem::ScreenHeight;
	Viewport.MaxZ = 1.0f;
	Viewport.MinZ = 0.0f;
	Aspect = (float)Viewport.Width / (float)Viewport.Height;
}

void iexView::SetViewport( int x, int y, int w, int h )
{
	Viewport.X = x;
	Viewport.Y = y;
	Viewport.Width = w;
	Viewport.Height = h;
	Viewport.MaxZ = 1.0f;
	Viewport.MinZ = 0.0f;

	Aspect = (float)w / (float)h;
}

//*****************************************************************************
//		投影設定
//*****************************************************************************
void iexView::SetProjection( float FovY, float Near, float Far )
{
	if( Viewport.Width == 0 )
	{
		Viewport.Width  = iexSystem::ScreenWidth;
		Viewport.Height = iexSystem::ScreenHeight;
	}
	SetProjection( FovY, Near, Far, (float)Viewport.Width / (float)Viewport.Height );
}

void iexView::SetProjection( float FovY, float Near, float Far, float asp )
{
	this->FovY    = FovY;
	this->Near    = Near;
	this->Far     = Far;
	this->Aspect  = asp;
}

//*****************************************************************************
//		適用
//*****************************************************************************
void iexView::Activate()
{
	//	ビュー行列設定
	Vector p, t;
	p = Pos;
	t = Target;
	LookAtLH( matView, p, t, Vector3( .0f, 1.0f, .0f ) );

	//	ビューポート設定
	if( Viewport.Width != 0 ) iexSystem::GetDevice()->SetViewport( &Viewport );

	//	投影行列設定
	PerspectiveLH( matProj, FovY, Aspect, Near, Far );

	//	グローバルへの適用
	::matProjection = matProj;
	::matView       = matView;

	//	DirectX設定
	iexSystem::Device->SetTransform( D3DTS_PROJECTION, &matProj );
	iexSystem::Device->SetTransform( D3DTS_VIEW, &matView );
}














//*****************************************************************************
//
//	過去のＩＥＸとの互換用
//
//*****************************************************************************

static iexView view;

//
//		投影設定
//

void	IEX_SetProjection( float FovY, float Near, float Far )
{
	view.SetProjection( FovY, Near, Far );
	view.Activate();
}

void	IEX_SetProjectionEx( float FovY, float Near, float Far, float asp )
{
	view.SetProjection( FovY, Near, Far, asp );
	view.Activate();
}

void	IEX_SetViewport( int x, int y, int w, int h )
{
	view.SetViewport(x,y,w,h); 
	view.Activate();
}

//*****************************************************************************
//
//		視界関連
//
//*****************************************************************************

//
//		視点設定
//

void	IEX_SetView( float posx, float posy, float posz, float anglex, float angley, float anglez )
{
	view.Set( posx, posy, posz, anglex, angley, anglez );
	view.Activate();
}

//
//		視点設定２
//

void	IEX_SetViewTarget( Vector3& pos, Vector3& target )
{
	view.Set( pos, target );
	view.Activate();
}

