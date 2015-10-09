
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
		SafeDelete( m_Camera );
		SafeDelete( back );
		characterManager->Release();
		Random::Release();
	}

	//	������
	bool	sceneResult::Initialize( void )
	{
		//	�J�����ݒ�
		m_Camera = new Camera();

		//	�摜������
		back = new iex2DObj( "DATA/Result/back.png");
		r_number = new iex2DObj( "DATA/UI/number.png");

		//	�R���W����
		collision = new iexMesh( "DATA/BG/CollisionGround.imo" );
		Collision::Initiallize( collision );
		
		for ( int i = 0; i < 4; i++ )
		{
			//	���U���g��񏉊���
			coinNum[i] = gameManager->GetCoinNum(i);
			resultInfo[i].p_Coin = gameManager->GetCoinNum( i );
			resultInfo[i].p_num = i;

			//	�v���C���[������
			int		characterType = gameManager->GetCharacterType(i);
			Vector3	pos = Vector3( 6.0f - ( 4.0f  *  i ), 0.0f, 0.0f );
			characterManager->Initialize( i, characterType, pos, true );
		}

		//	�ϐ�������
		lastBonus = 0;
		step = 0;
		playerNum = 0;
		wait = 0;
		for ( int i = 0; i < 4; i++ ){
			hundred[i] = 0;
			ten[i] = 0;
			one[i] = 0;
		}
		resultcount = 0;
		//	����������
		Random::Initialize();

		//	�\�[�g
		BubbleSort();

		//	���X�g�{�[�i�X�ݒ�
		SetLastBonus();

		return	true;
	}

//----------------------------------------------------------------------------
//	�X�V�E�`��
//----------------------------------------------------------------------------

	//	�X�V
	void	sceneResult::Update( void ) 
	{

		//	�J�����X�V
		m_Camera->Update(VIEW_MODE::RESULT, Vector3( 0.0f, 0.0f, 0.0f ) );

		//	�v���C���[�X�V
		characterManager->Update();

		Production();
		
		//	�^�C�g����
		if ( KEY( KEY_SPACE ) == 3 )
		{
			MainFrame->ChangeScene( new sceneTitle() );
			return;
		}
	}

	//	�`��
	void	sceneResult::Render( void ) 
	{
		m_Camera->Activate();
		m_Camera->Clear();

		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		back->Render(0, 0, 1280, 720, 0, 0, 2048, 1024);
		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

		characterManager->Render( shader3D, "toon" );

		//	�f�o�b�O�����`��
		DrawString( "[sceneResult]", 50, 50 );
		DrawString( "���؁[���Ń^�C�g����", 300, 400, 0xFFFFFF00 );
		DrawString( GameInfo::LastBonus[lastBonus], 300, 150 );

		for ( int i = 0; i < 4; i++ )
		{
			sprintf_s( str, "%d�� : player%d�@%d��", i + 1, resultInfo[i].p_num + 1, resultInfo[i].p_Coin );
			DrawString( str, 550, 250 + i * 30, 0xFFFFFFFF );
		}

		for (int i = 0; i < 4; i++){
			Vector3 stringPos;
			WorldToClient( characterManager->GetPos(i), stringPos, matView* matProjection );
			stringPos.y = 100;
			r_number->Render( ( int )stringPos.x - 40 * 2, ( int )stringPos.y, 64, 64, hundred[i] * 64, 0, 64, 64);	//	�R�C���O����
			r_number->Render( ( int )stringPos.x  -40 * 1, ( int )stringPos.y, 64, 64, ten[i] * 64, 0, 64, 64);	//	�R�C���񌅖�
			r_number->Render( ( int )stringPos.x  -40 * 0, ( int )stringPos.y, 64, 64, one[i] * 64, 0, 64, 64);	//	�R�C���ꌅ��

			
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
				if ( resultInfo[s].p_Coin >	resultInfo[s - 1].p_Coin )
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

	void	sceneResult::Production( void )
	{
		int	player = 0;
		int player_num = 0;

		switch (step)
		{
		case 0:
			Production_Rotation( player );
			break;

		case 1:
			player = 1;
			Production_Coin_hand_off(player_num);
			Production_Rotation( player );
			break;

		case 2:
			player=2;
			player_num=1;
			Production_Coin_hand_off(player_num);
			Production_Rotation(player);
			break;

		case 3:
			player=3;
			player_num=2;
			Production_Coin_hand_off(player_num);
			Production_Rotation(player);
			break;
		
		case 4:
			player_num=3;
			Production_Coin_hand_off(player_num);
			break;
		}

	}

	void	sceneResult::Production_Rotation(int playerNum)
	{
		resultcount++;
		for (int i = playerNum; i < 4; i++){

			hundred[i]++;
			if (hundred[i]>=10)hundred[i] = 0;
			ten[i]++;
			if (ten[i] >= 10)ten[i] = 0;
			one[i]++;
			if (one[i] >= 10)one[i] = 0;
		}
		if (resultcount > 60){
			resultcount = 0;
			step++;
		}
		if (KEY_Get(KEY_ENTER) == 3){
			resultcount = 0;
			step++;
		}
	}

	void	sceneResult::Production_Coin_hand_off(int playerNum)
	{
		hundred[playerNum] = coinNum[playerNum] / 100 % 10;
		ten[playerNum] = coinNum[playerNum] / 10 % 10;
		one[playerNum] = coinNum[playerNum] % 10;

	}