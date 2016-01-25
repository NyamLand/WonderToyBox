
#ifndef __SCENELOAD_H__
#define	__SCENELOAD_H__

//*******************************************************************************
//
//	sceneLoad�N���X
//
//*******************************************************************************
class sceneLoad : public	Scene
{
private:
	Scene*		newScene;		//	���̃V�[��
	static bool	threadState;	//	�X���b�h�̏��
	
	//	���o�n
	ImageObj	bgImage[4];
	ImageObj	pressAnyKeyImage;
	ImageObj	moveImage;
	ImageObj	backImage;
	iex2DObj*	nowLoading;
	int	timer;

	float	t;
	bool	loadflg;
	bool	isEnd;
	bool	changeSceneFlag;
	int	reverseFlag;
	int	loadingTimer;
	int	renderCount;
	int	order;
	int	num;

public:
	//	�������E���
	sceneLoad( Scene* nextScene );
	~sceneLoad( void );
	bool	Initialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
	void	RenderNowLoading( void );
	//	����֐�
	void	MoveNowLoading( void );
	void	MoveBG( void );
	static	void	Thread( void* arg );
};

//*******************************************************************************
#endif // !__SCENELOAD_H__