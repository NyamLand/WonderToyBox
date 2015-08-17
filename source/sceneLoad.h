
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
	static bool	threadState;		//	�X���b�h�̏��

public:
	//	�������E���
	sceneLoad( Scene* nextScene );
	~sceneLoad( void );
	bool	Initialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	����֐�
	static	void	Thread( void* arg );
};

//*******************************************************************************
#endif // !__SCENELOAD_H__