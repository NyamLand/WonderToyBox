
#ifndef __SCENETITLE_H__
#define	__SCENETITLE_H__

//*****************************************************************************
//
//	sceneTitle�N���X
//
//*****************************************************************************

class sceneTitle : public	Scene
{
private:
	iexView*	view;

public:
	//	�������E���
	sceneTitle( void );
	~sceneTitle( void );
	bool	Initialize( void );
	void	InitSound( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
};

//*****************************************************************************
#endif // !__SCENETITLE_H__
