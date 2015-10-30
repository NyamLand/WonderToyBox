
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"Random.h"
#include	"GameManager.h"
#include	"Collision.h"
#include	"Camera.h"
#include	"CharacterManager.h"
#include	"sceneTitle.h"
#include	"sceneMenu.h"
#include	"sceneMain.h"

#include	"sceneResult.h"

//********************************************************************************
//
//	sceneResult�N���X
//
//********************************************************************************

//----------------------------------------------------------------------------
//	�O���[�o��
//----------------------------------------------------------------------------

namespace
{
	namespace MOVE_MODE
	{
		enum
		{
			RESULT_MODE,
			SELECT_MODE
		};
	}
}

//----------------------------------------------------------------------------
//	�������E���
//----------------------------------------------------------------------------
	
	//	�R���X�g���N�^
	sceneResult::sceneResult( void )
	{
	
	}

	//	�f�X�g���N�^
	sceneResult::~sceneResult( void )
	{
		SafeDelete( mainView );
		SafeDelete( back );
		SafeDelete( Sback );
		SafeDelete( Smenu );
		characterManager->Release();
		Random::Release();
	}

	//	������
	bool	sceneResult::Initialize( void )
	{
		//	�J�����ݒ�
		mainView = new Camera();

		//	�摜������
		back = new iex2DObj( "DATA/Result/result-back.png");
		r_number = new iex2DObj( "DATA/UI/number.png");
		Sback = new iex2DObj("DATA/Result/black.png");
		Smenu = new iex2DObj("DATA/Result/result-cho.png");
		result = new iex2DObj("DATA/UI/menu-head.png");

		//	�R���W����
		collision = new iexMesh( "DATA/BG/CollisionGround.imo" );
		Collision::Initiallize( collision );
		
		//	�ϐ�������
		lastBonus = 0;
		step = 0;
		wait = 0;
		resultcount = 0;
		mode = MOVE_MODE::RESULT_MODE;
		Sy = -720;
		StringPos_Y = 0;
		waitTimer = 0;

		//�t���O�S������
		Modeflg = false;
		bonusflg = false;
		addCoinflg	= false;

		//	����������
		Random::Initialize();
		
		//	���X�g�{�[�i�X�ݒ�
		SetLastBonus();

		for ( int i = 0; i < 4; i++ )
		{
			lastBonusNum[i] = 0;

			//	���l�\���̏�����
			{
				number[i].hundred = 0;
				number[i].ten = 0;
				number[i].one = 0;
				number[i].H_flg = false;
			}

			//	�{�[�i�X���l�\����
			{
				BonusNumber[i].hundred = 0;
				BonusNumber[i].ten = 0;
				BonusNumber[i].one = 0;
				BonusNumber[i].H_flg = false;

			}
			
			//	�����L���O�p�\����
			{
				rank[i].rank = 0;
				rank[i].rankflg = false;
			}
			
			//	���U���g��񏉊���
			coinNum[i] = gameManager->GetCoinNum(i);
			lastBonusNum[i] = 0;
			totalCoinNum[i] = coinNum[i] + lastBonusNum[i];
			resultInfo[i].p_Coin = gameManager->GetCoinNum( i );
			resultInfo[i].p_num = i;
			resultInfo[i].p_addCoin = coinNum[i]+lastBonusNum[i];

			//	�v���C���[������
			int		characterType = gameManager->GetCharacterType( i );
			Vector3	pos = Vector3( 6.0f - ( 4.0f  *  i ), 0.0f, 0.0f );
			characterManager->Initialize( i, characterType, pos, true );
		}

		//	�\�[�g
		BubbleSort();

		//	�����L���O�ݒ�
		SetRank();

		return	true;
	}

