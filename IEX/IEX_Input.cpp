#include	"iextreme.h"

//*****************************************************************************
//
//		入力デバイス管理
//
//*****************************************************************************

LPDIRECTINPUT8 iexInputManager::pDI = NULL;
int iexInputManager::NumDevice = 0;
DIDEVICEINSTANCE	iexInputManager::didi[4];

//*****************************************************************************
//		初期化
//*****************************************************************************
//------------------------------------------------------
//		コントローラー列挙
//------------------------------------------------------
BOOL CALLBACK iexInputManager::EnumDeviceCallback( const DIDEVICEINSTANCE* pdidi, VOID* pContext )
{
	//	情報のコピー
	memcpy( &didi[NumDevice], pdidi, sizeof(DIDEVICEINSTANCE) );
	//	パッド数更新
	NumDevice++;
	//	最大４コまで。
	if( NumDevice >= 4 ) return DIENUM_STOP;

	return DIENUM_CONTINUE;
}

//------------------------------------------------------
//		コントローラー管理初期化
//------------------------------------------------------
void iexInputManager::Initialize()
{
	if( DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDI, NULL) != DI_OK ) return;
	pDI->Initialize( GetModuleHandle(NULL), DIRECTINPUT_VERSION);
	//	ゲームパッドの列挙
	NumDevice = 0;
	pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumDeviceCallback, pDI, DIEDFL_ATTACHEDONLY );
}


//*****************************************************************************
//		コントローラー初期化
//*****************************************************************************
//------------------------------------------------------
//		軸列挙
//------------------------------------------------------
BOOL CALLBACK iexInputManager::EnumAxes(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	// 軸範囲を設定（-1000～1000）
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize       = sizeof(diprg); 
	diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
	diprg.diph.dwObj	    = lpddoi->dwType;
	diprg.diph.dwHow	    = DIPH_BYID;
	diprg.lMin	            = -1000;
	diprg.lMax	            = +1000;
	if( ((LPDIRECTINPUTDEVICE8)pvRef)->SetProperty(DIPROP_RANGE, &diprg.diph) != DI_OK ) return DIENUM_STOP;
		
	return DIENUM_CONTINUE;
}

//------------------------------------------------------
//		コントローラー初期化
//------------------------------------------------------
LPDIRECTINPUTDEVICE8 iexInputManager::GetDevice( int n )
{
	HRESULT	hr;
	LPDIRECTINPUTDEVICE8	lpDevice;

	//	デバイス生成
	hr = pDI->CreateDevice( didi[n].guidInstance, &lpDevice, NULL);
	if( FAILED(hr) ) return NULL;

	if( lpDevice->SetDataFormat( &c_dfDIJoystick2 ) != DI_OK ) return FALSE;
	if( lpDevice->SetCooperativeLevel( iexSystem::Window, DISCL_EXCLUSIVE | DISCL_FOREGROUND ) != DI_OK ) return FALSE;

	//	自動センタリング無効
	DIPROPDWORD	dipdw;
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = DIPROPAUTOCENTER_OFF;
    lpDevice->SetProperty( DIPROP_AUTOCENTER, &dipdw.diph );

	// 各軸設定
	lpDevice->EnumObjects(EnumAxes, lpDevice, DIDFT_AXIS);

	// 入力制御開始
	lpDevice->Acquire();

	return lpDevice;
}


//**************************************************************************************************
//
//		コントローラー単体
//
//**************************************************************************************************

const int iexInput::OrgKeyMap[20] = { 'W', 'S', 'A', 'D', 'Z', 'X', 'C', 'V', '1', '2', VK_F1, VK_F2, VK_F3, VK_F4, VK_RETURN, VK_SPACE, VK_BACK, VK_SHIFT, VK_CONTROL, VK_F5 };
const int iexInput::OrgJoyMap[20] = { 0,1,2,3, 4,5,6,7, 8,9,10,11,12,13,14,15,16,17,18,19 };

//*****************************************************************************
//
//*****************************************************************************
//------------------------------------------------------
//		初期化
//------------------------------------------------------
iexInput::iexInput( int n )
{
	//	ゲームパッド初期化
	lpDevice = NULL;
	pEffect = NULL;

	if( n != -1 ){
		lpDevice = iexInputManager::GetDevice(n);
		if( lpDevice ) InitVibration();
	}
	memcpy( KeyMap, OrgKeyMap, sizeof(KeyMap) );
	memcpy( JoyMap, OrgJoyMap, sizeof(JoyMap) );

	PadAxisX = 0;
	PadAxisY = 0;
	PadAxisX2 = 0;
	PadAxisY2 = 0;
	for( int i=0 ; i<20 ; i++ ) KeyInfo[i] = 0;
}

