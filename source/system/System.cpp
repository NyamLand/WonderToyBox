#include	"iextreme.h"
#include	"system.h"

//*****************************************************************************************************************************
//
//
//
//*****************************************************************************************************************************

iexShader*	shader;
iexShader*	shader2D;

//*****************************************************************************************************************************
//
//*****************************************************************************************************************************
void	SYSTEM_Initialize()
{
	//	標準キー設定
	//                 ↑　 ↓　 ←　 →　 ○   ×   △   □    L1   L2   L3   R1   R2   R3   Sta        Sel
	KEYSET	keyset = { 'W', 'S', 'A', 'D', 'Z', 'X', 'C', 'V',  'A', 'Q', '1', 'S', 'W', '2', VK_RETURN, VK_SPACE  };
	KEY_Asign(keyset);

	//	コントローラーセッティング(DUALSHOCK2-ELECOM JC-PS101USV)
	//                 左X軸   左Y軸   右X軸   右Y軸   ○ × △ □   L1 L2 L3   R1 R2 R3   Sta Sel
	PADSET	padset = { AXIS_X, AXIS_Y, AXIS_Z, AXIS_RZ, 2, 3, 1, 4,   7, 5,11,   8, 6,12,   9, 10 };
	KEY_PadAsign( padset );
	//KEY_PadAsign( iexInput::ps101 );		// ELECOM JC-PS101USV
	//KEY_PadAsign( iexInput::sixaxis );	// PS3 SIXAXIS DUALSHOCK3
	//KEY_PadAsign( iexInput::xbox360 );	// XBOX360 Controller

	//	シェーダー読み込み
	shader = new iexShader("DATA\\SHADER\\3DEx.fx");
	shader2D = new iexShader("DATA\\SHADER\\2D.fx");
	//	パーティクルシステム初期化
	iexParticle::Initialize("DATA\\PARTICLE.PNG", 10000 );
}

void	SYSTEM_Release()
{
	delete	shader;
	delete	shader2D;
	IEX_Release();
}



