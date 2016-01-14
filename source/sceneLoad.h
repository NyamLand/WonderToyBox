
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
	bool		loadflg;		
	ImageObj	load_anykey;	//	ボタン
	ImageObj	bgImage[2];


	//デバック用
	char	stri[256];
public:
	//	初期化・解放
	sceneLoad( Scene* nextScene );
	~sceneLoad( void );
	bool	Initialize( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	動作関数
	static	void	Thread( void* arg );
};

//*******************************************************************************
#endif // !__SCENELOAD_H__