//------------------------------------------------------
//		解放
//------------------------------------------------------
iexInput::~iexInput()
{
	if( lpDevice ){
		lpDevice->Unacquire();
		lpDevice->Release();
	}
	if( pEffect ){
		pEffect->Stop();
		pEffect->Release();
	}
}

//**************************************************************************************************
//		振動関連
//**************************************************************************************************
//------------------------------------------------------
//		振動初期化
//------------------------------------------------------
BOOL	iexInput::InitVibration()
{
	HRESULT hr;

	// エフェクト周期設定
	DIPERIODIC diPeriodic;
	diPeriodic.dwMagnitude = DI_FFNOMINALMAX; 
	diPeriodic.lOffset = 0; 
	diPeriodic.dwPhase = 0; 
	diPeriodic.dwPeriod = (DWORD)(0.5f * DI_SECONDS); 

	// 振動エフェクト設定
	DIEFFECT diEffect;
	diEffect.dwSize         = sizeof(diEffect);
	diEffect.dwFlags        = DIEFF_OBJECTOFFSETS;
	diEffect.dwDuration     = 0;
	diEffect.dwSamplePeriod = 0;
	diEffect.dwGain         = DI_FFNOMINALMAX;
	//	トリガー設定
	diEffect.dwTriggerButton         = DIEB_NOTRIGGER;
	diEffect.dwTriggerRepeatInterval = 0;
	//	エフェクト軸設定
	diEffect.dwFlags     |= DIEFF_POLAR;
	DWORD Axes[]          ={DIJOFS_X, DIJOFS_Y}; // エフェクト軸
	LONG Direction[]      = {1, 0};				 // エフェクト方向
	diEffect.cAxes        = 2;
	diEffect.rgdwAxes     = Axes;
	diEffect.rglDirection = Direction;
	//	周期設定
	diEffect.lpEnvelope   = NULL; // エンベロープ構造体
	diEffect.cbTypeSpecificParams  = sizeof(diPeriodic); // エフェクト周期構造体のサイズ
	diEffect.lpvTypeSpecificParams = &diPeriodic; // エフェクト周期構造体

	hr = lpDevice->CreateEffect( GUID_Square, &diEffect, &pEffect, NULL );
	if (FAILED(hr)) 
	{
		return FALSE;
	}

	return TRUE;
}

//------------------------------------------------------
//		振動
//------------------------------------------------------
void iexInput::Vibration( u32 gain, float period )
{
	if( pEffect == NULL ) return;

	pEffect->Stop();
	if( gain == 0 ) return;

	// 振動エフェクト設定
	DIEFFECT diEffect;
	diEffect.dwSize     = sizeof(diEffect);
	diEffect.dwFlags    = DIEFF_POLAR | DIEFF_OBJECTOFFSETS;
	diEffect.dwDuration = (DWORD)( (period+0.2f) * DI_SECONDS); // エフェクト継続時間
	diEffect.dwGain     = gain;
	// 振動エフェクト適用
	pEffect->SetParameters( &diEffect, DIEP_DURATION | DIEP_GAIN );
	//	振動
	pEffect->Start(1, 0);
}


//*****************************************************************************
//		キー設定
//*****************************************************************************

//                  左X軸   左Y軸   右X軸    右Y軸    A  B  X  Y   LB LT L3   R1 R2 R3   St Sel
PADSET	iexInput::ps101   = { AXIS_X, AXIS_Y, AXIS_Z,  AXIS_RZ, 2, 3, 1, 4,   7, 5,11,   8, 6,12,   9,10 };	// ELECOM JS-PS101USV
PADSET	iexInput::sixaxis = { AXIS_X, AXIS_Y, AXIS_Z,  AXIS_RZ, 2, 3, 1, 4,   7, 5,10,   8, 6,11,  12, 9 };	// SixAxis	
PADSET	iexInput::xbox360 = { AXIS_X, AXIS_Y, AXIS_RX, AXIS_RY, 1, 2, 3, 4,   5,11, 9,   6,12,10,   7, 8 };	// XBOX360 Controller

