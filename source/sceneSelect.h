
#ifndef __SCENESELECT_H__
#define	__SCENESELECT_H__

//******************************************************************************
//
//	sceneSelectクラス
//
//******************************************************************************
class sceneSelect : public Scene
{
private:
	iexView*	view;

public:
	//	初期化・解放
	sceneSelect( void );
	~sceneSelect( void );
	bool	Initialize( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );
};

//******************************************************************************
#endif // !__SCENESELECT_H__
