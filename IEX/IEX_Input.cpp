#include	"iextreme.h"

//*****************************************************************************
//
//		���̓f�o�C�X�Ǘ�
//
//*****************************************************************************

LPDIRECTINPUT8 iexInputManager::pDI = NULL;
int iexInputManager::NumDevice = 0;
DIDEVICEINSTANCE	iexInputManager::didi[4];

//*****************************************************************************
//		������
//*****************************************************************************
//------------------------------------------------------
//		�R���g���[���[��
//------------------------------------------------------
BOOL CALLBACK iexInputManager::EnumDeviceCallback( const DIDEVICEINSTANCE* pdidi, VOID* pContext )
{
	//	���̃R�s�[
	memcpy( &didi[NumDevice], pdidi, sizeof(DIDEVICEINSTANCE) );
	//	�p�b�h���X�V
	NumDevice++;
	//	�ő�S�R�܂ŁB
	if( NumDevice >= 4 ) return DIENUM_STOP;

	return DIENUM_CONTINUE;
}

//------------------------------------------------------
//		�R���g���[���[�Ǘ�������
//------------------------------------------------------
void iexInputManager::Initialize()
{
	if( DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDI, NULL) != DI_OK ) return;
	pDI->Initialize( GetModuleHandle(NULL), DIRECTINPUT_VERSION);
	//	�Q�[���p�b�h�̗�
	NumDevice = 0;
	pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumDeviceCallback, pDI, DIEDFL_ATTACHEDONLY );
}


