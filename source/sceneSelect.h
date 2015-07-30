
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
	enum 
	{
		SELECT_PLAYERNUM,
		SELECT_CHARACTER,
		SELECT_STAGE,
		SELECT_CHECK
	};

	iexView*	view;
	int mode;

public:
	//	�������E���
	sceneSelect( void );
	~sceneSelect( void );
	bool	Initialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//-------------------------------------
	//�@�e��ʂ��Ƃ̃��\�b�h
	//-------------------------------------
	//�@�l���I��
	void	SelectPlayerNumUpdate( void );
	void	SelectPlayerNumRender( void );
	//�@�L�����I��
	void	SelectCharacterUpdate( void );
	void	SelectCharacterRender( void );
	//�@�X�e�[�W�I��
	void	SelectStageUpdate( void );
	void	SelectStageRender( void );
	//�@�ŏI�m�F
	void	SelectCheckUpdate( void );
	void	SelectCheckRender( void );
};

//******************************************************************************
#endif // !__SCENESELECT_H__
