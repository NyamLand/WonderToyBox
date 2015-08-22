
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
	//	�萔
	enum Title_Mode
	{
		TITLE,
		MENU,
		SELECT_PLAYERNUM,
		SELECT_CHARACTER,
		SELECT_STAGE,
		SELECT_CHECK,
		OPTION,
		CREDIT
	};

	//	�\����
	struct CharacterInfo
	{
		LPSTR name;
		bool	select;
	};

private:
	//	�I�u�W�F�N�g
	iexMesh*	m_CollisionStage;
	iexMesh*	m_Stage;	

	//	�J�����p�p�����[�^
	int			testpos;			//	���̃J�����ړ���̃|�W�V�������w�������l
	float		t;					//	��ԃp�����[�^
	Vector3		c_pos;		//	�J�����|�W�V����
	Vector3		t_pos;		//	�J�����̃^�[�Q�b�g�|�W�V����
	Vector3		s_pos;		//	���[�v�X�^�[�g�|�W�V����
	Quaternion orientation;	//	�N�H�[�^�j�I��
	
	//	�Q�[���ݒ�p�p�����[�^
	int		mode;
	CharacterInfo	characterInfo[PlayerData::CHARACTER_MAX];
	int 	playerNum;	//�@���l�ŗV�ԁH
	int		stageType;	//�@�ǂ̃X�e�[�W�H

public:
	//	�������E���
	sceneTitle( void );
	~sceneTitle( void );
	bool	Initialize( void );
	void	InitSound( void );

	//	�X�V�E�`��
	void	Update(void);
	void	Render(void);

	//-------------------------------------
	//�@�e��ʂ��Ƃ̃��\�b�h
	//-------------------------------------
	//�@�^�C�g��
	void	TitleUpdate(void);
	void	TitleRender(void);
	//�@���j���[���
	void	MenuUpdate(void);
	void	MenuRender(void);

	//�@�l���I��
	void	SelectPlayerNumUpdate(void);
	void	SelectPlayerNumRender(void);
	//�@�L�����I��
	void	SelectCharacterUpdate(void);
	void	SelectCharacterRender(void);
	//�@�X�e�[�W�I��
	void	SelectStageUpdate(void);
	void	SelectStageRender(void);
	//�@�ŏI�m�F
	void	SelectCheckUpdate(void);
	void	SelectCheckRender(void);

	//�@�I�v�V����
	void	OptionUpdate(void);
	void	OptionRender(void);

	//�@�N���W�b�g
	void	CreditUpdate(void);
	void	CreditRender(void);
};

//*****************************************************************************
#endif // !__SCENETITLE_H__
