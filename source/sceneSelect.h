
#ifndef __SCENESELECT_H__
#define	__SCENESELECT_H__

//******************************************************************************
//
//	sceneSelect�N���X
//
//******************************************************************************
class sceneSelect : public Scene
{
private:
	iexView*	view;

public:
	//	�������E���
	sceneSelect( void );
	~sceneSelect( void );
	bool	Initialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
};

//******************************************************************************
#endif // !__SCENESELECT_H__
