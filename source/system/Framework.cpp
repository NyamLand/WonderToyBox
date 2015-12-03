#include	"iextreme.h"
#include	"system.h"
#include	"Framework.h"
#include	<time.h>


//*****************************************************************************************************************************
//
//		�t���[�����[�N
//
//*****************************************************************************************************************************

//------------------------------------------------------
//		�������E���
//------------------------------------------------------
Framework::Framework( int FPSMode )
{
	this->FPSMode = FPSMode;

	scene = NULL;

	dwGameFrame = 0;
	dwFrameTime = clock();
}

Framework::~Framework()
{
	if( scene != NULL ) delete scene;
}


//*****************************************************************************************************************************
//		�X�V
//*****************************************************************************************************************************
bool Framework::Update()
{
	static 	DWORD	dwSec = 0;
	DWORD CurrentTime = clock()*10;

	//	�t���[������
	if( CurrentTime < dwFrameTime+167 ) return false;

	//	�o�ߎ���
	DWORD	dTime = CurrentTime - dwFrameTime;
	if( dTime > 2000 ) dwFrameTime = CurrentTime;

	//	�X�L�b�v�^�C�v 
	switch( FPSMode ){
	case FPS_60:	bRender = TRUE;	break;
	case FPS_30:	if( dwGameFrame & 0x01 ) bRender=TRUE; else bRender=FALSE;
					break;
	case FPS_FLEX:	if( dTime > 167*2 ) bRender = FALSE; else bRender = TRUE;
					break;
	}

	//	�t���[�����ԍX�V
	if( GetKeyState(VK_LCONTROL) < 0 ) dwFrameTime += 300;
	dwFrameTime += 167;

	//	�b�ԃt���[�����ۑ�
	if( dwSec < CurrentTime ){
		dwFPS       = dwCurFrame;
		dwRenderFPS = dwRCurFrame;
		dwCurFrame  = dwRCurFrame = 0;
		dwSec += 10000;
	}
	dwCurFrame ++;	//	�t���[�����X�V
	dwGameFrame++;	//	�Q�[���t���[�����X�V

	//	�X�V����
	KEY_SetInfo();
	if( scene != NULL ) scene->Update();

	return true;
}

//*****************************************************************************************************************************
//		�`��
//*****************************************************************************************************************************
void Framework::Render()
{
	if( !bRender ) return;
	//	�V�[���J�n
	iexSystem::BeginScene(); 
	//	�V�[���`��
	if( scene != NULL ) scene->Render();

	//	�t���[���\��
	#ifdef _DEBUG
	char	str[64];
	wsprintf( str, "FPS %03d / %03d\n", dwFPS, dwRenderFPS );
	IEX_DrawText( str, 10,10,200,32, 0xFFFFFF00 );
	#endif

	// �V�[���I��
	iexSystem::EndScene();

	dwRCurFrame ++;	//	�`��t���[�����X�V
}
