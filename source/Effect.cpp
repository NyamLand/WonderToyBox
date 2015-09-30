
#include	"iextreme.h"
#include	"system\System.h"
#include	"GlobalFunction.h"
#include	"Effect.h"
#include	"PlayerManager.h"
#include	"GameManager.h"

//*******************************************************************************
//
//	Effect�N���X
//
//*******************************************************************************

	//�@���̐錾
	Effect*		m_Effect = nullptr;

//-------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------

	//	�R���X�g���N�^
	Effect::Effect(void) : aura(nullptr), isAura(false), _adjustV(0)
	{
	
	}

	//	�f�X�g���N�^
	Effect::~Effect( void )
	{
		SafeDelete(aura);
	}

	//	������
	void	Effect::Initialize()
	{
		aura = new iexMesh("DATA\\Effect\\aura.IMO");
	}

//-------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------

	//	�X�V
	void	Effect::Update( void )
	{
		if (GameManager::GetMode() == GAME_MODE::CLIMAX)	isAura = true;
		if (isAura)
		{
			Vector3 p = m_Player->GetPos(GameManager::GetWorst()) + Vector3(0, 1.5f, 0);
			aura->SetPos(p);
			aura->SetScale(0.02f);
			aura->SetAngle(0.0f);
			aura->Update();
		}
		_adjustV += 0.02f;
		shader3D->SetValue("adjustV", _adjustV);
	}

	//	�`��
	void	Effect::Render( void )
	{
		if(isAura)	aura->Render(shader3D,"effect_add");
	}

//-------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------