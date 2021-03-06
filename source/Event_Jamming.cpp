#include	"../IEX/iextreme.h"
#include	"GlobalFunction.h"
#include	"Random.h"
#include	"Camera.h"
#include	"CoinManager.h"
#include	"CharacterManager.h"
#include	"GameManager.h"

#include	"Event_Jamming.h"




//**************************************************************
//　初期化・解放
//**************************************************************
bool	Event_Jamming::Initialize()
{
	//	画面傾きイベント初期化
	{
		m_Slope.step = 0;
		m_Slope.param = 0.0f;
		m_Slope.slope = 0.0f;
		m_Slope.eventFlag = false;
	}
	
	//	コイン逃走イベント初期化
	{
		m_CoinGetAway.step = 0;
		m_CoinGetAway.eventflag = false;
	}
	
	//　スリップイベント初期化
	{
		m_Slip.eventflag = 0;
		m_Slip.step = 0;
	}

	return true;
}


void	Event_Jamming::Release(void)
{

}

//**************************************************************
//	更新・描画
//**************************************************************
void	Event_Jamming::Update(void)
{
	//	カメラ傾き
	if (m_Slope.eventFlag)
	{
		eventManager->SetEventFlag(m_Slope.eventFlag);
		Slope();
	}

	//	コイン逃走
	if (m_CoinGetAway.eventflag)
	{
		eventManager->SetEventFlag(m_CoinGetAway.eventflag);
		CoinGetAway();
	}

	//	スリップ
	if (m_Slip.eventflag)
	{
		eventManager->SetEventFlag(m_Slip.eventflag);
		Slip();
	}
}

void	Event_Jamming::Render(void)
{

}

//**************************************************************
//	動作関数
//**************************************************************
void	Event_Jamming::Slope(void)
{
	//	パラメータ準備
	bool	isEnd;
	static	float	slopeAngle = 0.0f;

	switch (m_Slope.step)
	{
	case 0:
		//	初期化
		slopeAngle = Random::GetFloat(D3DX_PI / 180.0f * -90.0f, D3DX_PI / 180.0f * 90.0f);
		m_Slope.step++;
		break;

	case 1:
		//	パラメータ加算
		m_Slope.param += 0.01f;

		//	パラメータ限度設定
		if (m_Slope.param >= 1.0f)
		{
			m_Slope.param = 1.0f;
		}

		//	画面を４５度傾ける
		isEnd = CubicFunctionInterpolation(m_Slope.slope, 0.0f, slopeAngle, m_Slope.param);

		//	カメラへ設定
		mainView->SetSlope(m_Slope.slope);

		//	傾き終わると次のステップへ
		if (isEnd)
		{
			m_Slope.step++;
		}
		break;

	case 2:
		//	数秒待つ

		//count++;
		if (count >= SLOPE_COUNT_MAX)
		{
			m_Slope.param = 0.0f;
			m_Slope.step++;
		}
		break;

	case 3:
		//	パラメータ加算
		m_Slope.param += 0.01f;

		//	パラメータ限度設定
		if (m_Slope.param >= 1.0f)
		{
			m_Slope.param = 1.0f;
		}

		//	画面を傾ける
		isEnd = CubicFunctionInterpolation(m_Slope.slope, slopeAngle, 0.0f, m_Slope.param);

		//	カメラへ設定
		mainView->SetSlope(m_Slope.slope);

		//	傾き終わると次のステップへ
		if (isEnd)
		{
			count = 0;
			state = EVENT_STATE::INTRO;
			this->eventMode = EVENT_MODE::NONE;
			m_Slope.eventFlag = false;
			m_Slope.slope = 0.0f;
			m_Slope.param = 0.0f;
			m_Slope.step = 0;
		}
		break;
	}

	if (count >= FLYING_COUNT)	state = EVENT_STATE::ACTIVE;
	if (count >= SLOPE_COUNT_MAX - FLYING_COUNT)	state = EVENT_STATE::END;
	count++;
}

void	Event_Jamming::CoinGetAway(void)
{
	switch (m_CoinGetAway.step)
	{
	case 0:
		//	コインの動作切り替え
		coinManager->SetCoinGetAwayFlag(true);
		m_CoinGetAway.step++;
		break;

	case 1:
		if (count >= COIN_GETAWAY_COUNT_MAX)
		{
			coinManager->SetCoinGetAwayFlag(false);
		}
		break;
	}

	CommonSetting(m_CoinGetAway.eventflag, m_CoinGetAway.step, COIN_GETAWAY_COUNT_MAX);
}

void	Event_Jamming::Slip(void)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		characterManager->SetParameterInfo(i, PARAMETER_STATE::SLIP);
	}

	CommonSetting(m_Slip.eventflag, m_Slip.step, SLIP_COUNT_MAX);
}




//**************************************************************
//	情報取得・設定
//**************************************************************
Event_Jamming*	Event_Jamming::GetInstance(void)
{
	static	Event_Jamming	out;
	return	&out;
}

//int		Event_Jamming::GetEvent(void)const
//{
//	return	eventflag;
//}

void	Event_Jamming::SetEvent(int eventflag)
{
	switch (eventflag)
	{
	case EVENT_MODE::JAM_SLOPE_CAMERA:
		m_Slope.eventFlag = true;
		break;

	case EVENT_MODE::JAM_SLIP:
		m_Slip.eventflag = true;
		break;

	//case EVENT_MODE::JAM_FALL_BOMB:
	//	break;
	//
	//case EVENT_MODE::JAM_UFO:
	//	break;

	case EVENT_MODE::JAM_COIN_GETAWAY:
		m_CoinGetAway.eventflag = true;
		break;
	}
}