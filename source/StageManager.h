#ifndef __STAGEMANAGER_H__
#define __STAGEMANAGER_H__

//#include	"iextreme.h"

class StageManager
{
private:
	iexMesh*	obj_stage;			//�@�X�e�[�W
	iexMesh*	obj_collision;		//�@�����蔻��p


public:
	bool Initialize( Vector3 dir);
	void Release();

	void Render();
	void Render(iexShader* shader = nullptr, LPSTR technique = nullptr);

	//�@���擾
	static	StageManager*	GetInstance( void );
};

#define stageManager	( StageManager::GetInstance() )

#endif