//----------------------------------------------------------------------------
//	�X�V�E�`��
//----------------------------------------------------------------------------

	//	�X�V
	void	sceneResult::Update( void ) 
	{
		switch ( mode )
		{
		case MOVE_MODE::RESULT_MODE:
			ResultUpdate();
			break;

		case MOVE_MODE::SELECT_MODE:
			SelectUpdata();
			break;
		}
	}

	//���U���g���̍X�V
	void	sceneResult::ResultUpdate( void )
	{
		//	�J�����X�V
		mainView->Update( VIEW_MODE::RESULT, Vector3( 0.0f, 0.0f, 0.0f ) );

		//	�v���C���[�X�V
		characterManager->Update();
		
		//	���o
		Production();
		if ( Modeflg )mode = MOVE_MODE::SELECT_MODE;
	}

	//�Z���N�g���̍X�V
	void	sceneResult::SelectUpdata( void )
	{
		Sy +=20;
		if ( Sy > 0 ){
			Sy = 0;
		}

		if ( input[0]->Get( KEY_UP ) == 3)		StringPos_Y--;
		if ( input[0]->Get( KEY_DOWN ) == 3 )	StringPos_Y++;
		if ( StringPos_Y >= 3 )	StringPos_Y = 0;
		if ( StringPos_Y < 0 )		StringPos_Y = 2;

		if ( KEY_Get( KEY_SPACE ) == 3 || KEY_Get( KEY_A ) == 3 )
		{
			switch ( StringPos_Y )
			{
			case 0:
				//	�Q�[����񏉊���
				gameManager->RetryInitialize();
				MainFrame->ChangeScene( new sceneMain() );
				return;
				break;

			case 1:
				MainFrame->ChangeScene( new sceneMenu() );
				return;
				break;

			case 2:
				MainFrame->ChangeScene( new sceneTitle() );
				return;
				break;
			}
		}
	}

	//	�`��
	void	sceneResult::Render( void ) 
	{
		mainView->Activate();
		mainView->Clear();

		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		back->Render(0, 0, 1280, 720, 0, 0, 1280, 720);
		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

		characterManager->Render( shader3D, "toon" );

		result->Render(510, -10, 256, 128, 0, 0, 512, 256);	//	���U���g�`��

		for (int i = 0; i < 4; i++){
			Vector3 stringPos;
			Vector3	bonusPos;
			Vector3 rankingPos;
			WorldToClient( characterManager->GetPos(i), stringPos, matView* matProjection );
			WorldToClient( characterManager->GetPos(i), bonusPos, matView* matProjection);
			WorldToClient( characterManager->GetPos(i), rankingPos, matView* matProjection );
			stringPos.y = 100;
			bonusPos.y = 170;
			rankingPos.y = 350;
			
			
			//	�擾�R�C�������̕`��
			{
				ResultRender(number[i], stringPos);
			}

			//	�{�[�i�X�R�C�������̕`��
			{
				if(bonusflg){
					ResultRender(BonusNumber[i], bonusPos);
				}
			}

			//	���ʂ̕`��
			{
				if (rank[i].rankflg){
					r_number->Render((int)rankingPos.x - 40 * 2, (int)rankingPos.y, 128, 64, rank[i].rank * 128, 128, 128, 64);
				}
			}
		//	�f�o�b�O�����`��
		/*DrawString( "[sceneResult]", 50, 50 );
		DrawString( "���؁[���Ń^�C�g����", 300, 400, 0xFFFFFF00 );
		DrawString( GameInfo::LastBonus[lastBonus], 300, 150 );

		for ( int i = 0; i < 4; i++ )
		{
			sprintf_s( str, "%d�� : player%d�@%d��", i + 1, resultInfo[i].p_num + 1, resultInfo[i].p_addCoin );
			DrawString( str, 550, 250 + i * 30, 0xFFFFFFFF );
		}*/
			}
		

		if ( mode == MOVE_MODE::SELECT_MODE ){
			SelectRender();
		}
	}

	//	���U���g�`��
	void	sceneResult::ResultRender(NUMBER_INFO& number, Vector3 Pos)
	{
		if (number.H_flg){
			r_number->Render((int)Pos.x - 40 * 2, (int)Pos.y, 64, 64, number.hundred * 64, 0, 64, 64);	//	�R�C���O����
			r_number->Render((int)Pos.x - 40 * 1, (int)Pos.y, 64, 64, number.ten * 64, 0, 64, 64);		//	�R�C���񌅖�
			r_number->Render((int)Pos.x - 40 * 0, (int)Pos.y, 64, 64, number.one * 64, 0, 64, 64);		//	�R�C���ꌅ��
		}
		else{

			r_number->Render((int)Pos.x - 40 * 2, (int)Pos.y, 64, 64, number.ten * 64, 0, 64, 64);		//	�R�C���񌅖�
			r_number->Render((int)Pos.x - 40 * 1, (int)Pos.y, 64, 64, number.one * 64, 0, 64, 64);		//	�R�C���ꌅ��
		}

	}

	//�Z���N�g��ʕ`��
	void	sceneResult::SelectRender( void )
	{
		Sback->Render(0, Sy, 1280, 720, 0, 0, 64, 64);
		if (Sy >= 0)
		{
			for (int i = 0; i < 3; i++){
				Smenu->Render(400, 200 + i * 128, 512, 128, 0, i * 128, 512, 128);

			}
			Smenu->Render(400, 200 + StringPos_Y * 128, 512, 128, 512, StringPos_Y * 128, 512, 128);
			
			
		}
	}

