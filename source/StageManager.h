#ifndef __STAGEMANAGER_H__
#define __STAGEMANAGER_H__

#include	"iextreme.h"
#include	<list>

class StageManager
{
private:
	iexMesh*				obj_collision;		//�@�����蔻��p
	iexMesh*				obj_stage;			//�@�X�e�[�W
	iexMesh*				obj_ex1;				//�@���܂��p�i��Ƃ��j

	int		stage_type;
	float		_adjustV;

private:
	//	�R���X�g���N�^�E�f�X�g���N�^
	StageManager( void );
	~StageManager( void );

public:
	//�@�������E���
	bool Initialize( Vector3 dir );
	void Release();

	//�@�X�V
	void Update();
	void StageDeskUpdate();
	void StageForestUpdate();
	void StageToyUpdate();

	//�@�`��
	void Render( iexShader* shader = nullptr, LPSTR technique = nullptr );

	//�@���擾
	static	StageManager*	GetInstance( void );
};

#define stageManager	( StageManager::GetInstance() )

#endif