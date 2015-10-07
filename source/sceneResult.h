
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
	RESULT_INFO	resultInfo[4];
	
	//	�\�[�g�p
	char	str[256];

	//	�p�����[�^
	int		coinNum[4];
	int		lastBonus;
	int		step;
	int		playerNum;
	int		wait;
	int		ten[4];	//�R�C���񌅖�
	int		one[4];	//�R�C���ꌅ��
	int		resultcount;	//	���o�p�J�E���g
	iex2DObj*	back; //�w�i
	iex2DObj*	r_number; //�R�C���̖���
	iexMesh*	collision;

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
	//	���o�p�֐�
	void	Production( void );	
	void	Production_Rotation( int start );
	void	Production_Coin_hand_off(int chara);
};
//*******************************************************************************
#endif // !__SCENERESULT_H__
