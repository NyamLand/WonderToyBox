
#ifndef __SCENELOAD_H__
#define	__SCENELOAD_H__

//*******************************************************************************
//
//	sceneLoadクラス
//
//*******************************************************************************
class sceneLoad : public	Scene
{
private:
	Scene*		newScene;		//	次のシーン
	static bool	threadState;	//	スレッドの状態
	
	//	演出系
	ImageObj	bgImage[4];
	ImageObj	pressAnyKeyImage;
	iex2DObj*	nowLoading;
	int	timer;

	float	t;
	bool	loadflg;
	bool	isEnd;
	bool	changeSceneFlag;
	int	reverseFlag;
	int	loadingTimer;
	int	renderCount;

public:
	//	初期化・解放
	sceneLoad( Scene* nextScene );
	~sceneLoad( void );
	bool	Initialize( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );
	void	RenderNowLoading( void );
	//	動作関数
	void	MoveNowLoading( void );
	void	MoveBG( void );
	static	void	Thread( void* arg );
};

//*******************************************************************************
#endif // !__SCENELOAD_H__