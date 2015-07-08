#include	"iExtreme.h"

//*****************************************************************************
//
//		���E�֘A
//
//*****************************************************************************

//*****************************************************************************
//		���E�N���A
//*****************************************************************************
//------------------------------------------------------
//	�J���[�o�b�t�@�{�y�o�b�t�@�N���A
//------------------------------------------------------
void iexView::Clear( DWORD color, bool bClearZ )
{
	DWORD	flag = D3DCLEAR_TARGET;

	if( bClearZ ) flag |= D3DCLEAR_ZBUFFER;
	iexSystem::GetDevice()->Clear( 0, NULL, flag, color, 1.0f, 0 );
}

//------------------------------------------------------
//	�y�o�b�t�@�N���A
//------------------------------------------------------
void iexView::ClearZ()
{
	iexSystem::GetDevice()->Clear( 0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );
}


//*****************************************************************************
//		���_�ݒ�
//*****************************************************************************
//------------------------------------------------------
//	���_�E�����_�ݒ�
//------------------------------------------------------
void iexView::Set( Vector& pos, Vector& target )
{
	Pos = pos;
	Target = target;
}

//------------------------------------------------------
//	�ʒu�E�p�x�ݒ�
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
//		���e���ʐݒ�
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
//		���e�ݒ�
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
//		�K�p
//*****************************************************************************
void iexView::Activate()
{
	//	�r���[�s��ݒ�
	Vector p, t;
	p = Pos;
	t = Target;
	LookAtLH( matView, p, t, Vector3( .0f, 1.0f, .0f ) );

	//	�r���[�|�[�g�ݒ�
	if( Viewport.Width != 0 ) iexSystem::GetDevice()->SetViewport( &Viewport );

	//	���e�s��ݒ�
	PerspectiveLH( matProj, FovY, Aspect, Near, Far );

	//	�O���[�o���ւ̓K�p
	::matProjection = matProj;
	::matView       = matView;

	//	DirectX�ݒ�
	iexSystem::Device->SetTransform( D3DTS_PROJECTION, &matProj );
	iexSystem::Device->SetTransform( D3DTS_VIEW, &matView );
}














//*****************************************************************************
//
//	�ߋ��̂h�d�w�Ƃ̌݊��p
//
//*****************************************************************************

static iexView view;

//
//		���e�ݒ�
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
//		���E�֘A
//
//*****************************************************************************

//
//		���_�ݒ�
//

void	IEX_SetView( float posx, float posy, float posz, float anglex, float angley, float anglez )
{
	view.Set( posx, posy, posz, anglex, angley, anglez );
	view.Activate();
}

//
//		���_�ݒ�Q
//

void	IEX_SetViewTarget( Vector3& pos, Vector3& target )
{
	view.Set( pos, target );
	view.Activate();
}

