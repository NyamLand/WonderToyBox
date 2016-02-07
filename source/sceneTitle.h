
#ifndef		__SCENETITLE_H__
#define	__SCENETITLE_H__

//*****************************************************************************
//
//	sceneTitle�N���X
//
//*****************************************************************************

//	�^�C�g�����
namespace
{
	//	�^�C�g�����[�h
	namespace TITLE_MODE
	{
		enum
		{
			TITLE,
			MENU,
			PLAY,
			OPTION,
			CREDIT,
			MOVE_MAIN,
		};
	}
}

//	include
#include	"Curtain.h"

//	class
class sceneTitle : public	Scene
{
private:	//	�\����

	//	�e�L�����N�^�[���
	struct CharacterInfo
	{
		LPSTR	name;		//	�L�����N�^�[��
		bool	select;		//	�I������Ă��邩
	};

	//	�J�����p�p�����[�^�\����
	struct CameraInfo
	{
		int				posNum;				//	���̃J�����ړ���̃|�W�V�������w�������l
		float				t;					//	��ԃp�����[�^
		int				state;			//	���
		Vector3		pos;				//	�J�����|�W�V����
		Vector3		target;				//	�J�����̃^�[�Q�b�g�|�W�V����
		Vector3		lerpStartPos;		//	���[�v�X�^�[�g�|�W�V����
	};

	//	�J�[�e���p�\����
	struct CurtainInfo
	{
		iex2DObj*	obj;
		TLVERTEX	tlv[4];
		float			t;
	};

	//	�^�C�g���p�p�����[�^
	struct TITLE_INFO
	{
		int		step;
		int		nextMode;
		CurtainInfo	curtainR;
		CurtainInfo	curtainL;
		iex2DObj*	titleText;
		ImageObj		pressSpace;
		ImageObj		titleImage;
		ImageObj		gameStartImage;
		ImageObj		creditTextImage;
	};

	//	���j���[�p�p�����[�^
	struct MENU_INFO
	{
		int	 menu_num;
	};

	//	���I��p�p�����[�^
	struct SELECT_INFO
	{
		int		mode;
		int		select_mode;
		int		menu_num;
	};

	struct CREDIT_INFO
	{
		iex2DObj*	credit;
		int				sy;
		float				t;
	};

private:
	//	�J�[�e��
	Curtain*	curtain;

	//	�w�i
	iexMesh*	stage;

	//	�J�����p�p�����[�^
	CameraInfo	cameraInfo;

	//	�Q�[���ݒ�p�p�����[�^
	CharacterInfo	characterInfo[CHARACTER_TYPE::MAX];
	int				mode;

	//	�e���[�h�p�p�����[�^
	TITLE_INFO		titleInfo;
	MENU_INFO		menuInfo;
	SELECT_INFO	selectInfo;
	CREDIT_INFO	creditInfo;

public:
	//	�������E���
	sceneTitle( void );
	~sceneTitle( void );
	bool	Initialize( void );
	void	TitleInitialize( void );
	void	MenuInitialize( void );
	void	CreditInitialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	����֐�
	void	CurtainInitialize( void );

	//-------------------------------------
	//�@�e��ʂ��Ƃ̃��\�b�h
	//-------------------------------------
	//�@�^�C�g��
	void	TitleUpdate( void );
	void	TitleRender( void );

	//�@���j���[���
	void	MenuUpdate( void );
	void	MenuRender( void );

	//�@�I����ʂ�
	void	MoveSelectUpdate( void );
	void	MoveSelectRender( void );

	//�@�I�v�V����
	void	OptionUpdate( void );
	void	OptionRender( void );

	//�@�N���W�b�g
	void	CreditUpdate( void );
	void	CreditRender( void );
};

//*****************************************************************************
#endif // !__SCENETITLE_H__
