#include	"iExtreme.h"

#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )

#pragma comment( lib, "dsound.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "dinput8.lib" )

//*****************************************************************************
//
//		�Q�c�t�H���g�֘A
//
//*****************************************************************************

LPD3DXFONT	g_pd3dFont  = NULL;
LPD3DXFONT	g_pd3dFontM = NULL;

//
//		�Q�c�t�H���g������
//

void	IEX_InitText( void )
{
	HFONT		hF;
	D3DXFONT_DESC	lf;

	/*	�t�H���g�ݒ�	*/ 
	hF = (HFONT)GetStockObject( SYSTEM_FONT );
	GetObject( hF,sizeof( LOGFONT ),(LPSTR)&lf );
	lf.Height = 32;
	lf.Width  = 0;
	lf.Italic = 0;
	lf.CharSet = SHIFTJIS_CHARSET;
	strcpy_s( lf.FaceName,"���C���I" );	//�l�r �S�V�b�N

	D3DXCreateFontIndirect( iexSystem::Device, &lf, &g_pd3dFont );

	lf.Height = 16;
	D3DXCreateFontIndirect( iexSystem::Device, &lf, &g_pd3dFontM );
}

void	IEX_ReleaseText( void )
{
	g_pd3dFont->Release();
	g_pd3dFontM->Release();
}

//
//		�e�L�X�g�`��
//

void	IEX_DrawText( LPSTR str, int x, int y, int width, int height, DWORD color, BOOL bMini )
{
	RECT	rect;

	rect.left   = x;
	rect.top    = y;
	rect.right  = x + width;
	rect.bottom = y + height;

	iexSystem::Device->SetRenderState(D3DRS_FOGENABLE, FALSE);

	if( !bMini ) g_pd3dFont->DrawText(  NULL, str, -1, &rect, DT_LEFT|DT_WORDBREAK, color ); 
	 else        g_pd3dFontM->DrawText( NULL, str, -1, &rect, DT_LEFT|DT_WORDBREAK, color );
}

