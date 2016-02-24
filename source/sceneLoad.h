
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
	
	//	画像関連
	ImageObj		bgImage[4];
	ImageObj		pressAnyKeyImage;
	ImageObj		moveImage;
	ImageObj		backImage;
	iex2DObj*	nowLoading;

	//	パラメータ
	float	t;
	bool	loadflg;
	bool	isEnd;
	bool	changeSceneFlag;
	int	reverseFlag;
	int	loadingTimer;
	int	renderCount;
	int	order;
	int	num;
	int	timer;

public:
	//	初期化・解放
	sceneLoad( Scene* nextScene );
	~sceneLoad( void );
	bool	Initialize( void );
	void	Release( void );
	void	Load( void );

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