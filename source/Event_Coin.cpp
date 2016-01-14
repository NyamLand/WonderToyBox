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
		Fall();
		eventManager->SetEventFlag(m_Fall.eventflag);
	}

	//　コインの隊列
	if (m_Wave.eventflag)
	{
		Wave();
		eventManager->SetEventFlag(m_Wave.eventflag);
	}
	
	//　コイン２倍タイム
	if (m_Dubble.eventflag)
	{
		Dubble();
		eventManager->SetEventFlag(m_Dubble.eventflag);
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

}

//　滝
void	Event_Coin::Fall(void)
{

	count++;
	
	static	Vector3	vec = Vector3(0.0f, -1.0f, 0.0f);
	static	float	power = 1.0f;

	if (count % SECOND == 0)
	{
		for (int i = 0; i < 30; i++)
		{
			Vector3	pos = Vector3(Random::GetFloat(-20.0f, 20.0f), 50.0f, Random::GetFloat(-20.0f, 12.0f));
			coinManager->Append(pos, vec, power);
		}
	}

	if (count >= FLYING_COUNT)	state = EVENT_STATE::ACTIVE;
	if (count >= FALL_COUNT_MAX - FLYING_COUNT)	state = EVENT_STATE::END;

	if (count >= FALL_COUNT_MAX)
	{
		m_Fall.eventflag = false;
		m_Fall.step = 0;

		count = 0;
		state = EVENT_STATE::INTRO;
	}
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

}

//　コイン２倍タイム
void	Event_Coin::Dubble(void)
{
	count++;

	if (count >= FLYING_COUNT)	state = EVENT_STATE::ACTIVE;
	if (count >= DUBBLE_COUNT_MAX - FLYING_COUNT)	state = EVENT_STATE::END;

	if (count >= DUBBLE_COUNT_MAX)
	{
		m_Dubble.eventflag = false;
		m_Dubble.step = 0;

		count = 0;
		state = EVENT_STATE::INTRO;
	}
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
	case EVENT_MODE::COIN_SACK:
		m_Sack.eventflag = true;
		break;

	case EVENT_MODE::COIN_FALL:
		m_Fall.eventflag = true;
		break;

	case EVENT_MODE::COIN_SPLASH:
		m_Sack.eventflag = true;
		break;

	case EVENT_MODE::COIN_JUELBOX:
		m_Juelbox.eventflag = true;
		break;

	case EVENT_MODE::COIN_WAVE:
		m_Wave.eventflag = true;
		break;

	case EVENT_MODE::COIN_DUBBLE:
		m_Dubble.eventflag = true;
		break;

	}
}