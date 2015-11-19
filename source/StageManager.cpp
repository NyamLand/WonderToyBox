
#include	"iextreme.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Collision.h"

#include	"StageManager.h"

	namespace STAGE_DATA
	{
		const float SCALE_FOREST = 1.0f;
	}
	
	//**************************************************************************************************
	//
	//	������
	//
	//**************************************************************************************************
	bool StageManager::Initialize( Vector3 dir )
	{
		switch (gameManager->GetStageType())
		{
		case 0:	//	���X�e�[�W
			obj_collision = new iexMesh("DATA/back/Collision.IMO");
			obj_stage = new iexMesh("DATA/back/stage.IMO");
			iexLight::DirLight(shader3D, 0, &dir, 1.5f, 1.5f, 1.5f);
			obj_stage->SetAngle(D3DX_PI);
			break;
	
		case 1:	//	�X�X�e�[�W
			obj_collision = new iexMesh("DATA/BG/Forest/Collision/collision_forest.IMO");
			obj_stage = new iexMesh("DATA/BG/Forest/model/forest.IMO");
			iexLight::DirLight(shader3D, 0, &dir, 0.5f, 0.5f, 0.5f);
			obj_stage->SetScale(STAGE_DATA::SCALE_FOREST);
			break;
		}

		//�@�ǂݍ��݃`�F�b�N
		if (!obj_stage || !obj_collision) return false;
		
		//	�����ڃ��f�����P�W�O����]���ď��X�V
		//m_Stage->SetAngle( D3DX_PI );
		obj_stage->Update();
	
		//	�����蔻��p���f���o�^
		Collision::Initiallize(obj_collision);

		return true;
	}
	//**************************************************************************************************
	//
	//	���
	//
	//**************************************************************************************************

	void StageManager::Release( void )
	{
		SafeDelete(obj_stage);
		SafeDelete(obj_collision);
	}

	//**************************************************************************************************
	//
	//	�`��
	//
	//**************************************************************************************************
	void StageManager::Render()
	{
		obj_stage->Render();
	}

	void StageManager::Render(iexShader* shader, LPSTR technique)
	{
		obj_stage->Render(shader, technique);
	}

	//**************************************************************************************************
	//
	//	���ݒ�
	//
	//**************************************************************************************************
	StageManager*	StageManager::GetInstance(void)
	{
		static	StageManager	out;
		return	&out;
	}