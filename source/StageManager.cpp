
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
	//	‰Šú‰»
	//
	//**************************************************************************************************
	bool StageManager::Initialize( Vector3 dir )
	{
		switch (gameManager->GetStageType())
		{
		case 0:	//	Š÷ƒXƒe[ƒW
			obj_collision = new iexMesh("DATA/back/Collision.IMO");
			obj_stage = new iexMesh("DATA/back/stage.IMO");
			iexLight::DirLight(shader3D, 0, &dir, 1.5f, 1.5f, 1.5f);
			obj_stage->SetAngle(D3DX_PI);
			break;
	
		case 1:	//	XƒXƒe[ƒW
			obj_collision = new iexMesh("DATA/BG/Forest/Collision/collision_forest.IMO");
			obj_stage = new iexMesh("DATA/BG/Forest/model/forest.IMO");
			iexLight::DirLight(shader3D, 0, &dir, 0.5f, 0.5f, 0.5f);
			obj_stage->SetScale(STAGE_DATA::SCALE_FOREST);
			break;
		}

		//@“Ç‚İ‚İƒ`ƒFƒbƒN
		if (!obj_stage || !obj_collision) return false;
		
		//	Œ©‚½–Úƒ‚ƒfƒ‹‚ğ‚P‚W‚O‹‰ñ“]‚µ‚Äî•ñXV
		//m_Stage->SetAngle( D3DX_PI );
		obj_stage->Update();
	
		//	“–‚½‚è”»’è—pƒ‚ƒfƒ‹“o˜^
		Collision::Initiallize(obj_collision);

		return true;
	}
	//**************************************************************************************************
	//
	//	‰ğ•ú
	//
	//**************************************************************************************************

	void StageManager::Release( void )
	{
		SafeDelete(obj_stage);
		SafeDelete(obj_collision);
	}

	//**************************************************************************************************
	//
	//	•`‰æ
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
	//	î•ñİ’è
	//
	//**************************************************************************************************
	StageManager*	StageManager::GetInstance(void)
	{
		static	StageManager	out;
		return	&out;
	}