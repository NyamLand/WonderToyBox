
#ifndef __SCENERESULT_H__
#define	__SCENERESULT_H__

//*******************************************************************************
//
//	sceneResult�N���X
//
//*******************************************************************************

class	PlayerManager;

class sceneResult : public	Scene
{
private:
	struct RESULT_INFO
	{
		int		p_Coin;
		int		p_num;
	};

private:
	iexView*	view;
	RESULT_INFO	resultInfo[4];
	
	//	�\�[�g�p
	char	str[256];

	//	�p�����[�^
	int		coinNum[4];
	int		lastBonus;
	int		step;
	int		playerNum;
	int		wait;
	iex2DObj*	back; //�w�i
	PlayerManager	chara[4]; //�L�������

public:
	//	�������E���
	sceneResult( void );
	~sceneResult( void );
	bool	Initialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	����֐�
	void	BubbleSort( void );
	void	SetLastBonus( void );
};

//*******************************************************************************
#endif // !__SCENERESULT_H__
