
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
	bool		loadflg;		
	ImageObj	load_anykey;	//	�{�^��
	ImageObj	bgImage[2];


	//�f�o�b�N�p
	char	stri[256];
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