//------------------------------------------------------
//		キーボード設定
//------------------------------------------------------
void iexInput::Asign( KEYSET& keyset )
{
	//	上下左右キー
	KeyMap[KEY_UP]    = keyset.up;
	KeyMap[KEY_DOWN]  = keyset.down;
	KeyMap[KEY_LEFT]  = keyset.left;
	KeyMap[KEY_RIGHT] = keyset.right;
	//	メインボタン
	KeyMap[KEY_A] = keyset.A;
	KeyMap[KEY_B] = keyset.B;
	KeyMap[KEY_C] = keyset.X;
	KeyMap[KEY_D] = keyset.Y;
	//	ＬＲキー
	KeyMap[KEY_L1] = keyset.L1;
	KeyMap[KEY_L2] = keyset.L2;
	KeyMap[KEY_L3] = keyset.L3;

	KeyMap[KEY_R1] = keyset.R1;
	KeyMap[KEY_R2] = keyset.R2;
	KeyMap[KEY_R3] = keyset.R3;
	//	スタート・セレクト
	KeyMap[KEY_START]  = keyset.START;
	KeyMap[KEY_SELECT] = keyset.SELECT;
}
//------------------------------------------------------
//		ゲームパッド設定
//------------------------------------------------------
void iexInput::PadAsign( PADSET& padset )
{
	//	左スティック
	JoyMap[0]     = padset.lx;
	JoyMap[1]     = padset.ly;
	//	右スティック
	JoyMap[2]     = padset.rx;
	JoyMap[3]     = padset.ry;
	//	メインボタン
	JoyMap[KEY_A] = padset.A - 1;
	JoyMap[KEY_B] = padset.B - 1;
	JoyMap[KEY_C] = padset.X - 1;
	JoyMap[KEY_D] = padset.Y - 1;
	//	ＬＲキー
	JoyMap[KEY_L1] = padset.L1 - 1;
	JoyMap[KEY_L2] = padset.L2 - 1;
	JoyMap[KEY_L3] = padset.L3 - 1;

	JoyMap[KEY_R1] = padset.R1 - 1;
	JoyMap[KEY_R2] = padset.R2 - 1;
	JoyMap[KEY_R3] = padset.R3 - 1;
	//	スタート・セレクト
	JoyMap[KEY_START]  = padset.START - 1;
	JoyMap[KEY_SELECT] = padset.SELECT - 1;
}

//**************************************************************************************************
//		情報設定
//**************************************************************************************************
void iexInput::SetInfo()
{
	PadAxisX  = PadAxisY  = 0;
	PadAxisX2 = PadAxisY2 = 0;

	if( lpDevice ){
		//------------------------------------------------------
		//	パッド
		//------------------------------------------------------
		if( FAILED(lpDevice->Poll()) ){
			HRESULT hr;
			do { hr = lpDevice->Acquire(); } while( hr == DIERR_INPUTLOST );
			lpDevice->Poll();
		}
		
		DIJOYSTATE2 js;
		if( lpDevice->GetDeviceState(sizeof(DIJOYSTATE2),&js) == DI_OK ){
			//	軸状態保存
			long	axis[6];
			axis[0] = js.lX;
			axis[1] = js.lY;
			axis[2] = js.lZ;
			axis[3] = js.lRx;
			axis[4] = js.lRy;
			axis[5] = js.lRz;

			//	軸設定
			PadAxisX  = axis[ JoyMap[0] ];	//	左スティックＸ軸
			PadAxisY  = axis[ JoyMap[1] ];	//	左スティックＹ軸
			PadAxisX2 = axis[ JoyMap[2] ];	//	右スティックＸ軸
			PadAxisY2 = axis[ JoyMap[3] ];	//	右スティックＹ軸
			
			//	ハットスイッチ判定
			u32	angle = 8;
			u8	povkey[9] = { 0x01, 0x09, 0x08, 0x0A, 0x02, 0x06, 0x04, 0x05, 0x00 };
			//	８方向取得
			if( LOWORD(js.rgdwPOV[0]) != 0xFFFF ) angle = js.rgdwPOV[0] / 4500;
			//	上下左右キー設定
			for( int dir=0 ; dir<4 ; dir++ )
			{
				if( povkey[angle] & (0x01<<dir) ){
					if( JoyInfo[dir] & 0x01 ) JoyInfo[dir] = 1; else JoyInfo[dir] = 3;
				} else {
					if( JoyInfo[dir] & 0x01 ) JoyInfo[dir] = 2; else JoyInfo[dir] = 0;
				}
			}

			//	ボタン
			for( int i=4 ; i<16 ; i++ ){
				if( js.rgbButtons[ JoyMap[i] ] & 0x80 ){
					if( JoyInfo[i] & 0x01 ) JoyInfo[i] = 1; else JoyInfo[i] = 3;
				} else {
					if( JoyInfo[i] & 0x01 ) JoyInfo[i] = 2; else JoyInfo[i] = 0;
				}
			}
		}
	}

	//------------------------------------------------------
	//	キーボード
	//------------------------------------------------------
	//	キーによる軸設定
	if( GetAsyncKeyState(VK_UP)    < 0 ) PadAxisY = -1000;
	if( GetAsyncKeyState(VK_DOWN)  < 0 ) PadAxisY = 1000;
	if( GetAsyncKeyState(VK_LEFT)  < 0 ) PadAxisX = -1000;
	if( GetAsyncKeyState(VK_RIGHT) < 0 ) PadAxisX = 1000;
	//	軸正規化
	int	work = PadAxisX*PadAxisX + PadAxisY*PadAxisY;
	if( work > 1000*1000 )
	{
		work = (int)sqrtf((float)work);
		PadAxisX = PadAxisX*1000 / work;
		PadAxisY = PadAxisY*1000 / work;
	}

	//	キーによる軸設定
	if( GetAsyncKeyState(VK_NUMPAD8) < 0 ) PadAxisY2 = -1000;
	if( GetAsyncKeyState(VK_NUMPAD2) < 0 ) PadAxisY2 = 1000;
	if( GetAsyncKeyState(VK_NUMPAD4) < 0 ) PadAxisX2 = -1000;
	if( GetAsyncKeyState(VK_NUMPAD6) < 0 ) PadAxisX2 = 1000;
	//	軸正規化
	work = PadAxisX2*PadAxisX2 + PadAxisY2*PadAxisY2;
	if( work > 1000*1000 )
	{
		work = (int)sqrtf((float)work);
		PadAxisX2 = PadAxisX2*1000 / work;
		PadAxisY2 = PadAxisY2*1000 / work;
	}


	//	ボタン情報設定
	for( int i=0 ; i<16 ; i++ ){
		if( lpDevice && JoyInfo[i] != 0x00 ){
			KeyInfo[i] = JoyInfo[i];
		} else {
			if( GetAsyncKeyState(KeyMap[i]) & 0x8000 ) work = 1; else work = 0;
			if( KeyInfo[i] & 0x01 ){ if( work ) KeyInfo[i] = 1; else KeyInfo[i] = 2; }
			 else                  { if( work ) KeyInfo[i] = 3; else KeyInfo[i] = 0; }
		}

	}
}


