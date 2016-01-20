#ifndef __STAGEMANAGER_H__
#define __STAGEMANAGER_H__

#include	"iextreme.h"
#include	<list>

class StageManager
{
private:
	iexMesh*				obj_collision;		//　当たり判定用
	iexMesh*				obj_stage;			//　ステージ
	iexMesh*				obj_ex1;				//　おまけ用（川とか）

	int		stage_type;
	float		_adjustV;

private:
	//	コンストラクタ・デストラクタ
	StageManager( void );
	~StageManager( void );

public:
	//　初期化・解放
	bool Initialize( Vector3 dir );
	void Release();

	//　更新
	void Update();
	void StageDeskUpdate();
	void StageForestUpdate();
	void StageToyUpdate();

	//　描画
	void Render( iexShader* shader = nullptr, LPSTR technique = nullptr );

	//　情報取得
	static	StageManager*	GetInstance( void );
};

#define stageManager	( StageManager::GetInstance() )

#endif