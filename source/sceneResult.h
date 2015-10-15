
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

	struct NUMBER
	{
		int		hundred;
		int		ten;
		int		one;
	};


private:
	RESULT_INFO	resultInfo[4];
	
	//	�\�[�g�p
	char	str[256];

	//	�p�����[�^
	int		coinNum[4];
	int		rank[4];
	int		lastBonus;
	int		addcoinNum[4];
	int		step;
	int		playerNum;
	int		wait;
	int		hundred[4];		//�R�C���O����
	int		ten[4];			//�R�C���񌅖�
	int		one[4];			//�R�C���ꌅ��
	int		resultcount;	//	���o�p�J�E���g
	int		Mode;			//	���[�h�����p�ϐ�
	bool	Modeflg;		//���[�h�p�t���O
	float	Sx,Sy;				//�Z���N�g��ʃ|�W�V����
	iex2DObj*	back;		//�w�i
	iex2DObj*	r_number;	//�R�C���̖���
	iex2DObj*	Sback;		//�Z���N�g���̔w�i
	iex2DObj*	Smenu;		//�Z���N�g���̃��j���[
	iexMesh*	collision;
	NUMBER	number[4];

public:
	//	�������E���
	sceneResult( void );
	~sceneResult( void );
	bool	Initialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	void	ResultUpdate( void );
	void	SelectUpdata( void );

	void	SelectRender( void );
	//	����֐�
	void	BubbleSort( void );
	void	SetLastBonus( void );
	void	SetRank( void );

	//	���o�p�֐�
	void	Production( void );						//���U���g�̉��o�p�֐�
	void	ProductionRotation( int playerNum );	//�R�C��������]�֐�
	void	ProductionCoinHandOff(int playerNum);	//�R�C�����������n��
};
//*******************************************************************************
#endif // !__SCENERESULT_H__