//*****************************************************************************
//		�R���g���[���[������
//*****************************************************************************
//------------------------------------------------------
//		����
//------------------------------------------------------
BOOL CALLBACK iexInputManager::EnumAxes(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	// ���͈͂�ݒ�i-1000�`1000�j
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
//		�R���g���[���[������
//------------------------------------------------------
LPDIRECTINPUTDEVICE8 iexInputManager::GetDevice( int n )
{
	HRESULT	hr;
	LPDIRECTINPUTDEVICE8	lpDevice;

	//	�f�o�C�X����
	hr = pDI->CreateDevice( didi[n].guidInstance, &lpDevice, NULL);
	if( FAILED(hr) ) return NULL;

	if( lpDevice->SetDataFormat( &c_dfDIJoystick2 ) != DI_OK ) return FALSE;
	if( lpDevice->SetCooperativeLevel( iexSystem::Window, DISCL_EXCLUSIVE | DISCL_FOREGROUND ) != DI_OK ) return FALSE;

	//	�����Z���^�����O����
	DIPROPDWORD	dipdw;
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = DIPROPAUTOCENTER_OFF;
    lpDevice->SetProperty( DIPROP_AUTOCENTER, &dipdw.diph );

	// �e���ݒ�
	lpDevice->EnumObjects(EnumAxes, lpDevice, DIDFT_AXIS);

	// ���͐���J�n
	lpDevice->Acquire();

	return lpDevice;
}


//**************************************************************************************************
//
//		�R���g���[���[�P��
//
//**************************************************************************************************

const int iexInput::OrgKeyMap[20] = { 'W', 'S', 'A', 'D', 'Z', 'X', 'C', 'V', '1', '2', VK_F1, VK_F2, VK_F3, VK_F4, VK_RETURN, VK_SPACE, VK_BACK, VK_SHIFT, VK_CONTROL, VK_F5 };
const int iexInput::OrgJoyMap[20] = { 0,1,2,3, 4,5,6,7, 8,9,10,11,12,13,14,15,16,17,18,19 };

//*****************************************************************************
//
//*****************************************************************************
//------------------------------------------------------
//		������
//------------------------------------------------------
iexInput::iexInput( int n )
{
	//	�Q�[���p�b�h������
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
//		���
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
//		�U���֘A
//**************************************************************************************************
//------------------------------------------------------
//		�U��������
//------------------------------------------------------
BOOL	iexInput::InitVibration()
{
	HRESULT hr;

	// �G�t�F�N�g�����ݒ�
	DIPERIODIC diPeriodic;
	diPeriodic.dwMagnitude = DI_FFNOMINALMAX; 
	diPeriodic.lOffset = 0; 
	diPeriodic.dwPhase = 0; 
	diPeriodic.dwPeriod = (DWORD)(0.5f * DI_SECONDS); 

	// �U���G�t�F�N�g�ݒ�
	DIEFFECT diEffect;
	diEffect.dwSize         = sizeof(diEffect);
	diEffect.dwFlags        = DIEFF_OBJECTOFFSETS;
	diEffect.dwDuration     = 0;
	diEffect.dwSamplePeriod = 0;
	diEffect.dwGain         = DI_FFNOMINALMAX;
	//	�g���K�[�ݒ�
	diEffect.dwTriggerButton         = DIEB_NOTRIGGER;
	diEffect.dwTriggerRepeatInterval = 0;
	//	�G�t�F�N�g���ݒ�
	diEffect.dwFlags     |= DIEFF_POLAR;
	DWORD Axes[]          ={DIJOFS_X, DIJOFS_Y}; // �G�t�F�N�g��
	LONG Direction[]      = {1, 0};				 // �G�t�F�N�g����
	diEffect.cAxes        = 2;
	diEffect.rgdwAxes     = Axes;
	diEffect.rglDirection = Direction;
	//	�����ݒ�
	diEffect.lpEnvelope   = NULL; // �G���x���[�v�\����
	diEffect.cbTypeSpecificParams  = sizeof(diPeriodic); // �G�t�F�N�g�����\���̂̃T�C�Y
	diEffect.lpvTypeSpecificParams = &diPeriodic; // �G�t�F�N�g�����\����

	hr = lpDevice->CreateEffect( GUID_Square, &diEffect, &pEffect, NULL );
	if (FAILED(hr)) 
	{
		return FALSE;
	}

	return TRUE;
}

//------------------------------------------------------
//		�U��
//------------------------------------------------------
void iexInput::Vibration( u32 gain, float period )
{
	if( pEffect == NULL ) return;

	pEffect->Stop();
	if( gain == 0 ) return;

	// �U���G�t�F�N�g�ݒ�
	DIEFFECT diEffect;
	diEffect.dwSize     = sizeof(diEffect);
	diEffect.dwFlags    = DIEFF_POLAR | DIEFF_OBJECTOFFSETS;
	diEffect.dwDuration = (DWORD)( (period+0.2f) * DI_SECONDS); // �G�t�F�N�g�p������
	diEffect.dwGain     = gain;
	// �U���G�t�F�N�g�K�p
	pEffect->SetParameters( &diEffect, DIEP_DURATION | DIEP_GAIN );
	//	�U��
	pEffect->Start(1, 0);
}


//*****************************************************************************
//		�L�[�ݒ�
//*****************************************************************************

//                  ��X��   ��Y��   �EX��    �EY��    A  B  X  Y   LB LT L3   R1 R2 R3   St Sel
PADSET	iexInput::ps101   = { AXIS_X, AXIS_Y, AXIS_Z,  AXIS_RZ, 2, 3, 1, 4,   7, 5,11,   8, 6,12,   9,10 };	// ELECOM JS-PS101USV
PADSET	iexInput::sixaxis = { AXIS_X, AXIS_Y, AXIS_Z,  AXIS_RZ, 2, 3, 1, 4,   7, 5,10,   8, 6,11,  12, 9 };	// SixAxis	
PADSET	iexInput::xbox360 = { AXIS_X, AXIS_Y, AXIS_RX, AXIS_RY, 1, 2, 3, 4,   5,11, 9,   6,12,10,   7, 8 };	// XBOX360 Controller

//------------------------------------------------------
//		�L�[�{�[�h�ݒ�
//------------------------------------------------------
void iexInput::Asign( KEYSET& keyset )
{
	//	�㉺���E�L�[
	KeyMap[KEY_UP]    = keyset.up;
	KeyMap[KEY_DOWN]  = keyset.down;
	KeyMap[KEY_LEFT]  = keyset.left;
	KeyMap[KEY_RIGHT] = keyset.right;
	//	���C���{�^��
	KeyMap[KEY_A] = keyset.A;
	KeyMap[KEY_B] = keyset.B;
	KeyMap[KEY_C] = keyset.X;
	KeyMap[KEY_D] = keyset.Y;
	//	�k�q�L�[
	KeyMap[KEY_L1] = keyset.L1;
	KeyMap[KEY_L2] = keyset.L2;
	KeyMap[KEY_L3] = keyset.L3;

	KeyMap[KEY_R1] = keyset.R1;
	KeyMap[KEY_R2] = keyset.R2;
	KeyMap[KEY_R3] = keyset.R3;
	//	�X�^�[�g�E�Z���N�g
	KeyMap[KEY_START]  = keyset.START;
	KeyMap[KEY_SELECT] = keyset.SELECT;
}
//------------------------------------------------------
//		�Q�[���p�b�h�ݒ�
//------------------------------------------------------
void iexInput::PadAsign( PADSET& padset )
{
	//	���X�e�B�b�N
	JoyMap[0]     = padset.lx;
	JoyMap[1]     = padset.ly;
	//	�E�X�e�B�b�N
	JoyMap[2]     = padset.rx;
	JoyMap[3]     = padset.ry;
	//	���C���{�^��
	JoyMap[KEY_A] = padset.A - 1;
	JoyMap[KEY_B] = padset.B - 1;
	JoyMap[KEY_C] = padset.X - 1;
	JoyMap[KEY_D] = padset.Y - 1;
	//	�k�q�L�[
	JoyMap[KEY_L1] = padset.L1 - 1;
	JoyMap[KEY_L2] = padset.L2 - 1;
	JoyMap[KEY_L3] = padset.L3 - 1;

	JoyMap[KEY_R1] = padset.R1 - 1;
	JoyMap[KEY_R2] = padset.R2 - 1;
	JoyMap[KEY_R3] = padset.R3 - 1;
	//	�X�^�[�g�E�Z���N�g
	JoyMap[KEY_START]  = padset.START - 1;
	JoyMap[KEY_SELECT] = padset.SELECT - 1;
}

//**************************************************************************************************
//		���ݒ�
//**************************************************************************************************
void iexInput::SetInfo()
{
	PadAxisX  = PadAxisY  = 0;
	PadAxisX2 = PadAxisY2 = 0;

	if( lpDevice ){
		//------------------------------------------------------
		//	�p�b�h
		//------------------------------------------------------
		if( FAILED(lpDevice->Poll()) ){
			HRESULT hr;
			do { hr = lpDevice->Acquire(); } while( hr == DIERR_INPUTLOST );
			lpDevice->Poll();
		}
		
		DIJOYSTATE2 js;
		if( lpDevice->GetDeviceState(sizeof(DIJOYSTATE2),&js) == DI_OK ){
			//	����ԕۑ�
			long	axis[6];
			axis[0] = js.lX;
			axis[1] = js.lY;
			axis[2] = js.lZ;
			axis[3] = js.lRx;
			axis[4] = js.lRy;
			axis[5] = js.lRz;

			//	���ݒ�
			PadAxisX  = axis[ JoyMap[0] ];	//	���X�e�B�b�N�w��
			PadAxisY  = axis[ JoyMap[1] ];	//	���X�e�B�b�N�x��
			PadAxisX2 = axis[ JoyMap[2] ];	//	�E�X�e�B�b�N�w��
			PadAxisY2 = axis[ JoyMap[3] ];	//	�E�X�e�B�b�N�x��
			
			//	�n�b�g�X�C�b�`����
			u32	angle = 8;
			u8	povkey[9] = { 0x01, 0x09, 0x08, 0x0A, 0x02, 0x06, 0x04, 0x05, 0x00 };
			//	�W�����擾
			if( LOWORD(js.rgdwPOV[0]) != 0xFFFF ) angle = js.rgdwPOV[0] / 4500;
			//	�㉺���E�L�[�ݒ�
			for( int dir=0 ; dir<4 ; dir++ )
			{
				if( povkey[angle] & (0x01<<dir) ){
					if( JoyInfo[dir] & 0x01 ) JoyInfo[dir] = 1; else JoyInfo[dir] = 3;
				} else {
					if( JoyInfo[dir] & 0x01 ) JoyInfo[dir] = 2; else JoyInfo[dir] = 0;
				}
			}

			//	�{�^��
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
	//	�L�[�{�[�h
	//------------------------------------------------------
	//	�L�[�ɂ�鎲�ݒ�
	if( GetAsyncKeyState(VK_UP)    < 0 ) PadAxisY = -1000;
	if( GetAsyncKeyState(VK_DOWN)  < 0 ) PadAxisY = 1000;
	if( GetAsyncKeyState(VK_LEFT)  < 0 ) PadAxisX = -1000;
	if( GetAsyncKeyState(VK_RIGHT) < 0 ) PadAxisX = 1000;
	//	�����K��
	int	work = PadAxisX*PadAxisX + PadAxisY*PadAxisY;
	if( work > 1000*1000 )
	{
		work = (int)sqrtf((float)work);
		PadAxisX = PadAxisX*1000 / work;
		PadAxisY = PadAxisY*1000 / work;
	}

	//	�L�[�ɂ�鎲�ݒ�
	if( GetAsyncKeyState(VK_NUMPAD8) < 0 ) PadAxisY2 = -1000;
	if( GetAsyncKeyState(VK_NUMPAD2) < 0 ) PadAxisY2 = 1000;
	if( GetAsyncKeyState(VK_NUMPAD4) < 0 ) PadAxisX2 = -1000;
	if( GetAsyncKeyState(VK_NUMPAD6) < 0 ) PadAxisX2 = 1000;
	//	�����K��
	work = PadAxisX2*PadAxisX2 + PadAxisY2*PadAxisY2;
	if( work > 1000*1000 )
	{
		work = (int)sqrtf((float)work);
		PadAxisX2 = PadAxisX2*1000 / work;
		PadAxisY2 = PadAxisY2*1000 / work;
	}


	//	�{�^�����ݒ�
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
//		���擾
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
//		�A�N�Z�X�֐�
//
//**************************************************************************************************
namespace
{
	const	int	MAX = 4;
}

iexInput*	input[MAX];
//static iexInput*	input;

//*****************************************************************************************************************************
//		�L�[���֘A
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
//		�L�[��Ԑݒ�E�擾
//------------------------------------------------------
//	�L�[��Ԑݒ�
void KEY_SetInfo()
{ 
	for ( int i = 0; i < MAX; i++ )	input[i]->SetInfo();
}

//	�L�[��Ԏ擾
int	KEY_Get( KEYCODE key ){ return input[0]->Get(key); }

//	���擾
int KEY_GetAxisX() { return input[0]->Get( KEY_AXISX );  }
int KEY_GetAxisY() { return input[0]->Get( KEY_AXISY );  }
int KEY_GetAxisX2(){ return input[0]->Get( KEY_AXISX2 ); }
int KEY_GetAxisY2(){ return input[0]->Get( KEY_AXISY2 ); }

//*****************************************************************************************************************************
//	���͊֘A�������E���
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



