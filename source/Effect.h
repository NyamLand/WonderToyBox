
#ifndef __EFFECT_H__
#define	__EFFECT_H__

//*******************************************************************************
//	
//	Effect�N���X
//
//*******************************************************************************

class Effect
{
private:
	iexMesh*	aura;
	bool		isAura;
	float		_adjustV;

public:
	//	�������E���
	Effect( void );
	~Effect( void );

	void	Initialize();

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
};

extern	Effect*		m_Effect;

//*******************************************************************************
#endif // !__EFFECT_H__
