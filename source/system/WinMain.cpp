#include	"iextreme.h"
#include	"Framework.h"
#include	"GlobalFunction.h"
#include	"CharacterManager.h"

#include	"../sceneTitle.h"
#include	"../sceneMenu.h"
#include	"../sceneLoad.h"
#include	"../sceneMain.h"

//*****************************************************************************************************************************
//
//
//
//*****************************************************************************************************************************

char*	AppTitle = "IEX2010";

BOOL	bFullScreen = FALSE;
DWORD	ScreenMode = SCREEN1664;//SCREEN720p;

Framework*	MainFrame = NULL;

//*****************************************************************************************************************************
//		アプリケーション初期化
//*****************************************************************************************************************************

BOOL	InitApp( HWND hWnd )
{
	//	IEXシステム初期化
	IEX_Initialize( hWnd, bFullScreen, ScreenMode );
	IEX_InitAudio();
	IEX_InitInput();

	//	システムの初期化
	SYSTEM_Initialize();
	//	メインフレームワーク生成
	MainFrame = new Framework();
	//	初期シーン登録
	//MainFrame->ChangeScene( new sceneTitle() );	//　タイトル
	MainFrame->ChangeScene(new sceneLoad(new sceneMenu()));	//　メニュー
	//MainFrame->ChangeScene( new sceneMain() );	//　メイン

	return TRUE;
}

//*****************************************************************************************************************************
//		ウィンドウプロシージャ
//*****************************************************************************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case WM_DESTROY: PostQuitMessage(0); return 0;
	case WM_KEYDOWN:
			switch (wParam){
			case VK_ESCAPE: PostMessage(hWnd, WM_CLOSE, 0, 0); return 0;
			case VK_F1:		iexSystem::OpenDebugWindow(); return 0;
			}
			break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

//*****************************************************************************************************************************
//		ウィンドウ作成
//*****************************************************************************************************************************
HWND InitWindow( HINSTANCE hInstance, int nCmdShow )
{
	HWND			hWnd;

	//	スクリーンサイズ取得
	RECT	WindowSize;
	iexSystem::GetScreenRect( ScreenMode, WindowSize );

	//	ウィンドウクラス設定
	WNDCLASS	wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WindowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH )GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = "IEX2010";
	RegisterClass(&wc);

	//	ウィンドウ作成
	if( !bFullScreen ){
		AdjustWindowRect( &WindowSize, WS_OVERLAPPEDWINDOW, FALSE );
		hWnd = CreateWindow( "IEX2010", AppTitle, WS_OVERLAPPEDWINDOW, 
								0, 0, WindowSize.right-WindowSize.left, WindowSize.bottom-WindowSize.top,
								NULL, NULL, hInstance, NULL);
	} else {
		hWnd = CreateWindow( "IEX2010", AppTitle, WS_POPUP, 0, 0, WindowSize.right,WindowSize.bottom, NULL, NULL, hInstance, NULL);
		ShowCursor(FALSE);
	}
	if( !hWnd ) return NULL;
	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

    return hWnd;
}

//*****************************************************************************************************************************
//		WinMain
//*****************************************************************************************************************************
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	MSG		msg;
	HWND	hWnd;

	if( GetAsyncKeyState(VK_CONTROL)&0x8000 ) bFullScreen = TRUE;

	hWnd = InitWindow(hInstance, nCmdShow);
	InitApp(hWnd);

	//	メインループ
	for(;;){
		if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ){
			if( !GetMessage(&msg, NULL, 0, 0) ) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			if( MainFrame->Update() ) MainFrame->Render();
		}
	}

	//	全解放	
	delete	MainFrame;
	SYSTEM_Release();
	iexSystem::CloseDebugWindow();
	IEX_ReleaseInput();
	IEX_Release();

	return 0;
}
