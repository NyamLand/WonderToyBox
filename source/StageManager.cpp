
#include	"iextreme.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Collision.h"

#include	"StageManager.h"

	namespace STAGE_DATA
	{
		const float SCALE_FOREST = 1.0f;

		enum
		{
			TYPE_DESK,
			TYPE_FOREST
		};
	}
	
	//**************************************************************************************************
	//
	//	������
	//
	//**************************************************************************************************
	bool StageManager::Initialize( Vector3 dir )
	{
		stage_type = gameManager->GetStageType();
		_adjustV = 0.0f;

		switch (stage_type)
		{
		case STAGE_DATA::TYPE_DESK:	//	���X�e�[�W
			obj_collision = new iexMesh("DATA/back/Collision.IMO");
			obj_stage = new iexMesh("DATA/back/stage.IMO");
			obj_stage->SetAngle(D3DX_PI);
			iexLight::DirLight(shader3D, 0, &dir, 1.5f, 1.5f, 1.5f);
			break;
	
		case STAGE_DATA::TYPE_FOREST:	//	�X�X�e�[�W
			obj_collision = new iexMesh( "DATA/BG/Forest/Collision/collision_forest.IMO" );
			obj_stage = new iexMesh( "DATA/BG/Forest/model/forest2.IMO" );
			obj_stage->SetScale( STAGE_DATA::SCALE_FOREST );
			obj_ex1 = new iexMesh( "DATA/BG/Forest/model/forest_river.IMO" );
			obj_ex1->SetScale( STAGE_DATA::SCALE_FOREST );
			obj_ex1->SetPos( 0.0f, 1.0f, 0.0f );
			iexLight::DirLight(shader3D, 0, &dir, 0.5f, 0.5f, 0.5f);
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
		SafeDelete(obj_ex1);
		SafeDelete(obj_collision);
	}

	//**************************************************************************************************
	//
	//	�X�V
	//
	//**************************************************************************************************
	void StageManager::Update( void )
	{
		switch (stage_type)
		{
		case STAGE_DATA::TYPE_DESK:
			StageDeskUpdate();
			break;

		case STAGE_DATA::TYPE_FOREST:
			StageForestUpdate();
			break;
		}
		
		//�@���f���̒ʏ�X�V
		obj_stage->Update();
		obj_collision->Update();
		if (obj_ex1) obj_ex1->Update();

	}

	//�@���̃X�e�[�W
	void StageManager::StageDeskUpdate( void )
	{

	}

	//�@�X�̃X�e�[�W
	void StageManager::StageForestUpdate( void )
	{
		//�@��̗���iUV�A�j���j
		_adjustV -= 0.01f;
	}

	//**************************************************************************************************
	//
	//	�`��
	//
	//**************************************************************************************************
	void StageManager::Render()
	{
		obj_stage->Render();
		if (obj_ex1) obj_ex1->Render();
	}

	void StageManager::Render( iexShader* shader, LPSTR technique )
	{
		shader3D->SetValue( "adjustV", _adjustV );
		obj_ex1->Render( shader, "effect_add" );
		obj_stage->Render( shader, technique );
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