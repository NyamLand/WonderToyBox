
#ifndef __SCENETITLE_H__
#define	__SCENETITLE_H__

//*****************************************************************************
//
//	sceneTitle�N���X
//
//*****************************************************************************

class sceneTitle : public	Scene
{
private:	//	�萔
	static const int STAGE_MAX = 8;

private:	//	�\����

	//	�e�L�����N�^�[���
	struct CharacterInfo
	{
		LPSTR	name;		//	�L�����N�^�[��
		bool	select;		//	�I������Ă��邩
	};

	//	�I�����
	struct SelectInfo
	{
		int		characterType[4];
		int 	playerNum;		//�@���l�ŗV�ԁH
		int		stageType;		//�@�ǂ̃X�e�[�W�H
		bool	ready;			//	����OK���H
		int		step_cs;
	};

	//	�J�����p�p�����[�^�\����
	struct CameraInfo
	{
		int			posNum;				//	���̃J�����ړ���̃|�W�V�������w�������l
		float		t;					//	��ԃp�����[�^
		Vector3		pos;				//	�J�����|�W�V����
		Vector3		target;				//	�J�����̃^�[�Q�b�g�|�W�V����
		Vector3		lerpStartPos;		//	���[�v�X�^�[�g�|�W�V����
	};

private:
	//	�J�����p�p�����[�^
	CameraInfo	cameraInfo;

	//	�Q�[���ݒ�p�p�����[�^
	CharacterInfo	characterInfo[CHARACTER_TYPE::MAX];
	SelectInfo		selectInfo;
	int				mode;

	//	�^�C�g���p�p�����[�^
	iex2DObj*	curtain;
	iexMesh*	stage;


public:
	//	�������E���
	sceneTitle( void );
	~sceneTitle( void );
	bool	Initialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//-------------------------------------
	//�@�e��ʂ��Ƃ̃��\�b�h
	//-------------------------------------
	//�@�^�C�g��
	void	TitleUpdate( void );
	void	TitleRender( void );

	//�@���j���[���
	void	MenuUpdate( void );
	void	MenuRender( void );

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

	//�@�I�v�V����
	void	OptionUpdate( void );
	void	OptionRender( void );

	//�@�N���W�b�g
	void	CreditUpdate( void );
	void	CreditRender( void );
};

//*****************************************************************************
#endif // !__SCENETITLE_H__
