
#ifndef __SCENETITLE_H__
#define	__SCENETITLE_H__

//*****************************************************************************
//
//	sceneTitleクラス
//
//*****************************************************************************

class sceneTitle : public	Scene
{
private:
	iexView*	view;

public:
	//	初期化・解放
	sceneTitle( void );
	~sceneTitle( void );
	bool	Initialize( void );
	void	InitSound( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );
};

//*****************************************************************************
#endif // !__SCENETITLE_H__
