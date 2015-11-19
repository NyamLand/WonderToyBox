#ifndef __STAGEMANAGER_H__
#define __STAGEMANAGER_H__

//#include	"iextreme.h"

class StageManager
{
private:
	iexMesh*	obj_stage;			//　ステージ
	iexMesh*	obj_collision;		//　当たり判定用


public:
	bool Initialize( Vector3 dir);
	void Release();

	void Render();
	void Render(iexShader* shader = nullptr, LPSTR technique = nullptr);

	//　情報取得
	static	StageManager*	GetInstance( void );
};

#define stageManager	( StageManager::GetInstance() )

#endif