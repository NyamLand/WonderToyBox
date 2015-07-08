#include	"iExtreme.h"

//*****************************************************************************
//
//	i-Extremeシステム
//
//*****************************************************************************

LPDIRECT3D9				iexSystem::lpD3D;
D3DPRESENT_PARAMETERS	iexSystem::d3dpp;

//------------------------------------------------------
//	グローバルパラメータ
//------------------------------------------------------

//	スクリーンパラメータ
D3DFORMAT	iexSystem::ScreenFormat;
DWORD		iexSystem::ScreenWidth;
DWORD		iexSystem::ScreenHeight;

//	３Ｄデバイス
LPDEVICE	iexSystem::Device;
HWND		iexSystem::Window;

//	変換行列
Matrix		matView;
Matrix		matProjection;

//*****************************************************************************
//
//		初期化
//
//*****************************************************************************
BOOL iexSystem::Initialize( HWND hWnd, BOOL bFullScreen, DWORD ScreenMode )
{
	//	スクリーンサイズ取得
	RECT	rc;
	GetScreenRect( ScreenMode, rc );
	int	Width  = rc.right;
	int	Height = rc.bottom;

	// D3Dオブジェクトの作成
	lpD3D = Direct3DCreate9( D3D_SDK_VERSION );

	// アダプタの現在のディスプレイ モードを取得する
	D3DDISPLAYMODE d3ddm;
	lpD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

	// D3Dデバイスの作成
	ZeroMemory( &d3dpp, sizeof(D3DPRESENT_PARAMETERS) );
	d3dpp.Flags							= 0;
	d3dpp.BackBufferHeight				= Height;
	d3dpp.BackBufferWidth				= Width;
	d3dpp.BackBufferCount				= 1;

	d3dpp.hDeviceWindow					= hWnd;

	d3dpp.EnableAutoDepthStencil		= TRUE;
	d3dpp.AutoDepthStencilFormat		= D3DFMT_D16;

//	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_ONE;
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;

	d3dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;
//	d3dpp.MultiSampleType				= D3DMULTISAMPLE_4_SAMPLES;
	d3dpp.MultiSampleQuality            = 0;

	if( bFullScreen ){
		d3dpp.Windowed						= !bFullScreen;
		d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat				= d3ddm.Format;
		d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	} else {
		d3dpp.Windowed						= TRUE;
		d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat				= D3DFMT_UNKNOWN;
	}

	//	デバイス作成
    D3DCAPS9 Caps;
	ZeroMemory( &Caps, sizeof( Caps ) );
	//	HALチェック
	if( SUCCEEDED( lpD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &Caps ) ) ){
		if( FAILED( lpD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED, &d3dpp, &Device ) ) ){
			if( FAILED( lpD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_MIXED_VERTEXPROCESSING|D3DCREATE_MULTITHREADED, &d3dpp, &Device ) ) ){
				if( FAILED( lpD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED, &d3dpp, &Device ) ) ) return FALSE;
			}
		}
	}
	else {
		ZeroMemory( &Caps, sizeof( Caps ) );
		lpD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, &Caps );
		if( FAILED( lpD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED, &d3dpp, &Device ) ) ) return FALSE;
	}

	//	システム初期化
	IEX_InitText();
	iexRenderState::Initialize();
	iexTexture::Initialize();

	//	グローバルパラメータ設定
	ScreenFormat = d3dpp.BackBufferFormat;
	ScreenWidth  = d3dpp.BackBufferWidth;
	ScreenHeight = d3dpp.BackBufferHeight;

	Window = hWnd;

	return TRUE;
}

//*****************************************************************************
//		モード別スクリーンサイズ取得
//*****************************************************************************
void iexSystem::GetScreenRect( DWORD mode, RECT& rc )
{
	rc.left = rc.top = 0;

	switch( mode ){
	case SCREEN640:	 rc.right = 640;  rc.bottom = 480;	break;
	case SCREEN800:	 rc.right = 800;  rc.bottom = 600;	break;
	case SCREEN1024: rc.right = 1024; rc.bottom = 768;	break;
	case SCREEN720p: rc.right = 1280; rc.bottom = 720;	break;

	default:
			rc.right = (mode>>16);
			rc.bottom = (mode&0xFFFF);
			break;
	}
}

//*****************************************************************************
//		
//		デバッグ用ウィンドウ
//
//*****************************************************************************
#include <stdio.h>
static FILE* DebugFP = NULL;

void iexSystem::OpenDebugWindow()
{
#ifdef _DEBUG
	AllocConsole();
	freopen_s( &DebugFP, "CON", "w", stdout);
#endif
}

void iexSystem::CloseDebugWindow()
{
#ifdef _DEBUG
	if( DebugFP )
	{
		fclose(DebugFP);
		FreeConsole();
	}
#endif
}

void iexSystem::printf( const char* format, ...)
{
#ifdef _DEBUG
	va_list arg; 
	va_start(arg, format);

	vprintf( format, arg );

	va_end(arg);
#endif
}
