
#include	"Event_Mission.h"
//*****************************************************************
//	初期化・解放
//*****************************************************************
bool	Event_Mission::Initialize(void)
{
	{
		m_Collect.eventflag = false;
		m_Collect.count = 0;
		m_Collect.step = 0;
	}
	
	{
		m_Stay.eventflag = false;
		m_Stay.count = 0;
		m_Stay.step = 0;
	}

	return true;
}

void	Event_Mission::Release(void)
{

}
//*****************************************************************
//	更新・描画
//*****************************************************************
void	Event_Mission::Update(void)
{
	if (m_Collect.eventflag) Collect();
	if (m_Stay.eventflag) Stay();
}

void	Event_Mission::Render(void)
{

}

//*****************************************************************
//　動作関数
//*****************************************************************
void	Event_Mission::Collect(void)
{
	//　宝石を２０個回収！（風船を全部割れ！）

	/*
		１.ミッション演出＆説明
		２.風船出現
		３.制限時間減り始める
		　　・成功：演出 → ご褒美
		  　・失敗：演出 → 全員のコイン半減
	*/

	//switch (m_Collect.step)
	//{
	//case 0:
	//	//　
	//
	//
	//}
}

void	Event_Mission::Stay(void)
{
	//　エリアにとどまれ！
}

//*****************************************************************
//	情報取得
//*****************************************************************
Event_Mission*	Event_Mission::GetInstance(void)
{
	static	Event_Mission	out;
	return	&out;
}

//*****************************************************************
//	情報設定
//*****************************************************************
void	Event_Mission::SetEvent(int eventflag)
{
	switch (eventflag)
	{
	//case EVENT_MODE::MISSION_COLLECT:
	//	m_Collect.eventflag = true;
	//	break;
	//
	//case EVENT_MODE::MISSION_STAY:
	//	m_Stay.eventflag = true;
	//	break;
	}
}