//*****************************************************************************
//		情報取得
//*****************************************************************************
int	iexInput::Get( KEYCODE key )
{
	//if( GetForegroundWindow() != iexSystem::Window ) return 0;

	switch( key )
	{
	case KEY_AXISX:		return PadAxisX;
	case KEY_AXISY:		return PadAxisY;
	case KEY_AXISX2:	return PadAxisX2;
	case KEY_AXISY2:	return PadAxisY2;
	}
	return KeyInfo[key];
}



//**************************************************************************************************
//
//		アクセス関数
//
//**************************************************************************************************
namespace
{
	const	int	MAX = 4;
}

iexInput*	input[MAX];
//static iexInput*	input;

//*****************************************************************************************************************************
//		キー情報関連
//*****************************************************************************************************************************

void KEY_PadAsign( PADSET& padset )
{
	for ( int i = 0; i < MAX; i++ )
	{
		input[i]->PadAsign( padset );
	}
}
void KEY_Asign( KEYSET& keyset )
{ 
	for ( int i = 0; i < MAX; i++ )
	{
		input[i]->Asign( keyset );
	}
}


void KEY_Vibration( u32 gain, float period )
{ 
	for ( int i = 0; i < MAX; i++ )
	{
		input[i]->Vibration( gain, period );
	}
}


//------------------------------------------------------
//		キー状態設定・取得
//------------------------------------------------------
//	キー状態設定
void KEY_SetInfo()
{ 
	for ( int i = 0; i < MAX; i++ )	input[i]->SetInfo();
}

//	キー状態取得
int	KEY_Get( KEYCODE key ){ return input[0]->Get(key); }

//	軸取得
int KEY_GetAxisX() { return input[0]->Get( KEY_AXISX );  }
int KEY_GetAxisY() { return input[0]->Get( KEY_AXISY );  }
int KEY_GetAxisX2(){ return input[0]->Get( KEY_AXISX2 ); }
int KEY_GetAxisY2(){ return input[0]->Get( KEY_AXISY2 ); }

//*****************************************************************************************************************************
//	入力関連初期化・解放
//*****************************************************************************************************************************
BOOL	IEX_InitInput()
{
	iexInputManager::Initialize();
	input[0] = new iexInput(0);
	input[1] = new iexInput(1);
	input[2] = new iexInput(2);
	input[3] = new iexInput(3);

	return TRUE;
}

void	IEX_ReleaseInput()
{
	input[0]->Vibration( 0, 0 );
	if (input[0])		delete	input[0];

	input[1]->Vibration(0, 0);
	if (input[1])		delete	input[1];

	input[2]->Vibration(0, 0);
	if (input[2])		delete	input[2];

	input[3]->Vibration(0, 0);
	if (input[3])		delete	input[3];

	iexInputManager::Release();
}



