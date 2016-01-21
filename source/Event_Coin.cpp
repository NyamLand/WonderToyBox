#include	"../IEX/iextreme.h"
#include	"GameManager.h"
#include	"Random.h"
#include	"CoinManager.h"
#include	"Coin.h"

#include	"Event_Coin.h"

//**************************************************************
//　初期化・解放
//**************************************************************
bool	Event_Coin::Initialize()
{
	//　コイン袋
	{
		m_Sack.eventflag = false;
		m_Sack.step = 0;
	}

	//　コインの滝
	{
		m_Fall.eventflag = false;
		m_Fall.step = 0;
	}

	//　コインワッシャァア
	{
		m_Splash.eventflag = false;
		m_Splash.step = 0;
	}

	//　宝石箱
	{
		m_Juelbox.eventflag = false;
		m_Juelbox.step = 0;
	}

	//　コインのウェーブ
	{
		m_Wave.eventflag = false;
		m_Wave.step = 0;
	}

	//　コイン２倍
	{
		m_Dubble.eventflag = false;
		m_Dubble.step = 0;
	}

	return true;
}


void	Event_Coin::Release(void)
{

}

//**************************************************************
//	更新・描画
//**************************************************************
void	Event_Coin::Update(void)
{
	//	コインの滝
	if (m_Fall.eventflag)
	{
		eventManager->SetEventFlag(m_Fall.eventflag);
		Fall();
	}

	//　コインの隊列
	if (m_Wave.eventflag)
	{
		eventManager->SetEventFlag(m_Wave.eventflag);
		Wave();
	}
	
	//　コイン２倍タイム
	if (m_Dubble.eventflag)
	{
		eventManager->SetEventFlag(m_Dubble.eventflag);
		Dubble();
	}
}

void	Event_Coin::Render(void)
{

}

//**************************************************************
//	動作関数
//**************************************************************
void	Event_Coin::Sack(void)
{
	//　いろいろセット・リセット
	CommonSetting(m_Sack.eventflag, m_Sack.step, SACK_COUNT_MAX);
}

//　滝
void	Event_Coin::Fall(void)
{
	//　コイン落下
	if (count % SECOND == 0)
	{
		for (int i = 0; i < 50; i++)
		{
			Vector3	pos = Vector3(Random::GetFloat(-20.0f, 20.0f), COIN_APPEAR_HEIGHT, Random::GetFloat(-20.0f, 12.0f));
			static	Vector3	vec = Vector3(0.0f, -1.0f, 0.0f);
			static	float	power = 1.0f;
			coinManager->Append(pos, vec, power);
		}
	}

	CommonSetting(m_Fall.eventflag, m_Fall.step, FALL_COUNT_MAX);
}

void	Event_Coin::Splash(void)
{

}

void	Event_Coin::Juelbox(void)
{

}

//　隊列
void	Event_Coin::Wave(void)
{
	//　いろいろ定義（マクロてきなやつ）
	const float STAGE_X_MAX = 20.0f;
	const float STAGE_Z_MAX = 10.0f;
	const float distZ = 1.5f;
	const float power = 0.2f;
	const Vector3	vec = Vector3(0.0f, -0.1f, 0.0f);
	const int COIN_INVERS_RATE = 5;	//　大きい方が少なくなる
	const int POINT_MAX = 8;
	const Vector3 point[] = {
		Vector3(-STAGE_X_MAX, COIN_APPEAR_HEIGHT, STAGE_Z_MAX),		//　左上
		Vector3( STAGE_X_MAX, COIN_APPEAR_HEIGHT, STAGE_Z_MAX),		//　右上
		Vector3(-STAGE_X_MAX, COIN_APPEAR_HEIGHT, -STAGE_Z_MAX),	//　左下
		Vector3( STAGE_X_MAX, COIN_APPEAR_HEIGHT, -STAGE_Z_MAX),	//　右下
		Vector3(-STAGE_X_MAX, COIN_APPEAR_HEIGHT, STAGE_Z_MAX),		//　左上
		Vector3( STAGE_X_MAX, COIN_APPEAR_HEIGHT, STAGE_Z_MAX),		//　右上
		Vector3( STAGE_X_MAX, COIN_APPEAR_HEIGHT, -STAGE_Z_MAX),	//　右下
		Vector3(-STAGE_X_MAX, COIN_APPEAR_HEIGHT, -STAGE_Z_MAX),	//　左下
		Vector3(-STAGE_X_MAX, COIN_APPEAR_HEIGHT, STAGE_Z_MAX),		//　左上
	};

	//---------------------------------------------
	//　波のように隊列を組んで降ってくる
	//---------------------------------------------
	static  int index = 0;	
	static  Vector3 pos1 = point[index];
	//Vector3 pos2 = pos1;
	//pos2.z += distZ;
	
	//　発生
	if (count % COIN_INVERS_RATE == 0)	 coinManager->Append(pos1, vec, power);

	//　適用
	//switch (count % COIN_INVERS_RATE)
	//{
	//case 0:
	//	coinManager->Append(pos1, vec, power);
	//	break;
	//
	////　二列横隊
	////case 1:
	////	coinManager->Append(pos2, vec, power);
	////	break;
	//
	//default:	break;
	//}

	//　降らす位置更新
	if ((point[index + 1] - pos1).LengthSq() < 1.0f)	index++;
	Vector3 addVec = (index < POINT_MAX) ? (point[index + 1] - pos1) : (point[0] - pos1);
	addVec.Normalize();
	pos1 += addVec;


	//　いろいろセット・リセット
	CommonSetting(m_Wave.eventflag, m_Wave.step, WAVE_COUNT_MAX);
	if (count >= WAVE_COUNT_MAX || index >= POINT_MAX) index = 0;
}

//　コイン２倍タイム
void	Event_Coin::Dubble(void)
{
	if (count == 10)
	{
		for (int i = 0; i < 50; i++)
		{
			Vector3	pos = Vector3(Random::GetFloat(-20.0f, 20.0f), COIN_APPEAR_HEIGHT, Random::GetFloat(-20.0f, 12.0f));
			static	Vector3	vec = Vector3(0.0f, -1.0f, 0.0f);
			static	float	power = 1.0f;
			coinManager->Append(pos, vec, power);
		}
	}

	//　いろいろセット・リセット
	CommonSetting(m_Dubble.eventflag, m_Dubble.step, DUBBLE_COUNT_MAX);
}

//**************************************************************
//	情報取得・設定
//**************************************************************
Event_Coin*	Event_Coin::GetInstance(void)
{
	static	Event_Coin	out;
	return	&out;
}

int		Event_Coin::GetEvent(void)const
{
	return	eventflag;
}

Event_Coin::DUBBLE	Event_Coin::GetDubbleInst(void)const
{
	return m_Dubble;
}

void	Event_Coin::SetEvent(int eventflag)
{
	switch (eventflag)
	{
	//case EVENT_MODE::COIN_SACK:
	//	m_Sack.eventflag = true;
	//	break;

	case EVENT_MODE::COIN_FALL:
		m_Fall.eventflag = true;
		break;

	//case EVENT_MODE::COIN_SPLASH:
	//	m_Sack.eventflag = true;
	//	break;

	//case EVENT_MODE::COIN_JUELBOX:
	//	m_Juelbox.eventflag = true;
	//	break;

	case EVENT_MODE::COIN_WAVE:
		m_Wave.eventflag = true;
		break;

	case EVENT_MODE::COIN_DUBBLE:
		m_Dubble.eventflag = true;
		break;

	}
}