//----------------------------------------------------------------------------
//	����֐�
//----------------------------------------------------------------------------

	//	�o�u���\�[�g
	void	sceneResult::BubbleSort( void )
	{
		RESULT_INFO		temp;

		for ( int i = 0; i < 4; ++i )
		{
			//	��납�珇�ԂɃ`�F�b�N���Ă���
			for ( int s =3; s > i; --s )
			{
				//	����̗v�f�Ɣ�r
				if ( resultInfo[s].p_addCoin >	resultInfo[s - 1].p_addCoin )
				{
					//	�ꎞ�I�ɑޔ�
					temp = resultInfo[s - 1];

					//	����
					resultInfo[s - 1] = resultInfo[s];

					//	�ޔ����Ă����߂�
					resultInfo[s] = temp;
				}
			}
		}
	}

	//	���X�g�{�[�i�X�ݒ�
	void	sceneResult::SetLastBonus( void )
	{
		if ( Random::PercentageRandom( 0.3f ) )
		{
			//	���X�g�{�[�i�X��ݒ�
			lastBonus = gameManager->GetLastBonus();
		}
		else
		{
			//	�Ⴄ���ʂ��o��܂Ń��[�v
			while ( lastBonus == gameManager->GetLastBonus() )
			{
				lastBonus = rand() % 5;
			}
		}
	}

	//	���U���g���o
	void	sceneResult::Production( void )
	{
		switch ( step )
		{
		case 0://	�R�C�������̉�]
			ProductionRotation( step );
			break;

		case 1://	1P�̃R�C����������Ƃ��̑���]
			ProductionCoinHandOff( number[step - 1],coinNum[step - 1] );
			ProductionRotation( step );
			break;

		case 2://	2P�̃R�C����������Ƃ��̑���]
			ProductionCoinHandOff( number[step - 1], coinNum[step - 1] );
			ProductionRotation( step );
			break;

		case 3://	3P�̃R�C����������Ƃ��̑���]
			ProductionCoinHandOff( number[step - 1], coinNum[step - 1] );
			ProductionRotation( step );
			break;
		
		case 4://	4P�̃R�C�������`��
			ProductionCoinHandOff( number[step - 1], coinNum[step - 1] );
			step++;
			break;

		case 5://�{�[�i�X���o->��ō��
			waitTimer++;
			if (waitTimer > 30){
				waitTimer = 0;
				step++;
			}
			break;

		case 6://�{�[�i�X�\��->��ō��
			for (int i = 0; i < 4; i++)
			{
				ProductionCoinHandOff(BonusNumber[i], lastBonusNum[i]);
			}

			bonusflg = true;


			waitTimer++;
			if (waitTimer > 30){
				waitTimer = 0;
				step++;
			}
			break;

		case 7://	�R�C�����Z�l��`��


			waitTimer++;
			if (waitTimer > 60){
				waitTimer = 0;
				step++;
			}
			break;
		case 8:	//	�r�����珇�ɕ`��
			RankRender(3);
			waitTimer++;
			if (waitTimer > 30){
				waitTimer = 0;
				step++;
			}
			break;
		case 9:
			RankRender(2);
			waitTimer++;
			if (waitTimer > 30){
				waitTimer = 0;
				step++;
			}
			break;
		case 10:
			RankRender(1);
			waitTimer++;
			if (waitTimer > 30){
				waitTimer = 0;
				step++;
			}
			break;
		case 11:
			RankRender(0);
			if (KEY_Get(KEY_A) == 3 || KEY_Get(KEY_SPACE))step++;
			break;
		case 12:
			Modeflg = true;

			break;
		}

	}

	//	���U���g���̉�]���o�֐�
	void	sceneResult::ProductionRotation(int playerNum)
	{
		resultcount++;
		for (int i = playerNum; i < 4; i++){

			number[i].hundred++;
			if (number[i].hundred >= 10)number[i].hundred = 0;
			number[i].ten++;
			if (number[i].ten >= 10)number[i].ten = 0;
			number[i].one++;
			if (number[i].one >= 10)number[i].one = 0;
		}
		if (resultcount > 60){
			resultcount = 0;
			step++;
		}
		if (KEY_Get(KEY_A) == 3){
			resultcount = 0;
			step++;
		}
	}

	//	���U���g�̒l�n��
	void	sceneResult::ProductionCoinHandOff(NUMBER_INFO& number,int coinNum)
	{
		number.hundred = coinNum / 100 % 10;
		if (number.hundred > 0){
			number.H_flg = true;
		}
		number.ten = coinNum / 10 % 10;
		number.one = coinNum % 10;
	}

	//	�����N�ݒ�
	void	sceneResult::SetRank( void )
	{
		for ( int i = 0; i < 4; i++ )
		{
			for ( int n = 0; n < 4; n++ )
			{
				if ( i == resultInfo[n].p_num )
				{
					rank[i].rank = n;
				}
			}
		}
	}

	void	sceneResult::RankRender(int ranking)
	{
		for (int i = 0; i < 4; i++)
		{
			if (rank[i].rank == ranking)
			if (rank[i].rankflg == false){
				rank[i].rankflg = true;
			}
		}
	}