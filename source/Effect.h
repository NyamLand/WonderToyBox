
#ifndef __EFFECT_H__
#define	__EFFECT_H__

//*******************************************************************************
//	
//	Effectクラス
//
//*******************************************************************************

class Effect
{
private:
	iexMesh*	aura;
	bool		isAura;
	float		_adjustV;

public:
	//	初期化・解放
	Effect( void );
	~Effect( void );

	void	Initialize();

	//	更新・描画
	void	Update( void );
	void	Render( void );
};

extern	Effect*		m_Effect;

//*******************************************************************************
#endif // !__EFFECT_H__
